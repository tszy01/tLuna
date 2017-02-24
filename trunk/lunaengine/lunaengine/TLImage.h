#ifndef _TLIMAGE_H_
#define _TLIMAGE_H_

#include "TSCommonTypeDef.h"

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
		Image(TSun::TU32 width, TSun::TU32 height, PIXEL_FORMAT format);
		Image(const Image& right);
		~Image();
	public:
		// create from file
		static Image* createFromFile(const TSun::TCHAR* file);
		// write to file
		TSun::TBOOL writeToFile(const TSun::TCHAR* file);
		// get rgba
		// different formats using different parameters
		TSun::TBOOL getRGBA(TSun::TU32 pixelIndex, TSun::TUByte* pR, TSun::TUByte* pG, TSun::TUByte* pB, TSun::TUByte* pA);
		// set rgba
		// different formats using different parameters
		TSun::TBOOL setRGBA(TSun::TU32 pixelIndex, TSun::TUByte r, TSun::TUByte g, TSun::TUByte b, TSun::TUByte a);
		// get pixel format
		PIXEL_FORMAT getPixelFormat();
		// get image size
		TSun::TU32 getImageSize(TSun::TU32* pWidth,TSun::TU32* pHeight);
		// get pixel size
		TSun::TU32 getPixelSize();
		// clone
		// this will alloc memory
		Image* clone();
		// copy to memory
		TSun::TBOOL copyToMemory(TSun::TUByte** ppBuffer);
		// create from memory
		static Image* createFromMemory(const TSun::TUByte* pBuffer, TSun::TU32 width, TSun::TU32 height, PIXEL_FORMAT format); 
		// get buffer
		const TSun::TUByte* getBuffer()
		{
			return mPixelBuffer;
		}
	private:
		// create buffer
		TSun::TBOOL createBuffer(TSun::TU32 width, TSun::TU32 height, PIXEL_FORMAT format);
		// copy from buffer
		TSun::TBOOL copyFromBuffer(const TSun::TUByte* pBuffer);
		// get buffer pointer
		TSun::TUByte* getBufferPointer(TSun::TU32 pixelIndex);
	private:
		// pixel format
		PIXEL_FORMAT mPixelFormat;
		// width
		TSun::TU32 mWidth;
		// height
		TSun::TU32 mHeight;
		// pixel buffer
		TSun::TUByte* mPixelBuffer;
	};
}

#endif