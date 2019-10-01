/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/RendererList.hpp"

#include <common/FileUtils.hpp>

#include <common/Exception.hpp>

namespace ashes
{
#if defined( _WIN32 )
#if defined( NDEBUG )
	static std::string const libraryName = "ashes.dll";
#else
	static std::string const libraryName = "ashesd.dll";
#endif
#elif defined( __linux__ )
#if defined( NDEBUG )
	static std::string const libraryName = "libashes.so";
#else
	static std::string const libraryName = "libashesd.so";
#endif
#else
#	error Unsupported platform
#endif

	RendererList::RendererList()
		: m_library{ libraryName }
	{
		if ( !m_library.getFunction( "ashSelectPlugin", m_selectPlugin ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library.getPath() ) + "] is not Ashes" };
		}

		PFN_ashEnumeratePluginsDescriptions enumeratePluginDescriptions;
		if ( !m_library.getFunction( "ashEnumeratePluginsDescriptions", enumeratePluginDescriptions ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library.getPath() ) + "] is not Ashes" };
		}

		PFN_ashGetPluginDescription getCurrentPluginDescription;
		if ( !m_library.getFunction( "ashGetCurrentPluginDescription", getCurrentPluginDescription ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library.getPath() ) + "] is not Ashes" };
		}

		uint32_t count = 0u;
		enumeratePluginDescriptions( &count, nullptr );

		if ( count > 0 )
		{
			m_plugins.resize( count );
			enumeratePluginDescriptions( &count, m_plugins.data() );
		}

		getCurrentPluginDescription( &m_current );
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

		m_current = *it;
		m_selectPlugin( m_current );
		return *it;
	}

	std::vector< AshPluginDescription >::const_iterator RendererList::find( std::string const & name )const
	{
		return std::find_if( m_plugins.begin()
			, m_plugins.end()
			, [&name]( AshPluginDescription const & lookup )
			{
				return lookup.name == name;
			} );
	}
}
