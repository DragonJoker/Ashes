/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/RendererList.hpp"

#include <ashes/common/FileUtils.hpp>

#include <ashes/common/Exception.hpp>

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
#elif defined( __APPLE__ )
#if defined( NDEBUG )
	static std::string const libraryName = "libashes.dylib";
#else
	static std::string const libraryName = "libashesd.dylib";
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
		std::clog << "RendererList: " << libraryName << ", " << count << " renderer plugins detected" << std::endl;

		if ( count > 0 )
		{
			m_plugins.resize( count );
			enumeratePluginDescriptions( &count, m_plugins.data() );

			for ( auto & plugin : m_plugins )
			{
				std::clog << "    " << plugin.name << " - " << plugin.description << std::endl;
			}

			getCurrentPluginDescription( &m_current );
			std::clog << "  Currently using " << m_current.name << std::endl;
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
			static std::string const Error = "Unknown Ashes plugin type: ";
			auto error = Error + name;
			throw std::runtime_error{ error };
		}

		if ( !it->support.supported )
		{
			static std::string const Plugin = "Plugin [";
			static std::string const Error = "] is not supported";
			auto error = Plugin + it->description + Error;
			throw std::runtime_error{ error };
		}

		m_current = *it;
		m_selectPlugin( m_current );
		return *it;
	}

	AshPluginDescription RendererList::getSelectedPlugin()const
	{
		return m_current;
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
