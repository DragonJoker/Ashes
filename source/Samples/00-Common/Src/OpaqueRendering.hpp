#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class OpaqueRendering
	{
	public:
		OpaqueRendering( NodesRendererPtr && renderer
			, Object const & submeshes
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );
		virtual void update( renderer::TextureViewCRefArray const & views );
		virtual bool draw( std::chrono::nanoseconds & gpu )const;

	private:
		NodesRendererPtr m_renderer;
	};
}
