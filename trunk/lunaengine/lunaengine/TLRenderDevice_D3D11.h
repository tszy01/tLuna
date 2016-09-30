#ifndef _TLRENDERDEVICE_D3D11_H_
#define _TLRENDERDEVICE_D3D11_H_

#include "TLRenderDevice.h"
#include <d3d11.h>

namespace TLunaEngine
{
	// d3d11 render device
	class RenderDevice_D3D11 : public RenderDevice
	{
	public:
		RenderDevice_D3D11();
		virtual ~RenderDevice_D3D11();
	public:
		// 初始化设备
		virtual TBOOL InitDevice(HWND hWnd,TBOOL bWnd,TS32 width,TS32 height);
		// 销毁设备
		virtual TVOID DestroyDevice();
		// 开始渲染
		virtual TBOOL BeginRender();
		// 结束渲染
		virtual TVOID EndRender();
		// 使用默认的渲染目标
		virtual TVOID UseDefaultRenderTarget();
		// 使用默认视口
		virtual TVOID UseDefaultViewPort();
		// 得到后缓冲高度
		virtual TU32 GetBufferHeight();
		// 得到后缓冲宽度
		virtual TU32 GetBufferWidth();
		// resize buffer
		// note: resize buffer will change to default rt and vp
		virtual TBOOL resizeBuffer(TU32 width,TU32 height);
		// draw
		virtual TVOID draw(TU32 vertexCount,TU32 startVertexLocation);
		// draw indexed
		virtual TVOID drawIndexed(TU32 indexCount,TU32 startIndexLocation,TU32 baseVertexLocation);
		// get d3d driver type
		// this is for d3d
		virtual TVOID getD3DDriverType(RENDER_DEVICE_D3D_DRIVER_TYPE* pDriverType);
		// get d3d feature level
		// this is for d3d
		virtual TVOID getD3DFeatureLevel(RENDER_DEVICE_D3D_FEATURE_LEVEL* pFeatureLevel);
		// get shader profile
		virtual const TCHAR* getShaderProfileVersion();
	public:
		//---------------------- 构造硬件资源 -------------------------------
		// create buffer
		virtual RenderDeviceUsedBuffer* createBuffer(const TLRenderDeviceBufferDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create texture 1d
		virtual RenderDeviceUsedTex1D* createTex1D(const TLRenderDeviceTex1DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create texture 2d
		virtual RenderDeviceUsedTex2D* createTex2D(const TLRenderDeviceTex2DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create texture 3d
		virtual RenderDeviceUsedTex3D* createTex3D(const TLRenderDeviceTex3DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData);
		// create shader resource view
		virtual RenderDeviceUsedSRV* createShaderResourceView(RenderDeviceUsedResource* pResource, 
			const TLRenderDeviceSRVDesc* pDesc);
		// create render target view
		virtual RenderDeviceUsedRTV* createRenderTargetView(RenderDeviceUsedResource* pResource, 
			const TLRenderDeviceRTVDesc* pDesc);
		// create depth stencil view
		virtual RenderDeviceUsedDSV* createDepthStencilView(RenderDeviceUsedResource* pResource, 
			const TLRenderDeviceDSVDesc* pDesc);
		// create blend state
		virtual RenderDeviceUsedBlendState* createBlendState(const TLRenderDeviceBlendDesc* pDesc);
		// create depth stencil desc
		virtual RenderDeviceUsedDepthStencilState* createDepthStencilState(const TLRenderDeviceDepthStencilDesc* pDesc);
		// create rasterizer state
		virtual RenderDeviceUsedRasterizerState* createRasterizerState(const TLRenderDeviceRasterizerDesc* pDesc);
		// create sampler state
		virtual RenderDeviceUsedSamplerState* createSamplerState(const TLRenderDeviceSamplerDesc* pDesc);
		// create compiled shader
		// note: only new instance
		virtual RenderDeviceCompiledShader* createCompiledShader();
		// create input layout
		virtual RenderDeviceUsedInputLayout* createInputLayout(const TLRenderDeviceInputElementDesc* pElementDescs, 
			TU32 numElements, RenderDeviceCompiledShader* pBlob);
		// create vertex shader
		virtual RenderDeviceUsedVS* createVertexShader(RenderDeviceCompiledShader* pCompiledShader);
		// create hull shader
		virtual RenderDeviceUsedHS* createHullShader(RenderDeviceCompiledShader* pCompiledShader);
		// create domain shader
		virtual RenderDeviceUsedDS* createDomainShader(RenderDeviceCompiledShader* pCompiledShader);
		// create geometry shader
		virtual RenderDeviceUsedGS* createGeometryShader(RenderDeviceCompiledShader* pCompiledShader);
		// create geometry shader with stream output
		virtual RenderDeviceUsedGS* createGeometryShaderWithStreamOutput(RenderDeviceCompiledShader* pCompiledShader, 
			const TLRenderDeviceSODeclarationEntry* pSODeclaration, TU32 numEntries);
		// create pixel shader
		virtual RenderDeviceUsedPS* createPixelShader(RenderDeviceCompiledShader* pCompiledShader);
		// create compute shader
		virtual RenderDeviceUsedCS* createComputeShader(RenderDeviceCompiledShader* pCompiledShader);
		// create texture2d and texture2d srv from dds file
		virtual TBOOL createTex2DFromDDSFile(const TCHAR* file,RenderDeviceUsedTex2D** ppTex2D,RenderDeviceUsedSRV** ppSRV);
	public:
		//---------------------- 使用硬件资源 -------------------------------
		// use viewport
		virtual TBOOL useViewport(const TLRenderDeviceViewport* aVp,TU32 numVp);
		// set Primitive Topology
		virtual TVOID setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY pt);
		// set input layout
		virtual TVOID setInputLayout(RenderDeviceUsedInputLayout* pInputLayout);
		// map resource
		virtual TBOOL mapResource(RenderDeviceUsedResource* pResource,TU32 subResource,
			RENDER_DEVICE_MAP mapType,TLRenderDeviceMappedSubresource* mappedSubresource);
		// unmap resource
		virtual TVOID unmapResource(RenderDeviceUsedResource* pResource,TU32 subResource);
		// copy subresource region
		virtual TVOID copySubresourceRegion(RenderDeviceUsedResource* pDstResource,TU32 dstSubresource,
			TU32 dstX,TU32 dstY,TU32 dstZ,RenderDeviceUsedResource* pSrcResource,TU32 srcSubresource,
			const TLRenderDeviceBox* pSrcBox);
		// copy resource
		virtual TVOID copyResource(RenderDeviceUsedResource* pDstResource,RenderDeviceUsedResource* pSrcResource);
		// update subresource
		virtual TVOID updateSubresource(RenderDeviceUsedResource* pDstResource,TU32 dstSubresource,
			const TLRenderDeviceBox* pDstBox,const TVOID* pSrcData,TU32 srcRowPitch,TU32 srcDepthPitch);
		// set vertex buffer
		virtual TVOID setVertexBuffer(TU32 startSlot,RenderDeviceUsedBuffer* pBuffer, const TU32* pStride,const TU32* pOffset);
		// set index buffer
		virtual TVOID setIndexBuffer(RenderDeviceUsedBuffer* pBuffer,RENDER_DEVICE_FORMAT format,TU32 offset);
		// set stream output target
		virtual TVOID setSOTarget(RenderDeviceUsedBuffer* pBuffer,const TU32* pOffset);
		// clear render target view
		virtual TVOID clearRenderTargetView(RenderDeviceUsedRTV* pRenderTargetView, const TF32 colorRGBA[4]);
		// clear depth stencil view
		virtual TVOID clearDepthStencilView(RenderDeviceUsedDSV* pDepthStencilView, TF32 depth);
		// set render target
		virtual TVOID setRenderTarget(RenderDeviceUsedRTV* pRTV, RenderDeviceUsedDSV* pDSV);
		// set rasterizer state
		virtual TVOID setRasterizerState(RenderDeviceUsedRasterizerState* pRasterizerState);
		// set depth stencil state
		virtual TVOID setDepthStencilState(RenderDeviceUsedDepthStencilState* pDepthStencilState);
		// set blend state
		virtual TVOID setBlendState(RenderDeviceUsedBlendState* pBlendState,const TF32 blendFactor[4],TU32 sampleMask);
		// set constant buffer
		virtual TVOID setConstantBuffer(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TU32 startSlot, RenderDeviceUsedBuffer* pBuffer);
		// set sampler state
		virtual TVOID setSamplerState(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TU32 startSlot, RenderDeviceUsedSamplerState* pSampler);
		// set shader resource view
		virtual TVOID setShaderResourceView(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TU32 startSlot, RenderDeviceUsedSRV* pSRV);
		// set vertex shader
		virtual TVOID setVertexShader(RenderDeviceUsedVS* pVertexShader);
		// set hull shader
		virtual TVOID setHullShader(RenderDeviceUsedHS* pHullShader);
		// set domain shader
		virtual TVOID setDomainShader(RenderDeviceUsedDS* pDomainShader);
		// set geometry shader
		virtual TVOID setGeometryShader(RenderDeviceUsedGS* pGeometryShader);
		// set pixel shader
		virtual TVOID setPixelShader(RenderDeviceUsedPS* pPixelShader);
		// set compute shader
		virtual TVOID setComputeShader(RenderDeviceUsedCS* pComputeShader);
	public:
		//----------------------utils functions ----------------------------
		// calculate subresource
		virtual TU32 calcSubresource(TU32 mipSlice,TU32 arraySlice,TU32 mipLevels);
		// generate mips
		virtual TVOID generateMips(RenderDeviceUsedSRV* pSRV);
	private:
		ID3D11Device* m_pd3dDevice;
		ID3D11DeviceContext* m_pImmediateContext;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;		// 渲染视图
		ID3D11Texture2D* m_pDepthStencil;			// 深度缓冲区
		ID3D11DepthStencilView* m_pDepthStencilView;		// 深度视图
		D3D11_VIEWPORT m_ViewPort;					// 保存视口
		TLRenderDeviceViewport mDefaultVp;
		TBOOL m_loaded;
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