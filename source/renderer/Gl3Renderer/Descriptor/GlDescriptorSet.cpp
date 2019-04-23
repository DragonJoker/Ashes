#include "Descriptor/GlDescriptorSet.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlImage.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Descriptor/DescriptorSetLayoutBinding.hpp>

#include <algorithm>

namespace gl_renderer
{
	DescriptorSet::DescriptorSet( ashes::DescriptorPool const & pool
		, ashes::DescriptorSetLayout const & layout
		, uint32_t bindingPoint )
		: ashes::DescriptorSet{ pool, bindingPoint }
	{
	}

	void DescriptorSet::update()const
	{
		m_combinedTextureSamplers.clear();
		m_samplers.clear();
		m_sampledTextures.clear();
		m_storageTextures.clear();
		m_uniformBuffers.clear();
		m_storageBuffers.clear();
		m_texelBuffers.clear();
		m_dynamicUniformBuffers.clear();
		m_dynamicStorageBuffers.clear();
		m_dynamicBuffers.clear();

		for ( auto & write : m_writes )
		{
			switch ( write.descriptorType )
			{
			case ashes::DescriptorType::eSampler:
				m_samplers.push_back( write );
				break;
			case ashes::DescriptorType::eCombinedImageSampler:
				m_combinedTextureSamplers.push_back( write );
				break;
			case ashes::DescriptorType::eSampledImage:
				m_sampledTextures.push_back( write );
				break;
			case ashes::DescriptorType::eStorageImage:
				m_storageTextures.push_back( write );
				break;
			case ashes::DescriptorType::eUniformTexelBuffer:
				m_texelBuffers.push_back( write );
				break;
			case ashes::DescriptorType::eStorageTexelBuffer:
				m_texelBuffers.push_back( write );
				break;
			case ashes::DescriptorType::eUniformBuffer:
				m_uniformBuffers.push_back( write );
				break;
			case ashes::DescriptorType::eStorageBuffer:
				m_storageBuffers.push_back( write );
				break;
			case ashes::DescriptorType::eUniformBufferDynamic:
				m_dynamicUniformBuffers.push_back( write );
				m_dynamicBuffers.push_back( write );
				break;
			case ashes::DescriptorType::eStorageBufferDynamic:
				m_dynamicStorageBuffers.push_back( write );
				m_dynamicBuffers.push_back( write );
				break;
			case ashes::DescriptorType::eInputAttachment:
				break;
			}
		}

		std::sort( m_dynamicBuffers.begin()
			, m_dynamicBuffers.end()
			, []( ashes::WriteDescriptorSet const & lhs
				, ashes::WriteDescriptorSet const & rhs )
		{
			return lhs.dstBinding < rhs.dstBinding;
		} );
	}
}
