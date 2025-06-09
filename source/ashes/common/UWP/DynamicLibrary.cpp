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
#pragma comment(lib, "WindowsApp")

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
		std::wstring widePath{ m_path.begin(), m_path.end() };
		m_library = ::LoadPackagedLibrary( widePath.c_str(), 0u );

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
		::FreeLibrary( static_cast< HMODULE >( m_library ) );
		m_library = nullptr;
	}

	void * DynamicLibrary::doGetFunction( std::string const & name )noexcept
	{
		::SetLastError( ERROR_SUCCESS );
		auto result = ( void * )::GetProcAddress( static_cast< HMODULE >( m_library ), name.c_str() );
		
		if ( auto error = ::GetLastError();
			error != ERROR_SUCCESS && !result )
		{
			std::stringstream stream;
			stream << "Couldn't load function [" + name + "]: ";
			stream << "Error code: 0x" << std::hex << std::setw( 8u ) << std::setfill( '0' ) << error;
		}

		return result;
	}
}

#endif
