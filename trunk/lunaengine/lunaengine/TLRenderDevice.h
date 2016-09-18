#ifndef TL_RENDER_DEVICE_H
#define TL_RENDER_DEVICE_H
#include "TLRenderDef.h"

namespace TLunaEngine{
	// declaration
	class RenderDeviceUsed;
	class RenderDeviceUsedResource;
	class RenderDeviceUsedView;

	class RenderDeviceUsedBuffer;
	class RenderDeviceUsedTex1D;
	class RenderDeviceUsedTex2D;
	class RenderDeviceUsedTex3D;
	class RenderDeviceUsedSRV;
	class RenderDeviceUsedDSV;
	class RenderDeviceUsedRTV;
	class RenderDeviceUsedUAV;
	class RenderDeviceUsedVS;
	class RenderDeviceUsedHS;
	class RenderDeviceUsedDS;
	class RenderDeviceUsedGS;
	class RenderDeviceUsedPS;
	class RenderDeviceUsedCS;
	class RenderDeviceUsedSamplerState;
	class RenderDeviceUsedDepthStencilState;
	class RenderDeviceUsedRasterizerState;
	class RenderDeviceUsedBlendState;
	class RenderDeviceUsedInputLayout;
	class RenderDeviceCompiledShader;

	// 设备接口
	class RenderDevice
	{
	public:
		RenderDevice(void)
		{
		};
		virtual ~RenderDevice(void)
		{
		};
	protected:
	public:
		// 初始化设备
		virtual bool InitDevice(HWND hWnd,TBOOL bWnd,TS32 width,TS32 height) = 0;
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
		virtual TU32 GetBufferHeight()=0;
		// 得到后缓冲宽度
		virtual TU32 GetBufferWidth()=0;
		// resize buffer
		// note: resize buffer will change to default rt and vp
		virtual bool resizeBuffer(TU32 width,TU32 height) = 0;
		// draw
		virtual void draw(TU32 vertexCount,TU32 startVertexLocation) = 0;
		// draw indexed
		virtual void drawIndexed(TU32 indexCount,TU32 startIndexLocation,TU32 baseVertexLocation) = 0;
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
		virtual RenderDeviceUsedBuffer* createBuffer(const TLRenderDeviceBufferDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create texture 1d
		virtual RenderDeviceUsedTex1D* createTex1D(const TLRenderDeviceTex1DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create texture 2d
		virtual RenderDeviceUsedTex2D* createTex2D(const TLRenderDeviceTex2DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create texture 3d
		virtual RenderDeviceUsedTex3D* createTex3D(const TLRenderDeviceTex3DDesc* pDesc,
			const TLRenderDeviceSubresourceData* pInitData) = 0;
		// create shader resource view
		virtual RenderDeviceUsedSRV* createShaderResourceView(RenderDeviceUsedResource* pResource, 
			const TLRenderDeviceSRVDesc* pDesc) = 0;
		// create render target view
		virtual RenderDeviceUsedRTV* createRenderTargetView(RenderDeviceUsedResource* pResource, 
			const TLRenderDeviceRTVDesc* pDesc) = 0;
		// create depth stencil view
		virtual RenderDeviceUsedDSV* createDepthStencilView(RenderDeviceUsedResource* pResource, 
			const TLRenderDeviceDSVDesc* pDesc) = 0;
		// create blend state
		virtual RenderDeviceUsedBlendState* createBlendState(const TLRenderDeviceBlendDesc* pDesc) = 0;
		// create depth stencil desc
		virtual RenderDeviceUsedDepthStencilState* createDepthStencilState(const TLRenderDeviceDepthStencilDesc* pDesc)=0;
		// create rasterizer state
		virtual RenderDeviceUsedRasterizerState* createRasterizerState(const TLRenderDeviceRasterizerDesc* pDesc)=0;
		// create sampler state
		virtual RenderDeviceUsedSamplerState* createSamplerState(const TLRenderDeviceSamplerDesc* pDesc)=0;
		// create compiled shader
		// note: only new instance
		virtual RenderDeviceCompiledShader* createCompiledShader() = 0;
		// create input layout
		virtual RenderDeviceUsedInputLayout* createInputLayout(const TLRenderDeviceInputElementDesc* pElementDescs, 
			TU32 numElements, RenderDeviceCompiledShader* pBlob) = 0;
		// create vertex shader
		virtual RenderDeviceUsedVS* createVertexShader(RenderDeviceCompiledShader* pCompiledShader) = 0;
		// create hull shader
		virtual RenderDeviceUsedHS* createHullShader(RenderDeviceCompiledShader* pCompiledShader) = 0;
		// create domain shader
		virtual RenderDeviceUsedDS* createDomainShader(RenderDeviceCompiledShader* pCompiledShader) = 0;
		// create geometry shader
		virtual RenderDeviceUsedGS* createGeometryShader(RenderDeviceCompiledShader* pCompiledShader) = 0;
		// create geometry shader with stream output
		virtual RenderDeviceUsedGS* createGeometryShaderWithStreamOutput(RenderDeviceCompiledShader* pCompiledShader, 
			const TLRenderDeviceSODeclarationEntry* pSODeclaration, TU32 numEntries) = 0;
		// create pixel shader
		virtual RenderDeviceUsedPS* createPixelShader(RenderDeviceCompiledShader* pCompiledShader) = 0;
		// create compute shader
		virtual RenderDeviceUsedCS* createComputeShader(RenderDeviceCompiledShader* pCompiledShader) = 0;
		// create texture2d and texture2d srv from dds file
		virtual bool createTex2DFromDDSFile(const char* file,RenderDeviceUsedTex2D** ppTex2D,RenderDeviceUsedSRV** ppSRV) = 0;
	public:
		//---------------------- 使用硬件资源 -------------------------------
		// use viewport
		virtual bool useViewport(const TLRenderDeviceViewport* aVp,TU32 numVp) = 0;
		// set Primitive Topology
		virtual void setPrimitiveTopology(RENDER_DEVICE_PRIMITIVE_TOPOLOGY pt) = 0;
		// set input layout
		virtual void setInputLayout(RenderDeviceUsedInputLayout* pInputLayout) = 0;
		// map resource
		virtual bool mapResource(RenderDeviceUsedResource* pResource,TU32 subResource,
			RENDER_DEVICE_MAP mapType,TLRenderDeviceMappedSubresource* mappedSubresource) = 0;
		// unmap resource
		virtual void unmapResource(RenderDeviceUsedResource* pResource,TU32 subResource) = 0;
		// copy subresource region
		virtual void copySubresourceRegion(RenderDeviceUsedResource* pDstResource,TU32 dstSubresource,
			TU32 dstX,TU32 dstY,TU32 dstZ,RenderDeviceUsedResource* pSrcResource,TU32 srcSubresource,
			const TLRenderDeviceBox* pSrcBox)=0;
		// copy resource
		virtual void copyResource(RenderDeviceUsedResource* pDstResource,RenderDeviceUsedResource* pSrcResource)=0;
		// update subresource
		virtual void updateSubresource(RenderDeviceUsedResource* pDstResource,TU32 dstSubresource,
			const TLRenderDeviceBox* pDstBox,const void* pSrcData,TU32 srcRowPitch,TU32 srcDepthPitch)=0;
		// set vertex buffer
		virtual void setVertexBuffer(TU32 startSlot,RenderDeviceUsedBuffer* pBuffer, const TU32* pStride,const TU32* pOffset) = 0;
		// set index buffer
		virtual void setIndexBuffer(RenderDeviceUsedBuffer* pBuffer,RENDER_DEVICE_FORMAT format,TU32 offset)=0;
		// set stream output target
		virtual void setSOTarget(RenderDeviceUsedBuffer* pBuffer,const TU32* pOffset) = 0;
		// clear render target view
		virtual void clearRenderTargetView(RenderDeviceUsedRTV* pRenderTargetView, const TF32 colorRGBA[4]) = 0;
		// clear depth stencil view
		virtual void clearDepthStencilView(RenderDeviceUsedDSV* pDepthStencilView, TF32 depth) = 0;
		// set render target
		virtual void setRenderTarget(RenderDeviceUsedRTV* pRTV, RenderDeviceUsedDSV* pDSV) = 0;
		// set rasterizer state
		virtual void setRasterizerState(RenderDeviceUsedRasterizerState* pRasterizerState) = 0;
		// set depth stencil state
		virtual void setDepthStencilState(RenderDeviceUsedDepthStencilState* pDepthStencilState) = 0;
		// set blend state
		virtual void setBlendState(RenderDeviceUsedBlendState* pBlendState,const TF32 blendFactor[4],TU32 sampleMask) = 0;
		// set constant buffer
		virtual void setConstantBuffer(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TU32 startSlot, RenderDeviceUsedBuffer* pBuffer) = 0;
		// set sampler state
		virtual void setSamplerState(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TU32 startSlot, RenderDeviceUsedSamplerState* pSampler) = 0;
		// set shader resource view
		virtual void setShaderResourceView(RENDER_DEVICE_SHADER_USE_TYPE shaderType, TU32 startSlot, RenderDeviceUsedSRV* pSRV) = 0;
		// set vertex shader
		virtual void setVertexShader(RenderDeviceUsedVS* pVertexShader) = 0;
		// set hull shader
		virtual void setHullShader(RenderDeviceUsedHS* pHullShader) = 0;
		// set domain shader
		virtual void setDomainShader(RenderDeviceUsedDS* pDomainShader) = 0;
		// set geometry shader
		virtual void setGeometryShader(RenderDeviceUsedGS* pGeometryShader) = 0;
		// set pixel shader
		virtual void setPixelShader(RenderDeviceUsedPS* pPixelShader) = 0;
		// set compute shader
		virtual void setComputeShader(RenderDeviceUsedCS* pComputeShader) = 0;
	public:
		//----------------------utils functions ----------------------------
		// calculate subresource
		virtual TU32 calcSubresource(TU32 mipSlice,TU32 arraySlice,TU32 mipLevels) = 0;
		// generate mips
		virtual void generateMips(RenderDeviceUsedSRV* pSRV) = 0;
	};
};

#endif