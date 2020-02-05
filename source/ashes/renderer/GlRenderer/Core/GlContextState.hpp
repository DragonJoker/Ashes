/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <renderer/RendererCommon/Helper/ColourBlendState.hpp>
#include <renderer/RendererCommon/Helper/DepthStencilState.hpp>
#include <renderer/RendererCommon/Helper/DynamicState.hpp>
#include <renderer/RendererCommon/Helper/InputAssemblyState.hpp>
#include <renderer/RendererCommon/Helper/MultisampleState.hpp>
#include <renderer/RendererCommon/Helper/RasterisationState.hpp>
#include <renderer/RendererCommon/Helper/TessellationState.hpp>
#include <renderer/RendererCommon/Helper/ViewportState.hpp>

namespace ashes::gl
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

		ContextState & swap( ContextState && rhs );

		inline ContextState & operator=( ContextState const & rhs )
		{
			ContextState tmp = rhs;
			swap( std::move( tmp ) );
			doInit();
			return *this;
		}

		inline ContextState & operator=( ContextState && rhs )
		{
			swap( std::move( rhs ) );
			doInit();
			return *this;
		}

		void setViewports( VkViewportArray value )
		{
			viewports = value;
			viewportsView = makeArrayView( const_cast< VkViewportArray const & >( viewports ).data(), viewports.size() );
		}

		void setScissors( VkScissorArray value )
		{
			scissors = value;
			scissorsView = makeArrayView( const_cast< VkScissorArray const & >( scissors ).data(), scissors.size() );
		}

		VkPipelineColorBlendAttachmentStateArray cbStateAttachments;
		VkPipelineColorBlendStateCreateInfo cbState;
		VkPipelineDepthStencilStateCreateInfo dsState;
		Optional< VkSampleMask > sampleMask;
		VkPipelineMultisampleStateCreateInfo msState;
		VkPipelineTessellationStateCreateInfo tsState;
		VkPipelineInputAssemblyStateCreateInfo iaState;
		ArrayView< VkViewport const > viewportsView;
		ArrayView< VkRect2D const > scissorsView;
		VkPipelineViewportStateCreateInfo vpState;
		VkPipelineRasterizationStateCreateInfo rsState;
		VkDynamicStateArray dynamicStates;
		VkPipelineDynamicStateCreateInfo dyState;
		bool sRGB{ false };

	private:
		void doInit();

	private:
		VkViewportArray viewports;
		VkScissorArray scissors;
	};
}
