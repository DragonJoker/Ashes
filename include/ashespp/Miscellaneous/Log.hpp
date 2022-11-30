/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_Log_H___
#define ___AshesPP_Log_H___

#include <functional>
#include <memory>
#include <string>

#pragma warning( push )
#pragma warning( disable:5262 )
#include <sstream>
#pragma warning( pop )

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

	namespace log
	{
		template< typename StreambufT >
		struct LogStreamT
			: public std::ostringstream
		{
			using StreambufPtrT = std::unique_ptr< StreambufT >;

		public:
			LogStreamT()
				: std::ostringstream{}
				, m_streambuf{ std::make_unique< StreambufT >( static_cast< std::ostream & >( *this ) ) }
			{
				this->imbue( std::locale{ "C" } );
			}

		private:
			StreambufPtrT m_streambuf;
		};

		template< typename TraitsT >
		class LogStreambufT
			: public std::streambuf
		{
		public:
			using string_type = std::string;
			using ostream_type = std::ostream;
			using streambuf_type = std::streambuf;
			using int_type = std::streambuf::int_type;
			using traits_type = std::streambuf::traits_type;

			explicit LogStreambufT( ostream_type & stream )
				: m_stream( stream )
			{
				m_old = m_stream.rdbuf( this );
			}

			~LogStreambufT()noexcept override
			{
				doSync();
				m_stream.rdbuf( m_old );
			}

			int_type overflow( int_type c = traits_type::eof() )override
			{
				if ( traits_type::eq_int_type( c, traits_type::eof() ) )
				{
					doSync();
				}
				else if ( c == '\n' )
				{
					doSync();
				}
				else if ( c == '\r' )
				{
					m_buffer += '\r';
					doSyncNoLF();
				}
				else
				{
					m_buffer += traits_type::to_char_type( c );
				}

				return c;
			}

			int doSync()
			{
				if ( !m_buffer.empty() )
				{
					TraitsT::log( m_buffer, true );
					m_buffer.clear();
				}

				return 0;
			}

			int doSyncNoLF()
			{
				if ( !m_buffer.empty() )
				{
					TraitsT::log( m_buffer, false );
					m_buffer.clear();
				}

				return 0;
			}

		private:
			ostream_type & m_stream;
			streambuf_type * m_old;
			string_type m_buffer;
		};
		/**
		*\~english
		*\brief
		*	Streambuf traits for trace logging.
		*/
		struct TraceLoggerStreambufTraits
		{
			static void log( std::string const & msg, bool lf )
			{
				Logger::logTrace( msg, lf );
			}
		};
		/**
		*\~english
		*\brief
		*	Streambuf traits for debug logging.
		*/
		struct DebugLoggerStreambufTraits
		{
			static void log( std::string const & msg, bool lf )
			{
				Logger::logDebug( msg, lf );
			}
		};
		/**
		*\~english
		*\brief
		*	Streambuf traits for info logging.
		*/
		struct InfoLoggerStreambufTraits
		{
			static void log( std::string const & msg, bool lf )
			{
				Logger::logInfo( msg, lf );
			}
		};
		/**
		*\~english
		*\brief
		*	Streambuf traits for warning logging.
		*/
		struct WarningLoggerStreambufTraits
		{
			static void log( std::string const & msg, bool lf )
			{
				Logger::logWarning( msg, lf );
			}
		};
		/**
		*\~english
		*\brief
		*	Streambuf traits for error logging.
		*/
		struct ErrorLoggerStreambufTraits
		{
			static void log( std::string const & msg, bool lf )
			{
				Logger::logError( msg, lf );
			}
		};

		using TraceLoggerStreambuf = LogStreambufT< TraceLoggerStreambufTraits >;
		using DebugLoggerStreambuf = LogStreambufT< DebugLoggerStreambufTraits >;
		using InfoLoggerStreambuf = LogStreambufT< InfoLoggerStreambufTraits >;
		using WarningLoggerStreambuf = LogStreambufT< WarningLoggerStreambufTraits >;
		using ErrorLoggerStreambuf = LogStreambufT< ErrorLoggerStreambufTraits >;

		static LogStreamT< TraceLoggerStreambuf > trace;
		static LogStreamT< DebugLoggerStreambuf > debug;
		static LogStreamT< InfoLoggerStreambuf > info;
		static LogStreamT< WarningLoggerStreambuf > warn;
		static LogStreamT< ErrorLoggerStreambuf > error;
	}
}

#endif
