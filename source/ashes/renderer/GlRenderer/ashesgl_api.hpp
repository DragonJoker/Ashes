/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Command/GlCommandBuffer.hpp"
#include "Command/GlCommandPool.hpp"
#include "Command/GlQueue.hpp"
#include "Core/GlDebugReportCallback.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlDisplay.hpp"
#include "Core/GlDisplayMode.hpp"
#include "Core/GlInstance.hpp"
#include "Core/GlPhysicalDevice.hpp"
#include "Core/GlSurface.hpp"
#include "Core/GlSwapChain.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Descriptor/GlDescriptorSet.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Miscellaneous/GlQueryPool.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlSampler.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineCache.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"
#include "Sync/GlEvent.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"

#include <ashes/common/Exception.hpp>
#include <ashes/common/VkTypeTraits.hpp>

#define VK_NO_PROTOTYPES
#include <ashes/ashes.h>

#include <limits>

namespace ashes::gl
{
	static int constexpr MinMajor = 3;
	static int constexpr MinMinor = 2;
	static int constexpr MaxMajor = 10;
	static int constexpr MaxMinor = 10;

	template< typename T >
	static constexpr T NonAvailable = std::numeric_limits< T >::max();

	template< typename VkType  >
	struct VkGlTypeTraits;

	template< typename Type  >
	struct GlVkTypeTraits;

#define VK_IMPLEMENT_HANDLE(object)\
	template<>\
	struct VkGlTypeTraits< Vk##object >\
	{\
		using Type = object;\
	};\
	template<>\
	struct GlVkTypeTraits< object >\
	{\
		using VkType = Vk##object;\
	}

	VK_IMPLEMENT_HANDLE( Instance );
	VK_IMPLEMENT_HANDLE( PhysicalDevice );
	VK_IMPLEMENT_HANDLE( Device );
	VK_IMPLEMENT_HANDLE( DeviceMemory );
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
	VK_IMPLEMENT_HANDLE( PipelineCache );
	VK_IMPLEMENT_HANDLE( PipelineLayout );
	VK_IMPLEMENT_HANDLE( RenderPass );
	VK_IMPLEMENT_HANDLE( Pipeline );
	VK_IMPLEMENT_HANDLE( DescriptorSetLayout );
	VK_IMPLEMENT_HANDLE( Sampler );
	VK_IMPLEMENT_HANDLE( DescriptorPool );
	VK_IMPLEMENT_HANDLE( DescriptorSet );
	VK_IMPLEMENT_HANDLE( CommandPool );
	VK_IMPLEMENT_HANDLE( Framebuffer );
#ifdef VK_KHR_surface
	VK_IMPLEMENT_HANDLE( SurfaceKHR );
#endif
#ifdef VK_KHR_swapchain
	VK_IMPLEMENT_HANDLE( SwapchainKHR );
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
	VK_IMPLEMENT_HANDLE( SamplerYcbcrConversion );
#endif
#ifdef VK_KHR_push_descriptor
	VK_IMPLEMENT_HANDLE( DescriptorUpdateTemplate );
#endif
#ifdef VK_KHR_display
	VK_IMPLEMENT_HANDLE( DisplayKHR );
	VK_IMPLEMENT_HANDLE( DisplayModeKHR );
#endif
#ifdef VK_EXT_debug_report
	VK_IMPLEMENT_HANDLE( DebugReportCallbackEXT );
#endif
#ifdef VK_NVX_device_generated_commands
	VK_IMPLEMENT_HANDLE( ObjectTableNVX );
	VK_IMPLEMENT_HANDLE( IndirectCommandsLayoutNVX );
#endif
#ifdef VK_EXT_debug_utils
	VK_IMPLEMENT_HANDLE( DebugUtilsMessengerEXT );
#endif
#ifdef VK_EXT_validation_cache
	VK_IMPLEMENT_HANDLE( ValidationCacheEXT );
#endif

	template< typename VkType >
	typename VkGlTypeTraits< VkType >::Type * get( VkType vkValue )
	{
		using Type = typename VkGlTypeTraits< VkType >::Type;
		return ( ( Type * )vkValue );
	}

	template< typename Type >
	typename GlVkTypeTraits< Type >::VkType get( Type * vkValue )
	{
		using VkType = typename GlVkTypeTraits< Type >::VkType;
		return VkType( vkValue );
	}

	template< typename Type >
	typename GlVkTypeTraits< Type >::VkType get( Type const * vkValue )
	{
		using VkType = typename GlVkTypeTraits< Type >::VkType;
		return VkType( vkValue );
	}

	template< typename VkType, typename ... Params >
	VkResult allocate( VkType & vkValue
		, const VkAllocationCallbacks * allocInfo
		, Params && ... params )
	{
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			using Type = typename VkGlTypeTraits< VkType >::Type;
			vkValue = VkType( new Type{ std::forward< Params && >( params )... } );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			result = exc.getResult();
		}
		catch ( std::exception & exc )
		{
			std::cerr << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Unknown error" << std::endl;
		}

		return result;
	}

	template< typename VkType >
	VkResult deallocate( VkType & vkValue, const VkAllocationCallbacks * allocInfo )
	{
		delete get( vkValue );
		vkValue = nullptr;
		return VK_SUCCESS;
	}

	std::vector< VkExtensionProperties > const & getSupportedInstanceExtensions();
	std::vector< VkLayerProperties > const & getInstanceLayerProperties();

	inline VkInstance getInstance( VkInstance object )
	{
		return object;
	}

	inline VkInstance getInstance( VkPhysicalDevice object )
	{
		return get( object )->getInstance();
	}

	inline VkInstance getInstance( VkSurfaceKHR object )
	{
		return get( object )->getInstance();
	}

	inline VkInstance getInstance( VkDevice object )
	{
		return get( object )->getInstance();
	}
	
	inline VkInstance getInstance( VkBufferView object )
	{
		return getInstance( get( object )->getDevice() );
	}

	inline VkInstance getInstance( VkCommandBuffer object )
	{
		return getInstance( get( object )->getDevice() );
	}

	inline VkInstance getInstance( VkQueue object )
	{
		return getInstance( get( object )->getDevice() );
	}

	inline VkInstance getInstance( VkQueryPool object )
	{
		return getInstance( get( object )->getDevice() );
	}

	inline VkInstance getInstance( VkShaderModule object )
	{
		return getInstance( get( object )->getDevice() );
	}

	inline VkInstance getInstance( VkPipelineLayout object )
	{
		return getInstance( get( object )->getDevice() );
	}

	inline VkInstance getInstance( VkPipeline object )
	{
		return getInstance( get( object )->getLayout() );
	}

#if VK_EXT_debug_utils

	template< typename VkObject >
	inline void debugUtilsSubmit( VkDebugUtilsMessageSeverityFlagBitsEXT severity
		, VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
		VkInstance instance = getInstance( object );
		VkDebugUtilsObjectNameInfoEXT objectName
		{
			VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
			nullptr,
			ashes::VkTypeTraits< VkObject >::UtilsValue,
			uint64_t( object ),
			ashes::VkTypeTraits< VkObject >::getName().c_str(),
		};
		get( instance )->submitDebugUtilsMessenger( severity
			, VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			, {
				VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT,
				nullptr,
				0u,
				errorName.c_str(),
				result,
				name.c_str(),
				0u,
				nullptr,
				0u,
				nullptr,
				1u,
				&objectName,
			} );
	}

#endif
#if VK_EXT_debug_report

	template< typename VkObject >
	inline void debugReportMessage( VkDebugReportFlagBitsEXT report
		, VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
		VkInstance instance = getInstance( object );
		std::string text = errorName + ": " + name;
		get( instance )->reportMessage( report
			, ashes::VkTypeTraits< VkObject >::ReportValue
			, uint64_t( object )
			, 0u
			, result
			, "OpenGL"
			, text.c_str() );
	}

#endif

	template< typename VkObject >
	using Reporter = void ( * )( VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name );

	template< typename VkObject >
	inline void report( Reporter< VkObject > reporter
		, VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
		reporter( object, result, errorName, name );
	}

	template< typename VkObject >
	inline void reportError( VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
#if VK_EXT_debug_utils
		debugUtilsSubmit( VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			, object
			, result
			, errorName
			, name );
#endif
#if VK_EXT_debug_report
		debugReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
			, object
			, result
			, errorName
			, name );
#endif
	}

	template< typename VkObject >
	inline void reportWarning( VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
#if VK_EXT_debug_utils
		debugUtilsSubmit( VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			, object
			, result
			, errorName
			, name );
#endif
#if VK_EXT_debug_report
		debugReportMessage( VK_DEBUG_REPORT_WARNING_BIT_EXT
			, object
			, result
			, errorName
			, name );
#endif
	}

	template< typename VkObject >
	inline VkResult reportUnsupported( VkObject object
		, std::string const & name )
	{
		reportError( object
			, VK_ERROR_FEATURE_NOT_PRESENT
			, "Unsupported feature"
			, name );
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma region Vulkan 1.0
#ifdef VK_VERSION_1_0

	PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance instance,
		const char * pName );
	PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
		VkDevice device,
		const char * pName );
	VkResult VKAPI_CALL vkCreateInstance(
		const VkInstanceCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkInstance * pInstance );
	void VKAPI_CALL vkDestroyInstance(
		VkInstance instance,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkEnumeratePhysicalDevices(
		VkInstance instance,
		uint32_t * pPhysicalDeviceCount,
		VkPhysicalDevice * pPhysicalDevices );
	void VKAPI_CALL vkGetPhysicalDeviceFeatures(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures * pFeatures );
	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties * pFormatProperties );
	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkImageCreateFlags flags,
		VkImageFormatProperties * pImageFormatProperties );
	void VKAPI_CALL vkGetPhysicalDeviceProperties(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties * pProperties );
	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties * pQueueFamilyProperties );
	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties * pMemoryProperties );
	VkResult VKAPI_CALL vkCreateDevice(
		VkPhysicalDevice physicalDevice,
		const VkDeviceCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDevice * pDevice );
	void VKAPI_CALL vkDestroyDevice(
		VkDevice device,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(
		const char * pLayerName,
		uint32_t * pPropertyCount,
		VkExtensionProperties * pProperties );
	VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(
		VkPhysicalDevice physicalDevice,
		const char * pLayerName,
		uint32_t * pPropertyCount,
		VkExtensionProperties * pProperties );
	VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(
		uint32_t * pPropertyCount,
		VkLayerProperties * pProperties );
	VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkLayerProperties * pProperties );
	void VKAPI_CALL vkGetDeviceQueue(
		VkDevice device,
		uint32_t queueFamilyIndex,
		uint32_t queueIndex,
		VkQueue * pQueue );
	VkResult VKAPI_CALL vkQueueSubmit(
		VkQueue queue,
		uint32_t submitCount,
		const VkSubmitInfo * pSubmits,
		VkFence fence );
	VkResult VKAPI_CALL vkQueueWaitIdle(
		VkQueue queue );
	VkResult VKAPI_CALL vkDeviceWaitIdle(
		VkDevice device );
	VkResult VKAPI_CALL vkAllocateMemory(
		VkDevice device,
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory );
	void VKAPI_CALL vkFreeMemory(
		VkDevice device,
		VkDeviceMemory memory,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkMapMemory(
		VkDevice device,
		VkDeviceMemory memory,
		VkDeviceSize offset,
		VkDeviceSize size,
		VkMemoryMapFlags flags,
		void ** ppData );
	void VKAPI_CALL vkUnmapMemory(
		VkDevice device,
		VkDeviceMemory memory );
	VkResult VKAPI_CALL vkFlushMappedMemoryRanges(
		VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange * pMemoryRanges );
	VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(
		VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange * pMemoryRanges );
	void VKAPI_CALL vkGetDeviceMemoryCommitment(
		VkDevice device,
		VkDeviceMemory memory,
		VkDeviceSize * pCommittedMemoryInBytes );
	VkResult VKAPI_CALL vkBindBufferMemory(
		VkDevice device,
		VkBuffer buffer,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset );
	VkResult VKAPI_CALL vkBindImageMemory(
		VkDevice device,
		VkImage image,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset );
	void VKAPI_CALL vkGetBufferMemoryRequirements(
		VkDevice device,
		VkBuffer buffer,
		VkMemoryRequirements * pMemoryRequirements );
	void VKAPI_CALL vkGetImageMemoryRequirements(
		VkDevice device,
		VkImage image,
		VkMemoryRequirements * pMemoryRequirements );
	void VKAPI_CALL vkGetImageSparseMemoryRequirements(
		VkDevice device,
		VkImage image,
		uint32_t * pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements * pSparseMemoryRequirements );
	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkSampleCountFlagBits samples,
		VkImageUsageFlags usage,
		VkImageTiling tiling,
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties * pProperties );
	VkResult VKAPI_CALL vkQueueBindSparse(
		VkQueue queue,
		uint32_t bindInfoCount,
		const VkBindSparseInfo * pBindInfo,
		VkFence fence );
	VkResult VKAPI_CALL vkCreateFence(
		VkDevice device,
		const VkFenceCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkFence * pFence );
	void VKAPI_CALL vkDestroyFence(
		VkDevice device,
		VkFence fence,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkResetFences(
		VkDevice device,
		uint32_t fenceCount,
		const VkFence * pFences );
	VkResult VKAPI_CALL vkGetFenceStatus(
		VkDevice device,
		VkFence fence );
	VkResult VKAPI_CALL vkWaitForFences(
		VkDevice device,
		uint32_t fenceCount,
		const VkFence * pFences,
		VkBool32 waitAll,
		uint64_t timeout );
	VkResult VKAPI_CALL vkCreateSemaphore(
		VkDevice device,
		const VkSemaphoreCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSemaphore * pSemaphore );
	void VKAPI_CALL vkDestroySemaphore(
		VkDevice device,
		VkSemaphore semaphore,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateEvent(
		VkDevice device,
		const VkEventCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkEvent * pEvent );
	void VKAPI_CALL vkDestroyEvent(
		VkDevice device,
		VkEvent event,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkGetEventStatus(
		VkDevice device,
		VkEvent event );
	VkResult VKAPI_CALL vkSetEvent(
		VkDevice device,
		VkEvent event );
	VkResult VKAPI_CALL vkResetEvent(
		VkDevice device,
		VkEvent event );
	VkResult VKAPI_CALL vkCreateQueryPool(
		VkDevice device,
		const VkQueryPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkQueryPool * pQueryPool );
	void VKAPI_CALL vkDestroyQueryPool(
		VkDevice device,
		VkQueryPool queryPool,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkGetQueryPoolResults(
		VkDevice device,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount,
		size_t dataSize,
		void * pData,
		VkDeviceSize stride,
		VkQueryResultFlags flags );
	VkResult VKAPI_CALL vkCreateBuffer(
		VkDevice device,
		const VkBufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBuffer * pBuffer );
	void VKAPI_CALL vkDestroyBuffer(
		VkDevice device,
		VkBuffer buffer,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateBufferView(
		VkDevice device,
		const VkBufferViewCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBufferView * pView );
	void VKAPI_CALL vkDestroyBufferView(
		VkDevice device,
		VkBufferView bufferView,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateImage(
		VkDevice device,
		const VkImageCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkImage * pImage );
	void VKAPI_CALL vkDestroyImage(
		VkDevice device,
		VkImage image,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkGetImageSubresourceLayout(
		VkDevice device,
		VkImage image,
		const VkImageSubresource * pSubresource,
		VkSubresourceLayout * pLayout );
	VkResult VKAPI_CALL vkCreateImageView(
		VkDevice device,
		const VkImageViewCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkImageView * pView );
	void VKAPI_CALL vkDestroyImageView(
		VkDevice device,
		VkImageView imageView,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateShaderModule(
		VkDevice device,
		const VkShaderModuleCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkShaderModule * pShaderModule );
	void VKAPI_CALL vkDestroyShaderModule(
		VkDevice device,
		VkShaderModule shaderModule,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreatePipelineCache(
		VkDevice device,
		const VkPipelineCacheCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkPipelineCache * pPipelineCache );
	void VKAPI_CALL vkDestroyPipelineCache(
		VkDevice device,
		VkPipelineCache pipelineCache,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkGetPipelineCacheData(
		VkDevice device,
		VkPipelineCache pipelineCache,
		size_t * pDataSize,
		void * pData );
	VkResult VKAPI_CALL vkMergePipelineCaches(
		VkDevice device,
		VkPipelineCache dstCache,
		uint32_t srcCacheCount,
		const VkPipelineCache * pSrcCaches );
	VkResult VKAPI_CALL vkCreateGraphicsPipelines(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkGraphicsPipelineCreateInfo * pCreateInfos,
		const VkAllocationCallbacks * pAllocator,
		VkPipeline * pPipelines );
	VkResult VKAPI_CALL vkCreateComputePipelines(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkComputePipelineCreateInfo * pCreateInfos,
		const VkAllocationCallbacks * pAllocator,
		VkPipeline * pPipelines );
	void VKAPI_CALL vkDestroyPipeline(
		VkDevice device,
		VkPipeline pipeline,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreatePipelineLayout(
		VkDevice device,
		const VkPipelineLayoutCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkPipelineLayout * pPipelineLayout );
	void VKAPI_CALL vkDestroyPipelineLayout(
		VkDevice device,
		VkPipelineLayout pipelineLayout,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateSampler(
		VkDevice device,
		const VkSamplerCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSampler * pSampler );
	void VKAPI_CALL vkDestroySampler(
		VkDevice device,
		VkSampler sampler,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateDescriptorSetLayout(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorSetLayout * pSetLayout );
	void VKAPI_CALL vkDestroyDescriptorSetLayout(
		VkDevice device,
		VkDescriptorSetLayout descriptorSetLayout,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateDescriptorPool(
		VkDevice device,
		const VkDescriptorPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorPool * pDescriptorPool );
	void VKAPI_CALL vkDestroyDescriptorPool(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkResetDescriptorPool(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		VkDescriptorPoolResetFlags flags );
	VkResult VKAPI_CALL vkAllocateDescriptorSets(
		VkDevice device,
		const VkDescriptorSetAllocateInfo * pAllocateInfo,
		VkDescriptorSet * pDescriptorSets );
	VkResult VKAPI_CALL vkFreeDescriptorSets(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		uint32_t descriptorSetCount,
		const VkDescriptorSet * pDescriptorSets );
	void VKAPI_CALL vkUpdateDescriptorSets(
		VkDevice device,
		uint32_t descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites,
		uint32_t descriptorCopyCount,
		const VkCopyDescriptorSet * pDescriptorCopies );
	VkResult VKAPI_CALL vkCreateFramebuffer(
		VkDevice device,
		const VkFramebufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkFramebuffer * pFramebuffer );
	void VKAPI_CALL vkDestroyFramebuffer(
		VkDevice device,
		VkFramebuffer framebuffer,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateRenderPass(
		VkDevice device,
		const VkRenderPassCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkRenderPass * pRenderPass );
	void VKAPI_CALL vkDestroyRenderPass(
		VkDevice device,
		VkRenderPass renderPass,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkGetRenderAreaGranularity(
		VkDevice device,
		VkRenderPass renderPass,
		VkExtent2D * pGranularity );
	VkResult VKAPI_CALL vkCreateCommandPool(
		VkDevice device,
		const VkCommandPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkCommandPool * pCommandPool );
	void VKAPI_CALL vkDestroyCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkResetCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolResetFlags flags );
	VkResult VKAPI_CALL vkAllocateCommandBuffers(
		VkDevice device,
		const VkCommandBufferAllocateInfo * pAllocateInfo,
		VkCommandBuffer * pCommandBuffers );
	void VKAPI_CALL vkFreeCommandBuffers(
		VkDevice device,
		VkCommandPool commandPool,
		uint32_t commandBufferCount,
		const VkCommandBuffer * pCommandBuffers );
	VkResult VKAPI_CALL vkBeginCommandBuffer(
		VkCommandBuffer commandBuffer,
		const VkCommandBufferBeginInfo * pBeginInfo );
	VkResult VKAPI_CALL vkEndCommandBuffer(
		VkCommandBuffer commandBuffer );
	VkResult VKAPI_CALL vkResetCommandBuffer(
		VkCommandBuffer commandBuffer,
		VkCommandBufferResetFlags flags );
	void VKAPI_CALL vkCmdBindPipeline(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipeline pipeline );
	void VKAPI_CALL vkCmdSetViewport(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkViewport * pViewports );
	void VKAPI_CALL vkCmdSetScissor(
		VkCommandBuffer commandBuffer,
		uint32_t firstScissor,
		uint32_t scissorCount,
		const VkRect2D * pScissors );
	void VKAPI_CALL vkCmdSetLineWidth(
		VkCommandBuffer commandBuffer,
		float lineWidth );
	void VKAPI_CALL vkCmdSetDepthBias(
		VkCommandBuffer commandBuffer,
		float depthBiasConstantFactor,
		float depthBiasClamp,
		float depthBiasSlopeFactor );
	void VKAPI_CALL vkCmdSetBlendConstants(
		VkCommandBuffer commandBuffer,
		const float blendConstants[4] );
	void VKAPI_CALL vkCmdSetDepthBounds(
		VkCommandBuffer commandBuffer,
		float minDepthBounds,
		float maxDepthBounds );
	void VKAPI_CALL vkCmdSetStencilCompareMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t compareMask );
	void VKAPI_CALL vkCmdSetStencilWriteMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t writeMask );
	void VKAPI_CALL vkCmdSetStencilReference(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t reference );
	void VKAPI_CALL vkCmdBindDescriptorSets(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipelineLayout layout,
		uint32_t firstSet,
		uint32_t descriptorSetCount,
		const VkDescriptorSet * pDescriptorSets,
		uint32_t dynamicOffsetCount,
		const uint32_t * pDynamicOffsets );
	void VKAPI_CALL vkCmdBindIndexBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkIndexType indexType );
	void VKAPI_CALL vkCmdBindVertexBuffers(
		VkCommandBuffer commandBuffer,
		uint32_t firstBinding,
		uint32_t bindingCount,
		const VkBuffer * pBuffers,
		const VkDeviceSize * pOffsets );
	void VKAPI_CALL vkCmdDraw(
		VkCommandBuffer commandBuffer,
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstance );
	void VKAPI_CALL vkCmdDrawIndexed(
		VkCommandBuffer commandBuffer,
		uint32_t indexCount,
		uint32_t instanceCount,
		uint32_t firstIndex,
		int32_t vertexOffset,
		uint32_t firstInstance );
	void VKAPI_CALL vkCmdDrawIndirect(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride );
	void VKAPI_CALL vkCmdDrawIndexedIndirect(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride );
	void VKAPI_CALL vkCmdDispatch(
		VkCommandBuffer commandBuffer,
		uint32_t groupCountX,
		uint32_t groupCountY,
		uint32_t groupCountZ );
	void VKAPI_CALL vkCmdDispatchIndirect(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset );
	void VKAPI_CALL vkCmdCopyBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer srcBuffer,
		VkBuffer dstBuffer,
		uint32_t regionCount,
		const VkBufferCopy * pRegions );
	void VKAPI_CALL vkCmdCopyImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageCopy * pRegions );
	void VKAPI_CALL vkCmdBlitImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageBlit * pRegions,
		VkFilter filter );
	void VKAPI_CALL vkCmdCopyBufferToImage(
		VkCommandBuffer commandBuffer,
		VkBuffer srcBuffer,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkBufferImageCopy * pRegions );
	void VKAPI_CALL vkCmdCopyImageToBuffer(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkBuffer dstBuffer,
		uint32_t regionCount,
		const VkBufferImageCopy * pRegions );
	void VKAPI_CALL vkCmdUpdateBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize dataSize,
		const void * pData );
	void VKAPI_CALL vkCmdFillBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize size,
		uint32_t data );
	void VKAPI_CALL vkCmdClearColorImage(
		VkCommandBuffer commandBuffer,
		VkImage image,
		VkImageLayout imageLayout,
		const VkClearColorValue * pColor,
		uint32_t rangeCount,
		const VkImageSubresourceRange * pRanges );
	void VKAPI_CALL vkCmdClearDepthStencilImage(
		VkCommandBuffer commandBuffer,
		VkImage image,
		VkImageLayout imageLayout,
		const VkClearDepthStencilValue * pDepthStencil,
		uint32_t rangeCount,
		const VkImageSubresourceRange * pRanges );
	void VKAPI_CALL vkCmdClearAttachments(
		VkCommandBuffer commandBuffer,
		uint32_t attachmentCount,
		const VkClearAttachment * pAttachments,
		uint32_t rectCount,
		const VkClearRect * pRects );
	void VKAPI_CALL vkCmdResolveImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageResolve * pRegions );
	void VKAPI_CALL vkCmdSetEvent(
		VkCommandBuffer commandBuffer,
		VkEvent event,
		VkPipelineStageFlags stageMask );
	void VKAPI_CALL vkCmdResetEvent(
		VkCommandBuffer commandBuffer,
		VkEvent event,
		VkPipelineStageFlags stageMask );
	void VKAPI_CALL vkCmdWaitEvents(
		VkCommandBuffer commandBuffer,
		uint32_t eventCount,
		const VkEvent * pEvents,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		uint32_t memoryBarrierCount,
		const VkMemoryBarrier * pMemoryBarriers,
		uint32_t bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier * pBufferMemoryBarriers,
		uint32_t imageMemoryBarrierCount,
		const VkImageMemoryBarrier * pImageMemoryBarriers );
	void VKAPI_CALL vkCmdPipelineBarrier(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkDependencyFlags dependencyFlags,
		uint32_t memoryBarrierCount,
		const VkMemoryBarrier * pMemoryBarriers,
		uint32_t bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier * pBufferMemoryBarriers,
		uint32_t imageMemoryBarrierCount,
		const VkImageMemoryBarrier * pImageMemoryBarriers );
	void VKAPI_CALL vkCmdBeginQuery(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		VkQueryControlFlags flags );
	void VKAPI_CALL vkCmdEndQuery(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query );
	void VKAPI_CALL vkCmdResetQueryPool(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount );
	void VKAPI_CALL vkCmdWriteTimestamp(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlagBits pipelineStage,
		VkQueryPool queryPool,
		uint32_t query );
	void VKAPI_CALL vkCmdCopyQueryPoolResults(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize stride,
		VkQueryResultFlags flags );
	void VKAPI_CALL vkCmdPushConstants(
		VkCommandBuffer commandBuffer,
		VkPipelineLayout layout,
		VkShaderStageFlags stageFlags,
		uint32_t offset,
		uint32_t size,
		const void * pValues );
	void VKAPI_CALL vkCmdBeginRenderPass(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo * pRenderPassBegin,
		VkSubpassContents contents );
	void VKAPI_CALL vkCmdNextSubpass(
		VkCommandBuffer commandBuffer,
		VkSubpassContents contents );
	void VKAPI_CALL vkCmdEndRenderPass(
		VkCommandBuffer commandBuffer );
	void VKAPI_CALL vkCmdExecuteCommands(
		VkCommandBuffer commandBuffer,
		uint32_t commandBufferCount,
		const VkCommandBuffer * pCommandBuffers );

#endif
#pragma endregion
#pragma region Vulkan 1.1
#ifdef VK_VERSION_1_1

	VkResult VKAPI_PTR vkEnumerateInstanceVersion( uint32_t * version );
	VkResult VKAPI_CALL vkBindBufferMemory2(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo * pBindInfos );
	VkResult VKAPI_CALL vkBindImageMemory2(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo * pBindInfos );
	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(
		VkDevice device,
		uint32_t heapIndex,
		uint32_t localDeviceIndex,
		uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags * pPeerMemoryFeatures );
	void VKAPI_CALL vkCmdSetDeviceMask(
		VkCommandBuffer commandBuffer,
		uint32_t deviceMask );
	void VKAPI_CALL vkCmdDispatchBase(
		VkCommandBuffer commandBuffer,
		uint32_t baseGroupX,
		uint32_t baseGroupY,
		uint32_t baseGroupZ,
		uint32_t groupCountX,
		uint32_t groupCountY,
		uint32_t groupCountZ );
	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(
		VkInstance instance,
		uint32_t * pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties );
	void VKAPI_CALL vkGetImageMemoryRequirements2(
		VkDevice device,
		const VkImageMemoryRequirementsInfo2 * pInfo,
		VkMemoryRequirements2 * pMemoryRequirements );
	void VKAPI_CALL vkGetBufferMemoryRequirements2(
		VkDevice device,
		const VkBufferMemoryRequirementsInfo2 * pInfo,
		VkMemoryRequirements2 * pMemoryRequirements );
	void VKAPI_CALL vkGetImageSparseMemoryRequirements2(
		VkDevice device,
		const VkImageSparseMemoryRequirementsInfo2 * pInfo,
		uint32_t * pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements );
	void VKAPI_CALL vkGetPhysicalDeviceFeatures2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2 * pFeatures );
	void VKAPI_CALL vkGetPhysicalDeviceProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2 * pProperties );
	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2 * pFormatProperties );
	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2 * pImageFormatInfo,
		VkImageFormatProperties2 * pImageFormatProperties );
	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2 * pQueueFamilyProperties );
	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2 * pMemoryProperties );
	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2 * pFormatInfo,
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties2 * pProperties );
	void VKAPI_CALL vkTrimCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolTrimFlags flags );
	void VKAPI_CALL vkGetDeviceQueue2(
		VkDevice device,
		const VkDeviceQueueInfo2 * pQueueInfo,
		VkQueue * pQueue );
	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(
		VkDevice device,
		const VkSamplerYcbcrConversionCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSamplerYcbcrConversion * pYcbcrConversion );
	void VKAPI_CALL vkDestroySamplerYcbcrConversion(
		VkDevice device,
		VkSamplerYcbcrConversion ycbcrConversion,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(
		VkDevice device,
		const VkDescriptorUpdateTemplateCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorUpdateTemplate * pDescriptorUpdateTemplate );
	void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(
		VkDevice device,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(
		VkDevice device,
		VkDescriptorSet descriptorSet,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const void * pData );
	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalBufferInfo * pExternalBufferInfo,
		VkExternalBufferProperties * pExternalBufferProperties );
	void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalFenceInfo * pExternalFenceInfo,
		VkExternalFenceProperties * pExternalFenceProperties );
	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalSemaphoreInfo * pExternalSemaphoreInfo,
		VkExternalSemaphoreProperties * pExternalSemaphoreProperties );
	void VKAPI_CALL vkGetDescriptorSetLayoutSupport(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		VkDescriptorSetLayoutSupport * pSupport );

#endif
#pragma endregion
#pragma region VK_KHR_surface
#ifdef VK_KHR_surface

	void VKAPI_CALL vkDestroySurfaceKHR(
		VkInstance instance,
		VkSurfaceKHR surface,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		VkSurfaceKHR surface,
		VkBool32 * pSupported );
	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkSurfaceCapabilitiesKHR * pSurfaceCapabilities );
	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t * pSurfaceFormatCount,
		VkSurfaceFormatKHR * pSurfaceFormats );
	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t * pPresentModeCount,
		VkPresentModeKHR * pPresentModes );

#endif
#pragma endregion
#pragma region VK_KHR_swapchain
#ifdef VK_KHR_swapchain

	VkResult VKAPI_CALL vkCreateSwapchainKHR(
		VkDevice device,
		const VkSwapchainCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSwapchainKHR * pSwapchain );
	void VKAPI_CALL vkDestroySwapchainKHR(
		VkDevice device,
		VkSwapchainKHR swapchain,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkGetSwapchainImagesKHR(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint32_t * pSwapchainImageCount,
		VkImage * pSwapchainImages );
	VkResult VKAPI_CALL vkAcquireNextImageKHR(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint64_t timeout,
		VkSemaphore semaphore,
		VkFence fence,
		uint32_t * pImageIndex );
	VkResult VKAPI_CALL vkQueuePresentKHR(
		VkQueue queue,
		const VkPresentInfoKHR * pPresentInfo );

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(
		VkDevice device,
		VkDeviceGroupPresentCapabilitiesKHR * pDeviceGroupPresentCapabilities );
	VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(
		VkDevice device,
		VkSurfaceKHR surface,
		VkDeviceGroupPresentModeFlagsKHR * pModes );
	VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t * pRectCount,
		VkRect2D * pRects );
	VkResult VKAPI_CALL vkAcquireNextImage2KHR(
		VkDevice device,
		const VkAcquireNextImageInfoKHR * pAcquireInfo,
		uint32_t * pImageIndex );

#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPropertiesKHR * pProperties );
	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPlanePropertiesKHR * pProperties );
	VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t planeIndex,
		uint32_t * pDisplayCount,
		VkDisplayKHR * pDisplays );
	VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t * pPropertyCount,
		VkDisplayModePropertiesKHR * pProperties );
	VkResult VKAPI_CALL vkCreateDisplayModeKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		const VkDisplayModeCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDisplayModeKHR * pMode );
	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayModeKHR mode,
		uint32_t planeIndex,
		VkDisplayPlaneCapabilitiesKHR * pCapabilities );
	VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );

#endif
#pragma endregion
#pragma region VK_KHR_get_physical_device_properties2
#ifdef VK_KHR_get_physical_device_properties2

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2KHR * pFeatures );
	void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2KHR * pProperties );
	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2KHR * pFormatProperties );
	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2KHR * pImageFormatInfo,
		VkImageFormatProperties2KHR * pImageFormatProperties );
	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2KHR * pQueueFamilyProperties );
	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2KHR * pMemoryProperties );
	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2KHR * pFormatInfo,
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties2KHR * pProperties );

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(
		VkDevice device,
		uint32_t heapIndex,
		uint32_t localDeviceIndex,
		uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags * pPeerMemoryFeatures );
	void VKAPI_CALL vkCmdSetDeviceMaskKHR(
		VkCommandBuffer commandBuffer,
		uint32_t deviceMask );
	void VKAPI_CALL vkCmdDispatchBaseKHR(
		VkCommandBuffer commandBuffer,
		uint32_t baseGroupX,
		uint32_t baseGroupY,
		uint32_t baseGroupZ,
		uint32_t groupCountX,
		uint32_t groupCountY,
		uint32_t groupCountZ );

#endif
#pragma endregion
#pragma region VK_KHR_maintenance1
#ifdef VK_KHR_maintenance1

	void VKAPI_CALL vkTrimCommandPoolKHR(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolTrimFlagsKHR flags );

#endif
#pragma endregion
#pragma region VK_KHR_device_group_creation
#ifdef VK_KHR_device_group_creation

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(
		VkInstance instance,
		uint32_t * pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties );

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_capabilities
#ifdef VK_KHR_external_memory_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalBufferInfo * pExternalBufferInfo,
		VkExternalBufferProperties * pExternalBufferProperties );

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_fd
#ifdef VK_KHR_external_memory_fd

	VkResult VKAPI_CALL vkGetMemoryFdKHR(
		VkDevice device,
		const VkMemoryGetFdInfoKHR * pGetFdInfo,
		int * pFd );
	VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(
		VkDevice device,
		VkExternalMemoryHandleTypeFlagBits handleType,
		int fd,
		VkMemoryFdPropertiesKHR * pMemoryFdProperties );

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_capabilities
#ifdef VK_KHR_external_semaphore_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalSemaphoreInfo * pExternalSemaphoreInfo,
		VkExternalSemaphoreProperties * pExternalSemaphoreProperties );

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_fd
#ifdef VK_KHR_external_semaphore_fd

	VkResult VKAPI_CALL vkImportSemaphoreFdKHR(
		VkDevice device,
		const VkImportSemaphoreFdInfoKHR * pImportSemaphoreFdInfo );
	VkResult VKAPI_CALL vkGetSemaphoreFdKHR(
		VkDevice device,
		const VkSemaphoreGetFdInfoKHR * pGetFdInfo,
		int * pFd );

#endif
#pragma endregion
#pragma region VK_KHR_push_descriptor
#ifdef VK_KHR_push_descriptor

	void VKAPI_CALL vkCmdPushDescriptorSetKHR(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipelineLayout layout,
		uint32_t set,
		uint32_t descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites );
	void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(
		VkCommandBuffer commandBuffer,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		VkPipelineLayout layout,
		uint32_t set,
		const void * pData );

#endif
#pragma endregion
#pragma region VK_KHR_descriptor_update_template
#ifdef VK_KHR_descriptor_update_template

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(
		VkDevice device,
		const VkDescriptorUpdateTemplateCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorUpdateTemplate * pDescriptorUpdateTemplate );
	void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(
		VkDevice device,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(
		VkDevice device,
		VkDescriptorSet descriptorSet,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const void * pData );

#endif
#pragma endregion
#pragma region VK_KHR_create_renderpass2
#ifdef VK_KHR_create_renderpass2

	VkResult VKAPI_CALL vkCreateRenderPass2KHR(
		VkDevice device,
		const VkRenderPassCreateInfo2KHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkRenderPass * pRenderPass );
	void VKAPI_CALL vkCmdBeginRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo * pRenderPassBegin,
		const VkSubpassBeginInfoKHR * pSubpassBeginInfo );
	void VKAPI_CALL vkCmdNextSubpass2KHR(
		VkCommandBuffer commandBuffer,
		const VkSubpassBeginInfoKHR * pSubpassBeginInfo,
		const VkSubpassEndInfoKHR * pSubpassEndInfo );
	void VKAPI_CALL vkCmdEndRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		const VkSubpassEndInfoKHR * pSubpassEndInfo );

#endif
#pragma endregion
#pragma region VK_KHR_shared_presentable_image
#ifdef VK_KHR_shared_presentable_image

	VkResult VKAPI_CALL vkGetSwapchainStatusKHR(
		VkDevice device,
		VkSwapchainKHR swapchain );

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_capabilities
#ifdef VK_KHR_external_fence_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalFenceInfo * pExternalFenceInfo,
		VkExternalFenceProperties * pExternalFenceProperties );

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_fd
#ifdef VK_KHR_external_fence_fd

	VkResult VKAPI_CALL vkImportFenceFdKHR(
		VkDevice device,
		const VkImportFenceFdInfoKHR * pImportFenceFdInfo );
	VkResult VKAPI_CALL vkGetFenceFdKHR(
		VkDevice device,
		const VkFenceGetFdInfoKHR * pGetFdInfo,
		int * pFd );

#endif
#pragma endregion
#pragma region VK_KHR_get_surface_capabilities2
#ifdef VK_KHR_get_surface_capabilities2

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
		VkSurfaceCapabilities2KHR * pSurfaceCapabilities );
	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
		uint32_t * pSurfaceFormatCount,
		VkSurfaceFormat2KHR * pSurfaceFormats );

#endif
#pragma endregion
#pragma region VK_KHR_get_display_properties2
#ifdef VK_KHR_get_display_properties2

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayProperties2KHR * pProperties );
	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPlaneProperties2KHR * pProperties );
	VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t * pPropertyCount,
		VkDisplayModeProperties2KHR * pProperties );
	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		const VkDisplayPlaneInfo2KHR * pDisplayPlaneInfo,
		VkDisplayPlaneCapabilities2KHR * pCapabilities );

#endif
#pragma endregion
#pragma region VK_KHR_get_memory_requirements2
#ifdef VK_KHR_get_memory_requirements2

	void VKAPI_CALL vkGetImageMemoryRequirements2KHR(
		VkDevice device,
		const VkImageMemoryRequirementsInfo2 * pInfo,
		VkMemoryRequirements2 * pMemoryRequirements );
	void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(
		VkDevice device,
		const VkBufferMemoryRequirementsInfo2 * pInfo,
		VkMemoryRequirements2 * pMemoryRequirements );
	void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(
		VkDevice device,
		const VkImageSparseMemoryRequirementsInfo2 * pInfo,
		uint32_t * pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements );

#endif
#pragma endregion
#pragma region VK_KHR_sampler_ycbcr_conversion
#ifdef VK_KHR_sampler_ycbcr_conversion

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(
		VkDevice device,
		const VkSamplerYcbcrConversionCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSamplerYcbcrConversion * pYcbcrConversion );
	void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(
		VkDevice device,
		VkSamplerYcbcrConversion ycbcrConversion,
		const VkAllocationCallbacks * pAllocator );

#endif
#pragma endregion
#pragma region VK_KHR_bind_memory2
#ifdef VK_KHR_bind_memory2

	VkResult VKAPI_CALL vkBindBufferMemory2KHR(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo * pBindInfos );
	VkResult VKAPI_CALL vkBindImageMemory2KHR(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo * pBindInfos );

#endif
#pragma endregion
#pragma region VK_KHR_maintenance3
#ifdef VK_KHR_maintenance3

	void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		VkDescriptorSetLayoutSupport * pSupport );

#endif
#pragma endregion
#pragma region VK_KHR_draw_indirect_count
#ifdef VK_KHR_draw_indirect_count

	void VKAPI_CALL vkCmdDrawIndirectCountKHR(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride );
	void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride );

#endif
#pragma endregion
#pragma region VK_EXT_debug_report
#ifdef VK_EXT_debug_report

	VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDebugReportCallbackEXT * pCallback );
	void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
		VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkDebugReportMessageEXT(
		VkInstance instance,
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objectType,
		uint64_t object,
		size_t location,
		int32_t messageCode,
		const char * pLayerPrefix,
		const char * pMessage );

#endif
#pragma endregion
#pragma region VK_EXT_debug_marker
#ifdef VK_EXT_debug_marker

	VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT(
		VkDevice device,
		const VkDebugMarkerObjectTagInfoEXT * pTagInfo );
	VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(
		VkDevice device,
		const VkDebugMarkerObjectNameInfoEXT * pNameInfo );
	void VKAPI_CALL vkCmdDebugMarkerBeginEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT * pMarkerInfo );
	void VKAPI_CALL vkCmdDebugMarkerEndEXT(
		VkCommandBuffer commandBuffer );
	void VKAPI_CALL vkCmdDebugMarkerInsertEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT * pMarkerInfo );

#endif
#pragma endregion
#pragma region VK_EXT_transform_feedback
#ifdef VK_EXT_transform_feedback

	void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstBinding,
		uint32_t bindingCount,
		const VkBuffer * pBuffers,
		const VkDeviceSize * pOffsets,
		const VkDeviceSize * pSizes );
	void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstCounterBuffer,
		uint32_t counterBufferCount,
		const VkBuffer * pCounterBuffers,
		const VkDeviceSize * pCounterBufferOffsets );
	void VKAPI_CALL vkCmdEndTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstCounterBuffer,
		uint32_t counterBufferCount,
		const VkBuffer * pCounterBuffers,
		const VkDeviceSize * pCounterBufferOffsets );
	void VKAPI_CALL vkCmdBeginQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		VkQueryControlFlags flags,
		uint32_t index );
	void VKAPI_CALL vkCmdEndQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		uint32_t index );
	void VKAPI_CALL vkCmdDrawIndirectByteCountEXT(
		VkCommandBuffer commandBuffer,
		uint32_t instanceCount,
		uint32_t firstInstance,
		VkBuffer counterBuffer,
		VkDeviceSize counterBufferOffset,
		uint32_t counterOffset,
		uint32_t vertexStride );

#endif
#pragma endregion
#pragma region VK_AMD_draw_indirect_count
#ifdef VK_AMD_draw_indirect_count

	void VKAPI_CALL vkCmdDrawIndirectCountAMD(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride );
	void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride );

#endif
#pragma endregion
#pragma region VK_AMD_shader_info
#ifdef VK_AMD_shader_info

	VkResult VKAPI_CALL vkGetShaderInfoAMD(
		VkDevice device,
		VkPipeline pipeline,
		VkShaderStageFlagBits shaderStage,
		VkShaderInfoTypeAMD infoType,
		size_t * pInfoSize,
		void * pInfo );

#endif
#pragma endregion
#pragma region VK_NV_external_memory_capabilities
#ifdef VK_NV_external_memory_capabilities

	VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkImageCreateFlags flags,
		VkExternalMemoryHandleTypeFlagsNV externalHandleType,
		VkExternalImageFormatPropertiesNV * pExternalImageFormatProperties );

#endif
#pragma endregion
#pragma region VK_EXT_conditional_rendering
#ifdef VK_EXT_conditional_rendering

	void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(
		VkCommandBuffer commandBuffer,
		const VkConditionalRenderingBeginInfoEXT * pConditionalRenderingBegin );
	void VKAPI_CALL vkCmdEndConditionalRenderingEXT(
		VkCommandBuffer commandBuffer );

#endif
#pragma endregion
#pragma region VK_NVX_device_generated_commands
#ifdef VK_NVX_device_generated_commands

	void VKAPI_CALL vkCmdProcessCommandsNVX(
		VkCommandBuffer commandBuffer,
		const VkCmdProcessCommandsInfoNVX * pProcessCommandsInfo );
	void VKAPI_CALL vkCmdReserveSpaceForCommandsNVX(
		VkCommandBuffer commandBuffer,
		const VkCmdReserveSpaceForCommandsInfoNVX * pReserveSpaceInfo );
	VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNVX(
		VkDevice device,
		const VkIndirectCommandsLayoutCreateInfoNVX * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkIndirectCommandsLayoutNVX * pIndirectCommandsLayout );
	void VKAPI_CALL vkDestroyIndirectCommandsLayoutNVX(
		VkDevice device,
		VkIndirectCommandsLayoutNVX indirectCommandsLayout,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkCreateObjectTableNVX(
		VkDevice device,
		const VkObjectTableCreateInfoNVX * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkObjectTableNVX * pObjectTable );
	void VKAPI_CALL vkDestroyObjectTableNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkRegisterObjectsNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		uint32_t objectCount,
		const VkObjectTableEntryNVX * const * ppObjectTableEntries,
		const uint32_t * pObjectIndices );
	VkResult VKAPI_CALL vkUnregisterObjectsNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		uint32_t objectCount,
		const VkObjectEntryTypeNVX * pObjectEntryTypes,
		const uint32_t * pObjectIndices );
	void VKAPI_CALL vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(
		VkPhysicalDevice physicalDevice,
		VkDeviceGeneratedCommandsFeaturesNVX * pFeatures,
		VkDeviceGeneratedCommandsLimitsNVX * pLimits );

#endif
#pragma endregion
#pragma region VK_NV_clip_space_w_scaling
#ifdef VK_NV_clip_space_w_scaling

	void VKAPI_CALL vkCmdSetViewportWScalingNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkViewportWScalingNV * pViewportWScalings );

#endif
#pragma endregion
#pragma region VK_EXT_direct_mode_display
#ifdef VK_EXT_direct_mode_display

	VkResult VKAPI_CALL vkReleaseDisplayEXT(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display );

#endif
#pragma endregion
#pragma region VK_EXT_display_surface_counter
#ifdef VK_EXT_display_surface_counter

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkSurfaceCapabilities2EXT * pSurfaceCapabilities );

#endif
#pragma endregion
#pragma region VK_EXT_display_control
#ifdef VK_EXT_display_control

	VkResult VKAPI_CALL vkDisplayPowerControlEXT(
		VkDevice device,
		VkDisplayKHR display,
		const VkDisplayPowerInfoEXT * pDisplayPowerInfo );
	VkResult VKAPI_CALL vkRegisterDeviceEventEXT(
		VkDevice device,
		const VkDeviceEventInfoEXT * pDeviceEventInfo,
		const VkAllocationCallbacks * pAllocator,
		VkFence * pFence );
	VkResult VKAPI_CALL vkRegisterDisplayEventEXT(
		VkDevice device,
		VkDisplayKHR display,
		const VkDisplayEventInfoEXT * pDisplayEventInfo,
		const VkAllocationCallbacks * pAllocator,
		VkFence * pFence );
	VkResult VKAPI_CALL vkGetSwapchainCounterEXT(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkSurfaceCounterFlagBitsEXT counter,
		uint64_t * pCounterValue );

#endif
#pragma endregion
#pragma region VK_GOOGLE_display_timing
#ifdef VK_GOOGLE_display_timing

	VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkRefreshCycleDurationGOOGLE * pDisplayTimingProperties );
	VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint32_t * pPresentationTimingCount,
		VkPastPresentationTimingGOOGLE * pPresentationTimings );

#endif
#pragma endregion
#pragma region VK_EXT_discard_rectangles
#ifdef VK_EXT_discard_rectangles

	void VKAPI_CALL vkCmdSetDiscardRectangleEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstDiscardRectangle,
		uint32_t discardRectangleCount,
		const VkRect2D * pDiscardRectangles );

#endif
#pragma endregion
#pragma region VK_EXT_hdr_metadata
#ifdef VK_EXT_hdr_metadata

	void VKAPI_CALL vkSetHdrMetadataEXT(
		VkDevice device,
		uint32_t swapchainCount,
		const VkSwapchainKHR * pSwapchains,
		const VkHdrMetadataEXT * pMetadata );

#endif
#pragma endregion
#pragma region VK_EXT_debug_utils
#ifdef VK_EXT_debug_utils

	VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(
		VkDevice device,
		const VkDebugUtilsObjectNameInfoEXT * pNameInfo );
	VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(
		VkDevice device,
		const VkDebugUtilsObjectTagInfoEXT * pTagInfo );
	void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT * pLabelInfo );
	void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT(
		VkQueue queue );
	void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT * pLabelInfo );
	void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT * pLabelInfo );
	void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer );
	void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT * pLabelInfo );
	VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDebugUtilsMessengerEXT * pMessenger );
	void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(
		VkInstance instance,
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData );

#endif
#pragma endregion
#pragma region VK_EXT_sample_locations
#ifdef VK_EXT_sample_locations

	void VKAPI_CALL vkCmdSetSampleLocationsEXT(
		VkCommandBuffer commandBuffer,
		const VkSampleLocationsInfoEXT * pSampleLocationsInfo );
	void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(
		VkPhysicalDevice physicalDevice,
		VkSampleCountFlagBits samples,
		VkMultisamplePropertiesEXT * pMultisampleProperties );

#endif
#pragma endregion
#pragma region VK_EXT_image_drm_format_modifier
#ifdef VK_EXT_image_drm_format_modifier

	VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(
		VkDevice device,
		VkImage image,
		VkImageDrmFormatModifierPropertiesEXT * pProperties );

#endif
#pragma endregion
#pragma region VK_EXT_validation_cache
#ifdef VK_EXT_validation_cache

	VkResult VKAPI_CALL vkCreateValidationCacheEXT(
		VkDevice device,
		const VkValidationCacheCreateInfoEXT * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkValidationCacheEXT * pValidationCache );
	void VKAPI_CALL vkDestroyValidationCacheEXT(
		VkDevice device,
		VkValidationCacheEXT validationCache,
		const VkAllocationCallbacks * pAllocator );
	VkResult VKAPI_CALL vkMergeValidationCachesEXT(
		VkDevice device,
		VkValidationCacheEXT dstCache,
		uint32_t srcCacheCount,
		const VkValidationCacheEXT * pSrcCaches );
	VkResult VKAPI_CALL vkGetValidationCacheDataEXT(
		VkDevice device,
		VkValidationCacheEXT validationCache,
		size_t * pDataSize,
		void * pData );

#endif
#pragma endregion
#pragma region VK_NV_shading_rate_image
#ifdef VK_NV_shading_rate_image

	void VKAPI_CALL vkCmdBindShadingRateImageNV(
		VkCommandBuffer commandBuffer,
		VkImageView imageView,
		VkImageLayout imageLayout );
	void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkShadingRatePaletteNV * pShadingRatePalettes );
	void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(
		VkCommandBuffer commandBuffer,
		VkCoarseSampleOrderTypeNV sampleOrderType,
		uint32_t customSampleOrderCount,
		const VkCoarseSampleOrderCustomNV * pCustomSampleOrders );

#endif
#pragma endregion
#pragma region VK_NV_ray_tracing
#ifdef VK_NV_ray_tracing

	VkResult VKAPI_CALL vkCreateAccelerationStructureNV(
		VkDevice device,
		const VkAccelerationStructureCreateInfoNV * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkAccelerationStructureNV * pAccelerationStructure );
	void VKAPI_CALL vkDestroyAccelerationStructureNV(
		VkDevice device,
		VkAccelerationStructureNV accelerationStructure,
		const VkAllocationCallbacks * pAllocator );
	void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(
		VkDevice device,
		const VkAccelerationStructureMemoryRequirementsInfoNV * pInfo,
		VkMemoryRequirements2KHR * pMemoryRequirements );
	VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindAccelerationStructureMemoryInfoNV * pBindInfos );
	void VKAPI_CALL vkCmdBuildAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		const VkAccelerationStructureInfoNV * pInfo,
		VkBuffer instanceData,
		VkDeviceSize instanceOffset,
		VkBool32 update,
		VkAccelerationStructureNV dst,
		VkAccelerationStructureNV src,
		VkBuffer scratch,
		VkDeviceSize scratchOffset );
	void VKAPI_CALL vkCmdCopyAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		VkAccelerationStructureNV dst,
		VkAccelerationStructureNV src,
		VkCopyAccelerationStructureModeNV mode );
	void VKAPI_CALL vkCmdTraceRaysNV(
		VkCommandBuffer commandBuffer,
		VkBuffer raygenShaderBindingTableBuffer,
		VkDeviceSize raygenShaderBindingOffset,
		VkBuffer missShaderBindingTableBuffer,
		VkDeviceSize missShaderBindingOffset,
		VkDeviceSize missShaderBindingStride,
		VkBuffer hitShaderBindingTableBuffer,
		VkDeviceSize hitShaderBindingOffset,
		VkDeviceSize hitShaderBindingStride,
		VkBuffer callableShaderBindingTableBuffer,
		VkDeviceSize callableShaderBindingOffset,
		VkDeviceSize callableShaderBindingStride,
		uint32_t width,
		uint32_t height,
		uint32_t depth );
	VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkRayTracingPipelineCreateInfoNV * pCreateInfos,
		const VkAllocationCallbacks * pAllocator,
		VkPipeline * pPipelines );
	VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(
		VkDevice device,
		VkPipeline pipeline,
		uint32_t firstGroup,
		uint32_t groupCount,
		size_t dataSize,
		void * pData );
	VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(
		VkDevice device,
		VkAccelerationStructureNV accelerationStructure,
		size_t dataSize,
		void * pData );
	void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(
		VkCommandBuffer commandBuffer,
		uint32_t accelerationStructureCount,
		const VkAccelerationStructureNV * pAccelerationStructures,
		VkQueryType queryType,
		VkQueryPool queryPool,
		uint32_t firstQuery );
	VkResult VKAPI_CALL vkCompileDeferredNV(
		VkDevice device,
		VkPipeline pipeline,
		uint32_t shader );

#endif
#pragma endregion
#pragma region VK_EXT_external_memory_host
#ifdef VK_EXT_external_memory_host

	VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(
		VkDevice device,
		VkExternalMemoryHandleTypeFlagBits handleType,
		const void * pHostPointer,
		VkMemoryHostPointerPropertiesEXT * pMemoryHostPointerProperties );

#endif
#pragma endregion
#pragma region VK_AMD_buffer_marker
#ifdef VK_AMD_buffer_marker

	void VKAPI_CALL vkCmdWriteBufferMarkerAMD(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlagBits pipelineStage,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		uint32_t marker );

#endif
#pragma endregion
#pragma region VK_EXT_calibrated_timestamps
#ifdef VK_EXT_calibrated_timestamps

	VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(
		VkPhysicalDevice physicalDevice,
		uint32_t * pTimeDomainCount,
		VkTimeDomainEXT * pTimeDomains );
	VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(
		VkDevice device,
		uint32_t timestampCount,
		const VkCalibratedTimestampInfoEXT * pTimestampInfos,
		uint64_t * pTimestamps,
		uint64_t * pMaxDeviation );

#endif
#pragma endregion
#pragma region VK_NV_mesh_shader
#ifdef VK_NV_mesh_shader

	void VKAPI_CALL vkCmdDrawMeshTasksNV(
		VkCommandBuffer commandBuffer,
		uint32_t taskCount,
		uint32_t firstTask );
	void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride );
	void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride );

#endif
#pragma endregion
#pragma region VK_NV_scissor_exclusive
#ifdef VK_NV_scissor_exclusive

	void VKAPI_CALL vkCmdSetExclusiveScissorNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstExclusiveScissor,
		uint32_t exclusiveScissorCount,
		const VkRect2D * pExclusiveScissors );

#endif
#pragma endregion
#pragma region VK_NV_device_diagnostic_checkpoints
#ifdef VK_NV_device_diagnostic_checkpoints

	void VKAPI_CALL vkCmdSetCheckpointNV(
		VkCommandBuffer commandBuffer,
		const void * pCheckpointMarker );
	void VKAPI_CALL vkGetQueueCheckpointDataNV(
		VkQueue queue,
		uint32_t * pCheckpointDataCount,
		VkCheckpointDataNV * pCheckpointData );

#endif
#pragma endregion
#pragma region VK_EXT_buffer_device_address
#ifdef VK_EXT_buffer_device_address

	VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(
		VkDevice device,
		const VkBufferDeviceAddressInfoEXT * pInfo );

#endif
#pragma endregion
#pragma region VK_NV_cooperative_matrix
#ifdef VK_NV_cooperative_matrix

	VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkCooperativeMatrixPropertiesNV * pProperties );

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef ASHES_ANDROID

	VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR(
		VkInstance instance,
		const VkAndroidSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );

#	endif
#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_FUCHSIA_imagepipe_surface
#	ifdef VK_USE_PLATFORM_FUCHSIA

	VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA(
		VkInstance instance,
		const VkImagePipeSurfaceCreateInfoFUCHSIA * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_MVK_ios_surface
#	ifdef VK_USE_PLATFORM_IOS_MVK

	VkResult VKAPI_CALL vkCreateIOSSurfaceMVK(
		VkInstance instance,
		const VkIOSSurfaceCreateInfoMVK * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#ifdef VK_MVK_macos_surface
#	ifdef VK_USE_PLATFORM_MACOS_MVK

	VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK(
		VkInstance instance,
		const VkMacOSSurfaceCreateInfoMVK * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );

#	endif
#endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_NN_vi_surface
#	ifdef VK_USE_PLATFORM_VI_NN

	VkResult VKAPI_CALL vkCreateViSurfaceNN(
		VkInstance instance,
		const VkViSurfaceCreateInfoNN * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#ifdef VK_KHR_xcb_surface
#	ifdef __linux__

	VkResult VKAPI_CALL vkCreateXcbSurfaceKHR(
		VkInstance instance,
		const VkXcbSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );
	VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		xcb_connection_t * connection,
		xcb_visualid_t visual_id );

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#ifdef VK_KHR_xlib_surface
#	ifdef __linux__

	VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(
		VkInstance instance,
		const VkXlibSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );
	VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		Display * dpy,
		VisualID visualID );

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#ifdef VK_KHR_wayland_surface
#	ifdef __linux__

	VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR(
		VkInstance instance,
		const VkWaylandSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );
	VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		struct wl_display * display );

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_KHR_win32_surface
#	ifdef _WIN32

	VkResult VKAPI_CALL vkCreateWin32SurfaceKHR(
		VkInstance instance,
		const VkWin32SurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface );
	VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex );

#	endif
#endif
#pragma endregion
#pragma region VK_EXT_acquire_xlib_display
#ifdef VK_EXT_acquire_xlib_display
#	ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

	VkResult VKAPI_CALL vkAcquireXlibDisplayEXT(
		VkPhysicalDevice physicalDevice,
		Display * dpy,
		VkDisplayKHR display );
	VkResult VKAPI_CALL vkGetRandROutputDisplayEXT(
		VkPhysicalDevice physicalDevice,
		Display * dpy,
		RROutput rrOutput,
		VkDisplayKHR * pDisplay );

#	endif
#endif
#pragma endregion
}
