#include "GlRendererPrerequisites.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Command/GlCommandBuffer.hpp"
#include "Command/GlCommandPool.hpp"
#include "Command/GlQueue.hpp"
#include "Core/GlContext.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlSurface.hpp"
#include "Core/GlSwapChain.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlSampler.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Miscellaneous/GlQueryPool.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFramebuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Sync/GlEvent.hpp"

#include <Ashes/Core/Exception.hpp>

namespace gl_renderer
{
	uint32_t deduceMemoryType( uint32_t typeBits
		, ashes::MemoryPropertyFlags requirements )
	{
		auto & memoryProperties = Instance::getMemoryProperties();
		uint32_t result = 0xFFFFFFFFu;
		bool found{ false };

		// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
		uint32_t i{ 0 };

		while ( i < memoryProperties.memoryTypeCount && !found )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de m�moire est disponible, a-t-il les propri�t�s demand�es?
				if ( ( memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					result = i;
					found = true;
				}
			}

			typeBits >>= 1;
			++i;
		}

		if ( !found )
		{
			throw ashes::Exception{ ashes::Result::eErrorRenderer, "Could not deduce memory type" };
		}

		return result;
	}
}

#ifdef __cplusplus
extern "C" {
#endif

#define VK_IMPLEMENT_HANDLE(object)\
	struct object##_T\
	{\
		std::unique_ptr< gl_renderer::object > internal;\
	}

#define VK_IMPLEMENT_DUMMY_HANDLE(object)\
	struct object##_T\
	{\
	}

	VK_IMPLEMENT_HANDLE( Instance );
	VK_IMPLEMENT_HANDLE( PhysicalDevice );
	VK_IMPLEMENT_HANDLE( Device );
	VK_IMPLEMENT_HANDLE( Queue );
	VK_IMPLEMENT_HANDLE( Semaphore );
	VK_IMPLEMENT_HANDLE( CommandBuffer );
	VK_IMPLEMENT_HANDLE( Fence );
	VK_IMPLEMENT_HANDLE( Buffer );
	VK_IMPLEMENT_HANDLE( Image );
	VK_IMPLEMENT_HANDLE( Event );
	VK_IMPLEMENT_HANDLE( QueryPool );
	VK_IMPLEMENT_HANDLE( BufferView );
	VK_IMPLEMENT_HANDLE( ImageView );
	VK_IMPLEMENT_HANDLE( ShaderModule );
	VK_IMPLEMENT_DUMMY_HANDLE( PipelineCache );
	VK_IMPLEMENT_HANDLE( PipelineLayout );
	VK_IMPLEMENT_HANDLE( RenderPass );
	VK_IMPLEMENT_HANDLE( Pipeline );
	VK_IMPLEMENT_HANDLE( DescriptorSetLayout );
	VK_IMPLEMENT_HANDLE( Sampler );
	VK_IMPLEMENT_HANDLE( DescriptorPool );
	VK_IMPLEMENT_HANDLE( DescriptorSet );
	VK_IMPLEMENT_HANDLE( CommandPool );

	VK_IMPLEMENT_DUMMY_HANDLE( SamplerYcbcrConversion );
	VK_IMPLEMENT_DUMMY_HANDLE( DescriptorUpdateTemplate );

	VK_IMPLEMENT_DUMMY_HANDLE( DisplayKHR );
	VK_IMPLEMENT_DUMMY_HANDLE( DisplayModeKHR );
	VK_IMPLEMENT_DUMMY_HANDLE( DebugReportCallbackEXT );
	VK_IMPLEMENT_DUMMY_HANDLE( ObjectTableNVX );
	VK_IMPLEMENT_DUMMY_HANDLE( IndirectCommandsLayoutNVX );
	VK_IMPLEMENT_DUMMY_HANDLE( DebugUtilsMessengerEXT );
	VK_IMPLEMENT_DUMMY_HANDLE( ValidationCacheEXT );

	VK_DEFINE_HANDLE( SurfaceKHR );
	VK_DEFINE_HANDLE( SwapchainKHR );

	struct DeviceMemory_T
	{
		gl_renderer::DeviceMemoryPtr internal;
	};

	struct Framebuffer_T
	{
		gl_renderer::FrameBuffer internal;
	};

	struct SurfaceKHR_T
	{
		gl_renderer::Surface internal;
	};

	struct SwapchainKHR_T
	{
		gl_renderer::SwapChain internal;
	};

	Gl4Renderer_API PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance                                  instance,
		const char*                                 pName );

	Gl4Renderer_API PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
		VkDevice                                    device,
		const char*                                 pName );

	gl_renderer::Instance const & getInstance( VkInstance instance )
	{
		return *instance->internal;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateInstance(
		const VkInstanceCreateInfo*                 pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkInstance*                                 pInstance )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			assert( pInstance );
			auto instance = new Instance_T;
			instance->internal = std::make_unique< gl_renderer::Instance >( *pCreateInfo );
			( *pInstance ) = instance;
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyInstance(
		VkInstance                                  instance,
		const VkAllocationCallbacks*                pAllocator )
	{
		instance->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkEnumeratePhysicalDevices(
		VkInstance                                  instance,
		uint32_t*                                   pPhysicalDeviceCount,
		VkPhysicalDevice*                           pPhysicalDevices )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			if ( !pPhysicalDevices )
			{
				*pPhysicalDeviceCount = 1u;
				return result;
			}

			auto gpus = getInstance( instance ).enumeratePhysicalDevices();

			for ( auto & gpu : gpus )
			{
				*pPhysicalDevices = new PhysicalDevice_T;
				( *pPhysicalDevices )->internal = std::move( gpu );
				++pPhysicalDevices;
			}
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkGetPhysicalDeviceFeatures(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceFeatures*                   pFeatures )
	{
		*pFeatures = physicalDevice->internal->getFeatures();
	}

	Gl4Renderer_API void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(
		VkPhysicalDevice                            physicalDevice,
		VkFormat                                    format,
		VkFormatProperties*                         pFormatProperties )
	{
		*pFormatProperties = physicalDevice->internal->getFormatProperties( format );
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(
		VkPhysicalDevice                            physicalDevice,
		VkFormat                                    format,
		VkImageType                                 type,
		VkImageTiling                               tiling,
		VkImageUsageFlags                           usage,
		VkImageCreateFlags                          flags,
		VkImageFormatProperties*                    pImageFormatProperties )
	{
		// TODO
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	Gl4Renderer_API void VKAPI_CALL vkGetPhysicalDeviceProperties(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceProperties*                 pProperties )
	{
		*pProperties = physicalDevice->internal->getProperties();
	}

	Gl4Renderer_API void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
		VkPhysicalDevice                            physicalDevice,
		uint32_t*                                   pQueueFamilyPropertyCount,
		VkQueueFamilyProperties*                    pQueueFamilyProperties )
	{
		if ( !pQueueFamilyProperties )
		{
			*pQueueFamilyPropertyCount = 1u;
			return;
		}

		auto props = physicalDevice->internal->getQueueFamilyProperties();

		for ( auto & prop : props )
		{
			( *pQueueFamilyProperties ) = prop;
			++pQueueFamilyProperties;
		}
	}

	Gl4Renderer_API void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(
		VkPhysicalDevice                            physicalDevice,
		VkPhysicalDeviceMemoryProperties*           pMemoryProperties )
	{
		*pMemoryProperties = physicalDevice->internal->getMemoryProperties();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateDevice(
		VkPhysicalDevice                            physicalDevice,
		const VkDeviceCreateInfo*                   pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkDevice*                                   pDevice )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			auto & gpu = *physicalDevice->internal;
			assert( pDevice );
			auto device = new Device_T;
			device->internal = std::make_unique< gl_renderer::Device >( gpu.getInstance()
				, gpu
				, gpu.getInstance().getContext()
				, *pCreateInfo );
			( *pDevice ) = device;
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyDevice(
		VkDevice                                    device,
		const VkAllocationCallbacks*                pAllocator )
	{
		device->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(
		const char*                                 pLayerName,
		uint32_t*                                   pPropertyCount,
		VkExtensionProperties*                      pProperties )
	{
		static std::vector< VkExtensionProperties > const extensions
		{
			VkExtensionProperties{ VK_KHR_SURFACE_EXTENSION_NAME, ashes::makeVersion( 1, 0, 0 ) },
			[]()
			{
				VkExtensionProperties result;
				result.specVersion = ashes::makeVersion( 1, 0, 0 );
				strncpy( result.extensionName, gl_renderer::VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE );
				return result;
			}(),
			VkExtensionProperties{ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, ashes::makeVersion( 1, 0, 0 ) },
		};

		*pPropertyCount = uint32_t( extensions.size() );

		if ( pProperties )
		{
			for ( auto & extension : extensions )
			{
				*pProperties = extension;
				++pProperties;
			}
		}

		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(
		VkPhysicalDevice                            physicalDevice,
		const char*                                 pLayerName,
		uint32_t*                                   pPropertyCount,
		VkExtensionProperties*                      pProperties )
	{
		// TODO
		*pPropertyCount = 0;
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(
		uint32_t*                                   pPropertyCount,
		VkLayerProperties*                          pProperties )
	{
		// TODO
		*pPropertyCount = 0u;
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(
		VkPhysicalDevice                            physicalDevice,
		uint32_t*                                   pPropertyCount,
		VkLayerProperties*                          pProperties )
	{
		// TODO
		*pPropertyCount = 0u;
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkGetDeviceQueue(
		VkDevice                                    device,
		uint32_t                                    queueFamilyIndex,
		uint32_t                                    queueIndex,
		VkQueue*                                    pQueue )
	{
		( *pQueue )->internal = std::move( device->internal->getQueue( queueFamilyIndex, queueIndex ) );
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkQueueSubmit(
		VkQueue                                     queue,
		uint32_t                                    submitCount,
		const VkSubmitInfo*                         pSubmits,
		VkFence                                     fence )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			queue->internal->submit( { pSubmits, pSubmits + submitCount }, fence );
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkQueueWaitIdle(
		VkQueue                                     queue )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			queue->internal->waitIdle();
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkDeviceWaitIdle(
		VkDevice                                    device )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			device->internal->waitIdle();
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkAllocateMemory(
		VkDevice                                    device,
		const VkMemoryAllocateInfo*                 pAllocateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkDeviceMemory*                             pMemory )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			( *pMemory )->internal = std::make_unique< gl_renderer::DeviceMemory >( *device->internal
				, *pAllocateInfo );
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkFreeMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		const VkAllocationCallbacks*                pAllocator )
	{
		memory->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkMapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		VkDeviceSize                                offset,
		VkDeviceSize                                size,
		VkMemoryMapFlags                            flags,
		void**                                      ppData )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			*ppData = memory->internal->lock( offset, size, flags );
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkUnmapMemory(
		VkDevice                                    device,
		VkDeviceMemory                              memory );

	Gl4Renderer_API VkResult VKAPI_CALL vkFlushMappedMemoryRanges(
		VkDevice                                    device,
		uint32_t                                    memoryRangeCount,
		const VkMappedMemoryRange*                  pMemoryRanges )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
			{
				pMemoryRanges->memory->internal->flush( pMemoryRanges->offset, pMemoryRanges->size );
				++memoryRangeCount;
			}
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(
		VkDevice                                    device,
		uint32_t                                    memoryRangeCount,
		const VkMappedMemoryRange*                  pMemoryRanges )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
			{
				pMemoryRanges->memory->internal->invalidate( pMemoryRanges->offset, pMemoryRanges->size );
				++memoryRangeCount;
			}
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkGetDeviceMemoryCommitment(
		VkDevice                                    device,
		VkDeviceMemory                              memory,
		VkDeviceSize*                               pCommittedMemoryInBytes )
	{
		// TODO
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkBindBufferMemory(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkDeviceMemory                              memory,
		VkDeviceSize                                memoryOffset )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			buffer->internal->bindMemory( memory->internal );
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkBindImageMemory(
		VkDevice                                    device,
		VkImage                                     image,
		VkDeviceMemory                              memory,
		VkDeviceSize                                memoryOffset )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			image->internal->bindMemory( memory->internal );
		}
		catch ( ashes::Exception & exc )
		{
			result = VkResult( exc.getResult() );
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	Gl4Renderer_API void VKAPI_CALL vkGetBufferMemoryRequirements(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		VkMemoryRequirements*                       pMemoryRequirements )
	{
		*pMemoryRequirements = buffer->internal->getMemoryRequirements();
	}

	Gl4Renderer_API void VKAPI_CALL vkGetImageMemoryRequirements(
		VkDevice                                    device,
		VkImage                                     image,
		VkMemoryRequirements*                       pMemoryRequirements )
	{
		*pMemoryRequirements = image->internal->getMemoryRequirements();
	}

	Gl4Renderer_API void VKAPI_CALL vkGetImageSparseMemoryRequirements(
		VkDevice                                    device,
		VkImage                                     image,
		uint32_t*                                   pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements*            pSparseMemoryRequirements )
	{
		// TODO
		*pSparseMemoryRequirementCount = 0u;
	}

	Gl4Renderer_API void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(
		VkPhysicalDevice                            physicalDevice,
		VkFormat                                    format,
		VkImageType                                 type,
		VkSampleCountFlagBits                       samples,
		VkImageUsageFlags                           usage,
		VkImageTiling                               tiling,
		uint32_t*                                   pPropertyCount,
		VkSparseImageFormatProperties*              pProperties )
	{
		// TODO
		*pPropertyCount = 0u;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkQueueBindSparse(
		VkQueue                                     queue,
		uint32_t                                    bindInfoCount,
		const VkBindSparseInfo*                     pBindInfo,
		VkFence                                     fence )
	{
		// TODO
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateFence(
		VkDevice                                    device,
		const VkFenceCreateInfo*                    pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkFence*                                    pFence )
	{
		( *pFence )->internal = std::make_unique< gl_renderer::Fence >( *device->internal );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyFence(
		VkDevice                                    device,
		VkFence                                     fence,
		const VkAllocationCallbacks*                pAllocator )
	{
		fence->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkResetFences(
		VkDevice                                    device,
		uint32_t                                    fenceCount,
		const VkFence*                              pFences )
	{
		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			( *pFences )->internal->reset();
		}

		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkGetFenceStatus(
		VkDevice                                    device,
		VkFence                                     fence )
	{
		// TODO
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkWaitForFences(
		VkDevice                                    device,
		uint32_t                                    fenceCount,
		const VkFence*                              pFences,
		VkBool32                                    waitAll,
		uint64_t                                    timeout )
	{
		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			( *pFences )->internal->wait( timeout );
		}

		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateSemaphore(
		VkDevice                                    device,
		const VkSemaphoreCreateInfo*                pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkSemaphore*                                pSemaphore )
	{
		( *pSemaphore )->internal = std::make_unique< gl_renderer::Semaphore >();
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroySemaphore(
		VkDevice                                    device,
		VkSemaphore                                 semaphore,
		const VkAllocationCallbacks*                pAllocator )
	{
		semaphore->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateEvent(
		VkDevice                                    device,
		const VkEventCreateInfo*                    pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkEvent*                                    pEvent )
	{
		( *pEvent )->internal = std::make_unique< gl_renderer::Event >();
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyEvent(
		VkDevice                                    device,
		VkEvent                                     event,
		const VkAllocationCallbacks*                pAllocator )
	{
		event->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkGetEventStatus(
		VkDevice                                    device,
		VkEvent                                     event )
	{
		event->internal->getStatus();
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkSetEvent(
		VkDevice                                    device,
		VkEvent                                     event )
	{
		event->internal->set();
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkResetEvent(
		VkDevice                                    device,
		VkEvent                                     event )
	{
		event->internal->reset();
		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateQueryPool(
		VkDevice                                    device,
		const VkQueryPoolCreateInfo*                pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkQueryPool*                                pQueryPool )
	{
		( *pQueryPool )->internal = std::make_unique< gl_renderer::QueryPool >( *pCreateInfo );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyQueryPool(
		VkDevice                                    device,
		VkQueryPool                                 queryPool,
		const VkAllocationCallbacks*                pAllocator )
	{
		queryPool->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkGetQueryPoolResults(
		VkDevice                                    device,
		VkQueryPool                                 queryPool,
		uint32_t                                    firstQuery,
		uint32_t                                    queryCount,
		size_t                                      dataSize,
		void*                                       pData,
		VkDeviceSize                                stride,
		VkQueryResultFlags                          flags )
	{
		if ( flags & VK_QUERY_RESULT_64_BIT )
		{
			std::vector< uint64_t > results;
			results.reserve( queryCount );
			queryPool->internal->getResults( firstQuery
				, queryCount
				, stride
				, flags
				, results );
			auto data = reinterpret_cast< uint8_t * >( pData );

			for ( auto & result : results )
			{
				*reinterpret_cast< uint64_t * >( data ) = result;
				data += stride;
			}
		}
		else
		{
			std::vector< uint32_t > results;
			results.reserve( queryCount );
			queryPool->internal->getResults( firstQuery
				, queryCount
				, stride
				, flags
				, results );
			auto data = reinterpret_cast< uint8_t * >( pData );

			for ( auto & result : results )
			{
				*reinterpret_cast< uint64_t * >( data ) = result;
				data += stride;
			}
		}
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateBuffer(
		VkDevice                                    device,
		const VkBufferCreateInfo*                   pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkBuffer*                                   pBuffer )
	{
		( *pBuffer )->internal = std::make_unique< gl_renderer::Buffer >( *device->internal
			, *pCreateInfo );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyBuffer(
		VkDevice                                    device,
		VkBuffer                                    buffer,
		const VkAllocationCallbacks*                pAllocator )
	{
		buffer->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateBufferView(
		VkDevice                                    device,
		const VkBufferViewCreateInfo*               pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkBufferView*                               pView )
	{
		( *pView )->internal = std::make_unique< gl_renderer::BufferView >( *device->internal
			, *pCreateInfo->buffer->internal
			, *pCreateInfo );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyBufferView(
		VkDevice                                    device,
		VkBufferView                                bufferView,
		const VkAllocationCallbacks*                pAllocator )
	{
		bufferView->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateImage(
		VkDevice                                    device,
		const VkImageCreateInfo*                    pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkImage*                                    pImage )
	{
		( *pImage )->internal = std::make_unique< gl_renderer::Image >( *device->internal
			, *pCreateInfo );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyImage(
		VkDevice                                    device,
		VkImage                                     image,
		const VkAllocationCallbacks*                pAllocator )
	{
		image->internal.reset();
	}

	Gl4Renderer_API void VKAPI_CALL vkGetImageSubresourceLayout(
		VkDevice                                    device,
		VkImage                                     image,
		const VkImageSubresource*                   pSubresource,
		VkSubresourceLayout*                        pLayout )
	{
		device->internal->getImageSubresourceLayout( *image->internal
			, *pSubresource
			, *pLayout );
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateImageView(
		VkDevice                                    device,
		const VkImageViewCreateInfo*                pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkImageView*                                pView )
	{
		( *pView )->internal = std::make_unique< gl_renderer::ImageView >( *device->internal
			, *pCreateInfo->image->internal
			, *pCreateInfo );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyImageView(
		VkDevice                                    device,
		VkImageView                                 imageView,
		const VkAllocationCallbacks*                pAllocator )
	{
		imageView->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateShaderModule(
		VkDevice                                    device,
		const VkShaderModuleCreateInfo*             pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkShaderModule*                             pShaderModule )
	{
		( *pShaderModule )->internal = std::make_unique< gl_renderer::ShaderModule >( *device->internal
			, *pCreateInfo );
		return VK_SUCCESS;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyShaderModule(
		VkDevice                                    device,
		VkShaderModule                              shaderModule,
		const VkAllocationCallbacks*                pAllocator )
	{
		shaderModule->internal.reset();
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreatePipelineCache(
		VkDevice                                    device,
		const VkPipelineCacheCreateInfo*            pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkPipelineCache*                            pPipelineCache )
	{
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	Gl4Renderer_API void VKAPI_CALL vkDestroyPipelineCache(
		VkDevice                                    device,
		VkPipelineCache                             pipelineCache,
		const VkAllocationCallbacks*                pAllocator )
	{
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkGetPipelineCacheData(
		VkDevice                                    device,
		VkPipelineCache                             pipelineCache,
		size_t*                                     pDataSize,
		void*                                       pData )
	{
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkMergePipelineCaches(
		VkDevice                                    device,
		VkPipelineCache                             dstCache,
		uint32_t                                    srcCacheCount,
		const VkPipelineCache*                      pSrcCaches )
	{
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateGraphicsPipelines(
		VkDevice                                    device,
		VkPipelineCache                             pipelineCache,
		uint32_t                                    createInfoCount,
		const VkGraphicsPipelineCreateInfo*         pCreateInfos,
		const VkAllocationCallbacks*                pAllocator,
		VkPipeline*                                 pPipelines )
	{
		for ( uint32_t i = 0u; i < createInfoCount; ++i )
		{
			( *pPipelines ) = new Pipeline_T{ std::make_unique< gl_renderer::Pipeline >( *device->internal
				, *pCreateInfos->layout->internal
				, *pCreateInfos ) };
			++pPipelines;
			++pCreateInfos;
		}

		return VK_SUCCESS;
	}

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateComputePipelines(
		VkDevice                                    device,
		VkPipelineCache                             pipelineCache,
		uint32_t                                    createInfoCount,
		const VkComputePipelineCreateInfo*          pCreateInfos,
		const VkAllocationCallbacks*                pAllocator,
		VkPipeline*                                 pPipelines );

	Gl4Renderer_API void VKAPI_CALL vkDestroyPipeline(
		VkDevice                                    device,
		VkPipeline                                  pipeline,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreatePipelineLayout(
		VkDevice                                    device,
		const VkPipelineLayoutCreateInfo*           pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkPipelineLayout*                           pPipelineLayout );

	Gl4Renderer_API void VKAPI_CALL vkDestroyPipelineLayout(
		VkDevice                                    device,
		VkPipelineLayout                            pipelineLayout,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateSampler(
		VkDevice                                    device,
		const VkSamplerCreateInfo*                  pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkSampler*                                  pSampler );

	Gl4Renderer_API void VKAPI_CALL vkDestroySampler(
		VkDevice                                    device,
		VkSampler                                   sampler,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateDescriptorSetLayout(
		VkDevice                                    device,
		const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkDescriptorSetLayout*                      pSetLayout );

	Gl4Renderer_API void VKAPI_CALL vkDestroyDescriptorSetLayout(
		VkDevice                                    device,
		VkDescriptorSetLayout                       descriptorSetLayout,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateDescriptorPool(
		VkDevice                                    device,
		const VkDescriptorPoolCreateInfo*           pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkDescriptorPool*                           pDescriptorPool );

	Gl4Renderer_API void VKAPI_CALL vkDestroyDescriptorPool(
		VkDevice                                    device,
		VkDescriptorPool                            descriptorPool,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkResetDescriptorPool(
		VkDevice                                    device,
		VkDescriptorPool                            descriptorPool,
		VkDescriptorPoolResetFlags                  flags );

	Gl4Renderer_API VkResult VKAPI_CALL vkAllocateDescriptorSets(
		VkDevice                                    device,
		const VkDescriptorSetAllocateInfo*          pAllocateInfo,
		VkDescriptorSet*                            pDescriptorSets );

	Gl4Renderer_API VkResult VKAPI_CALL vkFreeDescriptorSets(
		VkDevice                                    device,
		VkDescriptorPool                            descriptorPool,
		uint32_t                                    descriptorSetCount,
		const VkDescriptorSet*                      pDescriptorSets );

	Gl4Renderer_API void VKAPI_CALL vkUpdateDescriptorSets(
		VkDevice                                    device,
		uint32_t                                    descriptorWriteCount,
		const VkWriteDescriptorSet*                 pDescriptorWrites,
		uint32_t                                    descriptorCopyCount,
		const VkCopyDescriptorSet*                  pDescriptorCopies );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateFramebuffer(
		VkDevice                                    device,
		const VkFramebufferCreateInfo*              pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkFramebuffer*                              pFramebuffer );

	Gl4Renderer_API void VKAPI_CALL vkDestroyFramebuffer(
		VkDevice                                    device,
		VkFramebuffer                               framebuffer,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateRenderPass(
		VkDevice                                    device,
		const VkRenderPassCreateInfo*               pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkRenderPass*                               pRenderPass );

	Gl4Renderer_API void VKAPI_CALL vkDestroyRenderPass(
		VkDevice                                    device,
		VkRenderPass                                renderPass,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API void VKAPI_CALL vkGetRenderAreaGranularity(
		VkDevice                                    device,
		VkRenderPass                                renderPass,
		VkExtent2D*                                 pGranularity );

	Gl4Renderer_API VkResult VKAPI_CALL vkCreateCommandPool(
		VkDevice                                    device,
		const VkCommandPoolCreateInfo*              pCreateInfo,
		const VkAllocationCallbacks*                pAllocator,
		VkCommandPool*                              pCommandPool );

	Gl4Renderer_API void VKAPI_CALL vkDestroyCommandPool(
		VkDevice                                    device,
		VkCommandPool                               commandPool,
		const VkAllocationCallbacks*                pAllocator );

	Gl4Renderer_API VkResult VKAPI_CALL vkResetCommandPool(
		VkDevice                                    device,
		VkCommandPool                               commandPool,
		VkCommandPoolResetFlags                     flags );

	Gl4Renderer_API VkResult VKAPI_CALL vkAllocateCommandBuffers(
		VkDevice                                    device,
		const VkCommandBufferAllocateInfo*          pAllocateInfo,
		VkCommandBuffer*                            pCommandBuffers );

	Gl4Renderer_API void VKAPI_CALL vkFreeCommandBuffers(
		VkDevice                                    device,
		VkCommandPool                               commandPool,
		uint32_t                                    commandBufferCount,
		const VkCommandBuffer*                      pCommandBuffers );

	Gl4Renderer_API VkResult VKAPI_CALL vkBeginCommandBuffer(
		VkCommandBuffer                             commandBuffer,
		const VkCommandBufferBeginInfo*             pBeginInfo );

	Gl4Renderer_API VkResult VKAPI_CALL vkEndCommandBuffer(
		VkCommandBuffer                             commandBuffer );

	Gl4Renderer_API VkResult VKAPI_CALL vkResetCommandBuffer(
		VkCommandBuffer                             commandBuffer,
		VkCommandBufferResetFlags                   flags );

	Gl4Renderer_API void VKAPI_CALL vkCmdBindPipeline(
		VkCommandBuffer                             commandBuffer,
		VkPipelineBindPoint                         pipelineBindPoint,
		VkPipeline                                  pipeline );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetViewport(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    firstViewport,
		uint32_t                                    viewportCount,
		const VkViewport*                           pViewports );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetScissor(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    firstScissor,
		uint32_t                                    scissorCount,
		const VkRect2D*                             pScissors );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetLineWidth(
		VkCommandBuffer                             commandBuffer,
		float                                       lineWidth );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetDepthBias(
		VkCommandBuffer                             commandBuffer,
		float                                       depthBiasConstantFactor,
		float                                       depthBiasClamp,
		float                                       depthBiasSlopeFactor );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetBlendConstants(
		VkCommandBuffer                             commandBuffer,
		const float                                 blendConstants[4] );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetDepthBounds(
		VkCommandBuffer                             commandBuffer,
		float                                       minDepthBounds,
		float                                       maxDepthBounds );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetStencilCompareMask(
		VkCommandBuffer                             commandBuffer,
		VkStencilFaceFlags                          faceMask,
		uint32_t                                    compareMask );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetStencilWriteMask(
		VkCommandBuffer                             commandBuffer,
		VkStencilFaceFlags                          faceMask,
		uint32_t                                    writeMask );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetStencilReference(
		VkCommandBuffer                             commandBuffer,
		VkStencilFaceFlags                          faceMask,
		uint32_t                                    reference );

	Gl4Renderer_API void VKAPI_CALL vkCmdBindDescriptorSets(
		VkCommandBuffer                             commandBuffer,
		VkPipelineBindPoint                         pipelineBindPoint,
		VkPipelineLayout                            layout,
		uint32_t                                    firstSet,
		uint32_t                                    descriptorSetCount,
		const VkDescriptorSet*                      pDescriptorSets,
		uint32_t                                    dynamicOffsetCount,
		const uint32_t*                             pDynamicOffsets );

	Gl4Renderer_API void VKAPI_CALL vkCmdBindIndexBuffer(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    buffer,
		VkDeviceSize                                offset,
		VkIndexType                                 indexType );

	Gl4Renderer_API void VKAPI_CALL vkCmdBindVertexBuffers(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    firstBinding,
		uint32_t                                    bindingCount,
		const VkBuffer*                             pBuffers,
		const VkDeviceSize*                         pOffsets );

	Gl4Renderer_API void VKAPI_CALL vkCmdDraw(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    vertexCount,
		uint32_t                                    instanceCount,
		uint32_t                                    firstVertex,
		uint32_t                                    firstInstance );

	Gl4Renderer_API void VKAPI_CALL vkCmdDrawIndexed(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    indexCount,
		uint32_t                                    instanceCount,
		uint32_t                                    firstIndex,
		int32_t                                     vertexOffset,
		uint32_t                                    firstInstance );

	Gl4Renderer_API void VKAPI_CALL vkCmdDrawIndirect(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    buffer,
		VkDeviceSize                                offset,
		uint32_t                                    drawCount,
		uint32_t                                    stride );

	Gl4Renderer_API void VKAPI_CALL vkCmdDrawIndexedIndirect(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    buffer,
		VkDeviceSize                                offset,
		uint32_t                                    drawCount,
		uint32_t                                    stride );

	Gl4Renderer_API void VKAPI_CALL vkCmdDispatch(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    groupCountX,
		uint32_t                                    groupCountY,
		uint32_t                                    groupCountZ );

	Gl4Renderer_API void VKAPI_CALL vkCmdDispatchIndirect(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    buffer,
		VkDeviceSize                                offset );

	Gl4Renderer_API void VKAPI_CALL vkCmdCopyBuffer(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    srcBuffer,
		VkBuffer                                    dstBuffer,
		uint32_t                                    regionCount,
		const VkBufferCopy*                         pRegions );

	Gl4Renderer_API void VKAPI_CALL vkCmdCopyImage(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     srcImage,
		VkImageLayout                               srcImageLayout,
		VkImage                                     dstImage,
		VkImageLayout                               dstImageLayout,
		uint32_t                                    regionCount,
		const VkImageCopy*                          pRegions );

	Gl4Renderer_API void VKAPI_CALL vkCmdBlitImage(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     srcImage,
		VkImageLayout                               srcImageLayout,
		VkImage                                     dstImage,
		VkImageLayout                               dstImageLayout,
		uint32_t                                    regionCount,
		const VkImageBlit*                          pRegions,
		VkFilter                                    filter );

	Gl4Renderer_API void VKAPI_CALL vkCmdCopyBufferToImage(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    srcBuffer,
		VkImage                                     dstImage,
		VkImageLayout                               dstImageLayout,
		uint32_t                                    regionCount,
		const VkBufferImageCopy*                    pRegions );

	Gl4Renderer_API void VKAPI_CALL vkCmdCopyImageToBuffer(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     srcImage,
		VkImageLayout                               srcImageLayout,
		VkBuffer                                    dstBuffer,
		uint32_t                                    regionCount,
		const VkBufferImageCopy*                    pRegions );

	Gl4Renderer_API void VKAPI_CALL vkCmdUpdateBuffer(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    dstBuffer,
		VkDeviceSize                                dstOffset,
		VkDeviceSize                                dataSize,
		const void*                                 pData );

	Gl4Renderer_API void VKAPI_CALL vkCmdFillBuffer(
		VkCommandBuffer                             commandBuffer,
		VkBuffer                                    dstBuffer,
		VkDeviceSize                                dstOffset,
		VkDeviceSize                                size,
		uint32_t                                    data );

	Gl4Renderer_API void VKAPI_CALL vkCmdClearColorImage(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     image,
		VkImageLayout                               imageLayout,
		const VkClearColorValue*                    pColor,
		uint32_t                                    rangeCount,
		const VkImageSubresourceRange*              pRanges );

	Gl4Renderer_API void VKAPI_CALL vkCmdClearDepthStencilImage(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     image,
		VkImageLayout                               imageLayout,
		const VkClearDepthStencilValue*             pDepthStencil,
		uint32_t                                    rangeCount,
		const VkImageSubresourceRange*              pRanges );

	Gl4Renderer_API void VKAPI_CALL vkCmdClearAttachments(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    attachmentCount,
		const VkClearAttachment*                    pAttachments,
		uint32_t                                    rectCount,
		const VkClearRect*                          pRects );

	Gl4Renderer_API void VKAPI_CALL vkCmdResolveImage(
		VkCommandBuffer                             commandBuffer,
		VkImage                                     srcImage,
		VkImageLayout                               srcImageLayout,
		VkImage                                     dstImage,
		VkImageLayout                               dstImageLayout,
		uint32_t                                    regionCount,
		const VkImageResolve*                       pRegions );

	Gl4Renderer_API void VKAPI_CALL vkCmdSetEvent(
		VkCommandBuffer                             commandBuffer,
		VkEvent                                     event,
		VkPipelineStageFlags                        stageMask );

	Gl4Renderer_API void VKAPI_CALL vkCmdResetEvent(
		VkCommandBuffer                             commandBuffer,
		VkEvent                                     event,
		VkPipelineStageFlags                        stageMask );

	Gl4Renderer_API void VKAPI_CALL vkCmdWaitEvents(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    eventCount,
		const VkEvent*                              pEvents,
		VkPipelineStageFlags                        srcStageMask,
		VkPipelineStageFlags                        dstStageMask,
		uint32_t                                    memoryBarrierCount,
		const VkMemoryBarrier*                      pMemoryBarriers,
		uint32_t                                    bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
		uint32_t                                    imageMemoryBarrierCount,
		const VkImageMemoryBarrier*                 pImageMemoryBarriers );

	Gl4Renderer_API void VKAPI_CALL vkCmdPipelineBarrier(
		VkCommandBuffer                             commandBuffer,
		VkPipelineStageFlags                        srcStageMask,
		VkPipelineStageFlags                        dstStageMask,
		VkDependencyFlags                           dependencyFlags,
		uint32_t                                    memoryBarrierCount,
		const VkMemoryBarrier*                      pMemoryBarriers,
		uint32_t                                    bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
		uint32_t                                    imageMemoryBarrierCount,
		const VkImageMemoryBarrier*                 pImageMemoryBarriers );

	Gl4Renderer_API void VKAPI_CALL vkCmdBeginQuery(
		VkCommandBuffer                             commandBuffer,
		VkQueryPool                                 queryPool,
		uint32_t                                    query,
		VkQueryControlFlags                         flags );

	Gl4Renderer_API void VKAPI_CALL vkCmdEndQuery(
		VkCommandBuffer                             commandBuffer,
		VkQueryPool                                 queryPool,
		uint32_t                                    query );

	Gl4Renderer_API void VKAPI_CALL vkCmdResetQueryPool(
		VkCommandBuffer                             commandBuffer,
		VkQueryPool                                 queryPool,
		uint32_t                                    firstQuery,
		uint32_t                                    queryCount );

	Gl4Renderer_API void VKAPI_CALL vkCmdWriteTimestamp(
		VkCommandBuffer                             commandBuffer,
		VkPipelineStageFlagBits                     pipelineStage,
		VkQueryPool                                 queryPool,
		uint32_t                                    query );

	Gl4Renderer_API void VKAPI_CALL vkCmdCopyQueryPoolResults(
		VkCommandBuffer                             commandBuffer,
		VkQueryPool                                 queryPool,
		uint32_t                                    firstQuery,
		uint32_t                                    queryCount,
		VkBuffer                                    dstBuffer,
		VkDeviceSize                                dstOffset,
		VkDeviceSize                                stride,
		VkQueryResultFlags                          flags );

	Gl4Renderer_API void VKAPI_CALL vkCmdPushConstants(
		VkCommandBuffer                             commandBuffer,
		VkPipelineLayout                            layout,
		VkShaderStageFlags                          stageFlags,
		uint32_t                                    offset,
		uint32_t                                    size,
		const void*                                 pValues );

	Gl4Renderer_API void VKAPI_CALL vkCmdBeginRenderPass(
		VkCommandBuffer                             commandBuffer,
		const VkRenderPassBeginInfo*                pRenderPassBegin,
		VkSubpassContents                           contents );

	Gl4Renderer_API void VKAPI_CALL vkCmdNextSubpass(
		VkCommandBuffer                             commandBuffer,
		VkSubpassContents                           contents );

	Gl4Renderer_API void VKAPI_CALL vkCmdEndRenderPass(
		VkCommandBuffer                             commandBuffer );

	Gl4Renderer_API void VKAPI_CALL vkCmdExecuteCommands(
		VkCommandBuffer                             commandBuffer,
		uint32_t                                    commandBufferCount,
		const VkCommandBuffer*                      pCommandBuffers );
#ifdef __cplusplus
}
#endif

