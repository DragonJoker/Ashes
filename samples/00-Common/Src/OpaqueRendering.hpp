#pragma once

#include "NodesRenderer.hpp"

namespace common
{
	class OpaqueRendering
	{
	public:
		OpaqueRendering( NodesInstancePtr renderer
			, Scene const & scene
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, common::TextureNodePtrArray const & textureNodes );
		virtual ~OpaqueRendering() = default;
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
		NodesInstancePtr m_instance;
	};
}
