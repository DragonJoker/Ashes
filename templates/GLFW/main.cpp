#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Command/CommandPool.hpp>
#include <ashespp/Command/Queue.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/RendererList.hpp>
#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <ashespp/Sync/Fence.hpp>
#include <ashespp/Sync/Semaphore.hpp>

#include <ashespp/Core/PlatformWindowHandle.hpp>

#include <ashes/common/Exception.hpp>

#include <GLFW/glfw3.h>

#if defined( VK_USE_PLATFORM_WIN32_KHR )
#	define GLFW_EXPOSE_NATIVE_WIN32
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
#	define GLFW_EXPOSE_NATIVE_X11
#elif defined( VK_USE_PLATFORM_MACOS_MVK )
#	define GLFW_EXPOSE_NATIVE_COCOA
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
	VkLayerProperties layerProperties;
	ashes::VkExtensionPropertiesArray extensionProperties;
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
	ashes::SurfacePtr surface;
	ashes::DevicePtr device;
	VkExtent2D dimensions;
	ashes::SwapChainPtr swapChain;
	ashes::ImageArray swapChainImages;
	ashes::ImageViewArray views;
	std::vector< ashes::FrameBufferPtr > frameBuffers;
	ashes::CommandPoolPtr commandPool;
	ashes::CommandBufferPtrArray commandBuffers;
	ashes::RenderPassPtr renderPass;
	VkClearColorValue clearColour;
	ashes::QueuePtr graphicsQueue;
	ashes::QueuePtr presentQueue;
	RenderingResourcesArray renderingResources;
	uint32_t resourceIndex{ 0u };
};

std::string processCommandLine( int argc, char ** argv );
std::vector< VkLayerProperties > enumerateLayerProperties( PFN_vkEnumerateInstanceLayerProperties enumLayerProperties );
std::vector< VkExtensionProperties > enumerateExtensionProperties( PFN_vkEnumerateInstanceExtensionProperties enumInstanceExtensionProperties
	, std::string const & layerName );
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
	, VkResult errCode
	, bool acquisition
	, char const * const action );
void onWindowResized( GLFWwindow * window, int width, int height );
static VkBool32 VKAPI_PTR DbgCallback( VkDebugReportFlagsEXT msgFlags
	, VkDebugReportObjectTypeEXT objType
	, uint64_t srcObject
	, size_t location
	, int32_t msgCode
	, const char *pLayerPrefix
	, const char * pMsg
	, void * pUserData );

int main( int argc, char * argv[] )
{
	// First, we need to retrieve the Ashes plugins
	ashes::RendererList renderers;

	// Then we check in the command line if the user has wanted a specific plugin to be used.
	AshPluginDescription plugin = renderers.getSelectedPlugin();
	std::string rendererName = processCommandLine( argc, argv );

	if ( !rendererName.empty() )
	{
		try
		{
			plugin = renderers.selectPlugin( rendererName );
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
			return EXIT_FAILURE;
		}
	}

	PFN_vkEnumerateInstanceLayerProperties enumLayerProperties;
	enumLayerProperties = ( PFN_vkEnumerateInstanceLayerProperties )plugin.getInstanceProcAddr( VK_NULL_HANDLE,
		"vkEnumerateInstanceLayerProperties" );
	PFN_vkEnumerateInstanceExtensionProperties enumInstanceExtensionProperties;
	enumInstanceExtensionProperties = ( PFN_vkEnumerateInstanceExtensionProperties )plugin.getInstanceProcAddr( VK_NULL_HANDLE,
		"vkEnumerateInstanceExtensionProperties" );

	auto globalLayerProperties = enumerateLayerProperties( enumLayerProperties );
	ashes::VkExtensionPropertiesArray globalExtensions;
	LayerExtensionListArray globalLayers( globalLayerProperties.size() );

	for ( uint32_t i = 0; i < globalLayerProperties.size(); ++i )
	{
		VkLayerProperties & srcInfo = globalLayerProperties[i];
		auto & dstInfo = globalLayers[i];
		dstInfo.layerProperties = srcInfo;

		// Gets layer extensions, if first parameter is not NULL
		dstInfo.extensionProperties = enumerateExtensionProperties( enumInstanceExtensionProperties
			, srcInfo.layerName );
	}

	// Gets instance extensions, if no layer was specified in the first paramteter
	globalExtensions = enumerateExtensionProperties( enumInstanceExtensionProperties
		, std::string{} );

	ashes::ApplicationInfo appInfo
	{
		"Ashes GLFW Template Application",
		ashes::makeVersion( 1, 0, 0 ),
		"GLFWTemplate",
		ashes::makeVersion( 1, 0, 0 ),
		VK_API_VERSION_1_0,
	};

	// Get all supported Instance extensions (excl. layer-provided ones)
	ashes::StringArray instanceExtensions;

	for ( auto & ext : globalExtensions )
	{
		instanceExtensions.push_back( ext.extensionName );
	}

	ashes::InstanceCreateInfo instInfo
	{
		0u,
		std::move( appInfo ),
		{},
		instanceExtensions,
	};

	// With that informations, we can now create the instance.
	ashes::InstancePtr instance = std::make_unique< ashes::Instance >( std::move( plugin )
		, std::move( instInfo ) );

	VkDebugReportCallbackCreateInfoEXT dbg_info
	{
		VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
		nullptr,
		VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
		DbgCallback,
		nullptr,
	};
	VkDebugReportCallbackEXT debugReport = instance->createDebugReportCallback( dbg_info );

	ashes::PhysicalDevicePtrArray gpus = instance->enumeratePhysicalDevices();

	// Now we need a window.
	static constexpr uint32_t width = 800u;
	static constexpr uint32_t height = 600u;
	glfwInit();
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	GLFWwindow * window = glfwCreateWindow( int( width )
		, int( height )
		, ( "GLFW Template (" + rendererName + ")" ).c_str()
		, nullptr
		, nullptr );
	Application app;
	glfwSetWindowUserPointer( window, &app );
	glfwSetWindowSizeCallback( window, onWindowResized );

	// We retrieve this window's native handle, and create the surface from it.
#if defined( VK_USE_PLATFORM_WIN32_KHR )
	auto hWnd = glfwGetWin32Window( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IMswWindowHandle >( nullptr, hWnd ) };
#elif defined( VK_USE_PLATFORM_MACOS_MVK )
	auto view = glfwGetCocoaWindow( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IMacOsWindowHandle >( view ) };
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
	auto display = glfwGetX11Display();
	auto drawable = glfwGetX11Window( window );
	auto handle = ashes::WindowHandle{ std::make_unique< ashes::IXWindowHandle >( drawable, display ) };
#else
#	error "Unimplemented."
#endif
	app.surface = instance->createSurface( *gpus[0], std::move( handle ) );

	// We now create the logical device, using this surface
	ashes::DeviceCreateInfo deviceInfo = getDeviceCreateInfo( *instance
		, *app.surface
		, *gpus[0]
		, app.presentQueueFamilyIndex
		, app.graphicsQueueFamilyIndex
		, app.computeQueueFamilyIndex );
	app.device = instance->createDevice( app.surface->getGpu(), std::move( deviceInfo ) );
	app.graphicsQueue = app.device->getQueue( app.graphicsQueueFamilyIndex, 0u );
	app.presentQueue = app.device->getQueue( app.presentQueueFamilyIndex, 0u );
	app.commandPool = app.device->createCommandPool( app.graphicsQueueFamilyIndex
		, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT );

	// Create the swapchain and set it up.
	app.dimensions.width = width;
	app.dimensions.height = height;
	createSwapChain( app );

	// We retrieve the render pass that we'll be using to do our stuff on the swapchain surface.
	app.renderPass = createRenderPass( *app.device
		, *app.swapChain );

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
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, *resources->finishedRenderingSemaphore
				, resources->fence.get() );

			try
			{
				// And we present the frame to the swap chain surface.
				auto res = app.presentQueue->present( *app.swapChain
					, resources->imageIndex
					, *resources->finishedRenderingSemaphore );
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
	app.swapChainImages.clear();
	app.swapChain.reset();
	app.graphicsQueue.reset();
	app.presentQueue.reset();
	app.commandPool.reset();
	app.device.reset();
	app.surface.reset();

	glfwTerminate();
	instance.reset();
	return 0;
}

std::string processCommandLine( int argc, char ** argv )
{
	std::string result;

	if ( argc > 1 )
	{
		result = argv[1];
		result = result.substr( result.find( '-' ) + 1 );
		result = result.substr( result.find( '/' ) + 1 );
	}

	return result;
}

std::vector< VkLayerProperties > enumerateLayerProperties( PFN_vkEnumerateInstanceLayerProperties enumLayerProperties )
{
	if ( !enumLayerProperties )
	{
		return {};
	}

	uint32_t count;
	std::vector< VkLayerProperties > result;
	VkResult res;

	do
	{
		res = enumLayerProperties( &count, nullptr );

		if ( count )
		{
			result.resize( count );
			res = enumLayerProperties( &count, result.data() );
		}
	}
	while ( res == VK_INCOMPLETE );

	if ( res != VK_SUCCESS )
	{
		throw ashes::Exception{ res, "Instance layers retrieval" };
	}

	return result;
}

std::vector< VkExtensionProperties > enumerateExtensionProperties( PFN_vkEnumerateInstanceExtensionProperties enumInstanceExtensionProperties
	, std::string const & layerName )
{
	std::vector< VkExtensionProperties > result;
	VkResult res;

	do
	{
		uint32_t count{ 0u };
		res = enumInstanceExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
			, &count
			, nullptr );

		if ( count )
		{
			result.resize( count );
			res = enumInstanceExtensionProperties( layerName.empty() ? nullptr : layerName.c_str()
				, &count
				, result.data() );
		}
	}
	while ( res == VK_INCOMPLETE );

	if ( res != VK_SUCCESS )
	{
		throw ashes::Exception{ res, "Instance layer extensions retrieval" };
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
			if ( queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
			{
				// Tout d'abord on choisit une file graphique
				if ( graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
				{
					graphicsQueueFamilyIndex = i;
				}

				// Si la file supporte aussi les calculs, on la choisit en compute queue
				if ( queueProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT
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
			else if ( queueProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT
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
		throw ashes::Exception{ VK_ERROR_INITIALIZATION_FAILED
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
	ashes::FloatArray queuePriorities = { 1.0f };
	ashes::DeviceQueueCreateInfoArray queueCreateInfos;

	if ( graphicsQueueFamilyIndex != uint32_t( ~( 0u ) ) )
	{
		queueCreateInfos.push_back(
			{
				0u,
				graphicsQueueFamilyIndex,
				queuePriorities,
			} );
	}

	if ( presentQueueFamilyIndex != graphicsQueueFamilyIndex )
	{
		queueCreateInfos.push_back(
			{
				0u,
				presentQueueFamilyIndex,
				queuePriorities,
			} );
	}

	if ( computeQueueFamilyIndex != graphicsQueueFamilyIndex )
	{
		queueCreateInfos.push_back(
			{
				0u,
				computeQueueFamilyIndex,
				queuePriorities,
			} );
	}

	ashes::StringArray enabledExtensions
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};
	return ashes::DeviceCreateInfo
	{
		0u,
		std::move( queueCreateInfos ),
		{},
		std::move( enabledExtensions ),
		gpu.getFeatures(),
	};
}

uint32_t doGetImageCount( ashes::Surface const & surface )
{
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

VkSurfaceFormatKHR doSelectFormat( ashes::Surface const & surface )
{
	VkSurfaceFormatKHR result;
	auto formats = surface.getFormats();
	// Si la liste de formats ne contient qu'une entr�e VK_FORMAT_UNDEFINED,
	// la surface n'a pas de format préféré. Sinon, au moins un format supporté
	// sera renvoyé.
	if ( formats.size() == 1u && formats[0].format == VK_FORMAT_UNDEFINED )
	{
		result.format = VK_FORMAT_R8G8B8A8_UNORM;
		result.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	}
	else
	{
		assert( !formats.empty() );
		auto it = std::find_if( formats.begin()
			, formats.end()
			, []( VkSurfaceFormatKHR const & lookup )
			{
				return lookup.format == VK_FORMAT_R8G8B8A8_UNORM;
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

VkPresentModeKHR doSelectPresentMode( ashes::Surface const & surface )
{
	auto presentModes = surface.getPresentModes();
	// Si le mode boîte aux lettres est disponible, on utilise celui-là, car c'est celui avec le
	// minimum de latence dans tearing.
	// Sinon, on essaye le mode IMMEDIATE, qui est normalement disponible, et est le plus rapide
	// (bien qu'il y ait du tearing). Sinon on utilise le mode FIFO qui est toujours disponible.
	VkPresentModeKHR result{ VK_PRESENT_MODE_FIFO_KHR };

	for ( auto mode : presentModes )
	{
		if ( mode == VK_PRESENT_MODE_MAILBOX_KHR )
		{
			result = mode;
			break;
		}

		if ( ( result != VK_PRESENT_MODE_MAILBOX_KHR )
			&& ( mode == VK_PRESENT_MODE_IMMEDIATE_KHR ) )
		{
			result = mode;
		}
	}

	return result;
}

VkSwapchainCreateInfoKHR doGetSwapChainCreateInfo( ashes::Surface const & surface
	, VkExtent2D const & size )
{
	VkExtent2D swapChainExtent{};
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
	VkSurfaceTransformFlagBitsKHR preTransform{};

	if ( ( surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) )
	{
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = surfaceCaps.currentTransform;
	}

	auto presentMode = doSelectPresentMode( surface );
	auto surfaceFormat = doSelectFormat( surface );
	return VkSwapchainCreateInfoKHR
	{
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		nullptr,
		0u,
		surface,
		doGetImageCount( surface ),
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		swapChainExtent,
		1u,
		surfaceCaps.supportedUsageFlags,
		VK_SHARING_MODE_EXCLUSIVE,
		0u,
		nullptr,
		preTransform,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		presentMode,
		VK_TRUE,
		nullptr
	};
}

void doCreateRenderingResources( Application & application )
{
	application.renderingResources.clear();

	for ( uint32_t i = 0u; i < uint32_t( application.swapChain->getImageCount() ); ++i )
	{
		application.renderingResources.emplace_back( std::make_unique< RenderingResources >( application.device->createSemaphore()
			, application.device->createSemaphore()
			, application.device->createFence( VK_FENCE_CREATE_SIGNALED_BIT )
			, application.commandPool->createCommandBuffer()
			, 0u ) );
	}
}

void createSwapChain( Application & application )
{
	application.swapChain = application.device->createSwapChain( doGetSwapChainCreateInfo( *application.surface
		, application.dimensions ) );
	application.swapChainImages = application.swapChain->getImages();
	application.clearColour = VkClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f };
	doCreateRenderingResources( application );
}

ashes::RenderPassPtr createRenderPass( ashes::Device const & device
	, ashes::SwapChain const & swapChain )
{
	ashes::VkAttachmentDescriptionArray attaches;
	attaches.push_back(
		// We'll have only one colour attachment for the render pass.
		{
			0u,
			// The format is the swapchain's pixel format.
			swapChain.getFormat(),
			// Multisampling is disabled for this attach.
			VK_SAMPLE_COUNT_1_BIT,
			// We want to clear the attach at the beginning of the render pass.
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			// And we want its result to be stored at the end of the render pass.
			VK_ATTACHMENT_STORE_OP_STORE,
			// We don't care about stencil attachment.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			// We don't care about stencil attachment.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			// The initial layout is the layout expected for the attachment at the beginning of the render pass.
			// We expect the attach to have been presented to the surface, so it should be either a present source or undefined.
			VK_IMAGE_LAYOUT_UNDEFINED,
			// The final layout is the layouts into which the attachment is transitioned at the end of the render pass.
			// We want the attach to be presented to the surface, so we make it a present source.
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		} );
	// A render pass always has at least one subpass.
	// In our case, this subpass is also the only one,
	// its only attachment is the render pass' one.
	// We want this attachment to be transitioned to colour attachment, so we can write into it.
	// We now create the subpasses.
	// The subpass state is used to setup the needed states at the beginning of the subpass.
	ashes::SubpassDescriptionArray subpasses;
	subpasses.emplace_back( 0u
		, VK_PIPELINE_BIND_POINT_GRAPHICS
		, ashes::VkAttachmentReferenceArray{}
		, ashes::VkAttachmentReferenceArray{ VkAttachmentReference{ 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } }
		, ashes::VkAttachmentReferenceArray{}
		, ashes::Optional< VkAttachmentReference >{}
		, ashes::UInt32Array{} );

	ashes::VkSubpassDependencyArray dependencies;
	dependencies.resize( 2u );
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0u;
	dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0u;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	ashes::RenderPassCreateInfo createInfo
	{
		0u,
		std::move( attaches ),
		std::move( subpasses ),
		std::move( dependencies ),
	};
	// Eventually, we create the render pass, using all previously built informations.
	return device.createRenderPass( std::move( createInfo ) );
}

ashes::ImageViewCRefArray doPrepareAttaches( Application const & application
	, uint32_t backBuffer
       	, ashes::ImageViewArray & views )
{
	ashes::ImageViewCRefArray attaches;

	for ( auto & attach : application.renderPass->getAttachments() )
	{
		auto & image = application.swapChainImages[backBuffer];
		views.push_back( image.createView( VkImageViewCreateInfo
			{
				VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				nullptr,
				0u,
				image,
				VK_IMAGE_VIEW_TYPE_2D,
				application.swapChain->getFormat(),
				{},
				{
					ashes::getAspectMask( application.swapChain->getFormat() ),
					0u,
					1u,
					0u,
					1u,
				}
			} ) );
	}

	for ( auto & view : views )
	{
		attaches.emplace_back( view );
	}

	return attaches;
}

void doCreateFrameBuffers( Application & application )
{
	application.frameBuffers.resize( application.swapChain->getImageCount() );

	for ( size_t i = 0u; i < application.frameBuffers.size(); ++i )
	{
		auto attaches = doPrepareAttaches( application, uint32_t( i ), application.views );
		application.frameBuffers[i] = application.renderPass->createFrameBuffer( application.swapChain->getDimensions()
			, std::move( attaches ) );
	}
}

void doCreateCommandBuffers( Application & application )
{
	application.commandBuffers.resize( application.swapChain->getImageCount() );

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

		commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
		commandBuffer.beginRenderPass( *application.renderPass
			, frameBuffer
			, ashes::VkClearValueArray{ { application.clearColour } }
			, VK_SUBPASS_CONTENTS_INLINE );
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}
}

RenderingResources * getResources( Application & application )
{
	auto & resources = *application.renderingResources[application.resourceIndex];
	application.resourceIndex = ( application.resourceIndex + 1 ) % application.renderingResources.size();
	bool res = resources.fence->wait( ashes::MaxTimeout ) == ashes::WaitResult::eSuccess;

	if ( res )
	{
		resources.fence->reset();
		uint32_t imageIndex{ 0u };
		auto res = application.swapChain->acquireNextImage( std::numeric_limits< uint64_t >::max()
			, *resources.imageAvailableSemaphore
			, imageIndex );

		if ( checkNeedReset( application
			, VkResult( res )
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
	, VkResult errCode
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
			doResetSwapChain( application );
		}
		else
		{
			result = true;
		}
		break;

	case VK_SUBOPTIMAL_KHR:
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

static VkBool32 VKAPI_PTR DbgCallback( VkDebugReportFlagsEXT msgFlags
	, VkDebugReportObjectTypeEXT objType
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
