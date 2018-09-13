/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Pipeline/Pipeline.hpp"

#include "Core/Device.hpp"
#include "Core/Exception.hpp"

namespace ashes
{
	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, GraphicsPipelineCreateInfo && createInfo )
		: m_device{ device }
		, m_createInfo
		{
			std::move( createInfo.stages ),
			createInfo.renderPass,
			createInfo.vertexInputState,
			createInfo.inputAssemblyState,
			createInfo.rasterisationState,
			createInfo.multisampleState,
			createInfo.colourBlendState,
			createInfo.dynamicStates,
			createInfo.depthStencilState,
			createInfo.tessellationState,
			createInfo.viewport,
			createInfo.scissor
		}
		, m_layout{ layout }
	{
		if ( bool( createInfo.tessellationState )
			&& ( createInfo.tessellationState.value().flags || createInfo.tessellationState.value().patchControlPoints )
			&& !device.getPhysicalDevice().getFeatures().tessellationShader )
		{
			throw Exception{ Result::eErrorFeatureNotPresent, "Tessellation shaders" };
		}

		registerObject( m_device, "Pipeline", this );
	}

	Pipeline::~Pipeline()
	{
		unregisterObject( m_device, this );
	}
}
