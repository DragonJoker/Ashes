/**
*\file
*	Submesh.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Submesh_HPP___
#define ___RenderLib_Submesh_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <Renderer/Buffer/Buffer.hpp>

namespace render
{
	/**
	*\brief
	*	Contient les indices des faces d'un sous-maillage.
	*\remarks
	*	Utilisé conjointement avec render::Mesh, il se sert
	*	des tampons de sommets de celui-ci pour les attributs
	*	que l'on donne aux fonctions bind() et unbind().
	*/
	class Submesh
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] mesh
		*	Le maillage parent.
		*\param[in] idx
		*	Les données d'indices des faces.
		*/
		Submesh( renderer::Device const & device
			, renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Mesh const & mesh
			, UInt16Array const & idx );
		/**
		*\return
		*	Le nombre d'indices.
		*/
		uint32_t getIndexCount()const noexcept;
		/**
		*\return
		*	Le tampon d'indices.
		*/
		renderer::Buffer< uint16_t > const & getIbo()const noexcept;

	private:
		//! Le maillage parent.
		Mesh const & m_mesh;
		//! Les indices des faces.
		renderer::BufferPtr< uint16_t > m_index;
	};
}

#endif
