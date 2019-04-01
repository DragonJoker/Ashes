/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Pipeline/Pipeline.hpp"

#include "AshesRenderer/Core/Device.hpp"
#include "AshesRenderer/Core/Exception.hpp"

namespace ashes
{
	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, GraphicsPipelineCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_layout{ layout }
	{
		if ( bool( m_createInfo.tessellationState )
			&& ( m_createInfo.tessellationState.value().flags || m_createInfo.tessellationState.value().patchControlPoints )
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
