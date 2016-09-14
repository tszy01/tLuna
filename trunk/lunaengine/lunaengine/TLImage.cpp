#include "stdafx.h"
#include "TLImage.h"
#include "FreeImage.h"

namespace TLunaEngine
{
	TLImage::TLImage(UINT width, UINT height, PIXEL_FORMAT format) : mPixelBuffer(NULL),mWidth(0),mHeight(0)
	{
		createBuffer(width,height,format);
	}

	TLImage::TLImage(const TLImage& right) : mPixelBuffer(NULL),mWidth(0),mHeight(0)
	{
		createBuffer(right.mWidth,right.mHeight,right.mPixelFormat);
		copyFromBuffer(right.mPixelBuffer);
	}

	TLImage::~TLImage()
	{
		if(mPixelBuffer)
		{
			delete [] mPixelBuffer;
			mPixelBuffer = NULL;
		}
	}

	bool TLImage::createBuffer(UINT width, UINT height, PIXEL_FORMAT format)
	{
		if(mPixelBuffer)
			return false;
		if(width==0 || height==0)
			return false;
		switch(format)
		{
		case PIXEL_FORMAT_R8:
			{
				mPixelBuffer = new BYTE[width*height];
				memset(mPixelBuffer,0,sizeof(BYTE)*width*height);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				mPixelBuffer = new BYTE[width*height*3];
				memset(mPixelBuffer,0,sizeof(BYTE)*width*height*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				mPixelBuffer = new BYTE[width*height*4];
				memset(mPixelBuffer,0,sizeof(BYTE)*width*height*4);
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

	bool TLImage::copyFromBuffer(const BYTE* pBuffer)
	{
		if(!pBuffer || !mPixelBuffer)
			return false;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(BYTE)*mWidth*mHeight);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(BYTE)*mWidth*mHeight*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(BYTE)*mWidth*mHeight*4);
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

	TLImage* TLImage::createFromMemory(const BYTE* pBuffer, UINT width, UINT height, TLImage::PIXEL_FORMAT format)
	{
		if(!pBuffer)
			return NULL;
		TLImage* pRet = new TLImage(width,height,format);
		if(pRet->mPixelBuffer==NULL)
		{
			delete pRet;
			return NULL;
		}
		if(!pRet->copyFromBuffer(pBuffer))
		{
			delete pRet;
			return NULL;
		}
		return pRet;
	}

	bool TLImage::copyToMemory(BYTE** ppBuffer)
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
				*ppBuffer = new BYTE[mWidth*mHeight];
				memcpy(*ppBuffer,mPixelBuffer,sizeof(BYTE)*mWidth*mHeight);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				*ppBuffer = new BYTE[mWidth*mHeight*3];
				memcpy(*ppBuffer,mPixelBuffer,sizeof(BYTE)*mWidth*mHeight*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				*ppBuffer = new BYTE[mWidth*mHeight*4];
				memcpy(*ppBuffer,mPixelBuffer,sizeof(BYTE)*mWidth*mHeight*4);
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

	TLImage* TLImage::clone()
	{
		if(!mPixelBuffer)
			return NULL;
		if(mWidth<=0 || mHeight<=0)
			return NULL;
		TLImage* pRet = new TLImage(mWidth,mHeight,mPixelFormat);
		if(pRet->mPixelBuffer==NULL)
		{
			delete pRet;
			return NULL;
		}
		if(!pRet->copyFromBuffer(mPixelBuffer))
		{
			delete pRet;
			return NULL;
		}
		return pRet;
	}

	UINT TLImage::getPixelSize()
	{
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				return sizeof(BYTE)*1;
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				return sizeof(BYTE)*3;
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				return sizeof(BYTE)*4;
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

	UINT TLImage::getImageSize(UINT* pWidth,UINT* pHeight)
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

	TLImage::PIXEL_FORMAT TLImage::getPixelFormat()
	{
		return mPixelFormat;
	}

	BYTE* TLImage::getBufferPointer(UINT pixelIndex)
	{
		if(!mPixelBuffer)
			return NULL;
		if(mWidth==0 || mHeight==0)
			return NULL;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				if(pixelIndex>=mWidth*mHeight)
					return NULL;
				return &mPixelBuffer[pixelIndex];
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				if(pixelIndex>=mWidth*mHeight*3)
					return NULL;
				return &mPixelBuffer[pixelIndex*3];
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				if(pixelIndex>=mWidth*mHeight*4)
					return NULL;
				return &mPixelBuffer[pixelIndex*4];
			}
			break;
		default:
			{
				return NULL;
			}
			break;
		}
		return NULL;
	}

	bool TLImage::setRGBA(UINT pixelIndex, BYTE r, BYTE g, BYTE b, BYTE a)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth==0 || mHeight==0)
			return false;
		BYTE* pBuffer = getBufferPointer(pixelIndex);
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

	bool TLImage::getRGBA(UINT pixelIndex, BYTE* pR, BYTE* pG, BYTE* pB, BYTE* pA)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth==0 || mHeight==0)
			return false;
		BYTE* pBuffer = getBufferPointer(pixelIndex);
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

	bool TLImage::writeToFile(const char* file)
	{
		if(!mPixelBuffer)
			return false;
		if(mWidth==0 || mHeight==0)
			return false;
		UINT pixelSize = getPixelSize();
		if(pixelSize==0)
			return false;
		// call free image
		FREE_IMAGE_FORMAT fif = FIF_TARGA;
		FIBITMAP* dib = FreeImage_Allocate(mWidth,mHeight,pixelSize*8);
		if(!dib)
			return false;
		BYTE* bits = FreeImage_GetBits(dib);
		if(!bits)
		{
			FreeImage_Unload(dib);
			return false;
		}
		BYTE* temp = bits;
		for(UINT i=0;i<mWidth*mHeight;++i)
		{
			getRGBA(i,&temp[FI_RGBA_RED],&temp[FI_RGBA_GREEN],&temp[FI_RGBA_BLUE],&temp[FI_RGBA_ALPHA]);
			temp += pixelSize;
		}
		// 翻转
		if(FreeImage_FlipVertical(dib)==FALSE)
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

	TLImage* TLImage::createFromFile(const char* file)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		BYTE* bits(0);
		//image width and height
		unsigned int width(0), height(0);
		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(file, 0);
		//if still unknown, try to guess the file format from the file extension
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(file);
		//if still unkown, return failure
		if(fif == FIF_UNKNOWN)
			return NULL;
		//check that the plugin has reading capabilities and load the file
		if(FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, file);
		//if the image failed to load, return failure
		if(!dib)
			return NULL;
		// 翻转
		if(FreeImage_FlipVertical(dib)==FALSE)
		{
			FreeImage_Unload(dib);
			return NULL;
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
			return FALSE;
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
						return FALSE;
					}
					break;
				}
			}
			break;
		default:
			{
				return FALSE;
			}
			break;
		}
		// 重新拷贝
		BYTE* newBuff = new BYTE[rawSize*height];
		if(pixelBits==32)
		{
			int count = 0;
			// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
			int bytespp = rawSize / width;
			for(unsigned int y = 0; y < height; y++) {
				BYTE *bits = FreeImage_GetScanLine(dib, y);
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
				BYTE *bits = FreeImage_GetScanLine(dib, y);
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

		TLImage* pRet = createFromMemory(newBuff,width,height,format);
		FreeImage_Unload(dib);
		delete [] newBuff;
		return pRet;
	}
}