/**
*\file
*	Camera.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Camera_HPP___
#define ___RenderLib_Camera_HPP___
#pragma once

#include "Frustum.h"
#include "Viewport.h"

namespace render
{
	/**
	*\brief
	*	Gère la caméra depuis laquelle les objets sont visualisés.
	*/
	class Camera
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] size
		*	La taille initiale de la zone de rendu.
		*/
		Camera( gl::IVec2 const & size );
		/**
		*\brief
		*	Met à jour la matrice de vue.
		*\return
		*	\p true si la caméra avait changé.
		*/
		bool update()noexcept;
		/**
		*\brief
		*	Redimensionne le viewport.
		*\param[in] size
		*	Les nouvelles dimensions.
		*/
		void resize( gl::IVec2 const & size )noexcept;
		/**
		*\brief
		*	Vérifie si le point donné est visible par la caméra.
		*\param[in] position
		*	La position à vérifier.
		*\return
		*	\p false si la position est dans le frustum du viewport orienté
		*	par la vue de la caméra.
		*/
		bool visible( gl::Vec3 const & position )const;
		/**
		*\return
		*	La position de la caméra.
		*/
		inline gl::Vec3 const & position()const noexcept
		{
			return m_position;
		}
		/**
		*\return
		*	La matrice de vue.
		*/
		inline gl::Mat4 const & view()const noexcept
		{
			return m_transform;
		}
		/**
		*\return
		*	La matrice de projection.
		*/
		inline gl::Mat4 const & projection()const noexcept
		{
			return m_viewport.transform();
		}
		/**
		*\brief
		*	Déplace la caméra de la translation voulue.
		*\param[in] translate
		*	Le vecteur représentant la translation à appliquer.
		*/
		inline void translate( gl::Vec3 const & translate )noexcept
		{
			m_position += translate;
			m_changed |= translate != gl::Vec3{};
		}
		/**
		*\brief
		*	Déplace la caméra à la position voulue.
		*\param[in] position
		*	La nouvelle position.
		*/
		inline void moveTo( gl::Vec3 const & position )noexcept
		{
			m_changed |= position != m_position;
			m_position = position;
		}
		/**
		*\brief
		*	Tourne la caméra de la rotation voulue.
		*\param[in] rotation
		*	Le quaternion représentant la rotation à appliquer.
		*/
		inline void rotate( gl::Quaternion const & rotation )noexcept
		{
			m_orientation *= rotation;
			m_changed |= rotation != gl::Quaternion{};
		}
		/**
		*\brief
		*	Définit l'orientation de la caméra.
		*\param[in] orientation
		*	La nouvelle orientation.
		*/
		inline void reorient( gl::Quaternion const & orientation )noexcept
		{
			m_changed |= orientation != m_orientation;
			m_orientation = orientation;
		}
		/**
		*\return
		*	Le viewport.
		*/
		inline Viewport const & viewport()const noexcept
		{
			return m_viewport;
		}
		/**
		*\return
		*	Le viewport.
		*/
		inline Viewport & viewport()noexcept
		{
			return m_viewport;
		}
		/**
		*\brief
		*	Définit l'angle d'ouverture vertical de la caméra.
		*/
		inline void fovY( gl::Angle const & fovy )noexcept
		{
			m_viewport.fovY( fovy );
		}
		/**
		*\return
		*	L'angle d'ouverture vertical de la caméra.
		*/
		inline gl::Angle const & fovY()const noexcept
		{
			return m_viewport.fovY();
		}
		/**
		*\return
		*	Les plans composant le frustum.
		*/
		inline Frustum const & frustum()const noexcept
		{
			return m_frustum;
		}

	private:
		//! La position de la caméra dans le monde.
		gl::Vec3 m_position;
		//! L'orientation de la caméra dans le monde.
		gl::Quaternion m_orientation;
		//! La matrice de vue.
		gl::Mat4 m_transform;
		//! Le viewport.
		Viewport m_viewport;
		//! Le frustum, dans l'espace vue.
		Frustum m_frustum;
		//! Dit si la caméra a changé depuis le dernier update.
		bool m_changed{ true };
	};
}

#endif
