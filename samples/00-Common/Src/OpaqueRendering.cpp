#include "OpaqueRendering.hpp"

#include "Scene.hpp"

#include <Ashes/Pipeline/VertexLayout.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>

namespace common
{
	OpaqueRendering::OpaqueRendering( NodesInstancePtr renderer
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

	void OpaqueRendering::update( RenderTarget const & target )
	{
		m_instance->update( target );
	}

	void OpaqueRendering::draw( ashes::Queue const & queue
		, std::chrono::nanoseconds & gpu )const
	{
		m_instance->draw( queue, gpu );
	}
}
