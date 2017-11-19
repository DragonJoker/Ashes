#include "RenderWindow.h"

#include "Scene.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlFrameBuffer.h>
#include <GlLib/GlRenderBuffer.h>
#include <GlLib/GlSampler.h>
#include <GlLib/GlTexture.h>
#include <GlLib/GlUniform.h>
#include <GlLib/OpenGL.h>

namespace render
{
	namespace
	{
		std::vector< RenderWindow::Vertex > doGetVtxData()
		{
			return std::vector< RenderWindow::Vertex >
			{
				{
					{ { -1, -1 }, { 1, 0 } },
					{ {  1, -1 }, { 0, 0 } },
					{ {  1,  1 }, { 0, 1 } },
					{ { -1,  1 }, { 1, 1 } },
				}
			};
		}
	}

	RenderWindow::RenderWindow( gl::IVec2 const & dimensions
		, render::FontLoader & loader
		, bool debug )
		: m_pipeline{ true, false, false, false }
		, m_target{ dimensions, gl::PixelFormat::eR8G8B8 }
		, m_scene{ dimensions }
		, m_size{ dimensions }
		, m_sampler{ gl::OpenGL::createSampler( gl::WrapMode::eClampToEdge
			, gl::WrapMode::eClampToEdge
			, gl::MinFilter::eLinear
			, gl::MagFilter::eLinear ) }
		, m_program{ UberShader::createShaderProgram( RenderType::eScene
			, TextureFlag::eNone
			, OpacityType::eOpaque
			, ObjectType::eTexture ) }
		, m_vbo{ gl::makeBuffer( gl::BufferTarget::eArrayBuffer
			, doGetVtxData() ) }
		, m_posAttrib{ m_program->createAttribute< gl::Vec2 >( "position"
			, sizeof( RenderWindow::Vertex )
			, 0 ) }
		, m_texAttrib{ m_program->createAttribute< gl::Vec2 >( "texture"
			, sizeof( RenderWindow::Vertex )
			, sizeof( gl::Vec2 ) ) }
		, m_texUniform{ gl::makeUniform< int >( "mapDiffuse"
			, *m_program ) }
		, m_viewport{ dimensions }
		, m_overlayRenderer{ std::make_unique< OverlayRenderer >() }
		, m_picking{ dimensions }
		, m_debug{ debug, m_scene, loader }
	{
	}

	void RenderWindow::beginFrame()
	{
		m_debug.start();
	}

	void RenderWindow::update()
	{
		state().update();
		m_scene.camera().reorient( state().rotation() );
		m_scene.camera().fovY( state().zoom() );
		m_scene.update();
	}

	void RenderWindow::updateOverlays()
	{
		m_scene.updateOverlays();
	}

	void RenderWindow::draw()const noexcept
	{
#if !DEBUG_PICKING
		if ( m_pick )
		{
			m_picking.pick( m_pickPosition
				, m_scene.camera()
				, state().zoomBounds().percent( state().zoom() )
				, m_scene.renderObjects()
				, m_scene.renderBillboards() );
			m_pick = false;
		}

		m_target.drawScene( m_scene );
		doRenderTextureToScreen( m_target.texture() );
#else
		m_picking.pick( m_pickPosition
			, m_scene.camera()
			, state().zoomBounds().percent( state().zoom() )
			, m_scene.renderObjects()
			, m_scene.renderBillboards() );
		doRenderTextureToScreen( m_picking.texture() );
#endif

		m_overlayRenderer->beginRender( m_size );
		m_overlayRenderer->draw( m_scene.overlays() );
		m_overlayRenderer->endRender();
	}

	void RenderWindow::endFrame()
	{
		m_debug.count( m_scene.billboards()
			, m_scene.billboardsBuffers() );
		m_debug.end();
	}

	void RenderWindow::resize( gl::IVec2 const & size )noexcept
	{
		m_size = size;
		m_viewport.resize( m_size );
		m_scene.resize( m_size );
	}

	void RenderWindow::doRenderTextureToScreen( gl::Texture const & texture )const noexcept
	{
		glCheckError( glClear, GL_COLOR_BUFFER_BIT );
		m_pipeline.apply();
		m_viewport.apply();
		m_program->bind();
		m_vbo->bind();
		m_posAttrib->bind();
		m_texAttrib->bind();
		m_texUniform->value( 0 );
		texture.bind( 0 );
		m_sampler->bind( 0 );
		glCheckError( glDrawArrays
			, GL_TRIANGLE_FAN
			, 0
			, m_vbo->count() );
		m_sampler->unbind( 0 );
		texture.unbind( 0 );
		m_texAttrib->unbind();
		m_posAttrib->unbind();
		m_vbo->unbind();
	}
}
