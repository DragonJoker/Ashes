#include "DescriptorSetLayout.hpp"

#include "DescriptorSetLayoutBinding.hpp"
#include "Device.hpp"

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
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, std::vector< DescriptorSetLayoutBinding > const & bindings )
		: m_layout{ device.getDevice().createDescriptorLayout( doConvert( bindings ) ) }
	{
	}
}
