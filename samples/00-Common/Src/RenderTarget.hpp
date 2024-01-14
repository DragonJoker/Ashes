#pragma once

#include "Scene.hpp"

#include <ashespp/Core/Device.hpp>

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
		virtual ~RenderTarget()noexcept;
		void resize( VkExtent2D const & size );
		void update( std::chrono::microseconds const & duration );
		void draw( ashes::Queue const & queue
			, std::chrono::microseconds & gpu )const;

		ashes::ImageView getColourView()const noexcept
		{
			return m_colourView;
		}

		ashes::ImageView getDepthView()const noexcept
		{
			return m_depthView;
		}

	protected:
		void doInitialise();

		OpaqueRendering const & getOpaqueRendering()const noexcept
		{
			assert( m_opaque );
			return *m_opaque;
		}

		TransparentRendering const & getTransparentRendering()const noexcept
		{
			assert( m_transparent );
			return *m_transparent;
		}

		utils::Device const & getDevice()const noexcept
		{
			return m_device;
		}

		ashes::CommandPool const & getCommandPool()const noexcept
		{
			return m_commandPool;
		}

		ashes::Queue const & getTransferQueue()const noexcept
		{
			return m_transferQueue;
		}

		ashes::StagingBuffer & getStagingBuffer()const noexcept
		{
			assert( m_stagingBuffer );
			return *m_stagingBuffer;
		}

		VkExtent2D const & getSize()const noexcept
		{
			return m_size;
		}

		void setSize( VkExtent2D const & v )noexcept
		{
			m_size = v;
		}

	private:
		void doCleanup()noexcept;
		void doCreateStagingBuffer();
		void doCreateTextures();
		void doCreateRenderPass();
		void doUpdateRenderViews();

		virtual void doUpdate( std::chrono::microseconds const & duration ) = 0;
		virtual void doResize( VkExtent2D const & size ) = 0;

		virtual OpaqueRenderingPtr doCreateOpaqueRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewArray views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;
		virtual TransparentRenderingPtr doCreateTransparentRendering( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::ImageViewArray views
			, Scene const & scene
			, TextureNodePtrArray const & textureNodes ) = 0;

	private:
		utils::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::Queue const & m_transferQueue;
		ashes::StagingBufferPtr m_stagingBuffer{};
		VkExtent2D m_size{};
		ImagePtrArray m_images{};
		Scene m_scene{};
		TextureNodePtrArray m_textureNodes{};
		utils::Mat4 m_rotate{};
		ashes::ImagePtr m_colour{};
		ashes::ImageView m_colourView{};
		ashes::ImagePtr m_depth{};
		ashes::ImageView m_depthView{};
		ashes::CommandBufferPtr m_commandBuffer{};
		std::shared_ptr< OpaqueRendering > m_opaque{};
		std::shared_ptr< TransparentRendering > m_transparent{};
	};
}
