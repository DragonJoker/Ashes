#include "OpaqueRendering.hpp"

namespace common
{
	OpaqueRendering::OpaqueRendering( NodesRendererPtr && renderer
		, Object const & submeshes
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
		, common::TextureNodePtrArray const & textureNodes )
		: m_renderer{ std::move( renderer ) }
	{
		m_renderer->initialise( submeshes
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
