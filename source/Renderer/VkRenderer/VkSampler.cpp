#include "VkSampler.hpp"

#include "VkDevice.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace vk_renderer
{
	Sampler::Sampler( Device const & device
		, renderer::WrapMode wrapS
		, renderer::WrapMode wrapT
		, renderer::WrapMode wrapR
		, renderer::Filter minFilter
		, renderer::Filter magFilter
		, renderer::MipmapMode mipFilter )
		: renderer::Sampler{ device
			, wrapS
			, wrapT
			, wrapR
			, minFilter
			, magFilter
			, mipFilter }
		, m_device{ device }
	{
		VkSamplerCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,                // sType
			nullptr,                                              // pNext
			0,                                                    // flags
			convert( minFilter ),                                 // magFilter
			convert( magFilter ),                                 // minFilter
			convert( mipFilter ),                                 // mipmapMode
			convert( wrapS ),                                     // addressModeU
			convert( wrapT ),                                     // addressModeV
			convert( wrapR ),                                     // addressModeW
			0.0f,                                                 // mipLodBias
			VK_FALSE,                                             // anisotropyEnable
			1.0f,                                                 // maxAnisotropy
			VK_FALSE,                                             // compareEnable
			VK_COMPARE_OP_ALWAYS,                                 // compareOp
			0.0f,                                                 // minLod
			0.0f,                                                 // maxLod
			VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,              // borderColor
			VK_FALSE                                              // unnormalizedCoordinates
		};
		DEBUG_DUMP( createInfo );
		auto res = CreateSampler( m_device, &createInfo, nullptr, &m_sampler );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Sampler creation failed: " + getLastError() };
		}
	}

	Sampler::~Sampler()
	{
		DestroySampler( m_device, m_sampler, nullptr );
	}
}
