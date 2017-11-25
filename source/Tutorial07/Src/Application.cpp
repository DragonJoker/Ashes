#include "Application.hpp"

#include "MainFrame.hpp"

wxIMPLEMENT_APP( vkapp::App );

namespace vkapp
{
	namespace
	{
		/**
		*\brief
		*	Surcharge de std::streambuf, permettant de personnaliser la sortie.
		*\param LogStreambufTraits
		*	Permet la personnalisation de la sortie.\n
		*	Doit implémenter la fonction statique suivante :\n
		*	Log( string_type appName, string_type buffer )
		*/
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
			/**
			*\brief
			*	Constructeur, échange le tampon du flux donné avec celui-ci.
			*\param[in] appName
			*	Le nom de l'application.
			*\param[in,out] stream
			*	Le flux recevant ce tampon de flux.
			*/
			LogStreambuf( string_type const & appName, ostream_type & stream )
				: m_stream( stream )
				, m_appName{ appName }
			{
				m_old = m_stream.rdbuf( this );
			}
			/**
			*\brief
			*	Destructeur, rétablit le tampon d'origine du flux.
			*/
			~LogStreambuf()
			{
				m_stream.rdbuf( m_old );
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

		std::string const LogFileName = "Debug.log";

		void logDebugString( std::string const & log )
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
			char buffer[33] = { 0 };
			strftime( buffer, 32, "%Y-%m-%d %H:%M:%S", &today );
			std::string timeStamp = buffer;

			std::ofstream file{ LogFileName, std::ios::app };

			if ( file )
			{
				file << timeStamp << " - " << log;
			}
		}

		void flushLogFile()
		{
			if ( wxFileExists( LogFileName ) )
			{
				wxRemoveFile( LogFileName );
			}
		}
	}

	bool App::OnInit()
	{
		flushLogFile();
		m_cout = new LogStreambuf< InfoLogStreambufTraits >( "Tutorial 07"
			, std::cout );
		m_cerr = new LogStreambuf< ErrorLogStreambufTraits >( "Tutorial 07"
			, std::cerr );
		m_clog = new LogStreambuf< DebugLogStreambufTraits >( "Tutorial 07"
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
		bool result = false;

		try
		{
			m_mainFrame = new MainFrame();
			SetTopWindow( m_mainFrame );
			result = true;
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}

		return result;
	}

	int App::OnExit()
	{
		wxImage::CleanUpHandlers();
#if !defined( NDEBUG )
#	if defined( _WIN32 )
		if ( m_allocated )
		{
			::FreeConsole();
		}
#	endif
#endif
		delete m_cout;
		delete m_cerr;
		delete m_clog;
		return wxApp::OnExit();
	}

	void App::updateFps( std::chrono::microseconds const & duration )
	{
		m_mainFrame->updateFps( duration );
	}
};
