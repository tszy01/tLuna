#ifndef _TLIMAGE_H_
#define _TLIMAGE_H_

#include "TLCommonTypeDef.h"

namespace TLunaEngine
{
	// image class
	class Image
	{
	public:
		// pixel format
		enum PIXEL_FORMAT
		{
			PIXEL_FORMAT_R8 = 1,
			PIXEL_FORMAT_R8G8B8 = 2,
			PIXEL_FORMAT_R8G8B8A8 = 3,
		};
	public:
		// constructor will alloc memory for pixel
		// use create functions for usual
		Image(TU32 width, TU32 height, PIXEL_FORMAT format);
		Image(const Image& right);
		~Image();
	public:
		// create from file
		static Image* createFromFile(const char* file);
		// write to file
		bool writeToFile(const char* file);
		// get rgba
		// different formats using different parameters
		bool getRGBA(TU32 pixelIndex, TUByte* pR, TUByte* pG, TUByte* pB, TUByte* pA);
		// set rgba
		// different formats using different parameters
		bool setRGBA(TU32 pixelIndex, TUByte r, TUByte g, TUByte b, TUByte a);
		// get pixel format
		PIXEL_FORMAT getPixelFormat();
		// get image size
		TU32 getImageSize(TU32* pWidth,TU32* pHeight);
		// get pixel size
		TU32 getPixelSize();
		// clone
		// this will alloc memory
		Image* clone();
		// copy to memory
		bool copyToMemory(TUByte** ppBuffer);
		// create from memory
		static Image* createFromMemory(const TUByte* pBuffer, TU32 width, TU32 height, PIXEL_FORMAT format); 
		// get buffer
		const TUByte* getBuffer()
		{
			return mPixelBuffer;
		}
	private:
		// create buffer
		bool createBuffer(TU32 width, TU32 height, PIXEL_FORMAT format);
		// copy from buffer
		bool copyFromBuffer(const TUByte* pBuffer);
		// get buffer pointer
		TUByte* getBufferPointer(TU32 pixelIndex);
	private:
		// pixel format
		PIXEL_FORMAT mPixelFormat;
		// width
		TU32 mWidth;
		// height
		TU32 mHeight;
		// pixel buffer
		TUByte* mPixelBuffer;
	};
}

#endif