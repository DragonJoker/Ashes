#include "TransparentRendering.hpp"

namespace common
{
	TransparentRendering::TransparentRendering( NodesRendererPtr && renderer
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

	void TransparentRendering::update( RenderTarget const & target )
	{
		m_renderer->update( target );
	}

	bool TransparentRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		return m_renderer->draw( gpu );
	}
}
