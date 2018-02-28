/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_GraphicsPipelineCreateInfo_HPP___
#define ___Renderer_GraphicsPipelineCreateInfo_HPP___
#pragma once

#include "Pipeline/ColourBlendState.hpp"
#include "Pipeline/DepthStencilState.hpp"
#include "Pipeline/InputAssemblyState.hpp"
#include "Pipeline/MultisampleState.hpp"
#include "Pipeline/RasterisationState.hpp"
#include "Pipeline/Scissor.hpp"
#include "Pipeline/SpecialisationInfo.hpp"
#include "Pipeline/TessellationState.hpp"
#include "Pipeline/VertexInputState.hpp"
#include "Pipeline/Viewport.hpp"

#include <map>

namespace renderer
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
		std::reference_wrapper< ShaderProgram const > program;
		std::reference_wrapper< RenderPass const > renderPass;
		VertexInputState vertexInputState;
		InputAssemblyState inputAssemblyState = InputAssemblyState{};
		RasterisationState rasterisationState = RasterisationState{};
		MultisampleState multisampleState = MultisampleState{};
		ColourBlendState colourBlendState = ColourBlendState::createDefault();
		std::optional< DepthStencilState > depthStencilState;
		std::optional< TessellationState > tessellationState;
		std::optional< Viewport > viewport;
		std::optional< Scissor > scissor;
		std::map< ShaderStageFlag, SpecialisationInfoBasePtr > specialisationInfo;
	};
}

#endif
