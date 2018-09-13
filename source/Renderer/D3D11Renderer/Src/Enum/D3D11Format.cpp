#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	DXGI_FORMAT convert( renderer::Format const & format )noexcept
	{
		switch ( format )
		{
		case renderer::Format::eUndefined: return DXGI_FORMAT_UNKNOWN;
		case renderer::Format::eR4G4B4A4_UNORM_PACK16: return DXGI_FORMAT_B4G4R4A4_UNORM;
		case renderer::Format::eB4G4R4A4_UNORM_PACK16: return DXGI_FORMAT_B4G4R4A4_UNORM;
		case renderer::Format::eR5G6B5_UNORM_PACK16: return DXGI_FORMAT_B5G6R5_UNORM;
		case renderer::Format::eB5G6R5_UNORM_PACK16: return DXGI_FORMAT_B5G6R5_UNORM;
		case renderer::Format::eR5G5B5A1_UNORM_PACK16: return DXGI_FORMAT_B5G5R5A1_UNORM;
		case renderer::Format::eB5G5R5A1_UNORM_PACK16: return DXGI_FORMAT_B5G5R5A1_UNORM;
		case renderer::Format::eA1R5G5B5_UNORM_PACK16: return DXGI_FORMAT_B5G5R5A1_UNORM;
		case renderer::Format::eR8_UNORM: return DXGI_FORMAT_R8_UNORM;
		case renderer::Format::eR8_SNORM: return DXGI_FORMAT_R8_SNORM;
		case renderer::Format::eR8_USCALED: return DXGI_FORMAT_R8_UNORM;
		case renderer::Format::eR8_SSCALED: return DXGI_FORMAT_R8_SNORM;
		case renderer::Format::eR8_UINT: return DXGI_FORMAT_R8_UINT;
		case renderer::Format::eR8_SINT: return DXGI_FORMAT_R8_SINT;
		case renderer::Format::eR8_SRGB: return DXGI_FORMAT_R8_SINT;
		case renderer::Format::eR8G8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
		case renderer::Format::eR8G8_SNORM: return DXGI_FORMAT_R8G8_SNORM;
		case renderer::Format::eR8G8_USCALED: return DXGI_FORMAT_R8G8_UNORM;
		case renderer::Format::eR8G8_SSCALED: return DXGI_FORMAT_R8G8_SNORM;
		case renderer::Format::eR8G8_UINT: return DXGI_FORMAT_R8G8_UINT;
		case renderer::Format::eR8G8_SINT: return DXGI_FORMAT_R8G8_SINT;
		case renderer::Format::eR8G8_SRGB: return DXGI_FORMAT_R8G8_SINT;
		case renderer::Format::eR8G8B8_UNORM: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case renderer::Format::eR8G8B8_SNORM: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case renderer::Format::eR8G8B8_USCALED: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case renderer::Format::eR8G8B8_SSCALED: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case renderer::Format::eR8G8B8_UINT: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case renderer::Format::eR8G8B8_SINT: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case renderer::Format::eR8G8B8_SRGB: return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
		case renderer::Format::eB8G8R8_UNORM: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case renderer::Format::eB8G8R8_SNORM: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case renderer::Format::eB8G8R8_USCALED: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case renderer::Format::eB8G8R8_SSCALED: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case renderer::Format::eB8G8R8_UINT: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case renderer::Format::eB8G8R8_SINT: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case renderer::Format::eB8G8R8_SRGB: return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
		case renderer::Format::eR8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case renderer::Format::eR8G8B8A8_SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
		case renderer::Format::eR8G8B8A8_USCALED: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case renderer::Format::eR8G8B8A8_SSCALED: return DXGI_FORMAT_R8G8B8A8_SNORM;
		case renderer::Format::eR8G8B8A8_UINT: return DXGI_FORMAT_R8G8B8A8_UINT;
		case renderer::Format::eR8G8B8A8_SINT: return DXGI_FORMAT_R8G8B8A8_SINT;
		case renderer::Format::eR8G8B8A8_SRGB: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case renderer::Format::eB8G8R8A8_UNORM: return DXGI_FORMAT_B8G8R8A8_UNORM;
		case renderer::Format::eB8G8R8A8_SNORM: return DXGI_FORMAT_B8G8R8A8_TYPELESS;
		case renderer::Format::eB8G8R8A8_USCALED: return DXGI_FORMAT_B8G8R8A8_UNORM;
		case renderer::Format::eB8G8R8A8_SSCALED: return DXGI_FORMAT_B8G8R8A8_TYPELESS;
		case renderer::Format::eB8G8R8A8_UINT: return DXGI_FORMAT_B8G8R8A8_UNORM;
		case renderer::Format::eB8G8R8A8_SINT: return DXGI_FORMAT_B8G8R8A8_TYPELESS;
		case renderer::Format::eB8G8R8A8_SRGB: return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case renderer::Format::eA2R10G10B10_UNORM_PACK32: return DXGI_FORMAT_R10G10B10A2_UNORM;
		case renderer::Format::eA2R10G10B10_SNORM_PACK32: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		case renderer::Format::eA2R10G10B10_USCALED_PACK32: return DXGI_FORMAT_R10G10B10A2_UNORM;
		case renderer::Format::eA2R10G10B10_SSCALED_PACK32: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		case renderer::Format::eA2R10G10B10_UINT_PACK32: return DXGI_FORMAT_R10G10B10A2_UINT;
		case renderer::Format::eA2R10G10B10_SINT_PACK32: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		case renderer::Format::eR16_UNORM: return DXGI_FORMAT_R16_UNORM;
		case renderer::Format::eR16_SNORM: return DXGI_FORMAT_R16_SNORM;
		case renderer::Format::eR16_USCALED: return DXGI_FORMAT_R16_UNORM;
		case renderer::Format::eR16_SSCALED: return DXGI_FORMAT_R16_SNORM;
		case renderer::Format::eR16_UINT: return DXGI_FORMAT_R16_UINT;
		case renderer::Format::eR16_SINT: return DXGI_FORMAT_R16_SINT;
		case renderer::Format::eR16_SFLOAT: return DXGI_FORMAT_R16_FLOAT;
		case renderer::Format::eR16G16_UNORM: return DXGI_FORMAT_R16G16_UNORM;
		case renderer::Format::eR16G16_SNORM: return DXGI_FORMAT_R16G16_SNORM;
		case renderer::Format::eR16G16_USCALED: return DXGI_FORMAT_R16G16_UNORM;
		case renderer::Format::eR16G16_SSCALED: return DXGI_FORMAT_R16G16_SNORM;
		case renderer::Format::eR16G16_UINT: return DXGI_FORMAT_R16G16_UINT;
		case renderer::Format::eR16G16_SINT: return DXGI_FORMAT_R16G16_SINT;
		case renderer::Format::eR16G16_SFLOAT: return DXGI_FORMAT_R16G16_FLOAT;
		case renderer::Format::eR16G16B16A16_UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
		case renderer::Format::eR16G16B16A16_SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
		case renderer::Format::eR16G16B16A16_USCALED: return DXGI_FORMAT_R16G16B16A16_UNORM;
		case renderer::Format::eR16G16B16A16_SSCALED: return DXGI_FORMAT_R16G16B16A16_SNORM;
		case renderer::Format::eR16G16B16A16_UINT: return DXGI_FORMAT_R16G16B16A16_UINT;
		case renderer::Format::eR16G16B16A16_SINT: return DXGI_FORMAT_R16G16B16A16_SINT;
		case renderer::Format::eR16G16B16A16_SFLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case renderer::Format::eR32_UINT: return DXGI_FORMAT_R32_UINT;
		case renderer::Format::eR32_SINT: return DXGI_FORMAT_R32_SINT;
		case renderer::Format::eR32_SFLOAT: return DXGI_FORMAT_R32_FLOAT;
		case renderer::Format::eR32G32_UINT: return DXGI_FORMAT_R32G32_UINT;
		case renderer::Format::eR32G32_SINT: return DXGI_FORMAT_R32G32_SINT;
		case renderer::Format::eR32G32_SFLOAT: return DXGI_FORMAT_R32G32_FLOAT;
		case renderer::Format::eR32G32B32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
		case renderer::Format::eR32G32B32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
		case renderer::Format::eR32G32B32_SFLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
		case renderer::Format::eR32G32B32A32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
		case renderer::Format::eR32G32B32A32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
		case renderer::Format::eR32G32B32A32_SFLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case renderer::Format::eB10G11R11_UFLOAT_PACK32: return DXGI_FORMAT_R11G11B10_FLOAT;
		case renderer::Format::eE5B9G9R9_UFLOAT_PACK32: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
		case renderer::Format::eD16_UNORM: return DXGI_FORMAT_D16_UNORM;
		case renderer::Format::eX8_D24_UNORM_PACK32: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case renderer::Format::eD32_SFLOAT: return DXGI_FORMAT_D32_FLOAT;
		case renderer::Format::eD24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case renderer::Format::eD32_SFLOAT_S8_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		case renderer::Format::eBC1_RGB_UNORM_BLOCK: return DXGI_FORMAT_BC1_UNORM;
		case renderer::Format::eBC1_RGB_SRGB_BLOCK: return DXGI_FORMAT_BC1_UNORM_SRGB;
		case renderer::Format::eBC2_UNORM_BLOCK: return DXGI_FORMAT_BC2_UNORM;
		case renderer::Format::eBC2_SRGB_BLOCK: return DXGI_FORMAT_BC2_UNORM_SRGB;
		case renderer::Format::eBC3_UNORM_BLOCK: return DXGI_FORMAT_BC3_UNORM;
		case renderer::Format::eBC3_SRGB_BLOCK: return DXGI_FORMAT_BC3_UNORM_SRGB;
		case renderer::Format::eBC4_UNORM_BLOCK: return DXGI_FORMAT_BC4_UNORM;
		case renderer::Format::eBC4_SNORM_BLOCK: return DXGI_FORMAT_BC4_SNORM;
		case renderer::Format::eBC5_UNORM_BLOCK: return DXGI_FORMAT_BC5_UNORM;
		case renderer::Format::eBC5_SNORM_BLOCK: return DXGI_FORMAT_BC5_SNORM;
		case renderer::Format::eBC7_UNORM_BLOCK: return DXGI_FORMAT_BC7_UNORM;
		case renderer::Format::eBC7_SRGB_BLOCK: return DXGI_FORMAT_BC7_UNORM_SRGB;

		default:
			assert( false && "Unsupported pixel format." );
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	renderer::Format convert( DXGI_FORMAT const & format )noexcept
	{
		switch ( format )
		{
		case DXGI_FORMAT_UNKNOWN: return renderer::Format::eUndefined;
		case DXGI_FORMAT_B4G4R4A4_UNORM: return renderer::Format::eB4G4R4A4_UNORM_PACK16;
		case DXGI_FORMAT_B5G6R5_UNORM: return renderer::Format::eB5G6R5_UNORM_PACK16;
		case DXGI_FORMAT_B5G5R5A1_UNORM: return renderer::Format::eB5G5R5A1_UNORM_PACK16;
		case DXGI_FORMAT_R8_UNORM: return renderer::Format::eR8_UNORM;
		case DXGI_FORMAT_R8_SNORM: return renderer::Format::eR8_SNORM;
		case DXGI_FORMAT_R8_UINT: return renderer::Format::eR8_UINT;
		case DXGI_FORMAT_R8_SINT: return renderer::Format::eR8_SINT;
		case DXGI_FORMAT_R8G8_UNORM: return renderer::Format::eR8G8_UNORM;
		case DXGI_FORMAT_R8G8_SNORM: return renderer::Format::eR8G8_SNORM;
		case DXGI_FORMAT_R8G8_UINT: return renderer::Format::eR8G8_UINT;
		case DXGI_FORMAT_R8G8_SINT: return renderer::Format::eR8G8_SINT;
		case DXGI_FORMAT_B8G8R8X8_UNORM: return renderer::Format::eB8G8R8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_TYPELESS: return renderer::Format::eB8G8R8_SNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: return renderer::Format::eB8G8R8_SRGB;
		case DXGI_FORMAT_R8G8B8A8_UNORM: return renderer::Format::eR8G8B8A8_UNORM;
		case DXGI_FORMAT_R8G8B8A8_SNORM: return renderer::Format::eR8G8B8A8_SNORM;
		case DXGI_FORMAT_R8G8B8A8_UINT: return renderer::Format::eR8G8B8A8_UINT;
		case DXGI_FORMAT_R8G8B8A8_SINT: return renderer::Format::eR8G8B8A8_SINT;
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return renderer::Format::eR8G8B8A8_SRGB;
		case DXGI_FORMAT_B8G8R8A8_UNORM: return renderer::Format::eB8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_TYPELESS: return renderer::Format::eB8G8R8A8_SNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return renderer::Format::eB8G8R8A8_SRGB;
		case DXGI_FORMAT_R10G10B10A2_UNORM: return renderer::Format::eA2R10G10B10_UNORM_PACK32;
		case DXGI_FORMAT_R10G10B10A2_TYPELESS: return renderer::Format::eA2R10G10B10_SNORM_PACK32;
		case DXGI_FORMAT_R10G10B10A2_UINT: return renderer::Format::eA2R10G10B10_UINT_PACK32;
		case DXGI_FORMAT_R16_UNORM: return renderer::Format::eR16_UNORM;
		case DXGI_FORMAT_R16_SNORM: return renderer::Format::eR16_SNORM;
		case DXGI_FORMAT_R16_UINT: return renderer::Format::eR16_UINT;
		case DXGI_FORMAT_R16_SINT: return renderer::Format::eR16_SINT;
		case DXGI_FORMAT_R16_FLOAT: return renderer::Format::eR16_SFLOAT;
		case DXGI_FORMAT_R16G16_UNORM: return renderer::Format::eR16G16_UNORM;
		case DXGI_FORMAT_R16G16_SNORM: return renderer::Format::eR16G16_SNORM;
		case DXGI_FORMAT_R16G16_UINT: return renderer::Format::eR16G16_UINT;
		case DXGI_FORMAT_R16G16_SINT: return renderer::Format::eR16G16_SINT;
		case DXGI_FORMAT_R16G16_FLOAT: return renderer::Format::eR16G16_SFLOAT;
		case DXGI_FORMAT_R16G16B16A16_UNORM: return renderer::Format::eR16G16B16A16_UNORM;
		case DXGI_FORMAT_R16G16B16A16_SNORM: return renderer::Format::eR16G16B16A16_SNORM;
		case DXGI_FORMAT_R16G16B16A16_UINT: return renderer::Format::eR16G16B16A16_UINT;
		case DXGI_FORMAT_R16G16B16A16_SINT: return renderer::Format::eR16G16B16A16_SINT;
		case DXGI_FORMAT_R16G16B16A16_FLOAT: return renderer::Format::eR16G16B16A16_SFLOAT;
		case DXGI_FORMAT_R32_UINT: return renderer::Format::eR32_UINT;
		case DXGI_FORMAT_R32_SINT: return renderer::Format::eR32_SINT;
		case DXGI_FORMAT_R32_FLOAT: return renderer::Format::eR32_SFLOAT;
		case DXGI_FORMAT_R32G32_UINT: return renderer::Format::eR32G32_UINT;
		case DXGI_FORMAT_R32G32_SINT: return renderer::Format::eR32G32_SINT;
		case DXGI_FORMAT_R32G32_FLOAT: return renderer::Format::eR32G32_SFLOAT;
		case DXGI_FORMAT_R32G32B32_UINT: return renderer::Format::eR32G32B32_UINT;
		case DXGI_FORMAT_R32G32B32_SINT: return renderer::Format::eR32G32B32_SINT;
		case DXGI_FORMAT_R32G32B32_FLOAT: return renderer::Format::eR32G32B32_SFLOAT;
		case DXGI_FORMAT_R32G32B32A32_UINT: return renderer::Format::eR32G32B32A32_UINT;
		case DXGI_FORMAT_R32G32B32A32_SINT: return renderer::Format::eR32G32B32A32_SINT;
		case DXGI_FORMAT_R32G32B32A32_FLOAT: return renderer::Format::eR32G32B32A32_SFLOAT;
		case DXGI_FORMAT_R11G11B10_FLOAT: return renderer::Format::eB10G11R11_UFLOAT_PACK32;
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: return renderer::Format::eE5B9G9R9_UFLOAT_PACK32;
		case DXGI_FORMAT_D16_UNORM: return renderer::Format::eD16_UNORM;
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS: return renderer::Format::eX8_D24_UNORM_PACK32;
		case DXGI_FORMAT_D32_FLOAT: return renderer::Format::eD32_SFLOAT;
		case DXGI_FORMAT_D24_UNORM_S8_UINT: return renderer::Format::eD24_UNORM_S8_UINT;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return renderer::Format::eD32_SFLOAT_S8_UINT;
		case DXGI_FORMAT_BC1_UNORM: return renderer::Format::eBC1_RGB_UNORM_BLOCK;
		case DXGI_FORMAT_BC1_UNORM_SRGB: return renderer::Format::eBC1_RGB_SRGB_BLOCK;
		case DXGI_FORMAT_BC2_UNORM: return renderer::Format::eBC2_UNORM_BLOCK;
		case DXGI_FORMAT_BC2_UNORM_SRGB: return renderer::Format::eBC2_SRGB_BLOCK;
		case DXGI_FORMAT_BC3_UNORM: return renderer::Format::eBC3_UNORM_BLOCK;
		case DXGI_FORMAT_BC3_UNORM_SRGB: return renderer::Format::eBC3_SRGB_BLOCK;
		case DXGI_FORMAT_BC4_UNORM: return renderer::Format::eBC4_UNORM_BLOCK;
		case DXGI_FORMAT_BC4_SNORM: return renderer::Format::eBC4_SNORM_BLOCK;
		case DXGI_FORMAT_BC5_UNORM: return renderer::Format::eBC5_UNORM_BLOCK;
		case DXGI_FORMAT_BC5_SNORM: return renderer::Format::eBC5_SNORM_BLOCK;
		case DXGI_FORMAT_BC7_UNORM: return renderer::Format::eBC7_UNORM_BLOCK;
		case DXGI_FORMAT_BC7_UNORM_SRGB: return renderer::Format::eBC7_SRGB_BLOCK;
		default:
			assert( false && "Unsupported pixel format." );
			return renderer::Format::eR8G8B8A8_UNORM;
		}
	}
}
