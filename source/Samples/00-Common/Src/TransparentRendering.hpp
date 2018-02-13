#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class TransparentRendering
	{
	public:
		TransparentRendering( NodesRendererPtr && renderer
			, Object const & submeshes
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );
		virtual void update( renderer::TextureViewCRefArray const & views );
		virtual bool draw( std::chrono::nanoseconds & gpu )const;

	protected:
		void doInitialise( Object const & submeshes
			, renderer::StagingBuffer & stagingBuffer
			, renderer::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );

	private:
		NodesRendererPtr m_renderer;
	};
}
