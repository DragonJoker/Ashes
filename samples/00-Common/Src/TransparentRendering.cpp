#include "TransparentRendering.hpp"

#include "Scene.hpp"

#include <Ashes/Pipeline/VertexLayout.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>

namespace common
{
	TransparentRendering::TransparentRendering( NodesInstancePtr renderer
		, Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
		, common::TextureNodePtrArray const & textureNodes )
		: m_instance{ std::move( renderer ) }
	{
		m_instance->initialise( scene
			, stagingBuffer
			, views
			, textureNodes );
	}

	void TransparentRendering::update( RenderTarget const & target )
	{
		m_instance->update( target );
	}

	void TransparentRendering::draw( ashes::Queue const & queue
		, std::chrono::nanoseconds & gpu )const
	{
		m_instance->draw( queue, gpu );
	}
}
