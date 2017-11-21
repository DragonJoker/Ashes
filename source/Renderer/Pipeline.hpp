/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Pipeline_HPP___
#define ___Renderer_Pipeline_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

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
		*\param[in] resources
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
		Pipeline( RenderingResources const & resources
			, PipelineLayout const & layout
			, ShaderProgram const & program
			, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
			, vk::RenderPass const & renderPass
			, PrimitiveTopology topology );
		/**
		*\brief
		*	Constructeur.
		*\param[in] resources
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	The shader program.
		*\param[in] vertexBuffers
		*	Les tampons de sommets utilisés.
		*\param[in] viewport
		*	Le viewport.
		*\param[in] scissor
		*	Le ciseau.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] topology
		*	La topologie d'affichage des sommets affichés via ce pipeline.
		*/
		Pipeline( RenderingResources const & resources
			, PipelineLayout const & layout
			, ShaderProgram const & program
			, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
			, Viewport const & viewport
			, Scissor const & scissor
			, vk::RenderPass const & renderPass
			, PrimitiveTopology topology );
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
