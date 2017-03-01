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
#include "TLMemDef.h"

namespace TLunaEngine {
	RenderDevice_D3D11::RenderDevice_D3D11() : RenderDevice(), m_pd3dDevice(TSun::TNULL),m_pImmediateContext(TSun::TNULL),
		m_pSwapChain(TSun::TNULL),m_pRenderTargetView(TSun::TNULL),m_pDepthStencil(TSun::TNULL),m_pDepthStencilView(TSun::TNULL),m_loaded(TSun::TFALSE)
	{
	}

	RenderDevice_D3D11::~RenderDevice_D3D11()
	{
	}

	TSun::TBOOL RenderDevice_D3D11::InitDevice(HWND hWnd,TSun::TBOOL bWnd,TSun::TS32 width,TSun::TS32 height)
	{
		if(m_loaded)
			return TSun::TFALSE;
		HRESULT hr = S_OK;
		TSun::TU32 createDeviceFlags = 0;
#ifdef _DEBUG
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		TSun::TU32 numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			//D3D_FEATURE_LEVEL_9_3,
			//D3D_FEATURE_LEVEL_9_2,
			//D3D_FEATURE_LEVEL_9_1,
		};
		TSun::TU32 numFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = (TSun::TU32)width;
		sd.BufferDesc.Height = (TSun::TU32)height;
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

		for( TSun::TU32 driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			m_DriverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( TSun::TNULL, m_DriverType, TSun::TNULL, createDeviceFlags, featureLevels, numFeatureLevels,
												D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		if( FAILED( hr ) )
			return TSun::TFALSE;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = TSun::TNULL;
		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if( FAILED( hr ) )
			return TSun::TFALSE;

		hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, TSun::TNULL, &m_pRenderTargetView );
		pBackBuffer->Release();
		if( FAILED( hr ) )
			return TSun::TFALSE;

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = (TSun::TU32)width;
		descDepth.Height = (TSun::TU32)height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = m_pd3dDevice->CreateTexture2D( &descDepth, TSun::TNULL, &m_pDepthStencil );
		if( FAILED( hr ) )
			return TSun::TFALSE;

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_pd3dDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
		if( FAILED( hr ) )
			return TSun::TFALSE;

		m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (TSun::TF32)width;
		vp.Height = (TSun::TF32)height;
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
		m_loaded = TSun::TTRUE;
		return TSun::TTRUE;
	}

	TSun::TVOID RenderDevice_D3D11::DestroyDevice()
	{
		if(!m_loaded)
			return;
		m_loaded = TSun::TFALSE;
		if(m_pDepthStencil)
		{
			m_pDepthStencil->Release();
			m_pDepthStencil = TSun::TNULL;
		}
		if(m_pDepthStencilView)
		{
			m_pDepthStencilView->Release();
			m_pDepthStencilView = TSun::TNULL;
		}
		if(m_pRenderTargetView)
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = TSun::TNULL;
		}
		if(m_pSwapChain)
		{
			m_pSwapChain->Release();
			m_pSwapChain = TSun::TNULL;
		}
		if(m_pImmediateContext)
		{
			m_pImmediateContext->Release();
			m_pImmediateContext = TSun::TNULL;
		}
		if(m_pd3dDevice)
		{
			m_pd3dDevice->Release();
			m_pd3dDevice = TSun::TNULL;
		}
	}

	TSun::TBOOL RenderDevice_D3D11::BeginRender()
	{
		if(m_pImmediateContext && m_pRenderTargetView && m_pDepthStencilView && m_loaded)
		{
			//
			// Clear the back buffer
			//
			TSun::TF32 ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha
			m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

			//
			// Clear the depth buffer to 1.0 (max depth)
			//
			m_pImmediateContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		}
		return TSun::TTRUE;
	}

	TSun::TVOID RenderDevice_D3D11::EndRender()
	{
		if(m_pSwapChain && m_loaded)
		{
			m_pSwapChain->Present( 0, 0 );
		}
	}

	TSun::TVOID RenderDevice_D3D11::UseDefaultRenderTarget()
	{
		if(m_pImmediateContext && m_pRenderTargetView && m_pDepthStencilView && m_loaded)
		{
			m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );
		}
	}

	TSun::TVOID RenderDevice_D3D11::UseDefaultViewPort()
	{
		if(m_pImmediateContext && m_loaded)
		{
			useViewport(&mDefaultVp,1);
		}
	}

	TSun::TU32 RenderDevice_D3D11::GetBufferWidth()
	{
		if(m_pSwapChain && m_loaded)
		{
			DXGI_SWAP_CHAIN_DESC desc;
			m_pSwapChain->GetDesc(&desc);
			return desc.BufferDesc.Width;
		}
		return 0;
	}

	TSun::TU32 RenderDevice_D3D11::GetBufferHeight()
	{
		if(m_pSwapChain && m_loaded)
		{
			DXGI_SWAP_CHAIN_DESC desc;
			m_pSwapChain->GetDesc(&desc);
			return desc.BufferDesc.Height;
		}
		return 0;
	}

	TSun::TBOOL RenderDevice_D3D11::resizeBuffer(TSun::TU32 width,TSun::TU32 height)
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
			HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D),(TSun::TVOID**) &pBuffer );
			// Perform error handling here!
			hr = m_pd3dDevice->CreateRenderTargetView(pBuffer, TSun::TNULL, &m_pRenderTargetView);
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
			hr = m_pd3dDevice->CreateTexture2D( &descDepth, TSun::TNULL, &m_pDepthStencil );
			if( FAILED( hr ) )
				return TSun::TFALSE;
			// Create the depth stencil view
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory( &descDSV, sizeof(descDSV) );
			descDSV.Format = descDepth.Format;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			hr = m_pd3dDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
			if( FAILED( hr ) )
				return TSun::TFALSE;

			m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

			// Setup the viewport
			mDefaultVp._Width = (TSun::TF32)width;
			mDefaultVp._Height = (TSun::TF32)height;
			mDefaultVp._MinDepth = 0.0f;
			mDefaultVp._MaxDepth = 1.0f;
			mDefaultVp._TopLeftX = 0;
			mDefaultVp._TopLeftY = 0;
			useViewport(&mDefaultVp,1);

			// Initialize the projection matrix
			//g_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, (TSun::TF32)width / (TSun::TF32)height, 0.01f, 100.0f );
			//CBChangeOnResize cbChangesOnResize;
			//cbChangesOnResize.mProjection = XMMatrixTranspose( g_Projection );
			//g_pImmediateContext->UpdateSubresource( g_pCBChangeOnResize, 0, TSun::TNULL, &cbChangesOnResize, 0, 0 );
		}
		return TSun::TTRUE;
	}

	TSun::TBOOL RenderDevice_D3D11::useViewport(const TLRenderDeviceViewport* aVp,TSun::TU32 numVp)
	{
		if(!aVp || numVp<1 || !m_loaded || !m_pImmediateContext)
			return TSun::TFALSE;
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
			D3D11_VIEWPORT* theVp = T_NEW_ARRAY(getRenderStructMemAllocator(), D3D11_VIEWPORT, numVp);
			for(TSun::TU32 i=0;i<numVp;i++)
			{
				theVp[i].Height = aVp[i]._Height;
				theVp[i].MaxDepth = aVp[i]._MaxDepth;
				theVp[i].MinDepth = aVp[i]._MinDepth;
				theVp[i].TopLeftX = aVp[i]._TopLeftX;
				theVp[i].TopLeftY = aVp[i]._TopLeftY;
				theVp[i].Width = aVp[i]._Width;
			}
			m_pImmediateContext->RSSetViewports( numVp, theVp );
			T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_VIEWPORT, theVp);
			theVp = TSun::TNULL;
		}
		return TSun::TTRUE;
	}

	TSun::TVOID RenderDevice_D3D11::setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY pt)
	{
		if(m_loaded && m_pImmediateContext)
		{
			m_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pt);
		}
	}

	TSun::TVOID RenderDevice_D3D11::setInputLayout(RenderDeviceUsedInputLayout* pInputLayout)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pInputLayout)
			{
				ID3D11InputLayout* d3dLayout = ((RenderDeviceUsedInputLayout_D3D11*)pInputLayout)->getD3DInputLayout();
				m_pImmediateContext->IASetInputLayout(d3dLayout);
			}
			else
			{
				m_pImmediateContext->IASetInputLayout(TSun::TNULL);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::getD3DDriverType(RENDER_DEVICE_D3D_DRIVER_TYPE* pDriverType)
	{
		if(pDriverType)
		{
			(*pDriverType) = (RENDER_DEVICE_D3D_DRIVER_TYPE)m_DriverType;
		}
	}

	TSun::TVOID RenderDevice_D3D11::getD3DFeatureLevel(RENDER_DEVICE_D3D_FEATURE_LEVEL* pFeatureLevel)
	{
		if(pFeatureLevel)
		{
			(*pFeatureLevel) = (RENDER_DEVICE_D3D_FEATURE_LEVEL)m_FeatureLevel;
		}
	}

	const TSun::TCHAR* RenderDevice_D3D11::getShaderProfileVersion()
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

	TSun::TVOID RenderDevice_D3D11::draw(TSun::TU32 vertexCount,TSun::TU32 startVertexLocation)
	{
		if(m_loaded && m_pImmediateContext)
		{
			m_pImmediateContext->Draw(vertexCount,startVertexLocation);
		}
	}

	TSun::TVOID RenderDevice_D3D11::drawIndexed(TSun::TU32 indexCount,TSun::TU32 startIndexLocation,TSun::TU32 baseVertexLocation)
	{
		if(m_loaded && m_pImmediateContext)
		{
			m_pImmediateContext->DrawIndexed(indexCount,startIndexLocation,baseVertexLocation);
		}
	}

	TSun::TU32 RenderDevice_D3D11::calcSubresource(TSun::TU32 mipSlice,TSun::TU32 arraySlice,TSun::TU32 mipLevels)
	{
		return D3D11CalcSubresource(mipSlice,arraySlice,mipLevels);
	}

	TSun::TVOID RenderDevice_D3D11::generateMips(RenderDeviceUsedSRV* pSRV)
	{
		if(m_loaded && m_pImmediateContext && pSRV)
		{
			ID3D11ShaderResourceView* pView = ((RenderDeviceUsedSRV_D3D11*)pSRV)->getD3DSRV();
			m_pImmediateContext->GenerateMips(pView);
		}
	}

	TSun::TBOOL RenderDevice_D3D11::mapResource(RenderDeviceUsedResource* pResource,TSun::TU32 subResource,
			RENDER_DEVICE_MAP mapType,TLRenderDeviceMappedSubresource* mappedSubresource)
	{
		if(m_loaded && m_pImmediateContext && pResource && mappedSubresource)
		{
			RenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pResource);
			D3D11_MAPPED_SUBRESOURCE mapped;
			if(FAILED(m_pImmediateContext->Map(pD3DResource->getD3DResource(),subResource,(D3D11_MAP)mapType,0,&mapped)))
				return TSun::TFALSE;
			mappedSubresource->DepthPitch = mapped.DepthPitch;
			mappedSubresource->pData = mapped.pData;
			mappedSubresource->RowPitch = mapped.RowPitch;
			return TSun::TTRUE;
		}
		return TSun::TFALSE;
	}

	TSun::TVOID RenderDevice_D3D11::unmapResource(RenderDeviceUsedResource* pResource,TSun::TU32 subResource)
	{
		if(m_loaded && m_pImmediateContext && pResource)
		{
			RenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pResource);
			m_pImmediateContext->Unmap(pD3DResource->getD3DResource(),subResource);
		}
	}

	TSun::TVOID RenderDevice_D3D11::copySubresourceRegion(RenderDeviceUsedResource* pDstResource,TSun::TU32 dstSubresource,
			TSun::TU32 dstX,TSun::TU32 dstY,TSun::TU32 dstZ,RenderDeviceUsedResource* pSrcResource,TSun::TU32 srcSubresource,
			const TLRenderDeviceBox* pSrcBox)
	{
		if(m_loaded && m_pImmediateContext && pDstResource && pSrcResource)
		{
			RenderDeviceUsedResource_D3D11* pD3DSrcResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pSrcResource);
			RenderDeviceUsedResource_D3D11* pD3DDstResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pDstResource);
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
				dstX,dstY,dstZ,pD3DSrcResource->getD3DResource(),srcSubresource,TSun::TNULL);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::copyResource(RenderDeviceUsedResource* pDstResource,RenderDeviceUsedResource* pSrcResource)
	{
		if(m_loaded && m_pImmediateContext && pDstResource && pSrcResource)
		{
			RenderDeviceUsedResource_D3D11* pD3DSrcResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pSrcResource);
			RenderDeviceUsedResource_D3D11* pD3DDstResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pDstResource);
			m_pImmediateContext->CopyResource(pD3DDstResource->getD3DResource(),pD3DSrcResource->getD3DResource());
		}
	}

	TSun::TVOID RenderDevice_D3D11::updateSubresource(RenderDeviceUsedResource* pDstResource,TSun::TU32 dstSubresource,
			const TLRenderDeviceBox* pDstBox,const TSun::TVOID* pSrcData,TSun::TU32 srcRowPitch,TSun::TU32 srcDepthPitch)
	{
		if(m_loaded && m_pImmediateContext && pDstResource && pSrcData)
		{
			RenderDeviceUsedResource_D3D11* pD3DDstResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pDstResource);
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
				m_pImmediateContext->UpdateSubresource(pD3DDstResource->getD3DResource(),dstSubresource,TSun::TNULL,pSrcData,srcRowPitch,srcDepthPitch);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setVertexBuffer(TSun::TU32 startSlot,RenderDeviceUsedBuffer* pBuffer, const TSun::TU32* pStride,const TSun::TU32* pOffset)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBuffer && pStride && pOffset)
			{
				ID3D11Buffer* pD3DBuffer = ((RenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
				m_pImmediateContext->IASetVertexBuffers(startSlot,1,&pD3DBuffer,pStride,pOffset);
			}
			else
			{
				m_pImmediateContext->IASetVertexBuffers(startSlot,0,TSun::TNULL,TSun::TNULL,TSun::TNULL);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setIndexBuffer(RenderDeviceUsedBuffer* pBuffer,RENDER_DEVICE_FORMAT format,TSun::TU32 offset)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBuffer)
			{
				ID3D11Buffer* pD3DBuffer = ((RenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
				m_pImmediateContext->IASetIndexBuffer(pD3DBuffer,(DXGI_FORMAT)format,offset);
			}
			else
			{
				m_pImmediateContext->IASetIndexBuffer(TSun::TNULL,(DXGI_FORMAT)format,offset);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setSOTarget(RenderDeviceUsedBuffer* pBuffer,const TSun::TU32* pOffset)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBuffer && pOffset)
			{
				ID3D11Buffer* pD3DBuffer = ((RenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
				m_pImmediateContext->SOSetTargets(1,&pD3DBuffer,pOffset);
			}
			else
			{
				m_pImmediateContext->SOSetTargets(0,TSun::TNULL,TSun::TNULL);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::clearRenderTargetView(RenderDeviceUsedRTV* pRenderTargetView, const TSun::TF32 colorRGBA[4])
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pRenderTargetView)
			{
				ID3D11RenderTargetView* pD3DView = ((RenderDeviceUsedRTV_D3D11*)pRenderTargetView)->getD3DRTV();
				m_pImmediateContext->ClearRenderTargetView(pD3DView,colorRGBA);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::clearDepthStencilView(RenderDeviceUsedDSV* pDepthStencilView, TSun::TF32 depth)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pDepthStencilView)
			{
				ID3D11DepthStencilView* pD3DView = ((RenderDeviceUsedDSV_D3D11*)pDepthStencilView)->getD3DDSV();
				m_pImmediateContext->ClearDepthStencilView(pD3DView,D3D11_CLEAR_DEPTH,depth,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setRenderTarget(RenderDeviceUsedRTV* pRTV, RenderDeviceUsedDSV* pDSV)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11RenderTargetView* pD3DRTV = TSun::TNULL;
			TSun::TU32 numRTV = 0;
			ID3D11DepthStencilView* pD3DDSV = TSun::TNULL;
			if(pRTV)
			{
				pD3DRTV = ((RenderDeviceUsedRTV_D3D11*)pRTV)->getD3DRTV();
				numRTV = 1;
			}
			if(pDSV)
			{
				pD3DDSV = ((RenderDeviceUsedDSV_D3D11*)pDSV)->getD3DDSV();
			}
			if(numRTV > 0)
			{
				m_pImmediateContext->OMSetRenderTargets(numRTV,&pD3DRTV,pD3DDSV);
			}
			else
			{
				m_pImmediateContext->OMSetRenderTargets(0,TSun::TNULL,pD3DDSV);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setRasterizerState(RenderDeviceUsedRasterizerState* pRasterizerState)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pRasterizerState)
			{
				ID3D11RasterizerState* pD3DState = ((RenderDeviceUsedRasterizerState_D3D11*)pRasterizerState)->getD3DRasterizerState();
				m_pImmediateContext->RSSetState(pD3DState);
			}
			else
			{
				m_pImmediateContext->RSSetState(TSun::TNULL);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setDepthStencilState(RenderDeviceUsedDepthStencilState* pDepthStencilState)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pDepthStencilState)
			{
				ID3D11DepthStencilState* pD3DState = ((RenderDeviceUsedDepthStencilState_D3D11*)pDepthStencilState)->getD3DDepthStencilState();
				m_pImmediateContext->OMSetDepthStencilState(pD3DState,0);
			}
			else
			{
				m_pImmediateContext->OMSetDepthStencilState(TSun::TNULL,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setBlendState(RenderDeviceUsedBlendState* pBlendState,const TSun::TF32 blendFactor[4],TSun::TU32 sampleMask)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pBlendState)
			{
				ID3D11BlendState* pD3DState = ((RenderDeviceUsedBlendState_D3D11*)pBlendState)->getD3DBlendState();
				m_pImmediateContext->OMSetBlendState(pD3DState,blendFactor,sampleMask);
			}
			else
			{
				m_pImmediateContext->OMSetBlendState(TSun::TNULL,TSun::TNULL,0xffffffff);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setConstantBuffer(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TSun::TU32 startSlot, RenderDeviceUsedBuffer* pBuffer)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11Buffer* pD3DBuffer = TSun::TNULL;
			if(pBuffer)
			{
				pD3DBuffer = ((RenderDeviceUsedBuffer_D3D11*)pBuffer)->getD3DBuffer();
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
						m_pImmediateContext->VSSetConstantBuffers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->HSSetConstantBuffers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->DSSetConstantBuffers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->GSSetConstantBuffers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->PSSetConstantBuffers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->CSSetConstantBuffers(startSlot,0,TSun::TNULL);
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

	TSun::TVOID RenderDevice_D3D11::setSamplerState(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TSun::TU32 startSlot, RenderDeviceUsedSamplerState* pSampler)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11SamplerState* pD3DState = TSun::TNULL;
			if(pSampler)
			{
				pD3DState = ((RenderDeviceUsedSamplerState_D3D11*)pSampler)->getD3DSamplerState();
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
						m_pImmediateContext->VSSetSamplers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->HSSetSamplers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->DSSetSamplers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->GSSetSamplers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->PSSetSamplers(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->CSSetSamplers(startSlot,0,TSun::TNULL);
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

	TSun::TVOID RenderDevice_D3D11::setShaderResourceView(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TSun::TU32 startSlot, RenderDeviceUsedSRV* pSRV)
	{
		if(m_loaded && m_pImmediateContext)
		{
			ID3D11ShaderResourceView* pD3DView = TSun::TNULL;
			if(pSRV)
			{
				pD3DView = ((RenderDeviceUsedSRV_D3D11*)pSRV)->getD3DSRV();
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
						m_pImmediateContext->VSSetShaderResources(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->HSSetShaderResources(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->DSSetShaderResources(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->GSSetShaderResources(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->PSSetShaderResources(startSlot,0,TSun::TNULL);
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
						m_pImmediateContext->CSSetShaderResources(startSlot,0,TSun::TNULL);
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

	TSun::TVOID RenderDevice_D3D11::setVertexShader(RenderDeviceUsedVS* pVertexShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pVertexShader)
			{
				ID3D11VertexShader* pD3DShader = ((RenderDeviceUsedVS_D3D11*)pVertexShader)->getD3DVS();
				m_pImmediateContext->VSSetShader(pD3DShader,TSun::TNULL,0);
			}
			else
			{
				m_pImmediateContext->VSSetShader(TSun::TNULL,0,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setHullShader(RenderDeviceUsedHS* pHullShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pHullShader)
			{
				ID3D11HullShader* pD3DShader = ((RenderDeviceUsedHS_D3D11*)pHullShader)->getD3DHS();
				m_pImmediateContext->HSSetShader(pD3DShader,TSun::TNULL,0);
			}
			else
			{
				m_pImmediateContext->HSSetShader(TSun::TNULL,0,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setDomainShader(RenderDeviceUsedDS* pDomainShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pDomainShader)
			{
				ID3D11DomainShader* pD3DShader = ((RenderDeviceUsedDS_D3D11*)pDomainShader)->getD3DDS();
				m_pImmediateContext->DSSetShader(pD3DShader,TSun::TNULL,0);
			}
			else
			{
				m_pImmediateContext->DSSetShader(TSun::TNULL,0,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setGeometryShader(RenderDeviceUsedGS* pGeometryShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pGeometryShader)
			{
				ID3D11GeometryShader* pD3DShader = ((RenderDeviceUsedGS_D3D11*)pGeometryShader)->getD3DGS();
				m_pImmediateContext->GSSetShader(pD3DShader,TSun::TNULL,0);
			}
			else
			{
				m_pImmediateContext->GSSetShader(TSun::TNULL,0,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setPixelShader(RenderDeviceUsedPS* pPixelShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pPixelShader)
			{
				ID3D11PixelShader* pD3DShader = ((RenderDeviceUsedPS_D3D11*)pPixelShader)->getD3DPS();
				m_pImmediateContext->PSSetShader(pD3DShader,TSun::TNULL,0);
			}
			else
			{
				m_pImmediateContext->PSSetShader(TSun::TNULL,0,0);
			}
		}
	}

	TSun::TVOID RenderDevice_D3D11::setComputeShader(RenderDeviceUsedCS* pComputeShader)
	{
		if(m_loaded && m_pImmediateContext)
		{
			if(pComputeShader)
			{
				ID3D11ComputeShader* pD3DShader = ((RenderDeviceUsedCS_D3D11*)pComputeShader)->getD3DCS();
				m_pImmediateContext->CSSetShader(pD3DShader,TSun::TNULL,0);
			}
			else
			{
				m_pImmediateContext->CSSetShader(TSun::TNULL,0,0);
			}
		}
	}

	RenderDeviceUsedBuffer* RenderDevice_D3D11::createBuffer(const TLRenderDeviceBufferDesc* pDesc,
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
			HRESULT hr = m_pd3dDevice->CreateBuffer( &bd, pInitData ? &data : TSun::TNULL, &pBuffer );
			if( FAILED( hr ) )
				return TSun::TNULL;

			RenderDeviceUsedBuffer_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedBuffer_D3D11);
			if(!pRet->initWithD3DBuffer(pBuffer))
			{
				pBuffer->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBuffer_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedTex1D* RenderDevice_D3D11::createTex1D(const TLRenderDeviceTex1DDesc* pDesc,
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
			D3D11_SUBRESOURCE_DATA* data = TSun::TNULL;
			if (pInitData != TSun::TNULL)
			{
				data = T_NEW_ARRAY(getRenderStructMemAllocator(), D3D11_SUBRESOURCE_DATA, pDesc->ArraySize * pDesc->MipLevels);
				for (TSun::TU32 j = 0; j < pDesc->ArraySize; ++ j)
				{
					for (TSun::TU32 i = 0; i < pDesc->MipLevels; ++ i)
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
				T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_SUBRESOURCE_DATA, data);
				data = TSun::TNULL;
			}
			if( FAILED( hr ) )
				return TSun::TNULL;

			RenderDeviceUsedTex1D_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedTex1D_D3D11);
			if(!pRet->initWithD3DTex1D(d3dTex))
			{
				d3dTex->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex1D_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedTex2D* RenderDevice_D3D11::createTex2D(const TLRenderDeviceTex2DDesc* pDesc,
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
			D3D11_SUBRESOURCE_DATA* data = TSun::TNULL;
			if (pInitData != TSun::TNULL)
			{
				data = T_NEW_ARRAY(getRenderStructMemAllocator(), D3D11_SUBRESOURCE_DATA, pDesc->ArraySize * pDesc->MipLevels);
				for (TSun::TU32 j = 0; j < pDesc->ArraySize; ++ j)
				{
					for (TSun::TU32 i = 0; i < pDesc->MipLevels; ++ i)
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
				T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_SUBRESOURCE_DATA, data);
				data = TSun::TNULL;
			}
			if( FAILED( hr ) )
				return TSun::TNULL;

			RenderDeviceUsedTex2D_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedTex2D_D3D11);
			if(!pRet->initWithD3DTex2D(d3dTex))
			{
				d3dTex->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex2D_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedTex3D* RenderDevice_D3D11::createTex3D(const TLRenderDeviceTex3DDesc* pDesc,
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
			D3D11_SUBRESOURCE_DATA* data = TSun::TNULL;
			if (pInitData != TSun::TNULL)
			{
				data = T_NEW_ARRAY(getRenderStructMemAllocator(), D3D11_SUBRESOURCE_DATA, 1 * pDesc->MipLevels);
				for (TSun::TU32 j = 0; j < 1; ++ j)
				{
					for (TSun::TU32 i = 0; i < pDesc->MipLevels; ++ i)
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
				T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_SUBRESOURCE_DATA, data);
				data = TSun::TNULL;
			}
			if( FAILED( hr ) )
				return TSun::TNULL;

			RenderDeviceUsedTex3D_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedTex3D_D3D11);
			if(!pRet->initWithD3DTex3D(d3dTex))
			{
				d3dTex->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedTex3D_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedSRV* RenderDevice_D3D11::createShaderResourceView(RenderDeviceUsedResource* pResource, 
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
					return TSun::TNULL;
				}
				break;
			}

			RenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pResource);
			ID3D11Resource* d3dResource = pD3DResource->getD3DResource();
			ID3D11ShaderResourceView* pView = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateShaderResourceView(d3dResource,&desc,&pView)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedSRV_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedSRV_D3D11);
			if(!pRet->initWithD3DSRV(pView))
			{
				pView->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSRV_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedRTV* RenderDevice_D3D11::createRenderTargetView(RenderDeviceUsedResource* pResource, 
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
					return TSun::TNULL;
				}
				break;
			}

			RenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pResource);
			ID3D11Resource* d3dResource = pD3DResource->getD3DResource();
			ID3D11RenderTargetView* pView = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateRenderTargetView(d3dResource,&desc,&pView)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedRTV_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedRTV_D3D11);
			if(!pRet->initWithD3DRTV(pView))
			{
				pView->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedRTV_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedDSV* RenderDevice_D3D11::createDepthStencilView(RenderDeviceUsedResource* pResource, 
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
					return TSun::TNULL;
				}
				break;
			}

			RenderDeviceUsedResource_D3D11* pD3DResource = dynamic_cast<RenderDeviceUsedResource_D3D11*>(pResource);
			ID3D11Resource* d3dResource = pD3DResource->getD3DResource();
			ID3D11DepthStencilView* pView = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateDepthStencilView(d3dResource,&desc,&pView)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedDSV_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedDSV_D3D11);
			if(!pRet->initWithD3DDSV(pView))
			{
				pView->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedDSV_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedBlendState* RenderDevice_D3D11::createBlendState(const TLRenderDeviceBlendDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_BLEND_DESC desc;
			desc.AlphaToCoverageEnable = TSun::TFALSE;
			desc.IndependentBlendEnable = TSun::TFALSE;
			for(TSun::TU32 i=0;i<8;i++)
			{
				desc.RenderTarget[i].BlendEnable = TSun::TFALSE;
			}
			desc.RenderTarget[0].BlendEnable = pDesc->RenderTarget[0].BlendEnable;
			desc.RenderTarget[0].BlendOp = (D3D11_BLEND_OP)pDesc->RenderTarget[0].BlendOp;
			desc.RenderTarget[0].BlendOpAlpha = (D3D11_BLEND_OP)pDesc->RenderTarget[0].BlendOpAlpha;
			desc.RenderTarget[0].DestBlend = (D3D11_BLEND)pDesc->RenderTarget[0].DestBlend;
			desc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)pDesc->RenderTarget[0].DestBlendAlpha;
			desc.RenderTarget[0].SrcBlend = (D3D11_BLEND)pDesc->RenderTarget[0].SrcBlend;
			desc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)pDesc->RenderTarget[0].SrcBlendAlpha;
			desc.RenderTarget[0].RenderTargetWriteMask = pDesc->RenderTarget[0].RenderTargetWriteMask;

			ID3D11BlendState* pState = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateBlendState(&desc,&pState)))
			{
				return TSun::TNULL;
			}
			
			RenderDeviceUsedBlendState_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedBlendState_D3D11);
			if(!pRet->initWithD3DBlendState(pState))
			{
				pState->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedBlendState_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedDepthStencilState* RenderDevice_D3D11::createDepthStencilState(const TLRenderDeviceDepthStencilDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			desc.DepthEnable = pDesc->DepthEnable;
			desc.DepthFunc = (D3D11_COMPARISON_FUNC)pDesc->DepthFunc;
			desc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)pDesc->DepthWriteMask;
			desc.StencilEnable = TSun::TFALSE;

			ID3D11DepthStencilState* pState = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateDepthStencilState(&desc,&pState)))
			{
				return TSun::TNULL;
			}
			
			RenderDeviceUsedDepthStencilState_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedDepthStencilState_D3D11);
			if(!pRet->initWithD3DDepthStencilState(pState))
			{
				pState->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedDepthStencilState_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedRasterizerState* RenderDevice_D3D11::createRasterizerState(const TLRenderDeviceRasterizerDesc* pDesc)
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
			desc.DepthClipEnable = TSun::TTRUE;
			desc.FrontCounterClockwise = TSun::TFALSE;
			desc.ScissorEnable = TSun::TFALSE;
			desc.SlopeScaledDepthBias = 0;

			ID3D11RasterizerState* pState = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateRasterizerState(&desc,&pState)))
			{
				return TSun::TNULL;
			}
			
			RenderDeviceUsedRasterizerState_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedRasterizerState_D3D11);
			if(!pRet->initWithD3DRasterizerState(pState))
			{
				pState->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedRasterizerState_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedSamplerState* RenderDevice_D3D11::createSamplerState(const TLRenderDeviceSamplerDesc* pDesc)
	{
		if(m_loaded && m_pd3dDevice && pDesc)
		{
			D3D11_SAMPLER_DESC desc;
			desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)pDesc->AddressU;
			desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)pDesc->AddressV;
			desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)pDesc->AddressW;
			memcpy(desc.BorderColor,pDesc->BorderColor,sizeof(TSun::TF32)*4);
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			desc.Filter = (D3D11_FILTER)pDesc->Filter;
			desc.MaxAnisotropy = pDesc->MaxAnisotropy;
			desc.MaxLOD = pDesc->MaxLOD;
			desc.MinLOD = pDesc->MinLOD;
			desc.MipLODBias = pDesc->MipLODBias;

			ID3D11SamplerState* pState = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateSamplerState(&desc,&pState)))
			{
				return TSun::TNULL;
			}
			
			RenderDeviceUsedSamplerState_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedSamplerState_D3D11);
			if(!pRet->initWithD3DSamplerState(pState))
			{
				pState->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedSamplerState_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceCompiledShader* RenderDevice_D3D11::createCompiledShader()
	{
		return T_NEW(getRenderStructMemAllocator(), RenderDeviceCompiledShader_D3D11);
	}

	RenderDeviceUsedInputLayout* RenderDevice_D3D11::createInputLayout(const TLRenderDeviceInputElementDesc* pElementDescs, 
			TSun::TU32 numElements, RenderDeviceCompiledShader* pBlob)
	{
		if(m_loaded && m_pd3dDevice && pElementDescs && numElements>0 && pBlob)
		{
			D3D11_INPUT_ELEMENT_DESC* pDesc = T_NEW_ARRAY(getRenderStructMemAllocator(), D3D11_INPUT_ELEMENT_DESC, numElements);
			for(TSun::TU32 i=0;i<numElements;i++)
			{
				pDesc[i].AlignedByteOffset = pElementDescs[i].AlignedByteOffset;
				pDesc[i].Format = (DXGI_FORMAT)pElementDescs[i].Format;
				pDesc[i].InputSlot = pElementDescs[i].InputSlot;
				pDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				pDesc[i].InstanceDataStepRate = 0;
				pDesc[i].SemanticIndex = pElementDescs[i].SemanticIndex;
				pDesc[i].SemanticName = pElementDescs[i].SemanticName.GetString();
			}

			ID3D11InputLayout* pInputLayout = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateInputLayout(pDesc,numElements,pBlob->getBufferPointer(),pBlob->getBufferSize(),&pInputLayout)))
			{
				T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_INPUT_ELEMENT_DESC, pDesc);
				return TSun::TNULL;
			}
			T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_INPUT_ELEMENT_DESC, pDesc);

			RenderDeviceUsedInputLayout_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedInputLayout_D3D11);
			if(!pRet->initWithD3DInputLayout(pInputLayout))
			{
				pInputLayout->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedInputLayout_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedVS* RenderDevice_D3D11::createVertexShader(RenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11VertexShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateVertexShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),TSun::TNULL,&pShader)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedVS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedVS_D3D11);
			if(!pRet->initWithD3DVS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedVS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedHS* RenderDevice_D3D11::createHullShader(RenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11HullShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateHullShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),TSun::TNULL,&pShader)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedHS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedHS_D3D11);
			if(!pRet->initWithD3DHS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedHS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedDS* RenderDevice_D3D11::createDomainShader(RenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11DomainShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateDomainShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),TSun::TNULL,&pShader)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedDS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedDS_D3D11);
			if(!pRet->initWithD3DDS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedDS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedGS* RenderDevice_D3D11::createGeometryShader(RenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11GeometryShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateGeometryShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),TSun::TNULL,&pShader)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedGS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedGS_D3D11);
			if(!pRet->initWithD3DGS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedGS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedGS* RenderDevice_D3D11::createGeometryShaderWithStreamOutput(RenderDeviceCompiledShader* pCompiledShader, 
			const TLRenderDeviceSODeclarationEntry* pSODeclaration, TSun::TU32 numEntries)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader && pSODeclaration && numEntries>0)
		{
			D3D11_SO_DECLARATION_ENTRY* pEntry = T_NEW_ARRAY(getRenderStructMemAllocator(), D3D11_SO_DECLARATION_ENTRY, numEntries);
			for(TSun::TU32 i=0;i<numEntries;i++)
			{
				pEntry[i].ComponentCount = pSODeclaration[i].ComponentCount;
				pEntry[i].OutputSlot = 0;
				pEntry[i].SemanticIndex = pSODeclaration[i].SemanticIndex;
				pEntry[i].SemanticName = pSODeclaration[i].SemanticName.GetString();
				pEntry[i].StartComponent = pSODeclaration[i].StartComponent;
				pEntry[i].Stream = 0;
			}

			ID3D11GeometryShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateGeometryShaderWithStreamOutput(pCompiledShader->getBufferPointer(),
				pCompiledShader->getBufferSize(),pEntry,numEntries,TSun::TNULL,0,0,TSun::TNULL,&pShader)))
			{
				T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_SO_DECLARATION_ENTRY, pEntry);
				return TSun::TNULL;
			}
			T_DELETE_ARRAY(getRenderStructMemAllocator(), D3D11_SO_DECLARATION_ENTRY, pEntry);

			RenderDeviceUsedGS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedGS_D3D11);
			if(!pRet->initWithD3DGS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedGS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedPS* RenderDevice_D3D11::createPixelShader(RenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11PixelShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreatePixelShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),TSun::TNULL,&pShader)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedPS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedPS_D3D11);
			if(!pRet->initWithD3DPS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedPS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	RenderDeviceUsedCS* RenderDevice_D3D11::createComputeShader(RenderDeviceCompiledShader* pCompiledShader)
	{
		if(m_loaded && m_pd3dDevice && pCompiledShader)
		{
			ID3D11ComputeShader* pShader = TSun::TNULL;
			if(FAILED(m_pd3dDevice->CreateComputeShader(pCompiledShader->getBufferPointer(),pCompiledShader->getBufferSize(),TSun::TNULL,&pShader)))
			{
				return TSun::TNULL;
			}

			RenderDeviceUsedCS_D3D11* pRet = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedCS_D3D11);
			if(!pRet->initWithD3DCS(pShader))
			{
				pShader->Release();
				T_DELETE(getRenderStructMemAllocator(), RenderDeviceUsedCS_D3D11, pRet);
				return TSun::TNULL;
			}
			return pRet;
		}
		return TSun::TNULL;
	}

	TSun::TBOOL RenderDevice_D3D11::createTex2DFromDDSFile(const TSun::TCHAR* file,RenderDeviceUsedTex2D** ppTex2D,RenderDeviceUsedSRV** ppSRV)
	{
		if(m_pd3dDevice && m_loaded)
		{
			TSun::String strFile(file);
			ID3D11Texture2D* d3dTex = TSun::TNULL;
			ID3D11ShaderResourceView* d3dSRV = TSun::TNULL;
			HRESULT hr = CreateDDSTextureFromFile( m_pd3dDevice, strFile.ToWString().get(), (ID3D11Resource**)&d3dTex, &d3dSRV );
			if( FAILED( hr ) )
				return TSun::TFALSE;
			if(ppTex2D)
			{
				*ppTex2D = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedTex2D_D3D11);
				((RenderDeviceUsedTex2D_D3D11*)(*ppTex2D))->initWithD3DTex2D(d3dTex);
			}
			else
			{
				d3dTex->Release();
			}
			if(ppSRV)
			{
				*ppSRV = T_NEW(getRenderStructMemAllocator(), RenderDeviceUsedSRV_D3D11);
				((RenderDeviceUsedSRV_D3D11*)(*ppSRV))->initWithD3DSRV(d3dSRV);
			}
			else
			{
				d3dSRV->Release();
			}
			return TSun::TTRUE;
		}
		return TSun::TFALSE;
	}
}