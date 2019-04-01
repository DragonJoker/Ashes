/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/Renderer.hpp"

#include "AshesPP/Core/Exception.hpp"

#include <AshesCommon/FileUtils.hpp>

namespace ashespp
{
#if defined( _WIN32 )
	static std::string const libraryName = "AshesC.dll";
#elif defined( __linux__ )
	static std::string const libraryName = "libAshesC.so";
#else
#	error Unsupported platform
#endif

	Renderer::Renderer()
		: m_library{ libraryName }
	{
		if ( !m_library.getFunction( "vkGetInstanceProcAddr", m_plugin.getInstanceProcAddr ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}
	}

	Renderer::~Renderer()
	{
	}
}
