/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/Renderer.hpp"

#include "AshesPP/Core/Exception.hpp"
#include "AshesPP/Utils/FileUtils.hpp"

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
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		//PFN_ashGetPluginsDescriptions getPluginsDescriptions;

		//if ( !m_library.getFunction( "ashGetPluginsDescriptions", getPluginsDescriptions ) )
		//{
		//	throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		//}

		//PFN_ashGetPluginDescription getPluginDescription;

		//if ( !m_library.getFunction( "ashGetCurrentPluginDescription", getPluginDescription ) )
		//{
		//	throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		//}

		//getPluginDescription( &m_plugin );
	}

	Renderer::~Renderer()
	{
	}
}
