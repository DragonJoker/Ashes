#pragma once

#include "Scene.hpp"

#include <AshesPP/Core/Device.hpp>

namespace common
{
	class RenderTarget
	{
	public:
		RenderTarget( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, VkExtent2D const & size
			, Scene scene
			, ImagePtrArray images );
		virtual ~RenderTarget();
		void resize( VkExtent2D const & size );
		void update( std::chrono::microseconds const & duration );
		void draw( ashes::Queue const & queue
			, std::chrono::microseconds & gpu );

		inline ashes::ImageViewPtr getColourView()const
		{
			return m_colourView;
		}

		inline ashes::ImageViewPtr getDepthView()const
		{
			return m_depthView;
		}

	protected:
		void doInitialise();

		inline OpaqueRendering const & getOpaqueRendering()const
		{
			assert( m_opaque );
			return *m_opaque;
		}

		inline TransparentRendering const & getTransparentRendering()const
		{
			assert( m_transparent );
			return *m_transparent;
		}

	private:
		void doCleanup();
		void doCreateStagingBuffer();
		void doCreateTextures();
		void doCreateRenderPass();
		void doUpdateRenderViews();

		virtual void doUpdate( std::chrono::microseconds const & duration ) = 0;
		virtual void doResize( VkExtent2D const & size ) = 0;

		virtual OpaqueRenderingPtr doCreateOpaqueRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;
		virtual TransparentRenderingPtr doCreateTransparentRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewPtrArray views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;

	protected:
		utils::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::Queue const & m_transferQueue;
		ashes::StagingBufferPtr m_stagingBuffer;
		VkExtent2D m_size;

	private:
		ImagePtrArray m_images;
		Scene m_scene;
		TextureNodePtrArray m_textureNodes;
		utils::Mat4 m_rotate;
		ashes::ImagePtr m_colour;
		ashes::ImageViewPtr m_colourView;
		ashes::ImagePtr m_depth;
		ashes::ImageViewPtr m_depthView;
		ashes::CommandBufferPtr m_commandBuffer;
		std::shared_ptr< OpaqueRendering > m_opaque;
		std::shared_ptr< TransparentRendering > m_transparent;
	};
}
