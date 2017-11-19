/**
*\file
*	PolyLine.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_PolyLine_H___
#define ___RenderLib_PolyLine_H___
#pragma once

#include "Material.h"
#include "Movable.h"

#include <array>

namespace render
{
	/**
	*\brief
		Contient les données d'une liste de points, liés par des lignes.
	*/
	class PolyLine
		: public Movable
	{
	public:
		/**
		*\brief
		*	Représente les deux points constituant une ligne.
		*/
		struct Points
		{
			//! Le premier point.
			gl::Vec3 m_a;
			//! Le second point.
			gl::Vec3 m_b;
		};
		//! Tableau de lignes.
		using LineArray = std::vector< Points >;
		/**
		*\brief
		*	Représente un sommet utilisé pour dessiner la ligne.
		*/
		struct Vertex
		{
			//! Sa position.
			gl::Vec3 m_position;
			//! Sa normale (par rapport à la pente de la ligne.)
			gl::Vec3 m_normal;
		};
		//! Un quad, constitué de 6 sommets.
		using LineQuad = std::array< Vertex, 6 >;
		//! Un tableau de quad.
		using LineQuadArray = std::vector< LineQuad >;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de l'objet.
		*\param[in] width
		*	La largeur de la ligne.
		*\param[in] feather
		*	La portion de la ligne à partir de laquelle l'alpha blending intervient.
		*/
		PolyLine( std::string const & name
			, float width = 0.05f
			, float feather = 0.5f );
		/**
		*\brief
		*	Retire un point de la liste.
		*\param[in] index
		*	L'index du point.
		*/
		void remove( uint32_t index );
		/**
		*\brief
		*	Ajoute une ligne à la liste.
		*\param[in] line
		*	Les données de la ligne.
		*/
		void add( Points const & line );
		/**
		*\brief
		*	Ajoute une liste de lignes à la liste.
		*\param[in] lines
		*	La liste de lignes.
		*/
		void add( LineArray const & lines );
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
		*\return
		*	Le nombre de lignes.
		*/
		inline uint32_t count()const
		{
			return uint32_t( m_buffer.size() );
		}
		/**
		*\brief
		*	Définit la largeur des lignes.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void width( float value )
		{
			m_width = value;
		}
		/**
		*\return
		*	La largeur des lignes.
		*/
		inline float width()const
		{
			return m_width;
		}
		/**
		*\brief
		*	Définit la portion de la ligne à partir de laquelle l'alpha blending
		*	intervient.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void feather( float value )
		{
			m_feather = value;
		}
		/**
		*\return
		*	La portion de la ligne à partir de laquelle l'alpha blending intervient.
		*/
		inline float feather()const
		{
			return m_feather;
		}
		/**
		*\return
		*	Le tampon contenant les lignes.
		*/
		inline LineQuadArray const & buffer()const
		{
			return m_buffer;
		}

	private:
		//! Le matériau.
		MaterialPtr m_material;
		//! La tampon GPU contenant les lignes.
		LineQuadArray m_buffer;
		//! La largeur des lignes.
		float m_width;
		//! La portion de la ligne à partir de laquelle l'alpha blending intervient.
		float m_feather;
	};
}

#endif
