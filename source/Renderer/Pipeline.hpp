/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Pipeline_HPP___
#define ___Renderer_Pipeline_HPP___
#pragma once

#include "ColourBlendState.hpp"
#include "RasterisationState.hpp"

#include <VkLib/Pipeline.hpp>

namespace renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	Le programme shader.
		*\param[in] vertexBuffers
		*	Les tampons de sommets utilisés.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] topology
		*	La topologie d'affichage des sommets affichés via ce pipeline.
		*/
		Pipeline( Device const & device
			, PipelineLayout const & layout
			, ShaderProgram const & program
			, VertexLayoutCRefArray const & vertexLayouts
			, RenderPass const & renderPass
			, PrimitiveTopology topology
			, RasterisationState const & rasterisationState = RasterisationState{}
			, ColourBlendState const & colourBlendState = ColourBlendState::createDefault() );
		/**
		*\brief
		*	Crée le pipeline.
		*/
		Pipeline & finish();
		/**
		*\brief
		*	Définit le MultisampleState.
		*\param[in] state
		*	La nouvelle valeur.
		*/
		Pipeline & multisampleState( MultisampleState const & state );
		/**
		*\brief
		*	Définit le DepthStencilState.
		*\param[in] state
		*	La nouvelle valeur.
		*/
		Pipeline & depthStencilState( DepthStencilState const & state );
		/**
		*\brief
		*	Définit le TessellationState.
		*\param[in] state
		*	La nouvelle valeur.
		*/
		Pipeline & tessellationState( TessellationState const & state );
		/**
		*\brief
		*	Définit le Viewport.
		*\param[in] viewport
		*	La nouvelle valeur.
		*/
		Pipeline & viewport( Viewport const & viewport );
		/**
		*\brief
		*	Définit le Scissor.
		*\param[in] scissor
		*	La nouvelle valeur.
		*/
		Pipeline & scissor( Scissor const & scissor );
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline vk::Pipeline const & getPipeline()const noexcept
		{
			return *m_pipeline;
		}

	private:
		//! Le pipeline de rendu.
		vk::PipelinePtr m_pipeline;
	};
}

#endif
