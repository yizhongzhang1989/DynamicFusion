/***********************************************************/
/**	
\file
\brief		Kinect for Azure interface
\author		Yizhong Zhang
\date		6/24/2019
*/
/***********************************************************/
#ifndef __KINECT_AZURE_H__
#define __KINECT_AZURE_H__

#pragma warning(disable:4996)

#include <Windows.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <k4a/k4a.h>
#include "DepthSensor.h"
#include "FrameIO.h"
#pragma comment(lib, "k4a.lib")

#define ENABLE_GPU_UNDISTORTOR

#ifdef ENABLE_GPU_UNDISTORTOR
#	include "GPUUndistorter.h"
#	pragma comment(lib, "igD3D.lib")
#endif // ENABLE_GPU_UNDISTORTOR


/**
raw data frame read from device directly
*/
class KinectAzureRawDataFrame {
public:
	//	color image
	unsigned long color_timestamp_usec;
	int color_width, color_height, color_stride;	//	stride in bytes
	std::vector<unsigned char> color_image_bgrx32;	//	each pixel consist 4 unsigned char (bgrx)

	//	depth image
	unsigned long depth_timestamp_usec;
	int depth_width, depth_height, depth_stride;	//	stride in bytes
	std::vector<unsigned char> depth_image_mm16;	//	each pixel consist 2 unsigned char (mm)

	//	ir image
	unsigned long ir_timestamp_usec;
	int ir_width, ir_height, ir_stride;				//	stride in bytes
	std::vector<unsigned char> ir_image_gray16;		//	each pixel consist 2 unsigned char (gray scale)

public:
	void GetRawData(k4a_image_t& color, k4a_image_t& depth, k4a_image_t& ir) {
		CopyImage(color_timestamp_usec, color_width, color_height, color_stride, color_image_bgrx32, color);
		CopyImage(depth_timestamp_usec, depth_width, depth_height, depth_stride, depth_image_mm16, depth);
		CopyImage(ir_timestamp_usec, ir_width, ir_height, ir_stride, ir_image_gray16, ir);
	}

	static int CopyImage(
		unsigned long&				timestemp,
		int&						width, 
		int&						height, 
		int&						stride, 
		std::vector<unsigned char>& image, 
		k4a_image_t&				k4a_image
	) {
		timestemp	= k4a_image_get_timestamp_usec(k4a_image);
		width		= k4a_image_get_width_pixels(k4a_image);
		height		= k4a_image_get_height_pixels(k4a_image);
		stride		= k4a_image_get_stride_bytes(k4a_image);
		image.resize(height * stride);
		if (!image.empty()) {
			char* ptr = (char*)(void*)k4a_image_get_buffer(k4a_image);
			memcpy(&image[0], ptr, image.size());
		}

		return 1;
	}
};

/**
aligned images by projecting bgrx\depth\ir to the same pin hole projection coordinate
*/
class KinectAzureDataFrame {
public:
	double	h_fov;
	int		width, height;

	unsigned long bgrx_timestamp_usec;
	unsigned long depth_timestamp_usec;
	unsigned long ir_timestamp_usec;

	std::vector<int>				bgrx;
	std::vector<unsigned short>		depth;
	std::vector<unsigned short>		ir;

public:
	int DumpDataFrame(
		const char* bgrx_file_name, 
		const char* depth_file_name, 
		const char* ir_file_name
	) {
		int pixel_num = width * height;
		if (!pixel_num)
			return 0;

		std::ofstream file;

		//	dump bgrx
		file.open(bgrx_file_name, std::ofstream::binary);
		if (!file.is_open())
			return 0;
		file.write((char*)&bgrx[0], pixel_num * sizeof(int));
		file.close();

		//	dump depth
		file.open(depth_file_name, std::ofstream::binary);
		if (!file.is_open())
			return 0;
		file.write((char*)&depth[0], pixel_num * sizeof(unsigned short));
		file.close();

		//	dump ir
		file.open(ir_file_name, std::ofstream::binary);
		if (!file.is_open())
			return 0;
		file.write((char*)&ir[0], pixel_num * sizeof(unsigned short));
		file.close();

		return 1;
	}

	int WriteToFileBinary(std::ofstream& dump_file) {
		int pixel_num = width * height;

		dump_file.write((char*)&width, sizeof(int));
		dump_file.write((char*)&height, sizeof(int));

		if (pixel_num) {
			dump_file.write((char*)&bgrx[0], sizeof(int)*pixel_num);
			dump_file.write((char*)&depth[0], sizeof(unsigned short)*pixel_num);
			dump_file.write((char*)&ir[0], sizeof(unsigned short)*pixel_num);
		}

		dump_file.write((char*)&h_fov, sizeof(double));
		dump_file.write((char*)&bgrx_timestamp_usec, sizeof(unsigned long));
		dump_file.write((char*)&depth_timestamp_usec, sizeof(unsigned long));
		dump_file.write((char*)&ir_timestamp_usec, sizeof(unsigned long));

		return 1;
	}

	int ReadFromFileBinary(std::ifstream& from_file) {
		if (!from_file.read((char*)&width, sizeof(int)))	return 0;
		if (!from_file.read((char*)&height, sizeof(int)))	return 0;

		int pixel_num = width * height;
		bgrx.resize(pixel_num);
		depth.resize(pixel_num);
		ir.resize(pixel_num);
		if (pixel_num) {
			if (!from_file.read((char*)&bgrx[0], sizeof(int)*pixel_num))				return 0;
			if (!from_file.read((char*)&depth[0], sizeof(unsigned short)*pixel_num))	return 0;
			if (!from_file.read((char*)&ir[0], sizeof(unsigned short)*pixel_num))		return 0;
		}

		if (!from_file.read((char*)&h_fov, sizeof(double)))							return 0;
		if (!from_file.read((char*)&bgrx_timestamp_usec, sizeof(unsigned long)))	return 0;
		if (!from_file.read((char*)&depth_timestamp_usec, sizeof(unsigned long)))	return 0;
		if (!from_file.read((char*)&ir_timestamp_usec, sizeof(unsigned long)))		return 0;

		return 1;
	}

	int GetThumbnailBGR(std::vector<unsigned char>& bgr, int w, int h) {
		bgr.resize(w*h * 3);

		for (int j = 0; j < h; j++) {
			for (int i = 0; i < w; i++) {
				int idx = (j*w + i) * 3;
				int u = width * i / w;
				int v = height * j / h;
				int pixel_bgrx = bgrx[v*width + u];
				bgr[idx + 0] = *((unsigned char*)&pixel_bgrx + 0);
				bgr[idx + 1] = *((unsigned char*)&pixel_bgrx + 1);
				bgr[idx + 2] = *((unsigned char*)&pixel_bgrx + 2);
			}
		}

		return 1;
	}
};

/**
distortor of raw data frames
*/
class Undistorter {
public:
	void SetupUndistorter(
		float	fov, 
		int		width, 
		int		height, 
		float*	color_lookup_uv_ptr,
		float	rot[],
		float	trans[]
	) {
		h_fov = fov;
		frame_width = width;
		frame_height = height;

		color_lookup_uv.resize(width*height * 2);
		color_lookup_uv.assign(color_lookup_uv_ptr, color_lookup_uv_ptr + color_lookup_uv.size());

		for (int i = 0; i < 9; i++)
			R[i] = rot[i];
		for (int i = 0; i < 3; i++)
			T[i] = trans[i];
	}

	void UndistortColor(
		int*			undistort_bgrx_ptr,
		int				raw_color_width,
		int				raw_color_height,
		int				raw_color_stride,
		const char*		raw_color_bgrx_ptr
	) {
		for (int j = 0; j < frame_height; j++) {
			for (int i = 0; i < frame_width; i++) {
				int idx = j * frame_width + i;
				float u = color_lookup_uv[idx * 2];
				float v = color_lookup_uv[idx * 2 + 1];
				if (u >= 0 && u < raw_color_width - 1 && v >= 0 && v < raw_color_height - 1) {
					int u0 = u;
					int v0 = v;
					int u1 = u0 + 1;
					int v1 = v0 + 1;
					float coef_x = u - u0;
					float coef_y = v - v0;
					int bgrx00 = *(int*)(raw_color_bgrx_ptr + v0 * raw_color_stride + u0 * 4);
					int bgrx01 = *(int*)(raw_color_bgrx_ptr + v0 * raw_color_stride + u1 * 4);
					int bgrx10 = *(int*)(raw_color_bgrx_ptr + v1 * raw_color_stride + u0 * 4);
					int bgrx11 = *(int*)(raw_color_bgrx_ptr + v1 * raw_color_stride + u1 * 4);
					undistort_bgrx_ptr[idx] = InterpBGRX(bgrx00, bgrx01, bgrx10, bgrx11, coef_x, coef_y);
				}
				else
					undistort_bgrx_ptr[idx] = 0;
			}
		}
	}

	void UndistortDepthIR(
		unsigned short*	undistort_depth_ptr,
		unsigned short*	undistort_ir_ptr,
		int				raw_depth_width,
		int				raw_depth_height,
		int				raw_depth_stride,
		int				raw_ir_stride,
		const char*		raw_depth_ptr,
		const char*		raw_ir_ptr,
		const short*	point_cloud_depth_ptr
	) {
		//	draw the point cloud (as mesh, ir as texture) in camera coordinate
		//	the resulting depth map is the desired depth map
		//	the resulting color image is the desired ir image
		memset(undistort_depth_ptr, 0, sizeof(unsigned short)*frame_width*frame_height);
		memset(undistort_ir_ptr, 0, sizeof(unsigned short)*frame_width*frame_height);

		const float DegreesToRadians = 3.14159265359f / 180.0f;
		const float fov = h_fov;
		const float xyScale = tanf(fov * DegreesToRadians * 0.5f) / (frame_width * 0.5f);
		const float inv_xyScale = 1.0 / xyScale;
		const float	half_width = frame_width / 2.0;
		const float	half_height = frame_height / 2.0;

		for (int j = 0; j < raw_depth_height - 1; j++) {
			for (int i = 0; i < raw_depth_width - 1; i++) {
				const short* v[4] = {
					&point_cloud_depth_ptr[(j*raw_depth_width + i) * 3],
					&point_cloud_depth_ptr[(j*raw_depth_width + (i + 1)) * 3],
					&point_cloud_depth_ptr[((j + 1)*raw_depth_width + i) * 3],
					&point_cloud_depth_ptr[((j + 1)*raw_depth_width + (i + 1)) * 3] };
				const short ir[4] = {
					*(unsigned short*)&raw_ir_ptr[j * raw_ir_stride + i * 2],
					*(unsigned short*)&raw_ir_ptr[j * raw_ir_stride + (i + 1) * 2],
					*(unsigned short*)&raw_ir_ptr[(j + 1) * raw_ir_stride + i * 2],
					*(unsigned short*)&raw_ir_ptr[(j + 1) * raw_ir_stride + (i + 1) * 2] };
				int vid[3] = { 0, 1, 2 };	//	top left triangle
				for (int k = 0; k < 2; k++) {
					if (k == 1) {	//	bottom right triangle
						vid[0] = 1;	
						vid[1] = 3;
						vid[2] = 2;
					}

					//	illegal triangle, skip
					if (!v[vid[0]][2] || !v[vid[1]][2] || !v[vid[2]][2])	
						continue;

					float p[3][3] = {
						v[vid[0]][0], v[vid[0]][1], v[vid[0]][2],
						v[vid[1]][0], v[vid[1]][1], v[vid[1]][2],
						v[vid[2]][0], v[vid[2]][1], v[vid[2]][2] };

					//	calculate normal in depth camera coordinate
					float n[3] = {
						(p[1][1] - p[0][1]) * (p[2][2] - p[0][2]) - (p[1][2] - p[0][2]) * (p[2][1] - p[0][1]),
						(p[1][2] - p[0][2]) * (p[2][0] - p[0][0]) - (p[1][0] - p[0][0]) * (p[2][2] - p[0][2]),
						(p[1][0] - p[0][0]) * (p[2][1] - p[0][1]) - (p[1][1] - p[0][1]) * (p[2][0] - p[0][0])
					};
					float n_len = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
					n[0] /= n_len;
					n[1] /= n_len;
					n[2] /= n_len;

					//	exclude triangle that is almost vertical to optical axis
					float r_len = sqrtf(p[0][0] * p[0][0] + p[0][1] * p[0][1] + p[0][2] * p[0][2]);
					float r[3] = { p[0][0] / r_len, p[0][1] / r_len, p[0][2] / r_len };
					unsigned short exclude_triangle_flag = 0x0000;
					if (n[0] * r[0] + n[1] * r[1] + n[2] * r[2] < 0.087f) {
						exclude_triangle_flag = 0x8000;
					}

					//	transform to rgb coordinate
					for (int ii = 0; ii < 3; ii++) {
						float col_x = R[0] * p[ii][0] + R[1] * p[ii][1] + R[2] * p[ii][2] + T[0];
						float col_y = R[3] * p[ii][0] + R[4] * p[ii][1] + R[5] * p[ii][2] + T[1];
						float col_z = R[6] * p[ii][0] + R[7] * p[ii][1] + R[8] * p[ii][2] + T[2];
						p[ii][0] = col_x;
						p[ii][1] = col_y;
						p[ii][2] = col_z;
					}

					//	now p is the 3 vertices of triangle, project it onto the screen
					int u[3], v[3];
					for (int ii = 0; ii < 3; ii++) {
						u[ii] = inv_xyScale * p[ii][0] / p[ii][2] + half_width - 0.5f;
						v[ii] = inv_xyScale * p[ii][1] / p[ii][2] + half_height - 0.5f;
						p[ii][0] /= p[ii][2];
						p[ii][1] /= p[ii][2];
					}

					//	exclude fliped triangle

					//	calculate edge vector of projected triangle, and orthogonalize 2x2 matrix (used to calculate projection coef)
					float r1[2] = { p[1][0] - p[0][0], p[1][1] - p[0][1] };
					float r2[2] = { p[2][0] - p[0][0], p[2][1] - p[0][1] };
					float det = r1[0] * r2[1] - r1[1] * r2[0];
					if (det == 0)	//	degenerate triangle
						continue;
					float tmp = r1[0];
					r1[0] = r2[1];
					r2[1] = tmp;
					r1[0] /= det;
					r2[0] /= -det;
					r1[1] /= -det;
					r2[1] /= det;

					//	calculate bonding box
					std::sort(u, u + 3);
					std::sort(v, v + 3);
					int bb_min_u = u[0] < 0 ? 0 : u[0];
					int bb_max_u = u[2] >= frame_width ? frame_width - 1 : u[2];
					int bb_min_v = v[0] < 0 ? 0 : v[0];
					int bb_max_v = v[2] >= frame_height ? frame_height - 1 : v[2];
					for (int jj = bb_min_v; jj <= bb_max_v; jj++) {
						for (int ii = bb_min_u; ii <= bb_max_u; ii++) {
							float x = (ii + 0.5 - half_width) * xyScale - p[0][0];
							float y = (jj + 0.5 - half_height) * xyScale - p[0][1];

							//	check whether the point is inside triangle
							float coef1 = r1[0] * x + r2[0] * y;
							float coef2 = r1[1] * x + r2[1] * y;
							if (coef1 >= 0 && coef2 >= 0 && coef1 + coef2 <= 1) {
								//	interpolate in triangle
								int idx = jj * frame_width + ii;
								unsigned short old_depth = undistort_depth_ptr[idx] & 0x7FFFF;
								unsigned short new_depth = (1 - coef1 - coef2)*p[0][2] + coef1*p[1][2] + coef2*p[2][2];
								if (!old_depth || new_depth < old_depth) {
									undistort_depth_ptr[idx] = new_depth | exclude_triangle_flag;
									undistort_ir_ptr[idx] = (1 - coef1 - coef2)*ir[vid[0]] + coef1*ir[vid[1]] + coef2*ir[vid[2]];
								}
							}
						}
					}
				}
			}
		}

		int pixel_num = frame_width*frame_height;
		for (int i = 0; i < pixel_num; i++) {
			if (undistort_depth_ptr[i] & 0x8000)
				undistort_depth_ptr[i] = 0;
		}
	}

	void UndistortDepthIR_NoInterp(
		unsigned short*	undistort_depth_ptr,
		unsigned short*	undistort_ir_ptr,
		int				raw_depth_width,
		int				raw_depth_height,
		int				raw_depth_stride,
		int				raw_ir_stride,
		const char*		raw_depth_ptr,
		const char*		raw_ir_ptr,
		const short*	point_cloud_depth_ptr
	) {
		memset(undistort_depth_ptr, 0, sizeof(unsigned short)*frame_width*frame_height);
		memset(undistort_ir_ptr, 0, sizeof(unsigned short)*frame_width*frame_height);

		const float DegreesToRadians = 3.14159265359f / 180.0f;
		const float fov = h_fov;
		const float xyScale = tanf(fov * DegreesToRadians * 0.5f) / (frame_width * 0.5f);
		const float inv_xyScale = 1.0 / xyScale;
		const float	half_width = frame_width / 2.0;
		const float	half_height = frame_height / 2.0;

		for (int j = 0; j < raw_depth_height; j++) {
			for (int i = 0; i < raw_depth_width; i++) {
				unsigned short depth_us = *(unsigned short*)(raw_depth_ptr + j * raw_depth_stride + i * 2);
				unsigned short ir_us = *(unsigned short*)(raw_ir_ptr + j * raw_ir_stride + i * 2);

				float dep_x = point_cloud_depth_ptr[(j*raw_depth_width + i) * 3];
				float dep_y = point_cloud_depth_ptr[(j*raw_depth_width + i) * 3 + 1];
				float dep_z = point_cloud_depth_ptr[(j*raw_depth_width + i) * 3 + 2];

				float offset_dep_x = dep_x;
				float offset_dep_y = dep_y;
				float offset_dep_z = dep_z;

				float col_x = R[0] * offset_dep_x + R[1] * offset_dep_y + R[2] * offset_dep_z + T[0];
				float col_y = R[3] * offset_dep_x + R[4] * offset_dep_y + R[5] * offset_dep_z + T[1];
				float col_z = R[6] * offset_dep_x + R[7] * offset_dep_y + R[8] * offset_dep_z + T[2];

				//	project the point onto camera plane
				int	u = inv_xyScale * col_x / col_z + half_width - 0.5f;
				int	v = inv_xyScale * col_y / col_z + half_height - 0.5f;
				if (u >= 0 && u < frame_width && v >= 0 && v < frame_height) {
					int idx = v * frame_width + u;
					if (!undistort_depth_ptr[idx] || depth_us < undistort_depth_ptr[idx]) {
						undistort_depth_ptr[idx] = depth_us;
						undistort_ir_ptr[idx] = ir_us;
					}
				}
			}
		}
	}

	inline int InterpBGRX(int bgrx00, int bgrx01, int bgrx10, int bgrx11, float coef_x, float coef_y) {
		int bgrx_interp = 0;

		for (int i = 0; i < 4; i++) {
			float sum0 = (bgrx00 & 0xFF) * (1.0 - coef_x) + (bgrx01 & 0xFF) * coef_x;
			float sum1 = (bgrx10 & 0xFF) * (1.0 - coef_x) + (bgrx11 & 0xFF) * coef_x;
			float sum = sum0 * (1.0 - coef_y) + sum1 * coef_y;
			int sum_int = sum < 0 ? 0 : (sum > 255 ? 255 : sum);

			sum_int <<= 8 * i;
			bgrx_interp |= sum_int;

			bgrx00 >>= 8;
			bgrx01 >>= 8;
			bgrx10 >>= 8;
			bgrx11 >>= 8;
		}

		return bgrx_interp;
	}

	void DebugInit() {
		h_fov = 80;
		frame_width = 640;
		frame_height = 480;

		color_lookup_uv.resize(frame_width*frame_height * 2);
		for (int j = 0; j < frame_height; j++) {
			for (int i = 0; i < frame_width; i++) {
				color_lookup_uv[(j*frame_width + i) * 2] = i;
				color_lookup_uv[(j*frame_width + i) * 2 + 1] = j;
			}
		}

		R[0] = 1.0;
		R[1] = 0.0;
		R[2] = 0.0;
		R[3] = 0.0;
		R[4] = 1.0;
		R[5] = 0.0;
		R[6] = 0.0;
		R[7] = 0.0;
		R[8] = 1.0;
		T[0] = 0.0;
		T[1] = 0.0;
		T[2] = 0.0;
	}

public:
	float	h_fov;
	int		frame_width, frame_height;

	std::vector<float>	color_lookup_uv;

	float	R[9], T[3];
};

/**
capture interface of kinect for Azure
*/
class KinectAzure : public DepthSensor {
public:
	KinectAzure() {
		device = NULL;
		transformation = NULL;

		get_raw_data = 0;
	}

	~KinectAzure() {
		CloseDevice();
	}

	//	========== overload functions ==========

	/**
	Init device

	illegal argument will be ignored

	standard argument format:					\n
	"dump_file %s"  dump data directory (char*)	\n
	"from_file %s"	load data directory (char*)	\n
	multiple lines must be seperated by '|'

	extended argument format:					\n
	"camera_projection h_fov=%f w=%d h=%d"		\n	
	"get_raw_data"								\n
	*/
	int InitDevice(const char* format = (char*)0, ...) {
		float h_fov = 80;
		int w = 640, h = 480;
		get_raw_data = 0;

		//	if no arguments, just use default 
		if (!format)
			return InitDevice(h_fov, w, h);

		//	parse the argument
		va_list args;
		va_start(args, format);
		int len = _vscprintf(format, args) + 1;
		char* buffer = new char[len];
		vsprintf(buffer, format, args);
		va_end(args);

		char* arg_str[32];
		int line_count = SeperateCmd(arg_str, 32, buffer);
		for (int i = 0; i < line_count; i++) {
			//	camera_projection
			if (strcmp(arg_str[i * 2], "camera_projection") == 0) {
				float cmd_h_fov;
				int cmd_w, cmd_h;
				//	only use data on read cmd succeed
				if (sscanf(arg_str[i * 2 + 1], "h_fov=%f w=%d h=%d", &cmd_h_fov, &cmd_w, &cmd_h) == 3) {
					h_fov = cmd_h_fov;
					w = cmd_w;
					h = cmd_h;
				}
				else {
					std::cout << "error: KinectAzure::InitDevice, invalid arg format: camera_projection h_fov=%f w=%d h=%d" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
				}
			}
			//	dump_file
			else if (strcmp(arg_str[i * 2], "dump_file") == 0) {
				char* dir = arg_str[i * 2 + 1];
				DWORD ftyp = GetFileAttributesA(dir);
				//	if dump directory doesn't exist
				if (ftyp == INVALID_FILE_ATTRIBUTES) {
					std::cout << "error: KinectAzure::InitDevice, dump_file directory not exist" << std::endl;
					std::cout << "the target directory is: " << arg_str[i * 2 + 1] << std::endl;
					delete[] buffer;
					return 0;
				}
				//	if dump directory exist, check whether file already exist
				if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
					char* first_file = new char[strlen(dir) + 20];
					*first_file = '\0';
					strcat(first_file, dir);
					strcat(first_file, "/0.kaf");
					if (_access(first_file, 0) == 0) {	//	first file already exist
						std::cout << "error: KinectAzure::InitDevice, dump_file directory is not empty" << std::endl;
						delete[] buffer;
						delete[] first_file;
						return 0;
					}
					delete[] first_file;

					dump_file_dir = dir;
					frame_async_dumper.SetIO(2, dir, "kaf");
				}
			}
			//	from_file
			else if (strcmp(arg_str[i * 2], "from_file") == 0) {
				char* dir = arg_str[i * 2 + 1];
				DWORD ftyp = GetFileAttributesA(dir);
				//	if from directory doesn't exist
				if (ftyp == INVALID_FILE_ATTRIBUTES) {
					std::cout << "error: KinectAzure::InitDevice, from_file directory not exist" << std::endl;
					std::cout << "the from directory is: " << arg_str[i * 2 + 1] << std::endl;
					delete[] buffer;
					return 0;
				}
				//	if dump directory exist, check whether file already exist
				if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
					char* first_file = new char[strlen(dir) + 20];
					*first_file = '\0';
					strcat(first_file, dir);
					strcat(first_file, "/0.kaf");
					if (_access(first_file, 0) != 0) {	//	first file does not exist
						std::cout << "error: KinectAzure::InitDevice, from_file directory is empty" << std::endl;
						delete[] buffer;
						delete[] first_file;
						return 0;
					}
					delete[] first_file;

					from_file_dir = dir;
					frame_async_loader.SetIO(1, dir, "kaf");
				}
			}
			else if (strcmp(arg_str[i * 2], "get_raw_data") == 0) {
				get_raw_data = 1;
			}
			else {
				std::cout << "error: KinectAzure::InitDevice, unknown cmd: " << arg_str[i * 2] 
					<< " " << arg_str[i * 2 + 1] << std::endl;
			}
		}

		//	offline read don't support get_raw_data
		if (get_raw_data && !from_file_dir.empty()) {
			std::cout << "error: KinectAzure::InitDevice, offline read don't support get_raw_data" << std::endl;
			return 0;
		}

		int succ = InitDevice(h_fov, w, h);
		delete[] buffer;

		return succ;
	}

	int CloseDevice(const char* format = (char*)0, ...) {
		if (device) {
			k4a_device_close(device);
			device = NULL;
		}
		get_raw_data = 0;
		dump_file_dir.clear();
		from_file_dir.clear();
		return 1;
	}

	/**
	Get data from the sensor

	standard argument format:												\n
	"rgbx8_dim w=%p h=%p"		get dimension of rgbx image (int*, int*)	\n
	"rgbx8_fov h_fov=%p"		get fov of rgbx image (float*)				\n
	"depth16_dim w=%p h=%p"		get dimension of depth image (int*, int*)	\n
	"depth16_fov h_fov=%p"		get fov of depth image (float*)				\n
	"rgbx8 %p"					get rgba image (unsigned char*)				\n
	"depth16 %p"				get depth image (unsigned short*)			\n
	"xyz32f %p"					get point cloud (float*), same dimension as depth	\n
	multiple lines must be seperated by '|'

	extended argument format:					\n
	"ir16_dim w=%p h=%p"		get dimension of ir image (int*, int*)		\n
	"ir16_fov h_fov=%p"			get fov of ir image (float*)				\n
	"ir16 %p"					get ir image (unsigned char*)				\n
	"raw_rgbx8_dim w=%p h=%p"	get dimension of raw rgbx image (int*, int*)\n
	"raw_depth16_dim w=%p h=%p"	get dimension of raw depth image (int*, int*)\n
	"raw_ir16_dim w=%p h=%p"	get dimension of raw ir image (int*, int*)	\n
	"raw_rgbx8 %p"				get raw rgba image (unsigned char*)			\n
	"raw_depth16 %p"			get raw depth image (unsigned char*)		\n
	"raw_ir16 %p"				get raw ir image (unsigned char*)			\n
	*/
	int GetData(const char* format = (char*)0, ...) {
		int succ = GetUndistortData();
		if (!succ)
			return succ;

		if (!dump_file_dir.empty())
			DumpFrameToFile();

		if (!format)
			return 1;

		//	parse the argument
		va_list args;
		va_start(args, format);
		int len = _vscprintf(format, args) + 1;
		char* buffer = new char[len];
		vsprintf(buffer, format, args);
		va_end(args);

		char* arg_str[64];
		int line_count = SeperateCmd(arg_str, 64, buffer);
		for (int i = 0; i < line_count; i++) {
			//	data frame
			if (strcmp(arg_str[i * 2], "rgbx8") == 0) {
				if (!GetDataRgbx8(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "depth16") == 0) {
				if (!GetDataDepth16(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "xyz32f") == 0) {
				if (!GetDataXyz32f(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "ir16") == 0) {
				if (!GetDataIr16(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			//	raw frame
			else if (strcmp(arg_str[i * 2], "raw_rgbx8") == 0) {
				if (!GetDataRawRgbx8(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "raw_depth16") == 0) {
				if (!GetDataRawDepth16(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "raw_ir16") == 0) {
				if (!GetDataRawIr16(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			//	dimension of each data image
			else if (
				strcmp(arg_str[i * 2], "rgbx8_dim") == 0 ||
				strcmp(arg_str[i * 2], "depth16_dim") == 0 ||
				strcmp(arg_str[i * 2], "ir16_dim") == 0)
			{
				if (!GetDataFrameDim(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			//	fov of each data image
			else if (
				strcmp(arg_str[i * 2], "rgbx8_fov") == 0 ||
				strcmp(arg_str[i * 2], "depth16_fov") == 0 ||
				strcmp(arg_str[i * 2], "ir16_fov") == 0)
			{
				if (!GetDataFrameFov(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			//	dimension of raw image
			else if (strcmp(arg_str[i * 2], "raw_rgbx8_dim") == 0) {
				if (!GetDataRawRgbx8Dim(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "raw_depth16_dim") == 0) {
				if (!GetDataRawDepth16Dim(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else if (strcmp(arg_str[i * 2], "raw_ir16_dim") == 0) {
				if (!GetDataRawIr16Dim(arg_str[i * 2], arg_str[i * 2 + 1]))
					break;
			}
			else {
				std::cout << "error: KinectAzure::GetData, unrecognized input cmd" << std::endl;
				std::cout << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
				succ = 0;
				break;
			}
		}

		delete[] buffer;
		return succ;
	}

	/**
	"dump_frame %s"				dump a signle frame
	*/
	int Command(const char* format = (char*)0, ...) {
		//	parse the argument
		va_list args;
		va_start(args, format);
		int len = _vscprintf(format, args) + 1;
		char* buffer = new char[len];
		vsprintf(buffer, format, args);
		va_end(args);

		char* arg_str[32];
		int line_count = SeperateCmd(arg_str, 32, buffer);
		for (int i = 0; i < line_count; i++) {
			if (strcmp(arg_str[i * 2], "dump_frame") == 0) {
				char* dir = arg_str[i * 2 + 1];
				DWORD ftyp = GetFileAttributesA(dir);
				//	if dump directory doesn't exist
				if (ftyp == INVALID_FILE_ATTRIBUTES) {
					std::cout << "error: KinectAzure::Command(dump_frame), directory not exist" << std::endl;
					std::cout << "the target directory is: " << arg_str[i * 2 + 1] << std::endl;
					delete[] buffer;
					continue;
				}
				//	if dump directory exist, check whether file already exist
				if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
					const int w = frame.width, h = frame.height;
					char* bgrx_file_name = new char[strlen(dir) + 50];
					char* depth_file_name = new char[strlen(dir) + 50];
					char* ir_file_name = new char[strlen(dir) + 50];
					sprintf(bgrx_file_name, "%s/bgrx%dx%d_%d.dat", dir, w, h, cmd_dump_frame_id);
					sprintf(depth_file_name, "%s/depth%dx%d_%d.dat", dir, w, h, cmd_dump_frame_id);
					sprintf(ir_file_name, "%s/ir%dx%d_%d.dat", dir, w, h, cmd_dump_frame_id);

					if (frame.DumpDataFrame(bgrx_file_name, depth_file_name, ir_file_name))
						cmd_dump_frame_id++;
					else {
						std::cout << "error: KinectAzure::Command(dump_frame), dump frame failed" << std::endl;
					}

					delete[] bgrx_file_name;
					delete[] depth_file_name;
					delete[] ir_file_name;
				}
			}
			else {
				std::cout << "error: KinectAzure::Command, unrecognized input cmd" << std::endl;
				std::cout << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
			}
		}

		return 1;
	}

protected:
	//	========== device operation functions ==========
	int InitDevice(
		float	h_fov,
		int		frame_width,
		int		frame_height
	) {
		//	if read from file, no need to init device
		if (!from_file_dir.empty()) {
			return 1;
		}

		//	read from device
		if (k4a_device_get_installed_count() == 0) {
			std::cout << "error: KinectAzure::InitDevice, No K4A devices found" << std::endl;
			return 0;
		}
		if (K4A_RESULT_SUCCEEDED != k4a_device_open(K4A_DEVICE_DEFAULT, &device)) {
			std::cout << "error: KinectAzure::InitDevice, Failed to open device" << std::endl;
			return 0;
		}

		k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
		config.synchronized_images_only = true;
		config.depth_delay_off_color_usec = 0;
		config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
		config.color_resolution = K4A_COLOR_RESOLUTION_1536P;
		config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
		config.camera_fps = K4A_FRAMES_PER_SECOND_30;

		if (K4A_RESULT_SUCCEEDED != k4a_device_get_calibration(device, config.depth_mode, config.color_resolution, &calibration)) {
			std::cout << "error: KinectAzure::InitDevice, Failed to get calibration" << std::endl;
			return 0;
		}
		transformation = k4a_transformation_create(&calibration);
		if (K4A_RESULT_SUCCEEDED != k4a_device_start_cameras(device, &config)) {
			std::cout << "error: KinectAzure::InitKinect, Failed to start device" << std::endl;
			return 0;
		}

		SetupUndistortion(h_fov, frame_width, frame_height);

		//	setup parameters
		cmd_dump_frame_id = 0;

		return 1;
	}

	/**
	\return		1: captured a legal frame; -1: frame is not ready; 0: device error
	*/
	int GetUndistortData() {
		//	if read from file
		if (!from_file_dir.empty()) {
			return ReadFrameFromFile();
		}

		//	read from device
		if (!device) {
			std::cout << "error: KinectForAzure::GetData, device not init" << std::endl;
			return 0;
		}

		//	-------- query data from device --------
		k4a_capture_t	capture;
		switch (k4a_device_get_capture(device, &capture, 1000)) {
		case K4A_WAIT_RESULT_SUCCEEDED:
			break;
		case K4A_WAIT_RESULT_TIMEOUT:
			std::cout << "error: KinectForAzure::GetData, Timed out waiting for a capture" << std::endl;
			return -1;
		case K4A_WAIT_RESULT_FAILED:
			std::cout << "error: KinectForAzure::GetData, Failed to read a capture" << std::endl;
			if (device != NULL) {
				k4a_device_close(device);
				return 0;
			}
		}

		k4a_image_t color_image = k4a_capture_get_color_image(capture);
		k4a_image_t depth_image = k4a_capture_get_depth_image(capture);
		k4a_image_t ir_image = k4a_capture_get_ir_image(capture);
		if (depth_image == NULL || color_image == NULL || ir_image == NULL) {
			std::cout << "error: KinectForAzure::GetData, get image failed" << std::endl;
			return -1;
		}

		//	-------- record raw data --------
		if (get_raw_data) {
			raw_frame.GetRawData(color_image, depth_image, ir_image);
		}

		//	-------- create point cloud in depth coordinate --------
		k4a_image_t point_cloud_image;
		if (K4A_RESULT_SUCCEEDED != k4a_image_create(
			K4A_IMAGE_FORMAT_DEPTH16,
			k4a_image_get_width_pixels(depth_image),
			k4a_image_get_height_pixels(depth_image),
			k4a_image_get_width_pixels(depth_image) * 3 * (int)sizeof(int16_t),
			&point_cloud_image)) {
			std::cout << "error: KinectForAzure::GetData, create point cloud image failed" << std::endl;
			return -1;
		}
		if (K4A_RESULT_SUCCEEDED != k4a_transformation_depth_image_to_point_cloud(
			transformation,
			depth_image,
			K4A_CALIBRATION_TYPE_DEPTH,
			point_cloud_image)
			) {
			std::cout << "error: KinectForAzure::GetData, get point cloud failed" << std::endl;
			return -1;
		}

		//	-------- undistort data --------
		frame.h_fov = undistorter.h_fov;
		frame.width = undistorter.frame_width;
		frame.height = undistorter.frame_height;
		frame.bgrx_timestamp_usec = k4a_image_get_timestamp_usec(color_image);
		frame.depth_timestamp_usec = k4a_image_get_timestamp_usec(depth_image);
		frame.ir_timestamp_usec = k4a_image_get_timestamp_usec(ir_image);
		frame.bgrx.clear();
		frame.depth.clear();
		frame.ir.clear();
		frame.bgrx.resize(undistorter.frame_width * undistorter.frame_height, 0);
		frame.depth.resize(undistorter.frame_width * undistorter.frame_height, 0);
		frame.ir.resize(undistorter.frame_width * undistorter.frame_height, 0);

		//	undistort color
		int gpu_undistort_succeed = 0;

#ifdef ENABLE_GPU_UNDISTORTOR
		gpu_undistorter.UndistortColor(
			&frame.bgrx[0],
			k4a_image_get_width_pixels(color_image),
			k4a_image_get_height_pixels(color_image),
			k4a_image_get_stride_bytes(color_image),
			(char*)(void*)k4a_image_get_buffer(color_image)
		);

		gpu_undistorter.UndistortDepthIR(
			&frame.depth[0],
			&frame.ir[0],
			k4a_image_get_width_pixels(depth_image),
			k4a_image_get_height_pixels(depth_image),
			k4a_image_get_stride_bytes(depth_image),
			k4a_image_get_stride_bytes(ir_image),
			(char*)(void*)k4a_image_get_buffer(depth_image),
			(char*)(void*)k4a_image_get_buffer(ir_image),
			(short*)(void*)k4a_image_get_buffer(point_cloud_image)
		);

		gpu_undistort_succeed = 1;
#endif

		if (!gpu_undistort_succeed) {
			undistorter.UndistortColor(
				&frame.bgrx[0],
				k4a_image_get_width_pixels(color_image),
				k4a_image_get_height_pixels(color_image),
				k4a_image_get_stride_bytes(color_image),
				(char*)(void*)k4a_image_get_buffer(color_image)
			);

			undistorter.UndistortDepthIR(
				&frame.depth[0],
				&frame.ir[0],
				k4a_image_get_width_pixels(depth_image),
				k4a_image_get_height_pixels(depth_image),
				k4a_image_get_stride_bytes(depth_image),
				k4a_image_get_stride_bytes(ir_image),
				(char*)(void*)k4a_image_get_buffer(depth_image),
				(char*)(void*)k4a_image_get_buffer(ir_image),
				(short*)(void*)k4a_image_get_buffer(point_cloud_image)
			);
		}

		//	-------- release context --------
		k4a_image_release(point_cloud_image);
		k4a_image_release(color_image);
		k4a_image_release(depth_image);
		k4a_image_release(ir_image);
		k4a_capture_release(capture);

		return 1;
	}

	int CalculatePointCloud(float* xyz_ptr) {
		return CalculatePointCloud(xyz_ptr, frame);
	}

	int CalculatePointCloud(float* xyz_ptr, KinectAzureDataFrame& frame) {
		if (frame.depth.empty())
			return 0;

		const float DegreesToRadians = 3.14159265359f / 180.0f;
		const float fov = frame.h_fov;
		const float xyScale = tanf(fov * DegreesToRadians * 0.5f) / (frame.width * 0.5f);
		const float half_width = frame.width / 2;
		const float half_height = frame.height / 2;

		for (int j = 0; j<frame.height; j++) {
			for (int i = 0; i<frame.width; i++) {
				int idx = j*frame.width + i;
				unsigned short pixel_depth = frame.depth[idx];
				float	depth = pixel_depth * 0.001;	//	unit in meters
				xyz_ptr[idx * 3] = (i + 0.5 - half_width) * xyScale * depth;
				xyz_ptr[idx * 3 + 1] = (j + 0.5 - half_height) * xyScale * depth;
				xyz_ptr[idx * 3 + 2] = depth;		//	in OpenGL coordinate
			}
		}

		return 1;
	}

	void SetupUndistortion(
		float	h_fov = 80,
		int		frame_width = 640,
		int		frame_height = 480
	) {
		//	create lookup table for color image
		std::vector<float>	color_lookup_uv;	//	each pixel of undistorted image, the uv on raw color image
		color_lookup_uv.resize(frame_width * frame_height * 2);

		const float DegreesToRadians = 3.14159265359f / 180.0f;
		const float fov = h_fov;
		const float xyScale = tanf(fov * DegreesToRadians * 0.5f) / (frame_width * 0.5f);
		const float inv_xyScale = 1.0 / xyScale;
		const float half_width = frame_width / 2.0;
		const float half_height = frame_height / 2.0;

		for (int j = 0; j < frame_height; j++) {
			for (int i = 0; i < frame_width; i++) {
				k4a_float3_t undistort_xyz;
				undistort_xyz.xyz.x = (i + 0.5 - half_width) * xyScale;
				undistort_xyz.xyz.y = (j + 0.5 - half_height) * xyScale;
				undistort_xyz.xyz.z = 1.0;

				k4a_float2_t distort_uv;
				int valid;
				k4a_calibration_3d_to_2d(
					&calibration,
					&undistort_xyz,
					K4A_CALIBRATION_TYPE_COLOR,
					K4A_CALIBRATION_TYPE_COLOR,
					&distort_uv,
					&valid);

				int idx = j*frame_width + i;
				color_lookup_uv[idx*2] = distort_uv.xy.x;
				color_lookup_uv[idx*2 + 1] = distort_uv.xy.y;
			}
		}

		//	setup undistorter
		undistorter.SetupUndistorter(
			h_fov,
			frame_width,
			frame_height,
			&color_lookup_uv[0],
			calibration.extrinsics[K4A_CALIBRATION_TYPE_DEPTH][K4A_CALIBRATION_TYPE_COLOR].rotation,
			calibration.extrinsics[K4A_CALIBRATION_TYPE_DEPTH][K4A_CALIBRATION_TYPE_COLOR].translation);

#ifdef ENABLE_GPU_UNDISTORTOR
		gpu_undistorter.SetupUndistorter(
			h_fov,
			frame_width,
			frame_height,
			&color_lookup_uv[0],
			calibration.extrinsics[K4A_CALIBRATION_TYPE_DEPTH][K4A_CALIBRATION_TYPE_COLOR].rotation,
			calibration.extrinsics[K4A_CALIBRATION_TYPE_DEPTH][K4A_CALIBRATION_TYPE_COLOR].translation);
#endif // ENABLE_GPU_UNDISTORTOR

	}

	int DumpFrameToFile() {
		return frame_async_dumper.DumpFrame(frame);
	}

	int ReadFrameFromFile() {
		return frame_async_loader.LoadFrame(frame);
	}

	//	========== GetData functions ==========

	inline int GetDataRgbx8(const char* cmd_str, const char* arg_str) {
		unsigned char *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			int pixel_num = frame.bgrx.size();
			memcpy(cmd_ptr, &frame.bgrx[0], pixel_num * 4);
			for (int i = 0; i < pixel_num; i++) {	//	swap bgrx to rgbx
				unsigned char tmp = cmd_ptr[i * 4 + 0];
				cmd_ptr[i * 4 + 0] = cmd_ptr[i * 4 + 2];
				cmd_ptr[i * 4 + 2] = tmp;
			}
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: rgbx8 %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataDepth16(const char* cmd_str, const char* arg_str) {
		unsigned short *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			memcpy(cmd_ptr, &frame.depth[0], frame.depth.size() * 2);
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: depth16 %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataXyz32f(const char* cmd_str, const char* arg_str) {
		float *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			CalculatePointCloud(cmd_ptr);
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: xyz32f %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataIr16(const char* cmd_str, const char* arg_str) {
		unsigned short *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			memcpy(cmd_ptr, &frame.ir[0], frame.ir.size() * 2);
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: ir16 %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataRawRgbx8(const char* cmd_str, const char* arg_str) {
		if (!get_raw_data || raw_frame.color_image_bgrx32.empty())
			return 0;

		unsigned char *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			//	no padding, copy whole array
			if (!raw_frame.color_stride || raw_frame.color_stride == raw_frame.color_width * 4) {
				memcpy(
					cmd_ptr, 
					&raw_frame.color_image_bgrx32[0], 
					raw_frame.color_width * raw_frame.color_height * 4);
			}
			//	with padding, copy line by line
			else {
				for (int j = 0; j < raw_frame.color_height; j++) {
					memcpy(
						cmd_ptr + j*raw_frame.color_width * 4,
						&raw_frame.color_image_bgrx32[j*raw_frame.color_stride],
						raw_frame.color_width * 4);
				}
			}
			//	swap bgrx to rgbx
			int pixel_num = raw_frame.color_width * raw_frame.color_height;
			for (int i = 0; i < pixel_num; i++) {	
				unsigned char tmp = cmd_ptr[i * 4 + 0];
				cmd_ptr[i * 4 + 0] = cmd_ptr[i * 4 + 2];
				cmd_ptr[i * 4 + 2] = tmp;
			}
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: raw_rgbx8 %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataRawDepth16(const char* cmd_str, const char* arg_str) {
		if (!get_raw_data || raw_frame.depth_image_mm16.empty())
			return 0;

		unsigned char *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			//	no padding, copy whole array
			if (!raw_frame.depth_stride || raw_frame.depth_stride == raw_frame.depth_width * 2) {
				memcpy(
					cmd_ptr,
					&raw_frame.depth_image_mm16[0],
					raw_frame.depth_width * raw_frame.depth_height * 2);
			}
			//	with padding, copy line by line
			else {
				for (int j = 0; j < raw_frame.depth_height; j++) {
					memcpy(
						cmd_ptr + j*raw_frame.depth_width * 2,
						&raw_frame.depth_image_mm16[j*raw_frame.depth_stride],
						raw_frame.depth_width * 2);
				}
			}
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: raw_depth16 %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataRawIr16(const char* cmd_str, const char* arg_str) {
		if (!get_raw_data || raw_frame.ir_image_gray16.empty())
			return 0;

		unsigned char *cmd_ptr;
		if (sscanf(arg_str, "%p", &cmd_ptr) == 1) {
			//	no padding, copy whole array
			if (!raw_frame.ir_stride || raw_frame.ir_stride == raw_frame.ir_width * 2) {
				memcpy(
					cmd_ptr,
					&raw_frame.ir_image_gray16[0],
					raw_frame.ir_width * raw_frame.ir_height * 2);
			}
			//	with padding, copy line by line
			else {
				for (int j = 0; j < raw_frame.ir_height; j++) {
					memcpy(
						cmd_ptr + j*raw_frame.ir_height * 2,
						&raw_frame.ir_image_gray16[j*raw_frame.ir_stride],
						raw_frame.ir_width * 2);
				}
			}
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: raw_ir16 %p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataFrameDim(const char* cmd_str, const char* arg_str) {
		int *cmd_w, *cmd_h;
		if (sscanf(arg_str, "w=%p h=%p", &cmd_w, &cmd_h) == 2) {
			*cmd_w = frame.width;
			*cmd_h = frame.height;
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: XXXX_dim w=%p h=%p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataFrameFov(const char* cmd_str, const char* arg_str) {
		float *cmd_fov;
		if (sscanf(arg_str, "h_fov=%p", &cmd_fov) == 1) {
			*cmd_fov = frame.h_fov;
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: XXXX_fov h_fov=%p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataRawRgbx8Dim(const char* cmd_str, const char* arg_str) {
		int *cmd_w, *cmd_h;
		if (sscanf(arg_str, "w=%p h=%p", &cmd_w, &cmd_h) == 2) {
			*cmd_w = raw_frame.color_width;
			*cmd_h = raw_frame.color_height;
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: raw_rgbx8_dim w=%p h=%p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataRawDepth16Dim(const char* cmd_str, const char* arg_str) {
		int *cmd_w, *cmd_h;
		if (sscanf(arg_str, "w=%p h=%p", &cmd_w, &cmd_h) == 2) {
			*cmd_w = raw_frame.depth_width;
			*cmd_h = raw_frame.depth_height;
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: raw_depth16_dim w=%p h=%p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

	inline int GetDataRawIr16Dim(const char* cmd_str, const char* arg_str) {
		int *cmd_w, *cmd_h;
		if (sscanf(arg_str, "w=%p h=%p", &cmd_w, &cmd_h) == 2) {
			*cmd_w = raw_frame.ir_width;
			*cmd_h = raw_frame.ir_height;
		}
		else {
			std::cout << "error: KinectAzure::GetData, invalid arg format: raw_ir16_dim w=%p h=%p" << std::endl;
			std::cout << "the input line is: " << cmd_str << ' ' << arg_str << std::endl;
			return 0;
		}
		return 1;
	}

protected:
	//	context
	k4a_device_t			device;
	k4a_transformation_t	transformation;	
	k4a_calibration_t		calibration;

	//	data IO
	int						get_raw_data;
	KinectAzureRawDataFrame	raw_frame;

	KinectAzureDataFrame	frame;
	std::string				dump_file_dir;
	std::string				from_file_dir;

	FrameAsyncIO<KinectAzureDataFrame>	frame_async_loader;
	FrameAsyncIO<KinectAzureDataFrame>	frame_async_dumper;

	int						cmd_dump_frame_id;

	//	undistort
	Undistorter				undistorter;	

#ifdef ENABLE_GPU_UNDISTORTOR
	GPUUndistorter	gpu_undistorter;
#endif // ENABLE_GPU_UNDISTORTOR

};

#endif	//__KINECT_AZURE_H__