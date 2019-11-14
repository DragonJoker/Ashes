/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_Log_H___
#define ___AshesPP_Log_H___

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
		*\brief
		*	Logs a trace message.
		*/
		static void logTrace( std::string const & message, bool newLine = true );
		/**
		*\brief
		*	Logs a debug message.
		*/
		static void logDebug( std::string const & message, bool newLine = true );
		/**
		*\brief
		*	Logs an info message.
		*/
		static void logInfo( std::string const & message, bool newLine = true );
		/**
		*\brief
		*	Logs a warning message.
		*/
		static void logWarning( std::string const & message, bool newLine = true );
		/**
		*\brief
		*	Logs an error message.
		*/
		static void logError( std::string const & message, bool newLine = true );
		/**
		*\brief
		*	Logs a trace message.
		*/
		static void logTrace( std::ostream const & message, bool newLine = true );
		/**
		*\brief
		*	Logs a debug message.
		*/
		static void logDebug( std::ostream const & message, bool newLine = true );
		/**
		*\brief
		*	Logs an info message.
		*/
		static void logInfo( std::ostream const & message, bool newLine = true );
		/**
		*\brief
		*	Logs a warning message.
		*/
		static void logWarning( std::ostream const & message, bool newLine = true );
		/**
		*\brief
		*	Logs an error message.
		*/
		static void logError( std::ostream const & message, bool newLine = true );
		/**
		*\brief
		*	Sets the trace callback.
		*/
		static void setTraceCallback( LogCallback callback );
		/**
		*\brief
		*	Sets the debug callback.
		*/
		static void setDebugCallback( LogCallback callback );
		/**
		*\brief
		*	Sets the info callback.
		*/
		static void setInfoCallback( LogCallback callback );
		/**
		*\brief
		*	Sets the warning callback.
		*/
		static void setWarningCallback( LogCallback callback );
		/**
		*\brief
		*	Sets the error callback.
		*/
		static void setErrorCallback( LogCallback callback );

	private:
		Logger();

		static Logger & doGetInstance();

	private:
		LogCallback m_trace;
		LogCallback m_debug;
		LogCallback m_info;
		LogCallback m_warning;
		LogCallback m_error;
	};
}

#endif
