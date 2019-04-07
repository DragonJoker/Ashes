#include "TransparentRendering.hpp"

#include "Scene.hpp"

#include <AshesPP/Pipeline/VertexLayout.hpp>
#include <AshesPP/RenderPass/RenderSubpass.hpp>

namespace common
{
	TransparentRendering::TransparentRendering( NodesInstancePtr renderer
		, Scene const & scene
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewPtrArray views
		, common::TextureNodePtrArray const & textureNodes )
		: m_instance{ std::move( renderer ) }
	{
		m_instance->initialise( scene
			, stagingBuffer
			, std::move( views )
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
