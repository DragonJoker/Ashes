/**
*\file
*	Billboard.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Billboard_HPP___
#define ___RenderLib_Billboard_HPP___
#pragma once

#include "BillboardBuffer.h"
#include "Movable.h"

namespace render
{
	/**
	*\brief
	*	Fait la liaison entre les tampons d'un billboard et un matériau.
	*/
	class Billboard
		: public Movable
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de l'objet
		*\param[in] buffer
		*	Les tampons du billboard.
		*/
		Billboard( std::string const & name, BillboardBuffer & buffer );
		/**
		*\brief
		*	Cache les billboards qui ne sont pas visibles par la caméra donnée.
		*\param[in] camera
		*	La caméra.
		*\param[in] scale
		*	L'échelle du zoom.
		*/
		void cull( Camera const & camera
			, float scale );
		/**
		*\return
		*	Le tampon contenant les positions.
		*/
		inline auto const & buffer()const
		{
			return m_buffer;
		}
		/**
		*\return
		*	Le tampon contenant les positions.
		*/
		inline auto & buffer()
		{
			return m_buffer;
		}
		/**
		*\brief
		*	Définit le matériau.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void material( MaterialPtr value )
		{
			m_material = value;
		}
		/**
		*\return
		*	Le matériau.
		*/
		inline Material const & material()const
		{
			assert( m_material );
			return *m_material;
		}
		/**
		*\brief
		*	Définit les dimensions des billboards.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void dimensions( gl::IVec2 const & value )
		{
			m_dimensions = value;
		}
		/**
		*\return
		*	Les dimensions des billboards.
		*/
		inline gl::IVec2 const & dimensions()const
		{
			return m_dimensions;
		}

	protected:
		//! Le matériau.
		MaterialPtr m_material;
		//! Les dimensions des billboards.
		gl::IVec2 m_dimensions;
		//! Les données du billboard.
		BillboardBuffer & m_buffer;
	};
}

#endif
