/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_GraphicsPipelineCreateInfo_HPP___
#define ___Ashes_GraphicsPipelineCreateInfo_HPP___
#pragma once

#include "Ashes/Pipeline/ColourBlendState.hpp"
#include "Ashes/Pipeline/DepthStencilState.hpp"
#include "Ashes/Pipeline/DynamicState.hpp"
#include "Ashes/Pipeline/InputAssemblyState.hpp"
#include "Ashes/Pipeline/MultisampleState.hpp"
#include "Ashes/Pipeline/RasterisationState.hpp"
#include "Ashes/Pipeline/ShaderStageState.hpp"
#include "Ashes/Pipeline/TessellationState.hpp"
#include "Ashes/Pipeline/VertexInputState.hpp"
#include "Ashes/Pipeline/ViewportState.hpp"

#include <map>

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the data used to create a graphic pipeline.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer un pipeline graphique.
	*/
	struct GraphicsPipelineCreateInfo
	{
		GraphicsPipelineCreateInfo( ShaderStageStateArray stages = ShaderStageStateArray{}
			, VertexInputState vertexInputState = VertexInputState{}
			, InputAssemblyState inputAssemblyState = InputAssemblyState{}
			, Optional< TessellationState > tessellationState = nullopt
			, ViewportState viewportState = ViewportState{}
			, RasterisationState rasterisationState = RasterisationState{}
			, MultisampleState multisampleState = MultisampleState{}
			, Optional< DepthStencilState > depthStencilState = nullopt
			, ColourBlendState colourBlendState = ColourBlendState::createDefault()
			, Optional< DynamicState > dynamicState = nullopt
			, RenderPass const * renderPass = nullptr )
			: stages{ std::move( stages ) }
			, vertexInputState{ std::move( vertexInputState ) }
			, inputAssemblyState{ std::move( inputAssemblyState ) }
			, tessellationState{ std::move( tessellationState ) }
			, viewportState{ std::move( viewportState ) }
			, rasterisationState{ std::move( rasterisationState ) }
			, multisampleState{ std::move( multisampleState ) }
			, depthStencilState{ std::move( depthStencilState ) }
			, colourBlendState{ std::move( colourBlendState ) }
			, dynamicState{ std::move( dynamicState ) }
			, renderPass{ std::move( renderPass ) }
		{
		}
		
		GraphicsPipelineCreateInfo( ShaderStageStateArray stages
			, RenderPass const & renderPass
			, VertexInputState vertexInputState
			, InputAssemblyState inputAssemblyState = InputAssemblyState{}
			, RasterisationState rasterisationState = RasterisationState{}
			, MultisampleState multisampleState = MultisampleState{}
			, ColourBlendState colourBlendState = ColourBlendState::createDefault()
			, DynamicStateEnableArray dynamicStates = DynamicStateEnableArray{}
			, Optional< DepthStencilState > depthStencilState = nullopt
			, Optional< TessellationState > tessellationState = nullopt
			, Optional< Viewport > viewport = nullopt
			, Optional< Scissor > scissor = nullopt )
			: stages{ std::move( stages ) }
			, vertexInputState{ std::move( vertexInputState ) }
			, inputAssemblyState{ std::move( inputAssemblyState ) }
			, tessellationState{ std::move( tessellationState ) }
			, rasterisationState{ std::move( rasterisationState ) }
			, multisampleState{ std::move( multisampleState ) }
			, depthStencilState{ std::move( depthStencilState ) }
			, colourBlendState{ std::move( colourBlendState ) }
			, renderPass{ &renderPass }
		{
			if ( !dynamicStates.empty() )
			{
				dynamicState = DynamicState{ 0u, std::move( dynamicStates ) };
			}

			if ( bool( viewport ) || bool( scissor ) )
			{
				viewportState = ViewportState{ 0u };

				if ( bool( viewport ) )
				{
					viewportState.viewports.emplace_back( std::move( viewport.value() ) );
				}

				if ( bool( scissor ) )
				{
					viewportState.scissors.emplace_back( std::move( scissor.value() ) );
				}
			}
		}

		ShaderStageStateArray stages{};
		VertexInputState vertexInputState{};
		InputAssemblyState inputAssemblyState{};
		Optional< TessellationState > tessellationState{ nullopt };
		ViewportState viewportState{};
		RasterisationState rasterisationState{};
		MultisampleState multisampleState{};
		Optional< DepthStencilState > depthStencilState{ nullopt };
		ColourBlendState colourBlendState = ColourBlendState::createDefault();
		Optional< DynamicState > dynamicState{ nullopt };
		RenderPass const * renderPass;
	};
}

#endif
