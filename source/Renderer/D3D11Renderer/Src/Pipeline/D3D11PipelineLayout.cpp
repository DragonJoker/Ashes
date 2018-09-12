#include "Pipeline/D3D11PipelineLayout.hpp"

#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11ComputePipeline.hpp"
#include "Pipeline/D3D11Pipeline.hpp"

namespace d3d11_renderer
{
	PipelineLayout::PipelineLayout( Device const & device
		, renderer::DescriptorSetLayoutCRefArray const & setLayouts
		, renderer::PushConstantRangeCRefArray const & pushConstantRanges )
		: renderer::PipelineLayout{ device, setLayouts, pushConstantRanges }
		, m_device{ device }
	{
	}

	PipelineLayout::~PipelineLayout()
	{
	}

	renderer::PipelinePtr PipelineLayout::createPipeline( renderer::GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< Pipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}

	renderer::ComputePipelinePtr PipelineLayout::createPipeline( renderer::ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}
}
