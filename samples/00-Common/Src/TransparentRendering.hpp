#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class TransparentRendering
	{
	public:
		TransparentRendering( NodesRendererPtr && renderer
			, Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );
		virtual ~TransparentRendering() = default;
		virtual void update( RenderTarget const & target );
		virtual void draw( std::chrono::nanoseconds & gpu )const;

	protected:
		void doInitialise( Object const & submeshes
			, ashes::StagingBuffer & stagingBuffer
			, ashes::TextureViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );

	private:
		NodesRendererPtr m_renderer;
	};
}