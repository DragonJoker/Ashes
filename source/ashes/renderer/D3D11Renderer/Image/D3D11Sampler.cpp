#include "Image/D3D11Sampler.hpp"

#include "Core/D3D11Device.hpp"

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

	void convert( ashes::BorderColour colour
		, float output[4] )
	{
		switch ( colour )
		{
		case ashes::BorderColour::eFloatTransparentBlack:
		case ashes::BorderColour::eIntTransparentBlack:
			output[0] = 0.0;
			output[1] = 0.0;
			output[2] = 0.0;
			output[3] = 0.0;
			break;
		case ashes::BorderColour::eFloatOpaqueBlack:
		case ashes::BorderColour::eIntOpaqueBlack:
			output[0] = 0.0;
			output[1] = 0.0;
			output[2] = 0.0;
			output[3] = 1.0;
			break;
		case ashes::BorderColour::eFloatOpaqueWhite:
		case ashes::BorderColour::eIntOpaqueWhite:
			output[0] = 1.0;
			output[1] = 1.0;
			output[2] = 1.0;
			output[3] = 1.0;
			break;
		}
	}

	D3D11_FILTER convert( VkFilter min
		, VkFilter mag
		, VkSamplerMipmapMode mip )
	{
		if ( min == VK_FILTER_NEAREST )
		{
			if ( mag == VK_FILTER_NEAREST )
			{
				switch ( mip )
				{
				case VkSamplerMipmapMode::eNone:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
				}
			}
			else
			{
				switch ( mip )
				{
				case VkSamplerMipmapMode::eNone:
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
				}
			}
		}
		else
		{
			if ( mag == VK_FILTER_NEAREST )
			{
				switch ( mip )
				{
				case VkSamplerMipmapMode::eNone:
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
				}
			}
			else
			{
				switch ( mip )
				{
				case VkSamplerMipmapMode::eNone:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_NEAREST:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

				case VK_SAMPLER_MIPMAP_MODE_LINEAR:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				}
			}
		}

		assert( false );
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	Sampler::Sampler( VkDevice device
		, VkSamplerCreateInfo const & createInfo )
		: ashes::Sampler{ device, createInfo }
	{
		auto d3ddevice = device.getDevice();
		D3D11_SAMPLER_DESC desc{};
		desc.AddressU = convert( createInfo.addressModeU );
		desc.AddressV = convert( createInfo.addressModeV );
		desc.AddressW = convert( createInfo.addressModeW );
		convert( createInfo.borderColor, desc.BorderColor );
		desc.ComparisonFunc = convert( createInfo.compareOp );
		desc.Filter = convert( createInfo.minFilter
			, createInfo.magFilter
			, createInfo.mipmapMode );
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
