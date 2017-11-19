/**
*\file
*	Mesh.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Mesh_H___
#define ___RenderLib_Mesh_H___
#pragma once

#include <GlLib/GlBuffer.h>

#include "Material.h"

namespace render
{
	/**
	*\brief
	*	Contient les données d'un maillage.
	*\remarks
	*	Un maillage est constitué obligatoirement de positions.
	*	Optionnellement, on peut lui ajouter des normales et des coordonnées
	*	de texture.
	*/
	class Mesh
	{
		friend class Submesh;

	public:
		/**
		*\brief
		*	Définit les données du maillage.
		*\param[in] pos
		*	Les positions du maillage.
		*\param[in] nml
		*	Les normales du maillage.
		*\param[in] tex
		*	Les coordonnées de texture du maillage.
		*/
		void data( Vec3Array const & pos
			, Vec3Array const & nml = Vec3Array{}
			, Vec2Array const & tex = Vec2Array{} );
		/**
		*\brief
		*	Ajoute un sous-maillage.
		*\param[in] idx
		*	Les indices des faces du sous-maillage à ajouter.
		*/
		void addSubmesh( UInt16Array const & idx );
		/**
		*\return
		*	Le nombre de sous-maillages.
		*/
		inline size_t size()const noexcept
		{
			return m_submeshes.size();
		}
		/**
		*\return
		*	L'itérateur de début des sous-maillages.
		*/
		inline SubmeshArray::const_iterator begin()const noexcept
		{
			return m_submeshes.begin();
		}
		/**
		*\return
		*	L'itérateur de début des sous-maillages.
		*/
		inline SubmeshArray::iterator begin()noexcept
		{
			return m_submeshes.begin();
		}
		/**
		*\return
		*	L'itérateur de fin des sous-maillages.
		*/
		inline SubmeshArray::const_iterator end()const noexcept
		{
			return m_submeshes.end();
		}
		/**
		*\return
		*	L'itérateur de fin des sous-maillages.
		*/
		inline SubmeshArray::iterator end()noexcept
		{
			return m_submeshes.end();
		}
		/**
		*\return
		*	Les dimensions du maillage.
		*/
		inline gl::Vec3 const & boundaries()const noexcept
		{
			return m_boundaries;
		}

	private:
		//! Les positions des sommets.
		gl::Vec3BufferPtr m_positions;
		//! Les normales des sommets.
		gl::Vec3BufferPtr m_normal;
		//! Les coordonnées de texture des sommets.
		gl::Vec2BufferPtr m_texcoord;
		//! Les sous-maillages.
		SubmeshArray m_submeshes;
		//! Les dimensions du maillage.
		gl::Vec3 m_boundaries;
	};
}

#endif
