/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/RendererList.hpp"

#include <ashes/common/FileUtils.hpp>

#include <ashes/common/Exception.hpp>

namespace ashes
{
	namespace
	{
#if defined( _WIN32 )
#	if defined( NDEBUG )
		static std::string const libraryName = "vulkan-1.dll";
#	else
		static std::string const libraryName = "vulkand-1.dll";
#	endif
#elif defined( __linux__ )
#	if defined( NDEBUG )
		static std::string const libraryName = "libvulkan.so.1";
#	else
		static std::string const libraryName = "libvulkand.so.1";
#	endif
#elif defined( __APPLE__ )
#	if defined( NDEBUG )
		static std::string const libraryName = "libvulkan.1.dylib";
#	else
		static std::string const libraryName = "libvulkand.1.dylib";
#	endif
#else
#	error Unsupported platform
#endif
	}

	RendererList::RendererList()
	{
		lookForSharedLibrary( [this]( std::string const & folder
			, std::string const & name )
			{
				if ( libraryName == name
					&& !m_library )
				{
					m_library = std::make_unique< ashes::DynamicLibrary >( folder / name );

					if ( !m_library->getFunction( "ashSelectPlugin", m_selectPlugin ) )
					{
						m_library.reset();
					}
				}

				return m_library != nullptr;
			} );

		if ( !m_library )
		{
			throw std::runtime_error{ "Could not find Ashes shared library" };
		}

		PFN_ashEnumeratePluginsDescriptions enumeratePluginDescriptions;
		if ( !m_library->getFunction( "ashEnumeratePluginsDescriptions", enumeratePluginDescriptions ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library->getPath() ) + "] is not Ashes" };
		}

		PFN_ashGetPluginDescription getCurrentPluginDescription;
		if ( !m_library->getFunction( "ashGetCurrentPluginDescription", getCurrentPluginDescription ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( m_library->getPath() ) + "] is not Ashes" };
		}

		uint32_t count = 0u;
		enumeratePluginDescriptions( &count, nullptr );
		log::info << "RendererList: " << libraryName << ", " << count << " renderer plugins detected" << std::endl;

		if ( count > 0 )
		{
			m_plugins.resize( count );
			enumeratePluginDescriptions( &count, m_plugins.data() );

			for ( auto & plugin : m_plugins )
			{
				log::info << "    " << plugin.name
					<< " - " << plugin.description
					<< " - " << ( plugin.support.supported ? std::string{ "Supported" } : std::string{ "Unsupported" } )
					<< std::endl;
			}

			getCurrentPluginDescription( &m_current );
			log::info << "  Currently using " << m_current.name << std::endl;
		}
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
