#include "ashespp/Utils/CallStack.hpp"

#if !defined( NDEBUG )
#	if ASHES_WIN32
#		ifndef NOMINMAX
#			define NOMINMAX
#		endif
#		include <Windows.h>
#		pragma warning( push )
#		pragma warning( disable:4091 ) //'typedef ': ignored on left of '' when no variable is declared
#		include <Dbghelp.h>
#		pragma warning( pop )
#	elif ASHES_ANDROID
#	else
#		include <execinfo.h>
#		include <cxxabi.h>
#	endif
#endif

#include <iostream>
#include <locale>
#include <mutex>

namespace ashes
{
	namespace callstack
	{
#if defined( NDEBUG )

		inline void doShowBacktrace( std::ostream &
			, int
			, int )
		{
		}

#else

#	if ASHES_WIN32

		namespace
		{
			bool & doGetInitialisationStatus()
			{
				static bool result = false;
				return result;
			}

			::HANDLE doGetProcess()
			{
				static ::HANDLE result( ::GetCurrentProcess() );
				return result;
			}

			inline std::string doDemangle( std::string const & name )
			{
				std::string result;

				try
				{
					char real[2048] = { 0 };

					if ( ::UnDecorateSymbolName( name.c_str()
						, real
						, sizeof( real )
						, UNDNAME_COMPLETE ) )
					{
						result = real;
					}
				}
				catch ( ... )
				{
					// What to do...
				}

				return result;
			}
		}

		inline void doShowBacktrace( std::ostream & stream
			, int toCapture
			, int toSkip )
		{
			static std::mutex mutex;
			std::unique_lock< std::mutex > lock{ mutex };
			const int MaxFnNameLen( 255 );

			std::vector< void * > backTrace( toCapture - toSkip );
			unsigned int num( ::RtlCaptureStackBackTrace( toSkip
				, toCapture - toSkip
				, backTrace.data()
				, nullptr ) );

			stream << "CALL STACK:" << std::endl;

			// symbol->Name type is char [1] so there is space for \0 already
			auto symbol( ( SYMBOL_INFO * )malloc( sizeof( SYMBOL_INFO ) + ( MaxFnNameLen * sizeof( char ) ) ) );

			if ( symbol )
			{
				symbol->MaxNameLen = MaxFnNameLen;
				symbol->SizeOfStruct = sizeof( SYMBOL_INFO );
				for ( unsigned int i = 0; i < num; ++i )
				{
					if ( ::SymFromAddr( doGetProcess()
						, reinterpret_cast< DWORD64 >( backTrace[i] )
						, nullptr
						, symbol ) )
					{
						stream << "== " << doDemangle( { symbol->Name, symbol->Name + symbol->NameLen } );
						IMAGEHLP_LINE64 line;
						DWORD displacement;
						line.SizeOfStruct = sizeof( IMAGEHLP_LINE64 );

						if ( ::SymGetLineFromAddr64( doGetProcess()
							, symbol->Address
							, &displacement
							, &line ) )
						{
							stream << "(" << line.FileName << ":" << line.LineNumber << ")";
						}

						stream << std::endl;
					}
					else
					{
						stream << "== Symbol not found." << std::endl;
					}
				}

				free( symbol );
			}
		}

		void initialise()
		{
			::SymSetOptions( SYMOPT_UNDNAME | SYMOPT_LOAD_LINES );
			doGetInitialisationStatus() = ( TRUE == ::SymInitialize( doGetProcess()
				, nullptr
				, TRUE ) );

			if ( !doGetInitialisationStatus() )
			{
				std::cerr << "Could not initialise DbgHelp: 0x" << std::hex << ::GetLastError() << std::endl;
			}
		}

		void cleanup()
		{
			if ( doGetInitialisationStatus() )
			{
				::SymCleanup( doGetProcess() );
			}
		}

#	elif ASHES_ANDROID

		inline void doShowBacktrace( std::ostream & stream
			, int
			, int )
		{
		}

		void initialise()
		{
		}

		void cleanup()
		{
		}

#	else

		inline std::string doDemangle( std::string const & name )
		{
			std::string result{ name };
			size_t lindex = result.find( "(" );
			size_t rindex = result.find( "+" );

			if ( lindex != std::string::npos && rindex != std::string::npos )
			{
				result = name.substr( lindex + 1, rindex - 1 - lindex );
			}

			int status;
			auto real = abi::__cxa_demangle( result.c_str(), 0, 0, &status );

			if ( !status )
			{
				result = name.substr( 0, lindex + 1 ) + real + name.substr( rindex );
			}
			else
			{
				result = name;
			}

			free( real );
			return result;
		}

		inline void doShowBacktrace( std::ostream & stream
			, int toCapture
			, int toSkip )
		{
			stream << "CALL STACK:" << std::endl;
			std::vector< void * > backTrace( toCapture );
			unsigned int num( ::backtrace( backTrace.data(), toCapture ) );
			char ** fnStrings( ::backtrace_symbols( backTrace.data(), num ) );

			for ( unsigned i = toSkip; i < num; ++i )
			{
				stream << "== " << doDemangle( fnStrings[i] ) << std::endl;
			}

			free( fnStrings );
		}


		void initialise()
		{
		}

		void cleanup()
		{
		}

#	endif

#endif

		std::ostream & operator<<( std::ostream & stream
			, Backtrace const & trace )
		{
			static std::locale const loc{ "C" };
			stream.imbue( loc );
			doShowBacktrace( stream
				, trace.m_toCapture
				, trace.m_toSkip );
			return stream;
		}
	}
}
