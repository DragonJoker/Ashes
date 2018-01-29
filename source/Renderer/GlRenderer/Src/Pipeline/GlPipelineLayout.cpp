#include "Pipeline/GlPipelineLayout.hpp"

#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	PipelineLayout::PipelineLayout( renderer::Device const & device
		, renderer::DescriptorSetLayoutCRefArray const & setLayouts
		, renderer::PushConstantRangeCRefArray const & pushConstantRanges )
		: renderer::PipelineLayout{ device, setLayouts, pushConstantRanges }
	{
	}
}
