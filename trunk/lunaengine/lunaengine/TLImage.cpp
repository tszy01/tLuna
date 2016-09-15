#include "TLImage.h"
#include "FreeImage.h"
#include <memory.h>

namespace TLunaEngine
{
	Image::Image(TU32 width, TU32 height, PIXEL_FORMAT format) : mPixelBuffer(TNULL),mWidth(0),mHeight(0)
	{
		createBuffer(width,height,format);
	}

	Image::Image(const Image& right) : mPixelBuffer(TNULL),mWidth(0),mHeight(0)
	{
		createBuffer(right.mWidth,right.mHeight,right.mPixelFormat);
		copyFromBuffer(right.mPixelBuffer);
	}

	Image::~Image()
	{
		if(mPixelBuffer)
		{
			delete [] mPixelBuffer;
			mPixelBuffer = TNULL;
		}
	}

	bool Image::createBuffer(TU32 width, TU32 height, PIXEL_FORMAT format)
	{
		if(mPixelBuffer)
			return false;
		if(width==0 || height==0)
			return false;
		switch(format)
		{
		case PIXEL_FORMAT_R8:
			{
				mPixelBuffer = new TUByte[width*height];
				memset(mPixelBuffer,0,sizeof(TUByte)*width*height);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				mPixelBuffer = new TUByte[width*height*3];
				memset(mPixelBuffer,0,sizeof(TUByte)*width*height*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				mPixelBuffer = new TUByte[width*height*4];
				memset(mPixelBuffer,0,sizeof(TUByte)*width*height*4);
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
		mPixelFormat = format;
		mWidth = width;
		mHeight = height;
		return true;
	}

	bool Image::copyFromBuffer(const TUByte* pBuffer)
	{
		if(!pBuffer || !mPixelBuffer)
			return false;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(TUByte)*mWidth*mHeight);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(TUByte)*mWidth*mHeight*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(TUByte)*mWidth*mHeight*4);
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
		return true;
	}

	Image* Image::createFromMemory(const TUByte* pBuffer, TU32 width, TU32 height, Image::PIXEL_FORMAT format)
	{
		if(!pBuffer)
			return TNULL;
		Image* pRet = new Image(width,height,format);
		if(pRet->mPixelBuffer==TNULL)
		{
			delete pRet;
			return TNULL;
		}
		if(!pRet->copyFromBuffer(pBuffer))
		{
			delete pRet;
			return TNULL;
		}
		return pRet;
	}

	bool Image::copyToMemory(TUByte** ppBuffer)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth<=0 || mHeight<=0)
			return false;
		if(!ppBuffer)
			return false;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				*ppBuffer = new TUByte[mWidth*mHeight];
				memcpy(*ppBuffer,mPixelBuffer,sizeof(TUByte)*mWidth*mHeight);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				*ppBuffer = new TUByte[mWidth*mHeight*3];
				memcpy(*ppBuffer,mPixelBuffer,sizeof(TUByte)*mWidth*mHeight*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				*ppBuffer = new TUByte[mWidth*mHeight*4];
				memcpy(*ppBuffer,mPixelBuffer,sizeof(TUByte)*mWidth*mHeight*4);
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
		return true;
	}

	Image* Image::clone()
	{
		if(!mPixelBuffer)
			return TNULL;
		if(mWidth<=0 || mHeight<=0)
			return TNULL;
		Image* pRet = new Image(mWidth,mHeight,mPixelFormat);
		if(pRet->mPixelBuffer==TNULL)
		{
			delete pRet;
			return TNULL;
		}
		if(!pRet->copyFromBuffer(mPixelBuffer))
		{
			delete pRet;
			return TNULL;
		}
		return pRet;
	}

	TU32 Image::getPixelSize()
	{
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				return sizeof(TUByte)*1;
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				return sizeof(TUByte)*3;
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				return sizeof(TUByte)*4;
			}
			break;
		default:
			{
				return 0;
			}
			break;
		}
		return 0;
	}

	TU32 Image::getImageSize(TU32* pWidth,TU32* pHeight)
	{
		if(pWidth)
		{
			*pWidth = mWidth;
		}
		if(pHeight)
		{
			*pHeight = mHeight;
		}
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				return mWidth*mHeight;
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				return mWidth*mHeight*3;
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				return mWidth*mHeight*4;
			}
			break;
		default:
			{
				return 0;
			}
			break;
		}
		return 0;
	}

	Image::PIXEL_FORMAT Image::getPixelFormat()
	{
		return mPixelFormat;
	}

	TUByte* Image::getBufferPointer(TU32 pixelIndex)
	{
		if(!mPixelBuffer)
			return TNULL;
		if(mWidth==0 || mHeight==0)
			return TNULL;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				if(pixelIndex>=mWidth*mHeight)
					return TNULL;
				return &mPixelBuffer[pixelIndex];
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				if(pixelIndex>=mWidth*mHeight*3)
					return TNULL;
				return &mPixelBuffer[pixelIndex*3];
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				if(pixelIndex>=mWidth*mHeight*4)
					return TNULL;
				return &mPixelBuffer[pixelIndex*4];
			}
			break;
		default:
			{
				return TNULL;
			}
			break;
		}
		return TNULL;
	}

	bool Image::setRGBA(TU32 pixelIndex, TUByte r, TUByte g, TUByte b, TUByte a)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth==0 || mHeight==0)
			return false;
		TUByte* pBuffer = getBufferPointer(pixelIndex);
		if(!pBuffer)
			return false;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				*pBuffer = r;
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				*pBuffer = r;
				pBuffer++;
				*pBuffer = g;
				pBuffer++;
				*pBuffer = b;
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				*pBuffer = r;
				pBuffer++;
				*pBuffer = g;
				pBuffer++;
				*pBuffer = b;
				pBuffer++;
				*pBuffer = a;
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
		return true;
	}

	bool Image::getRGBA(TU32 pixelIndex, TUByte* pR, TUByte* pG, TUByte* pB, TUByte* pA)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth==0 || mHeight==0)
			return false;
		TUByte* pBuffer = getBufferPointer(pixelIndex);
		if(!pBuffer)
			return false;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				if(pR)
				{
					*pR = *pBuffer;
				}
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				if(pR && pG && pB)
				{
					*pR = *pBuffer;
					pBuffer++;
					*pG = *pBuffer;
					pBuffer++;
					*pB = *pBuffer;
				}
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				if(pR && pG && pB && pA)
				{
					*pR = *pBuffer;
					pBuffer++;
					*pG = *pBuffer;
					pBuffer++;
					*pB = *pBuffer;
					pBuffer++;
					*pA = *pBuffer;
				}
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
		return true;
	}

	bool Image::writeToFile(const char* file)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth==0 || mHeight==0)
			return false;
		TU32 pixelSize = getPixelSize();
		if(pixelSize==0)
			return false;
		// call free image
		FREE_IMAGE_FORMAT fif = FIF_TARGA;
		FIBITMAP* dib = FreeImage_Allocate(mWidth,mHeight,pixelSize*8);
		if(!dib)
			return false;
		TUByte* bits = FreeImage_GetBits(dib);
		if(!bits)
		{
			FreeImage_Unload(dib);
			return false;
		}
		TUByte* temp = bits;
		for(TU32 i=0;i<mWidth*mHeight;++i)
		{
			getRGBA(i,&temp[FI_RGBA_RED],&temp[FI_RGBA_GREEN],&temp[FI_RGBA_BLUE],&temp[FI_RGBA_ALPHA]);
			temp += pixelSize;
		}
		// 翻转
		if(FreeImage_FlipVertical(dib)==TFALSE)
		{
			FreeImage_Unload(dib);
			return false;
		}
		if(!FreeImage_Save(fif,dib,file))
		{
			FreeImage_Unload(dib);
			return false;
		}
		FreeImage_Unload(dib);
		return true;
	}

	Image* Image::createFromFile(const char* file)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		TUByte* bits(0);
		//image width and height
		unsigned int width(0), height(0);
		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(file, 0);
		//if still unknown, try to guess the file format from the file extension
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(file);
		//if still unkown, return failure
		if(fif == FIF_UNKNOWN)
			return TNULL;
		//check that the plugin has reading capabilities and load the file
		if(FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, file);
		//if the image failed to load, return failure
		if(!dib)
			return TNULL;
		// 翻转
		if(FreeImage_FlipVertical(dib)==TFALSE)
		{
			FreeImage_Unload(dib);
			return TNULL;
		}
		// 得到一个像素的大小，单位是位
		unsigned int pixelBits = FreeImage_GetBPP(dib);
		// 得到一行的大小，单位是字节
		unsigned int rawSize = FreeImage_GetLine(dib);
		// 得到Image类型
		FREE_IMAGE_TYPE fit = FreeImage_GetImageType(dib);
		//retrieve the image data
		bits = FreeImage_GetBits(dib);
		//get the image width and height
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if((bits == 0) || (width == 0) || (height == 0))
		{
			FreeImage_Unload(dib);
			return TFALSE;
		}
		PIXEL_FORMAT format;
		switch(fit)
		{
		case FIT_BITMAP:
			{
				switch(pixelBits)
				{
				case 32:
					{
						format = PIXEL_FORMAT_R8G8B8A8;
					}
					break;
				case 24:
					{
						format = PIXEL_FORMAT_R8G8B8;
					}
					break;
				case 8:
					{
						format = PIXEL_FORMAT_R8;
					}
					break;
				default:
					{
						FreeImage_Unload(dib);
						return TFALSE;
					}
					break;
				}
			}
			break;
		default:
			{
				return TFALSE;
			}
			break;
		}
		// 重新拷贝
		TUByte* newBuff = new TUByte[rawSize*height];
		if(pixelBits==32)
		{
			int count = 0;
			// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
			int bytespp = rawSize / width;
			for(unsigned int y = 0; y < height; y++) {
				TUByte *bits = FreeImage_GetScanLine(dib, y);
				for(unsigned x = 0; x < width; x++) {
					// Set pixel color to green with a transparency of 128
					newBuff[count*4+0] = bits[FI_RGBA_RED];
					newBuff[count*4+1] = bits[FI_RGBA_GREEN];
					newBuff[count*4+2] = bits[FI_RGBA_BLUE];
					newBuff[count*4+3] = bits[FI_RGBA_ALPHA];
					// jump to next pixel
					bits += bytespp;
					count++;
				}
			}
		}
		else if(pixelBits==24)
		{
			int count = 0;
			// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
			int bytespp = rawSize / width;
			for(unsigned int y = 0; y < height; y++) {
				TUByte *bits = FreeImage_GetScanLine(dib, y);
				for(unsigned x = 0; x < width; x++) {
					// Set pixel color to green with a transparency of 128
					newBuff[count*3+0] = bits[FI_RGBA_RED];
					newBuff[count*3+1] = bits[FI_RGBA_GREEN];
					newBuff[count*3+2] = bits[FI_RGBA_BLUE];
					// jump to next pixel
					bits += bytespp;
					count++;
				}
			}
		}
		else
		{
			memcpy(newBuff,bits,rawSize*height);
		}

		Image* pRet = createFromMemory(newBuff,width,height,format);
		FreeImage_Unload(dib);
		delete [] newBuff;
		return pRet;
	}
}