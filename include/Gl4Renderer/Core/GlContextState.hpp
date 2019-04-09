/*
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <AshesRenderer/Helper/ColourBlendState.hpp>
#include <AshesRenderer/Helper/DepthStencilState.hpp>
#include <AshesRenderer/Helper/DynamicState.hpp>
#include <AshesRenderer/Helper/InputAssemblyState.hpp>
#include <AshesRenderer/Helper/MultisampleState.hpp>
#include <AshesRenderer/Helper/RasterisationState.hpp>
#include <AshesRenderer/Helper/TessellationState.hpp>
#include <AshesRenderer/Helper/ViewportState.hpp>

namespace ashes::gl4
{
	struct ContextState
	{
		ContextState( ContextState const & rhs );
		ContextState( ContextState && rhs );
		ContextState( VkPipelineColorBlendStateCreateInfo cbState
			, VkPipelineDepthStencilStateCreateInfo const * dsState = nullptr
			, VkPipelineMultisampleStateCreateInfo const * msState = nullptr
			, VkPipelineTessellationStateCreateInfo const * tsState = nullptr
			, VkPipelineInputAssemblyStateCreateInfo const * iaState = nullptr
			, VkPipelineViewportStateCreateInfo const * vpState = nullptr
			, VkPipelineRasterizationStateCreateInfo const * rsState = nullptr
			, VkPipelineDynamicStateCreateInfo const * dyState = nullptr );
		ContextState();

		inline ContextState & operator=( ContextState const & rhs )
		{
			ContextState tmp = rhs;
			std::swap( tmp, *this );
			doInit();
			return *this;
		}

		inline ContextState & operator=( ContextState && rhs )
		{
			ContextState tmp = std::move( rhs );
			std::swap( tmp, *this );
			doInit();
			return *this;
		}

		VkPipelineColorBlendAttachmentStateArray cbStateAttachments;
		VkPipelineColorBlendStateCreateInfo cbState;
		VkPipelineDepthStencilStateCreateInfo dsState;
		Optional< VkSampleMask > sampleMask;
		VkPipelineMultisampleStateCreateInfo msState;
		VkPipelineTessellationStateCreateInfo tsState;
		VkPipelineInputAssemblyStateCreateInfo iaState;
		VkViewportArray viewports;
		VkScissorArray scissors;
		VkPipelineViewportStateCreateInfo vpState;
		VkPipelineRasterizationStateCreateInfo rsState;
		VkDynamicStateArray dynamicStates;
		VkPipelineDynamicStateCreateInfo dyState;

	private:
		void doInit();
	};
}
