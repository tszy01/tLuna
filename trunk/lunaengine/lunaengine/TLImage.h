#ifndef _TLIMAGE_H_
#define _TLIMAGE_H_

namespace TLunaEngine
{
	// image class
	class TLImage
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
		TLImage(UINT width, UINT height, PIXEL_FORMAT format);
		TLImage(const TLImage& right);
		~TLImage();
	public:
		// create from file
		static TLImage* createFromFile(const char* file);
		// write to file
		bool writeToFile(const char* file);
		// get rgba
		// different formats using different parameters
		bool getRGBA(UINT pixelIndex, BYTE* pR, BYTE* pG, BYTE* pB, BYTE* pA);
		// set rgba
		// different formats using different parameters
		bool setRGBA(UINT pixelIndex, BYTE r, BYTE g, BYTE b, BYTE a);
		// get pixel format
		PIXEL_FORMAT getPixelFormat();
		// get image size
		UINT getImageSize(UINT* pWidth,UINT* pHeight);
		// get pixel size
		UINT getPixelSize();
		// clone
		// this will alloc memory
		TLImage* clone();
		// copy to memory
		bool copyToMemory(BYTE** ppBuffer);
		// create from memory
		static TLImage* createFromMemory(const BYTE* pBuffer, UINT width, UINT height, PIXEL_FORMAT format); 
		// get buffer
		const BYTE* getBuffer()
		{
			return mPixelBuffer;
		}
	private:
		// create buffer
		bool createBuffer(UINT width, UINT height, PIXEL_FORMAT format);
		// copy from buffer
		bool copyFromBuffer(const BYTE* pBuffer);
		// get buffer pointer
		BYTE* getBufferPointer(UINT pixelIndex);
	private:
		// pixel format
		PIXEL_FORMAT mPixelFormat;
		// width
		UINT mWidth;
		// height
		UINT mHeight;
		// pixel buffer
		BYTE* mPixelBuffer;
	};
}

#endif