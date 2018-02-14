#include "OpaqueRendering.hpp"

#include "Scene.hpp"

namespace common
{
	OpaqueRendering::OpaqueRendering( NodesRendererPtr && renderer
		, Scene const & scene
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
		, common::TextureNodePtrArray const & textureNodes )
		: m_renderer{ std::move( renderer ) }
	{
		m_renderer->initialise( scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	void OpaqueRendering::update( RenderTarget const & target )
	{
		m_renderer->update( target );
	}

	bool OpaqueRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		return m_renderer->draw( gpu );
	}
}
