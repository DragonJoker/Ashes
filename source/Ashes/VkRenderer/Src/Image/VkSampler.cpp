#include "Image/VkSampler.hpp"

#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	Sampler::Sampler( Device const & device
		, ashes::SamplerCreateInfo const & createInfo )
		: ashes::Sampler{ device, createInfo }
		, m_device{ device }
	{
		VkSamplerCreateInfo vkcreateInfo
		{
			VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,                // sType
			nullptr,                                              // pNext
			0,                                                    // flags
			convert( createInfo.minFilter ),                      // magFilter
			convert( createInfo.magFilter ),                      // minFilter
			convert( createInfo.mipmapMode ),                     // mipmapMode
			convert( createInfo.addressModeU ),                   // addressModeU
			convert( createInfo.addressModeV ),                   // addressModeV
			convert( createInfo.addressModeW ),                   // addressModeW
			createInfo.mipLodBias,                                // mipLodBias
			createInfo.anisotropyEnable,                          // anisotropyEnable
			createInfo.maxAnisotropy,                             // maxAnisotropy
			createInfo.compareEnable,                             // compareEnable
			convert( createInfo.compareOp ),                      // compareOp
			createInfo.minLod,                                    // minLod
			createInfo.maxLod,                                    // maxLod
			convert( createInfo.borderColor ),                    // borderColor
			createInfo.unnormalizedCoordinates                    // unnormalizedCoordinates
		};
		DEBUG_DUMP( vkcreateInfo );
		auto res = m_device.vkCreateSampler( m_device, &vkcreateInfo, nullptr, &m_sampler );
		checkError( res, "Sampler creation" );
	}

	Sampler::~Sampler()
	{
		m_device.vkDestroySampler( m_device, m_sampler, nullptr );
	}
}
