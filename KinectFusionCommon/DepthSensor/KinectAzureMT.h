/***********************************************************/
/**	
\file
\brief		Kinect for Azure (multi thread) interface
\author		Yizhong Zhang
\date		7/18/2019
*/
/***********************************************************/
#ifndef __KINECT_AZURE_MT_H__
#define __KINECT_AZURE_MT_H__

#include <Windows.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <time.h>
#include "KinectAzure.h"

/**
capture interface of kinect for Azure (multi-thread)
*/
class KinectAzureMT : protected KinectAzure {
public:
	KinectAzureMT() {
		h_imu_thread = NULL;
		h_frame_thread = NULL;
		device_status = 0;

		frame_frequency = 0;
		curr_buffer_read_idx = -1;
		buffer_status.resize(10, 0);
		buffer.resize(10);
	}

	~KinectAzureMT() {
		CloseDevice();
	}

	//	========== overload functions ==========

	int InitDevice(char* format = (char*)0, ...) {
		if (!format)
			KinectAzure::InitDevice();
		else {
			//	parse the argument
			va_list args;
			va_start(args, format);
			int len = _vscprintf(format, args) + 1;
			char* buffer = new char[len];
			vsprintf(buffer, format, args);
			va_end(args);

			if (!KinectAzure::InitDevice(buffer)) {
				delete buffer;
				return 0;
			}
			delete[] buffer;
		}

		device_status = 1;

		h_frame_thread = CreateThread(NULL, 0, FrameThread, this, 0, NULL);

		//	if create imu 
		if (!h_frame_thread) {
			std::cout << "error: KinectAzureMT::InitDevice, create Frame thread failed" << std::endl;
			return 0;
		}

		std::cout << "Frame started, thread created" << std::endl;

		return 1;
	}

	int CloseDevice(char* format = (char*)0, ...) {
		if (!format) {
			return KinectAzure::CloseDevice();
		}
		else {
			//	parse the argument
			va_list args;
			va_start(args, format);
			int len = _vscprintf(format, args) + 1;
			char* buffer = new char[len];
			vsprintf(buffer, format, args);
			va_end(args);

			device_status = 0;

			int succ = KinectAzure::CloseDevice(buffer);
			delete[] buffer;
			return succ;
		}
	}

	int GetData(char* format = (char*)0, ...) {
		//	get the frame to read
		int max_frame_id = -1;
		int prev_frame_id = -1;
		unsigned long max_frame_timestamp_usec;
		buffer_mutex.lock();
		for (int i = 0; i < buffer_status.size(); i++) {
			if (buffer_status[i] == 2) {
				buffer_status[i] = 1;
				prev_frame_id = i;
			}
			if (buffer_status[i] == 1) {	//	an OK space, check its timestamp
				if (max_frame_id < 0 || buffer[i].depth_timestamp_usec > max_frame_timestamp_usec) {
					max_frame_id = i;
					max_frame_timestamp_usec = buffer[i].depth_timestamp_usec;
				}
			}
		}
		if (max_frame_id >= 0) {
			buffer_status[max_frame_id] = 2;
			curr_buffer_read_idx = max_frame_id;
		}
		buffer_mutex.unlock();

		//	if no data ready, return 0
		if (curr_buffer_read_idx < 0 || max_frame_id == prev_frame_id)
			return 0;

		if (!format)
			return 1;

		//	parse the argument
		va_list args;
		va_start(args, format);
		int len = _vscprintf(format, args) + 1;
		char* buffer = new char[len];
		vsprintf(buffer, format, args);
		va_end(args);

		int succ = 1;

		char* arg_str[32];
		int line_count = SeperateCmd(arg_str, 32, buffer);
		for (int i = 0; i < line_count; i++) {
			//	dimension of each image
			if (strcmp(arg_str[i * 2], "rgbx8_dim") == 0 ||
				strcmp(arg_str[i * 2], "depth16_dim") == 0 ||
				strcmp(arg_str[i * 2], "ir16_dim") == 0)
			{
				int *cmd_w, *cmd_h;
				if (sscanf(arg_str[i * 2 + 1], "w=%p h=%p", &cmd_w, &cmd_h) == 2) {
					*cmd_w = this->buffer[curr_buffer_read_idx].width;
					*cmd_h = this->buffer[curr_buffer_read_idx].height;
				}
				else {
					std::cout << "error: KinectAzureMT::GetData, invalid arg format: XXXX_dim w=%p h=%p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	rgba8
			else if (strcmp(arg_str[i * 2], "rgbx8") == 0) {
				unsigned char *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					int pixel_num = this->buffer[curr_buffer_read_idx].bgrx.size();
					memcpy(cmd_ptr, &this->buffer[curr_buffer_read_idx].bgrx[0], pixel_num * 4);
					for (int i = 0; i < pixel_num; i++) {
						unsigned char tmp = cmd_ptr[i * 4 + 0];
						cmd_ptr[i * 4 + 0] = cmd_ptr[i * 4 + 2];
						cmd_ptr[i * 4 + 2] = tmp;
					}
				}
				else {
					std::cout << "error: KinectAzureMT::GetData, invalid arg format: rgbx8 %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	depth16
			else if (strcmp(arg_str[i * 2], "depth16") == 0) {
				unsigned short *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					int pixel_num = this->buffer[curr_buffer_read_idx].depth.size();
					memcpy(cmd_ptr, &this->buffer[curr_buffer_read_idx].depth[0], pixel_num * 2);
				}
				else {
					std::cout << "error: KinectAzureMT::GetData, invalid arg format: depth16 %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	xyz32f
			else if (strcmp(arg_str[i * 2], "xyz32f") == 0) {
				float *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					CalculatePointCloud(cmd_ptr, this->buffer[curr_buffer_read_idx]);
				}
				else {
					std::cout << "error: KinectAzureMT::GetData, invalid arg format: xyz32f %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			//	ir16
			else if (strcmp(arg_str[i * 2], "ir16") == 0) {
				unsigned short *cmd_ptr;
				if (sscanf(arg_str[i * 2 + 1], "%p", &cmd_ptr) == 1) {
					int pixel_num = this->buffer[curr_buffer_read_idx].ir.size();
					memcpy(cmd_ptr, &this->buffer[curr_buffer_read_idx].ir[0], pixel_num * 2);
				}
				else {
					std::cout << "error: KinectAzureMT::GetData, invalid arg format: ir16 %p" << std::endl;
					std::cout << "the input line is: " << arg_str[i * 2 + 1] << std::endl;
					succ = 0;
					break;
				}
			}
			else {
				std::cout << "error: KinectAzureMT::GetData, unrecognized input cmd" << std::endl;
				std::cout << arg_str[i * 2] << ' ' << arg_str[i * 2 + 1] << std::endl;
				succ = 0;
				break;
			}
		}

		delete[] buffer;
		return succ;
	}

	//	---------- Frame functions ----------
	float FrameFPS() {
		return frame_frequency;
	}

	//	---------- IMU functions ----------
	int StartIMU() {
		if (!device || K4A_RESULT_FAILED == k4a_device_start_imu(device)) {
			std::cout << "error: KinectForAzureMT::StartIMU, start IMU failed" << std::endl;
			return 0;
		}

		h_imu_thread = CreateThread(NULL, 0, IMUThread, this, 0, NULL);

		//	if create imu 
		if (!h_imu_thread) {
			std::cout << "error: KinectForAzureMT::StartIMU, create IMU thread failed" << std::endl;
			k4a_device_stop_imu(device);
			return 0;
		}

		std::cout << "IMU started, thread created" << std::endl;

		return 1;
	}

	int StopIMU() {
		if (!device) {
			std::cout << "error: KinectForAzureMT::StopIMU, stop IMU failed" << std::endl;
			return 0;
		}

		k4a_device_stop_imu(device);

		std::cout << "IMU stopped" << std::endl;

		return 1;
	}

protected:
	//	---------- Frame functions ----------
	static DWORD WINAPI FrameThread(LPVOID pParam) {
		KinectAzureMT *pthis = (KinectAzureMT*)pParam;
		return pthis->FrameThread();
	}

	DWORD WINAPI FrameThread() {
		clock_t prev_time = clock();
		int fps_counter = 0;

		while (1) {
			if (!device_status)
				break;
			int res = GetUndistortData();
			if (res == -1)
				break;
			else if (res == 0)
				continue;

			//	get the earlist frame in buffer
			int min_frame_id = -1;
			unsigned long min_frame_timestamp_usec;
			buffer_mutex.lock();
			for (int i = 0; i < buffer_status.size(); i++) {
				if (buffer_status[i] == 0) {		//	an empty space, write here
					min_frame_id = i;
					break;
				}
				else if (buffer_status[i] == 1) {	//	an existing space, check its timestamp
					if (min_frame_id < 0 || buffer[i].depth_timestamp_usec < min_frame_timestamp_usec) {
						min_frame_id = i;
						min_frame_timestamp_usec = buffer[i].depth_timestamp_usec;
					}
				}
			}
			if (min_frame_id < 0) {
				std::cout << "error: KinectAzureMT::FrameThread, no legal position: " << std::endl;
				for (int i = 0; i < buffer_status.size(); i++)
					std::cout << buffer_status[i] << ' ';
				std::cout << std::endl;
			}
			buffer_status[min_frame_id] = 3;
			buffer_mutex.unlock();

			//	record this frame
			buffer[min_frame_id] = frame;

			//	this space is OK
			buffer_mutex.lock();
			buffer_status[min_frame_id] = 1;
			buffer_mutex.unlock();

			fps_counter++;
			clock_t curr_time = clock();
			if (curr_time > prev_time + 1000) {
				float elapsed_s = float(curr_time - prev_time) / CLOCKS_PER_SEC;
				frame_frequency = float(fps_counter) / elapsed_s;
				fps_counter = 0;
				prev_time = curr_time;
			}
		}

		std::cout << "Frame thread killed" << std::endl;

		frame_frequency = 0;

		return 1;
	}

	//	---------- IMU functions ----------
	static DWORD WINAPI IMUThread(LPVOID pParam) {
		KinectAzureMT *pthis = (KinectAzureMT*)pParam;
		return pthis->IMUThread();
	}

	DWORD WINAPI IMUThread() {
		while (1) {
			if (!device_status)
				break;

			k4a_imu_sample_t imu_sample;

			k4a_wait_result_t result = k4a_device_get_imu_sample(
				device, &imu_sample, 1);

			if (K4A_WAIT_RESULT_FAILED == result) {
				std::cout << "error: KinectForAzureMT::IMUThread, read IMU failed, terminate the process" << std::endl;
				break;
			}
			else if (K4A_WAIT_RESULT_TIMEOUT == result)
				continue;
		}

		std::cout << "IMU thread killed" << std::endl;

		return 1;
	}

protected:
	HANDLE h_imu_thread;
	HANDLE h_frame_thread;
	int device_status;

	float									frame_frequency;
	int										curr_buffer_read_idx;
	std::mutex								buffer_mutex;
	std::vector<int>						buffer_status;	//	0: no data, 1: OK, 2: reading, 3: writing
	std::vector<KinectAzureDataFrame>		buffer;
};

#endif	//	__KINECT_AZURE_MT_H__