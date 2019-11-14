#include "Image/D3D11Sampler.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	D3D11_TEXTURE_ADDRESS_MODE convert( VkSamplerAddressMode mode )
	{
		switch ( mode )
		{
		case VK_SAMPLER_ADDRESS_MODE_REPEAT:
			return D3D11_TEXTURE_ADDRESS_WRAP;

		case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
			return D3D11_TEXTURE_ADDRESS_MIRROR;

		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:
			return D3D11_TEXTURE_ADDRESS_CLAMP;

		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:
			return D3D11_TEXTURE_ADDRESS_BORDER;

		case VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE:
			return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		}

		assert( false );
		return D3D11_TEXTURE_ADDRESS_WRAP;
	}

	void convert( VkBorderColor colour
		, float output[4] )
	{
		switch ( colour )
		{
		case VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK:
		case VK_BORDER_COLOR_INT_TRANSPARENT_BLACK:
			output[0] = 0.0;
			output[1] = 0.0;
			output[2] = 0.0;
			output[3] = 0.0;
			break;
		case VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK:
		case VK_BORDER_COLOR_INT_OPAQUE_BLACK:
			output[0] = 0.0;
			output[1] = 0.0;
			output[2] = 0.0;
			output[3] = 1.0;
			break;
		case VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE:
		case VK_BORDER_COLOR_INT_OPAQUE_WHITE:
			output[0] = 1.0;
			output[1] = 1.0;
			output[2] = 1.0;
			output[3] = 1.0;
			break;
		}
	}

	D3D11_FILTER convert( VkFilter min
		, VkFilter mag
		, VkSamplerMipmapMode mip
		, float maxAnisotropy )
	{
		if ( maxAnisotropy > 1.0 )
		{
			return D3D11_FILTER_ANISOTROPIC;
		}

		if ( min == VK_FILTER_NEAREST )
		{
			if ( mag == VK_FILTER_NEAREST )
			{
				switch ( mip )
				{
				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

				default:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;
				}
			}
			else
			{
				switch ( mip )
				{
				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

				default:
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
				}
			}
		}
		else
		{
			if ( mag == VK_FILTER_NEAREST )
			{
				switch ( mip )
				{
				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

				default:
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
				}
			}
			else
			{
				switch ( mip )
				{
				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

				default:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
				}
			}
		}

		assert( false );
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	Sampler::Sampler( VkDevice device
		, VkSamplerCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		auto d3ddevice = get( device )->getDevice();
		D3D11_SAMPLER_DESC desc{};
		desc.AddressU = convert( createInfo.addressModeU );
		desc.AddressV = convert( createInfo.addressModeV );
		desc.AddressW = convert( createInfo.addressModeW );
		convert( createInfo.borderColor, desc.BorderColor );
		desc.ComparisonFunc = getComparisonFunc( createInfo.compareOp );
		desc.Filter = convert( createInfo.minFilter
			, createInfo.magFilter
			, createInfo.mipmapMode
			, createInfo.maxAnisotropy );
		desc.MaxAnisotropy = UINT( createInfo.maxAnisotropy );
		desc.MinLOD = createInfo.minLod;
		desc.MaxLOD = createInfo.maxLod;
		desc.MipLODBias = createInfo.mipLodBias;
		auto hr = d3ddevice->CreateSamplerState( &desc, &m_sampler );

		if ( checkError( device, hr, "CreateSamplerState" ) )
		{
			dxDebugName( m_sampler, SamplerState );
		}
	}

	Sampler::~Sampler()
	{
		safeRelease( m_sampler );
	}
}
