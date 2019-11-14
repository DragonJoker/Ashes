/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_Callstack_HPP___
#define ___AshesPP_Callstack_HPP___

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
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
		*\brief
		*	Initialises debug data.
		*/
		void initialise();
		/**
		*\brief
		*	Cleans up debug data.
		*/
		void cleanup();
		/**
		*\brief
		*	Puts the backtrace into a stream.
		*\param[in,out] stream
		*	The stream.
		*/
		std::ostream & operator<<( std::ostream & stream, Backtrace const & trace );
		/**
		*\brief
		*	Helper class used to enable allocation backtrace retrieval.
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
