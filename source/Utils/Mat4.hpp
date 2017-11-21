/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Vectorisation.hpp"

#include <array>

namespace utils
{
	/**
	*\brief
	*	Matrice 4x4.
	*\remarks
	*	Column Major!
	*/
	template< typename T >
	class Mat4T
	{
	public:
		/**
		*\brief
		*	Constructeur sans initialisation.
		*\remarks
		*	N'initialise pas les données de la matrice.
		*/
		Mat4T( NoInit const & )noexcept;
		/**
		*\brief
		*	Constructeur par défaut.
		*\remarks
		*	Initialise la matrice à l'identité.
		*/
		Mat4T()noexcept;
		/**
		*\brief
		*	Constructeur par copie.
		*\param[in] rhs
		*	La matrice à copier.
		*/
		Mat4T( Mat4T< T > const & rhs ) = default;
		/**
		*\brief
		*	Opérateur d'affectation par copie.
		*\param[in] rhs
		*	La matrice à copier.
		*\return
		*	Une référence sur cette matrice.
		*/
		Mat4T & operator=( Mat4T< T > const & rhs ) = default;
		/**
		*\brief
		*	Constructeur, met la valeur donnée sur la diagonale,
		*	les autres valeurs sont mises à 0.
		*\param[in] v
		*	La valeur de la diagonale.
		*/
		template< typename U >
		Mat4T( U const & v )noexcept;
		/**
		*\brief
		*	Constructeur de conversion.
		*\param[in] rhs
		*	La matrice à convertir.
		*/
		template< typename U >
		Mat4T( Mat4T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'affectation par conversion.
		*\param[in] rhs
		*	La matrice à convertir.
		*\return
		*	Une référence sur cette matrice.
		*/
		template< typename U >
		Mat4T & operator=( Mat4T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'indexation.
		*\param[in] index
		*	L'indice de la colonne à récupérer.
		*\return
		*	Une référence sur la colonne.
		*/
		Vec4T< T > & operator[]( size_t index )noexcept;
		/**
		*\brief
		*	Opérateur d'indexation.
		*\param[in] index
		*	L'indice de la colonne à récupérer.
		*\return
		*	Une référence sur la colonne.
		*/
		Vec4T< T > const & operator[]( size_t index )const noexcept;
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		template< typename U >
		inline Mat4T & operator+=( Mat4T< U > const & rhs )noexcept;
		template< typename U >
		inline Mat4T & operator-=( Mat4T< U > const & rhs )noexcept;
		template< typename U >
		inline Mat4T & operator*=( Mat4T< U > const & rhs )noexcept;
		template< typename U >
		inline Mat4T & operator+=( U const & rhs )noexcept;
		template< typename U >
		inline Mat4T & operator-=( U const & rhs )noexcept;
		template< typename U >
		inline Mat4T & operator*=( U const & rhs )noexcept;
		template< typename U >
		inline Mat4T & operator/=( U const & rhs )noexcept;
		/**@}*/

	private:
		union
		{
			std::array< Vec4T< T >, 4 > data;
			struct
			{
				Vec4T< T > col0;
				Vec4T< T > col1;
				Vec4T< T > col2;
				Vec4T< T > col3;
			};
		};
	};
	/**
	*\name Opérateurs logiques.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( Mat4T< T > const & lhs
		, Mat4T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Mat4T< T > const & lhs
		, Mat4T< T > const & rhs )noexcept;
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T, typename U >
	inline Mat4T< T > operator+( Mat4T< T > const & lhs
		, Mat4T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Mat4T< T > operator-( Mat4T< T > const & lhs
		, Mat4T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Mat4T< T > operator*( Mat4T< T > const & lhs
		, Mat4T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Mat4T< T > operator+( Mat4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Mat4T< T > operator-( Mat4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Mat4T< T > operator*( Mat4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Mat4T< T > operator/( Mat4T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T >
	inline Vec4T< T > operator*( Mat4T< T > const & lhs
		, Vec4T< T > const & rhs )noexcept;
	/**@}*/
}

#include "Mat4.inl"
