/**
*\file
*	Frustum.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Frustum_HPP___
#define ___RenderLib_Frustum_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include "PlaneEquation.h"

#include <array>

namespace render
{
	/**
	*\brief
	*	Liste des plans du frustum.
	*/
	enum class FrustumPlane
	{
		//! Plan de gauche.
		eLeft,
		//! Plan de droite.
		eRight,
		//! Plan de dessus.
		eTop,
		//! Plan de dessous.
		eBottom,
		//! Premier plan.
		eNear,
		//! Arrière plan.
		eFar,
		GlLib_EnumBounds( eLeft )
	};
	/**
	\brief
	*	Implémente un frustum et les vérifications relatives au frustum
	*	culling.
	*/
	class Frustum
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] viewport
		*	Le viewport.
		*/
		Frustum( Viewport & viewport );
		/**
		*\brief
		*	Met à jour les plans du frustum.
		*\param[in] position
		*	La position de la vue.
		*\param[in] x
		*	Le vecteur X.
		*\param[in] y
		*	Le vecteur Y.
		*\param[in] z
		*	Le vecteur Z.
		*/
		void update( gl::Vec3 const & position
			, gl::Vec3 const & x
			, gl::Vec3 const & y
			, gl::Vec3 const & z );
		/**
		*\brief
		*	Vérifie si le point donné est dans le frustum de vue.
		*\param[in] point
		*	Le point.
		*\return
		*	\p false si le point en dehors du frustum de vue.
		*/
		bool visible( gl::Vec3 const & point )const;

	private:
		//! Le viewport.
		Viewport & m_viewport;
		//! Les plans du frustum de vue.
		std::array< PlaneEquation, size_t( FrustumPlane::eCount ) > m_planes;
	};
}

#endif
