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
		~ContextState()noexcept = default;
		ContextState( ContextState const & rhs );
		ContextState( ContextState && rhs )noexcept;
		ContextState( Optional< VkPipelineColorBlendStateCreateInfo > const & cbState
			, Optional< VkPipelineDepthStencilStateCreateInfo > const & dsState
			, Optional< VkPipelineMultisampleStateCreateInfo > const & msState
			, Optional< VkPipelineTessellationStateCreateInfo > const & tsState
			, VkPipelineInputAssemblyStateCreateInfo const & iaState
			, VkPipelineViewportStateCreateInfo const & vpState
			, VkPipelineRasterizationStateCreateInfo const & rsState
			, VkPipelineDynamicStateCreateInfo const & dyState );
		ContextState( VkPipelineColorBlendStateCreateInfo const * cbState = nullptr
			, VkPipelineDepthStencilStateCreateInfo const * dsState = nullptr
			, VkPipelineMultisampleStateCreateInfo const * msState = nullptr
			, VkPipelineTessellationStateCreateInfo const * tsState = nullptr
			, VkPipelineInputAssemblyStateCreateInfo const * iaState = nullptr
			, VkPipelineViewportStateCreateInfo const * vpState = nullptr
			, VkPipelineRasterizationStateCreateInfo const * rsState = nullptr
			, VkPipelineDynamicStateCreateInfo const * dyState = nullptr );
		ContextState( VkPipelineColorBlendStateCreateInfo const & cbState
			, VkPipelineDepthStencilStateCreateInfo const * dsState = nullptr
			, VkPipelineMultisampleStateCreateInfo const * msState = nullptr
			, VkPipelineTessellationStateCreateInfo const * tsState = nullptr
			, VkPipelineInputAssemblyStateCreateInfo const * iaState = nullptr
			, VkPipelineViewportStateCreateInfo const * vpState = nullptr
			, VkPipelineRasterizationStateCreateInfo const * rsState = nullptr
			, VkPipelineDynamicStateCreateInfo const * dyState = nullptr );

		void swap( ContextState & lhs, ContextState & rhs )const noexcept;

		ContextState & operator=( ContextState const & rhs )
		{
			ContextState tmp = rhs;
			swap( *this, tmp );
			doInit();
			return *this;
		}

		ContextState & operator=( ContextState && rhs )noexcept
		{
			swap( *this, rhs );
			doInit();
			return *this;
		}

		void setViewports( VkViewportArray value )
		{
			viewports = std::move( value );
			viewportsView = makeArrayView( const_cast< VkViewportArray const & >( viewports ).data()
				, const_cast< VkViewportArray const & >( viewports ).data() + viewports.size() );
		}

		void setScissors( VkScissorArray value )
		{
			scissors = std::move( value );
			scissorsView = makeArrayView( const_cast< VkScissorArray const & >( scissors ).data()
				, const_cast< VkScissorArray const & >( scissors ).data() + scissors.size() );
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
		int32_t packAlign{ 1 };
		int32_t unpackAlign{ 1 };

	private:
		void doInit()noexcept;

	private:
		VkViewportArray viewports;
		VkScissorArray scissors;
	};
}
