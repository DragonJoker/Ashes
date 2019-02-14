#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class OpaqueRendering
	{
	public:
		OpaqueRendering( NodesRendererPtr && renderer
			, Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );
		virtual ~OpaqueRendering() = default;
		virtual void update( RenderTarget const & target );
		virtual void draw( std::chrono::nanoseconds & gpu )const;

	protected:
		NodesRendererPtr m_renderer;
	};
}
