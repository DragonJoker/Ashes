#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <Ashes/Buffer/StagingBuffer.hpp>
#include <Ashes/Buffer/UniformBuffer.hpp>
#include <Ashes/Command/Queue.hpp>
#include <Ashes/Descriptor/DescriptorSet.hpp>
#include <Ashes/Descriptor/DescriptorSetLayout.hpp>
#include <Ashes/Descriptor/DescriptorSetPool.hpp>
#include <Ashes/Image/Image.hpp>
#include <Ashes/Image/ImageView.hpp>
#include <Ashes/Pipeline/PipelineLayout.hpp>
#include <Ashes/Pipeline/Pipeline.hpp>
#include <Ashes/Pipeline/VertexLayout.hpp>
#include <Ashes/RenderPass/FrameBuffer.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>
#include <Ashes/Sync/ImageMemoryBarrier.hpp>

#include <Utils/GlslToSpv.hpp>
#include <Utils/Transform.hpp>

#include <chrono>

namespace common
{
	namespace
	{
		static VkFormat const DepthFormat = VK_FORMAT_D24_UNORM_S8_UINT;
		static VkFormat const ColourFormat = VK_FORMAT_R8G8B8A8_UNORM;
	}

	RenderTarget::RenderTarget( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, VkExtent2D const & size
		, Scene scene
		, ImagePtrArray images )
		: m_device{ device }
		, m_commandPool{ commandPool }
		, m_transferQueue{ transferQueue }
		, m_scene{ std::move( scene ) }
		, m_images{ std::move( images ) }
		, m_size{ size }
	{
		try
		{
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateTextures();
			std::cout << "Textures created." << std::endl;
			doCreateRenderPass();
			std::cout << "Offscreen render pass created." << std::endl;
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}

	}

	RenderTarget::~RenderTarget()
	{
		doCleanup();
	}

	void RenderTarget::resize( VkExtent2D const & size )
	{
		if ( size != m_size )
		{
			m_size = size;
			doUpdateRenderViews();
			doResize( size );
			m_opaque->update( *this );
			m_transparent->update( *this );
		}
	}

	void RenderTarget::update( std::chrono::microseconds const & duration )
	{
		doUpdate( duration );
	}

	void RenderTarget::draw( ashes::Queue const & queue
		, std::chrono::microseconds & gpu )
	{
		std::chrono::nanoseconds opaque;
		std::chrono::nanoseconds transparent;
		m_opaque->draw( queue, opaque );
		m_transparent->draw( queue, transparent );
		gpu = std::chrono::duration_cast< std::chrono::microseconds >( opaque + transparent );
	}

	void RenderTarget::doInitialise()
	{
		m_opaque = doCreateOpaqueRendering( m_device
			, *m_stagingBuffer
			, { m_depthView, m_colourView }
			, m_scene
			, m_textureNodes );
		m_transparent = doCreateTransparentRendering( m_device
			, *m_stagingBuffer
			, { m_depthView, m_colourView }
			, m_scene
			, m_textureNodes );
	}

	void RenderTarget::doCleanup()
	{
		m_stagingBuffer.reset();

		m_transparent.reset();
		m_opaque.reset();
		m_depthView.reset();
		m_depth.reset();
		m_colourView.reset();
		m_colour.reset();

		m_images.clear();
		m_textureNodes.clear();
	}

	void RenderTarget::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device.getDevice()
			, 0u
			, 200u * 1024u * 1024u );
	}

	void RenderTarget::doCreateTextures()
	{
		for ( auto & image : m_images )
		{
			common::TextureNodePtr textureNode = std::make_shared< common::TextureNode >();
			textureNode->image = image;
			textureNode->texture = m_device.createImage(
				{
					0u,
					ashes::ImageType::e2D,
					image->format,
					VkExtent3D{ image->size.width, image->size.height, 1u },
					4u,
					1u,
					VK_SAMPLE_COUNT_1_BIT,
					ashes::ImageTiling::eOptimal,
					( VkImageUsageFlagBits::eTransferSrc
						| VkImageUsageFlagBits::eTransferDst
						| VkImageUsageFlagBits::eSampled )
				}
				, VkMemoryPropertyFlagBits::eDeviceLocal );
			textureNode->view = textureNode->texture->createView( VkImageViewType( textureNode->texture->getType() )
				, textureNode->texture->getFormat()
				, 0u
				, 4u );
			auto view = textureNode->texture->createView( VkImageViewType( textureNode->texture->getType() )
				, textureNode->texture->getFormat() );
			auto staging = ashes::StagingBuffer{ m_device, 0u, getSize( image->size, image->format ) };
			staging.uploadTextureData( m_transferQueue
				, m_commandPool
				, image->format
				, image->data
				, *view );
			textureNode->texture->generateMipmaps( m_commandPool
				, m_transferQueue );
			m_textureNodes.emplace_back( textureNode );
		}
	}

	void RenderTarget::doCreateRenderPass()
	{
		doUpdateRenderViews();
	}

	void RenderTarget::doUpdateRenderViews()
	{
		m_colourView.reset();
		m_colour = m_device.createImage(
			{
				0,
				ashes::ImageType::e2D,
				ColourFormat,
				VkExtent3D{ m_size.width, m_size.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				ashes::ImageTiling::eOptimal,
				( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
					| VkImageUsageFlagBits::eSampled )
			}
			, VkMemoryPropertyFlagBits::eDeviceLocal );
		m_colourView = m_colour->createView( VK_IMAGE_VIEW_TYPE_2D
			, m_colour->getFormat() );

		m_depthView.reset();
		m_depth = m_device.createImage(
			{
				0,
				ashes::ImageType::e2D,
				DepthFormat,
				VkExtent3D{ m_size.width, m_size.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				ashes::ImageTiling::eOptimal,
				VkImageUsageFlagBits::eDepthStencilAttachment
			}
			, VkMemoryPropertyFlagBits::eDeviceLocal );
		m_depthView = m_depth->createView( VK_IMAGE_VIEW_TYPE_2D
			, m_depth->getFormat() );
	}
}
