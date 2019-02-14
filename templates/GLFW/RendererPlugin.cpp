#include "RendererPlugin.hpp"

#include <Ashes/Core/Renderer.hpp>

#include <functional>

namespace common
{
	RendererPlugin::RendererPlugin( ashes::DynamicLibrary && library
		, RendererFactory & factory )
		: m_library{ std::move( library ) }
		, m_creator{ nullptr }
	{
		if ( !m_library.getFunction( "createRenderer", m_creator ) )
		{
			throw std::runtime_error{ "Not a renderer plugin" };
		}

		NamerFunction shortNamer;

		if ( !m_library.getFunction( "getShortName", shortNamer ) )
		{
			throw std::runtime_error{ "Not a renderer plugin" };
		}

		NamerFunction fullNamer;

		if ( !m_library.getFunction( "getFullName", fullNamer ) )
		{
			throw std::runtime_error{ "Not a renderer plugin" };
		}

		m_shortName = shortNamer();
		m_fullName = fullNamer();
		auto creator = m_creator;
		factory.registerType( m_shortName, [creator]( ashes::Renderer::Configuration const & configuration )
			{
				return ashes::RendererPtr{ creator( configuration ) };
			} );
	}

	ashes::RendererPtr RendererPlugin::create( ashes::Renderer::Configuration const & configuration )
	{
		return ashes::RendererPtr{ m_creator( configuration ) };
	}
}
