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
			, ashes::ImageViewArray views
			, common::TextureNodePtrArray const & textureNodes );
		virtual ~OpaqueRendering()noexcept = default;
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
		NodesRenderer & getInstance()const noexcept
		{
			assert( m_instance );
			return *m_instance;
		}

	private:
		NodesInstancePtr m_instance;
	};
}
