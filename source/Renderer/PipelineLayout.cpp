#include "PipelineLayout.hpp"

#include "DescriptorSetLayout.hpp"
#include "Device.hpp"
#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	PipelineLayout::PipelineLayout( Device const & device
		, DescriptorSetLayout const * layout )
		: m_layout{ device.getDevice().createPipelineLayout( layout 
			? &layout->getLayout()
			: nullptr ) }
	{
	}
}
