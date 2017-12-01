/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types de statistiques de pipeline.
	*/
	enum class QueryPipelineStatisticFlag
		: uint32_t
	{
		eInputAssemblyVertices = 0x00000001,
		eInputAssemblyPrimitives = 0x00000002,
		eVertexShaderInvocations = 0x00000004,
		eGeometryShaderInvocations = 0x00000008,
		eGeometryShaderPrimitives = 0x00000010,
		eClippingInvocations = 0x00000020,
		eClippingPrimitives = 0x00000040,
		eFragmentShaderInvocations = 0x00000080,
		eTessellationControlShaderPatches = 0x00000100,
		eTessellationControlShaderInvocations = 0x00000200,
		eComputeShaderInvocations = 0x00000400,
	};
	VkLib_ImplementFlag( QueryPipelineStatisticFlag )
	/**
	*\brief
	*	Convertit un renderer::QueryPipelineStatisticFlags en VkQueryPipelineStatisticFlags.
	*\param[in] flags
	*	Le renderer::QueryPipelineStatisticFlags.
	*\return
	*	Le VkQueryPipelineStatisticFlags.
	*/
	VkQueryPipelineStatisticFlags convert( QueryPipelineStatisticFlags const & flags );
}
