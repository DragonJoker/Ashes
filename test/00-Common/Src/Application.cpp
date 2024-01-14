#include "Application.hpp"

#include "FileUtils.hpp"
#include "MainFrame.hpp"

#include <util/GlslToSpv.hpp>

#include <ashes/ashes.h>

#include <wx/cmdline.h>

#include <fstream>

namespace common
{
	namespace
	{
		template< typename LogStreambufTraits >
		class LogStreambuf
			: public std::streambuf
		{
		public:
			using CharType = char;
			using string_type = std::basic_string< CharType >;
			using ostream_type = std::basic_ostream< CharType >;
			using streambuf_type = std::basic_streambuf< CharType >;
			using int_type = typename std::basic_streambuf< CharType >::int_type;
			using traits_type = typename std::basic_streambuf< CharType >::traits_type;

			LogStreambuf( LogStreambuf const & ) = delete;
			LogStreambuf & operator=( LogStreambuf const & ) = delete;
			LogStreambuf( LogStreambuf && )noexcept = delete;
			LogStreambuf & operator=( LogStreambuf && )noexcept = delete;

			LogStreambuf( string_type const & appName, ostream_type & stream )
				: m_stream( stream )
				, m_appName{ appName }
			{
				m_old = m_stream.rdbuf( this );
			}

			~LogStreambuf()noexcept override
			{
				try
				{
					m_stream.rdbuf( m_old );
				}
				catch ( ... )
				{
					// Nothing to do here
				}
			}

		private:
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
					doSyncNoNl();
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
					LogStreambufTraits::Log( m_appName, m_buffer );
					m_buffer.clear();
				}

				return 0;
			}

			int doSyncNoNl()
			{
				if ( !m_buffer.empty() )
				{
					LogStreambufTraits::Log( m_appName, m_buffer );
					m_buffer.clear();
				}

				return 0;
			}

		private:
			string_type m_buffer;
			ostream_type & m_stream;
			streambuf_type * m_old;
			string_type m_appName;
		};

		enum class LogType
		{
			eDebug,
			eInfo,
			eWarning,
			eError,
		};

		std::string logFileName = "Debug.log";

		void logDebugString( std::string const & log
			, FILE * stdStream )
		{
#if _MSC_VER
			::OutputDebugStringA( log.c_str() );
#endif
			std::tm today = { 0 };
			time_t ttime;
			time( &ttime );
#if _MSC_VER
			localtime_s( &today, &ttime );
#else
			today = *localtime( &ttime );
#endif
			std::array< char, 33u > buffer{};
			strftime( buffer.data(), 32, "%Y-%m-%d %H:%M:%S", &today );
			std::string timeStamp{ buffer.data() };

			if ( std::ofstream file{ logFileName, std::ios::app } )
			{
				file << timeStamp << " - " << log;
			}

			fprintf( stdStream, "%s", log.c_str() );
		}

		void flushLogFile()
		{
			if ( wxFileExists( logFileName ) )
			{
				wxRemoveFile( logFileName );
			}
		}

		template< LogType Type >
		struct TLogStreambufTraits
		{
			using CharType = char;
			using string_type = std::basic_string< CharType >;

			static inline std::array< std::string, 4u > const logTypeName
			{
				"Debug",
				"Info",
				"Warning",
				"Error",
			};
			static inline std::array< FILE *, 4u > const stdStream
			{
				stdout,
				stdout,
				stdout,
				stderr,
			};

			static void Log( string_type const & appName
				, string_type const & text )
			{
				logDebugString( appName + " - " + logTypeName[size_t( Type )] + ": " + text + "\n"
					, stdStream[size_t( Type )] );
			}
		};

		using DebugLogStreambufTraits = TLogStreambufTraits< LogType::eDebug >;
		using InfoLogStreambufTraits = TLogStreambufTraits< LogType::eInfo >;
		using WarningLogStreambufTraits = TLogStreambufTraits< LogType::eWarning >;
		using ErrorLogStreambufTraits = TLogStreambufTraits< LogType::eError >;
	}

	App::App ( wxString const & name )
		: m_name{ name }
	{
	}

	bool App::OnInit()
	{
		m_cout = std::make_unique< LogStreambuf< InfoLogStreambufTraits > >( m_name.ToStdString()
			, std::cout );
		m_cerr = std::make_unique < LogStreambuf< ErrorLogStreambufTraits > >( m_name.ToStdString()
			, std::cerr );
		m_clog = std::make_unique < LogStreambuf< DebugLogStreambufTraits > >( m_name.ToStdString()
			, std::clog );

#if !defined( NDEBUG )
#	if defined( _WIN32 )

		if ( ::AllocConsole() )
		{
			m_allocated = true;
			FILE * dump;
			freopen_s( &dump, "conout$", "w", stdout );
			freopen_s( &dump, "conout$", "w", stderr );
		}
		else if ( ::AttachConsole( ATTACH_PARENT_PROCESS ) )
		{
			FILE * dump;
			freopen_s( &dump, "conout$", "w", stdout );
			freopen_s( &dump, "conout$", "w", stderr );
		}
		else
		{
			DWORD lastError = ::GetLastError();

			if ( lastError == ERROR_ACCESS_DENIED )
			{
				FILE * dump;
				freopen_s( &dump, "conout$", "w", stdout );
				freopen_s( &dump, "conout$", "w", stderr );
			}
		}

#	endif
#endif

		wxInitAllImageHandlers();
		utils::initialiseGlslang();
		bool result = false;

		try
		{
			if ( doParseCommandLine() )
			{
				m_mainFrame = doCreateMainFrame( m_rendererName );
				m_mainFrame->initialise();
				SetTopWindow( m_mainFrame );
				result = true;
			}
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}

		return result;
	}

	int App::OnExit()
	{
		utils::cleanupGlslang();
		wxImage::CleanUpHandlers();

#if !defined( NDEBUG )
#	if defined( _WIN32 )

		if ( m_allocated )
		{
			::FreeConsole();
		}

#	endif
#endif

		m_cout.reset();
		m_cerr.reset();
		m_clog.reset();
		return wxApp::OnExit();
	}

	void App::updateFps( std::chrono::microseconds const & durationGpu
		, std::chrono::microseconds const & durationCpu )
	{
		m_mainFrame->updateFps( durationGpu, durationCpu );
	}

	void App::updateFps( std::chrono::microseconds const & duration )
	{
		m_mainFrame->updateFps( duration );
	}

	bool App::doParseCommandLine()
	{
		wxCmdLineParser parser( wxApp::argc, wxApp::argv );
		parser.AddSwitch( wxT( "h" ), wxT( "help" ), _( "Displays this help" ) );
		parser.AddOption( wxT( "l" ), wxT( "log" ), _( "Specifies the log file" ) );

		for ( auto & plugin : m_renderers )
		{
			parser.AddSwitch( plugin.name, wxEmptyString, _( "Defines the renderer to " ) + plugin.description );
		}

		bool result = parser.Parse( false ) == 0;

		// S'il y avait des erreurs ou "-h" ou "--help", on affiche l'aide et on sort
		if ( !result || parser.Found( wxT( 'h' ) ) )
		{
			parser.Usage();
			result = false;
		}

		if ( result )
		{
			if ( wxString fileName;
				parser.Found( wxT( "l" ), &fileName ) )
			{
				logFileName = fileName;
			}

			flushLogFile();
			AshPluginDescription desc;
			ashGetCurrentPluginDescription( &desc );
			m_rendererName = desc.name;

			for ( auto & plugin : m_renderers )
			{
				if ( m_rendererName == desc.name
					&& parser.Found( plugin.name ) )
				{
					m_rendererName = plugin.name;
				}
			}
		}

		return result;
	}
};
