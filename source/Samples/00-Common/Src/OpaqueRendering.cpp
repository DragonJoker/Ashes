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

	void OpaqueRendering::update( renderer::TextureViewCRefArray const & views )
	{
		m_renderer->update( views );
	}

	bool OpaqueRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		return m_renderer->draw( gpu );
	}
}
