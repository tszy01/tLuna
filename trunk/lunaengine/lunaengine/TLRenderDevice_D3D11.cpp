#include "stdafx.h"
#include "TLRenderDevice_D3D11.h"

#include "TLRenderDeviceUsedBuffer_D3D11.h"
#include "TLRenderDeviceUsedTex1D_D3D11.h"
#include "TLRenderDeviceUsedTex2D_D3D11.h"
#include "TLRenderDeviceUsedTex3D_D3D11.h"
#include "TLRenderDeviceUsedSRV_D3D11.h"
#include "TLRenderDeviceUsedDSV_D3D11.h"
#include "TLRenderDeviceUsedRTV_D3D11.h"
#include "TLRenderDeviceUsedUAV_D3D11.h"
#include "TLRenderDeviceUsedVS_D3D11.h"
#include "TLRenderDeviceUsedHS_D3D11.h"
#include "TLRenderDeviceUsedDS_D3D11.h"
#include "TLRenderDeviceUsedGS_D3D11.h"
#include "TLRenderDeviceUsedPS_D3D11.h"
#include "TLRenderDeviceUsedCS_D3D11.h"
#include "TLRenderDeviceUsedRasterizerState_D3D11.h"
#include "TLRenderDeviceUsedDepthStencilState_D3D11.h"
#include "TLRenderDeviceUsedBlendState_D3D11.h"
#include "TLRenderDeviceUsedSamplerState_D3D11.h"
#include "TLRenderDeviceUsedInputLayout_D3D11.h"
#include "TLRenderDeviceCompiledShader_D3D11.h"

#include "DDSTextureLoader.h"

namespace TLunaEngine {
	TLRenderDevice_D3D11::TLRenderDevice_D3D11() : TLIRenderDevice(), m_pd3dDevice(NULL),m_pImmediateContext(NULL),
		m_pSwapChain(NULL),m_pRenderTargetView(NULL),m_pDepthStencil(NULL),m_pDepthStencilView(NULL),m_loaded(false)
	{
	}

	TLRenderDevice_D3D11::~TLRenderDevice_D3D11()
	{
	}

	bool TLRenderDevice_D3D11::InitDevice(HWND hWnd,BOOL bWnd,LONG width,LONG height)
	{
		if(m_loaded)
			return false;
		HRESULT hr = S_OK;
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			//D3D_FEATURE_LEVEL_9_3,
			//D3D_FEATURE_LEVEL_9_2,
			//D3D_FEATURE_LEVEL_9_1,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = (UINT)width;
		sd.BufferDesc.Height = (UINT)height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = bWnd;
		sd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;

		m_DriverType = D3D_DRIVER_TYPE_NULL;
		m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			m_DriverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
												D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		if( FAILED( hr ) )
			return false;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if( FAILED( hr ) )
			return false;

		hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
		pBackBuffer->Release();
		if( FAILED( hr ) )
			return false;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = (UINT)width;
		descDepth.Height = (UINT)height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = m_pd3dDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
		if( FAILED( hr ) )
			return false;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_pd3dDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
		if( FAILED( hr ) )
			return false;

		m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pImmediateContext->RSSetViewports( 1, &vp );
		mDefaultVp._Height = vp.Height;
		mDefaultVp._MaxDepth = vp.MaxDepth;
		mDefaultVp._MinDepth = vp.MinDepth;
		mDefaultVp._TopLeftX = vp.TopLeftX;
		mDefaultVp._TopLeftY = vp.TopLeftY;
		mDefaultVp._Width = vp.Width;
		m_loaded = true;
		return true;
	}

	void TLRenderDevice_D3D11::DestroyDevice()
	{
		if(!m_loaded)
			return;
		m_loaded = false;
		if(m_pDepthStencil)
		{
			m_pDepthStencil->Release();
			m_pDepthStencil = NULL;
		}
		if(m_pDepthStencilView)
		{
			m_pDepthStencilView->Release();
			m_pDepthStencilView = NULL;
		}
		if(m_pRenderTargetView)
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = NULL;
		}
		if(m_pSwapChain)
		{
			m_pSwapChain->Release();
			m_pSwapChain = NULL;
		}
		if(m_pImmediateContext)
		{
			m_pImmediateContext->Release();
			m_pImmediateContext = NULL;
		}
		if(m_pd3dDevice)
		{
			m_pd3dDevice->Release();
			m_pd3dDevice = NULL;
		}
	}

	bool TLRenderDevice_D3D11::BeginRender()
	{
		if(m_pImmediateContext && m_pRenderTargetView && m_pDepthStencilView && m_loaded)
		{
			//
			// Clear the back buffer
			//
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha
			m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

			//
			// Clear the depth buffer to 1.0 (max depth)
			//
			m_pImmediateContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		}
		return true;
	}

	void TLRenderDevice_D3D11::EndRender()
	{
		if(m_pSwapChain && m_loaded)
		{
			m_pSwapChain->Present( 0, 0 );
		}
	}

	void TLRenderDevice_D3D11::UseDefaultRenderTarget()
	{
		if(m_pImmediateContext && m_pRenderTargetView && m_pDepthStencilView && m_loaded)
		{
			m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );
		}
	}

	void TLRenderDevice_D3D11::UseDefaultViewPort()
	{
		if(m_pImmediateContext && m_loaded)
		{
			useViewport(&mDefaultVp,1);
		}
	}

	UINT TLRenderDevice_D3D11::GetBufferWidth()
	{
		if(m_pSwapChain && m_loaded)
		{
			DXGI_SWAP_CHAIN_DESC desc;
			m_pSwapChain->GetDesc(&desc);
			return desc.BufferDesc.Width;
		}
		return 0;
	}

	UINT TLRenderDevice_D3D11::GetBufferHeight()
	{
		if(m_pSwapChain && m_loaded)
		{
			DXGI_SWAP_CHAIN_DESC desc;
			m_pSwapChain->GetDesc(&desc);
			return desc.BufferDesc.Height;
		}
		return 0;
	}

	bool TLRenderDevice_D3D11::resizeBuffer(UINT width,UINT height)
	{
		if(m_loaded && m_pd3dDevice && m_pImmediateContext && m_pSwapChain)
		{
			m_pImmediateContext->OMSetRenderTargets( 0, 0, 0 );
			m_pDepthStencilView->Release();
			m_pDepthStencil->Release();
			m_pRenderTargetView->Release();

			m_pSwapChain->ResizeBuffers(1,width,height,DXGI_FORMAT_R8G8B8A8_UNORM,/*DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH*/0);

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D),(void**) &pBuffer );
			// Perform error handling here!
			hr = m_pd3dDevice->CreateRenderTargetView(pBuffer, NULL, &m_pRenderTargetView);
			// Perform error handling here!
			pBuffer->Release();

			// Create depth stencil texture
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory( &descDepth, sizeof(descDepth) );
			descDepth.Width = width;
			descDepth.Height = height;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;
			hr = m_pd3dDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
			if( FAILED( hr ) )
				return false;
			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory( &descDSV, sizeof(descDSV) );
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = m_pd3dDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
			if( FAILED( hr ) )
				return false;

			m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

			// Setup the viewport
			mDefaultVp._Width = (FLOAT)width;
			mDefaultVp._Height = (FLOAT)height;
			mDefaultVp._MinDepth = 0.0f;
			mDefaultVp._MaxDepth = 1.0f;
			mDefaultVp._TopLeftX = 0;
			mDefaultVp._TopLeftY = 0;
			useViewport(&mDefaultVp,1);

			// Initialize the projection matrix
			//g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, (FLOAT)width / (FLOAT)height, 0.01f, 100.0f );
			//CBChangeOnResize cbChangesOnResize;
			//cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
			//g_pImmediateContext->UpdateSubresource( g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0 );
		}
		return true;
	}

	bool TLRenderDevice_D3D11::useViewport(const TLRenderDeviceViewport* aVp,UINT numVp)
	{
		if(!aVp || numVp<1 || !m_loaded || !m_pImmediateContext)
			return false;
		if(numVp==1)
		{
			D3D11_VIEWPORT theVp;
			theVp.Height = aVp->_Height;
			theVp.MaxDepth = aVp->_MaxDepth;
			theVp.MinDepth = aVp->_MinDepth;
			theVp.TopLeftX = aVp->_TopLeftX;
			theVp.TopLeftY = aVp->_TopLeftY;
			theVp.Width = aVp->_Width;
			m_pImmediateContext->RSSetViewports( numVp, &theVp );
		}
		else
		{
			D3D11_VIEWPORT* theVp = new D3D11_VIEWPORT[numVp];
			for(UINT i=0;i<numVp;i++)
			{
				theVp[i].Height = aVp[i]._Height;
				theVp[i].MaxDepth = aVp[i]._MaxDepth;
				theVp[i].MinDepth = aVp[i]._MinDepth;
				theVp[i].TopLeftX = aVp[i]._TopLeftX;
				theVp[i].TopLeftY = aVp[i]._TopLeftY;
				theVp[i].Width = aVp[i]._Width;
			}
			m_pImmediateContext->RSSetViewports( numVp, theVp );
			delete [] theVp;
			theVp = NULL;
		}
		return true;
	}

	void TLRenderDevice_D3D11::setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY pt)
	{
		if(m_loaded && m_pImmediateContext)
		{
			m_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pt);
		}
	}

	void TLRenderDevice_D3D11::setInputLayout(TLIRenderDeviceUsedInputLayout* pInputLayout)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pInputLayout)
			{
				ID3D11InputLayout* d3dLayout = ((TLRenderDeviceUsedInputLayout_D3D11*)pInputLayout)->getD3DInputLayout();
				m_pImmediateContext->IASetInputLayout(d3dLayout);
			}
			else
			{
				m_pImmediateContext->IASetInputLayout(NULL);
			}
		}
	}

	void TLRenderDevice_D3D11::getD3DDriverType(RENDER_DEVICE_D3D_DRIVER_TYPE* pDriverType)
	{
		if(pDriverType)
		{
			(*pDriverType) = (RENDER_DEVICE_D3D_DRIVER_TYPE)m_DriverType;
		}
	}

	void TLRenderDevice_D3D11::getD3DFeatureLevel(RENDER_DEVICE_D3D_FEATURE_LEVEL* pFeatureLevel)
	{
		if(pFeatureLevel)
		{
			(*pFeatureLevel) = (RENDER_DEVICE_D3D_FEATURE_LEVEL)m_FeatureLevel;
		}
	}

	const char* TLRenderDevice_D3D11::getShaderProfileVersion()
	{
		switch(m_FeatureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			{
				return "5_0";
			}
			break;
		case D3D_FEATURE_LEVEL_10_1:
			{
				return "4_1";
			}
			break;
		case D3D_FEATURE_LEVEL_10_0:
			{
				return "4_0";
			}
			break;
		default:
			{
				return "";
			}
			break;
		}
		return "";
	}

	void TLRenderDevice_D3D11::draw(UINT vertexCount,UINT startVertexLocation)
	{
		if(m_loaded && m_pImmediateContext)
		{
			m_pImmediateContext->Draw(vertexCount,startVertexLocation);
		}
	}

	void TLRenderDevice_D3D11::drawIndexed(UINT indexCount,UINT startIndexLocation,UINT baseVertexLocation)
	{
		if(m_loaded && m_pImmediateContext)
		{
			m_pImmediateContext->DrawIndexed(indexCount,startIndexLocation,baseVertexLocation);
		}
	}

	UINT TLRenderDevice_D3D11::calcSubresource(UINT mipSlice,UINT arraySlice,UINT mipLevels)
	{
		return D3D11CalcSubresource(mipSlice,arraySlice,mipLevels);
	}

	void TLRenderDevice_D3D11::generateMips(TLIRenderDeviceUsedSRV* pSRV)
	{
		if(m_loaded && m_pImmediateContext && pSRV)
		{
			ID3D11ShaderResourceView* pView = ((TLRenderDeviceUsedSRV_D3D11*)pSRV)->getD3DSRV();
			m_pImmediateContext->GenerateMips(pView);
		}
	}

	bool TLRenderDevice_D3D11::mapResource(TLIRenderDeviceUsedResource* pResource,UINT subResource,
			RENDER_DEVICE_MAP mapType,TLRenderDeviceMappedSubresource* mappedSubresource)
	{
		if(m_loaded && m_pImmediateContext && pResource && mappedSubresource)
		{
			TLIRenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pResource);
			D3D11_MAPPED_SUBRESOURCE mapped;
			if(FAILED(m_pImmediateContext->Map(pD3DResource->getD3DResource(),subResource,(D3D11_MAP)mapType,0,&mapped)))
				return false;
			mappedSubresource->DepthPitch = mapped.DepthPitch;
			mappedSubresource->pData = mapped.pData;
			mappedSubresource->RowPitch = mapped.RowPitch;
			return true;
		}
		return false;
	}

	void TLRenderDevice_D3D11::unmapResource(TLIRenderDeviceUsedResource* pResource,UINT subResource)
	{
		if(m_loaded && m_pImmediateContext && pResource)
		{
			TLIRenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pResource);
			m_pImmediateContext->Unmap(pD3DResource->getD3DResource(),subResource);
		}
	}

	void TLRenderDevice_D3D11::copySubresourceRegion(TLIRenderDeviceUsedResource* pDstResource,UINT dstSubresource,
			UINT dstX,UINT dstY,UINT dstZ,TLIRenderDeviceUsedResource* pSrcResource,UINT srcSubresource,
			const TLRenderDeviceBox* pSrcBox)
	{
		if(m_loaded && m_pImmediateContext && pDstResource && pSrcResource)
		{
			TLIRenderDeviceUsedResource_D3D11* pD3DSrcResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pSrcResource);
			TLIRenderDeviceUsedResource_D3D11* pD3DDstResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pDstResource);
			if(pSrcBox)
			{
				D3D11_BOX d3dBox;
				d3dBox.back = pSrcBox->back;
				d3dBox.bottom = pSrcBox->bottom;
				d3dBox.front = pSrcBox->front;
				d3dBox.left = pSrcBox->left;
				d3dBox.right = pSrcBox->right;
				d3dBox.top = pSrcBox->top;
				m_pImmediateContext->CopySubresourceRegion(pD3DDstResource->getD3DResource(),dstSubresource,
					dstX,dstY,dstZ,pD3DSrcResource->getD3DResource(),srcSubresource,&d3dBox);
			}
			else
			{
				m_pImmediateContext->CopySubresourceRegion(pD3DDstResource->getD3DResource(),dstSubresource,
				dstX,dstY,dstZ,pD3DSrcResource->getD3DResource(),srcSubresource,NULL);
			}
		}
	}

	void TLRenderDevice_D3D11::copyResource(TLIRenderDeviceUsedResource* pDstResource,TLIRenderDeviceUsedResource* pSrcResource)
	{
		if(m_loaded && m_pImmediateContext && pDstResource && pSrcResource)
		{
			TLIRenderDeviceUsedResource_D3D11* pD3DSrcResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pSrcResource);
			TLIRenderDeviceUsedResource_D3D11* pD3DDstResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pDstResource);
			m_pImmediateContext->CopyResource(pD3DDstResource->getD3DResource(),pD3DSrcResource->getD3DResource());
		}
	}

	void TLRenderDevice_D3D11::updateSubresource(TLIRenderDeviceUsedResource* pDstResource,UINT dstSubresource,
			const TLRenderDeviceBox* pDstBox,const void* pSrcData,UINT srcRowPitch,UINT srcDepthPitch)
	{
		if(m_loaded && m_pImmediateContext && pDstResource && pSrcData)
		{
			TLIRenderDeviceUsedResource_D3D11* pD3DDstResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pDstResource);
			if(pDstBox)
			{
				D3D11_BOX d3dBox;
				d3dBox.back = pDstBox->back;
				d3dBox.bottom = pDstBox->bottom;
				d3dBox.front = pDstBox->front;
				d3dBox.left = pDstBox->left;
				d3dBox.right = pDstBox->right;
				d3dBox.top = pDstBox->top;
				m_pImmediateContext->UpdateSubresource(pD3DDstResource->getD3DResource(),dstSubresource,&d3dBox,pSrcData,srcRowPitch,srcDepthPitch);
			}
			else
			{
				m_pImmediateContext->UpdateSubresource(pD3DDstResource->getD3DResource(),dstSubresource,NULL,pSrcData,srcRowPitch,srcDepthPitch);
			}
		}
	}

	void TLRenderDevice_D3D11::setVertexBuffer(UINT startSlot,TLIRenderDeviceUsedBuffer* pBuffer, const UINT* pStride,const UINT* pOffset)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBuffer && pStride && pOffset)
			{
				ID3D11Buffer* pD3DBuffer = ((TLRenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
				m_pImmediateContext->IASetVertexBuffers(startSlot,1,&pD3DBuffer,pStride,pOffset);
			}
			else
			{
				m_pImmediateContext->IASetVertexBuffers(startSlot,0,NULL,NULL,NULL);
			}
		}
	}

	void TLRenderDevice_D3D11::setIndexBuffer(TLIRenderDeviceUsedBuffer* pBuffer,RENDER_DEVICE_FORMAT format,UINT offset)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBuffer)
			{
				ID3D11Buffer* pD3DBuffer = ((TLRenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
				m_pImmediateContext->IASetIndexBuffer(pD3DBuffer,(DXGI_FORMAT)format,offset);
			}
			else
			{
				m_pImmediateContext->IASetIndexBuffer(NULL,(DXGI_FORMAT)format,offset);
			}
		}
	}

	void TLRenderDevice_D3D11::setSOTarget(TLIRenderDeviceUsedBuffer* pBuffer,const UINT* pOffset)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBuffer && pOffset)
			{
				ID3D11Buffer* pD3DBuffer = ((TLRenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
				m_pImmediateContext->SOSetTargets(1,&pD3DBuffer,pOffset);
			}
			else
			{
				m_pImmediateContext->SOSetTargets(0,NULL,NULL);
			}
		}
	}

	void TLRenderDevice_D3D11::clearRenderTargetView(TLIRenderDeviceUsedRTV* pRenderTargetView, const FLOAT colorRGBA[4])
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pRenderTargetView)
			{
				ID3D11RenderTargetView* pD3DView = ((TLRenderDeviceUsedRTV_D3D11*)pRenderTargetView)->getD3DRTV();
				m_pImmediateContext->ClearRenderTargetView(pD3DView,colorRGBA);
			}
		}
	}

	void TLRenderDevice_D3D11::clearDepthStencilView(TLIRenderDeviceUsedDSV* pDepthStencilView, FLOAT depth)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pDepthStencilView)
			{
				ID3D11DepthStencilView* pD3DView = ((TLRenderDeviceUsedDSV_D3D11*)pDepthStencilView)->getD3DDSV();
				m_pImmediateContext->ClearDepthStencilView(pD3DView,D3D11_CLEAR_DEPTH,depth,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setRenderTarget(TLIRenderDeviceUsedRTV* pRTV, TLIRenderDeviceUsedDSV* pDSV)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11RenderTargetView* pD3DRTV = NULL;
			UINT numRTV = 0;
			ID3D11DepthStencilView* pD3DDSV = NULL;
			if(pRTV)
			{
				pD3DRTV = ((TLRenderDeviceUsedRTV_D3D11*)pRTV)->getD3DRTV();
				numRTV = 1;
			}
			if(pDSV)
			{
				pD3DDSV = ((TLRenderDeviceUsedDSV_D3D11*)pDSV)->getD3DDSV();
			}
			if(numRTV > 0)
			{
				m_pImmediateContext->OMSetRenderTargets(numRTV,&pD3DRTV,pD3DDSV);
			}
			else
			{
				m_pImmediateContext->OMSetRenderTargets(0,NULL,pD3DDSV);
			}
		}
	}

	void TLRenderDevice_D3D11::setRasterizerState(TLIRenderDeviceUsedRasterizerState* pRasterizerState)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pRasterizerState)
			{
				ID3D11RasterizerState* pD3DState = ((TLRenderDeviceUsedRasterizerState_D3D11*)pRasterizerState)->getD3DRasterizerState();
				m_pImmediateContext->RSSetState(pD3DState);
			}
			else
			{
				m_pImmediateContext->RSSetState(NULL);
			}
		}
	}

	void TLRenderDevice_D3D11::setDepthStencilState(TLIRenderDeviceUsedDepthStencilState* pDepthStencilState)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pDepthStencilState)
			{
				ID3D11DepthStencilState* pD3DState = ((TLRenderDeviceUsedDepthStencilState_D3D11*)pDepthStencilState)->getD3DDepthStencilState();
				m_pImmediateContext->OMSetDepthStencilState(pD3DState,0);
			}
			else
			{
				m_pImmediateContext->OMSetDepthStencilState(NULL,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setBlendState(TLIRenderDeviceUsedBlendState* pBlendState,const FLOAT blendFactor[4],UINT sampleMask)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBlendState)
			{
				ID3D11BlendState* pD3DState = ((TLRenderDeviceUsedBlendState_D3D11*)pBlendState)->getD3DBlendState();
				m_pImmediateContext->OMSetBlendState(pD3DState,blendFactor,sampleMask);
			}
			else
			{
				m_pImmediateContext->OMSetBlendState(NULL,NULL,0xffffffff);
			}
		}
	}

	void TLRenderDevice_D3D11::setConstantBuffer(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedBuffer* pBuffer)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11Buffer* pD3DBuffer = NULL;
			if(pBuffer)
			{
				pD3DBuffer = ((TLRenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
			}
			switch(shaderType)
			{
			case RENDER_DEVICE_SHADER_USE_VS:
				{
					if(pBuffer)
					{
						m_pImmediateContext->VSSetConstantBuffers(startSlot,1,&pD3DBuffer);
					}
					else
					{
						m_pImmediateContext->VSSetConstantBuffers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_HS:
				{
					if(pBuffer)
					{
						m_pImmediateContext->HSSetConstantBuffers(startSlot,1,&pD3DBuffer);
					}
					else
					{
						m_pImmediateContext->HSSetConstantBuffers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_DS:
				{
					if(pBuffer)
					{
						m_pImmediateContext->DSSetConstantBuffers(startSlot,1,&pD3DBuffer);
					}
					else
					{
						m_pImmediateContext->DSSetConstantBuffers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_GS:
				{
					if(pBuffer)
					{
						m_pImmediateContext->GSSetConstantBuffers(startSlot,1,&pD3DBuffer);
					}
					else
					{
						m_pImmediateContext->GSSetConstantBuffers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_PS:
				{
					if(pBuffer)
					{
						m_pImmediateContext->PSSetConstantBuffers(startSlot,1,&pD3DBuffer);
					}
					else
					{
						m_pImmediateContext->PSSetConstantBuffers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_CS:
				{
					if(pBuffer)
					{
						m_pImmediateContext->CSSetConstantBuffers(startSlot,1,&pD3DBuffer);
					}
					else
					{
						m_pImmediateContext->CSSetConstantBuffers(startSlot,0,NULL);
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}
	}

	void TLRenderDevice_D3D11::setSamplerState(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedSamplerState* pSampler)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11SamplerState* pD3DState = NULL;
			if(pSampler)
			{
				pD3DState = ((TLRenderDeviceUsedSamplerState_D3D11*)pSampler)->getD3DSamplerState();
			}
			switch(shaderType)
			{
			case RENDER_DEVICE_SHADER_USE_VS:
				{
					if(pSampler)
					{
						m_pImmediateContext->VSSetSamplers(startSlot,1,&pD3DState);
					}
					else
					{
						m_pImmediateContext->VSSetSamplers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_HS:
				{
					if(pSampler)
					{
						m_pImmediateContext->HSSetSamplers(startSlot,1,&pD3DState);
					}
					else
					{
						m_pImmediateContext->HSSetSamplers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_DS:
				{
					if(pSampler)
					{
						m_pImmediateContext->DSSetSamplers(startSlot,1,&pD3DState);
					}
					else
					{
						m_pImmediateContext->DSSetSamplers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_GS:
				{
					if(pSampler)
					{
						m_pImmediateContext->GSSetSamplers(startSlot,1,&pD3DState);
					}
					else
					{
						m_pImmediateContext->GSSetSamplers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_PS:
				{
					if(pSampler)
					{
						m_pImmediateContext->PSSetSamplers(startSlot,1,&pD3DState);
					}
					else
					{
						m_pImmediateContext->PSSetSamplers(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_CS:
				{
					if(pSampler)
					{
						m_pImmediateContext->CSSetSamplers(startSlot,1,&pD3DState);
					}
					else
					{
						m_pImmediateContext->CSSetSamplers(startSlot,0,NULL);
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}
	}

	void TLRenderDevice_D3D11::setShaderResourceView(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedSRV* pSRV)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11ShaderResourceView* pD3DView = NULL;
			if(pSRV)
			{
				pD3DView = ((TLRenderDeviceUsedSRV_D3D11*)pSRV)->getD3DSRV();
			}
			switch(shaderType)
			{
			case RENDER_DEVICE_SHADER_USE_VS:
				{
					if(pSRV)
					{
						m_pImmediateContext->VSSetShaderResources(startSlot,1,&pD3DView);
					}
					else
					{
						m_pImmediateContext->VSSetShaderResources(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_HS:
				{
					if(pSRV)
					{
						m_pImmediateContext->HSSetShaderResources(startSlot,1,&pD3DView);
					}
					else
					{
						m_pImmediateContext->HSSetShaderResources(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_DS:
				{
					if(pSRV)
					{
						m_pImmediateContext->DSSetShaderResources(startSlot,1,&pD3DView);
					}
					else
					{
						m_pImmediateContext->DSSetShaderResources(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_GS:
				{
					if(pSRV)
					{
						m_pImmediateContext->GSSetShaderResources(startSlot,1,&pD3DView);
					}
					else
					{
						m_pImmediateContext->GSSetShaderResources(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_PS:
				{
					if(pSRV)
					{
						m_pImmediateContext->PSSetShaderResources(startSlot,1,&pD3DView);
					}
					else
					{
						m_pImmediateContext->PSSetShaderResources(startSlot,0,NULL);
					}
				}
				break;
			case RENDER_DEVICE_SHADER_USE_CS:
				{
					if(pSRV)
					{
						m_pImmediateContext->CSSetShaderResources(startSlot,1,&pD3DView);
					}
					else
					{
						m_pImmediateContext->CSSetShaderResources(startSlot,0,NULL);
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}
	}

	void TLRenderDevice_D3D11::setVertexShader(TLIRenderDeviceUsedVS* pVertexShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pVertexShader)
			{
				ID3D11VertexShader* pD3DShader = ((TLRenderDeviceUsedVS_D3D11*)pVertexShader)->getD3DVS();
				m_pImmediateContext->VSSetShader(pD3DShader,NULL,0);
			}
			else
			{
				m_pImmediateContext->VSSetShader(NULL,0,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setHullShader(TLIRenderDeviceUsedHS* pHullShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pHullShader)
			{
				ID3D11HullShader* pD3DShader = ((TLRenderDeviceUsedHS_D3D11*)pHullShader)->getD3DHS();
				m_pImmediateContext->HSSetShader(pD3DShader,NULL,0);
			}
			else
			{
				m_pImmediateContext->HSSetShader(NULL,0,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setDomainShader(TLIRenderDeviceUsedDS* pDomainShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pDomainShader)
			{
				ID3D11DomainShader* pD3DShader = ((TLRenderDeviceUsedDS_D3D11*)pDomainShader)->getD3DDS();
				m_pImmediateContext->DSSetShader(pD3DShader,NULL,0);
			}
			else
			{
				m_pImmediateContext->DSSetShader(NULL,0,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setGeometryShader(TLIRenderDeviceUsedGS* pGeometryShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pGeometryShader)
			{
				ID3D11GeometryShader* pD3DShader = ((TLRenderDeviceUsedGS_D3D11*)pGeometryShader)->getD3DGS();
				m_pImmediateContext->GSSetShader(pD3DShader,NULL,0);
			}
			else
			{
				m_pImmediateContext->GSSetShader(NULL,0,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setPixelShader(TLIRenderDeviceUsedPS* pPixelShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pPixelShader)
			{
				ID3D11PixelShader* pD3DShader = ((TLRenderDeviceUsedPS_D3D11*)pPixelShader)->getD3DPS();
				m_pImmediateContext->PSSetShader(pD3DShader,NULL,0);
			}
			else
			{
				m_pImmediateContext->PSSetShader(NULL,0,0);
			}
		}
	}

	void TLRenderDevice_D3D11::setComputeShader(TLIRenderDeviceUsedCS* pComputeShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pComputeShader)
			{
				ID3D11ComputeShader* pD3DShader = ((TLRenderDeviceUsedCS_D3D11*)pComputeShader)->getD3DCS();
				m_pImmediateContext->CSSetShader(pD3DShader,NULL,0);
			}
			else
			{
				m_pImmediateContext->CSSetShader(NULL,0,0);
			}
		}
	}

	TLIRenderDeviceUsedBuffer* TLRenderDevice_D3D11::createBuffer(const TLRenderDeviceBufferDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			// BufferDesc
			D3D11_BUFFER_DESC bd;
			bd.ByteWidth = pDesc->ByteWidth;
			bd.MiscFlags = 0;
			bd.CPUAccessFlags = 0;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = 0;
		
			if(pDesc->CPUAccessFlags)
			{
				bd.Usage = D3D11_USAGE_STAGING;
				bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				bd.BindFlags = pDesc->BindFlags;
			}

			// InitData
			D3D11_SUBRESOURCE_DATA data;
			if(pInitData)
			{
				data.pSysMem = pInitData->pSysMem;
			}

			// 构造
			ID3D11Buffer* pBuffer;
			HRESULT hr = m_pd3dDevice->CreateBuffer( &bd, pInitData ? &data : NULL, &pBuffer );
			if( FAILED( hr ) )
				return NULL;

			TLRenderDeviceUsedBuffer_D3D11* pRet = new TLRenderDeviceUsedBuffer_D3D11();
			if(!pRet->initWithD3DBuffer(pBuffer))
			{
				pBuffer->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedTex1D* TLRenderDevice_D3D11::createTex1D(const TLRenderDeviceTex1DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			// BufferDesc
			D3D11_TEXTURE1D_DESC desc;
			desc.Format = (DXGI_FORMAT)pDesc->Format;
			desc.ArraySize = pDesc->ArraySize;
			desc.MiscFlags = pDesc->GenerateMips?D3D11_RESOURCE_MISC_GENERATE_MIPS:0;
			desc.CPUAccessFlags = pDesc->CPUAccessFlags;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = 0;
			desc.MipLevels = pDesc->MipLevels;
			desc.Width = pDesc->Width;
		
			if(pDesc->CPUAccessFlags)
			{
				desc.Usage = D3D11_USAGE_STAGING;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				desc.BindFlags = pDesc->BindFlags;
			}

			// InitData
			D3D11_SUBRESOURCE_DATA* data = NULL;
			if (pInitData != NULL)
			{
				data = new D3D11_SUBRESOURCE_DATA[pDesc->ArraySize * pDesc->MipLevels];
				for (UINT j = 0; j < pDesc->ArraySize; ++ j)
				{
					for (UINT i = 0; i < pDesc->MipLevels; ++ i)
					{
						data[j * pDesc->MipLevels + i].pSysMem = pInitData[j * pDesc->MipLevels + i].pSysMem;
						data[j * pDesc->MipLevels + i].SysMemPitch = pInitData[j * pDesc->MipLevels + i].SysMemPitch;
						data[j * pDesc->MipLevels + i].SysMemSlicePitch = pInitData[j * pDesc->MipLevels + i].SysMemSlicePitch;
					}
				}
			}

			// 构造
			ID3D11Texture1D* d3dTex;
			HRESULT hr = m_pd3dDevice->CreateTexture1D(&desc,pInitData?data:0,&d3dTex);
			if(data)
			{
				delete [] data;
				data = NULL;
			}
			if( FAILED( hr ) )
				return NULL;

			TLRenderDeviceUsedTex1D_D3D11* pRet = new TLRenderDeviceUsedTex1D_D3D11();
			if(!pRet->initWithD3DTex1D(d3dTex))
			{
				d3dTex->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedTex2D* TLRenderDevice_D3D11::createTex2D(const TLRenderDeviceTex2DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			// BufferDesc
			D3D11_TEXTURE2D_DESC desc;
			desc.Format = (DXGI_FORMAT)pDesc->Format;
			desc.ArraySize = pDesc->ArraySize;
			desc.MiscFlags = pDesc->GenerateMips?D3D11_RESOURCE_MISC_GENERATE_MIPS:0;
			desc.CPUAccessFlags = pDesc->CPUAccessFlags;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = 0;
			desc.MipLevels = pDesc->MipLevels;
			desc.Width = pDesc->Width;
			desc.Height = pDesc->Height;
			desc.SampleDesc.Count = pDesc->SampleCount;
			desc.SampleDesc.Quality = pDesc->SampleQuality;
		
			if(pDesc->CPUAccessFlags)
			{
				desc.Usage = D3D11_USAGE_STAGING;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				desc.BindFlags = pDesc->BindFlags;
			}

			if(pDesc->Cube)
			{
				desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
			}

			// InitData
			D3D11_SUBRESOURCE_DATA* data = NULL;
			if (pInitData != NULL)
			{
				data = new D3D11_SUBRESOURCE_DATA[pDesc->ArraySize * pDesc->MipLevels];
				for (UINT j = 0; j < pDesc->ArraySize; ++ j)
				{
					for (UINT i = 0; i < pDesc->MipLevels; ++ i)
					{
						data[j * pDesc->MipLevels + i].pSysMem = pInitData[j * pDesc->MipLevels + i].pSysMem;
						data[j * pDesc->MipLevels + i].SysMemPitch = pInitData[j * pDesc->MipLevels + i].SysMemPitch;
						data[j * pDesc->MipLevels + i].SysMemSlicePitch = pInitData[j * pDesc->MipLevels + i].SysMemSlicePitch;
					}
				}
			}

			// 构造
			ID3D11Texture2D* d3dTex;
			HRESULT hr = m_pd3dDevice->CreateTexture2D(&desc,pInitData?data:0,&d3dTex);
			if(data)
			{
				delete [] data;
				data = NULL;
			}
			if( FAILED( hr ) )
				return NULL;

			TLRenderDeviceUsedTex2D_D3D11* pRet = new TLRenderDeviceUsedTex2D_D3D11();
			if(!pRet->initWithD3DTex2D(d3dTex))
			{
				d3dTex->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedTex3D* TLRenderDevice_D3D11::createTex3D(const TLRenderDeviceTex3DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			// BufferDesc
			D3D11_TEXTURE3D_DESC desc;
			desc.Format = (DXGI_FORMAT)pDesc->Format;
			desc.MiscFlags = pDesc->GenerateMips?D3D11_RESOURCE_MISC_GENERATE_MIPS:0;
			desc.CPUAccessFlags = pDesc->CPUAccessFlags;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = 0;
			desc.MipLevels = pDesc->MipLevels;
			desc.Width = pDesc->Width;
			desc.Height = pDesc->Height;
			desc.Depth = pDesc->Depth;
		
			if(pDesc->CPUAccessFlags)
			{
				desc.Usage = D3D11_USAGE_STAGING;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				desc.BindFlags = pDesc->BindFlags;
			}

			// InitData
			D3D11_SUBRESOURCE_DATA* data = NULL;
			if (pInitData != NULL)
			{
				data = new D3D11_SUBRESOURCE_DATA[1 * pDesc->MipLevels];
				for (UINT j = 0; j < 1; ++ j)
				{
					for (UINT i = 0; i < pDesc->MipLevels; ++ i)
					{
						data[j * pDesc->MipLevels + i].pSysMem = pInitData[j * pDesc->MipLevels + i].pSysMem;
						data[j * pDesc->MipLevels + i].SysMemPitch = pInitData[j * pDesc->MipLevels + i].SysMemPitch;
						data[j * pDesc->MipLevels + i].SysMemSlicePitch = pInitData[j * pDesc->MipLevels + i].SysMemSlicePitch;
					}
				}
			}

			// 构造
			ID3D11Texture3D* d3dTex;
			HRESULT hr = m_pd3dDevice->CreateTexture3D(&desc,pInitData?data:0,&d3dTex);
			if(data)
			{
				delete [] data;
				data = NULL;
			}
			if( FAILED( hr ) )
				return NULL;

			TLRenderDeviceUsedTex3D_D3D11* pRet = new TLRenderDeviceUsedTex3D_D3D11();
			if(!pRet->initWithD3DTex3D(d3dTex))
			{
				d3dTex->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedSRV* TLRenderDevice_D3D11::createShaderResourceView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceSRVDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pResource && pDesc)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = (DXGI_FORMAT)pDesc->Format;
			desc.ViewDimension = (D3D11_SRV_DIMENSION)pDesc->ViewDimension;
			switch(pDesc->ViewDimension)
			{
			case RENDER_DEVICE_SRV_DIMENSION_BUFFER:
				{
					desc.Buffer.NumElements = pDesc->Params[0];
					desc.Buffer.ElementWidth = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE1D:
				{
					desc.Texture1D.MostDetailedMip = pDesc->Params[0];
					desc.Texture1D.MipLevels = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE1DARRAY:
				{
					desc.Texture1DArray.MostDetailedMip = pDesc->Params[0];
					desc.Texture1DArray.MipLevels = pDesc->Params[1];
					desc.Texture1DArray.FirstArraySlice = pDesc->Params[2];
					desc.Texture1DArray.ArraySize = pDesc->Params[3];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE2D:
				{
					desc.Texture2D.MostDetailedMip = pDesc->Params[0];
					desc.Texture2D.MipLevels = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE2DARRAY:
				{
					desc.Texture2DArray.MostDetailedMip = pDesc->Params[0];
					desc.Texture2DArray.MipLevels = pDesc->Params[1];
					desc.Texture2DArray.FirstArraySlice = pDesc->Params[2];
					desc.Texture2DArray.ArraySize = pDesc->Params[3];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE2DMS:
				{
					desc.Texture2DMS.UnusedField_NothingToDefine = 0;
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE2DMSARRAY:
				{
					desc.Texture2DMSArray.FirstArraySlice = pDesc->Params[0];
					desc.Texture2DMSArray.ArraySize = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURE3D:
				{
					desc.Texture3D.MostDetailedMip = pDesc->Params[0];
					desc.Texture3D.MipLevels = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_SRV_DIMENSION_TEXTURECUBE:
				{
					desc.TextureCube.MostDetailedMip = pDesc->Params[0];
					desc.TextureCube.MipLevels = pDesc->Params[1];
				}
				break;
			default:
				{
					return NULL;
				}
				break;
			}

			TLIRenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pResource);
			ID3D11Resource* d3dResource = pD3DResource->getD3DResource();
			ID3D11ShaderResourceView* pView = NULL;
			if(FAILED(m_pd3dDevice->CreateShaderResourceView(d3dResource,&desc,&pView)))
			{
				return NULL;
			}

			TLRenderDeviceUsedSRV_D3D11* pRet = new TLRenderDeviceUsedSRV_D3D11();
			if(!pRet->initWithD3DSRV(pView))
			{
				pView->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedRTV* TLRenderDevice_D3D11::createRenderTargetView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceRTVDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pResource && pDesc)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			desc.Format = (DXGI_FORMAT)pDesc->Format;
			desc.ViewDimension = (D3D11_RTV_DIMENSION)pDesc->ViewDimension;
			switch(pDesc->ViewDimension)
			{
			case RENDER_DEVICE_RTV_DIMENSION_BUFFER:
				{
					desc.Buffer.NumElements = pDesc->Params[0];
					desc.Buffer.ElementWidth = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE1D:
				{
					desc.Texture1D.MipSlice = pDesc->Params[0];
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE1DARRAY:
				{
					desc.Texture1DArray.MipSlice = pDesc->Params[0];
					desc.Texture1DArray.FirstArraySlice = pDesc->Params[1];
					desc.Texture1DArray.ArraySize = pDesc->Params[2];
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE2D:
				{
					desc.Texture2D.MipSlice = pDesc->Params[0];
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE2DARRAY:
				{
					desc.Texture2DArray.MipSlice = pDesc->Params[0];
					desc.Texture2DArray.FirstArraySlice = pDesc->Params[1];
					desc.Texture2DArray.ArraySize = pDesc->Params[2];
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE2DMS:
				{
					desc.Texture2DMS.UnusedField_NothingToDefine = 0;
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE2DMSARRAY:
				{
					desc.Texture2DMSArray.FirstArraySlice = pDesc->Params[0];
					desc.Texture2DMSArray.ArraySize = pDesc->Params[1];
				}
				break;
			case RENDER_DEVICE_RTV_DIMENSION_TEXTURE3D:
				{
					desc.Texture3D.MipSlice = pDesc->Params[0];
					desc.Texture3D.FirstWSlice = pDesc->Params[1];
					desc.Texture3D.WSize = pDesc->Params[2];
				}
				break;
			default:
				{
					return NULL;
				}
				break;
			}

			TLIRenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pResource);
			ID3D11Resource* d3dResource = pD3DResource->getD3DResource();
			ID3D11RenderTargetView* pView = NULL;
			if(FAILED(m_pd3dDevice->CreateRenderTargetView(d3dResource,&desc,&pView)))
			{
				return NULL;
			}

			TLRenderDeviceUsedRTV_D3D11* pRet = new TLRenderDeviceUsedRTV_D3D11();
			if(!pRet->initWithD3DRTV(pView))
			{
				pView->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedDSV* TLRenderDevice_D3D11::createDepthStencilView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceDSVDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pResource && pDesc)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC desc;
			desc.Format = (DXGI_FORMAT)pDesc->Format;
			desc.ViewDimension = (D3D11_DSV_DIMENSION)pDesc->ViewDimension;
			desc.Flags = 0;
			switch(pDesc->ViewDimension)
			{
			case RENDER_DEVICE_DSV_DIMENSION_TEXTURE1D:
				{
					desc.Texture1D.MipSlice = pDesc->Params[0];
				}
				break;
			case RENDER_DEVICE_DSV_DIMENSION_TEXTURE1DARRAY:
				{
					desc.Texture1DArray.MipSlice = pDesc->Params[0];
					desc.Texture1DArray.FirstArraySlice = pDesc->Params[1];
					desc.Texture1DArray.ArraySize = pDesc->Params[2];
				}
				break;
			case RENDER_DEVICE_DSV_DIMENSION_TEXTURE2D:
				{
					desc.Texture2D.MipSlice = pDesc->Params[0];
				}
				break;
			case RENDER_DEVICE_DSV_DIMENSION_TEXTURE2DARRAY:
				{
					desc.Texture2DArray.MipSlice = pDesc->Params[0];
					desc.Texture2DArray.FirstArraySlice = pDesc->Params[1];
					desc.Texture2DArray.ArraySize = pDesc->Params[2];
				}
				break;
			case RENDER_DEVICE_DSV_DIMENSION_TEXTURE2DMS:
				{
					desc.Texture2DMS.UnusedField_NothingToDefine = 0;
				}
				break;
			case RENDER_DEVICE_DSV_DIMENSION_TEXTURE2DMSARRAY:
				{
					desc.Texture2DMSArray.FirstArraySlice = pDesc->Params[0];
					desc.Texture2DMSArray.ArraySize = pDesc->Params[1];
				}
				break;
			default:
				{
					return NULL;
				}
				break;
			}

			TLIRenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<TLIRenderDeviceUsedResource_D3D11*>(pResource);
			ID3D11Resource* d3dResource = pD3DResource->getD3DResource();
			ID3D11DepthStencilView* pView = NULL;
			if(FAILED(m_pd3dDevice->CreateDepthStencilView(d3dResource,&desc,&pView)))
			{
				return NULL;
			}

			TLRenderDeviceUsedDSV_D3D11* pRet = new TLRenderDeviceUsedDSV_D3D11();
			if(!pRet->initWithD3DDSV(pView))
			{
				pView->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedBlendState* TLRenderDevice_D3D11::createBlendState(const TLRenderDeviceBlendDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_BLEND_DESC desc;
			desc.AlphaToCoverageEnable = FALSE;
			desc.IndependentBlendEnable = FALSE;
			for(UINT i=0;i<8;i++)
			{
				desc.RenderTarget[i].BlendEnable = FALSE;
			}
			desc.RenderTarget[0].BlendEnable = pDesc->RenderTarget[0].BlendEnable;
			desc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)pDesc->RenderTarget[0].BlendOp;
			desc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)pDesc->RenderTarget[0].BlendOpAlpha;
			desc.RenderTarget[0].DestBlend = (D3D11_BLEND)pDesc->RenderTarget[0].DestBlend;
			desc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)pDesc->RenderTarget[0].DestBlendAlpha;
			desc.RenderTarget[0].SrcBlend = (D3D11_BLEND)pDesc->RenderTarget[0].SrcBlend;
			desc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)pDesc->RenderTarget[0].SrcBlendAlpha;
			desc.RenderTarget[0].RenderTargetWriteMask = pDesc->RenderTarget[0].RenderTargetWriteMask;

			ID3D11BlendState* pState = NULL;
			if(FAILED(m_pd3dDevice->CreateBlendState(&desc,&pState)))
			{
				return NULL;
			}
			
			TLRenderDeviceUsedBlendState_D3D11* pRet = new TLRenderDeviceUsedBlendState_D3D11();
			if(!pRet->initWithD3DBlendState(pState))
			{
				pState->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedDepthStencilState* TLRenderDevice_D3D11::createDepthStencilState(const TLRenderDeviceDepthStencilDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = pDesc->DepthEnable;
			desc.DepthFunc = (D3D11_COMPARISON_FUNC)pDesc->DepthFunc;
			desc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)pDesc->DepthWriteMask;
			desc.StencilEnable = FALSE;

			ID3D11DepthStencilState* pState = NULL;
			if(FAILED(m_pd3dDevice->CreateDepthStencilState(&desc,&pState)))
			{
				return NULL;
			}
			
			TLRenderDeviceUsedDepthStencilState_D3D11* pRet = new TLRenderDeviceUsedDepthStencilState_D3D11();
			if(!pRet->initWithD3DDepthStencilState(pState))
			{
				pState->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedRasterizerState* TLRenderDevice_D3D11::createRasterizerState(const TLRenderDeviceRasterizerDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_RASTERIZER_DESC desc;
			desc.CullMode = (D3D11_CULL_MODE)pDesc->CullMode;
			desc.FillMode = (D3D11_FILL_MODE)pDesc->FillMode;
			desc.AntialiasedLineEnable = pDesc->AntialiasedLineEnable;
			desc.MultisampleEnable = pDesc->MultisampleEnable;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0;
			desc.DepthClipEnable = TRUE;
			desc.FrontCounterClockwise = FALSE;
			desc.ScissorEnable = FALSE;
			desc.SlopeScaledDepthBias = 0;

			ID3D11RasterizerState* pState = NULL;
			if(FAILED(m_pd3dDevice->CreateRasterizerState(&desc,&pState)))
			{
				return NULL;
			}
			
			TLRenderDeviceUsedRasterizerState_D3D11* pRet = new TLRenderDeviceUsedRasterizerState_D3D11();
			if(!pRet->initWithD3DRasterizerState(pState))
			{
				pState->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedSamplerState* TLRenderDevice_D3D11::createSamplerState(const TLRenderDeviceSamplerDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_SAMPLER_DESC desc;
			desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)pDesc->AddressU;
			desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)pDesc->AddressV;
			desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)pDesc->AddressW;
			memcpy(desc.BorderColor,pDesc->BorderColor,sizeof(FLOAT)*4);
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			desc.Filter = (D3D11_FILTER)pDesc->Filter;
			desc.MaxAnisotropy = pDesc->MaxAnisotropy;
			desc.MaxLOD = pDesc->MaxLOD;
			desc.MinLOD = pDesc->MinLOD;
			desc.MipLODBias = pDesc->MipLODBias;

			ID3D11SamplerState* pState = NULL;
			if(FAILED(m_pd3dDevice->CreateSamplerState(&desc,&pState)))
			{
				return NULL;
			}
			
			TLRenderDeviceUsedSamplerState_D3D11* pRet = new TLRenderDeviceUsedSamplerState_D3D11();
			if(!pRet->initWithD3DSamplerState(pState))
			{
				pState->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceCompiledShader* TLRenderDevice_D3D11::createCompiledShader()
	{
		return new TLRenderDeviceCompiledShader_D3D11();
	}

	TLIRenderDeviceUsedInputLayout* TLRenderDevice_D3D11::createInputLayout(const TLRenderDeviceInputElementDesc* pElementDescs, 
			UINT numElements, TLIRenderDeviceCompiledShader* pBlob)
	{
		if(m_loaded && m_pd3dDevice && pElementDescs && numElements>0 && pBlob)
		{
			D3D11_INPUT_ELEMENT_DESC* pDesc = new D3D11_INPUT_ELEMENT_DESC[numElements];
			for(UINT i=0;i<numElements;i++)
			{
				pDesc[i].AlignedByteOffset = pElementDescs[i].AlignedByteOffset;
				pDesc[i].Format = (DXGI_FORMAT)pElementDescs[i].Format;
				pDesc[i].InputSlot = pElementDescs[i].InputSlot;
				pDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				pDesc[i].InstanceDataStepRate = 0;
				pDesc[i].SemanticIndex = pElementDescs[i].SemanticIndex;
				pDesc[i].SemanticName = pElementDescs[i].SemanticName.GetString();
			}

			ID3D11InputLayout* pInputLayout = NULL;
			if(FAILED(m_pd3dDevice->CreateInputLayout(pDesc,numElements,pBlob->getBufferPointer(),pBlob->getBufferSize(),&pInputLayout)))
			{
				delete [] pDesc;
				return NULL;
			}
			delete [] pDesc;

			TLRenderDeviceUsedInputLayout_D3D11* pRet = new TLRenderDeviceUsedInputLayout_D3D11();
			if(!pRet->initWithD3DInputLayout(pInputLayout))
			{
				pInputLayout->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedVS* TLRenderDevice_D3D11::createVertexShader(TLIRenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11VertexShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreateVertexShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),NULL,&pShader)))
			{
				return NULL;
			}

			TLRenderDeviceUsedVS_D3D11* pRet = new TLRenderDeviceUsedVS_D3D11();
			if(!pRet->initWithD3DVS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedHS* TLRenderDevice_D3D11::createHullShader(TLIRenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11HullShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreateHullShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),NULL,&pShader)))
			{
				return NULL;
			}

			TLRenderDeviceUsedHS_D3D11* pRet = new TLRenderDeviceUsedHS_D3D11();
			if(!pRet->initWithD3DHS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedDS* TLRenderDevice_D3D11::createDomainShader(TLIRenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11DomainShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreateDomainShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),NULL,&pShader)))
			{
				return NULL;
			}

			TLRenderDeviceUsedDS_D3D11* pRet = new TLRenderDeviceUsedDS_D3D11();
			if(!pRet->initWithD3DDS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedGS* TLRenderDevice_D3D11::createGeometryShader(TLIRenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11GeometryShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreateGeometryShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),NULL,&pShader)))
			{
				return NULL;
			}

			TLRenderDeviceUsedGS_D3D11* pRet = new TLRenderDeviceUsedGS_D3D11();
			if(!pRet->initWithD3DGS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedGS* TLRenderDevice_D3D11::createGeometryShaderWithStreamOutput(TLIRenderDeviceCompiledShader* pCompiledShader, 
			const TLRenderDeviceSODeclarationEntry* pSODeclaration, UINT numEntries)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader && pSODeclaration && numEntries>0)
		{
			D3D11_SO_DECLARATION_ENTRY* pEntry = new D3D11_SO_DECLARATION_ENTRY[numEntries];
			for(UINT i=0;i<numEntries;i++)
			{
				pEntry[i].ComponentCount = pSODeclaration[i].ComponentCount;
				pEntry[i].OutputSlot = 0;
				pEntry[i].SemanticIndex = pSODeclaration[i].SemanticIndex;
				pEntry[i].SemanticName = pSODeclaration[i].SemanticName.GetString();
				pEntry[i].StartComponent = pSODeclaration[i].StartComponent;
				pEntry[i].Stream = 0;
			}

			ID3D11GeometryShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreateGeometryShaderWithStreamOutput(pCompiledShader->getBufferPointer(),
				pCompiledShader->getBufferSize(),pEntry,numEntries,NULL,0,0,NULL,&pShader)))
			{
				delete [] pEntry;
				return NULL;
			}
			delete [] pEntry;

			TLRenderDeviceUsedGS_D3D11* pRet = new TLRenderDeviceUsedGS_D3D11();
			if(!pRet->initWithD3DGS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedPS* TLRenderDevice_D3D11::createPixelShader(TLIRenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11PixelShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreatePixelShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),NULL,&pShader)))
			{
				return NULL;
			}

			TLRenderDeviceUsedPS_D3D11* pRet = new TLRenderDeviceUsedPS_D3D11();
			if(!pRet->initWithD3DPS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	TLIRenderDeviceUsedCS* TLRenderDevice_D3D11::createComputeShader(TLIRenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11ComputeShader* pShader = NULL;
			if(FAILED(m_pd3dDevice->CreateComputeShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),NULL,&pShader)))
			{
				return NULL;
			}

			TLRenderDeviceUsedCS_D3D11* pRet = new TLRenderDeviceUsedCS_D3D11();
			if(!pRet->initWithD3DCS(pShader))
			{
				pShader->Release();
				delete pRet;
				return NULL;
			}
			return pRet;
		}
		return NULL;
	}

	bool TLRenderDevice_D3D11::createTex2DFromDDSFile(const char* file,TLIRenderDeviceUsedTex2D** ppTex2D,TLIRenderDeviceUsedSRV** ppSRV)
	{
		if(m_pd3dDevice && m_loaded)
		{
			TString strFile(file);
			ID3D11Texture2D* d3dTex = NULL;
			ID3D11ShaderResourceView* d3dSRV = NULL;
			HRESULT hr = CreateDDSTextureFromFile( m_pd3dDevice, strFile.GetWString().get(), (ID3D11Resource**)&d3dTex, &d3dSRV );
			if( FAILED( hr ) )
				return false;
			if(ppTex2D)
			{
				*ppTex2D = new TLRenderDeviceUsedTex2D_D3D11();
				((TLRenderDeviceUsedTex2D_D3D11*)(*ppTex2D))->initWithD3DTex2D(d3dTex);
			}
			else
			{
				d3dTex->Release();
			}
			if(ppSRV)
			{
				*ppSRV = new TLRenderDeviceUsedSRV_D3D11();
				((TLRenderDeviceUsedSRV_D3D11*)(*ppSRV))->initWithD3DSRV(d3dSRV);
			}
			else
			{
				d3dSRV->Release();
			}
			return true;
		}
		return false;
	}
}