/**
*\file
*	Object.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Object_HPP___
#define ___RenderLib_Object_HPP___
#pragma once

#include "Mesh.h"
#include "Movable.h"

namespace render
{
	/**
	*\brief
	*	Classe d'objet complexe défini par un maillage.
	*/
	class Object
		: public Movable
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de l'objet.
		*\param[in] mesh
		*	Le maillage de l'objet.
		*\param[in] materials
		*	Les matériaux utilisés par l'objet.
		*/
		Object( std::string const & name
			, MeshPtr mesh
			, MaterialArray const & materials )noexcept;
		/**
		*\return
		*	La liste de matériaux.
		*/
		inline MaterialArray const & materials()const noexcept
		{
			return m_materials;
		}
		/**
		*\return
		*	Le maillage.
		*/
		inline MeshPtr mesh()const noexcept
		{
			return m_mesh;
		}
		/**
		*\return
		*	Les dimensions de l'objet.
		*/
		inline gl::Vec3 const & boundaries()const noexcept
		{
			return m_mesh->boundaries();
		}

	private:
		//! Le maillage.
		MeshPtr m_mesh;
		//! La liste de matériaux.
		MaterialArray m_materials;
	};
}

#endif
