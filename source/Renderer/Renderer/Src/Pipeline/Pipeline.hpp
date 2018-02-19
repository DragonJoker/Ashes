/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Pipeline_HPP___
#define ___Renderer_Pipeline_HPP___
#pragma once

#include "Pipeline/ColourBlendState.hpp"
#include "Pipeline/RasterisationState.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	A rendering pipeline.
	*\~french
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
	{
	protected:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] program
		*	The shader program.
		*\param[in] vertexLayouts
		*	The vertex layout used by the pipeline.
		*\param[in] renderPass
		*	The render pass.
		*\param[in] inputAssemblyState
		*	The input assembly state.
		*\param[in] rasterisationState
		*	The rasterisation state state.
		*\param[in] colourBlendState
		*	The colour blend state.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	Le programme shader.
		*\param[in] vertexLayouts
		*	Le layout de sommets utilisé par le pipeline.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] inputAssemblyState
		*	L'état d'assemblage des entrées.
		*\param[in] rasterisationState
		*	L'état de rastérisation.
		*\param[in] colourBlendState
		*	L'état de mélange des couleurs.
		*/
		Pipeline( Device const & device
			, PipelineLayout const & layout
			, ShaderProgram const & program
			, VertexLayoutCRefArray const & vertexLayouts
			, RenderPass const & renderPass
			, InputAssemblyState const & inputAssemblyState
			, RasterisationState const & rasterisationState = RasterisationState{}
			, ColourBlendState const & colourBlendState = ColourBlendState::createDefault() );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Pipeline() = default;
		/**
		*\~english
		*\brief
		*	Creates the pipeline.
		*\~french
		*\brief
		*	Crée le pipeline.
		*/
		virtual Pipeline & finish() = 0;
		/**
		*\~english
		*\brief
		*	Specifies the MultisampleState.
		*\param[in] state
		*	he new value.
		*\~french
		*\brief
		*	Définit le MultisampleState.
		*\param[in] state
		*	La nouvelle valeur.
		*/
		virtual Pipeline & multisampleState( MultisampleState const & state ) = 0;
		/**
		*\~english
		*\brief
		*	Specifies the DepthStencilState.
		*\param[in] state
		*	he new value.
		*\~french
		*\brief
		*	Définit le DepthStencilState.
		*\param[in] state
		*	La nouvelle valeur.
		*/
		virtual Pipeline & depthStencilState( DepthStencilState const & state ) = 0;
		/**
		*\~english
		*\brief
		*	Specifies the TessellationState.
		*\param[in] state
		*	he new value.
		*\~french
		*\brief
		*	Définit le TessellationState.
		*\param[in] state
		*	La nouvelle valeur.
		*/
		virtual Pipeline & tessellationState( TessellationState const & state ) = 0;
		/**
		*\~english
		*\brief
		*	Specifies the Viewport.
		*\param[in] viewport
		*	he new value.
		*\~french
		*\brief
		*	Définit le Viewport.
		*\param[in] viewport
		*	La nouvelle valeur.
		*/
		virtual Pipeline & viewport( Viewport const & viewport ) = 0;
		/**
		*\~english
		*\brief
		*	Specifies the scissor.
		*\param[in] scissor
		*	he new value.
		*\~french
		*\brief
		*	Définit le Scissor.
		*\param[in] scissor
		*	La nouvelle valeur.
		*/
		virtual Pipeline & scissor( Scissor const & scissor ) = 0;
	};
}

#endif
