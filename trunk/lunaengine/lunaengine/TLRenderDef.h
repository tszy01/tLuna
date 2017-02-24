#ifndef _TLRENDERDEF_H_
#define _TLRENDERDEF_H_
#include "TSCommonTypeDef.h"
#include "TSString.h"
#include "TSFPU.h"

namespace TLunaEngine
{
	// d3d driver type
	enum RENDER_DEVICE_D3D_DRIVER_TYPE { 
		RENDER_DEVICE_D3D_DRIVER_TYPE_UNKNOWN     = 0,
		RENDER_DEVICE_D3D_DRIVER_TYPE_HARDWARE    = ( RENDER_DEVICE_D3D_DRIVER_TYPE_UNKNOWN + 1 ),
		RENDER_DEVICE_D3D_DRIVER_TYPE_REFERENCE   = ( RENDER_DEVICE_D3D_DRIVER_TYPE_HARDWARE + 1 ),
		RENDER_DEVICE_D3D_DRIVER_TYPE_NULL        = ( RENDER_DEVICE_D3D_DRIVER_TYPE_REFERENCE + 1 ),
		RENDER_DEVICE_D3D_DRIVER_TYPE_SOFTWARE    = ( RENDER_DEVICE_D3D_DRIVER_TYPE_NULL + 1 ),
		RENDER_DEVICE_D3D_DRIVER_TYPE_WARP        = ( RENDER_DEVICE_D3D_DRIVER_TYPE_SOFTWARE + 1 ) 
	};

	// d3d feature level
	enum RENDER_DEVICE_D3D_FEATURE_LEVEL { 
		RENDER_DEVICE_D3D_FEATURE_LEVEL_9_1    = 0x9100,
		RENDER_DEVICE_D3D_FEATURE_LEVEL_9_2    = 0x9200,
		RENDER_DEVICE_D3D_FEATURE_LEVEL_9_3    = 0x9300,
		RENDER_DEVICE_D3D_FEATURE_LEVEL_10_0   = 0xa000,
		RENDER_DEVICE_D3D_FEATURE_LEVEL_10_1   = 0xa100,
		RENDER_DEVICE_D3D_FEATURE_LEVEL_11_0   = 0xb000,
		RENDER_DEVICE_D3D_FEATURE_LEVEL_11_1   = 0xb100 
	};

	// device tex format
	enum RENDER_DEVICE_FORMAT 
	{ 
		RENDER_DEVICE_FORMAT_UNKNOWN                      = 0,
		RENDER_DEVICE_FORMAT_R32G32B32A32_TYPELESS        = 1,
		RENDER_DEVICE_FORMAT_R32G32B32A32_FLOAT           = 2,
		RENDER_DEVICE_FORMAT_R32G32B32A32_UINT            = 3,
		RENDER_DEVICE_FORMAT_R32G32B32A32_SINT            = 4,
		RENDER_DEVICE_FORMAT_R32G32B32_TYPELESS           = 5,
		RENDER_DEVICE_FORMAT_R32G32B32_FLOAT              = 6,
		RENDER_DEVICE_FORMAT_R32G32B32_UINT               = 7,
		RENDER_DEVICE_FORMAT_R32G32B32_SINT               = 8,
		RENDER_DEVICE_FORMAT_R16G16B16A16_TYPELESS        = 9,
		RENDER_DEVICE_FORMAT_R16G16B16A16_FLOAT           = 10,
		RENDER_DEVICE_FORMAT_R16G16B16A16_UNORM           = 11,
		RENDER_DEVICE_FORMAT_R16G16B16A16_UINT            = 12,
		RENDER_DEVICE_FORMAT_R16G16B16A16_SNORM           = 13,
		RENDER_DEVICE_FORMAT_R16G16B16A16_SINT            = 14,
		RENDER_DEVICE_FORMAT_R32G32_TYPELESS              = 15,
		RENDER_DEVICE_FORMAT_R32G32_FLOAT                 = 16,
		RENDER_DEVICE_FORMAT_R32G32_UINT                  = 17,
		RENDER_DEVICE_FORMAT_R32G32_SINT                  = 18,
		RENDER_DEVICE_FORMAT_R32G8X24_TYPELESS            = 19,
		RENDER_DEVICE_FORMAT_D32_FLOAT_S8X24_UINT         = 20,
		RENDER_DEVICE_FORMAT_R32_FLOAT_X8X24_TYPELESS     = 21,
		RENDER_DEVICE_FORMAT_X32_TYPELESS_G8X24_UINT      = 22,
		RENDER_DEVICE_FORMAT_R10G10B10A2_TYPELESS         = 23,
		RENDER_DEVICE_FORMAT_R10G10B10A2_UNORM            = 24,
		RENDER_DEVICE_FORMAT_R10G10B10A2_UINT             = 25,
		RENDER_DEVICE_FORMAT_R11G11B10_FLOAT              = 26,
		RENDER_DEVICE_FORMAT_R8G8B8A8_TYPELESS            = 27,
		RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM               = 28,
		RENDER_DEVICE_FORMAT_R8G8B8A8_UNORM_SRGB          = 29,
		RENDER_DEVICE_FORMAT_R8G8B8A8_UINT                = 30,
		RENDER_DEVICE_FORMAT_R8G8B8A8_SNORM               = 31,
		RENDER_DEVICE_FORMAT_R8G8B8A8_SINT                = 32,
		RENDER_DEVICE_FORMAT_R16G16_TYPELESS              = 33,
		RENDER_DEVICE_FORMAT_R16G16_FLOAT                 = 34,
		RENDER_DEVICE_FORMAT_R16G16_UNORM                 = 35,
		RENDER_DEVICE_FORMAT_R16G16_UINT                  = 36,
		RENDER_DEVICE_FORMAT_R16G16_SNORM                 = 37,
		RENDER_DEVICE_FORMAT_R16G16_SINT                  = 38,
		RENDER_DEVICE_FORMAT_R32_TYPELESS                 = 39,
		RENDER_DEVICE_FORMAT_D32_FLOAT                    = 40,
		RENDER_DEVICE_FORMAT_R32_FLOAT                    = 41,
		RENDER_DEVICE_FORMAT_R32_UINT                     = 42,
		RENDER_DEVICE_FORMAT_R32_SINT                     = 43,
		RENDER_DEVICE_FORMAT_R24G8_TYPELESS               = 44,
		RENDER_DEVICE_FORMAT_D24_UNORM_S8_UINT            = 45,
		RENDER_DEVICE_FORMAT_R24_UNORM_X8_TYPELESS        = 46,
		RENDER_DEVICE_FORMAT_X24_TYPELESS_G8_UINT         = 47,
		RENDER_DEVICE_FORMAT_R8G8_TYPELESS                = 48,
		RENDER_DEVICE_FORMAT_R8G8_UNORM                   = 49,
		RENDER_DEVICE_FORMAT_R8G8_UINT                    = 50,
		RENDER_DEVICE_FORMAT_R8G8_SNORM                   = 51,
		RENDER_DEVICE_FORMAT_R8G8_SINT                    = 52,
		RENDER_DEVICE_FORMAT_R16_TYPELESS                 = 53,
		RENDER_DEVICE_FORMAT_R16_FLOAT                    = 54,
		RENDER_DEVICE_FORMAT_D16_UNORM                    = 55,
		RENDER_DEVICE_FORMAT_R16_UNORM                    = 56,
		RENDER_DEVICE_FORMAT_R16_UINT                     = 57,
		RENDER_DEVICE_FORMAT_R16_SNORM                    = 58,
		RENDER_DEVICE_FORMAT_R16_SINT                     = 59,
		RENDER_DEVICE_FORMAT_R8_TYPELESS                  = 60,
		RENDER_DEVICE_FORMAT_R8_UNORM                     = 61,
		RENDER_DEVICE_FORMAT_R8_UINT                      = 62,
		RENDER_DEVICE_FORMAT_R8_SNORM                     = 63,
		RENDER_DEVICE_FORMAT_R8_SINT                      = 64,
		RENDER_DEVICE_FORMAT_A8_UNORM                     = 65,
		RENDER_DEVICE_FORMAT_R1_UNORM                     = 66,
		RENDER_DEVICE_FORMAT_R9G9B9E5_SHAREDEXP           = 67,
		RENDER_DEVICE_FORMAT_R8G8_B8G8_UNORM              = 68,
		RENDER_DEVICE_FORMAT_G8R8_G8B8_UNORM              = 69,
		RENDER_DEVICE_FORMAT_BC1_TYPELESS                 = 70,
		RENDER_DEVICE_FORMAT_BC1_UNORM                    = 71,
		RENDER_DEVICE_FORMAT_BC1_UNORM_SRGB               = 72,
		RENDER_DEVICE_FORMAT_BC2_TYPELESS                 = 73,
		RENDER_DEVICE_FORMAT_BC2_UNORM                    = 74,
		RENDER_DEVICE_FORMAT_BC2_UNORM_SRGB               = 75,
		RENDER_DEVICE_FORMAT_BC3_TYPELESS                 = 76,
		RENDER_DEVICE_FORMAT_BC3_UNORM                    = 77,
		RENDER_DEVICE_FORMAT_BC3_UNORM_SRGB               = 78,
		RENDER_DEVICE_FORMAT_BC4_TYPELESS                 = 79,
		RENDER_DEVICE_FORMAT_BC4_UNORM                    = 80,
		RENDER_DEVICE_FORMAT_BC4_SNORM                    = 81,
		RENDER_DEVICE_FORMAT_BC5_TYPELESS                 = 82,
		RENDER_DEVICE_FORMAT_BC5_UNORM                    = 83,
		RENDER_DEVICE_FORMAT_BC5_SNORM                    = 84,
		RENDER_DEVICE_FORMAT_B5G6R5_UNORM                 = 85,
		RENDER_DEVICE_FORMAT_B5G5R5A1_UNORM               = 86,
		RENDER_DEVICE_FORMAT_B8G8R8A8_UNORM               = 87,
		RENDER_DEVICE_FORMAT_B8G8R8X8_UNORM               = 88,
		RENDER_DEVICE_FORMAT_R10G10B10_XR_BIAS_A2_UNORM   = 89,
		RENDER_DEVICE_FORMAT_B8G8R8A8_TYPELESS            = 90,
		RENDER_DEVICE_FORMAT_B8G8R8A8_UNORM_SRGB          = 91,
		RENDER_DEVICE_FORMAT_B8G8R8X8_TYPELESS            = 92,
		RENDER_DEVICE_FORMAT_B8G8R8X8_UNORM_SRGB          = 93,
		RENDER_DEVICE_FORMAT_BC6H_TYPELESS                = 94,
		RENDER_DEVICE_FORMAT_BC6H_UF16                    = 95,
		RENDER_DEVICE_FORMAT_BC6H_SF16                    = 96,
		RENDER_DEVICE_FORMAT_BC7_TYPELESS                 = 97,
		RENDER_DEVICE_FORMAT_BC7_UNORM                    = 98,
		RENDER_DEVICE_FORMAT_BC7_UNORM_SRGB               = 99,
		RENDER_DEVICE_FORMAT_AYUV                         = 100,
		RENDER_DEVICE_FORMAT_Y410                         = 101,
		RENDER_DEVICE_FORMAT_Y416                         = 102,
		RENDER_DEVICE_FORMAT_NV12                         = 103,
		RENDER_DEVICE_FORMAT_P010                         = 104,
		RENDER_DEVICE_FORMAT_P016                         = 105,
		RENDER_DEVICE_FORMAT_420_OPAQUE                   = 106,
		RENDER_DEVICE_FORMAT_YUY2                         = 107,
		RENDER_DEVICE_FORMAT_Y210                         = 108,
		RENDER_DEVICE_FORMAT_Y216                         = 109,
		RENDER_DEVICE_FORMAT_NV11                         = 110,
		RENDER_DEVICE_FORMAT_AI44                         = 111,
		RENDER_DEVICE_FORMAT_IA44                         = 112,
		RENDER_DEVICE_FORMAT_P8                           = 113,
		RENDER_DEVICE_FORMAT_A8P8                         = 114,
		RENDER_DEVICE_FORMAT_B4G4R4A4_UNORM               = 115,
		RENDER_DEVICE_FORMAT_FORCE_UINT                   = 0xffffffffUL 
	};

	// compare
	enum RENDER_DEVICE_COMPARISON_FUNC { 
		RENDER_DEVICE_COMPARISON_NEVER           = 1,
		RENDER_DEVICE_COMPARISON_LESS            = 2,
		RENDER_DEVICE_COMPARISON_EQUAL           = 3,
		RENDER_DEVICE_COMPARISON_LESS_EQUAL      = 4,
		RENDER_DEVICE_COMPARISON_GREATER         = 5,
		RENDER_DEVICE_COMPARISON_NOT_EQUAL       = 6,
		RENDER_DEVICE_COMPARISON_GREATER_EQUAL   = 7,
		RENDER_DEVICE_COMPARISON_ALWAYS          = 8 
	};

	// render primitive topology
	enum RENDER_DEVICE_PRIMITIVE_TOPOLOGY 
	{ 
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_UNDEFINED                    = 0,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_POINTLIST                    = 1,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_LINELIST                     = 2,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_LINESTRIP                    = 3,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST                 = 4,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP                = 5,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_LINELIST_ADJ                 = 10,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ                = 11,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ             = 12,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ            = 13,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST    = 33,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST    = 34,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST    = 35,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST    = 36,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST    = 37,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST    = 38,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST    = 39,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST    = 40,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST    = 41,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST   = 42,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST   = 43,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST   = 44,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST   = 45,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST   = 46,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST   = 47,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST   = 48,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST   = 49,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST   = 50,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST   = 51,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST   = 52,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST   = 53,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST   = 54,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST   = 55,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST   = 56,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST   = 57,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST   = 58,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST   = 59,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST   = 60,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST   = 61,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST   = 62,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST   = 63,
		RENDER_DEVICE_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST   = 64 
	};

	// input layout create desc
	struct TLRenderDeviceInputElementDesc {
		TSun::String					   SemanticName;
		TSun::TU32                       SemanticIndex;
		RENDER_DEVICE_FORMAT       Format;
		TSun::TU32                       InputSlot;
		TSun::TU32                       AlignedByteOffset;
		//D3D11_INPUT_CLASSIFICATION InputSlotClass;
		//TSun::TU32                       InstanceDataStepRate;
	};

	// VP struct
	struct TLRenderDeviceViewport
	{
		TSun::TF32 _Width;
		TSun::TF32 _Height;
		TSun::TF32 _MinDepth;
		TSun::TF32 _MaxDepth;
		TSun::TF32 _TopLeftX;
		TSun::TF32 _TopLeftY;
	};

	// render device map type
	enum RENDER_DEVICE_MAP 
	{
		RENDER_DEVICE_MAP_READ                 = 1,
		RENDER_DEVICE_MAP_WRITE                = 2,
		RENDER_DEVICE_MAP_READ_WRITE           = 3,
		RENDER_DEVICE_MAP_WRITE_DISCARD        = 4,
		RENDER_DEVICE_MAP_WRITE_NO_OVERWRITE   = 5 
	};

	// used by map function
	struct TLRenderDeviceMappedSubresource 
	{
		TSun::TVOID *pData;
		TSun::TU32 RowPitch;
		TSun::TU32 DepthPitch;
	};

	// box using by device
	struct TLRenderDeviceBox {
		TSun::TU32 left;
		TSun::TU32 top;
		TSun::TU32 front;
		TSun::TU32 right;
		TSun::TU32 bottom;
		TSun::TU32 back;
	};

	// resource init data
	struct TLRenderDeviceSubresourceData {
		const TSun::TVOID *pSysMem;
		TSun::TU32       SysMemPitch;
		TSun::TU32       SysMemSlicePitch;
	};

	// shader use type
	enum RENDER_DEVICE_SHADER_USE_TYPE
	{
		RENDER_DEVICE_SHADER_USE_VS = 1,
		RENDER_DEVICE_SHADER_USE_HS = 2,
		RENDER_DEVICE_SHADER_USE_DS = 3,
		RENDER_DEVICE_SHADER_USE_GS = 4,
		RENDER_DEVICE_SHADER_USE_PS = 5,
		RENDER_DEVICE_SHADER_USE_CS = 6
	};

	// resource bind flag
	enum RENDER_DEVICE_BIND_FLAG
	{
		RENDER_DEVICE_BIND_FLAG_VERTEX_BUFFER      = 0x1L,		// vb
		RENDER_DEVICE_BIND_FLAG_INDEX_BUFFER       = 0x2L,		// ib
		RENDER_DEVICE_BIND_FLAG_CONSTANT_BUFFER    = 0x4L,		// cb
		RENDER_DEVICE_BIND_FLAG_SHADER_RESOURCE    = 0x8L,		// sr
		RENDER_DEVICE_BIND_FLAG_STREAM_OUTPUT      = 0x10L,	// so
		RENDER_DEVICE_BIND_FLAG_RENDER_TARGET      = 0x20L,	// rt
		RENDER_DEVICE_BIND_FLAG_DEPTH_STENCIL      = 0x40L,	// ds
		RENDER_DEVICE_BIND_FLAG_UNORDERED_ACCESS   = 0x80L,	// ua
		RENDER_DEVICE_BIND_FLAG_DECODER            = 0x200L,	// decoder
		RENDER_DEVICE_BIND_FLAG_VIDEO_ENCODER      = 0x400L	// video encoder
	};

	// resource type
	enum RENDER_DEVICE_RESOURCE_DIMENSION { 
		RENDER_DEVICE_RESOURCE_DIMENSION_UNKNOWN     = 0,
		RENDER_DEVICE_RESOURCE_DIMENSION_BUFFER      = 1,
		RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE1D   = 2,
		RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE2D   = 3,
		RENDER_DEVICE_RESOURCE_DIMENSION_TEXTURE3D   = 4 
	};

	// stream output decl
	struct TLRenderDeviceSODeclarationEntry {
		//TSun::TU32   Stream;
		TSun::String SemanticName;
		TSun::TU32   SemanticIndex;
		TSun::TUByte   StartComponent;
		TSun::TUByte   ComponentCount;
		//TSun::TUByte   OutputSlot;
	};

	// render tex3d create desc
	struct TLRenderDeviceTex3DDesc 
	{
		TSun::TU32        Width;
		TSun::TU32        Height;
		TSun::TU32        Depth;
		TSun::TU32        MipLevels;
		RENDER_DEVICE_FORMAT Format;
		TSun::TU32        BindFlags;
		TSun::TBOOL        CPUAccessFlags;
		TSun::TBOOL		GenerateMips;
	};

	// render tex2d create desc
	struct TLRenderDeviceTex2DDesc 
	{
		TSun::TU32             Width;
		TSun::TU32             Height;
		TSun::TU32             MipLevels;
		TSun::TU32             ArraySize;
		RENDER_DEVICE_FORMAT    Format;
		TSun::TU32			 SampleCount;
		TSun::TU32			 SampleQuality;
		TSun::TU32             BindFlags;
		TSun::TBOOL             CPUAccessFlags;
		TSun::TBOOL			 GenerateMips;
		TSun::TBOOL			 Cube;
	};

	// render tex1d create desc
	struct TLRenderDeviceTex1DDesc 
	{
		TSun::TU32        Width;
		TSun::TU32        MipLevels;
		TSun::TU32        ArraySize;
		RENDER_DEVICE_FORMAT Format;
		TSun::TU32        BindFlags;
		TSun::TBOOL        CPUAccessFlags;
		TSun::TBOOL		GenerateMips;
	};

	// buffer create desc
	struct TLRenderDeviceBufferDesc 
	{
		TSun::TU32        ByteWidth;
		TSun::TU32        BindFlags;
		TSun::TBOOL        CPUAccessFlags;
	};

	// srv used resource
	enum RENDER_DEVICE_SRV_DIMENSION { 
		RENDER_DEVICE_SRV_DIMENSION_UNKNOWN            = 0,
		RENDER_DEVICE_SRV_DIMENSION_BUFFER             = 1,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE1D          = 2,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE1DARRAY     = 3,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE2D          = 4,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE2DARRAY     = 5,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE2DMS        = 6,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE2DMSARRAY   = 7,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURE3D          = 8,
		RENDER_DEVICE_SRV_DIMENSION_TEXTURECUBE        = 9,
		//RENDER_DEVICE_SRV_DIMENSION_TEXTURECUBEARRAY   = 10,
		//RENDER_DEVICE_SRV_DIMENSION_BUFFEREX           = 11 
	};

	// srv create desc
	struct TLRenderDeviceSRVDesc 
	{
		RENDER_DEVICE_FORMAT         Format;
		RENDER_DEVICE_SRV_DIMENSION ViewDimension;
		// for buffer
		// Params[0] for NumElements
		// Params[1] for ElementWidth
		// for texture (1D,2D,3D,Cube)
		// Params[0] for MostDetailedMip
		// Params[1] for MipLevels
		// for texture array (1D,2D)
		// Params[0] for MostDetailedMip
		// Params[1] for MipLevels
		// Params[2] for FirstArraySlice
		// Params[3] for ArraySize
		// for texture 2D ms
		// Params not used
		// for texture 2D ms array
		// Params[0] for FirstArraySlice
		// Params[1] for ArraySize
		TSun::TU32 Params[4];
	};

	// rtv used resource
	enum RENDER_DEVICE_RTV_DIMENSION { 
		RENDER_DEVICE_RTV_DIMENSION_UNKNOWN            = 0,
		RENDER_DEVICE_RTV_DIMENSION_BUFFER             = 1,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE1D          = 2,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE1DARRAY     = 3,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE2D          = 4,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE2DARRAY     = 5,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE2DMS        = 6,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE2DMSARRAY   = 7,
		RENDER_DEVICE_RTV_DIMENSION_TEXTURE3D          = 8 
	};

	// rtv create desc
	struct TLRenderDeviceRTVDesc {
		RENDER_DEVICE_FORMAT         Format;
		RENDER_DEVICE_RTV_DIMENSION ViewDimension;
		// for buffer
		// Params[0] for NumElements
		// Params[1] for ElementWidth
		// for texture (1D,2D)
		// Params[0] for MipSlice
		// for texture array (1D,2D)
		// Params[0] for MipSlice
		// Params[1] for FirstArraySlice
		// Params[2] for ArraySize
		// for texture 3D
		// Params[0] for MipSlice
		// Params[1] for FirstWSlice
		// Params[2] for WSize
		// for texture 2D ms
		// Params not used
		// for texture 2D ms array
		// Params[0] for FirstArraySlice
		// Params[1] for ArraySize
		TSun::TU32 Params[3];
	};

	// dsv used resource
	enum RENDER_DEVICE_DSV_DIMENSION { 
		RENDER_DEVICE_DSV_DIMENSION_UNKNOWN            = 0,
		RENDER_DEVICE_DSV_DIMENSION_TEXTURE1D          = 1,
		RENDER_DEVICE_DSV_DIMENSION_TEXTURE1DARRAY     = 2,
		RENDER_DEVICE_DSV_DIMENSION_TEXTURE2D          = 3,
		RENDER_DEVICE_DSV_DIMENSION_TEXTURE2DARRAY     = 4,
		RENDER_DEVICE_DSV_DIMENSION_TEXTURE2DMS        = 5,
		RENDER_DEVICE_DSV_DIMENSION_TEXTURE2DMSARRAY   = 6 
	};

	// dsv create desc
	struct TLRenderDeviceDSVDesc {
		RENDER_DEVICE_FORMAT         Format;
		RENDER_DEVICE_DSV_DIMENSION ViewDimension;
		//TSun::TU32                Flags;
		// for texture (1D,2D)
		// Params[0] for MipSlice
		// for texture array (1D,2D)
		// Params[0] for MipSlice
		// Params[1] for FirstArraySlice
		// Params[2] for ArraySize
		// for texture 2D ms
		// Params not used
		// for texture 2D ms array
		// Params[0] for FirstArraySlice
		// Params[1] for ArraySize
		TSun::TU32 Params[3];
	};

	// uav used resource type
	enum RENDER_DEVICE_UAV_DIMENSION { 
		RENDER_DEVICE_UAV_DIMENSION_UNKNOWN          = 0,
		RENDER_DEVICE_UAV_DIMENSION_BUFFER           = 1,
		RENDER_DEVICE_UAV_DIMENSION_TEXTURE1D        = 2,
		RENDER_DEVICE_UAV_DIMENSION_TEXTURE1DARRAY   = 3,
		RENDER_DEVICE_UAV_DIMENSION_TEXTURE2D        = 4,
		RENDER_DEVICE_UAV_DIMENSION_TEXTURE2DARRAY   = 5,
		RENDER_DEVICE_UAV_DIMENSION_TEXTURE3D        = 8 
	};

	// uav flag
	enum RENDER_DEVICE_BUFFER_UAV_FLAG { 
		RENDER_DEVICE_BUFFER_UAV_FLAG_RAW       = 0x1,
		RENDER_DEVICE_BUFFER_UAV_FLAG_APPEND    = 0x2,
		RENDER_DEVICE_BUFFER_UAV_FLAG_COUNTER   = 0x4 
	};

	// uav create desc
	struct TLRenderDeviceUAVDesc {
		RENDER_DEVICE_FORMAT         Format;
		RENDER_DEVICE_UAV_DIMENSION ViewDimension;
		// for buffer
		// Params[0] for FirstElement
		// Params[1] for NumElements
		// Params[2] for Flags
		// for texture (1D,2D)
		// Params[0] for MipSlice
		// for texture array (1D,2D)
		// Params[0] for MipSlice
		// Params[1] for FirstArraySlice
		// Params[2] for ArraySize
		// for texture 3D
		// Params[0] for MipSlice
		// Params[1] for FirstWSlice
		// Params[2] for WSize
		TSun::TU32 Params[3];
	};

	// rasterizer fill mode
	enum RENDER_DEVICE_FILL_MODE { 
		RENDER_DEVICE_FILL_WIREFRAME   = 2,
		RENDER_DEVICE_FILL_SOLID       = 3 
	};

	// rasterizer cull mode
	enum RENDER_DEVICE_CULL_MODE { 
		RENDER_DEVICE_CULL_NONE    = 1,
		RENDER_DEVICE_CULL_FRONT   = 2,
		RENDER_DEVICE_CULL_BACK    = 3 
	};

	// rasterizer create desc
	struct TLRenderDeviceRasterizerDesc {
		RENDER_DEVICE_FILL_MODE FillMode;
		RENDER_DEVICE_CULL_MODE CullMode;
		//TSun::TBOOL            FrontCounterClockwise;
		//TSun::TS32             DepthBias;
		//TSun::TF32           DepthBiasClamp;
		//TSun::TF32           SlopeScaledDepthBias;
		//TSun::TBOOL            DepthClipEnable;
		//TSun::TBOOL            ScissorEnable;
		TSun::TBOOL            MultisampleEnable;
		TSun::TBOOL            AntialiasedLineEnable;

		TLRenderDeviceRasterizerDesc()
		{
			FillMode = RENDER_DEVICE_FILL_SOLID;
			CullMode = RENDER_DEVICE_CULL_BACK;
			MultisampleEnable = TSun::TFALSE;
			AntialiasedLineEnable = TSun::TFALSE;
		};
	};

	// depth write mask
	enum RENDER_DEVICE_DEPTH_WRITE_MASK { 
		RENDER_DEVICE_DEPTH_WRITE_MASK_ZERO   = 0,
		RENDER_DEVICE_DEPTH_WRITE_MASK_ALL    = 1 
	};

	// depth stencil create desc
	// 模板不支持
	struct TLRenderDeviceDepthStencilDesc {
		TSun::TBOOL                       DepthEnable;
		RENDER_DEVICE_DEPTH_WRITE_MASK     DepthWriteMask;
		RENDER_DEVICE_COMPARISON_FUNC      DepthFunc;
		//TSun::TBOOL                       StencilEnable;
		//UINT8                      StencilReadMask;
		//UINT8                      StencilWriteMask;
		//D3D11_DEPTH_STENCILOP_DESC FrontFace;
		//D3D11_DEPTH_STENCILOP_DESC BackFace;

		TLRenderDeviceDepthStencilDesc()
		{
			DepthEnable = TSun::TTRUE;
			DepthWriteMask = RENDER_DEVICE_DEPTH_WRITE_MASK_ALL;
			DepthFunc = RENDER_DEVICE_COMPARISON_LESS;
		};
	};

	// blend type
	enum RENDER_DEVICE_BLEND { 
		RENDER_DEVICE_BLEND_ZERO               = 1,
		RENDER_DEVICE_BLEND_ONE                = 2,
		RENDER_DEVICE_BLEND_SRC_COLOR          = 3,
		RENDER_DEVICE_BLEND_INV_SRC_COLOR      = 4,
		RENDER_DEVICE_BLEND_SRC_ALPHA          = 5,
		RENDER_DEVICE_BLEND_INV_SRC_ALPHA      = 6,
		RENDER_DEVICE_BLEND_DEST_ALPHA         = 7,
		RENDER_DEVICE_BLEND_INV_DEST_ALPHA     = 8,
		RENDER_DEVICE_BLEND_DEST_COLOR         = 9,
		RENDER_DEVICE_BLEND_INV_DEST_COLOR     = 10,
		RENDER_DEVICE_BLEND_SRC_ALPHA_SAT      = 11,
		RENDER_DEVICE_BLEND_BLEND_FACTOR       = 14,
		RENDER_DEVICE_BLEND_INV_BLEND_FACTOR   = 15,
		RENDER_DEVICE_BLEND_SRC1_COLOR         = 16,
		RENDER_DEVICE_BLEND_INV_SRC1_COLOR     = 17,
		RENDER_DEVICE_BLEND_SRC1_ALPHA         = 18,
		RENDER_DEVICE_BLEND_INV_SRC1_ALPHA     = 19 
	};

	// blend op
	enum RENDER_DEVICE_BLEND_OP { 
		RENDER_DEVICE_BLEND_OP_ADD            = 1,
		RENDER_DEVICE_BLEND_OP_SUBTRACT       = 2,
		RENDER_DEVICE_BLEND_OP_REV_SUBTRACT   = 3,
		RENDER_DEVICE_BLEND_OP_MIN            = 4,
		RENDER_DEVICE_BLEND_OP_MAX            = 5 
	};

	// color write flag
	enum RENDER_DEVICE_COLOR_WRITE_ENABLE { 
		RENDER_DEVICE_COLOR_WRITE_ENABLE_RED     = 1,
		RENDER_DEVICE_COLOR_WRITE_ENABLE_GREEN   = 2,
		RENDER_DEVICE_COLOR_WRITE_ENABLE_BLUE    = 4,
		RENDER_DEVICE_COLOR_WRITE_ENABLE_ALPHA   = 8,
		RENDER_DEVICE_COLOR_WRITE_ENABLE_ALL     = 
			( RENDER_DEVICE_COLOR_WRITE_ENABLE_RED | RENDER_DEVICE_COLOR_WRITE_ENABLE_GREEN |  
			RENDER_DEVICE_COLOR_WRITE_ENABLE_BLUE | RENDER_DEVICE_COLOR_WRITE_ENABLE_ALPHA ) 
	};

	// render target blend desc
	struct TLRenderDeviceRenderTargetBlendDesc {
		TSun::TBOOL           BlendEnable;
		RENDER_DEVICE_BLEND    SrcBlend;
		RENDER_DEVICE_BLEND    DestBlend;
		RENDER_DEVICE_BLEND_OP BlendOp;
		RENDER_DEVICE_BLEND    SrcBlendAlpha;
		RENDER_DEVICE_BLEND    DestBlendAlpha;
		RENDER_DEVICE_BLEND_OP BlendOpAlpha;
		UINT8          RenderTargetWriteMask;	// RENDER_DEVICE_COLOR_WRITE_ENABLE
	};

	// blend create desc
	struct TLRenderDeviceBlendDesc {
		//TSun::TBOOL                           AlphaToCoverageEnable;
		//TSun::TBOOL                           IndependentBlendEnable;
		TLRenderDeviceRenderTargetBlendDesc RenderTarget[1];	// 只用第一个

		TLRenderDeviceBlendDesc()
		{
			RenderTarget[0].BlendEnable = TSun::TFALSE;
			RenderTarget[0].SrcBlend = RENDER_DEVICE_BLEND_ONE;
			RenderTarget[0].DestBlend = RENDER_DEVICE_BLEND_ZERO;
			RenderTarget[0].BlendOp = RENDER_DEVICE_BLEND_OP_ADD;
			RenderTarget[0].SrcBlendAlpha = RENDER_DEVICE_BLEND_ONE;
			RenderTarget[0].DestBlendAlpha = RENDER_DEVICE_BLEND_ZERO;
			RenderTarget[0].BlendOpAlpha = RENDER_DEVICE_BLEND_OP_ADD;
			RenderTarget[0].RenderTargetWriteMask = RENDER_DEVICE_COLOR_WRITE_ENABLE_ALL;
		};
	};

	// sampler filter flag
	enum RENDER_DEVICE_FILTER { 
		RENDER_DEVICE_FILTER_MIN_MAG_MIP_POINT                            = 0,
		RENDER_DEVICE_FILTER_MIN_MAG_POINT_MIP_LINEAR                     = 0x1,
		RENDER_DEVICE_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT               = 0x4,
		RENDER_DEVICE_FILTER_MIN_POINT_MAG_MIP_LINEAR                     = 0x5,
		RENDER_DEVICE_FILTER_MIN_LINEAR_MAG_MIP_POINT                     = 0x10,
		RENDER_DEVICE_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR              = 0x11,
		RENDER_DEVICE_FILTER_MIN_MAG_LINEAR_MIP_POINT                     = 0x14,
		RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR                           = 0x15,
		RENDER_DEVICE_FILTER_ANISOTROPIC                                  = 0x55,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_MAG_MIP_POINT                 = 0x80,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR          = 0x81,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT    = 0x84,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR          = 0x85,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT          = 0x90,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR   = 0x91,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT          = 0x94,
		RENDER_DEVICE_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR                = 0x95,
		RENDER_DEVICE_FILTER_COMPARISON_ANISOTROPIC                       = 0xd5,
		RENDER_DEVICE_FILTER_TEXT_1BIT                                    = 0x80000000 
	};

	// sampler texture address mode
	enum RENDER_DEVICE_TEXTURE_ADDRESS_MODE { 
		RENDER_DEVICE_TEXTURE_ADDRESS_WRAP          = 1,
		RENDER_DEVICE_TEXTURE_ADDRESS_MIRROR        = 2,
		RENDER_DEVICE_TEXTURE_ADDRESS_CLAMP         = 3,
		RENDER_DEVICE_TEXTURE_ADDRESS_BORDER        = 4,
		RENDER_DEVICE_TEXTURE_ADDRESS_MIRROR_ONCE   = 5 
	};

	// sampler create desc
	struct TLRenderDeviceSamplerDesc {
		RENDER_DEVICE_FILTER               Filter;
		RENDER_DEVICE_TEXTURE_ADDRESS_MODE AddressU;
		RENDER_DEVICE_TEXTURE_ADDRESS_MODE AddressV;
		RENDER_DEVICE_TEXTURE_ADDRESS_MODE AddressW;
		TSun::TF32                      MipLODBias;
		TSun::TU32                       MaxAnisotropy;
		//D3D11_COMPARISON_FUNC      ComparisonFunc;
		TSun::TF32                      BorderColor[4];
		TSun::TF32                      MinLOD;
		TSun::TF32                      MaxLOD;

		TLRenderDeviceSamplerDesc()
		{
			Filter = RENDER_DEVICE_FILTER_MIN_MAG_MIP_LINEAR;
			AddressU = RENDER_DEVICE_TEXTURE_ADDRESS_CLAMP;
			AddressV = RENDER_DEVICE_TEXTURE_ADDRESS_CLAMP;
			AddressW = RENDER_DEVICE_TEXTURE_ADDRESS_CLAMP;
			MipLODBias = 0.0f;
			MaxAnisotropy = 16;
			BorderColor[0] = BorderColor[1] = BorderColor[2] = BorderColor[3] = 0.0f;
			MinLOD = -(TSun::TF32)TSun::T_FLT_MAX;
			MaxLOD = (TSun::TF32)TSun::T_FLT_MAX;
		};
	};
}

#endif