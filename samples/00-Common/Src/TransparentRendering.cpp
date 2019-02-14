#include "TransparentRendering.hpp"

#include "Scene.hpp"

#include <Ashes/Pipeline/VertexLayout.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>

namespace common
{
	TransparentRendering::TransparentRendering( NodesRendererPtr && renderer
		, Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
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

	void TransparentRendering::draw( std::chrono::nanoseconds & gpu )const
	{
		m_renderer->draw( gpu );
	}
}
