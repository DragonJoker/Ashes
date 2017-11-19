/**
*\file
*	FlagCombination.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_FlagCombination_HPP___
#define ___RenderLib_FlagCombination_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

namespace render
{
	/**
	*\brief
	*	Classe template qui fournit une conversion implicite depuis un scoped
	*	enum vers un type entier de base.
	*\remarks
	*	Permet de définir des indicateurs via des combinaisons binaires (&, |),
	*	dont les opérandes seront obligatoirement de même taille binaire.
	*\param FlagType
	*	Le type de scoped enum.
	*/
	template< typename FlagType >
	class FlagCombination
	{
	public:
		//! Le type de base de l'enum.
		using BaseType = typename std::underlying_type< FlagType >::type;

	public:
		/**
		*\brief
		*	Constructeur depuis le type d'indicateur.
		*\param[in]	value
		*	La valeur.
		*/
		inline FlagCombination( FlagType value )noexcept
			: m_value{ BaseType( value ) }
		{
			static_assert( sizeof( FlagType ) == sizeof( BaseType )
				, "Can't combine different size parameters" );
		}
		/**
		*\brief
		*	Constructeur depuis des indicateurs combinés.
		*\param[in] value
		*	La valeur.
		*/
		inline FlagCombination( BaseType value = BaseType{} )noexcept
			: m_value{ value }
		{
			static_assert( sizeof( FlagType ) == sizeof( BaseType )
				, "Can't combine different size parameters" );
		}
		/**
		*\brief
		*	Opérateur d'affectation par ET binaire.
		*\param[in] rhs
		*	La valeur.
		*\return
		*	Une référence sur cet objet.
		*/
		inline FlagCombination & operator&=( BaseType rhs )noexcept
		{
			m_value &= rhs;
			return *this;
		}
		/**
		*\brief
		*	Opérateur d'affectation par OU binaire.
		*\param[in] rhs
		*	La valeur.
		*\return
		*	Une référence sur cet objet.
		*/
		inline FlagCombination & operator|=( BaseType rhs )noexcept
		{
			m_value |= rhs;
			return *this;
		}
		/**
		*\brief
		*	Opérateur d'affectation par ET binaire.
		*\param[in] rhs
		*	La valeur.
		*\return
		*	Une référence sur cet objet.
		*/
		inline FlagCombination & operator&=( FlagType rhs )noexcept
		{
			m_value &= BaseType( rhs );
			return *this;
		}
		/**
		*\brief
		*	Opérateur d'affectation par OU binaire.
		*\param[in] rhs
		*	La valeur.
		*\return
		*	Une référence sur cet objet.
		*/
		inline FlagCombination & operator|=( FlagType rhs )noexcept
		{
			m_value |= BaseType( rhs );
			return *this;
		}
		/**
		*\brief
		*	Opérateur d'affectation par ET binaire.
		*\param[in] rhs
		*	La valeur.
		*\return
		*	Une référence sur cet objet.
		*/
		inline FlagCombination & operator&=
			( FlagCombination< FlagType > const & rhs )noexcept
		{
			m_value &= BaseType( rhs );
			return *this;
		}
		/**
		*\brief
		*	Opérateur d'affectation par OU binaire.
		*\param[in] rhs
		*	La valeur.
		*\return
		*	Une référence sur cet objet.
		*/
		inline FlagCombination & operator|=
			( FlagCombination< FlagType > const & rhs )noexcept
		{
			m_value |= BaseType( rhs );
			return *this;
		}
		/**
		*\brief
		*	Conversion implicite vers le type entier de base.
		*/
		inline operator BaseType()const noexcept
		{
			return m_value;
		}

	private:
		//! La valeur résultant des combinaisons.
		BaseType m_value;
	};
	/**
	*\brief
	*	Opérateur d'égalité.
	*\param[in] lhs, rhs
	*	Les opérandes.
	*/
	template< typename FlagType >
	inline bool operator==( FlagCombination< FlagType > const & lhs
		, FlagCombination< FlagType > const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) == Type( rhs );
	}
	/**
	*\brief
	*	Opérateur d'égalité.
	*\param[in] lhs, rhs
	*	Les opérandes.
	*/
	template< typename FlagType >
	inline bool operator==( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) == Type( rhs );
	}
	/**
	*\brief
	*	Opérateur de différence.
	*\param[in] lhs, rhs
	*	Les opérandes.
	*/
	template< typename FlagType >
	inline bool operator!=( FlagCombination< FlagType > const & lhs
		, FlagCombination< FlagType > const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) != Type( rhs );
	}
	/**
	*\brief
	*	Opérateur de différence.
	*\param[in] lhs, rhs
	*	Les opérandes.
	*/
	template< typename FlagType >
	inline bool operator!=( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) != Type( rhs );
	}
	/**
	*\brief
	*	Opérateur ET binaire.
	*\param[in] lhs, rhs
	*	Les opérandes.
	*\return
	*	La combinaison de flags résultante.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > operator&
		( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret &= rhs;
	}
	/**
	*\brief
	*	Opérateur OU binaire.
	*\param[in] lhs, rhs
	*	Les opérandes.
	*\return
	*	La combinaison de flags résultante.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > operator|
		( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret |= rhs;
	}
	/**
	*\brief
	*	Vérifie si l'indicateur donné est présent dans la combinaison donnée.
	*\param[in] value
	*	La combinaison.
	*\param[in] flag
	*	L'indicateur recherché.
	*\return
	*	\p true si value contient flag.
	*/
	template< typename T, typename U >
	inline bool checkFlag( T const & value, U const & flag )noexcept
	{
		static_assert( sizeof( T ) == sizeof( U )
			, "Can't check flags for different size parameters" );
		return U( value & T( flag ) ) == flag;
	}
	/**
	*\brief
	*	Vérifie si l'indicateur donné est présent dans la combinaison donnée.
	*\param[in] value
	*	La combinaison.
	*\param[in] flag
	*	L'indicateur recherché.
	*\return
	*	\p true si value contient flag.
	*/
	template< typename FlagType >
	inline bool checkFlag( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( value & flag ) == Type( flag );
	}
	/**
	*\brief
	*	Vérifie si l'indicateur donné est présent dans la combinaison donnée.
	*\param[in] value
	*	La combinaison.
	*\param[in] flag
	*	L'indicateur recherché.
	*\return
	*	\p true si value contient flag.
	*/
	template< typename FlagType, typename Type >
	inline bool checkFlag( FlagCombination< FlagType > const & value
		, Type const & flag )noexcept
	{
		static_assert( sizeof( FlagType ) == sizeof( Type )
			, "Can't check flags for different size parameters" );
		return Type( value & flag ) == flag;
	}
	/**
	*\brief
	*	Ajoute un indicateur à la valeur donnée.
	*\param[in,out] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à ajouter.
	*\return
	*	La valeur.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > & addFlag
		( FlagCombination< FlagType > & value
		, FlagType const & flag )noexcept
	{
		value |= flag;
		return value;
	}
	/**
	*\brief
	*	Enlève un indicateur de la valeur donnée.
	*\param[in,out] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à enlever.
	*\return
	*	La valeur.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > & remFlag
		( FlagCombination< FlagType > & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		value &= ~Type( flag );
		return value;
	}
	/**
	*\brief
	*	Ajoute un indicateur à la valeur donnée.
	*\param[in] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à ajouter.
	*\return
	*	La nouvelle valeur.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > addFlag
		( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		return ( value | flag );
	}
	/**
	*\brief
	*	Enlève un indicateur de la valeur donnée.
	*\param[in] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à enlever.
	*\return
	*	La nouvelle valeur.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > remFlag
		( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return value & ~Type( flag );
	}
}

//! Implémente les opérateurs de combinaison binaire sur un scoped enum.
#define RenderLib_ImplementFlag( FlagType )\
	using FlagType##s = render::FlagCombination< FlagType >;\
	inline constexpr typename render::FlagCombination< FlagType >::BaseType\
	operator|\
		( FlagType lhs\
		, FlagType rhs )\
	{\
		using Type = typename render::FlagCombination< FlagType >::BaseType;\
		return Type( lhs ) | Type( rhs );\
	}\
	inline constexpr typename render::FlagCombination< FlagType >::BaseType\
	operator|\
		( typename render::FlagCombination< FlagType >::BaseType lhs\
		, FlagType rhs )\
	{\
		using Type = typename render::FlagCombination< FlagType >::BaseType;\
		return lhs | Type( rhs );\
	}\
	inline constexpr typename render::FlagCombination< FlagType >::BaseType\
	operator|\
		( FlagType lhs\
		, typename render::FlagCombination< FlagType >::BaseType rhs )\
	{\
		using Type = typename render::FlagCombination< FlagType >::BaseType;\
		return Type( lhs ) | rhs;\
	}

#endif
