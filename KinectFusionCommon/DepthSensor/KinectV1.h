/***********************************************************/
/**	
\file
\brief		Kinect v1 interface
\author		Yizhong Zhang
\date		7/30/2019
*/
/***********************************************************/
#ifndef __KINECT_V1_H__
#define __KINECT_V1_H__

#pragma warning(disable:4996)

#include <Windows.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <NuiApi.h>
#include "DepthSensor.h"
#include "FrameIO.h"

#pragma comment(lib, "Kinect10.lib")


class KinectV1DataFrame {
public:
	double	h_fov;
	int		width, height;

	std::vector<int>				bgrx;
	std::vector<unsigned short>		depth;

public:
	int WriteToFileBinary(std::ofstream& dump_file) {
		int pixel_num = width * height;

		dump_file.write((char*)&width, sizeof(int));
		dump_file.write((char*)&height, sizeof(int));

		if (pixel_num) {
			dump_file.write((char*)&bgrx[0], sizeof(int)*pixel_num);
			dump_file.write((char*)&depth[0], sizeof(unsigned short)*pixel_num);
		}

		dump_file.write((char*)&h_fov, sizeof(double));

		return 1;
	}

	int ReadFromFileBinary(std::ifstream& from_file) {
		if (!from_file.read((char*)&width, sizeof(int)))	return 0;
		if (!from_file.read((char*)&height, sizeof(int)))	return 0;

		int pixel_num = width * height;
		bgrx.resize(pixel_num);
		depth.resize(pixel_num);
		if (pixel_num) {
			if (!from_file.read((char*)&bgrx[0], sizeof(int)*pixel_num))				return 0;
			if (!from_file.read((char*)&depth[0], sizeof(unsigned short)*pixel_num))	return 0;
		}
		
		if (!from_file.read((char*)&h_fov, sizeof(double)))	return 0;

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
capture interface of kinect v1
*/
class KinectV1 : public DepthSensor {
public:
	KinectV1() {
		pNuiSensor = NULL;
		pNuiMapper = NULL;

		frame_h_fov = NUI_CAMERA_COLOR_NOMINAL_HORIZONTAL_FOV;
		frame_w = 640;
		frame_h = 480;
	}

	~KinectV1() {
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
	*/
	virtual int InitDevice(const char* format = (char*)0, ...) {
		//	if no arguments, just use default 
		if (!format)
			return InitDevice(frame_w, frame_h);

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
			//	dump_file
			if (strcmp(arg_str[i * 2], "dump_file") == 0) {
				if (!SetDumpFile(arg_str[i * 2 + 1])) {
					delete[] buffer;
					return 0;
				}
			}
			//	from_file
			else if (strcmp(arg_str[i * 2], "from_file") == 0) {
				if (!SetFromFile(arg_str[i * 2 + 1])) {
					delete[] buffer;
					return 0;
				}
			}
		}

		int succ = InitDevice(frame_w, frame_h);
		delete[] buffer;

		return succ;
	}

	virtual int CloseDevice(const char* format = (char*)0, ...) {
		if (NULL != pNuiSensor) {
			pNuiSensor->NuiShutdown();
			pNuiSensor->Release();
		}

		CloseHandle(hNextDepthFrameEvent);
		CloseHandle(hNextColorFrameEvent);

		dump_file_dir.clear();
		from_file_dir.clear();
		frame_async_loader.Reset();
		frame_async_dumper.Reset();

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
	*/
	virtual int GetData(const char* format = (char*)0, ...) {
		int succ = GetMappedData();
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

		char* arg_str[32];
		int line_count = SeperateCmd(arg_str, 32, buffer);
		for (int i = 0; i < line_count; i++) {
			//	dimension of each image
			if (strcmp(arg_str[i * 2], "rgbx8_dim") == 0 ||
				strcmp(arg_str[i * 2], "depth16_dim") == 0)
			{
				int *cmd_w, *cmd_h;
				if (sscanf(arg_str[i * 2 + 1], "w=%p h=%p", &cmd_w, &cmd_h) == 2) {
					*cmd_w = frame.width;
					*cmd_h = frame.height;
				}
				else {
					std::cout << "error: KinectV1::GetData, invalid arg format: XXXX_dim w=%p h=%p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	fov of each image
			else if (
				strcmp(arg_str[i * 2], "rgbx8_fov") == 0 ||
				strcmp(arg_str[i * 2], "depth16_fov") == 0)
			{
				float *cmd_fov;
				if (sscanf(arg_str[i * 2 + 1], "h_fov=%p", &cmd_fov) == 1) {
					*cmd_fov = frame.h_fov;
				}
				else {
					std::cout << "error: KinectV1::GetData, invalid arg format: XXXX_fov h_fov=%p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	rgba8
			else if (strcmp(arg_str[i * 2], "rgbx8") == 0) {
				unsigned char *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					int pixel_num = frame.bgrx.size();
					memcpy(cmd_ptr, &frame.bgrx[0], pixel_num * 4);
					for (int i = 0; i < pixel_num; i++) {
						unsigned char tmp = cmd_ptr[i * 4 + 0];
						cmd_ptr[i * 4 + 0] = cmd_ptr[i * 4 + 2];
						cmd_ptr[i * 4 + 2] = tmp;
					}
				}
				else {
					std::cout << "error: KinectV1::GetData, invalid arg format: rgbx8 %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	depth16
			else if (strcmp(arg_str[i * 2], "depth16") == 0) {
				unsigned short *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					memcpy(cmd_ptr, &frame.depth[0], frame.depth.size() * 2);
				}
				else {
					std::cout << "error: KinectV1::GetData, invalid arg format: depth16 %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	xyz32f
			else if (strcmp(arg_str[i * 2], "xyz32f") == 0) {
				float *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					CalculatePointCloud(cmd_ptr);
				}
				else {
					std::cout << "error: KinectV1::GetData, invalid arg format: xyz32f %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			else {
				std::cout << "error: KinectV1::GetData, unrecognized input cmd" << std::endl;
				std::cout << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
				succ = 0;
				break;
			}
		}

		delete[] buffer;
		return succ;
	}

	/**
	"dump_file %s"		dump data directory (char*)	\n
	"reset_dump_file"	stop dump file
	multiple lines must be seperated by '|'
	*/
	virtual int Command(const char* format = (char*)0, ...) {
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
			//	dump_file
			if (strcmp(arg_str[i * 2], "dump_file") == 0) {		
				if (!SetDumpFile(arg_str[i * 2 + 1])) {
					delete[] buffer;
					return 0;
				}
			}
			//	reset dump
			else if (strcmp(arg_str[i * 2], "reset_dump_file") == 0) {
				if (!ResetDumpFile())
					delete[] buffer;
					return 0;
			}
		}

		delete[] buffer;

		return 1;
	}

protected:
	//	========== device operation functions ==========
	int InitDevice(
		int		frame_width,
		int		frame_height
	) {
		if (frame_width != frame_w || frame_height != frame_h) {
			std::cout << "error: KinectV1::InitDevice, only allow 640*480" << std::endl;
			return 0;
		}

		//	if read from file, no need to init device
		if (!from_file_dir.empty()) {
			return 1;
		}

		//	check the number of connected sensor
		int iSensorCount = 0;
		HRESULT hr = NuiGetSensorCount(&iSensorCount);
		if (ErrorCheck(hr, "KinectV1::InitDevice: unable to count connected"))
			return 0;

		//	create kinect by given index
		hr = NuiCreateSensorByIndex(0, &pNuiSensor);
		if (ErrorCheck(hr, "KinectV1::InitDevice: unable to connect to kinect by index"))
			return 0;

		// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if (S_OK != hr) {
			pNuiSensor->Release();
			std::cout << "error: KinectV1::InitDevice: "
				<< "connection invalid, error code: " << hr << std::endl;
			return 0;
		}

		//	initialize parameters 
		int init_flag = NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH;
		hr = pNuiSensor->NuiInitialize(init_flag);
		if (ErrorCheck(hr, "KinectV1::InitDevice: initialization failed"))
			return 0;

		//	create event and streams
		hNextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		hr = pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_DEPTH,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			hNextDepthFrameEvent,
			&pDepthStreamHandle);
		if (ErrorCheck(hr, "KinectV1::InitDevice: open depth frame"))
			return 0;

		hNextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		hr = pNuiSensor->NuiImageStreamOpen(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_640x480,
			0,
			2,
			hNextColorFrameEvent,
			&pColorStreamHandle);
		if (ErrorCheck(hr, "KinectV1::InitDevice: open color frame"))
			return 0;

		//	initialize mapper
		hr = pNuiSensor->NuiGetCoordinateMapper(&pNuiMapper);
		if (ErrorCheck(hr, "KinectV1::InitDevice: setup mapper"))
			return 0;

		return 1;
	}

	/**
	\return		1: captured a legal frame; -1: frame is not ready; 0: device error
	*/
	int GetMappedData() {
		//	if read from file
		if (!from_file_dir.empty()) {
			return ReadFrameFromFile();
		}

		//	read from device
		if (!pNuiSensor) {
			std::cout << "error: KinectV1::GetMappedData, device not init" << std::endl;
			return 0;
		}

		frame.h_fov = frame_h_fov;
		frame.width = frame_w;
		frame.height = frame_h;
		frame.bgrx.resize(frame_w * frame_h);
		frame.depth.resize(frame_w * frame_h);

		static std::vector<NUI_DEPTH_IMAGE_PIXEL>	raw_depth;
		static std::vector<NUI_DEPTH_IMAGE_POINT>	depth_image_point;
		if (raw_depth.empty())
			raw_depth.resize(frame_w*frame_h);
		if (depth_image_point.empty())
			depth_image_point.resize(frame_w * frame_h);

		//	-------- query depth from device --------
		NUI_IMAGE_FRAME imageFrame;
		HRESULT hr = pNuiSensor->NuiImageStreamGetNextFrame(pDepthStreamHandle, 0, &imageFrame);
		if (hr == E_NUI_FRAME_NO_DATA) {	//	new frame not arrived yet, this is not an error
			return -1;
		}
		ErrorCheck(hr, "error: KinectV1::GetMappedData: get next depth frame");

		BOOL nearMode;
		INuiFrameTexture* pTexture;
		hr = pNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(pDepthStreamHandle, &imageFrame, &nearMode, &pTexture);
		if (FAILED(hr)) {
			std::cout << "error: KinectV1::GetMappedData, get depth texture" << std::endl;
			pNuiSensor->NuiImageStreamReleaseFrame(pDepthStreamHandle, &imageFrame);
			return 0;
		}

		NUI_LOCKED_RECT LockedRect;
		hr = pTexture->LockRect(0, &LockedRect, NULL, 0);
		ErrorCheck(hr, "error: KinectV1::GetMappedData, lock rect");
		if (LockedRect.Pitch == 0) {
			std::cout << "error: KinectV1::GetMappedData, depth data error" << std::endl;
			pNuiSensor->NuiImageStreamReleaseFrame(pDepthStreamHandle, &imageFrame);
			return 0;
		}

		memcpy(&raw_depth[0], LockedRect.pBits, LockedRect.size);

		hr = pTexture->UnlockRect(0);
		ErrorCheck(hr, "error: KinectV1::GetMappedData, unlock rect");
		pTexture->Release();
		pNuiSensor->NuiImageStreamReleaseFrame(pDepthStreamHandle, &imageFrame);
		ErrorCheck(hr, "error: KinectV1::GetMappedData, release depth frame");

		//	-------- query color from device --------
		hr = pNuiSensor->NuiImageStreamGetNextFrame(pColorStreamHandle, 0, &imageFrame);
		if (hr == E_NUI_FRAME_NO_DATA) {	//	new frame not arrived yet, this is not an error
			return -1;
		}
		ErrorCheck(hr, "error: KinectV1::GetMappedData: get next bgrx frame");

		hr = imageFrame.pFrameTexture->LockRect(0, &LockedRect, NULL, 0);
		ErrorCheck(hr, "error: KinectV1::GetMappedData: lock rect");

		memcpy(&frame.bgrx[0], LockedRect.pBits, LockedRect.size);

		hr = imageFrame.pFrameTexture->UnlockRect(0);
		ErrorCheck(hr, "error: KinectV1::GetMappedData: unlock rect");
		hr = pNuiSensor->NuiImageStreamReleaseFrame(pColorStreamHandle, &imageFrame);
		ErrorCheck(hr, "error: KinectV1::GetMappedData, release color frame");

		//	mapping
		hr = pNuiMapper->MapColorFrameToDepthFrame(
			NUI_IMAGE_TYPE_COLOR,
			NUI_IMAGE_RESOLUTION_640x480,
			NUI_IMAGE_RESOLUTION_640x480,
			frame_w * frame_h,
			&raw_depth[0],
			frame_w * frame_h,
			&depth_image_point[0]);
		ErrorCheck(hr, "error: KinectV1::GetMappedData, mapper failed");

		//	copy depth 
		for (int i = 0; i < raw_depth.size(); i++) {
			int u = depth_image_point[i].x;
			int v = depth_image_point[i].y;
			if (u >= 0 && u < frame_w && v >= 0 && v < frame_h) {
				int prj_idx = v*frame_w + u;
				frame.depth[i] = raw_depth[prj_idx].depth;
			}
			else
				frame.depth[i] = 0;
		}

		//	flip
		for (int j = 0; j < frame_h; j++) {
			std::reverse(frame.bgrx.begin() + j*frame_w, frame.bgrx.begin() + (j + 1)*frame_w);
		}
		for (int j = 0; j < frame_h; j++) {
			std::reverse(frame.depth.begin() + j*frame_w, frame.depth.begin() + (j + 1)*frame_w);
		}

		return 1;
	}

	int CalculatePointCloud(float* xyz_ptr) {
		return CalculatePointCloud(xyz_ptr, frame);
	}

	int CalculatePointCloud(float* xyz_ptr, KinectV1DataFrame& frame) {
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

	int SetDumpFile(const char* dir) {
		//	check whether dump already set
		if (!dump_file_dir.empty()) {
			std::cout << "error: KinectV1::SetDumpFile, dump_file already set" << std::endl;
			return 0;
		}

		DWORD ftyp = GetFileAttributesA(dir);
		//	if dump directory doesn't exist
		if (ftyp == INVALID_FILE_ATTRIBUTES) {
			std::cout << "error: KinectV1::SetDumpFile, dump_file directory not exist" << std::endl;
			std::cout << "the target directory is: " << dir << std::endl;
			return 0;
		}
		//	if dump directory exist, check whether file already exist
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
			char* first_file = new char[strlen(dir) + 20];
			*first_file = '\0';
			strcat(first_file, dir);
			strcat(first_file, "/0.k1f");
			if (_access(first_file, 0) == 0) {	//	first file already exist
				std::cout << "error: KinectV1::SetDumpFile, dump_file directory is not empty" << std::endl;
				delete[] first_file;
				return 0;
			}
			delete[] first_file;

			dump_file_dir = dir;
			frame_async_dumper.SetIO(2, dir, "k1f");

			return 1;
		}

		return 0;
	}

	int ResetDumpFile() {
		dump_file_dir.clear();
		frame_async_dumper.Reset();

		return 1;
	}

	int SetFromFile(const char* dir) {
		if (!from_file_dir.empty())
			return 0;

		DWORD ftyp = GetFileAttributesA(dir);
		//	if from directory doesn't exist
		if (ftyp == INVALID_FILE_ATTRIBUTES) {
			std::cout << "error: KinectV1::SetFromFile, from_file directory not exist" << std::endl;
			std::cout << "the from directory is: " << dir << std::endl;
			return 0;
		}
		//	if dump directory exist, check whether file already exist
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
			char* first_file = new char[strlen(dir) + 20];
			*first_file = '\0';
			strcat(first_file, dir);
			strcat(first_file, "/0.k1f");
			if (_access(first_file, 0) != 0) {	//	first file does not exist
				std::cout << "error: KinectV1::SetFromFile, from_file directory is empty" << std::endl;
				delete[] first_file;
				return 0;
			}
			delete[] first_file;

			from_file_dir = dir;
			frame_async_loader.SetIO(1, dir, "k1f");

			return 1;
		}

		return 0;
	}

	int DumpFrameToFile() {
		return frame_async_dumper.DumpFrame(frame);
	}

	int ReadFrameFromFile() {
		return frame_async_loader.LoadFrame(frame);
	}

	int ErrorCheck(HRESULT hr, std::string str) {
		if (FAILED(hr)) {
			std::cout << "error: KinectV1, "
				<< str << ", error code: " << std::hex << hr << std::endl;
			return 1;
		}
		return 0;
	}

protected:
	//	context
	INuiSensor*				pNuiSensor;
	INuiCoordinateMapper*	pNuiMapper;
	HANDLE					pColorStreamHandle;
	HANDLE					pDepthStreamHandle;
	HANDLE					hNextColorFrameEvent;
	HANDLE					hNextDepthFrameEvent;

	//	data storage
	KinectV1DataFrame		frame;
	std::string				dump_file_dir;
	std::string				from_file_dir;

	FrameAsyncIO<KinectV1DataFrame>	frame_async_loader;
	FrameAsyncIO<KinectV1DataFrame>	frame_async_dumper;

	//	parameter
	float frame_h_fov;
	int frame_w, frame_h;
};

#endif	//__KINECT_V1_H__