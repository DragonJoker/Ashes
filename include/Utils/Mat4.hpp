/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Utils/Vec4.hpp"

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
		*	Constructeur.
		*\param[in] v
		*	Les valeurs de la matrice.
		*/
		explicit Mat4T( std::array< T, 16u > const & v )noexcept;
		/**
		*\brief
		*	Constructeur, met la valeur donnée sur la diagonale,
		*	les autres valeurs sont mises à 0.
		*\param[in] v
		*	La valeur de la diagonale.
		*/
		template< typename U >
		explicit Mat4T( U const & v )noexcept;
		/**
		*\brief
		*	Constructeur, depuis 4 colonnes.
		*\param[in] col0, col1, col2, col3
		*	Les 4 colonnes de la matrice.
		*/
		Mat4T( Vec4T< T > const & col0
			, Vec4T< T > const & col1
			, Vec4T< T > const & col2
			, Vec4T< T > const & col3 );
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
			return data.data();
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
			return data.data();
		}
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**\{*/
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
		/**\}*/

	private:
		union
		{
			std::array< T, 16 > data;
			std::array< Vec4T< T >, 4 > cols;
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
	/**\{*/
	template< typename T >
	inline bool operator==( Mat4T< T > const & lhs
		, Mat4T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Mat4T< T > const & lhs
		, Mat4T< T > const & rhs )noexcept;
	/**\}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**\{*/
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
	/**\}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**\{*/
	/**
	*\~english
	*\brief
	*	Return the inverse of a squared matrix.
	*\~french
	*\brief
	*	Retourne l'inverse d'une matrice carrée.
	*/
	template< typename T >
	inline Mat4T< T > inverse( Mat4T< T > const & matrix );
	/**\}*/
}

#include "Utils/Mat4.inl"
