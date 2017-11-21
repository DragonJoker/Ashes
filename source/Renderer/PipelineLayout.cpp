#include "PipelineLayout.hpp"

#include "DescriptorSetLayout.hpp"
#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	PipelineLayout::PipelineLayout( RenderingResources const & resources
		, DescriptorSetLayout const * layout )
		: m_layout{ resources.getDevice().createPipelineLayout( layout 
			? &layout->getLayout()
			: nullptr ) }
	{
	}
}
