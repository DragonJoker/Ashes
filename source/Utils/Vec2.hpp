/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "UtilsPrerequisites.hpp"

namespace utils
{
	/**
	*\brief
	*	Classe servant uniquement à construire des points et matrices sans
	*	initialiser leurs données.
	*/
	struct NoInit
	{
	};
	//! Instance statique de noInit pour éviter d'en construire.
	static const NoInit noInit;
	/**
	*\brief
	*	Point à 2 composantes.
	*/
	template< typename T >
	class Vec2T
	{
	public:
		/**
		*\brief
		*	Constructeur sans initialisation.
		*/
		Vec2T( NoInit const & )noexcept;
		/**
		*\brief
		*	Constructeur par défaut.
		*/
		constexpr Vec2T()noexcept;
		/**
		*\brief
		*	Constructeur par copie.
		*\param[in] rhs
		*	Le point à copier.
		*/
		Vec2T( Vec2T< T > const & rhs ) = default;
		/**
		*\brief
		*	Opérateur d'affectation par copie.
		*\param[in] rhs
		*	Le point à copier.
		*\return
		*	Une référence sur ce point.
		*/
		Vec2T & operator=( Vec2T< T > const & rhs ) = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y
		*	Les valeurs des composantes.
		*/
		template< typename X
			, typename Y>
		constexpr Vec2T( X const & x
			, Y const & y )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] v
		*	La valeur des composantes.
		*/
		explicit constexpr Vec2T( T const & v )noexcept;
		/**
		*\brief
		*	Constructeur de conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*/
		template< typename U >
		explicit Vec2T( Vec2T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'affectation par conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*\return
		*	Une référence sur ce point.
		*/
		template< typename U >
		Vec2T & operator=( Vec2T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'indexation.
		*\param[in] index
		*	L'indice de la composante à récupérer.
		*\return
		*	Une référence sur la composante.
		*/
		T & operator[]( size_t index )noexcept;
		/**
		*\brief
		*	Opérateur d'indexation.
		*\param[in] index
		*	L'indice de la composante à récupérer.
		*\return
		*	Une référence sur la composante.
		*/
		T const & operator[]( size_t index )const noexcept;
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		template< typename U >
		inline Vec2T & operator+=( Vec2T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec2T & operator-=( Vec2T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec2T & operator*=( Vec2T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec2T & operator+=( U const & rhs )noexcept;
		template< typename U >
		inline Vec2T & operator-=( U const & rhs )noexcept;
		template< typename U >
		inline Vec2T & operator*=( U const & rhs )noexcept;
		template< typename U >
		inline Vec2T & operator/=( U const & rhs )noexcept;
		/**@}*/

	public:
		//! Les composantes du point.
		union
		{
			T data[2];
			struct
			{
				T x;
				T y;
			};
			struct
			{
				T r;
				T g;
			};
		};
	};
	/**
	*\brief
	*	Produit scalaire de 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La produit scalaire.
	*/
	template< typename T >
	T dot( Vec2T< T > const & lhs, Vec2T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Donne la norme d'un point.
	*\param[in] vec
	*	Le point.
	*\return
	*	La norme.
	*/
	template< typename T >
	T length( Vec2T< T > const & vec )noexcept;
	/**
	*\brief
	*	Donne la distance entre 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La distance.
	*/
	template< typename T >
	T distance( Vec2T< T > const & lhs, Vec2T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Normalise un point (divise chaque composante par la norme du point.
	*\param[in] vec
	*	Le point à normaliser.
	*\return
	*	Le point normalisé.
	*/
	template< typename T >
	Vec2T< T > normalize( Vec2T< T > const & vec )noexcept;
	/**
	*\name Opérateurs logiques.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( Vec2T< T > const & lhs
		, Vec2T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Vec2T< T > const & lhs
		, Vec2T< T > const & rhs )noexcept;
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline Vec2T< T > operator-( Vec2T< T > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator+( Vec2T< T > const & lhs
		, Vec2T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator-( Vec2T< T > const & lhs
		, Vec2T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator*( Vec2T< T > const & lhs
		, Vec2T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator+( Vec2T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator-( Vec2T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator*( Vec2T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec2T< T > operator/( Vec2T< T > const & lhs
		, U const & rhs )noexcept;
	/**@}*/
}

#include "Vec2.inl"
