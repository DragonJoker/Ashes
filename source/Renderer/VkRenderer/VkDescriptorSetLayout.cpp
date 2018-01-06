#include "VkDescriptorSetLayout.hpp"

#include "VkDescriptorSetLayoutBinding.hpp"
#include "VkDescriptorSetPool.hpp"
#include "VkDevice.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace vk_renderer
{
	namespace
	{
		std::vector< vk::DescriptorLayoutBinding > doConvert( renderer::DescriptorSetLayoutBindingArray const & bindings )
		{
			std::vector< vk::DescriptorLayoutBinding > result;
			result.reserve( bindings.size() );

			for ( auto & binding : bindings )
			{
				result.emplace_back( convert( binding ) );
			}

			return result;
		}
	}

	DescriptorSetLayout::DescriptorSetLayout( renderer::Device const & device
		, renderer::DescriptorSetLayoutBindingArray && bindings )
		: renderer::DescriptorSetLayout{ device, std::move( bindings ) }
		, m_layout{ static_cast< Device const & >( device ).getDevice().createDescriptorLayout( doConvert( m_bindings ) ) }
	{
	}

	renderer::DescriptorSetPoolPtr DescriptorSetLayout::createPool( uint32_t maxSets )const
	{
		return std::make_unique< DescriptorSetPool >( *this, maxSets );
	}
}
