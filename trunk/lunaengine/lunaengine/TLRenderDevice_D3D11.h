#ifndef _TLRENDERDEVICE_D3D11_H_
#define _TLRENDERDEVICE_D3D11_H_

#include "TLIRenderDevice.h"
#include <d3d11.h>

namespace TLunaEngine
{
	// d3d11 render device
	class TLRenderDevice_D3D11 : public TLIRenderDevice
	{
	public:
		TLRenderDevice_D3D11();
		virtual ~TLRenderDevice_D3D11();
	public:
		// 初始化设备
		virtual bool InitDevice(HWND hWnd,BOOL bWnd,LONG width,LONG height);
		// 销毁设备
		virtual void DestroyDevice();
		// 开始渲染
		virtual bool BeginRender();
		// 结束渲染
		virtual void EndRender();
		// 使用默认的渲染目标
		virtual void UseDefaultRenderTarget();
		// 使用默认视口
		virtual void UseDefaultViewPort();
		// 得到后缓冲高度
		virtual UINT GetBufferHeight();
		// 得到后缓冲宽度
		virtual UINT GetBufferWidth();
		// resize buffer
		// note: resize buffer will change to default rt and vp
		virtual bool resizeBuffer(UINT width,UINT height);
		// draw
		virtual void draw(UINT vertexCount,UINT startVertexLocation);
		// draw indexed
		virtual void drawIndexed(UINT indexCount,UINT startIndexLocation,UINT baseVertexLocation);
		// get d3d driver type
		// this is for d3d
		virtual void getD3DDriverType(RENDER_DEVICE_D3D_DRIVER_TYPE* pDriverType);
		// get d3d feature level
		// this is for d3d
		virtual void getD3DFeatureLevel(RENDER_DEVICE_D3D_FEATURE_LEVEL* pFeatureLevel);
		// get shader profile
		virtual const char* getShaderProfileVersion();
	public:
		//---------------------- 构造硬件资源 -------------------------------
		// create buffer
		virtual TLIRenderDeviceUsedBuffer* createBuffer(const TLRenderDeviceBufferDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create texture 1d
		virtual TLIRenderDeviceUsedTex1D* createTex1D(const TLRenderDeviceTex1DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create texture 2d
		virtual TLIRenderDeviceUsedTex2D* createTex2D(const TLRenderDeviceTex2DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create texture 3d
		virtual TLIRenderDeviceUsedTex3D* createTex3D(const TLRenderDeviceTex3DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create shader resource view
		virtual TLIRenderDeviceUsedSRV* createShaderResourceView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceSRVDesc* pDesc);
		// create render target view
		virtual TLIRenderDeviceUsedRTV* createRenderTargetView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceRTVDesc* pDesc);
		// create depth stencil view
		virtual TLIRenderDeviceUsedDSV* createDepthStencilView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceDSVDesc* pDesc);
		// create blend state
		virtual TLIRenderDeviceUsedBlendState* createBlendState(const TLRenderDeviceBlendDesc* pDesc);
		// create depth stencil desc
		virtual TLIRenderDeviceUsedDepthStencilState* createDepthStencilState(const TLRenderDeviceDepthStencilDesc* pDesc);
		// create rasterizer state
		virtual TLIRenderDeviceUsedRasterizerState* createRasterizerState(const TLRenderDeviceRasterizerDesc* pDesc);
		// create sampler state
		virtual TLIRenderDeviceUsedSamplerState* createSamplerState(const TLRenderDeviceSamplerDesc* pDesc);
		// create compiled shader
		// note: only new instance
		virtual TLIRenderDeviceCompiledShader* createCompiledShader();
		// create input layout
		virtual TLIRenderDeviceUsedInputLayout* createInputLayout(const TLRenderDeviceInputElementDesc* pElementDescs, 
			UINT numElements, TLIRenderDeviceCompiledShader* pBlob);
		// create vertex shader
		virtual TLIRenderDeviceUsedVS* createVertexShader(TLIRenderDeviceCompiledShader* pCompiledShader);
		// create hull shader
		virtual TLIRenderDeviceUsedHS* createHullShader(TLIRenderDeviceCompiledShader* pCompiledShader);
		// create domain shader
		virtual TLIRenderDeviceUsedDS* createDomainShader(TLIRenderDeviceCompiledShader* pCompiledShader);
		// create geometry shader
		virtual TLIRenderDeviceUsedGS* createGeometryShader(TLIRenderDeviceCompiledShader* pCompiledShader);
		// create geometry shader with stream output
		virtual TLIRenderDeviceUsedGS* createGeometryShaderWithStreamOutput(TLIRenderDeviceCompiledShader* pCompiledShader, 
			const TLRenderDeviceSODeclarationEntry* pSODeclaration, UINT numEntries);
		// create pixel shader
		virtual TLIRenderDeviceUsedPS* createPixelShader(TLIRenderDeviceCompiledShader* pCompiledShader);
		// create compute shader
		virtual TLIRenderDeviceUsedCS* createComputeShader(TLIRenderDeviceCompiledShader* pCompiledShader);
		// create texture2d and texture2d srv from dds file
		virtual bool createTex2DFromDDSFile(const char* file,TLIRenderDeviceUsedTex2D** ppTex2D,TLIRenderDeviceUsedSRV** ppSRV);
	public:
		//---------------------- 使用硬件资源 -------------------------------
		// use viewport
		virtual bool useViewport(const TLRenderDeviceViewport* aVp,UINT numVp);
		// set Primitive Topology
		virtual void setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY pt);
		// set input layout
		virtual void setInputLayout(TLIRenderDeviceUsedInputLayout* pInputLayout);
		// map resource
		virtual bool mapResource(TLIRenderDeviceUsedResource* pResource,UINT subResource,
			RENDER_DEVICE_MAP mapType,TLRenderDeviceMappedSubresource* mappedSubresource);
		// unmap resource
		virtual void unmapResource(TLIRenderDeviceUsedResource* pResource,UINT subResource);
		// copy subresource region
		virtual void copySubresourceRegion(TLIRenderDeviceUsedResource* pDstResource,UINT dstSubresource,
			UINT dstX,UINT dstY,UINT dstZ,TLIRenderDeviceUsedResource* pSrcResource,UINT srcSubresource,
			const TLRenderDeviceBox* pSrcBox);
		// copy resource
		virtual void copyResource(TLIRenderDeviceUsedResource* pDstResource,TLIRenderDeviceUsedResource* pSrcResource);
		// update subresource
		virtual void updateSubresource(TLIRenderDeviceUsedResource* pDstResource,UINT dstSubresource,
			const TLRenderDeviceBox* pDstBox,const void* pSrcData,UINT srcRowPitch,UINT srcDepthPitch);
		// set vertex buffer
		virtual void setVertexBuffer(UINT startSlot,TLIRenderDeviceUsedBuffer* pBuffer, const UINT* pStride,const UINT* pOffset);
		// set index buffer
		virtual void setIndexBuffer(TLIRenderDeviceUsedBuffer* pBuffer,RENDER_DEVICE_FORMAT format,UINT offset);
		// set stream output target
		virtual void setSOTarget(TLIRenderDeviceUsedBuffer* pBuffer,const UINT* pOffset);
		// clear render target view
		virtual void clearRenderTargetView(TLIRenderDeviceUsedRTV* pRenderTargetView, const FLOAT colorRGBA[4]);
		// clear depth stencil view
		virtual void clearDepthStencilView(TLIRenderDeviceUsedDSV* pDepthStencilView, FLOAT depth);
		// set render target
		virtual void setRenderTarget(TLIRenderDeviceUsedRTV* pRTV, TLIRenderDeviceUsedDSV* pDSV);
		// set rasterizer state
		virtual void setRasterizerState(TLIRenderDeviceUsedRasterizerState* pRasterizerState);
		// set depth stencil state
		virtual void setDepthStencilState(TLIRenderDeviceUsedDepthStencilState* pDepthStencilState);
		// set blend state
		virtual void setBlendState(TLIRenderDeviceUsedBlendState* pBlendState,const FLOAT blendFactor[4],UINT sampleMask);
		// set constant buffer
		virtual void setConstantBuffer(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedBuffer* pBuffer);
		// set sampler state
		virtual void setSamplerState(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedSamplerState* pSampler);
		// set shader resource view
		virtual void setShaderResourceView(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedSRV* pSRV);
		// set vertex shader
		virtual void setVertexShader(TLIRenderDeviceUsedVS* pVertexShader);
		// set hull shader
		virtual void setHullShader(TLIRenderDeviceUsedHS* pHullShader);
		// set domain shader
		virtual void setDomainShader(TLIRenderDeviceUsedDS* pDomainShader);
		// set geometry shader
		virtual void setGeometryShader(TLIRenderDeviceUsedGS* pGeometryShader);
		// set pixel shader
		virtual void setPixelShader(TLIRenderDeviceUsedPS* pPixelShader);
		// set compute shader
		virtual void setComputeShader(TLIRenderDeviceUsedCS* pComputeShader);
	public:
		//----------------------utils functions ----------------------------
		// calculate subresource
		virtual UINT calcSubresource(UINT mipSlice,UINT arraySlice,UINT mipLevels);
		// generate mips
		virtual void generateMips(TLIRenderDeviceUsedSRV* pSRV);
	private:
		ID3D11Device* m_pd3dDevice;
		ID3D11DeviceContext* m_pImmediateContext;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;		// 渲染视图
		ID3D11Texture2D* m_pDepthStencil;			// 深度缓冲区
		ID3D11DepthStencilView* m_pDepthStencilView;		// 深度视图
		D3D11_VIEWPORT m_ViewPort;					// 保存视口
		TLRenderDeviceViewport mDefaultVp;
		bool m_loaded;
		D3D_DRIVER_TYPE m_DriverType;
		D3D_FEATURE_LEVEL m_FeatureLevel;
	public:
		ID3D11DeviceContext* getD3DDeviceContext()
		{
			return m_pImmediateContext;
		}
		ID3D11Device* getD3DDevice()
		{
			return m_pd3dDevice;
		}
	};
}

#endif