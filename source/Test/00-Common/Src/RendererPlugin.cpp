#include "RendererPlugin.hpp"

#include <Core/Renderer.hpp>

#include <functional>

namespace common
{
	RendererPlugin::RendererPlugin( renderer::DynamicLibrary && library
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
		factory.registerType( m_shortName, [creator]( renderer::Renderer::Configuration const & configuration )
			{
				return renderer::RendererPtr{ creator( configuration ) };
			} );
	}

	renderer::RendererPtr RendererPlugin::create( renderer::Renderer::Configuration const & configuration )
	{
		return renderer::RendererPtr{ m_creator( configuration ) };
	}
}
