#include "common/DynamicLibrary.hpp"

#include "common/Exception.hpp"

#if defined( __linux__ )

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <dlfcn.h>

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
		m_library = dlopen( m_path.c_str(), RTLD_LAZY );

		if ( !m_library )
		{
			std::stringstream stream;
			stream << "Couldn't load dynamic library at [" << m_path << "]: ";
			stream << "Error code: " << std::hex << dlerror();
			throw ashes::BaseException{ stream.str() };
		}
	}

	void DynamicLibrary::doClose()noexcept
	{
		dlclose( m_library );
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
		}

		return result;
	}
}

#endif
