#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"
#include "FileUtils.hpp"

#include <Renderer/BackBuffer.hpp>
#include <Renderer/Connection.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/FrameBuffer.hpp>
#include <Renderer/ImageMemoryBarrier.hpp>
#include <Renderer/Queue.hpp>
#include <Renderer/RenderPass.hpp>
#include <Renderer/RenderPassState.hpp>
#include <Renderer/RenderSubpass.hpp>
#include <Renderer/RenderSubpassState.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Scissor.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/SwapChain.hpp>
#include <Renderer/Texture.hpp>
#include <Renderer/UniformBuffer.hpp>
#include <Renderer/VertexBuffer.hpp>
#include <Renderer/VertexLayout.hpp>
#include <Renderer/Viewport.hpp>

#include <Utils/Transform.hpp>

#include <chrono>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			RenderTimer = 42
		}	Ids;

		static int const TimerTimeMs = 10;
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, renderer::Renderer const & vulkan )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
		, m_vertexData
		{
			{
				{
					{ -200.0f, -200.0f, 0.0f, 1.0f },
					{ -0.1f, -0.1f },
				},
				{
					{ -200.0f, 200.0f, 0.0f, 1.0f },
					{ -0.1f, 1.1f },
				},
				{
					{ 200.0f, -200.0f, 0.0f, 1.0f },
					{ 1.1f, -0.1f },
				},
				{
					{ 200.0f, 200.0f, 0.0f, 1.0f },
					{ 1.1f, 1.1f },
				}
			}
		}
	{
		try
		{
			doCreateDevice( vulkan );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swap chain created." << std::endl;
			doCreateStagingBuffer();
			std::cout << "Staging buffer created." << std::endl;
			doCreateTexture();
			std::cout << "Truck texture created." << std::endl;
			doCreateUniformBuffer();
			std::cout << "Uniform buffer created." << std::endl;
			doCreateDescriptorSet();
			std::cout << "Descriptor set created." << std::endl;
			doCreateRenderPass();
			std::cout << "Render pass created." << std::endl;
			doCreateVertexBuffer();
			std::cout << "Vertex buffer created." << std::endl;
			doCreatePipeline();
			std::cout << "Pipeline created." << std::endl;
			doPrepareFrames();
			DEBUG_WRITE( "tutorial07.log" );

			m_timer->Start( TimerTimeMs );

			Connect( int( Ids::RenderTimer )
				, wxEVT_TIMER
				, wxTimerEventHandler( RenderPanel::onTimer )
				, nullptr
				, this );
			Connect( wxID_ANY
				, wxEVT_SIZE
				, wxSizeEventHandler( RenderPanel::onSize )
				, nullptr
				, this );
		}
		catch ( std::exception & )
		{
			delete m_timer;

			if ( m_device )
			{
				m_device->getDevice().waitIdle();
			}

			m_commandBuffers.clear();
			m_frameBuffers.clear();
			m_uniformBuffer.reset();
			m_descriptorSet.reset();
			m_descriptorPool.reset();
			m_descriptorLayout.reset();
			m_sampler.reset();
			m_texture.reset();
			m_stagingBuffer.reset();
			m_pipeline.reset();
			m_pipelineLayout.reset();
			m_program.reset();
			m_vertexBuffer.reset();
			m_renderPass.reset();
			m_swapChain.reset();
			m_device.reset();

			throw;
		}
	}

	RenderPanel::~RenderPanel()
	{
		delete m_timer;
		m_device->getDevice().waitIdle();
		m_commandBuffers.clear();
		m_frameBuffers.clear();
		m_uniformBuffer.reset();
		m_descriptorSet.reset();
		m_descriptorPool.reset();
		m_descriptorLayout.reset();
		m_sampler.reset();
		m_texture.reset();
		m_stagingBuffer.reset();
		m_pipeline.reset();
		m_pipelineLayout.reset();
		m_program.reset();
		m_vertexBuffer.reset();
		m_renderPass.reset();
		m_swapChain.reset();
		m_device.reset();
	}

	void RenderPanel::doCreateDevice( renderer::Renderer const & vulkan )
	{
		m_device = vulkan.createDevice( makeConnection( this, vulkan ) );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( { size.x, size.y } );
		m_swapChain->setClearColour( { 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			auto size = m_swapChain->getDimensions();
			float halfWidth = static_cast< float >( size.x ) * 0.5f;
			float halfHeight = static_cast< float >( size.y ) * 0.5f;
			m_uniformBuffer->getData( 0u ) = utils::ortho( -halfWidth
				, halfWidth
				, -halfHeight
				, halfHeight
				, -1.0f
				, 1.0f );
			m_stagingBuffer->copyUniformData( m_swapChain->getDefaultResources().getCommandBuffer()
				, m_uniformBuffer->getDatas()
				, *m_uniformBuffer
				, renderer::PipelineStageFlag::eVertexShader );
			doPrepareFrames();
		} );
	}

	void RenderPanel::doCreateTexture()
	{
		std::string shadersFolder = getPath( getExecutableDirectory() ) / "share" / "Tutorial07" / "Shaders";

		if ( !wxFileExists( shadersFolder / "texture.png" ) )
		{
			throw std::runtime_error{ "Couldn't find truck texture." };
		}

		wxImage image{ shadersFolder / "texture.png", wxBITMAP_TYPE_PNG };

		if ( image.IsOk() )
		{
			uint8_t * data = image.GetData();
			uint32_t size = image.GetSize().x * image.GetSize().y;
			vk::ByteArray buffer( size * 4 );

			if ( image.HasAlpha() )
			{
				uint8_t * alpha = image.GetData();
				auto it = buffer.begin();

				for ( uint32_t i{ 0u }; i < size; ++i )
				{
					*it++ = *data++;
					*it++ = *data++;
					*it++ = *data++;
					*it++ = *alpha++;
				}
			}
			else
			{
				auto it = buffer.begin();

				for ( uint32_t i{ 0u }; i < size; ++i )
				{
					*it++ = *data++;
					*it++ = *data++;
					*it++ = *data++;
					*it++ = 0xFF;
				}
			}

			m_texture = std::make_unique< renderer::Texture >( *m_device );
			m_texture->setImage( utils::PixelFormat::eR8G8B8A8
				, { image.GetSize().x, image.GetSize().y } );
			m_sampler = std::make_unique< renderer::Sampler >( *m_device
				, renderer::WrapMode::eClampToEdge
				, renderer::WrapMode::eClampToEdge
				, renderer::WrapMode::eClampToEdge
				, renderer::Filter::eLinear
				, renderer::Filter::eLinear );
			m_stagingBuffer->copyTextureData( m_swapChain->getDefaultResources().getCommandBuffer()
				, buffer
				, *m_texture );
		}
		else
		{
			throw std::runtime_error{ "Failed to load truck texture image" };
		}
	}

	void RenderPanel::doCreateUniformBuffer()
	{
		m_uniformBuffer = std::make_unique< renderer::UniformBuffer< utils::Mat4 > >( *m_device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		auto size = m_swapChain->getDimensions();
		float halfWidth = static_cast< float >( size.x ) * 0.5f;
		float halfHeight = static_cast< float >( size.y ) * 0.5f;
		m_uniformBuffer->getData( 0u ) = utils::ortho( -halfWidth
			, halfWidth
			, -halfHeight
			, halfHeight
			, -1.0f
			, 1.0f );
		m_stagingBuffer->copyUniformData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_uniformBuffer->getDatas()
			, *m_uniformBuffer
			, renderer::PipelineStageFlag::eVertexShader );
	}

	void RenderPanel::doCreateDescriptorSet()
	{
		std::vector< renderer::DescriptorSetLayoutBinding > bindings
		{
			renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
			renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex }
		};
		m_descriptorLayout = std::make_unique< renderer::DescriptorSetLayout >( *m_device, bindings );
		m_descriptorPool = std::make_unique< renderer::DescriptorSetPool >( *m_descriptorLayout );
		m_descriptorSet = std::make_unique< renderer::DescriptorSet >( *m_descriptorPool );
		m_descriptorSet->createBinding( bindings[0]
			, *m_texture
			, *m_sampler );
		m_descriptorSet->createBinding( bindings[1]
			, *m_uniformBuffer );
		m_descriptorSet->update();
	}

	void RenderPanel::doCreateRenderPass()
	{
		std::vector< utils::PixelFormat > formats{ { m_swapChain->getFormat() } };
		renderer::RenderSubpass subpass{ *m_device
			, formats
			, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite } };
		m_renderPass = std::make_unique< renderer::RenderPass >( *m_device
			, formats
			, std::vector< renderer::RenderSubpass >{ subpass }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal } }
			, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal } } );
	}

	void RenderPanel::doCreateVertexBuffer()
	{
		m_vertexBuffer = renderer::makeVertexBuffer< TexturedVertexData >( *m_device
			, 0u
			, uint32_t( m_vertexData.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		auto layout = std::make_unique< renderer::VertexLayout >( 0 );
		layout->createAttribute< utils::Vec4 >( 0u
			, uint32_t( offsetof( TexturedVertexData, position ) ) );
		layout->createAttribute< utils::Vec2 >( 1u
			, uint32_t( offsetof( TexturedVertexData, uv ) ) );
		m_vertexBuffer->setLayout( std::move( layout ) );
		m_stagingBuffer->copyVertexData( m_swapChain->getDefaultResources().getCommandBuffer()
			, m_vertexData
			, *m_vertexBuffer
			, renderer::PipelineStageFlag::eVertexInput );
	}

	void RenderPanel::doCreateStagingBuffer()
	{
		m_stagingBuffer = std::make_unique < renderer::StagingBuffer > ( *m_device );
	}

	void RenderPanel::doCreatePipeline()
	{
		m_pipelineLayout = std::make_unique< renderer::PipelineLayout >( *m_device, m_descriptorLayout.get() );
		wxSize size{ GetClientSize() };
		std::string shadersFolder = getPath( getExecutableDirectory() ) / "share" / "Tutorial07" / "Shaders";

		if ( !wxFileExists( shadersFolder / "vert.spv" )
			|| !wxFileExists( shadersFolder / "frag.spv" ) )
		{
			throw std::runtime_error{ "Shader files are missing" };
		}

		m_program = std::make_unique< renderer::ShaderProgram >( *m_device );
		m_program->createModule( dumpBinaryFile( shadersFolder / "vert.spv" )
			, renderer::ShaderStageFlag::eVertex );
		m_program->createModule( dumpBinaryFile( shadersFolder / "frag.spv" )
			, renderer::ShaderStageFlag::eFragment );
		m_pipeline = std::make_shared< renderer::Pipeline >( *m_device
			, *m_pipelineLayout
			, *m_program
			, std::vector< std::reference_wrapper< renderer::VertexLayout const > >{ { std::ref( m_vertexBuffer->getLayout() ) } }
			, *m_renderPass
			, renderer::PrimitiveTopology::eTriangleStrip );
	}

	void RenderPanel::doPrepareFrames()
	{
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		m_commandBuffers = m_swapChain->createCommandBuffers();

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			wxSize size{ GetClientSize() };

			if ( commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT ) )
			{
				auto dimensions = m_swapChain->getDimensions();
				m_swapChain->preRenderCommands( i, commandBuffer );
				commandBuffer.beginRenderPass( *m_renderPass
					, frameBuffer
					, m_swapChain->getClearColour() );
				commandBuffer.bindPipeline( *m_pipeline );
				commandBuffer.setViewport( { uint32_t( dimensions.x )
					, uint32_t( dimensions.y )
					, 0
					, 0 } );
				commandBuffer.setScissor( { 0
					, 0
					, uint32_t( dimensions.x )
					, uint32_t( dimensions.y ) } );
				commandBuffer.bindVertexBuffer( *m_vertexBuffer
					, 0u );
				commandBuffer.bindDescriptorSet( *m_descriptorSet
					, *m_pipelineLayout );
				commandBuffer.draw( 4u, 1u, 0u, 0u );
				commandBuffer.endRenderPass();
				m_swapChain->postRenderCommands( i, commandBuffer );

				auto res = commandBuffer.end();

				if ( !res )
				{
					std::stringstream stream;
					stream << "Command buffers recording failed.";
					throw std::runtime_error{ stream.str() };
				}
			}
		}
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			auto & queue = m_device->getGraphicsQueue();
			auto res = queue.submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources );

			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
			std::cerr << "Can't render: " << vk::getLastError() << std::endl;
		}
	}

	void RenderPanel::doResetSwapChain()
	{
		m_device->getDevice().waitIdle();
		wxSize size{ GetClientSize() };
		m_swapChain->reset( { size.GetWidth(), size.GetHeight() } );
	}

	void RenderPanel::onTimer( wxTimerEvent & event )
	{
		if ( event.GetId() == int( Ids::RenderTimer ) )
		{
			doDraw();
		}
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_timer->Stop();
		doResetSwapChain();
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
