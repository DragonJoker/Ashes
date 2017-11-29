#include "DescriptorSetLayout.hpp"

#include "DescriptorSetLayoutBinding.hpp"
#include "Device.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	namespace
	{
		std::vector< vk::DescriptorLayoutBinding > doConvert( DescriptorSetLayoutBindingArray const & bindings )
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
		, DescriptorSetLayoutBindingArray const & bindings )
		: m_layout{ device.getDevice().createDescriptorLayout( doConvert( bindings ) ) }
		, m_bindings{ bindings }
	{
	}

	DescriptorSetLayoutBinding const & DescriptorSetLayout::getBinding( uint32_t index )const
	{
		auto it = std::find_if( m_bindings.begin()
			, m_bindings.end()
			, [index]( DescriptorSetLayoutBinding const & lookup )
		{
			return lookup.getBinding().getIndex() == index;
		} );

		if ( it == m_bindings.end() )
		{
			throw std::range_error{ "Descriptor set layout binding was not found." };
		}

		return *it;
	}
}
