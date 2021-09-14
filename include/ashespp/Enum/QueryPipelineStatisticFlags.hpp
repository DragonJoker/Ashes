/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_QueryPipelineStatisticFlags_HPP___
#define ___AshesPP_QueryPipelineStatisticFlags_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkQueryPipelineStatisticFlagBits value )
	{
		switch ( value )
		{
		case VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT:
			return "input_assembly_vertices";
		case VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT:
			return "input_assembly_primitives";
		case VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT:
			return "vertex_shader_invocations";
		case VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_INVOCATIONS_BIT:
			return "geometry_shader_invocations";
		case VK_QUERY_PIPELINE_STATISTIC_GEOMETRY_SHADER_PRIMITIVES_BIT:
			return "geometry_shader_primitives";
		case VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT:
			return "clipping_invocations";
		case VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT:
			return "clipping_primitives";
		case VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT:
			return "fragment_shader_invocations";
		case VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_CONTROL_SHADER_PATCHES_BIT:
			return "tessellation_control_shader_patches";
		case VK_QUERY_PIPELINE_STATISTIC_TESSELLATION_EVALUATION_SHADER_INVOCATIONS_BIT:
			return "tessellation_evaluation_shader_invocations";
		case VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT:
			return "compute_shader_invocations";
		default:
			assert( false && "Unsupported VkQueryPipelineStatisticFlagBits." );
			return "Unsupported VkQueryPipelineStatisticFlagBits";
		}
	}
}

#endif
