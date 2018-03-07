#include "RenderPanel.hpp"

#include "RenderTarget.hpp"

#include <AssimpLoader.hpp>
#include <FileUtils.hpp>

#include <Buffer/UniformBuffer.hpp>

namespace vkapp
{
	namespace
	{
		static uint32_t constexpr ObjectCount = 100u;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size )
		: common::RenderPanel{ parent
		, size
		, AppName.ToStdString()
		, AppDesc.ToStdString() }
	{
	}

	void RenderPanel::doInitialise( renderer::Device const & device
		, renderer::Extent2D const & size )
	{
		common::ImagePtrArray images;
		common::Scene scene;

		// Initialise material.
		std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / "Assets";
		images.emplace_back( std::make_shared< common::Image >( common::loadImage( shadersFolder / "texture.png" ) ) );
		auto init = ObjectCount * -2.0f;
		renderer::Vec3 position{ init, init, init };
		scene.billboard.material.textures.emplace_back( images[0] );
		scene.billboard.material.hasOpacity = false;
		scene.billboard.material.data.texturesCount = 1u;
		scene.billboard.material.data.textureOperators[0].diffuse = 1;

		// Initialise billboards list.
		scene.billboard.list.reserve( ObjectCount * ObjectCount * ObjectCount );

		for ( auto i = 0u; i < ObjectCount; ++i )
		{
			position[1] = init;

			for ( auto j = 0u; j < ObjectCount; ++j )
			{
				position[2] = init;

				for ( auto k = 0u; k < ObjectCount; ++k )
				{
					scene.billboard.list.push_back( { position, renderer::Vec2{ 2.0, 2.0 } } );
					position[2] += 4;
				}

				position[1] += 4;
			}

			position[0] += 4;
		}

		m_renderTarget = std::make_unique< RenderTarget >( device
			, size
			, std::move( scene )
			, std::move( images ) );
	}

	void RenderPanel::doUpdateOverlays( common::Gui const & overlay )
	{
	}

	void RenderPanel::doUpdate()
	{
		auto & renderTarget = static_cast< RenderTarget & >( *m_renderTarget );
		renderTarget.enableMoveCamera( m_mouse.left );
		renderTarget.updateMousePosition( m_mouse.position );
	}
}
