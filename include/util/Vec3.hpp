/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "util/Vec2.hpp"

namespace utils
{
	/**
	*\brief
	*	Point à 3 composantes.
	*/
	template< typename T >
	class Vec3T
	{
	public:
		/**
		*\brief
		*	Constructeur sans initialisation.
		*/
		explicit Vec3T( NoInit const & )noexcept;
		/**
		*\brief
		*	Constructeur par défaut.
		*/
		constexpr Vec3T()noexcept;
		/**
		*\brief
		*	Constructeur par copie.
		*\param[in] rhs
		*	Le point à copier.
		*/
		Vec3T( Vec3T< T > const & rhs ) = default;
		/**
		*\brief
		*	Opérateur d'affectation par copie.
		*\param[in] rhs
		*	Le point à copier.
		*\return
		*	Une référence sur ce point.
		*/
		Vec3T & operator=( Vec3T< T > const & rhs ) = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y, z
		*	Les valeurs des composantes.
		*/
		template< typename X
			, typename Y
			, typename Z >
		constexpr Vec3T( X const & x
			, Y const & y
			, Z const & z )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y, z
		*	Les valeurs des composantes.
		*/
		template< typename X
			, typename YZ >
		constexpr Vec3T( X const & x
			, Vec2T< YZ > const & yz )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y, z
		*	Les valeurs des composantes.
		*/
		template< typename XY
			, typename Z >
		constexpr Vec3T( Vec2T< XY > const & xy
			, Z const & z )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] v
		*	La valeur des composantes.
		*/
		explicit constexpr Vec3T( T const & v )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] xy
		*	La valeur des composantes x et y.
		*\param[in] z
		*	La valeur de la composante z.
		*/
		constexpr Vec3T( Vec2T< T > const & xy, T const & z )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x
		*	La valeur de la composante x.
		*\param[in] yz
		*	La valeur des composantes y et z.
		*/
		constexpr Vec3T( T const & x, Vec2T< T > const & yz )noexcept;
		/**
		*\brief
		*	Constructeur de conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*/
		template< typename U >
		explicit Vec3T( Vec3T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'affectation par conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*\return
		*	Une référence sur ce point.
		*/
		template< typename U >
		Vec3T & operator=( Vec3T< U > const & rhs )noexcept;
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
		*\~english
		*\brief
		*	Retrieves a constant pointer to the values.
		*\return
		*	The pointer.
		*\~french
		*\brief
		*	Récupère un pointeur constant sur les valeurs.
		*\return
		*	Le pointeur.
		*/
		T const * constPtr()const
		{
			return &data[0];
		}
		/**
		*\~english
		*\brief
		*	Retrieves a pointer to the values.
		*\return
		*	The pointer.
		*\~french
		*\brief
		*	Récupère un pointeur sur les valeurs.
		*\return
		*	Le pointeur.
		*/
		T * ptr()
		{
			return &data[0];
		}
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**\{*/
		template< typename U >
		Vec3T & operator+=( Vec3T< U > const & rhs )noexcept;
		template< typename U >
		Vec3T & operator-=( Vec3T< U > const & rhs )noexcept;
		template< typename U >
		Vec3T & operator*=( Vec3T< U > const & rhs )noexcept;
		template< typename U >
		Vec3T & operator+=( U const & rhs )noexcept;
		template< typename U >
		Vec3T & operator-=( U const & rhs )noexcept;
		template< typename U >
		Vec3T & operator*=( U const & rhs )noexcept;
		template< typename U >
		Vec3T & operator/=( U const & rhs )noexcept;
		/**\}*/

	public:
#pragma warning( push )
#pragma warning( disable: 4201 )
#pragma warning( disable: 4068 )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
		union
		{
			std::array< T, 3u > data;
			struct
			{
				T x;
				T y;
				T z;
			};
			struct
			{
				T r;
				T g;
				T b;
			};
		};
#pragma GCC diagnostic pop
#pragma warning( pop )
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
	T dot( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Produit vectoriel de 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La produit vectoriel.
	*/
	template< typename T >
	Vec3T< T > cross( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Donne la norme d'un point.
	*\param[in] vec
	*	Le point.
	*\return
	*	La norme.
	*/
	template< typename T >
	T length( Vec3T< T > const & vec )noexcept;
	/**
	*\brief
	*	Donne la distance entre 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La distance.
	*/
	template< typename T >
	T distance( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Normalise un point (divise chaque composante par la norme du point.
	*\param[in] vec
	*	Le point à normaliser.
	*\return
	*	Le point normalisé.
	*/
	template< typename T >
	Vec3T< T > normalize( Vec3T< T > const & vec )noexcept;
	/**
	*\brief
	*	Supprime la composante z du point 3D donné pour le transformer en
	*	point 2D.
	*\param[in] vec
	*	Le point 3D.
	*\return
	*	Le point 2D.
	*/
	template< typename T >
	Vec2T< T > toVec2( Vec3T< T > const & vec )noexcept;
	/**
	*\name Opérateurs logiques.
	*/
	/**\{*/
	template< typename T >
	inline bool operator==( Vec3T< T > const & lhs
		, Vec3T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Vec3T< T > const & lhs
		, Vec3T< T > const & rhs )noexcept;
	/**\}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**\{*/
	template< typename T >
	inline Vec3T< T > operator-( Vec3T< T > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator+( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator-( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator*( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator+( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator-( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator*( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator/( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	/**\}*/
}

#include "util/Vec3.inl"
