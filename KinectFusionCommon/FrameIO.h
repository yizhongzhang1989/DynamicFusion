/***********************************************************/
/**	\file
	\brief		frame i/o interface
	\author		Yizhong Zhang
	\date		8/6/2019
*/
/***********************************************************/
#ifndef __FRAME_IO_H__
#define __FRAME_IO_H__

#include <windows.h>
#include <time.h>
#include <mutex>

/**
	Asynchronous I/O for data frames (as template). 

	Data is dumped or loaded between disk and RAM with a background thread,
	so that when using the data, data is already loaded to RAM

	The template frame must have the following 3 functions:	\n
	int WriteToFileBinary(std::ofstream& dump_file)			\n
	int ReadFromFileBinary(std::ifstream& from_file)		\n
	int GetThumbnailBGR(std::vector<unsigned char>& bgr, int w, int h)	\n

	To use this class, following these steps:	\n
	1, instance a template class FrameAsyncIO<FrameType>, buffer and file size should be set at construction	\n
	2, call SetIO() to set whether this class is used as in or out, and set directory and file extension at the same time	\n
	3, call LoadFrame() or DumpFrame() for data in or out	\n
	4, if you want to switch in and out, call Reset(), then call SetIO() again to switch	\n

	When used for output, a thunbmail bmp image may be created at the same time
*/
template <class T>
class FrameAsyncIO {
protected:

	class Timer{
	public:
		Timer() : status(0), t_start(0), t_end(0){}

		inline void Start(){
			if( status == 0 ){
				t_start = clock();
				status = 1;
			}
		}

		inline float Stop(){
			if( status == 1 ){
				t_end = clock();
				status = 0;
				return float(t_end-t_start) * 1000 / CLOCKS_PER_SEC;
			}
			return 0;
		}

		inline float Elapsed(){
			if( status == 1 )
				return float(clock()-t_start) * 1000 / CLOCKS_PER_SEC;
			else	//	 status == TIMER_STATUS_STOPED
				return float(t_end-t_start) * 1000 / CLOCKS_PER_SEC;
		}

		inline void Restart(){
			Stop();
			Start();
		}

		int		status;		///<	0: stopped, 1: ticking
		clock_t t_start;	///<	ticks when start counting
		clock_t	t_end;		///<	ticks when end counting
	};


public:
	//	size
	const int buffer_size;			///< size of the whole buffer
	const int dump_size;			///< size of each dump file

	//	control and status parameters
	int io_flag;					///< whether this class is used for in or out: 0 not set; 1 in; 2 out
	int io_thread_running;			///< whether the io thread is running
	int io_thread_terminate;		///< whether the io thread is terminated (from running to ternimate)
	HANDLE io_thread_handle;

	//	buffer, implemented as a queue
	std::vector<T>	buffer;
	int				head, tail;
	std::mutex		buffer_mutex;

	//	file
	std::string		file_directory;
	std::string		file_extension;
	int				file_idx;

	//	last frame timer
	Timer				last_frame_timer;
	std::mutex			last_frame_mutex;

	//	thumbnail
	int					dump_thumbnail_flag;
	std::ofstream		thumbnail_file;
	int					thumbnail_count;

public:
	FrameAsyncIO() : buffer_size(200), dump_size(100) {
		io_flag = 0;
		io_thread_running = 0;
		io_thread_terminate = 0;
		io_thread_handle = NULL;

		buffer.resize(buffer_size);
		head = 0;
		tail = 0;

		file_idx = 0;

		dump_thumbnail_flag = 1;
		thumbnail_count = 0;
	}

	~FrameAsyncIO() {
		if (!io_thread_running)		//	io thread is not running
			return;

		//	terminate the io thread
		io_thread_running = 0;
		Timer	timer;
		timer.Start();
		while (!io_thread_terminate) {
			if (timer.Elapsed() > 1000) {	//	timeout, drop the remaining frames
				return;
			}
			Sleep(1);
		}

		int data_num = (tail + buffer.size() - head) % buffer.size();
		if (data_num)
			WriteFrames(head, data_num);
	}

	int SetIO(int i1o2, const char* directory, const char* extension, int thumbnail_flag = 1) {
		if (io_flag) {
			std::cout << "error: FrameAsyncIO::SetIO, already set" << std::endl;
			return 0;
		}
		io_flag = i1o2;

		file_directory = directory;
		file_extension = extension;
		file_idx = 0;

		//	read the first file
		if (io_flag == 1) {
			int read_num = ReadFrames(tail, dump_size);
			if (!read_num)	//	no data
				return 1;

			tail = (tail + read_num) % buffer.size();
		}

		//	create io thread
		io_thread_running = 1;
		io_thread_terminate = 0;
		io_thread_handle = CreateThread(NULL, 0, IOThread, this, 0, NULL);
		if (!io_thread_handle) {	//	create thread failed
			io_thread_running = 0;
			io_thread_terminate = 1;
		}

		dump_thumbnail_flag = thumbnail_flag;

		return 1;
	}

	int LoadFrame(T& frame) {
		if (io_flag == 0) {
			std::cout << "error: FrameAsyncIO::LoadFrame, io not set" << std::endl;
			return 0;
		}
		if (io_flag == 2) {
			std::cout << "error: FrameAsyncIO::LoadFrame, currently used for out, load failed" << std::endl;
			return 0;
		}

		//	check whether data exist
		buffer_mutex.lock();
		int data_num = (tail + buffer.size() - head) % buffer.size();
		buffer_mutex.unlock();
		
		if (data_num == 0)
			return 0;

		//	read the frame
		frame = buffer[head];

		buffer_mutex.lock();
		head = (head + 1) % buffer.size();
		buffer_mutex.unlock();

		return 1;
	}

	int DumpFrame(const T& frame) {
		if (io_flag == 0) {
			std::cout << "error: FrameAsyncIO::DumpFrame, io not set" << std::endl;
			return 0;
		}
		if (io_flag == 1) {
			std::cout << "error: FrameAsyncIO::DumpFrame, currently used for in, dump failed" << std::endl;
			return 0;
		}

		//	wait for an empty position
		Timer	timer;
		timer.Start();
		while (1) {
			buffer_mutex.lock();
			int full_flag = ((tail + 1) % buffer.size() == head);
			buffer_mutex.unlock();

			if (full_flag){
				if (timer.Elapsed() > 10000) {	//	waited for 10s, drop the frame
					std::cout << "error: FrameAsyncIO::DumpFrame, wait time out, drop the frame" << std::endl;
					return 0;
				}
				Sleep(1);
			}
			else
				break;
		}

		//	record this frame
		buffer[tail] = frame;

		buffer_mutex.lock();
		tail = (tail + 1) % buffer.size();
		buffer_mutex.unlock();

		last_frame_mutex.lock();
		last_frame_timer.Restart();
		last_frame_mutex.unlock();

		return 1;
	}

	int Reset() {
		//	ternimate io thread
		io_thread_running = 0;
		Timer	timer;
		timer.Start();
		while (!io_thread_terminate) {
			if (timer.Elapsed() > 5000) {	//	timeout, reset failed
				return 0;
			}
			Sleep(1);
		}

		//	reset all data
		io_flag = 0;
		io_thread_running = 0;
		io_thread_terminate = 0;
		io_thread_handle = NULL;

		buffer.resize(buffer_size);
		head = 0;
		tail = 0;

		file_directory.clear();
		file_extension.clear();
		file_idx = 0;

		if (thumbnail_file.is_open())
			thumbnail_file.close();
		thumbnail_count = 0;

		return 1;
	}

protected:
	//	---------- IO thread ----------
	static DWORD WINAPI IOThread(LPVOID pParam) {
		FrameAsyncIO *pthis = (FrameAsyncIO*)pParam;
		return pthis->IOThread();
	}

	DWORD WINAPI IOThread() {
		while (io_thread_running) {
			if (io_flag == 0) {
				Sleep(1);
				continue;
			}

			if (io_flag == 1) {
				buffer_mutex.lock();
				int data_num = (tail + buffer.size() - head) % buffer.size();
				buffer_mutex.unlock();

				int empty_spaces = buffer.size() - 1 - data_num;
				if (empty_spaces < dump_size) {
					Sleep(1);
					continue;
				}

				//	read frames. read fail means no more data
				int read_count = ReadFrames(tail, empty_spaces);
				if (!read_count)
					break;

				buffer_mutex.lock();
				tail = (tail + read_count) % buffer.size();
				buffer_mutex.unlock();
			}

			if (io_flag == 2) {
				//	count number of data in buffer
				last_frame_mutex.lock();
				float last_frame_interval = last_frame_timer.Elapsed();
				last_frame_mutex.unlock();

				buffer_mutex.lock();
				int data_num = (tail + buffer.size() - head) % buffer.size();
				buffer_mutex.unlock();

				//	if no data, or frames are still comming but not enough, continue
				if (!data_num || (last_frame_interval < 1000 && data_num < dump_size)) {
					Sleep(1);
					continue;
				}

				//	write frames
				int frame_to_dump = data_num < dump_size ? data_num : dump_size;
				if (!WriteFrames(head, frame_to_dump)) {
					Sleep(10);
					continue;
				}

				//	write thumbnail
				if (dump_thumbnail_flag)
					WriteThumbnail(head);

				//	remove in the buffer
				buffer_mutex.lock();
				head = (head + frame_to_dump) % buffer.size();
				buffer_mutex.unlock();
			}
		}

		io_thread_running	= 0;
		io_thread_terminate = 1;

		return 1;
	}

	int WriteFrames(int start_id, int count) {
		std::string dump_file_name = file_directory + "/";
		dump_file_name += std::to_string(file_idx) + "." + file_extension;

		std::ofstream dump_file(dump_file_name.c_str(), std::ofstream::binary);
		if (!dump_file.is_open()) {
			std::cout << "error: FrameAsyncIO::WriteFrames, dump failed: " << dump_file_name << std::endl;
			return 0;
		}

		dump_file.write((char*)&count, sizeof(int));
		for (int i = 0, buf_idx = start_id; i < count; i++) {
			buffer[buf_idx].WriteToFileBinary(dump_file);
			buf_idx = (buf_idx + 1) % buffer.size();
		}

		dump_file.close();

		file_idx++;

		return 1;
	}

	int ReadFrames(int start_id, int empty_space) {
		std::string load_file_name = file_directory + "/";
		load_file_name += std::to_string(file_idx) + "." + file_extension;

		std::ifstream read_file(load_file_name.c_str(), std::ifstream::binary);
		if (!read_file.is_open()) {
			return 0;
		}

		//	read data
		int frame_num;
		if (!read_file.read((char*)&frame_num, sizeof(int)))	return 0;

		int read_count = 0;
		for (int i = 0, buf_idx = start_id; i < frame_num && i < empty_space; i++) {
			int succ = buffer[buf_idx].ReadFromFileBinary(read_file);
			if (!succ)
				break;
			buf_idx = (buf_idx + 1) % buffer.size();
			read_count++;
		}

		read_file.close();

		//	handle exceptions
		if (frame_num > empty_space) {
			std::cout << "error: FrameAsyncIO::ReadFrames, "
				<< frame_num << " frames expected, "
				<< empty_space << " empty spaces" << std::endl;
		}
		if (read_count != frame_num) {
			std::cout << "error: FrameAsyncIO::ReadFrames, "
				<< frame_num << " frames expected, "
				<< read_count << " frames read" << std::endl;
		}

		file_idx++;

		return read_count;
	}

	int WriteThumbnail(int frame_id) {
		const int thumbnail_width = 100;
		const int thumbnail_height = 75;

		std::vector<unsigned char> thumbnail_bgr;
		if (!buffer[frame_id].GetThumbnailBGR(thumbnail_bgr, thumbnail_width, thumbnail_height))
			return 0;

		//	create bmp file header
		char bmp_header[54];
		memset(bmp_header, 0, 54);

		const int thumbnail_dim_x	= 10;
		const int image_width		= thumbnail_dim_x * thumbnail_width;
		const int image_height		= (thumbnail_count + thumbnail_dim_x) / thumbnail_dim_x * thumbnail_height;
		const int image_size_bytes	= image_width * image_height * 3;
		const int file_size_bytes	= sizeof(bmp_header) + image_size_bytes;

		*(short*)&bmp_header[0]		= 0x4D42;				//	signature
		*(int*)&bmp_header[2]		= file_size_bytes;		//	filesize in bytes
		*(int*)&bmp_header[10]		= 54;					//	offset
		*(int*)&bmp_header[14]		= 40;					//	info header size
		*(int*)&bmp_header[18]		= image_width;			//	image width
		*(int*)&bmp_header[22]		= image_height;			//	image height
		*(short*)&bmp_header[26]	= 1;					//	planes, must be 1
		*(short*)&bmp_header[28]	= 24;					//	bits per pixel, 24
		*(int*)&bmp_header[34]		= image_size_bytes;		//	image size bytes

		//	open the file
		if (!thumbnail_file.is_open()) {
			std::string thumbnail_file_name = file_directory + "/thumbnail.bmp";
			thumbnail_file.open(thumbnail_file_name.c_str(), std::ofstream::binary);
		}
		if (!thumbnail_file.is_open()) {	//	cannot open thumbnail file					
			return 0;
		}

		//	write the header
		static std::vector<unsigned char> gray;
		if (gray.empty())
			gray.resize(image_width * 3 * thumbnail_height, 127);
		if (thumbnail_count % thumbnail_dim_x == 0) {
			thumbnail_file.seekp(0, std::ofstream::beg);
			thumbnail_file.write(bmp_header, sizeof(bmp_header));
			thumbnail_file.seekp(0, std::ofstream::end);
			thumbnail_file.write((char*)&gray[0], gray.size());
		}

		//	write the thumbnail image
		int dim_x_idx = thumbnail_count % thumbnail_dim_x;
		int dim_y_idx = thumbnail_count / thumbnail_dim_x;
		thumbnail_file.seekp(
			sizeof(bmp_header) + dim_y_idx*image_width * 3 * thumbnail_height + dim_x_idx * thumbnail_width * 3,
			std::ofstream::beg);
		for (int j = 0; j < thumbnail_height; j++) {
			thumbnail_file.write(
				(char*)&thumbnail_bgr[(thumbnail_height - 1 - j)* thumbnail_width * 3],
				thumbnail_width * 3);
			if (j != thumbnail_height - 1)
				thumbnail_file.seekp((thumbnail_dim_x - 1)*thumbnail_width * 3, std::ofstream::cur);
		}

		thumbnail_count++;

		return 1;
	}
};


#endif	//	__FRAME_IO_H__
