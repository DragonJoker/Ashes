/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_Callstack_HPP___
#define ___AshesPP_Callstack_HPP___

#include "AshesPP/AshesPPPrerequisites.hpp"

namespace ashespp
{
	namespace callstack
	{
		struct Backtrace
		{
			int const m_toCapture;
			int const m_toSkip;

			inline Backtrace( int p_toCapture = 20, int p_toSkip = 2 )
				: m_toCapture{ p_toCapture }
				, m_toSkip{ p_toSkip }
			{
			}
		};
		/**
		 *\~english
		 *\brief		Initialises debug data.
		 *\~french
		 *\brief		Initialise les données de débogage.
		 */
		void initialise();
		/**
		 *\~english
		 *\brief		Cleans up debug data.
		 *\~french
		 *\brief		Nettoie les données de débogage.
		 */
		void cleanup();
		/**
		 *\~english
		 *\brief			Puts the backtrace into a stream
		 *\param[in,out]	stream	The stream
		 *\~french
		 *\brief			Transmet la pile d'appels dans un flux
		 *\param[in,out]	stream	Le flux
		 */
		std::ostream & operator<<( std::ostream & stream, Backtrace const & trace );
		/*!
		\author 	Sylvain DOREMUS
		\date		05/10/2015
		\version	0.8.0
		\~english
		\brief		Helper class used to enable allocation backtrace retrieval.
		\~french
		\brief		Classe d'aide permettant la récupération de la pile d'appels d'allocation.
		*/
		class Backtraced
		{
#if !defined( NDEBUG )

		protected:
			Backtraced()
				: m_callStack{ doGetCallStack() }
			{
			}

			~Backtraced()
			{
			}

		private:
			static std::string doGetCallStack()
			{
				std::stringstream callStack;
				callStack << Backtrace{};
				return callStack.str();
			}

		protected:
			std::string m_callStack;
			friend std::ostream & operator<<( std::ostream &, Backtraced const & );

#endif
		};

		inline std::ostream & operator<<( std::ostream & stream
			, Backtraced const & traced )
		{
#if !defined( NDEBUG )

			stream << traced.m_callStack;

#endif

			return stream;
		}
	}
}

#endif
