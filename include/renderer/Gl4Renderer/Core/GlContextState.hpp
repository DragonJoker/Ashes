/*
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

#include <renderer/RendererCommon/Helper/ColourBlendState.hpp>
#include <renderer/RendererCommon/Helper/DepthStencilState.hpp>
#include <renderer/RendererCommon/Helper/DynamicState.hpp>
#include <renderer/RendererCommon/Helper/InputAssemblyState.hpp>
#include <renderer/RendererCommon/Helper/MultisampleState.hpp>
#include <renderer/RendererCommon/Helper/RasterisationState.hpp>
#include <renderer/RendererCommon/Helper/TessellationState.hpp>
#include <renderer/RendererCommon/Helper/ViewportState.hpp>

namespace ashes::gl4
{
	struct ContextState
	{
		ContextState( ContextState const & rhs );
		ContextState( ContextState && rhs );
		ContextState( VkPipelineColorBlendStateCreateInfo cbState
			, Optional< VkPipelineDepthStencilStateCreateInfo > dsState
			, Optional< VkPipelineMultisampleStateCreateInfo > msState
			, Optional< VkPipelineTessellationStateCreateInfo > tsState
			, VkPipelineInputAssemblyStateCreateInfo iaState
			, VkPipelineViewportStateCreateInfo vpState
			, VkPipelineRasterizationStateCreateInfo rsState
			, VkPipelineDynamicStateCreateInfo dyState );
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
