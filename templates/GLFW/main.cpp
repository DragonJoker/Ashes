#include "FileUtils.hpp"
#include "RendererPlugin.hpp"

#include <Ashes/Core/Connection.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <Ashes/Core/RenderingResources.hpp>
#include <Ashes/Core/SwapChain.hpp>
#include <Ashes/RenderPass/FrameBuffer.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/AttachmentDescription.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>
#include <Ashes/RenderPass/AttachmentReference.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>

#include <GLFW/glfw3.h>

#if ASHES_WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#elif ASHES_XLIB
#	define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>

struct Application
{
	ashes::SwapChainPtr swapChain;
	std::vector< ashes::FrameBufferPtr > frameBuffers;
	ashes::CommandBufferPtrArray commandBuffers;
	ashes::RenderPassPtr renderPass;
};

std::string processCommandLine( int argc, char ** argv );
std::vector< common::RendererPlugin > doListPlugins( common::RendererFactory & factory );
ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
	, ashes::SwapChain const & swapChain );
void doPrepareFrames( Application & application );
void onWindowResized( GLFWwindow * window, int width, int height );

int main( int argc, char * argv[] )
{
	// First, we need to retrieve the Ashes plugins
	common::RendererFactory factory;
	auto plugins = doListPlugins( factory );

	// Then we check in the command line if the user has wanted a specific plugin to be used.
	std::string rendererName = processCommandLine( argc, argv );

	// With that informations, we can now create the renderer instance.
	ashes::Renderer::Configuration config
	{
		"GLFW Template",
		"Ashes",
#if !defined( NDEBUG )
		true,
#else
		false,
#endif
	};
	ashes::RendererPtr renderer = factory.create( rendererName, config );

	// Now we need a window.
	static constexpr uint32_t width = 800u;
	static constexpr uint32_t height = 600u;
	glfwInit();
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	GLFWwindow * window = glfwCreateWindow( int( width ), int( height ), ( "GLFW Template (" + renderer->getName() + ")" ).c_str(), nullptr, nullptr );
	Application app;
	glfwSetWindowUserPointer( window, &app );
	glfwSetWindowSizeCallback( window, onWindowResized );

#if ASHES_WIN32
	// We retrieve this window's native handle, and create the logical device from it.
	auto hWnd = glfwGetWin32Window( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IMswWindowHandle >( nullptr, hWnd ) };
#elif ASHES_XLIB
	auto display = glfwGetX11Display();
	auto drawable = glfwGetX11Window( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IXWindowHandle >( drawable, display ) };
#else
#	error "Unimplemented."
#endif
	ashes::DevicePtr device = renderer->createDevice( renderer->createConnection( 0u, std::move( handle ) ) );

	// Retrieve the swapchain and set it up.
	app.swapChain = device->createSwapChain( { width, height } );
	app.swapChain->setClearColour( ashes::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f } );

	// We retrieve the render pass that we'll be using to do our stuff on the swapchain surface.
	app.renderPass = doCreateRenderPass( *device, *app.swapChain );

	// From all those things, we can now prepare our frames (one per framebuffer).
	doPrepareFrames( app );

	// Here we connect to the swap chain reset signal, to handle resizes and other device lost problems.
	auto swapChainReset = app.swapChain->onReset.connect( [&device, &app]()
	{
		// We only prepare our frames again.
		doPrepareFrames( app );
	} );

	while ( !glfwWindowShouldClose( window ) )
	{
		glfwPollEvents();

		// Acquire the next frame to present.
		auto resources = app.swapChain->getResources();

		if ( resources )
		{
			// Submit the command buffer to the graphics queue.
			device->getGraphicsQueue().submit( *app.commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			// And we present the frame to the swap chain surface.
			app.swapChain->present( *resources );
		}
		else
		{
			std::cerr << "Can't render" << std::endl;
		}
	}

	// Before destroying any resource, we need to make sure the device is idle.
	device->waitIdle();
	app.commandBuffers.clear();
	app.frameBuffers.clear();
	app.renderPass.reset();
	app.swapChain.reset();
	device.reset();

	glfwTerminate();
	renderer.reset();
	return 0;
}

std::string processCommandLine( int argc, char ** argv )
{
	std::string result = "vk";

	if ( argc > 1 )
	{
		result = argv[1];
		result = result.substr( result.find( '-' ) + 1 );
		result = result.substr( result.find( '/' ) + 1 );
	}

	return result;
}

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
				ashes::DynamicLibrary lib{ file };
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

ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
	, ashes::SwapChain const & swapChain )
{
	ashes::AttachmentDescriptionArray attaches
	{
		// We'll have only one colour attachment for the render pass.
		{
			// The format is the swapchain's pixel format.
			swapChain.getFormat(),
			// Multisampling is disabled for this attach.
			ashes::SampleCountFlag::e1,
			// We want to clear the attach at the beginning of the render pass.
			ashes::AttachmentLoadOp::eClear,
			// And we want its result to be stored at the end of the render pass.
			ashes::AttachmentStoreOp::eStore,
			// We don't care about stencil attachment.
			ashes::AttachmentLoadOp::eDontCare,
			// We don't care about stencil attachment.
			ashes::AttachmentStoreOp::eDontCare,
			// The initial layout is the layout expected for the attachment at the beginning of the render pass.
			// We expect the attach to have been presented to the surface, so it should be either a present source or undefined.
			ashes::ImageLayout::eUndefined,
			// The final layout is the layouts into which the attachment is transitioned at the end of the render pass.
			// We want the attach to be presented to the surface, so we make it a present source.
			ashes::ImageLayout::ePresentSrc,
		}
	};
	// A render pass always has at least one subpass.
	// In our case, this subpass is also the only one,
	// its only attachment is the render pass' one.
	// We want this attachment to be transitioned to colour attachment, so we can write into it.
	ashes::AttachmentReferenceArray subAttaches
	{
		ashes::AttachmentReference{ 0u, ashes::ImageLayout::eColourAttachmentOptimal }
	};
	// We now create the subpasses.
	// The subpass state is used to setup the needed states at the beginning of the subpass.
	ashes::RenderSubpassPtrArray subpasses;
	subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( ashes::PipelineBindPoint::eGraphics
		, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
			, ashes::AccessFlag::eColourAttachmentWrite }
		, subAttaches ) );
	// Eventually, we create the render pass, using all previously built informations.
	return device.createRenderPass( attaches
		, std::move( subpasses )
		, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
			, ashes::AccessFlag::eMemoryRead }
		, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
			, ashes::AccessFlag::eMemoryRead } );
}

void doPrepareFrames( Application & application )
{
	// We retrieve the framebuffers and command buffers for each backbuffer of the swapchain.
	application.frameBuffers = application.swapChain->createFrameBuffers( *application.renderPass );
	application.commandBuffers = application.swapChain->createCommandBuffers();

	// We'll simply clear the swap chain, using its colour defined previously.
	for ( size_t i = 0u; i < application.commandBuffers.size(); ++i )
	{
		auto & frameBuffer = *application.frameBuffers[i];
		auto & commandBuffer = *application.commandBuffers[i];

		commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
		commandBuffer.beginRenderPass( *application.renderPass
			, frameBuffer
			, { application.swapChain->getClearColour() }
		, ashes::SubpassContents::eInline );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}
}

void onWindowResized( GLFWwindow * window, int width, int height )
{
	auto * app = reinterpret_cast< Application * >( glfwGetWindowUserPointer( window ) );
	app->swapChain->reset( { uint32_t( width ), uint32_t( height ) } );
}
