#include "Utils/UtilsPlugin.hpp"

#include "Utils/UtilsFile.hpp"

#include <functional>

namespace utils
{
	Plugin::Plugin( ashes::DynamicLibrary library
		, RendererFactory & factory )
		: m_library{ std::move( library ) }
		, m_creator{ nullptr }
	{
		if ( !m_library.getFunction( "createInstance", m_creator ) )
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
		factory.registerType( m_shortName, [creator]( ashes::Instance::Configuration const & configuration )
			{
				return ashes::InstancePtr{ creator( configuration ) };
			} );
	}

	ashes::InstancePtr Plugin::create( ashes::Instance::Configuration const & configuration )const
	{
		return ashes::InstancePtr{ m_creator( configuration ) };
	}
}
