#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class OpaqueRendering
	{
	public:
		OpaqueRendering( NodesRendererPtr && renderer
			, Scene const & scene
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );
		virtual void update( RenderTarget const & target );
		virtual bool draw( std::chrono::nanoseconds & gpu )const;

	protected:
		NodesRendererPtr m_renderer;
	};
}
