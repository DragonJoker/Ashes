#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class TransparentRendering
	{
	public:
		TransparentRendering( NodesInstancePtr renderer
			, Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewArray views
			, common::TextureNodePtrArray const & textureNodes );
		virtual ~TransparentRendering()noexcept = default;
		virtual void update( RenderTarget const & target );
		virtual void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const;

		ashes::CommandPool const & getCommandPool()const noexcept
		{
			return m_instance->getCommandPool();
		}

		ashes::Queue const & getTransferQueue()const noexcept
		{
			return m_instance->getTransferQueue();
		}

	protected:
		void doInitialise( Object const & submeshes
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewCRefArray const & views
			, common::TextureNodePtrArray const & textureNodes );

	private:
		NodesInstancePtr m_instance;
	};
}
