#include "DescriptorSetLayout.hpp"

#include "DescriptorSetLayoutBinding.hpp"
#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	namespace
	{
		std::vector< vk::DescriptorLayoutBinding > doConvert( std::vector< DescriptorSetLayoutBinding > const & bindings )
		{
			std::vector< vk::DescriptorLayoutBinding > result;
			result.reserve( bindings.size() );

			for ( auto & binding : bindings )
			{
				result.emplace_back( binding.getBinding() );
			}

			return result;
		}
	}
	DescriptorSetLayout::DescriptorSetLayout( RenderingResources const & resources
		, std::vector< DescriptorSetLayoutBinding > const & bindings )
		: m_layout{ resources.getDevice().createDescriptorLayout( doConvert( bindings ) ) }
	{
	}
}
