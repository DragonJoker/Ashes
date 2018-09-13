#include "RendererPlugin.hpp"

#include <Core/Renderer.hpp>

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

		std::string name;

		if ( m_library.getPath().find( "Gl3R" ) != std::string::npos )
		{
			name = "gl3";
		}
		else if ( m_library.getPath().find( "GlR" ) != std::string::npos )
		{
			name = "gl";
		}
		else if ( m_library.getPath().find( "VkR" ) != std::string::npos )
		{
			name = "vk";
		}
		else
		{
			throw std::runtime_error{ "Not a supported renderer plugin" };
		}

		auto creator = m_creator;
		factory.registerType( name, [creator]( ashes::Renderer::Configuration const & configuration )
			{
				return ashes::RendererPtr{ creator( configuration ) };
			} );
	}

	ashes::RendererPtr RendererPlugin::create( ashes::Renderer::Configuration const & configuration )
	{
		return ashes::RendererPtr{ m_creator( configuration ) };
	}
}
