#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/Sync/Queue.hpp>

#include <util/GlslToSpv.hpp>
#include <util/Transform.hpp>

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
		m_depth.reset();
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
					VK_IMAGE_TYPE_2D,
					image->format,
					VkExtent3D{ image->size.width, image->size.height, 1u },
					4u,
					1u,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					( VK_IMAGE_USAGE_TRANSFER_SRC_BIT
						| VK_IMAGE_USAGE_TRANSFER_DST_BIT
						| VK_IMAGE_USAGE_SAMPLED_BIT )
				}
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
			textureNode->view = textureNode->texture->createView( VkImageViewType( textureNode->texture->getType() )
				, textureNode->texture->getFormat()
				, 0u
				, 4u );
			auto view = textureNode->texture->createView( VkImageViewType( textureNode->texture->getType() )
				, textureNode->texture->getFormat() );
			auto staging = ashes::StagingBuffer{ m_device, 0u, ashes::getSize( image->size, image->format ) };
			staging.uploadTextureData( m_transferQueue
				, m_commandPool
				, image->format
				, image->data
				, view );
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
		m_colour = m_device.createImage(
			{
				0,
				VK_IMAGE_TYPE_2D,
				ColourFormat,
				VkExtent3D{ m_size.width, m_size.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
					| VK_IMAGE_USAGE_SAMPLED_BIT )
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_colourView = m_colour->createView( VK_IMAGE_VIEW_TYPE_2D
			, m_colour->getFormat() );

		m_depth = m_device.createImage(
			{
				0,
				VK_IMAGE_TYPE_2D,
				DepthFormat,
				VkExtent3D{ m_size.width, m_size.height, 1u },
				1u,
				1u,
				VK_SAMPLE_COUNT_1_BIT,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
			}
			, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
		m_depthView = m_depth->createView( VK_IMAGE_VIEW_TYPE_2D
			, m_depth->getFormat() );
	}
}
