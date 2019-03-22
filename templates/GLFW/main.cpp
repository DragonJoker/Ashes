#include "FileUtils.hpp"
#include "RendererPlugin.hpp"

#include <Ashes/Command/CommandBuffer.hpp>
#include <Ashes/Command/CommandPool.hpp>
#include <Ashes/Command/Queue.hpp>
#include <Ashes/Core/DebugReportCallback.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Core/PlatformWindowHandle.hpp>
#include <Ashes/Core/Surface.hpp>
#include <Ashes/Core/SwapChain.hpp>
#include <Ashes/RenderPass/ClearValue.hpp>
#include <Ashes/RenderPass/FrameBuffer.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>
#include <Ashes/RenderPass/AttachmentDescription.hpp>
#include <Ashes/RenderPass/RenderSubpass.hpp>
#include <Ashes/RenderPass/AttachmentReference.hpp>
#include <Ashes/RenderPass/RenderSubpassState.hpp>
#include <Ashes/Sync/Fence.hpp>
#include <Ashes/Sync/Semaphore.hpp>

#include <GLFW/glfw3.h>

#if ASHES_WIN32
#	define GLFW_EXPOSE_NATIVE_WIN32
#elif ASHES_XLIB
#	define GLFW_EXPOSE_NATIVE_X11
#endif

#ifdef min
#	undef min
#endif
#ifdef max
#	undef max
#endif

#include <GLFW/glfw3native.h>

struct LayerExtensionList
{
	ashes::LayerProperties layerProperties;
	ashes::ExtensionPropertiesArray extensionProperties;
};
using LayerExtensionListArray = std::vector< LayerExtensionList >;

struct RenderingResources
{
	RenderingResources( ashes::SemaphorePtr imageAvailableSemaphore
		, ashes::SemaphorePtr finishedRenderingSemaphore
		, ashes::FencePtr fence
		, ashes::CommandBufferPtr commandBuffer
		, uint32_t imageIndex )
		: imageAvailableSemaphore{ std::move( imageAvailableSemaphore ) }
		, finishedRenderingSemaphore{ std::move( finishedRenderingSemaphore ) }
		, fence{ std::move( fence ) }
		, commandBuffer{ std::move( commandBuffer ) }
		, imageIndex{ imageIndex }
	{
	}

	ashes::SemaphorePtr imageAvailableSemaphore;
	ashes::SemaphorePtr finishedRenderingSemaphore;
	ashes::FencePtr fence;
	ashes::CommandBufferPtr commandBuffer;
	uint32_t imageIndex{ 0u };
};
using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
using RenderingResourcesArray = std::vector< RenderingResourcesPtr >;

struct Application
{
	uint32_t presentQueueFamilyIndex;
	uint32_t graphicsQueueFamilyIndex;
	uint32_t computeQueueFamilyIndex;
	ashes::DevicePtr device;
	ashes::Extent2D dimensions;
	ashes::SwapChainPtr swapChain;
	std::vector< ashes::FrameBufferPtr > frameBuffers;
	ashes::CommandPoolPtr commandPool;
	ashes::CommandBufferPtrArray commandBuffers;
	ashes::RenderPassPtr renderPass;
	ashes::ClearColorValue clearColour;
	ashes::QueuePtr graphicsQueue;
	ashes::QueuePtr presentQueue;
	RenderingResourcesArray renderingResources;
	uint32_t resourceIndex{ 0u };
};

std::string processCommandLine( int argc, char ** argv );
std::vector< common::RendererPlugin > listPlugins();
ashes::DeviceCreateInfo getDeviceCreateInfo( ashes::Instance const & instance
	, ashes::Surface const & surface
	, ashes::PhysicalDevice const & gpu
	, uint32_t & presentQueueFamilyIndex
	, uint32_t & graphicsQueueFamilyIndex
	, uint32_t & computeQueueFamilyIndex );
void createSwapChain( Application & application );
ashes::RenderPassPtr createRenderPass( ashes::Device const & device
	, ashes::SwapChain const & swapChain );
void prepareFrames( Application & application );
RenderingResources * getResources( Application & application );
bool checkNeedReset( Application & application
	, ashes::Result errCode
	, bool acquisition
	, char const * const action );
void onWindowResized( GLFWwindow * window, int width, int height );
static bool ASHES_API DbgCallback( ashes::DebugReportFlags msgFlags
	, ashes::DebugReportObjectType objType
	, uint64_t srcObject
	, size_t location
	, int32_t msgCode
	, const char *pLayerPrefix
	, const char * pMsg
	, void * pUserData );

int main( int argc, char * argv[] )
{
	// First, we need to retrieve the Ashes plugins
	common::RendererFactory factory;
	auto plugins = listPlugins();

	for ( auto & plugin : plugins )
	{
		factory.registerType( plugin.getShortName(), &plugin );
	}

	// Then we check in the command line if the user has wanted a specific plugin to be used.
	std::string rendererName = processCommandLine( argc, argv );

	auto plugin = factory.findPlugin( rendererName );

	auto globalLayerProperties = plugin->enumerateLayerProperties();
	ashes::ExtensionPropertiesArray globalExtensions;
	LayerExtensionListArray globalLayers( globalLayerProperties.size() );

	for ( uint32_t i = 0; i < globalLayerProperties.size(); ++i )
	{
		ashes::LayerProperties & srcInfo = globalLayerProperties[i];
		auto & dstInfo = globalLayers[i];
		dstInfo.layerProperties = srcInfo;

		// Gets layer extensions, if first parameter is not NULL
		dstInfo.extensionProperties = plugin->enumerateExtensionProperties( srcInfo.layerName );
	}

	// Gets instance extensions, if no layer was specified in the first paramteter
	globalExtensions = plugin->enumerateExtensionProperties( std::string{} );

	const ashes::ApplicationInfo appInfo
	{
		"Ashes GLFW Template Application",
		ashes::makeVersion( 1, 0, 0 ),
		"GLFWTemplate",
		ashes::makeVersion( 1, 0, 0 ),
		ashes::API_VERSION_1_0,
	};

	// Get all supported Instance extensions (excl. layer-provided ones)
	ashes::StringArray instanceExtensions;

	for ( auto & ext : globalExtensions )
	{
		instanceExtensions.push_back( ext.extensionName );
	}

	const ashes::InstanceCreateInfo instInfo
	{
		0,
		appInfo,
		{},
		instanceExtensions,
	};

	// With that informations, we can now create the renderer instance.
	ashes::InstancePtr instance = plugin->create( std::move( instInfo ) );

	const ashes::DebugReportCallbackCreateInfo dbg_info
	{
		ashes::DebugReportFlag::eError | ashes::DebugReportFlag::eWarning,
		DbgCallback,
		nullptr,
	};
	ashes::DebugReportCallbackPtr debugReport = instance->createDebugReportCallback( std::move( dbg_info ) );

	ashes::PhysicalDevicePtrArray gpus = instance->enumeratePhysicalDevices();

	// Now we need a window.
	static constexpr uint32_t width = 800u;
	static constexpr uint32_t height = 600u;
	glfwInit();
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	GLFWwindow * window = glfwCreateWindow( int( width ), int( height ), ( "GLFW Template (" + instance->getName() + ")" ).c_str(), nullptr, nullptr );
	Application app;
	glfwSetWindowUserPointer( window, &app );
	glfwSetWindowSizeCallback( window, onWindowResized );

	// We retrieve this window's native handle, and create the surface from it.
#if ASHES_WIN32
	auto hWnd = glfwGetWin32Window( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IMswWindowHandle >( nullptr, hWnd ) };
#elif ASHES_XLIB
	auto display = glfwGetX11Display();
	auto drawable = glfwGetX11Window( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IXWindowHandle >( drawable, display ) };
#else
#	error "Unimplemented."
#endif
	ashes::SurfacePtr surface = instance->createSurface( *gpus[0], std::move( handle ) );

	// We now create the logical device, using this surface
	ashes::DeviceCreateInfo deviceInfo = getDeviceCreateInfo( *instance
		, *surface
		, *gpus[0]
		, app.presentQueueFamilyIndex
		, app.graphicsQueueFamilyIndex
		, app.computeQueueFamilyIndex );
	app.device = instance->createDevice( std::move( surface ), std::move( deviceInfo ) );
	app.graphicsQueue = app.device->getQueue( app.graphicsQueueFamilyIndex, 0u );
	app.presentQueue = app.device->getQueue( app.presentQueueFamilyIndex, 0u );
	app.commandPool = app.device->createCommandPool( app.graphicsQueueFamilyIndex
		, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );

	// Create the swapchain and set it up.
	app.dimensions.width = width;
	app.dimensions.height = height;
	createSwapChain( app );

	// We retrieve the render pass that we'll be using to do our stuff on the swapchain surface.
	app.renderPass = createRenderPass( *app.device, *app.swapChain );

	// From all those things, we can now prepare our frames (one per framebuffer).
	prepareFrames( app );

	// And let's roll !!
	while ( !glfwWindowShouldClose( window ) )
	{
		glfwPollEvents();

		// Acquire the next frame to present.
		auto resources = getResources( app );

		if ( resources )
		{
			// Submit the command buffer to the graphics queue.
			app.graphicsQueue->submit( *app.commandBuffers[resources->imageIndex]
				, *resources->imageAvailableSemaphore
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, *resources->finishedRenderingSemaphore
				, resources->fence.get() );

			try
			{
				// And we present the frame to the swap chain surface.
				auto res = app.presentQueue->present( { *app.swapChain }
					, { resources->imageIndex }
				, { *resources->finishedRenderingSemaphore } );
			}
			catch ( ashes::Exception & exc )
			{
				// Swapchain reset management.
				checkNeedReset( app
					, exc.getResult()
					, false
					, "Image presentation" );
			}

			resources->imageIndex = ~0u;
		}
	}

	// Before destroying any resource, we need to make sure the device is idle.
	app.device->waitIdle();
	app.commandBuffers.clear();
	app.frameBuffers.clear();
	app.renderPass.reset();
	app.renderingResources.clear();
	app.swapChain.reset();
	app.graphicsQueue.reset();
	app.presentQueue.reset();
	app.commandPool.reset();
	app.device.reset();

	glfwTerminate();
	instance.reset();
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

std::vector< common::RendererPlugin > listPlugins()
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
				result.emplace_back( std::move( lib ) );
			}
			catch ( std::exception & exc )
			{
				std::clog << exc.what() << std::endl;
			}
		}
	}

	return result;
}

void doInitialiseQueueFamilies( ashes::Instance const & instance
	, ashes::Surface const & surface
	, ashes::PhysicalDevice const & gpu
	, uint32_t & presentQueueFamilyIndex
	, uint32_t & graphicsQueueFamilyIndex
	, uint32_t & computeQueueFamilyIndex )
{
	// Parcours des propriétés des files, pour vérifier leur support de la présentation.
	auto queueProps = gpu.getQueueFamilyProperties();
	std::vector< uint32_t > supportsPresent( static_cast< uint32_t >( queueProps.size() ) );
	uint32_t i{ 0u };
	graphicsQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
	presentQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
	computeQueueFamilyIndex = std::numeric_limits< uint32_t >::max();

	for ( auto & present : supportsPresent )
	{
		present = surface.getSupport( i );

		if ( queueProps[i].queueCount > 0 )
		{
			if ( queueProps[i].queueFlags & ashes::QueueFlag::eGraphics )
			{
				// Tout d'abord on choisit une file graphique
				if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
				{
					graphicsQueueFamilyIndex = i;
				}

				// Si la file supporte aussi les calculs, on la choisit en compute queue
				if ( queueProps[i].queueFlags & ashes::QueueFlag::eCompute
					&& computeQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
				{
					computeQueueFamilyIndex = i;
				}

				// Si une file supporte les graphismes et la présentation, on la préfère.
				if ( present )
				{
					graphicsQueueFamilyIndex = i;
					presentQueueFamilyIndex = i;
					break;
				}
			}
			else if ( queueProps[i].queueFlags & ashes::QueueFlag::eCompute
				&& computeQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
			{
				computeQueueFamilyIndex = i;
			}

		}

		++i;
	}

	if ( presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
	{
		// Pas de file supportant les deux, on a donc 2 files distinctes.
		for ( size_t i = 0; i < queueProps.size(); ++i )
		{
			if ( supportsPresent[i] )
			{
				presentQueueFamilyIndex = static_cast< uint32_t >( i );
				break;
			}
		}
	}

	// Si on n'en a pas trouvé, on génère une erreur.
	if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max()
		|| presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max()
		|| computeQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
	{
		throw ashes::Exception{ ashes::Result::eErrorInitializationFailed
			, "Queue families retrieval" };
	}
}

ashes::DeviceCreateInfo getDeviceCreateInfo( ashes::Instance const & instance
	, ashes::Surface const & surface
	, ashes::PhysicalDevice const & gpu
	, uint32_t & presentQueueFamilyIndex
	, uint32_t & graphicsQueueFamilyIndex
	, uint32_t & computeQueueFamilyIndex )
{
	doInitialiseQueueFamilies( instance
		, surface
		, gpu
		, presentQueueFamilyIndex
		, graphicsQueueFamilyIndex
		, computeQueueFamilyIndex );
	std::vector< float > queuePriorities = { 1.0f };
	ashes::DeviceCreateInfo result;

	if ( graphicsQueueFamilyIndex != uint32_t( ~( 0u ) ) )
	{
		result.queueCreateInfos.push_back(
			{
				ashes::DeviceQueueCreateFlag::eNone,
				graphicsQueueFamilyIndex,
				queuePriorities,
			} );
	}

	if ( presentQueueFamilyIndex != graphicsQueueFamilyIndex )
	{
		result.queueCreateInfos.push_back(
			{
				ashes::DeviceQueueCreateFlag::eNone,
				presentQueueFamilyIndex,
				queuePriorities,
			} );
	}

	if ( computeQueueFamilyIndex != graphicsQueueFamilyIndex )
	{
		result.queueCreateInfos.push_back(
			{
				ashes::DeviceQueueCreateFlag::eNone,
				computeQueueFamilyIndex,
				queuePriorities,
			} );
	}

	result.enabledFeatures = gpu.getFeatures();
	result.enabledLayerNames = instance.getEnabledLayerNames();
	result.enabledExtensionNames.push_back( ashes::KHR_SWAPCHAIN_EXTENSION_NAME );
	return result;
}
uint32_t doGetImageCount( ashes::Device const & device )
{
	auto & surface = device.getSurface();
	auto surfaceCaps = surface.getCapabilities();
	uint32_t desiredNumberOfSwapChainImages{ surfaceCaps.minImageCount + 1 };

	if ( ( surfaceCaps.maxImageCount > 0 ) &&
		( desiredNumberOfSwapChainImages > surfaceCaps.maxImageCount ) )
	{
		// L'application doit s'initialiser avec moins d'images que voulu.
		desiredNumberOfSwapChainImages = surfaceCaps.maxImageCount;
	}

	return desiredNumberOfSwapChainImages;
}

ashes::SurfaceFormat doSelectFormat( ashes::Device const & device )
{
	auto & surface = device.getSurface();
	ashes::SurfaceFormat result;
	auto formats = surface.getFormats();
	// Si la liste de formats ne contient qu'une entr�e VK_FORMAT_UNDEFINED,
	// la surface n'a pas de format préféré. Sinon, au moins un format supporté
	// sera renvoyé.
	if ( formats.size() == 1u && formats[0].format == ashes::Format::eUndefined )
	{
		result.format = ashes::Format::eR8G8B8A8_UNORM;
		result.colorSpace = ashes::ColorSpace::eSRGBNonLinear;
	}
	else
	{
		assert( formats.size() > 1u );
		auto it = std::find_if( formats.begin()
			, formats.end()
			, []( ashes::SurfaceFormat const & lookup )
			{
				return lookup.format == ashes::Format::eR8G8B8A8_UNORM;
			} );

		if ( it != formats.end() )
		{
			result = *it;
		}
		else
		{
			result = formats[0];
		}
	}

	return result;
}

ashes::PresentMode doSelectPresentMode( ashes::Device const & device )
{
	auto & surface = device.getSurface();
	auto presentModes = surface.getPresentModes();
	// Si le mode boîte aux lettres est disponible, on utilise celui-là, car c'est celui avec le
	// minimum de latence dans tearing.
	// Sinon, on essaye le mode IMMEDIATE, qui est normalement disponible, et est le plus rapide
	// (bien qu'il y ait du tearing). Sinon on utilise le mode FIFO qui est toujours disponible.
	ashes::PresentMode result{ ashes::PresentMode::eFifo };

	for ( auto mode : presentModes )
	{
		if ( mode == ashes::PresentMode::eMailbox )
		{
			result = mode;
			break;
		}

		if ( ( result != ashes::PresentMode::eMailbox )
			&& ( mode == ashes::PresentMode::eImmediate ) )
		{
			result = mode;
		}
	}

	return result;
}

ashes::SwapChainCreateInfo doGetSwapChainCreateInfo( ashes::Device const & device
	, ashes::Extent2D const & size )
{
	ashes::Extent2D swapChainExtent{};
	auto & surface = device.getSurface();
	auto surfaceCaps = surface.getCapabilities();

	// width et height valent soient tous les deux -1 ou tous les deux autre chose que -1.
	if ( surfaceCaps.currentExtent.width == uint32_t( -1 ) )
	{
		// Si les dimensions de la surface sont indéfinies, elles sont initialisées
		// aux dimensions des images requises.
		swapChainExtent = size;
	}
	else
	{
		// Si les dimensions de la surface sont définies, alors les dimensions de la swap chain
		// doivent correspondre.
		swapChainExtent = surfaceCaps.currentExtent;
	}

	// Parfois, les images doivent être transformées avant d'être présentées (lorsque l'orientation
	// du périphérique est différente de l'orientation par défaut, par exemple).
	// Si la transformation spécifiée est différente de la transformation par défaut, le moteur de 
	// présentation va transformer l'image lors de la présentation. Cette opération peut avoir un
	// impact sur les performances sur certaines plateformes.
	// Ici, nous ne voulons aucune transformation, donc si la transformation identité est supportée,
	// nous l'utilisons, sinon nous utiliserons la même transformation que la transformation courante.
	ashes::SurfaceTransformFlag preTransform{};

	if ( checkFlag( surfaceCaps.supportedTransforms, ashes::SurfaceTransformFlag::eIdentity ) )
	{
		preTransform = ashes::SurfaceTransformFlag::eIdentity;
	}
	else
	{
		preTransform = surfaceCaps.currentTransform;
	}

	auto presentMode = doSelectPresentMode( device );
	auto surfaceFormat = doSelectFormat( device );
	return ashes::SwapChainCreateInfo
	{
		ashes::SwapChainCreateFlag::eNone,
		std::ref( surface ),
		doGetImageCount( device ),
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		swapChainExtent,
		1u,
		surfaceCaps.supportedUsageFlags,
		ashes::SharingMode::eExclusive,
		{},
		preTransform,
		ashes::CompositeAlphaFlag::eOpaque,
		presentMode,
		false,
		ashes::nullopt
	};
}

void doCreateRenderingResources( Application & application )
{
	application.renderingResources.clear();

	for ( uint32_t i = 0u; i < uint32_t( application.swapChain->getImages().size() ); ++i )
	{
		application.renderingResources.emplace_back( std::make_unique< RenderingResources >( application.device->createSemaphore()
			, application.device->createSemaphore()
			, application.device->createFence( ashes::FenceCreateFlag::eSignaled )
			, application.commandPool->createCommandBuffer()
			, 0u ) );
	}
}

void createSwapChain( Application & application )
{
	application.swapChain = application.device->createSwapChain( doGetSwapChainCreateInfo( *application.device, application.dimensions ) );
	application.clearColour = ashes::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f };
	doCreateRenderingResources( application );
}

ashes::RenderPassPtr createRenderPass( ashes::Device const & device
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

ashes::FrameBufferAttachmentArray doPrepareAttaches( Application const & application
	, uint32_t backBuffer )
{
	ashes::FrameBufferAttachmentArray attaches;

	for ( auto & attach : application.renderPass->getAttachments() )
	{
		if ( !ashes::isDepthOrStencilFormat( attach.format ) )
		{
			attaches.emplace_back( attach, application.swapChain->getImages()[backBuffer]->getView() );
		}
		else
		{
			attaches.emplace_back( attach, application.swapChain->getDepthStencilView() );
		}
	}

	return attaches;
}

void doCreateFrameBuffers( Application & application )
{
	application.frameBuffers.resize( application.swapChain->getImages().size() );

	for ( size_t i = 0u; i < application.frameBuffers.size(); ++i )
	{
		auto attaches = doPrepareAttaches( application, uint32_t( i ) );
		application.frameBuffers[i] = application.renderPass->createBackBuffer( application.swapChain->getDimensions()
			, std::move( attaches ) );
	}
}

void doCreateCommandBuffers( Application & application )
{
	application.commandBuffers.resize( application.swapChain->getImages().size() );

	for ( auto & commandBuffer : application.commandBuffers )
	{
		commandBuffer = application.commandPool->createCommandBuffer( true );
	}
}

void prepareFrames( Application & application )
{
	// We retrieve the framebuffers and command buffers for each backbuffer of the swapchain.
	doCreateFrameBuffers( application );
	doCreateCommandBuffers( application );

	// We'll simply clear the swap chain, using its colour defined previously.
	for ( size_t i = 0u; i < application.commandBuffers.size(); ++i )
	{
		auto & frameBuffer = *application.frameBuffers[i];
		auto & commandBuffer = *application.commandBuffers[i];

		commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
		commandBuffer.beginRenderPass( *application.renderPass
			, frameBuffer
			, { application.clearColour }
		, ashes::SubpassContents::eInline );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}
}

RenderingResources * getResources( Application & application )
{
	auto & resources = *application.renderingResources[application.resourceIndex];
	application.resourceIndex = ( application.resourceIndex + 1 ) % application.renderingResources.size();
	bool res = resources.fence->wait( ashes::FenceTimeout ) == ashes::WaitResult::eSuccess;

	if ( res )
	{
		resources.fence->reset();
		uint32_t imageIndex{ 0u };
		auto res = application.swapChain->acquireNextImage( std::numeric_limits< uint64_t >::max()
			, *resources.imageAvailableSemaphore
			, imageIndex );

		if ( checkNeedReset( application
			, ashes::Result( res )
			, true
			, "Swap chain image acquisition" ) )
		{
			resources.imageIndex = imageIndex;
			return &resources;
		}

		return nullptr;
	}

	ashes::Logger::logError( "Couldn't retrieve rendering resources" );
	return nullptr;
}

void doResetSwapChain( Application & application )
{
	application.device->waitIdle();
	createSwapChain( application );
	prepareFrames( application );
}

bool checkNeedReset( Application & application
	, ashes::Result errCode
	, bool acquisition
	, char const * const action )
{
	bool result{ false };

	switch ( errCode )
	{
	case ashes::Result::eSuccess:
		result = true;
		break;

	case ashes::Result::eErrorOutOfDate:
		if ( !acquisition )
		{
			doResetSwapChain( application );
		}
		else
		{
			result = true;
		}
		break;

	case ashes::Result::eSubOptimal:
		doResetSwapChain( application );
		break;

	default:
		throw ashes::Exception{ errCode, action };
		break;
	}

	return result;
}

void onWindowResized( GLFWwindow * window, int width, int height )
{
	auto * app = reinterpret_cast< Application * >( glfwGetWindowUserPointer( window ) );
	app->dimensions.width = width;
	app->dimensions.height = height;
	doResetSwapChain( *app );
}

static bool ASHES_API DbgCallback( ashes::DebugReportFlags msgFlags
	, ashes::DebugReportObjectType objType
	, uint64_t srcObject
	, size_t location
	, int32_t msgCode
	, const char * pLayerPrefix
	, const char *pMsg
	, void *pUserData )
{
	std::cerr << msgFlags << ": [" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg << std::endl;

	// True is reserved for layer developers, and MAY mean calls are not distributed down the layer chain after validation error.
	// False SHOULD always be returned by apps:
	return false;
}
