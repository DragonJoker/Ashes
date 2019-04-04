#include <ashes/ashes.h>

#include "Core/GlContext.hpp"

#include "ashesgl4_api.hpp"

#include <AshesRenderer/Util/Exception.hpp>

#include <iostream>

namespace ashes::gl4
{
#pragma region Vulkan 1.0

	PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance instance,
		const char* pName );

	PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
		VkDevice device,
		const char* pName );

	VkResult VKAPI_CALL vkCreateInstance(
		const VkInstanceCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkInstance* pInstance )
	{
		assert( pInstance );
		return allocate( *pInstance
			, pAllocator
			, *pCreateInfo );
	}

	PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
		VkDevice device,
		const char* pName );

	void VKAPI_CALL vkDestroyInstance(
		VkInstance instance,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( instance, pAllocator );
	}

	VkResult VKAPI_CALL vkEnumeratePhysicalDevices(
		VkInstance instance,
		uint32_t* pPhysicalDeviceCount,
		VkPhysicalDevice* pPhysicalDevices )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			if ( !pPhysicalDevices )
			{
				*pPhysicalDeviceCount = 1u;
				return result;
			}

			auto gpus = get( instance )->enumeratePhysicalDevices();

			for ( auto & gpu : gpus )
			{
				*pPhysicalDevices = gpu;
				++pPhysicalDevices;
			}
		}
		catch ( Exception & exc )
		{
			result = exc.getResult();
		}
		catch ( ... )
		{
			result = VK_ERROR_INITIALIZATION_FAILED;
		}

		return result;
	}

	void VKAPI_CALL vkGetPhysicalDeviceFeatures(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures* pFeatures )
	{
		*pFeatures = get( physicalDevice )->getFeatures();
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties* pFormatProperties )
	{
		*pFormatProperties = get( physicalDevice )->getFormatProperties( format );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkImageCreateFlags flags,
		VkImageFormatProperties* pImageFormatProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceImageFormatProperties Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties* pProperties )
	{
		*pProperties = get( physicalDevice )->getProperties();
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t* pQueueFamilyPropertyCount,
		VkQueueFamilyProperties* pQueueFamilyProperties )
	{
		if ( !pQueueFamilyProperties )
		{
			*pQueueFamilyPropertyCount = 1u;
			return;
		}

		auto props = get( physicalDevice )->getQueueFamilyProperties();

		for ( auto & prop : props )
		{
			( *pQueueFamilyProperties ) = prop;
			++pQueueFamilyProperties;
		}
	}

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties* pMemoryProperties )
	{
		*pMemoryProperties = get( physicalDevice )->getMemoryProperties();
	}

	VkResult VKAPI_CALL vkCreateDevice(
		VkPhysicalDevice physicalDevice,
		const VkDeviceCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDevice* pDevice )
	{
		auto & gpu = *get( physicalDevice );
		auto instance = gpu.getInstance();
		assert( pDevice );
		return allocate( *pDevice
			, pAllocator
			, instance
			, physicalDevice
			, get( instance )->getContext()
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDevice(
		VkDevice device,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( device, pAllocator );
	}

	VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(
		const char* pLayerName,
		uint32_t* pPropertyCount,
		VkExtensionProperties* pProperties )
	{
		static std::vector< VkExtensionProperties > const extensions
		{
			VkExtensionProperties{ VK_KHR_SURFACE_EXTENSION_NAME, makeVersion( 1, 0, 0 ) },
			[]()
			{
				VkExtensionProperties result;
				result.specVersion = makeVersion( 1, 0, 0 );
				strncpy( result.extensionName, VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE );
				return result;
			}(),
			VkExtensionProperties{ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, makeVersion( 1, 0, 0 ) },
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

	VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(
		VkPhysicalDevice physicalDevice,
		const char* pLayerName,
		uint32_t* pPropertyCount,
		VkExtensionProperties* pProperties )
	{
		// TODO
		*pPropertyCount = 0;
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(
		uint32_t* pPropertyCount,
		VkLayerProperties* pProperties )
	{
		// TODO
		*pPropertyCount = 0u;
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkLayerProperties* pProperties )
	{
		// TODO
		*pPropertyCount = 0u;
		return VK_SUCCESS;
	}

	void VKAPI_CALL vkGetDeviceQueue(
		VkDevice device,
		uint32_t queueFamilyIndex,
		uint32_t queueIndex,
		VkQueue* pQueue )
	{
		assert( pQueue );
		*pQueue = get( device )->getQueue( queueFamilyIndex, queueIndex );
	}

	VkResult VKAPI_CALL vkQueueSubmit(
		VkQueue queue,
		uint32_t submitCount,
		const VkSubmitInfo* pSubmits,
		VkFence fence )
	{
		return get( queue )->submit( { pSubmits, pSubmits + submitCount }, fence );
	}

	VkResult VKAPI_CALL vkQueueWaitIdle(
		VkQueue queue )
	{
		return get( queue )->waitIdle();
	}

	VkResult VKAPI_CALL vkDeviceWaitIdle(
		VkDevice device )
	{
		return get( device )->waitIdle();
	}

	VkResult VKAPI_CALL vkAllocateMemory(
		VkDevice device,
		const VkMemoryAllocateInfo* pAllocateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDeviceMemory* pMemory )
	{
		assert( pMemory );
		return allocate( *pMemory
			, pAllocator
			, device
			, *pAllocateInfo );
	}

	void VKAPI_CALL vkFreeMemory(
		VkDevice device,
		VkDeviceMemory memory,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( memory, pAllocator );
	}

	VkResult VKAPI_CALL vkMapMemory(
		VkDevice device,
		VkDeviceMemory memory,
		VkDeviceSize offset,
		VkDeviceSize size,
		VkMemoryMapFlags flags,
		void** ppData )
	{
		return get( memory )->lock( offset, size, flags, ppData );
	}

	void VKAPI_CALL vkUnmapMemory(
		VkDevice device,
		VkDeviceMemory memory )
	{
		get( memory )->unlock();
	}

	VkResult VKAPI_CALL vkFlushMappedMemoryRanges(
		VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange* pMemoryRanges )
	{
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
		{
			result = get( pMemoryRanges->memory )->flush( pMemoryRanges->offset, pMemoryRanges->size );
			++memoryRangeCount;
		}

		return result;
	}

	VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(
		VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange* pMemoryRanges )
	{
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
		{
			result = get( pMemoryRanges->memory )->invalidate( pMemoryRanges->offset, pMemoryRanges->size );
			++memoryRangeCount;
		}

		return result;
	}

	void VKAPI_CALL vkGetDeviceMemoryCommitment(
		VkDevice device,
		VkDeviceMemory memory,
		VkDeviceSize* pCommittedMemoryInBytes )
	{
		// TODO
		std::cerr << "vkGetDeviceMemoryCommitment Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkBindBufferMemory(
		VkDevice device,
		VkBuffer buffer,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset )
	{
		return get( memory )->bindToBuffer( buffer, memoryOffset );
	}

	VkResult VKAPI_CALL vkBindImageMemory(
		VkDevice device,
		VkImage image,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset )
	{
		return get( memory )->bindToImage( image, memoryOffset );
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements(
		VkDevice device,
		VkBuffer buffer,
		VkMemoryRequirements* pMemoryRequirements )
	{
		*pMemoryRequirements = get( buffer )->getMemoryRequirements();
	}

	void VKAPI_CALL vkGetImageMemoryRequirements(
		VkDevice device,
		VkImage image,
		VkMemoryRequirements* pMemoryRequirements )
	{
		*pMemoryRequirements = get( image )->getMemoryRequirements();
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements(
		VkDevice device,
		VkImage image,
		uint32_t* pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements* pSparseMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetImageSparseMemoryRequirements Unsupported" << std::endl;
		*pSparseMemoryRequirementCount = 0u;
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkSampleCountFlagBits samples,
		VkImageUsageFlags usage,
		VkImageTiling tiling,
		uint32_t* pPropertyCount,
		VkSparseImageFormatProperties* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceSparseImageFormatProperties Unsupported" << std::endl;
		*pPropertyCount = 0u;
	}

	VkResult VKAPI_CALL vkQueueBindSparse(
		VkQueue queue,
		uint32_t bindInfoCount,
		const VkBindSparseInfo* pBindInfo,
		VkFence fence )
	{
		// TODO
		std::cerr << "vkQueueBindSparse Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkCreateFence(
		VkDevice device,
		const VkFenceCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkFence* pFence )
	{
		assert( pFence );
		return allocate( *pFence
			, pAllocator
			, device
			, pCreateInfo->flags );
	}

	void VKAPI_CALL vkDestroyFence(
		VkDevice device,
		VkFence fence,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( fence, pAllocator );
	}

	VkResult VKAPI_CALL vkResetFences(
		VkDevice device,
		uint32_t fenceCount,
		const VkFence* pFences )
	{
		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			get( *pFences )->reset();
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetFenceStatus(
		VkDevice device,
		VkFence fence )
	{
		// TODO
		std::cerr << "vkGetFenceStatus Unsupported" << std::endl;
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkWaitForFences(
		VkDevice device,
		uint32_t fenceCount,
		const VkFence* pFences,
		VkBool32 waitAll,
		uint64_t timeout )
	{
		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			get( *pFences )->wait( timeout );
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkCreateSemaphore(
		VkDevice device,
		const VkSemaphoreCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSemaphore* pSemaphore )
	{
		assert( pSemaphore );
		return allocate( *pSemaphore
			, pAllocator
			, device );
	}

	void VKAPI_CALL vkDestroySemaphore(
		VkDevice device,
		VkSemaphore semaphore,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( semaphore, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateEvent(
		VkDevice device,
		const VkEventCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkEvent* pEvent )
	{
		assert( pEvent );
		return allocate( *pEvent
			, pAllocator
			, device );
	}

	void VKAPI_CALL vkDestroyEvent(
		VkDevice device,
		VkEvent event,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( event, pAllocator );
	}

	VkResult VKAPI_CALL vkGetEventStatus(
		VkDevice device,
		VkEvent event )
	{
		return get( event )->getStatus();
	}

	VkResult VKAPI_CALL vkSetEvent(
		VkDevice device,
		VkEvent event )
	{
		return get( event )->set();
	}

	VkResult VKAPI_CALL vkResetEvent(
		VkDevice device,
		VkEvent event )
	{
		return get( event )->reset();
	}

	VkResult VKAPI_CALL vkCreateQueryPool(
		VkDevice device,
		const VkQueryPoolCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkQueryPool* pQueryPool )
	{
		assert( pQueryPool );
		return allocate( *pQueryPool
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyQueryPool(
		VkDevice device,
		VkQueryPool queryPool,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( queryPool, pAllocator );
	}

	VkResult VKAPI_CALL vkGetQueryPoolResults(
		VkDevice device,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount,
		size_t dataSize,
		void* pData,
		VkDeviceSize stride,
		VkQueryResultFlags flags )
	{
		VkResult result;

		if ( flags & VK_QUERY_RESULT_64_BIT )
		{
			std::vector< uint64_t > results;
			results.reserve( queryCount );
			result = get( queryPool )->getResults( firstQuery
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
			result = get( queryPool )->getResults( firstQuery
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

		return result;
	}

	VkResult VKAPI_CALL vkCreateBuffer(
		VkDevice device,
		const VkBufferCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkBuffer* pBuffer )
	{
		assert( pBuffer );
		return allocate( *pBuffer
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyBuffer(
		VkDevice device,
		VkBuffer buffer,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( buffer, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateBufferView(
		VkDevice device,
		const VkBufferViewCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkBufferView* pView )
	{
		assert( pView );
		return allocate( *pView
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyBufferView(
		VkDevice device,
		VkBufferView bufferView,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( bufferView, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateImage(
		VkDevice device,
		const VkImageCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkImage* pImage )
	{
		assert( pImage );
		return allocate( *pImage
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyImage(
		VkDevice device,
		VkImage image,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( image, pAllocator );
	}

	void VKAPI_CALL vkGetImageSubresourceLayout(
		VkDevice device,
		VkImage image,
		const VkImageSubresource* pSubresource,
		VkSubresourceLayout* pLayout )
	{
		get( device )->getImageSubresourceLayout( image
			, *pSubresource
			, *pLayout );
	}

	VkResult VKAPI_CALL vkCreateImageView(
		VkDevice device,
		const VkImageViewCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkImageView* pView )
	{
		assert( pView );
		return allocate( *pView
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyImageView(
		VkDevice device,
		VkImageView imageView,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( imageView, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateShaderModule(
		VkDevice device,
		const VkShaderModuleCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkShaderModule* pShaderModule )
	{
		assert( pShaderModule );
		return allocate( *pShaderModule
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyShaderModule(
		VkDevice device,
		VkShaderModule shaderModule,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( shaderModule, pAllocator );
	}

	VkResult VKAPI_CALL vkCreatePipelineCache(
		VkDevice device,
		const VkPipelineCacheCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkPipelineCache* pPipelineCache )
	{
		// TODO
		std::cerr << "vkCreatePipelineCache Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyPipelineCache(
		VkDevice device,
		VkPipelineCache pipelineCache,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyPipelineCache Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkGetPipelineCacheData(
		VkDevice device,
		VkPipelineCache pipelineCache,
		size_t* pDataSize,
		void* pData )
	{
		// TODO
		std::cerr << "vkGetPipelineCacheData Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkMergePipelineCaches(
		VkDevice device,
		VkPipelineCache dstCache,
		uint32_t srcCacheCount,
		const VkPipelineCache* pSrcCaches )
	{
		// TODO
		std::cerr << "vkMergePipelineCaches Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkCreateGraphicsPipelines(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkGraphicsPipelineCreateInfo* pCreateInfos,
		const VkAllocationCallbacks* pAllocator,
		VkPipeline* pPipelines )
	{
		assert( pPipelines );
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		for ( uint32_t i = 0u; i < createInfoCount; ++i )
		{
			if ( result == VK_SUCCESS )
			{
				result = allocate( *pPipelines
					, pAllocator
					, device
					, *pCreateInfos );
			}

			++pPipelines;
			++pCreateInfos;
		}

		return result;
	}

	VkResult VKAPI_CALL vkCreateComputePipelines(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkComputePipelineCreateInfo* pCreateInfos,
		const VkAllocationCallbacks* pAllocator,
		VkPipeline* pPipelines )
	{
		assert( pPipelines );
		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		for ( uint32_t i = 0u; i < createInfoCount; ++i )
		{
			if ( result == VK_SUCCESS )
			{
				result = allocate( *pPipelines
					, pAllocator
					, device
					, *pCreateInfos );
			}

			++pPipelines;
			++pCreateInfos;
		}

		return result;
	}

	void VKAPI_CALL vkDestroyPipeline(
		VkDevice device,
		VkPipeline pipeline,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( pipeline, pAllocator );
	}

	VkResult VKAPI_CALL vkCreatePipelineLayout(
		VkDevice device,
		const VkPipelineLayoutCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkPipelineLayout* pPipelineLayout )
	{
		assert( pPipelineLayout );
		return allocate( *pPipelineLayout
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyPipelineLayout(
		VkDevice device,
		VkPipelineLayout pipelineLayout,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( pipelineLayout, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateSampler(
		VkDevice device,
		const VkSamplerCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSampler* pSampler )
	{
		assert( pSampler );
		return allocate( *pSampler
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroySampler(
		VkDevice device,
		VkSampler sampler,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( sampler, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateDescriptorSetLayout(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDescriptorSetLayout* pSetLayout )
	{
		assert( pSetLayout );
		return allocate( *pSetLayout
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDescriptorSetLayout(
		VkDevice device,
		VkDescriptorSetLayout descriptorSetLayout,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( descriptorSetLayout, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateDescriptorPool(
		VkDevice device,
		const VkDescriptorPoolCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDescriptorPool* pDescriptorPool )
	{
		assert( pDescriptorPool );
		return allocate( *pDescriptorPool
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDescriptorPool(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( descriptorPool, pAllocator );
	}

	VkResult VKAPI_CALL vkResetDescriptorPool(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		VkDescriptorPoolResetFlags flags )
	{
		return get( descriptorPool )->reset( flags );
	}

	VkResult VKAPI_CALL vkAllocateDescriptorSets(
		VkDevice device,
		const VkDescriptorSetAllocateInfo* pAllocateInfo,
		VkDescriptorSet* pDescriptorSets )
	{
		VkResult result = VK_SUCCESS;
		auto itLayout = pAllocateInfo->pSetLayouts;

		for ( auto itSet = pDescriptorSets;
			itSet != pDescriptorSets + pAllocateInfo->descriptorSetCount;
			++itLayout, ++itSet )
		{
			if ( result == VK_SUCCESS )
			{
				result = allocate( *itSet
					, nullptr );
			}
		}

		return result;
	}

	VkResult VKAPI_CALL vkFreeDescriptorSets(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		uint32_t descriptorSetCount,
		const VkDescriptorSet* pDescriptorSets )
	{
		return get( descriptorPool )->free( { pDescriptorSets, pDescriptorSets + descriptorSetCount } );
	}

	void VKAPI_CALL vkUpdateDescriptorSets(
		VkDevice device,
		uint32_t descriptorWriteCount,
		const VkWriteDescriptorSet* pDescriptorWrites,
		uint32_t descriptorCopyCount,
		const VkCopyDescriptorSet* pDescriptorCopies )
	{
		for ( auto it = pDescriptorWrites;
			it != pDescriptorWrites + descriptorWriteCount;
			++it )
		{
			get( it->dstSet )->update( *it );
			get( it->dstSet )->update();
		}

		for ( auto it = pDescriptorCopies;
			it != pDescriptorCopies + descriptorWriteCount;
			++it )
		{
			get( it->dstSet )->update( *it );
			get( it->dstSet )->update();
		}
	}

	VkResult VKAPI_CALL vkCreateFramebuffer(
		VkDevice device,
		const VkFramebufferCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkFramebuffer* pFramebuffer )
	{
		assert( pFramebuffer );
		return allocate( *pFramebuffer
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyFramebuffer(
		VkDevice device,
		VkFramebuffer framebuffer,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( framebuffer, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateRenderPass(
		VkDevice device,
		const VkRenderPassCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkRenderPass* pRenderPass )
	{
		assert( pRenderPass );
		return allocate( *pRenderPass
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyRenderPass(
		VkDevice device,
		VkRenderPass renderPass,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( renderPass, pAllocator );
	}

	void VKAPI_CALL vkGetRenderAreaGranularity(
		VkDevice device,
		VkRenderPass renderPass,
		VkExtent2D* pGranularity )
	{
		*pGranularity = get( renderPass )->getRenderAreaGranularity();
	}

	VkResult VKAPI_CALL vkCreateCommandPool(
		VkDevice device,
		const VkCommandPoolCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkCommandPool* pCommandPool )
	{
		assert( pCommandPool );
		return allocate( *pCommandPool
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( commandPool, pAllocator );
	}

	VkResult VKAPI_CALL vkResetCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolResetFlags flags )
	{
		return get( commandPool )->reset( flags );
	}

	VkResult VKAPI_CALL vkAllocateCommandBuffers(
		VkDevice device,
		const VkCommandBufferAllocateInfo* pAllocateInfo,
		VkCommandBuffer* pCommandBuffers )
	{
		VkResult result = VK_SUCCESS;
		auto itLayout = pAllocateInfo->commandBufferCount;

		for ( auto it = pCommandBuffers;
			it != pCommandBuffers + pAllocateInfo->commandBufferCount;
			++it )
		{
			if ( result == VK_SUCCESS )
			{
				result = allocate( *it
					, nullptr
					, device
					, pAllocateInfo->level );
			}
		}

		return result;
	}

	void VKAPI_CALL vkFreeCommandBuffers(
		VkDevice device,
		VkCommandPool commandPool,
		uint32_t commandBufferCount,
		const VkCommandBuffer* pCommandBuffers )
	{
		get( commandPool )->free( { pCommandBuffers, pCommandBuffers + commandBufferCount } );
	}

	VkResult VKAPI_CALL vkBeginCommandBuffer(
		VkCommandBuffer commandBuffer,
		const VkCommandBufferBeginInfo* pBeginInfo )
	{
		return get( commandBuffer )->begin( *pBeginInfo );
	}

	VkResult VKAPI_CALL vkEndCommandBuffer(
		VkCommandBuffer commandBuffer )
	{
		return get( commandBuffer )->end();
	}

	VkResult VKAPI_CALL vkResetCommandBuffer(
		VkCommandBuffer commandBuffer,
		VkCommandBufferResetFlags flags )
	{
		return get( commandBuffer )->reset( flags );
	}

	void VKAPI_CALL vkCmdBindPipeline(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipeline pipeline )
	{
		get( commandBuffer )->bindPipeline( pipeline, pipelineBindPoint );
	}

	void VKAPI_CALL vkCmdSetViewport(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkViewport* pViewports )
	{
		get( commandBuffer )->setViewport( firstViewport
			, { pViewports, pViewports + viewportCount } );
	}

	void VKAPI_CALL vkCmdSetScissor(
		VkCommandBuffer commandBuffer,
		uint32_t firstScissor,
		uint32_t scissorCount,
		const VkRect2D* pScissors )
	{
		get( commandBuffer )->setScissor( firstScissor
			, { pScissors, pScissors + scissorCount } );
	}

	void VKAPI_CALL vkCmdSetLineWidth(
		VkCommandBuffer commandBuffer,
		float lineWidth )
	{
		get( commandBuffer )->setLineWidth( lineWidth );
	}

	void VKAPI_CALL vkCmdSetDepthBias(
		VkCommandBuffer commandBuffer,
		float depthBiasConstantFactor,
		float depthBiasClamp,
		float depthBiasSlopeFactor )
	{
		get( commandBuffer )->setDepthBias( depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
	}

	void VKAPI_CALL vkCmdSetBlendConstants(
		VkCommandBuffer commandBuffer,
		const float blendConstants[4] )
	{
		// TODO
		std::cerr << "vkCmdSetBlendConstants Unsupported" << std::endl;
		//get( commandBuffer )->setBlendConstants( blendConstants );
	}

	void VKAPI_CALL vkCmdSetDepthBounds(
		VkCommandBuffer commandBuffer,
		float minDepthBounds,
		float maxDepthBounds )
	{
		// TODO
		std::cerr << "vkCmdSetDepthBounds Unsupported" << std::endl;
		//get( commandBuffer )->setDepthBounds( minDepthBounds, maxDepthBounds );
	}

	void VKAPI_CALL vkCmdSetStencilCompareMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t compareMask )
	{
		// TODO
		std::cerr << "vkCmdSetStencilCompareMask Unsupported" << std::endl;
		//get( commandBuffer )->setStencilCompareMask( faceMask, compareMask );
	}

	void VKAPI_CALL vkCmdSetStencilWriteMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t writeMask )
	{
		// TODO
		std::cerr << "vkCmdSetStencilWriteMask Unsupported" << std::endl;
		//get( commandBuffer )->setStencilWriteMask( faceMask, writeMask );
	}

	void VKAPI_CALL vkCmdSetStencilReference(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t reference )
	{
		// TODO
		std::cerr << "vkCmdSetStencilReference Unsupported" << std::endl;
		//get( commandBuffer )->setStencilRefeerence( faceMask, reference );
	}

	void VKAPI_CALL vkCmdBindDescriptorSets(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipelineLayout layout,
		uint32_t firstSet,
		uint32_t descriptorSetCount,
		const VkDescriptorSet* pDescriptorSets,
		uint32_t dynamicOffsetCount,
		const uint32_t* pDynamicOffsets )
	{
		get( commandBuffer )->bindDescriptorSets( pipelineBindPoint
			, layout
			, firstSet
			, { pDescriptorSets, pDescriptorSets + descriptorSetCount }
			, { pDynamicOffsets, pDynamicOffsets + dynamicOffsetCount } );
	}

	void VKAPI_CALL vkCmdBindIndexBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkIndexType indexType )
	{
		get( commandBuffer )->bindIndexBuffer( buffer, offset, indexType );
	}

	void VKAPI_CALL vkCmdBindVertexBuffers(
		VkCommandBuffer commandBuffer,
		uint32_t firstBinding,
		uint32_t bindingCount,
		const VkBuffer* pBuffers,
		const VkDeviceSize* pOffsets )
	{
		get( commandBuffer )->bindVertexBuffers( firstBinding
			, { pBuffers, pBuffers + bindingCount }
			, { pOffsets, pOffsets + bindingCount } );
	}

	void VKAPI_CALL vkCmdDraw(
		VkCommandBuffer commandBuffer,
		uint32_t vertexCount,
		uint32_t instanceCount,
		uint32_t firstVertex,
		uint32_t firstInstance )
	{
		get( commandBuffer )->draw( vertexCount
			, instanceCount
			, firstVertex
			, firstInstance );
	}

	void VKAPI_CALL vkCmdDrawIndexed(
		VkCommandBuffer commandBuffer,
		uint32_t indexCount,
		uint32_t instanceCount,
		uint32_t firstIndex,
		int32_t vertexOffset,
		uint32_t firstInstance )
	{
		get( commandBuffer )->drawIndexed( indexCount
			, instanceCount
			, firstIndex
			, vertexOffset
			, firstInstance );
	}

	void VKAPI_CALL vkCmdDrawIndirect(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride )
	{
		get( commandBuffer )->drawIndirect( buffer
			, offset
			, drawCount
			, stride );
	}

	void VKAPI_CALL vkCmdDrawIndexedIndirect(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride )
	{
		get( commandBuffer )->drawIndexedIndirect( buffer
			, offset
			, drawCount
			, stride );
	}

	void VKAPI_CALL vkCmdDispatch(
		VkCommandBuffer commandBuffer,
		uint32_t groupCountX,
		uint32_t groupCountY,
		uint32_t groupCountZ )
	{
		get( commandBuffer )->dispatch( groupCountX, groupCountY, groupCountZ );
	}

	void VKAPI_CALL vkCmdDispatchIndirect(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset )
	{
		get( commandBuffer )->dispatchIndirect( buffer, offset );
	}

	void VKAPI_CALL vkCmdCopyBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer srcBuffer,
		VkBuffer dstBuffer,
		uint32_t regionCount,
		const VkBufferCopy* pRegions )
	{
		get( commandBuffer )->copyBuffer( srcBuffer
			, dstBuffer
			, { pRegions, pRegions + regionCount } );
	}

	void VKAPI_CALL vkCmdCopyImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageCopy* pRegions )
	{
		get( commandBuffer )->copyImage( srcImage
			, srcImageLayout
			, dstImage
			, dstImageLayout
			, { pRegions, pRegions + regionCount } );
	}

	void VKAPI_CALL vkCmdBlitImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageBlit* pRegions,
		VkFilter filter )
	{
		get( commandBuffer )->blitImage( srcImage
			, srcImageLayout
			, dstImage
			, dstImageLayout
			, { pRegions, pRegions + regionCount }
			, filter );
	}

	void VKAPI_CALL vkCmdCopyBufferToImage(
		VkCommandBuffer commandBuffer,
		VkBuffer srcBuffer,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkBufferImageCopy* pRegions )
	{
		get( commandBuffer )->copyToImage( srcBuffer
			, dstImage
			, dstImageLayout
			, { pRegions, pRegions + regionCount } );
	}

	void VKAPI_CALL vkCmdCopyImageToBuffer(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkBuffer dstBuffer,
		uint32_t regionCount,
		const VkBufferImageCopy* pRegions )
	{
		get( commandBuffer )->copyToBuffer( srcImage
			, srcImageLayout
			, dstBuffer
			, { pRegions, pRegions + regionCount } );
	}

	void VKAPI_CALL vkCmdUpdateBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize dataSize,
		const void* pData )
	{
		// TODO
		std::cerr << "vkCmdUpdateBuffer Unsupported" << std::endl;
		//get( commandBuffer )->updateBuffer( dstBuffer
		//	, dstOffset
		//	, dataSize
		//	, pData );
	}

	void VKAPI_CALL vkCmdFillBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize size,
		uint32_t data )
	{
		// TODO
		std::cerr << "vkCmdFillBuffer Unsupported" << std::endl;
		//get( commandBuffer )->fillBuffer( dstBuffer
		//	, dstOffset
		//	, size
		//	, data );
	}

	void VKAPI_CALL vkCmdClearColorImage(
		VkCommandBuffer commandBuffer,
		VkImage image,
		VkImageLayout imageLayout,
		const VkClearColorValue* pColor,
		uint32_t rangeCount,
		const VkImageSubresourceRange* pRanges )
	{
		get( commandBuffer )->clearColorImage( image
			, imageLayout
			, *pColor
			, { pRanges, pRanges + rangeCount } );
	}

	void VKAPI_CALL vkCmdClearDepthStencilImage(
		VkCommandBuffer commandBuffer,
		VkImage image,
		VkImageLayout imageLayout,
		const VkClearDepthStencilValue* pDepthStencil,
		uint32_t rangeCount,
		const VkImageSubresourceRange* pRanges )
	{
		get( commandBuffer )->clearDepthStencilImage( image
			, imageLayout
			, *pDepthStencil
			, { pRanges, pRanges + rangeCount } );
	}

	void VKAPI_CALL vkCmdClearAttachments(
		VkCommandBuffer commandBuffer,
		uint32_t attachmentCount,
		const VkClearAttachment* pAttachments,
		uint32_t rectCount,
		const VkClearRect* pRects )
	{
		get( commandBuffer )->clearAttachments( { pAttachments, pAttachments + attachmentCount }
			, { pRects, pRects + rectCount } );
	}

	void VKAPI_CALL vkCmdResolveImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		VkImageLayout srcImageLayout,
		VkImage dstImage,
		VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageResolve* pRegions )
	{
		// TODO
		std::cerr << "vkCmdResolveImage Unsupported" << std::endl;
		//get( commandBuffer )->resolveImage( srcImage
		//	, srcImageLayout
		//	, dstImage
		//	, dstImageLayout
		//	, { pRegions, pRegions + regionCount } );
	}

	void VKAPI_CALL vkCmdSetEvent(
		VkCommandBuffer commandBuffer,
		VkEvent event,
		VkPipelineStageFlags stageMask )
	{
		get( commandBuffer )->setEvent( event, stageMask );
	}

	void VKAPI_CALL vkCmdResetEvent(
		VkCommandBuffer commandBuffer,
		VkEvent event,
		VkPipelineStageFlags stageMask )
	{
		get( commandBuffer )->resetEvent( event, stageMask );
	}

	void VKAPI_CALL vkCmdWaitEvents(
		VkCommandBuffer commandBuffer,
		uint32_t eventCount,
		const VkEvent* pEvents,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		uint32_t memoryBarrierCount,
		const VkMemoryBarrier* pMemoryBarriers,
		uint32_t bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier* pBufferMemoryBarriers,
		uint32_t imageMemoryBarrierCount,
		const VkImageMemoryBarrier* pImageMemoryBarriers )
	{
		get( commandBuffer )->waitEvents( { pEvents, pEvents + eventCount }
			, srcStageMask
			, dstStageMask
			, { pMemoryBarriers, pMemoryBarriers + memoryBarrierCount }
			, { pBufferMemoryBarriers, pBufferMemoryBarriers + bufferMemoryBarrierCount }
			, { pImageMemoryBarriers, pImageMemoryBarriers + imageMemoryBarrierCount } );
	}

	void VKAPI_CALL vkCmdPipelineBarrier(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkDependencyFlags dependencyFlags,
		uint32_t memoryBarrierCount,
		const VkMemoryBarrier* pMemoryBarriers,
		uint32_t bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier* pBufferMemoryBarriers,
		uint32_t imageMemoryBarrierCount,
		const VkImageMemoryBarrier* pImageMemoryBarriers )
	{
		get( commandBuffer )->pipelineBarrier( srcStageMask
			, dstStageMask
			, dependencyFlags
			, { pMemoryBarriers, pMemoryBarriers + memoryBarrierCount }
			, { pBufferMemoryBarriers, pBufferMemoryBarriers + bufferMemoryBarrierCount }
			, { pImageMemoryBarriers, pImageMemoryBarriers + imageMemoryBarrierCount } );
	}

	void VKAPI_CALL vkCmdBeginQuery(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		VkQueryControlFlags flags )
	{
		get( commandBuffer )->beginQuery( queryPool
			, query
			, flags );
	}

	void VKAPI_CALL vkCmdEndQuery(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query )
	{
		get( commandBuffer )->endQuery( queryPool
			, query );
	}

	void VKAPI_CALL vkCmdResetQueryPool(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount )
	{
		get( commandBuffer )->resetQueryPool( queryPool
			, firstQuery
			, queryCount );
	}

	void VKAPI_CALL vkCmdWriteTimestamp(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlagBits pipelineStage,
		VkQueryPool queryPool,
		uint32_t query )
	{
		get( commandBuffer )->writeTimestamp( pipelineStage
			, queryPool
			, query );
	}

	void VKAPI_CALL vkCmdCopyQueryPoolResults(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize stride,
		VkQueryResultFlags flags )
	{
		// TODO
		std::cerr << "vkCmdCopyQueryPoolResults Unsupported" << std::endl;
		//get( commandBuffer )->copyQueryPoolResults( queryPool
		//	, firstQuery
		//	, queryCount
		//	, dstBuffer
		//	, dstOffset
		//	, stride
		//	, flags );
	}

	void VKAPI_CALL vkCmdPushConstants(
		VkCommandBuffer commandBuffer,
		VkPipelineLayout layout,
		VkShaderStageFlags stageFlags,
		uint32_t offset,
		uint32_t size,
		const void* pValues )
	{
		get( commandBuffer )->pushConstants( layout
			, stageFlags
			, offset
			, size
			, pValues );
	}

	void VKAPI_CALL vkCmdBeginRenderPass(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo* pRenderPassBegin,
		VkSubpassContents contents )
	{
		get( commandBuffer )->beginRenderPass( *pRenderPassBegin, contents );
	}

	void VKAPI_CALL vkCmdNextSubpass(
		VkCommandBuffer commandBuffer,
		VkSubpassContents contents )
	{
		get( commandBuffer )->nextSubpass( contents );
	}

	void VKAPI_CALL vkCmdEndRenderPass(
		VkCommandBuffer commandBuffer )
	{
		get( commandBuffer )->endRenderPass();
	}

	void VKAPI_CALL vkCmdExecuteCommands(
		VkCommandBuffer commandBuffer,
		uint32_t commandBufferCount,
		const VkCommandBuffer* pCommandBuffers )
	{
		get( commandBuffer )->executeCommands( { pCommandBuffers, pCommandBuffers + commandBufferCount } );
	}

#pragma endregion
#pragma region Vulkan 1.1

	VkResult VKAPI_PTR vkEnumerateInstanceVersion( uint32_t * version )
	{
		assert( version );
		*version = makeVersion( 1, 0, 0 );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkBindBufferMemory2(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo* pBindInfos )
	{
		// TODO
		std::cerr << "vkBindBufferMemory2 Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkBindImageMemory2(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo* pBindInfos )
	{
		// TODO
		std::cerr << "vkBindImageMemory2 Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(
		VkDevice device,
		uint32_t heapIndex,
		uint32_t localDeviceIndex,
		uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		// TODO
		std::cerr << "vkGetDeviceGroupPeerMemoryFeatures Unsupported" << std::endl;
		*pPeerMemoryFeatures = VK_NULL_HANDLE;
	}

	void VKAPI_CALL vkCmdSetDeviceMask(
		VkCommandBuffer commandBuffer,
		uint32_t deviceMask )
	{
		// TODO
		std::cerr << "vkCmdSetDeviceMask Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDispatchBase(
		VkCommandBuffer commandBuffer,
		uint32_t baseGroupX,
		uint32_t baseGroupY,
		uint32_t baseGroupZ,
		uint32_t groupCountX,
		uint32_t groupCountY,
		uint32_t groupCountZ )
	{
		// TODO
		std::cerr << "vkCmdDispatchBase Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(
		VkInstance instance,
		uint32_t* pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties )
	{
		// TODO
		std::cerr << "vkEnumeratePhysicalDeviceGroups Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkGetImageMemoryRequirements2(
		VkDevice device,
		const VkImageMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetImageMemoryRequirements2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements2(
		VkDevice device,
		const VkBufferMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetBufferMemoryRequirements2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements2(
		VkDevice device,
		const VkImageSparseMemoryRequirementsInfo2* pInfo,
		uint32_t* pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetImageSparseMemoryRequirements2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2* pFeatures )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceFeatures2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceProperties2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2* pFormatProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceFormatProperties2 Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
		VkImageFormatProperties2* pImageFormatProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceImageFormatProperties2 Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(
		VkPhysicalDevice physicalDevice,
		uint32_t* pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2* pQueueFamilyProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceQueueFamilyProperties2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2* pMemoryProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceMemoryProperties2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
		uint32_t* pPropertyCount,
		VkSparseImageFormatProperties2* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceSparseImageFormatProperties2 Unsupported" << std::endl;
	}

	void VKAPI_CALL vkTrimCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolTrimFlags flags )
	{
		// TODO
		std::cerr << "vkTrimCommandPool Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetDeviceQueue2(
		VkDevice device,
		const VkDeviceQueueInfo2* pQueueInfo,
		VkQueue* pQueue )
	{
		// TODO
		std::cerr << "vkGetDeviceQueue2 Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(
		VkDevice device,
		const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		// TODO
		std::cerr << "vkCreateSamplerYcbcrConversion Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroySamplerYcbcrConversion(
		VkDevice device,
		VkSamplerYcbcrConversion ycbcrConversion,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroySamplerYcbcrConversion Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(
		VkDevice device,
		const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		// TODO
		std::cerr << "vkCreateDescriptorUpdateTemplate Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(
		VkDevice device,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyDescriptorUpdateTemplate Unsupported" << std::endl;
	}

	void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(
		VkDevice device,
		VkDescriptorSet descriptorSet,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const void* pData )
	{
		// TODO
		std::cerr << "vkUpdateDescriptorSetWithTemplate Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
		VkExternalBufferProperties* pExternalBufferProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalBufferProperties Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
		VkExternalFenceProperties* pExternalFenceProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalFenceProperties Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
		VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalSemaphoreProperties Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetDescriptorSetLayoutSupport(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
		VkDescriptorSetLayoutSupport* pSupport )
	{
		// TODO
		std::cerr << "vkGetDescriptorSetLayoutSupport Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_surface

	void VKAPI_CALL vkDestroySurfaceKHR(
		VkInstance instance,
		VkSurfaceKHR surface,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( surface, pAllocator );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		VkSurfaceKHR surface,
		VkBool32* pSupported )
	{
		*pSupported = get( physicalDevice )->getPresentationSupport( queueFamilyIndex );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkSurfaceCapabilitiesKHR* pSurfaceCapabilities )
	{
		*pSurfaceCapabilities = get( surface )->getCapabilities();
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t* pSurfaceFormatCount,
		VkSurfaceFormatKHR* pSurfaceFormats )
	{
		if ( !pSurfaceFormats )
		{
			*pSurfaceFormatCount = uint32_t( get( surface )->getFormats().size() );
			return VK_SUCCESS;
		}

		auto formats = get( surface )->getFormats();

		for ( auto & format : formats )
		{
			*pSurfaceFormats = format;
			++pSurfaceFormats;
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t* pPresentModeCount,
		VkPresentModeKHR* pPresentModes )
	{
		if ( !pPresentModes )
		{
			*pPresentModeCount = uint32_t( get( surface )->getPresentModes().size() );
			return VK_SUCCESS;
		}

		auto modes = get( surface )->getPresentModes();

		for ( auto & mode : modes )
		{
			*pPresentModes = mode;
			++pPresentModes;
		}

		return VK_SUCCESS;
	}

#pragma endregion
#pragma region VK_KHR_swapchain

	VkResult VKAPI_CALL vkCreateSwapchainKHR(
		VkDevice device,
		const VkSwapchainCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSwapchainKHR* pSwapchain )
	{
		assert( pSwapchain );
		return allocate( *pSwapchain
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroySwapchainKHR(
		VkDevice device,
		VkSwapchainKHR swapchain,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( swapchain, pAllocator );
	}

	VkResult VKAPI_CALL vkGetSwapchainImagesKHR(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint32_t* pSwapchainImageCount,
		VkImage* pSwapchainImages )
	{
		if ( !pSwapchainImages )
		{
			*pSwapchainImageCount = get( swapchain )->getImageCount();
			return VK_SUCCESS;
		}

		auto result = get( swapchain )->getImages();

		for ( auto & image : result )
		{
			*pSwapchainImages = image;
			++pSwapchainImages;
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkAcquireNextImageKHR(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint64_t timeout,
		VkSemaphore semaphore,
		VkFence fence,
		uint32_t* pImageIndex )
	{
		return get( swapchain )->acquireNextImage( timeout, semaphore, fence, *pImageIndex );
	}

	VkResult VKAPI_CALL vkQueuePresentKHR(
		VkQueue queue,
		const VkPresentInfoKHR* pPresentInfo )
	{
		return get( queue )->present( *pPresentInfo );
	}

	VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(
		VkDevice device,
		VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities )
	{
		// TODO
		std::cerr << "vkGetDeviceGroupPresentCapabilitiesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(
		VkDevice device,
		VkSurfaceKHR surface,
		VkDeviceGroupPresentModeFlagsKHR* pModes )
	{
		// TODO
		std::cerr << "vkGetDeviceGroupSurfacePresentModesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t* pRectCount,
		VkRect2D* pRects )
	{
		// TODO
		std::cerr << "vkGetPhysicalDevicePresentRectanglesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkAcquireNextImage2KHR(
		VkDevice device,
		const VkAcquireNextImageInfoKHR* pAcquireInfo,
		uint32_t* pImageIndex )
	{
		// TODO
		std::cerr << "vkAcquireNextImage2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_display

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkDisplayPropertiesKHR* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceDisplayPropertiesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkDisplayPlanePropertiesKHR* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceDisplayPlanePropertiesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t planeIndex,
		uint32_t* pDisplayCount,
		VkDisplayKHR* pDisplays )
	{
		// TODO
		std::cerr << "vkGetDisplayPlaneSupportedDisplaysKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t* pPropertyCount,
		VkDisplayModePropertiesKHR* pProperties )
	{
		// TODO
		std::cerr << "vkGetDisplayModePropertiesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkCreateDisplayModeKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		const VkDisplayModeCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDisplayModeKHR* pMode )
	{
		// TODO
		std::cerr << "vkCreateDisplayModeKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayModeKHR mode,
		uint32_t planeIndex,
		VkDisplayPlaneCapabilitiesKHR* pCapabilities )
	{
		// TODO
		std::cerr << "vkGetDisplayPlaneCapabilitiesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		const VkDisplaySurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		// TODO
		std::cerr << "vkCreateDisplayPlaneSurfaceKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_get_physical_device_properties2

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2* pFeatures )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceFeatures2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceProperties2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2* pFormatProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceFormatProperties2KHR Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo,
		VkImageFormatProperties2* pImageFormatProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceImageFormatProperties2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2* pQueueFamilyProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceQueueFamilyProperties2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2* pMemoryProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceMemoryProperties2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
		uint32_t* pPropertyCount,
		VkSparseImageFormatProperties2* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceSparseImageFormatProperties2KHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_device_group

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(
		VkDevice device,
		uint32_t heapIndex,
		uint32_t localDeviceIndex,
		uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		// TODO
		std::cerr << "vkGetDeviceGroupPeerMemoryFeaturesKHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdSetDeviceMaskKHR(
		VkCommandBuffer commandBuffer,
		uint32_t deviceMask )
	{
		// TODO
		std::cerr << "vkCmdSetDeviceMaskKHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDispatchBaseKHR(
		VkCommandBuffer commandBuffer,
		uint32_t baseGroupX,
		uint32_t baseGroupY,
		uint32_t baseGroupZ,
		uint32_t groupCountX,
		uint32_t groupCountY,
		uint32_t groupCountZ )
	{
		// TODO
		std::cerr << "vkCmdDispatchBaseKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_maintenance1

	void VKAPI_CALL vkTrimCommandPoolKHR(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolTrimFlags flags )
	{
		// TODO
		std::cerr << "vkTrimCommandPoolKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_device_group_creation

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(
		VkInstance instance,
		uint32_t* pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties )
	{
		// TODO
		std::cerr << "vkEnumeratePhysicalDeviceGroupsKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_external_memory_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
		VkExternalBufferProperties* pExternalBufferProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalBufferPropertiesKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_external_memory_fd

	VkResult VKAPI_CALL vkGetMemoryFdKHR(
		VkDevice device,
		const VkMemoryGetFdInfoKHR* pGetFdInfo,
		int* pFd )
	{
		// TODO
		std::cerr << "vkGetMemoryFdKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(
		VkDevice device,
		VkExternalMemoryHandleTypeFlagBits handleType,
		int fd,
		VkMemoryFdPropertiesKHR* pMemoryFdProperties )
	{
		// TODO
		std::cerr << "vkGetMemoryFdPropertiesKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_external_semaphore_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
		VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_external_semaphore_fd

	VkResult VKAPI_CALL vkImportSemaphoreFdKHR(
		VkDevice device,
		const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo )
	{
		// TODO
		std::cerr << "vkImportSemaphoreFdKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetSemaphoreFdKHR(
		VkDevice device,
		const VkSemaphoreGetFdInfoKHR* pGetFdInfo,
		int* pFd )
	{
		// TODO
		std::cerr << "vkGetSemaphoreFdKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_push_descriptor

	void VKAPI_CALL vkCmdPushDescriptorSetKHR(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipelineLayout layout,
		uint32_t set,
		uint32_t descriptorWriteCount,
		const VkWriteDescriptorSet* pDescriptorWrites )
	{
		// TODO
		std::cerr << "vkCmdPushDescriptorSetKHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(
		VkCommandBuffer commandBuffer,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		VkPipelineLayout layout,
		uint32_t set,
		const void* pData )
	{
		// TODO
		std::cerr << "vkCmdPushDescriptorSetWithTemplateKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_descriptor_update_template

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(
		VkDevice device,
		const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		// TODO
		std::cerr << "vkCreateDescriptorUpdateTemplateKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(
		VkDevice device,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyDescriptorUpdateTemplateKHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(
		VkDevice device,
		VkDescriptorSet descriptorSet,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const void* pData )
	{
		// TODO
		std::cerr << "vkUpdateDescriptorSetWithTemplateKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_create_renderpass2

	VkResult VKAPI_CALL vkCreateRenderPass2KHR(
		VkDevice device,
		const VkRenderPassCreateInfo2KHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkRenderPass* pRenderPass )
	{
		// TODO
		std::cerr << "vkCreateRenderPass2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkCmdBeginRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo* pRenderPassBegin,
		const VkSubpassBeginInfoKHR* pSubpassBeginInfo )
	{
		// TODO
		std::cerr << "vkCmdBeginRenderPass2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdNextSubpass2KHR(
		VkCommandBuffer commandBuffer,
		const VkSubpassBeginInfoKHR* pSubpassBeginInfo,
		const VkSubpassEndInfoKHR* pSubpassEndInfo )
	{
		// TODO
		std::cerr << "vkCmdNextSubpass2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdEndRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		const VkSubpassEndInfoKHR* pSubpassEndInfo )
	{
		// TODO
		std::cerr << "vkCmdEndRenderPass2KHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_shared_presentable_image

	VkResult VKAPI_CALL vkGetSwapchainStatusKHR(
		VkDevice device,
		VkSwapchainKHR swapchain )
	{
		// TODO
		std::cerr << "vkGetSwapchainStatusKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_external_fence_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
		VkExternalFenceProperties* pExternalFenceProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalFencePropertiesKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_external_fence_fd

	VkResult VKAPI_CALL vkImportFenceFdKHR(
		VkDevice device,
		const VkImportFenceFdInfoKHR* pImportFenceFdInfo )
	{
		// TODO
		std::cerr << "vkImportFenceFdKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetFenceFdKHR(
		VkDevice device,
		const VkFenceGetFdInfoKHR* pGetFdInfo,
		int* pFd )
	{
		// TODO
		std::cerr << "vkGetFenceFdKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_get_surface_capabilities2

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
		VkSurfaceCapabilities2KHR* pSurfaceCapabilities )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceSurfaceCapabilities2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
		uint32_t* pSurfaceFormatCount,
		VkSurfaceFormat2KHR* pSurfaceFormats )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceSurfaceFormats2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_get_display_properties2

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkDisplayProperties2KHR* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceDisplayProperties2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkDisplayPlaneProperties2KHR* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceDisplayPlaneProperties2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t* pPropertyCount,
		VkDisplayModeProperties2KHR* pProperties )
	{
		// TODO
		std::cerr << "vkGetDisplayModeProperties2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo,
		VkDisplayPlaneCapabilities2KHR* pCapabilities )
	{
		// TODO
		std::cerr << "vkGetDisplayPlaneCapabilities2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_get_memory_requirements2

	void VKAPI_CALL vkGetImageMemoryRequirements2KHR(
		VkDevice device,
		const VkImageMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetImageMemoryRequirements2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(
		VkDevice device,
		const VkBufferMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetBufferMemoryRequirements2KHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(
		VkDevice device,
		const VkImageSparseMemoryRequirementsInfo2* pInfo,
		uint32_t* pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetImageSparseMemoryRequirements2KHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_sampler_ycbcr_conversion

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(
		VkDevice device,
		const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		// TODO
		std::cerr << "vkCreateSamplerYcbcrConversionKHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(
		VkDevice device,
		VkSamplerYcbcrConversion ycbcrConversion,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroySamplerYcbcrConversionKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_bind_memory2

	VkResult VKAPI_CALL vkBindBufferMemory2KHR(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo* pBindInfos )
	{
		// TODO
		std::cerr << "vkBindBufferMemory2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkBindImageMemory2KHR(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo* pBindInfos )
	{
		// TODO
		std::cerr << "vkBindImageMemory2KHR Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_maintenance3

	void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
		VkDescriptorSetLayoutSupport* pSupport )
	{
		// TODO
		std::cerr << "vkGetDescriptorSetLayoutSupportKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_KHR_draw_indirect_count

	void VKAPI_CALL vkCmdDrawIndirectCountKHR(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		// TODO
		std::cerr << "vkCmdDrawIndirectCountKHR Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		// TODO
		std::cerr << "vkCmdDrawIndexedIndirectCountKHR Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_debug_report

	VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback )
	{
		assert( pCallback );
		return allocate( *pCallback
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
		VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( callback, pAllocator );
	}

	void VKAPI_CALL vkDebugReportMessageEXT(
		VkInstance instance,
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objectType,
		uint64_t object,
		size_t location,
		int32_t messageCode,
		const char* pLayerPrefix,
		const char* pMessage )
	{
		get( instance )->reportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

#pragma endregion
#pragma region VK_EXT_debug_marker

	VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT(
		VkDevice device,
		const VkDebugMarkerObjectTagInfoEXT* pTagInfo )
	{
		// TODO
		std::cerr << "vkDebugMarkerSetObjectTagEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(
		VkDevice device,
		const VkDebugMarkerObjectNameInfoEXT* pNameInfo )
	{
		// TODO
		std::cerr << "vkDebugMarkerSetObjectNameEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkCmdDebugMarkerBeginEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT* pMarkerInfo )
	{
		// TODO
		std::cerr << "vkCmdDebugMarkerBeginEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDebugMarkerEndEXT(
		VkCommandBuffer commandBuffer )
	{
		// TODO
		std::cerr << "vkCmdDebugMarkerEndEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDebugMarkerInsertEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT* pMarkerInfo )
	{
		// TODO
		std::cerr << "vkCmdDebugMarkerInsertEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_transform_feedback

	void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstBinding,
		uint32_t bindingCount,
		const VkBuffer* pBuffers,
		const VkDeviceSize* pOffsets,
		const VkDeviceSize* pSizes )
	{
		// TODO
		std::cerr << "vkCmdBindTransformFeedbackBuffersEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstCounterBuffer,
		uint32_t counterBufferCount,
		const VkBuffer* pCounterBuffers,
		const VkDeviceSize* pCounterBufferOffsets )
	{
		// TODO
		std::cerr << "vkCmdBeginTransformFeedbackEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdEndTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstCounterBuffer,
		uint32_t counterBufferCount,
		const VkBuffer* pCounterBuffers,
		const VkDeviceSize* pCounterBufferOffsets )
	{
		// TODO
		std::cerr << "vkCmdEndTransformFeedbackEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdBeginQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		VkQueryControlFlags flags,
		uint32_t index )
	{
		// TODO
		std::cerr << "vkCmdBeginQueryIndexedEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdEndQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		uint32_t index )
	{
		// TODO
		std::cerr << "vkCmdEndQueryIndexedEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDrawIndirectByteCountEXT(
		VkCommandBuffer commandBuffer,
		uint32_t instanceCount,
		uint32_t firstInstance,
		VkBuffer counterBuffer,
		VkDeviceSize counterBufferOffset,
		uint32_t counterOffset,
		uint32_t vertexStride )
	{
		// TODO
		std::cerr << "vkCmdDrawIndirectByteCountEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_AMD_draw_indirect_count

	void VKAPI_CALL vkCmdDrawIndirectCountAMD(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		// TODO
		std::cerr << "vkCmdDrawIndirectCountAMD Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		// TODO
		std::cerr << "vkCmdDrawIndexedIndirectCountAMD Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_AMD_shader_info

	VkResult VKAPI_CALL vkGetShaderInfoAMD(
		VkDevice device,
		VkPipeline pipeline,
		VkShaderStageFlagBits shaderStage,
		VkShaderInfoTypeAMD infoType,
		size_t* pInfoSize,
		void* pInfo )
	{
		// TODO
		std::cerr << "vkGetShaderInfoAMD Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_NV_external_memory_capabilities

	VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkImageType type,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkImageCreateFlags flags,
		VkExternalMemoryHandleTypeFlagsNV externalHandleType,
		VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceExternalImageFormatPropertiesNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_EXT_conditional_rendering

	void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(
		VkCommandBuffer commandBuffer,
		const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin )
	{
		// TODO
		std::cerr << "vkCmdBeginConditionalRenderingEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdEndConditionalRenderingEXT(
		VkCommandBuffer commandBuffer )
	{
		// TODO
		std::cerr << "vkCmdEndConditionalRenderingEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_NVX_device_generated_commands

	void VKAPI_CALL vkCmdProcessCommandsNVX(
		VkCommandBuffer commandBuffer,
		const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo )
	{
		// TODO
		std::cerr << "vkCmdProcessCommandsNVX Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdReserveSpaceForCommandsNVX(
		VkCommandBuffer commandBuffer,
		const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo )
	{
		// TODO
		std::cerr << "vkCmdReserveSpaceForCommandsNVX Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNVX(
		VkDevice device,
		const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout )
	{
		// TODO
		std::cerr << "vkCreateIndirectCommandsLayoutNVX Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyIndirectCommandsLayoutNVX(
		VkDevice device,
		VkIndirectCommandsLayoutNVX indirectCommandsLayout,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyIndirectCommandsLayoutNVX Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCreateObjectTableNVX(
		VkDevice device,
		const VkObjectTableCreateInfoNVX* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkObjectTableNVX* pObjectTable )
	{
		// TODO
		std::cerr << "vkCreateObjectTableNVX Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyObjectTableNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyObjectTableNVX Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkRegisterObjectsNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		uint32_t objectCount,
		const VkObjectTableEntryNVX* const* ppObjectTableEntries,
		const uint32_t* pObjectIndices )
	{
		// TODO
		std::cerr << "vkRegisterObjectsNVX Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkUnregisterObjectsNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		uint32_t objectCount,
		const VkObjectEntryTypeNVX* pObjectEntryTypes,
		const uint32_t* pObjectIndices )
	{
		// TODO
		std::cerr << "vkUnregisterObjectsNVX Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(
		VkPhysicalDevice physicalDevice,
		VkDeviceGeneratedCommandsFeaturesNVX* pFeatures,
		VkDeviceGeneratedCommandsLimitsNVX* pLimits )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_NV_clip_space_w_scaling

	void VKAPI_CALL vkCmdSetViewportWScalingNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkViewportWScalingNV* pViewportWScalings )
	{
		// TODO
		std::cerr << "vkCmdSetViewportWScalingNV Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_direct_mode_display

	VkResult VKAPI_CALL vkReleaseDisplayEXT(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display )
	{
		// TODO
		std::cerr << "vkReleaseDisplayEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_EXT_display_surface_counter

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkSurfaceCapabilities2EXT* pSurfaceCapabilities )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceSurfaceCapabilities2EXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_EXT_display_control

	VkResult VKAPI_CALL vkDisplayPowerControlEXT(
		VkDevice device,
		VkDisplayKHR display,
		const VkDisplayPowerInfoEXT* pDisplayPowerInfo )
	{
		// TODO
		std::cerr << "vkDisplayPowerControlEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkRegisterDeviceEventEXT(
		VkDevice device,
		const VkDeviceEventInfoEXT* pDeviceEventInfo,
		const VkAllocationCallbacks* pAllocator,
		VkFence* pFence )
	{
		// TODO
		std::cerr << "vkRegisterDeviceEventEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkRegisterDisplayEventEXT(
		VkDevice device,
		VkDisplayKHR display,
		const VkDisplayEventInfoEXT* pDisplayEventInfo,
		const VkAllocationCallbacks* pAllocator,
		VkFence* pFence )
	{
		// TODO
		std::cerr << "vkRegisterDisplayEventEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetSwapchainCounterEXT(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkSurfaceCounterFlagBitsEXT counter,
		uint64_t* pCounterValue )
	{
		// TODO
		std::cerr << "vkGetSwapchainCounterEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_GOOGLE_display_timing

	VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties )
	{
		// TODO
		std::cerr << "vkGetRefreshCycleDurationGOOGLE Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint32_t* pPresentationTimingCount,
		VkPastPresentationTimingGOOGLE* pPresentationTimings )
	{
		// TODO
		std::cerr << "vkGetPastPresentationTimingGOOGLE Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_EXT_discard_rectangles

	void VKAPI_CALL vkCmdSetDiscardRectangleEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstDiscardRectangle,
		uint32_t discardRectangleCount,
		const VkRect2D* pDiscardRectangles )
	{
		// TODO
		std::cerr << "vkCmdSetDiscardRectangleEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_hdr_metadata

	void VKAPI_CALL vkSetHdrMetadataEXT(
		VkDevice device,
		uint32_t swapchainCount,
		const VkSwapchainKHR* pSwapchains,
		const VkHdrMetadataEXT* pMetadata )
	{
		// TODO
		std::cerr << "vkSetHdrMetadataEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_debug_utils

	VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(
		VkDevice device,
		const VkDebugUtilsObjectNameInfoEXT* pNameInfo )
	{
		// TODO
		std::cerr << "vkSetDebugUtilsObjectNameEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(
		VkDevice device,
		const VkDebugUtilsObjectTagInfoEXT* pTagInfo )
	{
		// TODO
		std::cerr << "vkSetDebugUtilsObjectTagEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		// TODO
		std::cerr << "vkQueueBeginDebugUtilsLabelEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT(
		VkQueue queue )
	{
		// TODO
		std::cerr << "vkQueueEndDebugUtilsLabelEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		// TODO
		std::cerr << "vkQueueInsertDebugUtilsLabelEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		// TODO
		std::cerr << "vkCmdBeginDebugUtilsLabelEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer )
	{
		// TODO
		std::cerr << "vkCmdEndDebugUtilsLabelEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		// TODO
		std::cerr << "vkCmdInsertDebugUtilsLabelEXT Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pMessenger )
	{
		// TODO
		std::cerr << "vkCreateDebugUtilsMessengerEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyDebugUtilsMessengerEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(
		VkInstance instance,
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData )
	{
		// TODO
		std::cerr << "vkSubmitDebugUtilsMessageEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_sample_locations

	void VKAPI_CALL vkCmdSetSampleLocationsEXT(
		VkCommandBuffer commandBuffer,
		const VkSampleLocationsInfoEXT* pSampleLocationsInfo )
	{
		// TODO
		std::cerr << "vkCmdSetSampleLocationsEXT Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(
		VkPhysicalDevice physicalDevice,
		VkSampleCountFlagBits samples,
		VkMultisamplePropertiesEXT* pMultisampleProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceMultisamplePropertiesEXT Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_image_drm_format_modifier

	VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(
		VkDevice device,
		VkImage image,
		VkImageDrmFormatModifierPropertiesEXT* pProperties )
	{
		// TODO
		std::cerr << "vkGetImageDrmFormatModifierPropertiesEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_EXT_validation_cache

	VkResult VKAPI_CALL vkCreateValidationCacheEXT(
		VkDevice device,
		const VkValidationCacheCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkValidationCacheEXT* pValidationCache )
	{
		// TODO
		std::cerr << "vkCreateValidationCacheEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyValidationCacheEXT(
		VkDevice device,
		VkValidationCacheEXT validationCache,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyValidationCacheEXT Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkMergeValidationCachesEXT(
		VkDevice device,
		VkValidationCacheEXT dstCache,
		uint32_t srcCacheCount,
		const VkValidationCacheEXT* pSrcCaches )
	{
		// TODO
		std::cerr << "vkMergeValidationCachesEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetValidationCacheDataEXT(
		VkDevice device,
		VkValidationCacheEXT validationCache,
		size_t* pDataSize,
		void* pData )
	{
		// TODO
		std::cerr << "vkGetValidationCacheDataEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_NV_shading_rate_image

	void VKAPI_CALL vkCmdBindShadingRateImageNV(
		VkCommandBuffer commandBuffer,
		VkImageView imageView,
		VkImageLayout imageLayout )
	{
		// TODO
		std::cerr << "vkCmdBindShadingRateImageNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkShadingRatePaletteNV* pShadingRatePalettes )
	{
		// TODO
		std::cerr << "vkCmdSetViewportShadingRatePaletteNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(
		VkCommandBuffer commandBuffer,
		VkCoarseSampleOrderTypeNV sampleOrderType,
		uint32_t customSampleOrderCount,
		const VkCoarseSampleOrderCustomNV* pCustomSampleOrders )
	{
		// TODO
		std::cerr << "vkCmdSetCoarseSampleOrderNV Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_NV_ray_tracing

	VkResult VKAPI_CALL vkCreateAccelerationStructureNV(
		VkDevice device,
		const VkAccelerationStructureCreateInfoNV* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkAccelerationStructureNV* pAccelerationStructure )
	{
		// TODO
		std::cerr << "vkCreateAccelerationStructureNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkDestroyAccelerationStructureNV(
		VkDevice device,
		VkAccelerationStructureNV accelerationStructure,
		const VkAllocationCallbacks* pAllocator )
	{
		// TODO
		std::cerr << "vkDestroyAccelerationStructureNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(
		VkDevice device,
		const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo,
		VkMemoryRequirements2KHR* pMemoryRequirements )
	{
		// TODO
		std::cerr << "vkGetAccelerationStructureMemoryRequirementsNV Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindAccelerationStructureMemoryInfoNV* pBindInfos )
	{
		// TODO
		std::cerr << "vkBindAccelerationStructureMemoryNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkCmdBuildAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		const VkAccelerationStructureInfoNV* pInfo,
		VkBuffer instanceData,
		VkDeviceSize instanceOffset,
		VkBool32 update,
		VkAccelerationStructureNV dst,
		VkAccelerationStructureNV src,
		VkBuffer scratch,
		VkDeviceSize scratchOffset )
	{
		// TODO
		std::cerr << "vkCmdBuildAccelerationStructureNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdCopyAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		VkAccelerationStructureNV dst,
		VkAccelerationStructureNV src,
		VkCopyAccelerationStructureModeNV mode )
	{
		// TODO
		std::cerr << "vkCmdCopyAccelerationStructureNV Unsupported" << std::endl;
	}

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
		uint32_t depth )
	{
		// TODO
		std::cerr << "vkCmdTraceRaysNV Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkRayTracingPipelineCreateInfoNV* pCreateInfos,
		const VkAllocationCallbacks* pAllocator,
		VkPipeline* pPipelines )
	{
		// TODO
		std::cerr << "vkCreateRayTracingPipelinesNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(
		VkDevice device,
		VkPipeline pipeline,
		uint32_t firstGroup,
		uint32_t groupCount,
		size_t dataSize,
		void* pData )
	{
		// TODO
		std::cerr << "vkGetRayTracingShaderGroupHandlesNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(
		VkDevice device,
		VkAccelerationStructureNV accelerationStructure,
		size_t dataSize,
		void* pData )
	{
		// TODO
		std::cerr << "vkGetAccelerationStructureHandleNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(
		VkCommandBuffer commandBuffer,
		uint32_t accelerationStructureCount,
		const VkAccelerationStructureNV* pAccelerationStructures,
		VkQueryType queryType,
		VkQueryPool queryPool,
		uint32_t firstQuery )
	{
		// TODO
		std::cerr << "vkCmdWriteAccelerationStructuresPropertiesNV Unsupported" << std::endl;
	}

	VkResult VKAPI_CALL vkCompileDeferredNV(
		VkDevice device,
		VkPipeline pipeline,
		uint32_t shader )
	{
		// TODO
		std::cerr << "vkCompileDeferredNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_EXT_external_memory_host

	VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(
		VkDevice device,
		VkExternalMemoryHandleTypeFlagBits handleType,
		const void* pHostPointer,
		VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties )
	{
		// TODO
		std::cerr << "vkGetMemoryHostPointerPropertiesEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_AMD_buffer_marker

	void VKAPI_CALL vkCmdWriteBufferMarkerAMD(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlagBits pipelineStage,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		uint32_t marker )
	{
		// TODO
		std::cerr << "vkCmdWriteBufferMarkerAMD Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_calibrated_timestamps

	VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(
		VkPhysicalDevice physicalDevice,
		uint32_t* pTimeDomainCount,
		VkTimeDomainEXT* pTimeDomains )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(
		VkDevice device,
		uint32_t timestampCount,
		const VkCalibratedTimestampInfoEXT* pTimestampInfos,
		uint64_t* pTimestamps,
		uint64_t* pMaxDeviation )
	{
		// TODO
		std::cerr << "vkGetCalibratedTimestampsEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_NV_mesh_shader

	void VKAPI_CALL vkCmdDrawMeshTasksNV(
		VkCommandBuffer commandBuffer,
		uint32_t taskCount,
		uint32_t firstTask )
	{
		// TODO
		std::cerr << "vkCmdDrawMeshTasksNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride )
	{
		// TODO
		std::cerr << "vkCmdDrawMeshTasksIndirectNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		// TODO
		std::cerr << "vkCmdDrawMeshTasksIndirectCountNV Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_NV_scissor_exclusive

	void VKAPI_CALL vkCmdSetExclusiveScissorNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstExclusiveScissor,
		uint32_t exclusiveScissorCount,
		const VkRect2D* pExclusiveScissors )
	{
		// TODO
		std::cerr << "vkCmdSetExclusiveScissorNV Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_NV_device_diagnostic_checkpoints

	void VKAPI_CALL vkCmdSetCheckpointNV(
		VkCommandBuffer commandBuffer,
		const void* pCheckpointMarker )
	{
		// TODO
		std::cerr << "vkCmdSetCheckpointNV Unsupported" << std::endl;
	}

	void VKAPI_CALL vkGetQueueCheckpointDataNV(
		VkQueue queue,
		uint32_t* pCheckpointDataCount,
		VkCheckpointDataNV* pCheckpointData )
	{
		// TODO
		std::cerr << "vkGetQueueCheckpointDataNV Unsupported" << std::endl;
	}

#pragma endregion
#pragma region VK_EXT_buffer_device_address

	VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(
		VkDevice device,
		const VkBufferDeviceAddressInfoEXT* pInfo )
	{
		// TODO
		std::cerr << "vkGetBufferDeviceAddressEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_NV_cooperative_matrix

	VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkCooperativeMatrixPropertiesNV* pProperties )
	{
		// TODO
		std::cerr << "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_USE_PLATFORM_ANDROID_KHR

	VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR(
		VkInstance instance,
		const VkAndroidSurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_USE_PLATFORM_FUCHSIA

	VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA(
		VkInstance instance,
		const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_USE_PLATFORM_IOS_MVK

	VkResult VKAPI_CALL vkCreateIOSSurfaceMVK(
		VkInstance instance,
		const VkIOSSurfaceCreateInfoMVK* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#ifdef VK_USE_PLATFORM_MACOS_MVK

	VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK(
		VkInstance instance,
		const VkMacOSSurfaceCreateInfoMVK* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

#endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_USE_PLATFORM_VI_NN

	VkResult VKAPI_CALL vkCreateViSurfaceNN(
		VkInstance instance,
		const VkViSurfaceCreateInfoNN* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#ifdef VK_USE_PLATFORM_XCB_KHR

	VkResult VKAPI_CALL vkCreateXcbSurfaceKHR(
		VkInstance instance,
		const VkXcbSurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		xcb_connection_t* connection,
		xcb_visualid_t visual_id)
	{
		return get( physicalDevice )->getPresentationSupport( queueFamilyIndex );
	}

#endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#ifdef VK_USE_PLATFORM_XLIB_KHR

	VkResult VKAPI_CALL vkCreateXlibSurfaceKHR(
		VkInstance instance,
		const VkXlibSurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		Display* dpy,
		VisualID visualID )
	{
		return get( physicalDevice )->getPresentationSupport( queueFamilyIndex );
	}

#endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

	VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR(
		VkInstance instance,
		const VkWaylandSurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface)
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex,
		struct wl_display* display )
	{
		return get( physicalDevice )->getPresentationSupport( queueFamilyIndex );
	}

#endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_USE_PLATFORM_WIN32_KHR

	VkResult VKAPI_CALL vkCreateWin32SurfaceKHR(
		VkInstance instance,
		const VkWin32SurfaceCreateInfoKHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSurfaceKHR* pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t queueFamilyIndex )
	{
		return get( physicalDevice )->getPresentationSupport( queueFamilyIndex );
	}

#pragma endregion
#pragma region VK_EXT_acquire_xlib_display
#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

	VkResult VKAPI_CALL vkAcquireXlibDisplayEXT(
		VkPhysicalDevice physicalDevice,
		Display* dpy,
		VkDisplayKHR display )
	{
		// TODO
		std::cerr << "vkAcquireXlibDisplayEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult VKAPI_CALL vkGetRandROutputDisplayEXT(
		VkPhysicalDevice physicalDevice,
		Display* dpy,
		RROutput rrOutput,
		VkDisplayKHR* pDisplay )
	{
		// TODO
		std::cerr << "vkGetRandROutputDisplayEXT Unsupported" << std::endl;
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

#endif
#pragma endregion

#endif
}

namespace ashes::gl4
{
	struct GlLibrary
	{
		AshPluginDescription description
		{
			"gl4",
			"OpenGL 4.2 renderer for Ashes.",
		};

		VkResult init()
		{
			VkResult result = description.getInstanceProcAddr
				? VK_SUCCESS
				: VK_ERROR_INITIALIZATION_FAILED;

			if ( result != VK_SUCCESS )
			{
				description.getInstanceProcAddr = &vkGetInstanceProcAddr;
				description.features =
				{
					true, // hasTexBufferRange
					true, // hasImageTexture
					true, // hasBaseInstance
					true, // hasClearTexImage
					true, // hasComputeShaders
					true, // hasStorageBuffers
					true, // supportsPersistentMapping
				};
				result = VK_SUCCESS;
			}

			return result;
		}
	};

#define VK_LIB_GLOBAL_FUNCTION( x )\
	static std::string const x = "vk"#x;

#define VK_LIB_INSTANCE_FUNCTION( x )\
	static std::string const x = "vk"#x;

#define VK_LIB_DEVICE_FUNCTION( x )\
	static std::string const x = "vk"#x;

#include <AshesRenderer/Util/VulkanFunctionsList.inl>
}

thread_local ashes::gl4::GlLibrary g_library;

namespace ashes::gl4
{
	PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance instance,
		const char* pName )
	{
		PFN_vkVoidFunction result{ nullptr };
		auto init = g_library.init();

		if ( init == VK_SUCCESS )
		{
			static std::map< std::string, PFN_vkVoidFunction > functions
			{
#define VK_LIB_GLOBAL_FUNCTION( x )\
				{ "vk"#x, PFN_vkVoidFunction( vk##x ) },
#define VK_LIB_INSTANCE_FUNCTION( x )\
				{ "vk"#x, PFN_vkVoidFunction( vk##x ) },
#include <AshesRenderer/Util/VulkanFunctionsList.inl>
			};

			auto it = functions.find( pName );

			if ( it != functions.end() )
			{
				result = it->second;
			}
		}

		return result;
	}

	PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
		VkDevice device,
		const char* pName )
	{
		PFN_vkVoidFunction result{ nullptr };
		auto init = g_library.init();

		if ( init == VK_SUCCESS )
		{
			static std::map< std::string, PFN_vkVoidFunction > functions
			{
#define VK_LIB_DEVICE_FUNCTION( x )\
				{ "vk"#x,PFN_vkVoidFunction( vk##x ) },
#include <AshesRenderer/Util/VulkanFunctionsList.inl>
			};

			auto it = functions.find( pName );

			if ( it != functions.end() )
			{
				result = it->second;
			}
		}

		return result;
	}

}

#ifdef __cplusplus
extern "C"
{
#endif

	Gl4Renderer_API VkResult VKAPI_PTR ashGetPluginDescription( AshPluginDescription * pDescription )
	{
		auto result = g_library.init();

		if ( result == VK_SUCCESS )
		{
			*pDescription = g_library.description;
		}

		return result;
	}

#ifdef __cplusplus
}
#endif
