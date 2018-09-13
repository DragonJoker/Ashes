/*
See LICENSE file in root folder
*/
#ifndef ___Ashes_Log_H___
#define ___Ashes_Log_H___

#include <functional>
#include <string>

namespace ashes
{
	class Logger
	{
	public:
		using LogCallback = std::function< void( std::string const & msg, bool newLine ) >;

	public:
		/**
		*\~english
		*\brief
		*	Logs a debug message.
		*\~french
		*\brief
		*	Logge un message de débogage.
		*/
		static void logDebug( std::string const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs an info message.
		*\~french
		*\brief
		*	Logge un message d'informations.
		*/
		static void logInfo( std::string const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs a warning message.
		*\~french
		*\brief
		*	Logge un message de warning.
		*/
		static void logWarning( std::string const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs an error message.
		*\~french
		*\brief
		*	Logge un message d'erreur.
		*/
		static void logError( std::string const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs a debug message.
		*\~french
		*\brief
		*	Logge un message de débogage.
		*/
		static void logDebug( std::ostream const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs an info message.
		*\~french
		*\brief
		*	Logge un message d'informations.
		*/
		static void logInfo( std::ostream const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs a warning message.
		*\~french
		*\brief
		*	Logge un message de warning.
		*/
		static void logWarning( std::ostream const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Logs an error message.
		*\~french
		*\brief
		*	Logge un message d'erreur.
		*/
		static void logError( std::ostream const & message, bool newLine = true );
		/**
		*\~english
		*\brief
		*	Sets the debug callback.
		*\~french
		*\brief
		*	Définit le callback de débogage.
		*/
		static void setDebugCallback( LogCallback callback );
		/**
		*\~english
		*\brief
		*	Sets the info callback.
		*\~french
		*\brief
		*	Définit le callback d'information.
		*/
		static void setInfoCallback( LogCallback callback );
		/**
		*\~english
		*\brief
		*	Sets the warning callback.
		*\~french
		*\brief
		*	Définit le callback de warning.
		*/
		static void setWarningCallback( LogCallback callback );
		/**
		*\~english
		*\brief
		*	Sets the error callback.
		*\~french
		*\brief
		*	Définit le callback d'erreur
		*/
		static void setErrorCallback( LogCallback callback );

	private:
		Logger();

		static Logger & doGetInstance();

	private:
		LogCallback m_debug;
		LogCallback m_info;
		LogCallback m_warning;
		LogCallback m_error;
	};
}

#endif
