/**
*\file
*	Movable.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Movable_HPP___
#define ___RenderLib_Movable_HPP___
#pragma once

#include "RenderLibPrerequisites.h"
#include <Utils/Quaternion.hpp>

namespace render
{
	/**
	*\brief
	*	Classe de base des objets positionnables.
	*/
	class Movable
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de l'objet.
		*/
		Movable( std::string const & name )noexcept;
		/**
		*\brief
		*	Met à jour la matrice de transformation de l'objet.
		*/
		void update()noexcept;
		/**
		*\return
		*	La matrice de transformation de l'objet.
		*/
		inline renderer::Mat4 const & transform()const noexcept
		{
			return m_transform;
		}
		/**
		*\brief
		*	Change l'échelle de l'objet, relativement à son échelle courante.
		*\param[in] scale
		*	Le vecteur représentant la mise à l'échelle à appliquer.
		*/
		inline void scale( renderer::Vec3 const & scale )noexcept
		{
			static const renderer::Vec3 noChangeScale{ 1, 1, 1 };

			if ( scale != noChangeScale )
			{
				m_scale *= scale;
				onMovableChanged( *this );
			}
		}
		/**
		*\brief
		*	Définit l'échelle de l'objet, de manière absolue.
		*\param[in] scale
		*	Le vecteur représentant la mise à l'échelle à appliquer.
		*/
		inline void rescale( renderer::Vec3 const & scale )noexcept
		{
			if ( scale != m_scale )
			{
				m_scale = scale;
				onMovableChanged( *this );
			}
		}
		/**
		*\brief
		*	Déplace l'objet de la translation voulue.
		*\param[in] translate
		*	Le vecteur représentant la translation à appliquer.
		*/
		inline void translate( renderer::Vec3 const & translate )noexcept
		{
			static const renderer::Vec3 noChangeTranslate;

			if ( translate != noChangeTranslate )
			{
				m_position += translate;
				onMovableChanged( *this );
			}
		}
		/**
		*\brief
		*	Déplace l'objet à la position voulue.
		*\param[in] position
		*	La nouvelle position.
		*/
		inline void moveTo( renderer::Vec3 const & position )noexcept
		{
			if ( position != m_position )
			{
				m_position = position;
				onMovableChanged( *this );
			}
		}
		/**
		*\brief
		*	Tourne l'objet de la rotation voulue.
		*\param[in] rotation
		*	Le quaternion représentant la rotation à appliquer.
		*/
		inline void rotate( renderer::Quaternion const & rotation )noexcept
		{
			static const renderer::Quaternion noChangeRotate;

			if ( rotation != noChangeRotate )
			{
				m_orientation *= rotation;
				onMovableChanged( *this );
			}
		}
		/**
		*\brief
		*	Définit l'orientation de l'objet.
		*\param[in] orientation
		*	La nouvelle orientation.
		*/
		inline void reorient( renderer::Quaternion const & orientation )noexcept
		{
			if ( orientation != m_orientation )
			{
				m_orientation = orientation;
				onMovableChanged( *this );
			}
		}
		/**
		*\return
		*	La position de l'objet.
		*/
		inline renderer::Vec3 const & position()const noexcept
		{
			return m_position;
		}
		/**
		*\return
		*	L'échelle de l'objet.
		*/
		inline renderer::Vec3 const & scale()const noexcept
		{
			return m_scale;
		}
		/**
		*\return
		*	L'orientation de l'objet.
		*/
		inline renderer::Quaternion const & orientation()const noexcept
		{
			return m_orientation;
		}
		/**
		*\return
		*	Le nom de l'objet.
		*/
		inline std::string const & name()const noexcept
		{
			return m_name;
		}
		/**
		*\return
		*	Le statut de visibilité de l'objet.
		*/
		inline bool visible()const noexcept
		{
			return m_visible;
		}
		/**
		*\brief
		*	Définit le statut de visibilité de l'objet.
		*\param[in] show
		*	Le nouveau statut.
		*/
		inline void show( bool show = true )noexcept
		{
			if ( m_visible != show )
			{
				m_visible = show;
				onMovableChanged( *this );
			}
		}

	public:
		//! La notification d'objet changé.
		OnMovableChanged onMovableChanged;

	protected:

		//! La position de l'objet dans le monde.
		renderer::Vec3 m_position{ 0, 0, 0 };
		//! L'échelle de l'objet dans le monde.
		renderer::Vec3 m_scale{ 1, 1, 1 };
		//! L'orientation de l'objet dans le monde.
		renderer::Quaternion m_orientation;
		//! La matrice de transformation de l'objet.
		renderer::Mat4 m_transform;
		// Le nom de l'objet.
		std::string m_name;
		//! Le statut de visibilité.
		bool m_visible{ true };
	};
}

#endif
