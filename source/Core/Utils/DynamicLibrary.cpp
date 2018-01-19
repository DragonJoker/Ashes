#include "DynamicLibrary.hpp"

#include <sstream>
#include <iostream>

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
		m_library = ::LoadLibraryA( m_path.c_str() );

		if ( !m_library )
		{
			std::stringstream stream;
			stream << "Couldn't load dynamic library at [" << m_path << "]: ";
			stream << "Error code: " << std::hex << ::GetLastError();
			std::cerr << stream.str() << std::endl;
			throw std::runtime_error{ stream.str() };
		}
	}

	void DynamicLibrary::doClose()noexcept
	{
		UINT oldMode = ::SetErrorMode( SEM_FAILCRITICALERRORS );

		try
		{
			::FreeLibrary( static_cast< HMODULE >( m_library ) );
		}
		catch ( ... )
		{
			std::cerr << "Couldn't unload dynamic library" << std::endl;
		}

		::SetErrorMode( oldMode );
		m_library = nullptr;
	}

	void * DynamicLibrary::doGetFunction( std::string const & name )noexcept
	{
		UINT oldMode = ::SetErrorMode( SEM_FAILCRITICALERRORS );
		::GetLastError();
		void * result = result = ::GetProcAddress( static_cast< HMODULE >( m_library ), name.c_str() );
		auto error = ::GetLastError();
		::SetErrorMode( oldMode );

		if ( error != ERROR_SUCCESS && !result )
		{
			std::stringstream stream;
			stream << "Couldn't load function [" + name + "]: ";
			stream << "Error code: " << std::hex << error;
			std::cerr << stream.str() << std::endl;
		}

		return result;
	}

#elif defined( __linux__ )

	void DynamicLibrary::doOpen()
	{
		m_library = dlopen( m_path.c_str(), RTLD_LAZY );

		if ( !m_library )
		{
			std::stringstream stream;
			stream << "Couldn't load dynamic library at [" << m_path << "]: ";
			stream << "Error code: " << std::hex << dlerror();
			std::cerr << stream.str() << std::endl;
			throw std::runtime_error{ error.str() };
		}
	}

	void DynamicLibrary::doClose()noexcept
	{
		try
		{
			dlclose( m_library );
		}
		catch ( ... )
		{
			std::cerr << "Couldn't unload dynamic library" << std::endl;
		}
	}

	void * DynamicLibrary::doGetFunction( std::string const & name )noexcept
	{
		dlerror();
		void * result = dlsym( m_library, name.c_str() );
		auto error = dlerror();

		if ( error && !result )
		{
			std::stringstream stream;
			stream << "Couldn't load function [" + name + "]: ";
			stream << "Error code: " << std::hex << std::string( error );
			std::cerr << stream.str() << std::endl;
		}

		return result;
	}

#endif
}
