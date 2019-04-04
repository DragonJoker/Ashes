/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/Renderer.hpp"

#include <AshesCommon/FileUtils.hpp>

#include <AshesRenderer/Util/Exception.hpp>

namespace ashes
{
#if defined( _WIN32 )
	static std::string const libraryName = "ashes.dll";
#elif defined( __linux__ )
	static std::string const libraryName = "libashes.so";
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

		PFN_ashGetPluginDescription getPluginDescription;

		if ( !m_library.getFunction( "ashGetCurrentPluginDescription", getPluginDescription ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		getPluginDescription( &m_plugin );
	}

	Renderer::~Renderer()
	{
	}
}
