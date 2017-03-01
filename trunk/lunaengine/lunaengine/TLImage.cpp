#include "TLImage.h"
#include "TLMemDef.h"
#include "FreeImage.h"
#include <memory.h>

namespace TLunaEngine
{
	Image::Image() : mPixelBuffer(TSun::TNULL),mWidth(0),mHeight(0)
	{
		
	}

	Image::Image(const Image& right) : mPixelBuffer(TSun::TNULL),mWidth(0),mHeight(0)
	{
		createBuffer(right.mWidth,right.mHeight,right.mPixelFormat);
		copyFromBuffer(right.mPixelBuffer);
	}

	Image::~Image()
	{
		if(mPixelBuffer)
		{
			getEngineBlockMemAllocator()->freeMem(mPixelBuffer, __FILE__, __LINE__);
			//delete [] mPixelBuffer;
			mPixelBuffer = TSun::TNULL;
		}
	}

	TSun::TVOID Image::initImage(TSun::TU32 width, TSun::TU32 height, PIXEL_FORMAT format)
	{
		createBuffer(width, height, format);
	}

	TSun::TBOOL Image::createBuffer(TSun::TU32 width, TSun::TU32 height, PIXEL_FORMAT format)
	{
		if(mPixelBuffer)
			return TSun::TFALSE;
		if(width==0 || height==0)
			return TSun::TFALSE;
		switch(format)
		{
		case PIXEL_FORMAT_R8:
			{
				//mPixelBuffer = new TSun::TUByte[width*height];
				mPixelBuffer = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(width*height, __FILE__, __LINE__);
				memset(mPixelBuffer,0,sizeof(TSun::TUByte)*width*height);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				//mPixelBuffer = new TSun::TUByte[width*height*3];
				mPixelBuffer = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(width*height * 3, __FILE__, __LINE__);
				memset(mPixelBuffer,0,sizeof(TSun::TUByte)*width*height*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				//mPixelBuffer = new TSun::TUByte[width*height*4];
				mPixelBuffer = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(width*height * 4, __FILE__, __LINE__);
				memset(mPixelBuffer,0,sizeof(TSun::TUByte)*width*height*4);
			}
			break;
		default:
			{
				return TSun::TFALSE;
			}
			break;
		}
		mPixelFormat = format;
		mWidth = width;
		mHeight = height;
		return TSun::TTRUE;
	}

	TSun::TBOOL Image::copyFromBuffer(const TSun::TUByte* pBuffer)
	{
		if(!pBuffer || !mPixelBuffer)
			return TSun::TFALSE;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(TSun::TUByte)*mWidth*mHeight);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(TSun::TUByte)*mWidth*mHeight*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				memcpy(mPixelBuffer,pBuffer,sizeof(TSun::TUByte)*mWidth*mHeight*4);
			}
			break;
		default:
			{
				return TSun::TFALSE;
			}
			break;
		}
		return TSun::TTRUE;
	}

	Image* Image::createFromMemory(const TSun::TUByte* pBuffer, TSun::TU32 width, TSun::TU32 height, Image::PIXEL_FORMAT format)
	{
		if(!pBuffer)
			return TSun::TNULL;
		Image* pRet = T_NEW(getEngineStructMemAllocator(), Image);
		pRet->initImage(width, height, format);
		if(pRet->mPixelBuffer==TSun::TNULL)
		{
			T_DELETE(getEngineStructMemAllocator(), Image, pRet);
			return TSun::TNULL;
		}
		if(!pRet->copyFromBuffer(pBuffer))
		{
			T_DELETE(getEngineStructMemAllocator(), Image, pRet);
			return TSun::TNULL;
		}
		return pRet;
	}

	TSun::TBOOL Image::copyToMemory(TSun::TUByte** ppBuffer)
	{
		if(!mPixelBuffer)
			return TSun::TFALSE;
		if(mWidth<=0 || mHeight<=0)
			return TSun::TFALSE;
		if(!ppBuffer)
			return TSun::TFALSE;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				//*ppBuffer = new TSun::TUByte[mWidth*mHeight];
				*ppBuffer = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(mWidth*mHeight, __FILE__, __LINE__);
				memcpy(*ppBuffer,mPixelBuffer,sizeof(TSun::TUByte)*mWidth*mHeight);
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				//*ppBuffer = new TSun::TUByte[mWidth*mHeight*3];
				*ppBuffer = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(mWidth*mHeight * 3, __FILE__, __LINE__);
				memcpy(*ppBuffer,mPixelBuffer,sizeof(TSun::TUByte)*mWidth*mHeight*3);
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				//*ppBuffer = new TSun::TUByte[mWidth*mHeight*4];
				*ppBuffer = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(mWidth*mHeight * 4, __FILE__, __LINE__);
				memcpy(*ppBuffer,mPixelBuffer,sizeof(TSun::TUByte)*mWidth*mHeight*4);
			}
			break;
		default:
			{
				return TSun::TFALSE;
			}
			break;
		}
		return TSun::TTRUE;
	}

	Image* Image::clone()
	{
		if(!mPixelBuffer)
			return TSun::TNULL;
		if(mWidth<=0 || mHeight<=0)
			return TSun::TNULL;
		Image* pRet = T_NEW(getEngineStructMemAllocator(), Image);
		pRet->initImage(mWidth, mHeight, mPixelFormat);
		if(pRet->mPixelBuffer==TSun::TNULL)
		{
			T_DELETE(getEngineStructMemAllocator(), Image, pRet);
			return TSun::TNULL;
		}
		if(!pRet->copyFromBuffer(mPixelBuffer))
		{
			T_DELETE(getEngineStructMemAllocator(), Image, pRet);
			return TSun::TNULL;
		}
		return pRet;
	}

	TSun::TU32 Image::getPixelSize()
	{
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				return sizeof(TSun::TUByte)*1;
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				return sizeof(TSun::TUByte)*3;
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				return sizeof(TSun::TUByte)*4;
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

	TSun::TU32 Image::getImageSize(TSun::TU32* pWidth,TSun::TU32* pHeight)
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

	TSun::TUByte* Image::getBufferPointer(TSun::TU32 pixelIndex)
	{
		if(!mPixelBuffer)
			return TSun::TNULL;
		if(mWidth==0 || mHeight==0)
			return TSun::TNULL;
		switch(mPixelFormat)
		{
		case PIXEL_FORMAT_R8:
			{
				if(pixelIndex>=mWidth*mHeight)
					return TSun::TNULL;
				return &mPixelBuffer[pixelIndex];
			}
			break;
		case PIXEL_FORMAT_R8G8B8:
			{
				if(pixelIndex>=mWidth*mHeight*3)
					return TSun::TNULL;
				return &mPixelBuffer[pixelIndex*3];
			}
			break;
		case PIXEL_FORMAT_R8G8B8A8:
			{
				if(pixelIndex>=mWidth*mHeight*4)
					return TSun::TNULL;
				return &mPixelBuffer[pixelIndex*4];
			}
			break;
		default:
			{
				return TSun::TNULL;
			}
			break;
		}
		return TSun::TNULL;
	}

	TSun::TBOOL Image::setRGBA(TSun::TU32 pixelIndex, TSun::TUByte r, TSun::TUByte g, TSun::TUByte b, TSun::TUByte a)
	{
		if(!mPixelBuffer)
			return TSun::TFALSE;
		if(mWidth==0 || mHeight==0)
			return TSun::TFALSE;
		TSun::TUByte* pBuffer = getBufferPointer(pixelIndex);
		if(!pBuffer)
			return TSun::TFALSE;
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
				return TSun::TFALSE;
			}
			break;
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL Image::getRGBA(TSun::TU32 pixelIndex, TSun::TUByte* pR, TSun::TUByte* pG, TSun::TUByte* pB, TSun::TUByte* pA)
	{
		if(!mPixelBuffer)
			return TSun::TFALSE;
		if(mWidth==0 || mHeight==0)
			return TSun::TFALSE;
		TSun::TUByte* pBuffer = getBufferPointer(pixelIndex);
		if(!pBuffer)
			return TSun::TFALSE;
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
				return TSun::TFALSE;
			}
			break;
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL Image::writeToFile(const TSun::TCHAR* file)
	{
		if(!mPixelBuffer)
			return TSun::TFALSE;
		if(mWidth==0 || mHeight==0)
			return TSun::TFALSE;
		TSun::TU32 pixelSize = getPixelSize();
		if(pixelSize==0)
			return TSun::TFALSE;
		// call free image
		FREE_IMAGE_FORMAT fif = FIF_TARGA;
		FIBITMAP* dib = FreeImage_Allocate(mWidth,mHeight,pixelSize*8);
		if(!dib)
			return TSun::TFALSE;
		TSun::TUByte* bits = FreeImage_GetBits(dib);
		if(!bits)
		{
			FreeImage_Unload(dib);
			return TSun::TFALSE;
		}
		TSun::TUByte* temp = bits;
		for(TSun::TU32 i=0;i<mWidth*mHeight;++i)
		{
			getRGBA(i,&temp[FI_RGBA_RED],&temp[FI_RGBA_GREEN],&temp[FI_RGBA_BLUE],&temp[FI_RGBA_ALPHA]);
			temp += pixelSize;
		}
		// 翻转
		if(FreeImage_FlipVertical(dib)==TSun::TFALSE)
		{
			FreeImage_Unload(dib);
			return TSun::TFALSE;
		}
		if(!FreeImage_Save(fif,dib,file))
		{
			FreeImage_Unload(dib);
			return TSun::TFALSE;
		}
		FreeImage_Unload(dib);
		return TSun::TTRUE;
	}

	Image* Image::createFromFile(const TSun::TCHAR* file)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		TSun::TUByte* bits(0);
		//image width and height
		TSun::TU32 width(0), height(0);
		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(file, 0);
		//if still unknown, try to guess the file format from the file extension
		if(fif == FIF_UNKNOWN) 
			fif = FreeImage_GetFIFFromFilename(file);
		//if still unkown, return failure
		if(fif == FIF_UNKNOWN)
			return TSun::TNULL;
		//check that the plugin has reading capabilities and load the file
		if(FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, file);
		//if the image failed to load, return failure
		if(!dib)
			return TSun::TNULL;
		// 翻转
		if(FreeImage_FlipVertical(dib)==TSun::TFALSE)
		{
			FreeImage_Unload(dib);
			return TSun::TNULL;
		}
		// 得到一个像素的大小，单位是位
		TSun::TU32 pixelBits = FreeImage_GetBPP(dib);
		// 得到一行的大小，单位是字节
		TSun::TU32 rawSize = FreeImage_GetLine(dib);
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
			return TSun::TFALSE;
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
						return TSun::TFALSE;
					}
					break;
				}
			}
			break;
		default:
			{
				return TSun::TFALSE;
			}
			break;
		}
		// 重新拷贝
		//TSun::TUByte* newBuff = new TSun::TUByte[rawSize*height];
		TSun::TUByte* newBuff = (TSun::TUByte*)getEngineBlockMemAllocator()->allocateMem(rawSize*height, __FILE__, __LINE__);
		if(pixelBits==32)
		{
			TSun::TS32 count = 0;
			// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
			TSun::TS32 bytespp = rawSize / width;
			for(TSun::TU32 y = 0; y < height; y++) {
				TSun::TUByte *bits = FreeImage_GetScanLine(dib, y);
				for(TSun::TU32 x = 0; x < width; x++) {
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
			TSun::TS32 count = 0;
			// Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
			TSun::TS32 bytespp = rawSize / width;
			for(TSun::TU32 y = 0; y < height; y++) {
				TSun::TUByte *bits = FreeImage_GetScanLine(dib, y);
				for(TSun::TU32 x = 0; x < width; x++) {
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
		getEngineBlockMemAllocator()->freeMem(newBuff, __FILE__, __LINE__);
		//delete [] newBuff;
		return pRet;
	}
}