#ifndef TLI_RENDER_DEVICE_H
#define TLI_RENDER_DEVICE_H
#include "TLRenderDef.h"

namespace TLunaEngine{
	// declaration
	class TLIRenderDeviceUsed;
	class TLIRenderDeviceUsedResource;
	class TLIRenderDeviceUsedView;

	class TLIRenderDeviceUsedBuffer;
	class TLIRenderDeviceUsedTex1D;
	class TLIRenderDeviceUsedTex2D;
	class TLIRenderDeviceUsedTex3D;
	class TLIRenderDeviceUsedSRV;
	class TLIRenderDeviceUsedDSV;
	class TLIRenderDeviceUsedRTV;
	class TLIRenderDeviceUsedUAV;
	class TLIRenderDeviceUsedVS;
	class TLIRenderDeviceUsedHS;
	class TLIRenderDeviceUsedDS;
	class TLIRenderDeviceUsedGS;
	class TLIRenderDeviceUsedPS;
	class TLIRenderDeviceUsedCS;
	class TLIRenderDeviceUsedSamplerState;
	class TLIRenderDeviceUsedDepthStencilState;
	class TLIRenderDeviceUsedRasterizerState;
	class TLIRenderDeviceUsedBlendState;
	class TLIRenderDeviceUsedInputLayout;
	class TLIRenderDeviceCompiledShader;

	// 设备接口
	class TLIRenderDevice
	{
	public:
		TLIRenderDevice(void)
		{
		};
		virtual ~TLIRenderDevice(void)
		{
		};
	protected:
	public:
		// 初始化设备
		virtual bool InitDevice(HWND hWnd,BOOL bWnd,LONG width,LONG height) = 0;
		// 销毁设备
		virtual void DestroyDevice() = 0;
		// 开始渲染
		virtual bool BeginRender() = 0;
		// 结束渲染
		virtual void EndRender() = 0;
		// 使用默认的渲染目标
		virtual void UseDefaultRenderTarget() = 0;
		// 使用默认视口
		virtual void UseDefaultViewPort() = 0;
		// 得到后缓冲高度
		virtual UINT GetBufferHeight()=0;
		// 得到后缓冲宽度
		virtual UINT GetBufferWidth()=0;
		// resize buffer
		// note: resize buffer will change to default rt and vp
		virtual bool resizeBuffer(UINT width,UINT height) = 0;
		// draw
		virtual void draw(UINT vertexCount,UINT startVertexLocation) = 0;
		// draw indexed
		virtual void drawIndexed(UINT indexCount,UINT startIndexLocation,UINT baseVertexLocation) = 0;
		// get d3d driver type
		// this is for d3d
		virtual void getD3DDriverType(RENDER_DEVICE_D3D_DRIVER_TYPE* pDriverType) = 0;
		// get d3d feature level
		// this is for d3d
		virtual void getD3DFeatureLevel(RENDER_DEVICE_D3D_FEATURE_LEVEL* pFeatureLevel) = 0;
		// get shader profile
		virtual const char* getShaderProfileVersion() = 0;
	public:
		//---------------------- 构造硬件资源 -------------------------------
		// create buffer
		virtual TLIRenderDeviceUsedBuffer* createBuffer(const TLRenderDeviceBufferDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create texture 1d
		virtual TLIRenderDeviceUsedTex1D* createTex1D(const TLRenderDeviceTex1DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create texture 2d
		virtual TLIRenderDeviceUsedTex2D* createTex2D(const TLRenderDeviceTex2DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create texture 3d
		virtual TLIRenderDeviceUsedTex3D* createTex3D(const TLRenderDeviceTex3DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create shader resource view
		virtual TLIRenderDeviceUsedSRV* createShaderResourceView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceSRVDesc* pDesc) = 0;
		// create render target view
		virtual TLIRenderDeviceUsedRTV* createRenderTargetView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceRTVDesc* pDesc) = 0;
		// create depth stencil view
		virtual TLIRenderDeviceUsedDSV* createDepthStencilView(TLIRenderDeviceUsedResource* pResource, 
			const TLRenderDeviceDSVDesc* pDesc) = 0;
		// create blend state
		virtual TLIRenderDeviceUsedBlendState* createBlendState(const TLRenderDeviceBlendDesc* pDesc) = 0;
		// create depth stencil desc
		virtual TLIRenderDeviceUsedDepthStencilState* createDepthStencilState(const TLRenderDeviceDepthStencilDesc* pDesc)=0;
		// create rasterizer state
		virtual TLIRenderDeviceUsedRasterizerState* createRasterizerState(const TLRenderDeviceRasterizerDesc* pDesc)=0;
		// create sampler state
		virtual TLIRenderDeviceUsedSamplerState* createSamplerState(const TLRenderDeviceSamplerDesc* pDesc)=0;
		// create compiled shader
		// note: only new instance
		virtual TLIRenderDeviceCompiledShader* createCompiledShader() = 0;
		// create input layout
		virtual TLIRenderDeviceUsedInputLayout* createInputLayout(const TLRenderDeviceInputElementDesc* pElementDescs, 
			UINT numElements, TLIRenderDeviceCompiledShader* pBlob) = 0;
		// create vertex shader
		virtual TLIRenderDeviceUsedVS* createVertexShader(TLIRenderDeviceCompiledShader* pCompiledShader) = 0;
		// create hull shader
		virtual TLIRenderDeviceUsedHS* createHullShader(TLIRenderDeviceCompiledShader* pCompiledShader) = 0;
		// create domain shader
		virtual TLIRenderDeviceUsedDS* createDomainShader(TLIRenderDeviceCompiledShader* pCompiledShader) = 0;
		// create geometry shader
		virtual TLIRenderDeviceUsedGS* createGeometryShader(TLIRenderDeviceCompiledShader* pCompiledShader) = 0;
		// create geometry shader with stream output
		virtual TLIRenderDeviceUsedGS* createGeometryShaderWithStreamOutput(TLIRenderDeviceCompiledShader* pCompiledShader, 
			const TLRenderDeviceSODeclarationEntry* pSODeclaration, UINT numEntries) = 0;
		// create pixel shader
		virtual TLIRenderDeviceUsedPS* createPixelShader(TLIRenderDeviceCompiledShader* pCompiledShader) = 0;
		// create compute shader
		virtual TLIRenderDeviceUsedCS* createComputeShader(TLIRenderDeviceCompiledShader* pCompiledShader) = 0;
		// create texture2d and texture2d srv from dds file
		virtual bool createTex2DFromDDSFile(const char* file,TLIRenderDeviceUsedTex2D** ppTex2D,TLIRenderDeviceUsedSRV** ppSRV) = 0;
	public:
		//---------------------- 使用硬件资源 -------------------------------
		// use viewport
		virtual bool useViewport(const TLRenderDeviceViewport* aVp,UINT numVp) = 0;
		// set Primitive Topology
		virtual void setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY pt) = 0;
		// set input layout
		virtual void setInputLayout(TLIRenderDeviceUsedInputLayout* pInputLayout) = 0;
		// map resource
		virtual bool mapResource(TLIRenderDeviceUsedResource* pResource,UINT subResource,
			RENDER_DEVICE_MAP mapType,TLRenderDeviceMappedSubresource* mappedSubresource) = 0;
		// unmap resource
		virtual void unmapResource(TLIRenderDeviceUsedResource* pResource,UINT subResource) = 0;
		// copy subresource region
		virtual void copySubresourceRegion(TLIRenderDeviceUsedResource* pDstResource,UINT dstSubresource,
			UINT dstX,UINT dstY,UINT dstZ,TLIRenderDeviceUsedResource* pSrcResource,UINT srcSubresource,
			const TLRenderDeviceBox* pSrcBox)=0;
		// copy resource
		virtual void copyResource(TLIRenderDeviceUsedResource* pDstResource,TLIRenderDeviceUsedResource* pSrcResource)=0;
		// update subresource
		virtual void updateSubresource(TLIRenderDeviceUsedResource* pDstResource,UINT dstSubresource,
			const TLRenderDeviceBox* pDstBox,const void* pSrcData,UINT srcRowPitch,UINT srcDepthPitch)=0;
		// set vertex buffer
		virtual void setVertexBuffer(UINT startSlot,TLIRenderDeviceUsedBuffer* pBuffer, const UINT* pStride,const UINT* pOffset) = 0;
		// set index buffer
		virtual void setIndexBuffer(TLIRenderDeviceUsedBuffer* pBuffer,RENDER_DEVICE_FORMAT format,UINT offset)=0;
		// set stream output target
		virtual void setSOTarget(TLIRenderDeviceUsedBuffer* pBuffer,const UINT* pOffset) = 0;
		// clear render target view
		virtual void clearRenderTargetView(TLIRenderDeviceUsedRTV* pRenderTargetView, const FLOAT colorRGBA[4]) = 0;
		// clear depth stencil view
		virtual void clearDepthStencilView(TLIRenderDeviceUsedDSV* pDepthStencilView, FLOAT depth) = 0;
		// set render target
		virtual void setRenderTarget(TLIRenderDeviceUsedRTV* pRTV, TLIRenderDeviceUsedDSV* pDSV) = 0;
		// set rasterizer state
		virtual void setRasterizerState(TLIRenderDeviceUsedRasterizerState* pRasterizerState) = 0;
		// set depth stencil state
		virtual void setDepthStencilState(TLIRenderDeviceUsedDepthStencilState* pDepthStencilState) = 0;
		// set blend state
		virtual void setBlendState(TLIRenderDeviceUsedBlendState* pBlendState,const FLOAT blendFactor[4],UINT sampleMask) = 0;
		// set constant buffer
		virtual void setConstantBuffer(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedBuffer* pBuffer) = 0;
		// set sampler state
		virtual void setSamplerState(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedSamplerState* pSampler) = 0;
		// set shader resource view
		virtual void setShaderResourceView(RENDER_DEVICE_SHADER_USE_TYPE shaderType, UINT startSlot, TLIRenderDeviceUsedSRV* pSRV) = 0;
		// set vertex shader
		virtual void setVertexShader(TLIRenderDeviceUsedVS* pVertexShader) = 0;
		// set hull shader
		virtual void setHullShader(TLIRenderDeviceUsedHS* pHullShader) = 0;
		// set domain shader
		virtual void setDomainShader(TLIRenderDeviceUsedDS* pDomainShader) = 0;
		// set geometry shader
		virtual void setGeometryShader(TLIRenderDeviceUsedGS* pGeometryShader) = 0;
		// set pixel shader
		virtual void setPixelShader(TLIRenderDeviceUsedPS* pPixelShader) = 0;
		// set compute shader
		virtual void setComputeShader(TLIRenderDeviceUsedCS* pComputeShader) = 0;
	public:
		//----------------------utils functions ----------------------------
		// calculate subresource
		virtual UINT calcSubresource(UINT mipSlice,UINT arraySlice,UINT mipLevels) = 0;
		// generate mips
		virtual void generateMips(TLIRenderDeviceUsedSRV* pSRV) = 0;
	};
};

#endif