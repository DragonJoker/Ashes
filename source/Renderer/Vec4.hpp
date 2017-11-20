/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Vec3.hpp"

namespace renderer
{
	/**
	*\brief
	*	Point à 4 composantes.
	*/
	template< typename T >
	class Vec4T
	{
	public:
		/**
		*\brief
		*	Constructeur sans initialisation.
		*/
		Vec4T( NoInit const & )noexcept;
		/**
		*\brief
		*	Constructeur par défaut.
		*/
		constexpr Vec4T()noexcept;
		/**
		*\brief
		*	Constructeur par copie.
		*\param[in] rhs
		*	Le point à copier.
		*/
		Vec4T( Vec4T< T > const & rhs ) = default;
		/**
		*\brief
		*	Opérateur d'affectation par copie.
		*\param[in] rhs
		*	Le point à copier.
		*\return
		*	Une référence sur ce point.
		*/
		Vec4T & operator=( Vec4T< T > const & rhs ) = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y, z, w
		*	Les valeurs des composantes.
		*/
		template< typename X
			, typename Y
			, typename Z
			, typename W >
		constexpr Vec4T( X const & x
			, Y const & y
			, Z const & z
			, W const & w )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] v
		*	La valeur des composantes.
		*/
		explicit constexpr Vec4T( T const & v )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] xy
		*	La valeur des composantes x et y.
		*\param[in] z
		*	La valeur de la composante z.
		*\param[in] w
		*	La valeur de la composante w.
		*/
		constexpr Vec4T( Vec2T< T > const & xy, T const & z, T const & w )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x
		*	La valeur de la composante x.
		*\param[in] yz
		*	La valeur des composantes y et z.
		*\param[in] w
		*	La valeur de la composante w.
		*/
		constexpr Vec4T( T const & x, Vec2T< T > const & yz, T const & w )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x
		*	La valeur de la composante x.
		*\param[in] y
		*	La valeur de la composante y.
		*\param[in] zw
		*	La valeur des composantes z et w.
		*/
		constexpr Vec4T( T const & x, T const & y, Vec2T< T > const & zw )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] xyz
		*	La valeur des composantes x, y et z.
		*\param[in] w
		*	La valeur de la composante w.
		*/
		constexpr Vec4T( Vec3T< T > const & xyz, T const & w )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x
		*	La valeur de la composante x.
		*\param[in] yzw
		*	La valeur des composantes y, z et w.
		*/
		constexpr Vec4T( T const & x, Vec3T< T > const & yzw )noexcept;
		/**
		*\brief
		*	Constructeur de conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*/
		template< typename U >
		explicit Vec4T( Vec4T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'affectation par conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*\return
		*	Une référence sur ce point.
		*/
		template< typename U >
		Vec4T & operator=( Vec4T< U > const & rhs )noexcept;
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
		inline Vec4T & operator+=( Vec4T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec4T & operator-=( Vec4T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec4T & operator*=( Vec4T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec4T & operator+=( U const & rhs )noexcept;
		template< typename U >
		inline Vec4T & operator-=( U const & rhs )noexcept;
		template< typename U >
		inline Vec4T & operator*=( U const & rhs )noexcept;
		template< typename U >
		inline Vec4T & operator/=( U const & rhs )noexcept;
		/**@}*/

	public:
		//! Les composantes du point.
		union
		{
			T data[4];
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};
			struct
			{
				T r;
				T g;
				T b;
				T a;
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
	T dot( Vec4T< T > const & lhs, Vec4T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Donne la norme d'un point.
	*\param[in] vec
	*	Le point.
	*\return
	*	La norme.
	*/
	template< typename T >
	T length( Vec4T< T > const & vec )noexcept;
	/**
	*\brief
	*	Donne la distance entre 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La distance.
	*/
	template< typename T >
	T distance( Vec4T< T > const & lhs, Vec4T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Normalise un point (divise chaque composante par la norme du point.
	*\param[in] vec
	*	Le point à normaliser.
	*\return
	*	Le point normalisé.
	*/
	template< typename T >
	Vec4T< T > normalize( Vec4T< T > const & vec )noexcept;
	/**
	*\brief
	*	Supprime les composantes z et w du point 4D donné pour le transformer
	*	en point 2D.
	*\param[in] vec
	*	Le point 4D.
	*\return
	*	Le point 2D.
	*/
	template< typename T >
	Vec2T< T > toVec2( Vec4T< T > const & vec )noexcept;
	/**
	*\brief
	*	Supprime la composante w du point 4D donné pour le transformer en
	*	point 3D.
	*\param[in] vec
	*	Le point 4D.
	*\return
	*	Le point 3D.
	*/
	template< typename T >
	Vec3T< T > toVec3( Vec4T< T > const & vec )noexcept;
	/**
	*\name Opérateurs logiques.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( Vec4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Vec4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept;
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline Vec4T< T > operator-( Vec4T< T > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator+( Vec4T< T > const & lhs
		, Vec4T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator-( Vec4T< T > const & lhs
		, Vec4T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator*( Vec4T< T > const & lhs
		, Vec4T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator+( Vec4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator-( Vec4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator*( Vec4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator/( Vec4T< T > const & lhs
		, U const & rhs )noexcept;
	/**@}*/
}

#include "Vec4.inl"
