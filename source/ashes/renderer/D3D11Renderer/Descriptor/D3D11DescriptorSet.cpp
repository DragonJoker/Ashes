#include "Descriptor/D3D11DescriptorSet.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Core/D3D11Device.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Descriptor/D3D11DescriptorPool.hpp"
#include "Image/D3D11Sampler.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>

namespace ashes::d3d11
{
	namespace
	{
		WriteDescriptorSetBinding doCreateBinding( ashes::DescriptorSetLayout const & layout
			, VkWriteDescriptorSet const & write )
		{
			return { write, layout.getBinding( write.dstBinding, write.dstArrayElement ) };
		}
	}

	DescriptorSet::DescriptorSet( Device const & device
		, DescriptorPool const & pool
		, DescriptorSetLayout const & layout
		, uint32_t bindingPoint )
		: ashes::DescriptorSet{ pool, bindingPoint }
		, m_pool{ pool }
		, m_layout{ layout }
	{
	}

	DescriptorSet::~DescriptorSet()
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
				m_samplers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eCombinedImageSampler:
				m_combinedTextureSamplers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eSampledImage:
				m_sampledTextures.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eStorageImage:
				m_storageTextures.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eUniformTexelBuffer:
				m_texelBuffers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eStorageTexelBuffer:
				m_texelBuffers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eUniformBuffer:
				m_uniformBuffers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eStorageBuffer:
				m_storageBuffers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eUniformBufferDynamic:
				m_dynamicUniformBuffers.push_back( doCreateBinding( m_layout, write ) );
				m_dynamicBuffers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eStorageBufferDynamic:
				m_dynamicStorageBuffers.push_back( doCreateBinding( m_layout, write ) );
				m_dynamicBuffers.push_back( doCreateBinding( m_layout, write ) );
				break;
			case ashes::DescriptorType::eInputAttachment:
				break;
			}
		}

		std::sort( m_dynamicBuffers.begin()
			, m_dynamicBuffers.end()
			, []( WriteDescriptorSetBinding const & lhs
				, WriteDescriptorSetBinding const & rhs )
			{
				return lhs.write.dstBinding < rhs.write.dstBinding;
			} );
	}
}
