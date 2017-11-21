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

#include <Renderer/Buffer.hpp>

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
		Submesh( renderer::RenderingResources const & resources
			, Mesh const & mesh
			, UInt16Array const & idx );
		/**
		*\brief
		*	Dessine le sous-maillage, en utilisant son tampon d'indices.
		*/
		void draw()const noexcept;
		/**
		*\brief
		*	Active les tampons du maillage parent sur les attributs donnés.
		*\param[in] position
		*	L'attribut de position.
		*\param[in] normal
		*	L'attribut de normale.
		*\param[in] texture
		*	L'attribut de coordonnées de texture.
		*/
		void bind( renderer::Vec3Attribute const * position
			, renderer::Vec3Attribute const * normal
			, renderer::Vec2Attribute const * texture )const noexcept;
		/**
		*\brief
		*	Désactive les tampons du maillage.
		*\param[in] position
		*	L'attribut de position.
		*\param[in] normal
		*	L'attribut de normale.
		*\param[in] texture
		*	L'attribut de coordonnées de texture.
		*/
		void unbind( renderer::Vec3Attribute const * position
			, renderer::Vec3Attribute const * normal
			, renderer::Vec2Attribute const * texture )const noexcept;

	private:
		//! Le maillage parent.
		Mesh const & m_mesh;
		//! Les indices des faces.
		renderer::BufferPtr< uint16_t > m_index;
	};
}

#endif
