#include "Pipeline/D3D11PipelineLayout.hpp"

#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11ComputePipeline.hpp"
#include "Pipeline/D3D11Pipeline.hpp"

namespace ashes::d3d11
{
	PipelineLayout::PipelineLayout( Device const & device
		, ashes::DescriptorSetLayoutCRefArray const & setLayouts
		, ashes::PushConstantRangeArray const & pushConstantRanges )
		: ashes::PipelineLayout{ device, setLayouts, pushConstantRanges }
		, m_device{ device }
	{
	}

	PipelineLayout::~PipelineLayout()
	{
	}

	ashes::PipelinePtr PipelineLayout::createPipeline( ashes::GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< Pipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}

	ashes::ComputePipelinePtr PipelineLayout::createPipeline( ashes::ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}
}
