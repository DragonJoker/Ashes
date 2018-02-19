#include "TransparentRendering.hpp"

#include "Scene.hpp"

#include <Pipeline/VertexLayout.hpp>
#include <RenderPass/RenderSubpass.hpp>

namespace common
{
	TransparentRendering::TransparentRendering( NodesRendererPtr && renderer
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

	void TransparentRendering::update( RenderTarget const & target )
	{
		m_renderer->update( target );
	}

	bool TransparentRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		return m_renderer->draw( gpu );
	}
}
