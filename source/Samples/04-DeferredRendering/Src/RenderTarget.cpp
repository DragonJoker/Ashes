#include "RenderTarget.hpp"

#include "OpaqueRendering.hpp"
#include "TransparentRendering.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Command/Queue.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Utils/Transform.hpp>

#include <chrono>

namespace vkapp
{
	namespace
	{
		static renderer::PixelFormat const DepthFormat = renderer::PixelFormat::eD24S8;
	}

	RenderTarget::RenderTarget( renderer::Device const & device
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::UIVec2 const & size
		, common::Object && object
		, common::ImagePtrArray && images )
		: m_device{ device }
		, m_lightsUbo{ lightsUbo }
		, m_object{ std::move( object ) }
		, m_images{ std::move( images ) }
		, m_size{ size }
	{
		try
		{
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateUniformBuffer();
			std::cout << "Uniform buffers created." << std::endl;
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

	void RenderTarget::resize( renderer::UIVec2 const & size )
	{
		if ( size != m_size )
		{
			m_size = size;
			doUpdateMatrixUbo();
			doUpdateRenderViews();
			m_opaque->update( *m_colourView, *m_depthView );
			m_transparent->update( *m_colourView, *m_depthView );
		}
	}

	void RenderTarget::update()
	{
		static renderer::Mat4 const originalTranslate = []()
		{
			renderer::Mat4 result;
			result = utils::translate( result, { 0, 0, -5 } );
			return result;
		}( );
		static renderer::Clock::time_point save = renderer::Clock::now();
		auto duration = std::chrono::duration_cast< std::chrono::microseconds >( renderer::Clock::now() - save );
		m_rotate = utils::rotate( m_rotate
			, float( utils::DegreeToRadian ) * ( duration.count() / 20000.0f )
			, { 0, 1, 0 } );
		m_objectUbo->getData( 0 ) = originalTranslate * m_rotate;
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_objectUbo->getDatas()
			, *m_objectUbo
			, renderer::PipelineStageFlag::eVertexShader );
		save = renderer::Clock::now();
	}

	bool RenderTarget::draw()
	{
		auto result = m_opaque->draw();
		result &= m_transparent->draw();
		return result;
	}

	void RenderTarget::doCleanup()
	{
		m_updateCommandBuffer.reset();

		m_stagingBuffer.reset();

		m_matrixUbo.reset();
		m_objectUbo.reset();

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
		m_stagingBuffer = std::make_unique< renderer::StagingBuffer >( m_device
			, 0u
			, 200u * 1024u * 1024u );
	}

	void RenderTarget::doCreateUniformBuffer()
	{
		m_matrixUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_objectUbo = std::make_unique< renderer::UniformBuffer< renderer::Mat4 > >( m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
	}

	void RenderTarget::doCreateTextures()
	{
		for ( auto & image : m_images )
		{
			common::TextureNodePtr textureNode = std::make_shared< common::TextureNode >();
			textureNode->image = image;
			textureNode->texture = m_device.createTexture();
			textureNode->texture->setImage( image->format
				, { image->size[0], image->size[1] }
				, 4u
				, renderer::ImageUsageFlag::eTransferSrc | renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled );
			textureNode->view = textureNode->texture->createView( textureNode->texture->getType()
				, textureNode->texture->getFormat()
				, 0u
				, 4u
				, 0u
				, 1u );
			auto view = textureNode->texture->createView( textureNode->texture->getType()
				, textureNode->texture->getFormat()
				, 0u
				, 1u
				, 0u
				, 1u );
			m_stagingBuffer->uploadTextureData( *m_updateCommandBuffer
				, image->data
				, *view );
			textureNode->texture->generateMipmaps();
			m_textureNodes.emplace_back( textureNode );
		}
	}

	void RenderTarget::doCreateRenderPass()
	{
		doUpdateMatrixUbo();
		doUpdateRenderViews();
		m_opaque = std::make_unique< OpaqueRendering >( m_device
			, m_object
			, *m_matrixUbo
			, *m_objectUbo
			, m_lightsUbo
			, *m_stagingBuffer
			, *m_colourView
			, *m_depthView
			, m_textureNodes );
		m_transparent = std::make_unique< TransparentRendering >( m_device
			, m_object
			, *m_matrixUbo
			, *m_objectUbo
			, m_lightsUbo
			, *m_stagingBuffer
			, *m_colourView
			, *m_depthView
			, m_textureNodes );
	}

	void RenderTarget::doUpdateMatrixUbo()
	{
#if 0
		float halfWidth = static_cast< float >( m_size[0] ) * 0.5f;
		float halfHeight = static_cast< float >( m_size[1] ) * 0.5f;
		float wRatio = 1.0f;
		float hRatio = 1.0f;

		if ( halfHeight > halfWidth )
		{
			hRatio = halfHeight / halfWidth;
		}
		else
		{
			wRatio = halfWidth / halfHeight;
		}

		m_matrixUbo->getData( 0u ) = m_device->ortho( -2.0f * wRatio
			, 2.0f * wRatio
			, -2.0f * hRatio
			, 2.0f * hRatio
			, 0.0f
			, 10.0f );
#else
		auto width = float( m_size[0] );
		auto height = float( m_size[1] );
		m_matrixUbo->getData( 0u ) = m_device.perspective( utils::toRadians( 90.0_degrees )
			, width / height
			, 0.01f
			, 100.0f );
#endif
		m_stagingBuffer->uploadUniformData( *m_updateCommandBuffer
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, renderer::PipelineStageFlag::eVertexShader );
	}

	void RenderTarget::doUpdateRenderViews()
	{
		m_colourView.reset();
		m_colour = m_device.createTexture();
		m_colour->setImage( renderer::PixelFormat::eR8G8B8A8
			, m_size
			, renderer::ImageUsageFlag::eColourAttachment | renderer::ImageUsageFlag::eSampled | renderer::ImageUsageFlag::eTransferDst );
		m_colourView = m_colour->createView( m_colour->getType()
			, m_colour->getFormat()
			, 0u
			, 1u
			, 0u
			, 1u );

		m_depthView.reset();
		m_depth = m_device.createTexture();
		m_depth->setImage( DepthFormat
			, m_size
			, renderer::ImageUsageFlag::eDepthStencilAttachment | renderer::ImageUsageFlag::eSampled );
		m_depthView = m_depth->createView( m_depth->getType()
			, m_depth->getFormat()
			, 0u
			, 1u
			, 0u
			, 1u );
	}
}
