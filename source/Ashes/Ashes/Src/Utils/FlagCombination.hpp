/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#ifndef ___Ashes_FlagCombination_HPP___
#define ___Ashes_FlagCombination_HPP___
#pragma once

#include <type_traits>

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Classe template d'itérateur sur une combinaison binaire d'indicateurs.
	*\remarks
	*	Le parcours se fait uniquement vers "l'avant", en déplaçant un index servant à construire un masque.
	*\param FlagType
	*	Le type de scoped enum.
	*\~english
	*\brief
	*	Template iterator class on a binary combination of flags.
	*\remarks
	*	Allows definition of flags, through binary operations (&, |),
	*	for which operands will be of same binary size.
	*\param FlagType
	*	The scoped enum type.
	*/
	template< typename FlagType >
	struct FlagIterator
	{
	public:
		//!@~french		Le type entier de base de l'enum.
		//!@~english	The basic integer integer.
		using BaseType = typename std::underlying_type< FlagType >::type;

	public:
		/**
		* Begin ctor.
		*/
		FlagIterator( BaseType value )
			: m_initialValue{ value }
		{
			doGetNextValue();
		}
		/**
		* End ctor.
		*/
		FlagIterator()
			: m_initialValue{ 0u }
			, m_index{ sizeof( BaseType ) * 8 }
		{
		}

		FlagIterator & operator++()
		{
			doGetNextValue();
			return *this;
		}

		FlagIterator operator++( int )
		{
			FlagIterator result{ *this };
			++( *this );
			return result;
		}

		FlagType operator*()const
		{
			return m_value;
		}

		bool operator==( FlagIterator< FlagType > const & rhs )
		{
			return m_index == rhs.m_index
				&& m_initialValue == rhs.m_initialValue;
		}

		bool operator!=( FlagIterator< FlagType > const & rhs )
		{
			return m_index != rhs.m_index
				|| m_initialValue != rhs.m_initialValue;
		}

	private:
		void doGetNextValue()
		{
			auto v = BaseType{ 1u };

			do
			{
				m_value = FlagType( m_initialValue & ( v << m_index ) );
				++m_index;
			}
			while ( m_value == FlagType( 0 ) && m_index < sizeof( BaseType ) * 8 );
		}

	private:
		BaseType m_initialValue;
		size_t m_index{ 0u };
		FlagType m_value;
	};
	/**
	*\~french
	*\brief
	*	Classe template qui fournit une conversion implicite depuis un scoped
	*	enum vers un type entier de base.
	*\remarks
	*	Permet de définir des indicateurs via des combinaisons binaires (&, |),
	*	dont les opérandes seront obligatoirement de même taille binaire.
	*\param FlagType
	*	Le type de scoped enum.
	*\~english
	*\brief
	*	Template class providing implicit conversion from a scoped enum to a
	*	basic integer type.
	*\remarks
	*	Allows definition of flags, through binary operations (&, |),
	*	for which operands will be of same binary size.
	*\param FlagType
	*	The scoped enum type.
	*/
	template< typename FlagType >
	class FlagCombination
	{
	public:
		//!@~french		Le type entier de base de l'enum.
		//!@~english	The basic integer integer.
		using BaseType = typename std::underlying_type< FlagType >::type;

	public:
		/**
		*\~french
		*\brief
		*	Constructeur depuis le type d'indicateur.
		*\param[in]	value
		*	La valeur.
		*\~french
		*\brief
		*	Constructor from flag type.
		*\param[in]	value
		*	The value.
		*/
		inline FlagCombination( FlagType value )noexcept
			: m_value{ BaseType( value ) }
		{
			static_assert( sizeof( FlagType ) == sizeof( BaseType )
				, "Can't combine different size parameters" );
		}
		/**
		*\~french
		*\brief
		*	Constructeur depuis des indicateurs combinés.
		*\param[in] value
		*	La valeur.
		*\~french
		*\brief
		*	Constructor from combined flags.
		*\param[in] value
		*	The value.
		*/
		inline FlagCombination( BaseType value = BaseType{} )noexcept
			: m_value{ value }
		{
			static_assert( sizeof( FlagType ) == sizeof( BaseType )
				, "Can't combine different size parameters" );
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers le type entier de base.
		*\~english
		*\brief
		*	Implicit convertion to the basic integer type.
		*/
		inline operator BaseType const &()const noexcept
		{
			return m_value;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers le type entier de base.
		*\~english
		*\brief
		*	Implicit convertion to the basic integer type.
		*/
		inline BaseType value()const noexcept
		{
			return m_value;
		}
		/**
		*\~french
		*\return
		*	L'itérateur de début.
		*\~english
		*\brief
		*	The beginning iterator.
		*/
		inline FlagIterator< FlagType > begin()const noexcept
		{
			return FlagIterator< FlagType >( m_value );
		}
		/**
		*\~french
		*\return
		*	L'itérateur de fin (0).
		*\~english
		*\brief
		*	The end iterator (0).
		*/
		inline FlagIterator< FlagType > end()const noexcept
		{
			return FlagIterator< FlagType >();
		}
		/**
		*\~french
		*\name Opérateurs binaires membres.
		*\~english
		*\name Member binary operators.
		*/
		/**\{*/
		inline FlagCombination & operator&=( BaseType rhs )noexcept
		{
			m_value &= rhs;
			return *this;
		}

		inline FlagCombination & operator|=( BaseType rhs )noexcept
		{
			m_value |= rhs;
			return *this;
		}

		inline FlagCombination & operator&=( FlagType rhs )noexcept
		{
			m_value &= BaseType( rhs );
			return *this;
		}

		inline FlagCombination & operator|=( FlagType rhs )noexcept
		{
			m_value |= BaseType( rhs );
			return *this;
		}

		inline FlagCombination & operator&=
			( FlagCombination< FlagType > const & rhs )noexcept
		{
			m_value &= BaseType( rhs );
			return *this;
		}

		inline FlagCombination & operator|=
			( FlagCombination< FlagType > const & rhs )noexcept
		{
			m_value |= BaseType( rhs );
			return *this;
		}
		/**\}*/

	private:
		//! La valeur résultant des combinaisons.
		BaseType m_value;
	};
	/**
	*\~french
	*\name Opérateurs de comparaison.
	*\~english
	*\name Comparison operators.
	*/
	/**\{*/
	template< typename FlagType >
	inline bool operator==( FlagCombination< FlagType > const & lhs
		, FlagCombination< FlagType > const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) == Type( rhs );
	}

	template< typename FlagType >
	inline bool operator==( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) == Type( rhs );
	}

	template< typename FlagType >
	inline bool operator!=( FlagCombination< FlagType > const & lhs
		, FlagCombination< FlagType > const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) != Type( rhs );
	}

	template< typename FlagType >
	inline bool operator!=( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( lhs ) != Type( rhs );
	}
	/**\}*/
	/**
	*\~french
	*\name Opérateurs binaires.
	*\~english
	*\name Binary operators.
	*/
	/**\{*/
	template< typename FlagType >
	inline FlagCombination< FlagType > operator&
		( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret &= rhs;
	}

	template< typename FlagType >
	inline FlagCombination< FlagType > operator|
		( FlagCombination< FlagType > const & lhs
		, FlagType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret |= rhs;
	}

	template< typename FlagType >
	inline FlagCombination< FlagType > operator&
		( FlagCombination< FlagType > const & lhs
		, typename FlagCombination< FlagType >::BaseType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret &= rhs;
	}

	template< typename FlagType >
	inline FlagCombination< FlagType > operator|
		( FlagCombination< FlagType > const & lhs
		, typename FlagCombination< FlagType >::BaseType const & rhs )noexcept
	{
		FlagCombination< FlagType > ret{ lhs };
		return ret |= rhs;
	}
	/**\}*/
	/**
	*\~french
	*\brief
	*	Vérifie si l'indicateur donné est présent dans la combinaison donnée.
	*\param[in] value
	*	La combinaison.
	*\param[in] flag
	*	L'indicateur recherché.
	*\return
	*	\p true si \p value contient \p flag.
	*\~english
	*\brief
	*	Checks if the given flag is part of the given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag.
	*\return
	*	\p true if \p value contains \p flag.
	*/
	template< typename T, typename U >
	inline bool checkFlag( T const & value, U const & flag )noexcept
	{
		static_assert( sizeof( T ) == sizeof( U )
			, "Can't check flags for different size parameters" );
		return U( value & T( flag ) ) == flag;
	}
	/**
	*\~french
	*\brief
	*	Vérifie si l'indicateur donné est présent dans la combinaison donnée.
	*\param[in] value
	*	La combinaison.
	*\param[in] flag
	*	L'indicateur recherché.
	*\return
	*	\p true si \p value contient \p flag.
	*\~english
	*\brief
	*	Checks if the given flag is part of the given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag.
	*\return
	*	\p true if \p value contains \p flag.
	*/
	template< typename FlagType >
	inline bool checkFlag( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		using Type = typename FlagCombination< FlagType >::BaseType;
		return Type( value & flag ) == Type( flag );
	}
	/**
	*\~french
	*\brief
	*	Vérifie si l'indicateur donné est présent dans la combinaison donnée.
	*\param[in] value
	*	La combinaison.
	*\param[in] flag
	*	L'indicateur recherché.
	*\return
	*	\p true si value contient flag.
	*\~english
	*\brief
	*	Checks if the given flag is part of the given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag.
	*\return
	*	\p true if \p value contains \p flag.
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
	*\~french
	*\brief
	*	Ajoute un indicateur à la valeur donnée.
	*\param[in,out] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à ajouter.
	*\return
	*	La valeur.
	*\~english
	*\brief
	*	Adds a flag to given combination.
	*\param[in,out] value
	*	The combination.
	*\param[in] flag
	*	The flag to add.
	*\return
	*	The combination.
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
	*\~french
	*\brief
	*	Enlève un indicateur de la valeur donnée.
	*\param[in,out] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à enlever.
	*\return
	*	La valeur.
	*\~english
	*\brief
	*	Removes a flag from given combination.
	*\param[in,out] value
	*	The combination.
	*\param[in] flag
	*	The flag to remove.
	*\return
	*	The combination.
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
	*\~french
	*\brief
	*	Ajoute un indicateur à la valeur donnée.
	*\param[in] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à ajouter.
	*\return
	*	La nouvelle valeur.
	*\~english
	*\brief
	*	Adds a flag to given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag to add.
	*\return
	*	The new combination.
	*/
	template< typename FlagType >
	inline FlagCombination< FlagType > addFlag
		( FlagCombination< FlagType > const & value
		, FlagType const & flag )noexcept
	{
		return ( value | flag );
	}
	/**
	*\~french
	*\brief
	*	Enlève un indicateur de la valeur donnée.
	*\param[in] value
	*	La valeur.
	*\param[in] flag
	*	L'indicateur à enlever.
	*\return
	*	La nouvelle valeur.
	*\~english
	*\brief
	*	Removes a flag from given combination.
	*\param[in] value
	*	The combination.
	*\param[in] flag
	*	The flag to remove.
	*\return
	*	The new combination.
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

//!@~french		Implémente les opérateurs de combinaison binaire sur un scoped enum.
//!@~english	Implements binary combination operators on a scoped enum.
#define Ashes_ImplementFlag( FlagType )\
	using FlagType##s = ashes::FlagCombination< FlagType >;\
	inline FlagType##s operator|( FlagType lhs, FlagType rhs )\
	{\
		return FlagType##s( lhs ) | rhs;\
	}\
	inline FlagType##s operator|( FlagType##s::BaseType lhs, FlagType rhs )\
	{\
		return FlagType##s( lhs ) | rhs;\
	}\
	inline FlagType##s operator|( FlagType lhs, FlagType##s::BaseType rhs )\
	{\
		return FlagType##s( lhs ) | rhs;\
	}

#endif
