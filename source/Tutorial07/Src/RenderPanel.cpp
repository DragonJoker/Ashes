#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"
#include "FileUtils.hpp"

#include <Renderer/Connection.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/Device.hpp>
#include <Renderer/ImageMemoryBarrier.hpp>
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

#include <VkLib/BackBuffer.hpp>
//#include <Renderer/Queue.hpp>
#include <VkLib/RenderPass.hpp>
#include <VkLib/RenderSubpass.hpp>

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
		std::vector< VkFormat > formats{ { m_swapChain->getSwapChain().getFormat() } };
		vk::RenderSubpass subpass{ m_device->getDevice()
			, formats
			, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
		};
		m_renderPass = m_device->getDevice().createRenderPass( formats
			, { subpass }
			, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			, { VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } }
			, { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			, { VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } } );
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

	void RenderPanel::doCreateFrameBuffer( vk::ImageView const & view
		, renderer::RenderingResources & resources )
	{
		wxSize size{ GetClientSize() };
		resources.setFrameBuffer( m_renderPass->createCompatibleFrameBuffer( size.x
			, size.y
			, { view } ) );
	}

	bool RenderPanel::doPrepareFrame( renderer::RenderingResources & resources )
	{
		bool result{ false };
		auto & backBuffer = resources.getBackBuffer();
		doCreateFrameBuffer( backBuffer.getImage().getView()
			, resources );
		auto & commandBuffer = resources.getCommandBuffer();

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			auto dimensions = m_swapChain->getDimensions();
			commandBuffer.getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, backBuffer.getImage().makeColourAttachment() );
			m_renderPass->begin( commandBuffer.getCommandBuffer()
				, resources.getFrameBuffer()
				, backBuffer.getClearColour() );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( { uint32_t(  dimensions.x )
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
			m_renderPass->end( commandBuffer.getCommandBuffer() );
			commandBuffer.getCommandBuffer().memoryBarrier( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
				, backBuffer.getImage().makePresentSource() );

			result = commandBuffer.end();

			if ( !result )
			{
				std::cerr << "Command buffers recording failed: " << vk::getLastError() << std::endl;
			}
		}

		return result;
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();

			if ( doPrepareFrame( *resources ) )
			{
				m_swapChain->present( *resources );
			}

			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
			std::cerr << "Can't render: " << vk::getLastError() << std::endl;
		}
	}

	bool RenderPanel::doCheckNeedReset( VkResult errCode
		, bool acquisition
		, char const * const action )
	{
		bool result{ false };

		switch ( errCode )
		{
		case VK_SUCCESS:
			result = true;
			break;

		case VK_ERROR_OUT_OF_DATE_KHR:
			if ( !acquisition )
			{
				doResetSwapChain();
			}
			else
			{
				result = true;
			}
			break;

		case VK_SUBOPTIMAL_KHR:
			doResetSwapChain();
			break;

		default:
			std::cerr << action << " failed: " << vk::getLastError() << std::endl;
			break;
		}

		return result;
	}

	void RenderPanel::doResetSwapChain()
	{
		m_device->getDevice().waitIdle();
		m_swapChain.reset();
		doCreateSwapChain();
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
