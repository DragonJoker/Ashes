#include "VkPipelineLayout.hpp"

#include "VkDescriptorSetLayout.hpp"
#include "VkDevice.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace vk_renderer
{
	PipelineLayout::PipelineLayout( renderer::Device const & device
		, renderer::DescriptorSetLayout const * layout )
		: renderer::PipelineLayout{ device, layout }
		, m_layout{ static_cast< Device const & >( device ).getDevice().createPipelineLayout( layout 
			? &static_cast< DescriptorSetLayout const & >( *layout ).getLayout()
			: nullptr ) }
	{
	}
}
