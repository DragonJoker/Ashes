/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Quaternion.hpp"
#include "Angle.hpp"

namespace utils
{
	/**
	*\brief
	*	Effectue sur \p m une translation du vecteur \p v donné.
	*\param[in] m
	*	La matrice.
	*\param[in] v
	*	La translation.
	*\return
	*	Le résultat de la translation.
	*/
	template< typename T >
	Mat4T< T > translate( Mat4T< T > const & m, Vec3T< T > const & v );
	/**
	*\brief
	*	Effectue sur \p m une mise à l'échelle de du vecteur \p v donné.
	*\param[in] m
	*	La matrice.
	*\param[in] v
	*	La valeur de mise à l'échelle.
	*\return
	*	Le résultat de la mise à l'échelle.
	*/
	template< typename T >
	Mat4T< T > scale( Mat4T< T > const & m, Vec3T< T > const & v );
	/**
	*\brief
	*	Effectue sur \p m une rotation de \p angle autour de l'axe \p v donné.
	*\param[in] m
	*	La matrice.
	*\param[in] angle
	*	L'angle de rotation.
	*\param[in] v
	*	L'axe de rotation.
	*\return
	*	Le résultat de la rotation.
	*/
	template< typename T >
	Mat4T< T > rotate( Mat4T< T > const & m, T angle, Vec3T< T > const & v );
	/**
	*\brief
	*	Calcule une matrice de vue de type look at.
	*\param[in] eye
	*	La position de la caméra.
	*\param[in] center
	*	Le point regardé par la caméra.
	*\param[in] up
	*	L'axe vertical de la caméra.
	*/
	template< typename T >
	Mat4T< T > lookAt( Vec3T< T > const & eye
		, Vec3T< T > const & center
		, Vec3T< T > const & up );
	/**
	*\brief
	*	Calcule une matrice de projection en perspective sans clipping
	*	d'arrière plan.
	*\param[in] fovy
	*	L'angle d'ouverture verticale.
	*\param[in] aspect
	*	Le ratio largeur / hauteur.
	*\param[in] zNear
	*	La position du premier plan (pour le clipping).
	*/
	template< typename T >
	Mat4T< T > infinitePerspective( RadiansT< T > fovy
		, T aspect
		, T zNear );
	/**
	*\brief
	*	Calcule une matrice de projection orthographique.
	*\param[in] left, right
	*	La position des plans gauche et droite.
	*\param[in] top, bottom
	*	La position des plans haut et bas.
	*\param[in] zNear, zFar
	*	La position des premier et arrière plans.
	*/
	template< typename T >
	Mat4T< T > ortho( T left
		, T right
		, T bottom
		, T top
		, T zNear
		, T zFar );
	/**
	*\brief
	*	La position dans le monde de la coordonnée écran donnée.
	*\param[in] obj
	*	La coordonnée écran.
	*\param[in] model, proj
	*	Les matrices permettant de calculer la matrice monde.
	*\param[in] viewport
	*	La définition du champ de vision.
	*/
	template< typename T, typename U >
	Vec3T< T > project( Vec3T< T > const & obj
		, Mat4T< T > const & model
		, Mat4T< T > const & proj
		, Vec4T< U > const & viewport );
}

#include "Transform.inl"
