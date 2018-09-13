/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "AshesPrerequisites.hpp"

#include <exception>

namespace ashes
{
	/**
	*\~english
	*\brief
	*	API exception class.
	*\~french
	*\brief
	*	Classe d'exception de l'API.
	*/
	class Exception
		: public std::exception
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] result
		*	Le résultat de l'action.
		*\param[in] text
		*	Le texte de l'exception.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] result
		*	The action result.
		*\param[in] text
		*	The exception text.
		*/
		Exception( Result result
			, std::string text )
			: m_result{ result }
			, m_text{ std::move( text ) }
		{
			m_text += " - " + getName( result );
		}
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline Result getResult()const
		{
			return m_result;
		}

		inline char const * what()const noexcept override
		{
			return m_text.c_str();
		}
		/**@}*/

	protected:
		Result m_result;
		std::string m_text;
	};

#define tryAshesFunc( Func, ... )\
	auto result = Result::eSuccess;\
	try\
	{\
		Func( __VA_ARGS__ );\
	}\
	catch ( Exception & exc )\
	{\
		ashes::Logger::logError( exc.what() );\
		result = exc.getResult(); \
	}\
	return result;
}
