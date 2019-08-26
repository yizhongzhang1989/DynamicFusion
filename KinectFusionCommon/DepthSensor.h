/***********************************************************/
/**	\file
	\brief		Virtual base of depth sensor
	\author		Yizhong Zhang
	\date		7/6/2019
*/
/***********************************************************/
#ifndef __DEPTH_SENSOR_H__
#define __DEPTH_SENSOR_H__

#include "ArgParser.h"

/**
	The virtual base of depth sensor

	4 universal virtual functions are provided to operate the device and get data.

	For more functions, use the Command() interface
*/
class DepthSensor : public ArgParser {
public:
	/**
	Initialize the device

	standard argument format:					\n
	"dump_file %s"  dump data directory (char*)	\n
	"from_file %s"	load data directory (char*)	\n
	*/
	virtual int InitDevice(const char* format = (char*)0, ...) = 0;

	/**
	Close the device
	*/
	virtual int CloseDevice(const char* format = (char*)0, ...) = 0;

	/**
	Get data from the sensor

	standard argument format:												\n
	"rgbx8_dim w=%p h=%p"		get dimension of rgbx image (int*, int*)	\n
	"rgbx8_fov h_fov=%p"		get fov of rgbx image (float*)				\n
	"depth16_dim w=%p h=%p"		get dimension of depth image (int*, int*)	\n
	"depth16_fov h_fov=%p"		get fov of depth image (float*)				\n
	"rgbx8 %p"					get rgbx image (unsigned char*)				\n
	"depth16 %p"				get depth image (unsigned short*)			\n
	"xyz32f %p"					get point cloud (float*), same demension as depth	\n
	multiple lines must be seperated by '|'
	*/
	virtual int GetData(const char* format = (char*)0, ...) = 0;

	/**
	Other functions
	*/
	virtual int Command(const char* format = (char*)0, ...) {
		return 0;
	}
};


#endif	//	__DEPTH_SENSOR_H__
