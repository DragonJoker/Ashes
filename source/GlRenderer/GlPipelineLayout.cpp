#include "GlPipelineLayout.hpp"

#include "GlDescriptorSetLayout.hpp"
#include "GlDevice.hpp"

namespace gl_renderer
{
	PipelineLayout::PipelineLayout( renderer::Device const & device
		, renderer::DescriptorSetLayout const * layout )
		: renderer::PipelineLayout{ device, layout }
	{
	}
}
