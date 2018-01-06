#include "DynamicLibrary.hpp"

#include <sstream>

#if defined( _WIN32 )
#	include <Windows.h>
#elif defined( __linux__ )
#	include <dlfcn.h>
#endif

namespace utils
{
	DynamicLibrary::DynamicLibrary( DynamicLibrary && rhs )
		: m_path{ std::move( rhs.m_path ) }
		, m_library{ rhs.m_library }
	{
		rhs.m_library = nullptr;
	}

	DynamicLibrary & DynamicLibrary::operator=( DynamicLibrary && rhs )
	{
		if ( this != &rhs )
		{
			m_path = std::move( rhs.m_path );
			m_library = rhs.m_library;
			rhs.m_library = nullptr;
		}

		return *this;
	}

	DynamicLibrary::DynamicLibrary( std::string const & path )
		: m_path{ path }
	{
		doOpen();
	}

	DynamicLibrary::~DynamicLibrary()noexcept
	{
		if ( m_library )
		{
			doClose();
		}
	}

#if defined( _WIN32 )

	void DynamicLibrary::doOpen()
	{
		try
		{
			m_library = ::LoadLibraryA( m_path.c_str() );

			if ( !m_library )
			{
				std::stringstream error;
				error << "Error code: " << std::hex << ::GetLastError();
				throw std::runtime_error{ error.str() };
			}
		}
		catch ( std::exception & exc )
		{
			std::stringstream error;
			error << "Can't load dynamic library at [" << m_path << "]: ";
			error << exc.what();
			m_library = nullptr;
			m_path.clear();
			throw std::runtime_error{ error.str() };
		}
		catch ( ... )
		{
			std::stringstream error;
			error << "Can't load dynamic library at [" << m_path << "]: ";
			error << "Error code: " << std::hex << ::GetLastError();
			m_library = nullptr;
			m_path.clear();
			throw std::runtime_error{ error.str() };
		}
	}

	void DynamicLibrary::doClose()noexcept
	{
		UINT oldMode = ::SetErrorMode( SEM_FAILCRITICALERRORS );
		::FreeLibrary( static_cast< HMODULE >( m_library ) );
		::SetErrorMode( oldMode );
		m_library = nullptr;
	}

	void * DynamicLibrary::doGetFunction( std::string const & name )noexcept
	{
		void * result = nullptr;
		UINT oldMode = ::SetErrorMode( SEM_FAILCRITICALERRORS );
		result = ::GetProcAddress( static_cast< HMODULE >( m_library ), name.c_str() );
		::SetErrorMode( oldMode );
		return result;
	}

#elif defined( __linux__ )

	void DynamicLibrary::doOpen()
	{
		try
		{
			m_library = dlopen( m_path.c_str(), RTLD_LAZY );

			if ( !m_library )
			{
				std::stringstream error;
				error << "Error code: " << std::hex << dlerror();
				throw std::runtime_error{ error.str() };
			}
		}
		catch ( std::exception & exc )
		{
			std::stringstream error;
			error << "Can't load dynamic library at [" << m_path << "]: ";
			error << exc.what();
			m_library = nullptr;
			m_path.clear();
			throw std::runtime_error{ error.str() };
		}
		catch ( ... )
		{
			std::stringstream error;
			error << "Can't load dynamic library at [" << m_path << "]: ";
			error << "Error code: " << std::hex << dlerror();
			m_library = nullptr;
			m_path.clear();
			throw std::runtime_error{ error.str() };
		}
	}

	void DynamicLibrary::doClose()throw( )
	{
		try
		{
			dlclose( m_library );
		}
		catch ( ... )
		{
			std::cerr << "Can't unload dynamic library" << std::endl;
		}
	}

	void * DynamicLibrary::doGetFunction( std::string const & name )noexcept
	{
		void * result = nullptr;

		try
		{
			dlerror();
			result = dlsym( m_library, name.c_str() );
			auto error = dlerror();

			if ( error != NULL )
			{
				throw std::runtime_error{ std::string( error ) };
			}
		}
		catch ( std::exception & exc )
		{
			result = nullptr;
			std::cerr << "Can't load function [" + name + "]: " + exc.what() << std::endl;
		}
		catch ( ... )
		{
			result = nullptr;
			std::cerr << "Can't load function [" + name + "]: Unknown error." << std::endl;
		}

		return result;
	}

#endif
}
