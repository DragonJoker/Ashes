#include "common/DynamicLibrary.hpp"

#include "common/Exception.hpp"

#if defined( _WIN32 )

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 5262 )
#include <iomanip>
#include <sstream>
#pragma warning( pop )
#include <stdexcept>

#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <Windows.h>

namespace ashes
{
	DynamicLibrary::DynamicLibrary( DynamicLibrary && rhs )noexcept
		: m_path{ std::move( rhs.m_path ) }
		, m_library{ rhs.m_library }
	{
		rhs.m_library = nullptr;
	}

	DynamicLibrary & DynamicLibrary::operator=( DynamicLibrary && rhs )noexcept
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

	void DynamicLibrary::doOpen()
	{
		m_library = ::LoadLibraryExA( m_path.c_str()
			, nullptr
			, 0u );

		if ( !m_library )
		{
			std::stringstream stream;
			stream << "Couldn't load dynamic library at [" << m_path << "]: ";
			stream << "Error code: " << std::hex << ::GetLastError();
			throw ashes::BaseException{ stream.str() };
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
		UINT oldMode = ::SetErrorMode( SEM_FAILCRITICALERRORS );
		( void )::GetLastError();
		auto result = ( void * )::GetProcAddress( static_cast< HMODULE >( m_library ), name.c_str() );
		auto error = ::GetLastError();
		::SetErrorMode( oldMode );

		if ( error != ERROR_SUCCESS && !result )
		{
			std::stringstream stream;
			stream << "Couldn't load function [" + name + "]: ";
			stream << "Error code: 0x" << std::hex << std::setw( 8u ) << std::setfill( '0' ) << error;
		}

		return result;
	}
}

#endif
