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
		virtual ~TransparentRendering() = default;
		virtual void update( RenderTarget const & target );
		virtual void draw( ashes::Queue const & queue
			, std::chrono::nanoseconds & gpu )const;

		inline ashes::CommandPool const & getCommandPool()const
		{
			return m_instance->getCommandPool();
		}

		inline ashes::Queue const & getTransferQueue()const
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
