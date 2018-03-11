/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
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
		inline T const * constPtr()const
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
		inline T * ptr()
		{
			return &data[0];
		}
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**\{*/
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
		/**\}*/

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
	*\name Opérateurs logiques.
	*/
	/**\{*/
	template< typename T >
	inline bool operator==( Vec4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Vec4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept;
	/**\}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**\{*/
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
	/**\}*/
}

#include "Vec4.inl"
