/**
*\file
*	PlaneEquation.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_PlaneEquation_HPP___
#define ___RenderLib_PlaneEquation_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

namespace render
{
	/**
	*\brief
	*	Equation de plan
	*\remark
	*	Vous connaissez ax + by + cz + d = 0 ?
	*/
	class PlaneEquation
	{
	public:
		/**
		*\brief
		*	Constructeur par défaut.
		*/
		PlaneEquation();
		/**
		*\brief
		*	Constructeur.
		*\param[in] p1
		*	Le premier point appartenant au plan.
		*\param[in] p2
		*	Le second point appartenant au plan.
		*\param[in] p3
		*	Le troisième point appartenant au plan.
		*/
		PlaneEquation(gl::Vec3 const & p1
			, gl::Vec3 const & p2
			, gl::Vec3 const & p3 );
		/**
		*\brief
		*	Constructeur.
		*\param[in] normal
		*	La normale du plan.
		*\param[in] point
		*	Un point appartenant au plan.
		*/
		PlaneEquation( gl::Vec3 const & normal
			, gl::Vec3 const & point );
		/**
		*\brief
		*	Destructeur.
		*/
		~PlaneEquation();
		/**
		*\brief
		*	Calcule l'équation du plan en fonction de 3 points.
		*\param[in] p1
		*	Le premier point appartenant au plan.
		*\param[in] p2
		*	Le second point appartenant au plan.
		*\param[in] p3
		*	Le troisième point appartenant au plan.
		*/
		void set( gl::Vec3 const & p1
			, gl::Vec3 const & p2
			, gl::Vec3 const & p3 );
		/**
		*\brief
		*	Définit l'équation du plan.
		*\param[in] normal
		*	La normale du plan.
		*\param[in] point
		*	Un point appartenant au plan.
		*/
		void set( gl::Vec3 const & normal
			, gl::Vec3 const & point );
		/**
		*\brief
		*	Vérifie si ce plan est parallèle à un autre, id est leurs normales sont
		*	parallèles.
		*\param[in] plane
		*	Le plan a tester.
		*/
		bool parallel( PlaneEquation const & plane )const;
		/**
		*\brief
		*	Calcule la distance entre un point et ce plan.
		*\param[in] point
		*	Le point.
		*\return
		*	La distance, positive si le point est du même côté que la normale
		*	du plan.
		*/
		float distance( gl::Vec3 const & point )const;
		/**
		*\brief
		*	Récupère la projection d'un point sur ce plan.
		*\param[in] point
		*	Le point à projeter.
		*\return
		*	Le résultat de la projection.
		*/
		gl::Vec3 project( gl::Vec3 const & point )const;
		/**
		*\brief
		*	Vérifie si ce plan en croise un autre, donne la ligne
		*	d'intersection si elle existe.
		*\param[in] plane
		*	Le plan à tester.
		*\return
		*	\p true s'il y a une intersection.
		*/
		bool intersects( PlaneEquation const & plane )const;
		/**
		*\brief
		*	Vérifie si ce plan en croise deux autres, donne le point d
		*	intersection si il existe.
		*\param[in] plane1
		*	Le permier plan à tester.
		*\param[in] plane2
		*	Le second plan à tester.
		*\param[out] intersection
		*	Le point d'intersection.
		*\return
		*	\p true s'il y a un point d'intersection entre les 3 plans.
		*/
		bool intersects( PlaneEquation const & plane1
			, PlaneEquation const & plane2
			, gl::Vec3 & intersection )const;
		/**
		*\return
		*	La normale du plan.
		*/
		inline gl::Vec3 const & normal()const
		{
			return m_normal;
		}
		/**
		*\return
		*	Le point de référence du plan.
		*/
		inline gl::Vec3 const & point()const
		{
			return m_point;
		}

	private:
		//! La normale du plan.
		gl::Vec3 m_normal;
		//! Le point de référence du plan.
		gl::Vec3 m_point;
		//! Le coefficient.
		float m_d;

		friend bool operator==( PlaneEquation const &, PlaneEquation const & );
		friend bool operator!=( PlaneEquation const &, PlaneEquation const & );
	};
	/**
	*\brief
	*	Vérifie si un plan est égal à un autre.
	*\remarks
	*	Deux plans sont égaux si ils sont parallèles et si leurs normales et
	*	coeff sont égaux.
	*/
	bool operator==( PlaneEquation const & lhs, PlaneEquation const & rhs );
	/**
	*\brief
	*	Vérifie si ce plan est différent d'un autre.
	*/
	bool operator!=( PlaneEquation const & lhs, PlaneEquation const & rhs );
}

#endif
