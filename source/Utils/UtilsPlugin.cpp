#include "Utils/UtilsPlugin.hpp"

#include "Utils/UtilsFile.hpp"
#include "Utils/Factory.hpp"

#include <Ashes/Core/Exception.hpp>

#include <functional>

namespace utils
{
	Plugin::Plugin( ashes::DynamicLibrary library )
		: m_library{ std::move( library ) }
		, m_creator{ nullptr }
	{
		if ( !m_library.getFunction( "createInstance", m_creator ) )
		{
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}
		
		if ( !m_library.getFunction( "enumerateVersion", m_enumerateVersion ) )
		{
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}
		
		if ( !m_library.getFunction( "enumerateLayerProperties", m_enumerateLayerProperties ) )
		{
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}
		
		if ( !m_library.getFunction( "enumerateExtensionProperties", m_enumerateExtensionProperties ) )
		{
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		NamerFunction shortNamer;

		if ( !m_library.getFunction( "getShortName", shortNamer ) )
		{
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		NamerFunction fullNamer;

		if ( !m_library.getFunction( "getFullName", fullNamer ) )
		{
			throw std::runtime_error{ "[" + getFileName( m_library.getPath() ) + "] is not a renderer plugin" };
		}

		m_shortName = shortNamer();
		m_fullName = fullNamer();
		auto creator = m_creator;
	}

	uint32_t Plugin::enumerateVersion()const
	{
		uint32_t result;
		auto res = m_enumerateVersion( &result );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Version retrieval" };
		}

		return result;
	}

	ashes::LayerPropertiesArray Plugin::enumerateLayerProperties()const
	{
		uint32_t count;
		ashes::LayerPropertiesArray result;
		ashes::Result res;

		do
		{
			res = m_enumerateLayerProperties( &count, nullptr );

			if ( count )
			{
				result.resize( count );
				res = m_enumerateLayerProperties( &count, result.data() );
			}
		}
		while ( res == ashes::Result::eIncomplete );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Instance layers retrieval" };
		}

		return result;
	}

	ashes::ExtensionPropertiesArray Plugin::enumerateExtensionProperties( std::string const & layerName )const
	{
		uint32_t count;
		ashes::ExtensionPropertiesArray result;
		ashes::Result res;

		do
		{
			res = m_enumerateExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
				, &count
				, nullptr );

			if ( count )
			{
				result.resize( count );
				res = m_enumerateExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
					, &count
					, result.data() );
			}
		}
		while ( res == ashes::Result::eIncomplete );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Instance layer extensions retrieval" };
		}

		return result;
	}

	ashes::InstancePtr Plugin::create( ashes::InstanceCreateInfo createInfo )const
	{
		ashes::Instance * result;
		auto res = m_creator( std::move( createInfo ), &result );

		if ( res != ashes::Result::eSuccess )
		{
			throw ashes::Exception{ res, "Instance creation" };
		}

		return ashes::InstancePtr{ result };
	}
}
