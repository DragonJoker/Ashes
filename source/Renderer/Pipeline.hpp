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
		*\param[in] vertexShaderFile
		*	Le chemin d'accès au fichier du vertex shader.
		*\param[in] fragmentShaderFile
		*	Le chemin d'accès au fichier du fragment shader.
		*\param[in] vertexBuffers
		*	Les tampons de sommets utilisés.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] topology
		*	La topologie d'affichage des sommets affichés via ce pipeline.
		*/
		Pipeline( RenderingResources const & resources
			, vk::PipelineLayout const & layout
			, std::string const & vertexShaderFile
			, std::string const & fragmentShaderFile
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
		*\param[in] vertexShaderFile
		*	Le chemin d'accès au fichier du vertex shader.
		*\param[in] fragmentShaderFile
		*	Le chemin d'accès au fichier du fragment shader.
		*\param[in] vertexBuffers
		*	Les tampons de sommets utilisés.
		*\param[in] viewport
		*	Le viewport.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] topology
		*	La topologie d'affichage des sommets affichés via ce pipeline.
		*/
		Pipeline( RenderingResources const & resources
			, vk::PipelineLayout const & layout
			, std::string const & vertexShaderFile
			, std::string const & fragmentShaderFile
			, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
			, vk::Viewport const & viewport
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
