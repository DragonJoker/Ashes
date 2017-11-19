/**
*\file
*	Log.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Log_HPP___
#define ___DesktopUtils_Log_HPP___
#pragma once

#include <string>
#include <ostream>

#include <RenderLib/LogUtils.h>

namespace utils
{
	enum class LogType
	{
		eDebug,
		eInfo,
		eWarning,
		eError,
	};
	/**
	*\brief
	*	Affiche la ligne donnée dans la console de débogage.
	*\param[in] log
	*	La ligne à logger.
	*/
	void logDebugString( std::string const & log );
	/**
	*\brief
	*	Classe trait, pour afficher une ligne dans le logcat.
	*/
	template< LogType Type >
	struct TLogStreambufTraits
	{
		using CharType = char;
		using string_type = std::basic_string< CharType >;

		/**
		*\brief
		*	Affiche la ligne donnée dans le logcat.
		*\param[in] appName
		*	Le nom de l'application.
		*\param[in] text
		*	La ligne à logger.
		*/
		static void Log( string_type const & appName
			, string_type const & text )
		{
			static std::string const LogName[]
			{
				"Debug",
				"Info",
				"Warning",
				"Error",
			};
			auto log = appName + " - " + LogName[size_t( Type )] + ": " + text + "\n";
			printf( "%s", log.c_str() );
			logDebugString( log );
		}
	};

	//! Spécialisation de TLogStreambufTraits pour les logs de débogage.
	using DebugLogStreambufTraits = TLogStreambufTraits< LogType::eDebug >;
	//! Spécialisation de TLogStreambufTraits pour les logs informatifs.
	using InfoLogStreambufTraits = TLogStreambufTraits< LogType::eInfo >;
	//! Spécialisation de TLogStreambufTraits pour les logs de warning.
	using WarningLogStreambufTraits = TLogStreambufTraits< LogType::eWarning >;
	//! Spécialisation de TLogStreambufTraits pour les logs d'erreur.
	using ErrorLogStreambufTraits = TLogStreambufTraits< LogType::eError >;
}

#endif
