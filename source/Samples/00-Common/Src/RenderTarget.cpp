#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Command/Queue.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/StagingTexture.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <GlslToSpv.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Transform.hpp>

#include <chrono>

namespace common
{
	namespace
	{
		static ashes::Format const DepthFormat = ashes::Format::eD24_UNORM_S8_UINT;
		static ashes::Format const ColourFormat = ashes::Format::eR8G8B8A8_UNORM;
	}

	RenderTarget::RenderTarget( ashes::Device const & device
		, ashes::Extent2D const & size
		, Scene && scene
		, ImagePtrArray && images )
		: m_device{ device }
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

	void RenderTarget::resize( ashes::Extent2D const & size )
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

	void RenderTarget::draw( std::chrono::microseconds & gpu )
	{
		std::chrono::nanoseconds opaque;
		std::chrono::nanoseconds transparent;
		m_opaque->draw( opaque );
		m_transparent->draw( transparent );
		gpu = std::chrono::duration_cast< std::chrono::microseconds >( opaque + transparent );
	}

	void RenderTarget::doInitialise()
	{
		m_opaque = doCreateOpaqueRendering( m_device
			, *m_stagingBuffer
			, { *m_depthView, *m_colourView }
			, m_scene
			, m_textureNodes );
		m_transparent = doCreateTransparentRendering( m_device
			, *m_stagingBuffer
			, { *m_depthView, *m_colourView }
			, m_scene
			, m_textureNodes );
	}

	void RenderTarget::doCleanup()
	{
		m_updateCommandBuffer.reset();

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
		m_updateCommandBuffer = m_device.getGraphicsCommandPool().createCommandBuffer();
		m_stagingBuffer = std::make_unique< ashes::StagingBuffer >( m_device
			, 0u
			, 200u * 1024u * 1024u );
	}

	void RenderTarget::doCreateTextures()
	{
		for ( auto & image : m_images )
		{
			common::TextureNodePtr textureNode = std::make_shared< common::TextureNode >();
			textureNode->image = image;
			auto stagingTexture = m_device.createStagingTexture( image->format
				, { image->size.width, image->size.height } );
			textureNode->texture = m_device.createTexture(
				{
					0u,
					ashes::TextureType::e2D,
					image->format,
					ashes::Extent3D{ image->size.width, image->size.height, 1u },
					4u,
					1u,
					ashes::SampleCountFlag::e1,
					ashes::ImageTiling::eOptimal,
					( ashes::ImageUsageFlag::eTransferSrc
						| ashes::ImageUsageFlag::eTransferDst
						| ashes::ImageUsageFlag::eSampled )
				}
				, ashes::MemoryPropertyFlag::eDeviceLocal );
			textureNode->view = textureNode->texture->createView( ashes::TextureViewType( textureNode->texture->getType() )
				, textureNode->texture->getFormat()
				, 0u
				, 4u );
			auto view = textureNode->texture->createView( ashes::TextureViewType( textureNode->texture->getType() )
				, textureNode->texture->getFormat() );
			stagingTexture->uploadTextureData( *m_updateCommandBuffer
				, image->format
				, image->data
				, *view );
			textureNode->texture->generateMipmaps();
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
		m_colour = m_device.createTexture(
			{
				0,
				ashes::TextureType::e2D,
				ColourFormat,
				ashes::Extent3D{ m_size.width, m_size.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				( ashes::ImageUsageFlag::eColourAttachment
					| ashes::ImageUsageFlag::eSampled )
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_colourView = m_colour->createView( ashes::TextureViewType::e2D
			, m_colour->getFormat() );

		m_depthView.reset();
		m_depth = m_device.createTexture(
			{
				0,
				ashes::TextureType::e2D,
				DepthFormat,
				ashes::Extent3D{ m_size.width, m_size.height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eDepthStencilAttachment
			}
			, ashes::MemoryPropertyFlag::eDeviceLocal );
		m_depthView = m_depth->createView( ashes::TextureViewType::e2D
			, m_depth->getFormat() );
	}
}
