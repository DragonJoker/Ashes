#include "FileUtils.hpp"
#include "RendererPlugin.hpp"

#include <Core/Connection.hpp>
#include <Core/Device.hpp>
#include <Core/PlatformWindowHandle.hpp>
#include <Core/RenderingResources.hpp>
#include <Core/SwapChain.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassAttachment.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassAttachment.hpp>
#include <RenderPass/RenderSubpassState.hpp>

#include <GLFW/glfw3.h>

#if RENDERLIB_WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

std::vector< common::RendererPlugin > doListPlugins( common::RendererFactory & factory )
{
	common::StringArray files;
	std::vector< common::RendererPlugin > result;

	if ( common::listDirectoryFiles( common::getExecutableDirectory(), files, false ) )
	{
		for ( auto file : files )
		{
			if ( file.find( ".dll" ) != std::string::npos
				|| file.find( ".so" ) != std::string::npos )
				try
			{
				renderer::DynamicLibrary lib{ file };
				result.emplace_back( std::move( lib )
					, factory );
			}
			catch ( std::exception & exc )
			{
				std::clog << exc.what() << std::endl;
			}
		}
	}

	return result;
}

renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
	, renderer::SwapChain const & swapChain )
{
	// We'll have only one colour attachment for the render pass,
	// for which the format is the swapchain's pixel format,
	// and we want to clear it at the beginning of the render pass (\p true parameter).
	renderer::RenderPassAttachmentArray attaches
	{ 
		renderer::RenderPassAttachment::createColourAttachment( 0u, swapChain.getFormat(), true )
	};
	// A render pass always has at least one subpass.
	// In our case, this subpass is also the only one,
	// its only attachment is the render pass' one.
	// We want this attachment to be transitioned to colour attachment, so we can write into it.
	renderer::RenderSubpassAttachmentArray subAttaches
	{
		renderer::RenderSubpassAttachment{ attaches[0], renderer::ImageLayout::eColourAttachmentOptimal }
	};
	// We now create the subpasses.
	// The subpass state is used to setup the needed states at the beginning of the subpass.
	renderer::RenderSubpassPtrArray subpasses;
	subpasses.emplace_back( device.createRenderSubpass( subAttaches
		, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
			, renderer::AccessFlag::eColourAttachmentWrite } ) );
	// The initial layouts are the layouts expected for each attachment at the beginning of the render pass.
	// We expect the attach to have been presented to the surface, so it should be a present source.
	renderer::ImageLayoutArray initialLayouts
	{
		renderer::ImageLayout::ePresentSrc
	};
	// The final layouts are the layouts into which the attachments are transitioned at the end of the render pass.
	// We want the attach to be presented to the surface, so we make it a present source.
	renderer::ImageLayoutArray finalLayouts
	{
		renderer::ImageLayout::ePresentSrc
	};
	// Eventually, we create the render pass, using all previously built informations.
	return device.createRenderPass( attaches
		, std::move( subpasses )
		, renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
			, renderer::AccessFlag::eMemoryRead
			, initialLayouts }
		, renderer::RenderPassState{ renderer::PipelineStageFlag::eBottomOfPipe
			, renderer::AccessFlag::eMemoryRead
			, finalLayouts } );
}

void doPrepareFrames( renderer::SwapChain const & swapChain
	, renderer::RenderPass const & renderPass
	, std::vector< renderer::FrameBufferPtr > const & frameBuffers
	, renderer::CommandBufferPtrArray const & commandBuffers )
{
	// We'll simply clear the swap chain, using its colour defined previously.
	for ( size_t i = 0u; i < commandBuffers.size(); ++i )
	{
		auto & frameBuffer = *frameBuffers[i];
		auto & commandBuffer = *commandBuffers[i];

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.beginRenderPass( renderPass
				, frameBuffer
				, { swapChain.getClearColour() }
			, renderer::SubpassContents::eInline );
			commandBuffer.endRenderPass();

			if ( !commandBuffer.end() )
			{
				std::cerr << "Command buffers recording failed" << std::endl;
			}
		}
	}
}

int main( int argc, char * argv[] )
{
	// First, we need to retrieve the RendererLib plugins
	common::RendererFactory factory;
	auto plugins = doListPlugins( factory );

	// Then we check in the command line if the user has wanted a specific plugin to be used.
	std::string rendererName = "vk";

	if ( argc > 1 )
	{
		rendererName = argv[1];
		rendererName = rendererName.substr( rendererName.find( '-' ) + 1 );
		rendererName = rendererName.substr( rendererName.find( '/' ) + 1 );
	}

	// With that informations, we can already create the renderer instance.
	renderer::Renderer::Configuration config
	{
		"GLFW Template",
		"RendererLib",
#if !defined( NDEBUG )
		true,
#else
		false,
#endif
	};
	renderer::RendererPtr renderer = factory.create( rendererName, config );

	// Now we need a window.
	static constexpr uint32_t width = 800u;
	static constexpr uint32_t height = 600u;
	glfwInit();
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	GLFWwindow * window = glfwCreateWindow( int( width ), int( height ), "GLFW Template", nullptr, nullptr );

	// We retrieve this window's native handle, and create the logical device from it.
	auto hWnd = glfwGetWin32Window( window );
	auto handle = renderer::WindowHandle{ std::make_unique< renderer::IMswWindowHandle >( nullptr
		, hWnd ) };

	{
		renderer::DevicePtr device = renderer->createDevice( renderer->createConnection( 0u, std::move( handle ) ) );

		// This is the only line related to OpenGL : context activation.
		device->enable();

		// Retrieve the swapchain and set it up.
		renderer::SwapChainPtr swapChain = device->createSwapChain( { width, height } );
		swapChain->setClearColour( renderer::RgbaColour{ 1.0f, 0.8f, 0.4f, 0.0f } );

		// We retrieve the render pass that we'll be using to do our stuff on the swapchain surface.
		renderer::RenderPassPtr renderPass = doCreateRenderPass( *device, *swapChain );

		// We now retrieve the framebuffers and command buffers for each backbuffer of the swapchain.
		std::vector< renderer::FrameBufferPtr > frameBuffers = swapChain->createFrameBuffers( *renderPass );
		renderer::CommandBufferPtrArray commandBuffers = swapChain->createCommandBuffers();

		// From all those things, we can now prepare our frames (one per framebuffer).
		doPrepareFrames( *swapChain, *renderPass, frameBuffers, commandBuffers );

		// Here we connect to the swap chain reset signal, to handle resizes and other device lost problems.
		auto swapChainReset = swapChain->onReset.connect( [&swapChain, &renderPass, &frameBuffers, &commandBuffers]()
		{
			// We retrieve those once again, since the swap chain has been reset.
			frameBuffers = swapChain->createFrameBuffers( *renderPass );
			commandBuffers = swapChain->createCommandBuffers();
			// And we prepare our frames again.
			doPrepareFrames( *swapChain, *renderPass, frameBuffers, commandBuffers );
		} );

		while ( !glfwWindowShouldClose( window ) )
		{
			glfwPollEvents();

			// Acquire the next frame to present.
			auto resources = swapChain->getResources();

			if ( resources )
			{
				// Submit the command buffer to the graphics queue.
				auto res = device->getGraphicsQueue().submit( *commandBuffers[resources->getBackBuffer()]
					, resources->getImageAvailableSemaphore()
					, renderer::PipelineStageFlag::eColourAttachmentOutput
					, resources->getRenderingFinishedSemaphore()
					, &resources->getFence() );
				// And we present the frame to the swap chain surface.
				swapChain->present( *resources );
			}
			else
			{
				std::cerr << "Can't render" << std::endl;
			}
		}

		// Before destroying any resource, we need to make sure the device is idle.
		device->waitIdle();
		commandBuffers.clear();
		frameBuffers.clear();
		renderPass.reset();
		swapChain.reset();
		device->disable();
		device.reset();
	}

	return 0;
}
