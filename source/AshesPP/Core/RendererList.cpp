/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/RendererList.hpp"

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

	RendererList::RendererList()
		: m_library{ libraryName }
	{
		PFN_ashEnumeratePluginsDescriptions enumeratePluginDescriptions;

		if ( !m_library.getFunction( "ashEnumeratePluginsDescriptions", enumeratePluginDescriptions ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		uint32_t count = 0u;
		enumeratePluginDescriptions( &count, nullptr );

		if ( count > 0 )
		{
			m_plugins.resize( count );
			enumeratePluginDescriptions( &count, m_plugins.data() );
		}
	}

	RendererList::~RendererList()
	{
	}

	AshPluginDescription RendererList::selectPlugin( std::string const & name )const
	{
		auto it = std::find_if( m_plugins.begin()
			, m_plugins.end()
			, [&name]( AshPluginDescription const & lookup )
			{
				return name == lookup.name;
			} );

		if ( it == m_plugins.end() )
		{
			static std::string const Error = "Unknown object type";
			Logger::logError( Error );
			throw std::runtime_error{ Error };
		}

		return *it;
	}
}
