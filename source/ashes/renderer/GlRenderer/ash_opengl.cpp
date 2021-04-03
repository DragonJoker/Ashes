#include "Core/GlContext.hpp"

#include "ashesgl_api.hpp"

#include <renderer/GlRenderer/Miscellaneous/GlWindow.hpp>
#include <renderer/GlRenderer/Miscellaneous/GlExtensionsHandler.hpp>

#include <ashes/common/Exception.hpp>

#include <cstring>
#include <iostream>

namespace ashes::gl
{
#pragma region Vulkan 1.0
#ifdef VK_VERSION_1_0

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
			auto gpus = get( instance )->enumeratePhysicalDevices();

			if ( pPhysicalDevices
				&& pPhysicalDeviceCount )
			{
				auto requested = *pPhysicalDeviceCount;
				uint32_t index = 0u;

				for ( auto & gpu : gpus )
				{
					if ( index < requested )
					{
						*pPhysicalDevices = gpu;
						++pPhysicalDevices;
					}

					++index;
				}

				*pPhysicalDeviceCount = std::min( requested, uint32_t( gpus.size() ) );

				if ( requested >= uint32_t( gpus.size() ) )
				{
					return VK_SUCCESS;
				}

				return VK_INCOMPLETE;
			}

			if ( !pPhysicalDeviceCount )
			{
				return VK_ERROR_OUT_OF_HOST_MEMORY;
			}

			*pPhysicalDeviceCount = uint32_t( gpus.size() );
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
		return get( physicalDevice )->getImageFormatProperties( format
			, type
			, tiling
			, usage
			, flags
			, *pImageFormatProperties );
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
		auto props = get( physicalDevice )->getQueueFamilyProperties();
		*pQueueFamilyPropertyCount = uint32_t( props.size() );

		if ( pQueueFamilyProperties )
		{
			for ( auto & prop : props )
			{
				( *pQueueFamilyProperties ) = prop;
				++pQueueFamilyProperties;
			}
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
		auto & props = getSupportedInstanceExtensions();

		if ( pProperties
			&& pPropertyCount )
		{
			auto requested = *pPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					*pProperties = prop;
					++pProperties;
				}

				++index;
			}

			*pPropertyCount = std::min( requested, uint32_t( props.size() ) );

			if ( requested >= uint32_t( props.size() ) )
			{
				return VK_SUCCESS;
			}

			return VK_INCOMPLETE;
		}

		if ( !pPropertyCount )
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		*pPropertyCount = uint32_t( props.size() );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(
		VkPhysicalDevice physicalDevice,
		const char* pLayerName,
		uint32_t* pPropertyCount,
		VkExtensionProperties* pProperties )
	{
		auto props = get( physicalDevice )->enumerateExtensionProperties( pLayerName );

		if ( pProperties
			&& pPropertyCount )
		{
			auto requested = *pPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					*pProperties = prop;
					++pProperties;
				}

				++index;
			}

			*pPropertyCount = std::min( requested, uint32_t( props.size() ) );

			if ( requested >= uint32_t( props.size() ) )
			{
				return VK_SUCCESS;
			}

			return VK_INCOMPLETE;
		}

		if ( !pPropertyCount )
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		*pPropertyCount = uint32_t( props.size() );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(
		uint32_t* pPropertyCount,
		VkLayerProperties* pProperties )
	{
		auto & props = getInstanceLayerProperties();

		if ( pProperties
			&& pPropertyCount )
		{
			auto requested = *pPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					*pProperties = prop;
					++pProperties;
				}

				++index;
			}

			*pPropertyCount = std::min( requested, uint32_t( props.size() ) );

			if ( requested >= uint32_t( props.size() ) )
			{
				return VK_SUCCESS;
			}

			return VK_INCOMPLETE;
		}

		if ( !pPropertyCount )
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		*pPropertyCount = uint32_t( props.size() );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkLayerProperties* pProperties )
	{
		auto props = get( physicalDevice )->enumerateLayerProperties();

		if ( pProperties
			&& pPropertyCount )
		{
			auto requested = *pPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					*pProperties = prop;
					++pProperties;
				}

				++index;
			}

			*pPropertyCount = std::min( requested, uint32_t( props.size() ) );

			if ( requested >= uint32_t( props.size() ) )
			{
				return VK_SUCCESS;
			}

			return VK_INCOMPLETE;
		}

		if ( !pPropertyCount )
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		*pPropertyCount = uint32_t( props.size() );
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
		auto context = get( device )->getContext();
		return get( memory )->lock( context, offset, size, flags, ppData );
	}

	void VKAPI_CALL vkUnmapMemory(
		VkDevice device,
		VkDeviceMemory memory )
	{
		auto context = get( device )->getContext();
		get( memory )->unlock( context );
	}

	VkResult VKAPI_CALL vkFlushMappedMemoryRanges(
		VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange* pMemoryRanges )
	{
		auto context = get( device )->getContext();
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
		{
			result = get( pMemoryRanges->memory )->flush( context, pMemoryRanges->offset, pMemoryRanges->size );
			++pMemoryRanges;
		}

		return result;
	}

	VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(
		VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange* pMemoryRanges )
	{
		auto context = get( device )->getContext();
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
		{
			result = get( pMemoryRanges->memory )->invalidate( context, pMemoryRanges->offset, pMemoryRanges->size );
			++pMemoryRanges;
		}

		return result;
	}

	void VKAPI_CALL vkGetDeviceMemoryCommitment(
		VkDevice device,
		VkDeviceMemory memory,
		VkDeviceSize* pCommittedMemoryInBytes )
	{
		*pCommittedMemoryInBytes = 0;
	}

	VkResult VKAPI_CALL vkBindBufferMemory(
		VkDevice device,
		VkBuffer buffer,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset )
	{
		return get( memory )->bindBuffer( buffer, memoryOffset );
	}

	VkResult VKAPI_CALL vkBindImageMemory(
		VkDevice device,
		VkImage image,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset )
	{
		return get( memory )->bindImage( image, memoryOffset );
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
		auto props = get( image )->getSparseImageMemoryRequirements();
		*pSparseMemoryRequirementCount = uint32_t( props.size() );

		if ( pSparseMemoryRequirements )
		{
			for ( auto & prop : props )
			{
				*pSparseMemoryRequirements = prop;
				++pSparseMemoryRequirements;
			}
		}
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
		std::vector< VkSparseImageFormatProperties > props;
		get( physicalDevice )->getSparseImageFormatProperties( format
			, type
			, samples
			, usage
			, tiling
			, props );
		*pPropertyCount = uint32_t( props.size() );

		if ( pProperties )
		{
			for ( auto & prop : props )
			{
				*pProperties = prop;
				++pProperties;
			}
		}
	}

	VkResult VKAPI_CALL vkQueueBindSparse(
		VkQueue queue,
		uint32_t bindInfoCount,
		const VkBindSparseInfo* pBindInfo,
		VkFence fence )
	{
		return get( queue )->bindSparse( makeArrayView( pBindInfo, bindInfoCount ), fence );
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
		auto context = get( device )->getContext();

		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			get( *pFences )->reset( context );
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetFenceStatus(
		VkDevice device,
		VkFence fence )
	{
		auto context = get( device )->getContext();
		return get( fence )->getStatus( context );
	}

	VkResult VKAPI_CALL vkWaitForFences(
		VkDevice device,
		uint32_t fenceCount,
		const VkFence* pFences,
		VkBool32 waitAll,
		uint64_t timeout )
	{
		auto context = get( device )->getContext();

		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			get( *pFences )->wait( context, timeout );
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
		auto context = get( device )->getContext();
		return get( queryPool )->getResults( context
			, firstQuery
			, queryCount
			, stride
			, flags
			, dataSize
			, pData );
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

		if ( getInternalFormat( pCreateInfo->format ) == GL_INTERNAL_UNSUPPORTED )
		{
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}

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

		if ( getInternalFormat( pCreateInfo->format ) == GL_INTERNAL_UNSUPPORTED )
		{
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}

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

		if ( getInternalFormat( pCreateInfo->format ) == GL_INTERNAL_UNSUPPORTED )
		{
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}

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
		assert( pPipelineCache );
		return allocate( *pPipelineCache
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyPipelineCache(
		VkDevice device,
		VkPipelineCache pipelineCache,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( pipelineCache, pAllocator );
	}

	VkResult VKAPI_CALL vkGetPipelineCacheData(
		VkDevice device,
		VkPipelineCache pipelineCache,
		size_t* pDataSize,
		void* pData )
	{
		auto & data = get( pipelineCache )->getData();
		*pDataSize = data.size();

		if ( pData )
		{
			std::memcpy( pData, data.data(), data.size() );
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkMergePipelineCaches(
		VkDevice device,
		VkPipelineCache dstCache,
		uint32_t srcCacheCount,
		const VkPipelineCache* pSrcCaches )
	{
		return get( dstCache )->merge( makeVector( pSrcCaches, srcCacheCount ) );
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
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < createInfoCount; ++i )
		{
			auto tmp = allocate( *pPipelines
				, pAllocator
				, device
				, *pCreateInfos );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
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
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < createInfoCount; ++i )
		{
			auto tmp = allocate( *pPipelines
				, pAllocator
				, device
				, *pCreateInfos );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
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
			result = std::max( result
				, get( pAllocateInfo->descriptorPool )->allocate( *itLayout, *itSet ) );
		}

		return result;
	}

	VkResult VKAPI_CALL vkFreeDescriptorSets(
		VkDevice device,
		VkDescriptorPool descriptorPool,
		uint32_t descriptorSetCount,
		const VkDescriptorSet* pDescriptorSets )
	{
		return get( descriptorPool )->free( ashes::makeArrayView( pDescriptorSets
			, pDescriptorSets + descriptorSetCount ) );
	}

	void VKAPI_CALL vkUpdateDescriptorSets(
		VkDevice device,
		uint32_t descriptorWriteCount,
		const VkWriteDescriptorSet* pDescriptorWrites,
		uint32_t descriptorCopyCount,
		const VkCopyDescriptorSet* pDescriptorCopies )
	{
		for ( auto & write : makeArrayView( pDescriptorWrites, descriptorWriteCount ) )
		{
			get( write.dstSet )->update( write );
		}

		for ( auto & copy : makeArrayView( pDescriptorCopies, descriptorCopyCount ) )
		{
			get( copy.dstSet )->update( copy );
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
			auto tmp = allocateNA( *it
				, device
				, pAllocateInfo->level );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
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
		get( commandBuffer )->setDepthBias( depthBiasConstantFactor
			, depthBiasClamp
			, depthBiasSlopeFactor );
	}

	void VKAPI_CALL vkCmdSetBlendConstants(
		VkCommandBuffer commandBuffer,
		const float blendConstants[4] )
	{
		get( commandBuffer )->setBlendConstants( blendConstants );
	}

	void VKAPI_CALL vkCmdSetDepthBounds(
		VkCommandBuffer commandBuffer,
		float minDepthBounds,
		float maxDepthBounds )
	{
		get( commandBuffer )->setDepthBounds( minDepthBounds, maxDepthBounds );
	}

	void VKAPI_CALL vkCmdSetStencilCompareMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t compareMask )
	{
		get( commandBuffer )->setStencilCompareMask( faceMask, compareMask );
	}

	void VKAPI_CALL vkCmdSetStencilWriteMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t writeMask )
	{
		get( commandBuffer )->setStencilWriteMask( faceMask, writeMask );
	}

	void VKAPI_CALL vkCmdSetStencilReference(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t reference )
	{
		get( commandBuffer )->setStencilReference( faceMask, reference );
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
			, makeArrayView( pDescriptorSets, pDescriptorSets + descriptorSetCount )
			, makeArrayView( pDynamicOffsets, pDynamicOffsets + dynamicOffsetCount ) );
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
			, makeArrayView( pBuffers, bindingCount )
			, makeArrayView( pOffsets, bindingCount ) );
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
		get( commandBuffer )->updateBuffer( dstBuffer
			, dstOffset
			, makeArrayView( reinterpret_cast< uint8_t const * >( pData ), dataSize ) );
	}

	void VKAPI_CALL vkCmdFillBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize size,
		uint32_t data )
	{
		get( commandBuffer )->fillBuffer( dstBuffer
			, dstOffset
			, size
			, data );
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
			, makeArrayView( pRanges, pRanges + rangeCount ) );
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
			, makeArrayView( pRanges, pRanges + rangeCount ) );
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
		get( commandBuffer )->resolveImage( srcImage
			, srcImageLayout
			, dstImage
			, dstImageLayout
			, { pRegions, pRegions + regionCount } );
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
		get( commandBuffer )->copyQueryPoolResults( queryPool
			, firstQuery
			, queryCount
			, dstBuffer
			, dstOffset
			, stride
			, flags );
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

#endif
#pragma endregion
#pragma region Vulkan 1.1
#ifdef VK_VERSION_1_1

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
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			auto tmp = get( bindInfo.memory )->bindBuffer( bindInfo.buffer, bindInfo.memoryOffset );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
		}

		return result;
	}

	VkResult VKAPI_CALL vkBindImageMemory2(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo* pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			auto tmp = get( bindInfo.memory )->bindImage( bindInfo.image, bindInfo.memoryOffset );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
		}

		return result;
	}

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(
		VkDevice device,
		uint32_t heapIndex,
		uint32_t localDeviceIndex,
		uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		reportUnsupported( device, "vkGetDeviceGroupPeerMemoryFeatures" );
		*pPeerMemoryFeatures = VK_NULL_HANDLE;
	}

	void VKAPI_CALL vkCmdSetDeviceMask(
		VkCommandBuffer commandBuffer,
		uint32_t deviceMask )
	{
		reportUnsupported( commandBuffer, "vkCmdSetDeviceMask" );
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
		reportUnsupported( commandBuffer, "vkCmdDispatchBase" );
	}

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(
		VkInstance instance,
		uint32_t* pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties )
	{
		return reportUnsupported( instance, "vkEnumeratePhysicalDeviceGroups" );
	}

	void VKAPI_CALL vkGetImageMemoryRequirements2(
		VkDevice device,
		const VkImageMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageMemoryRequirements2" );
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements2(
		VkDevice device,
		const VkBufferMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetBufferMemoryRequirements2" );
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements2(
		VkDevice device,
		const VkImageSparseMemoryRequirementsInfo2* pInfo,
		uint32_t* pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageSparseMemoryRequirements2" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2 * pFeatures )
	{
		pFeatures->features = get( physicalDevice )->getFeatures();
		VkBaseOutStructure * pNext = reinterpret_cast< VkBaseOutStructure * >( pFeatures->pNext );

		while ( pNext )
		{
#if VK_KHR_portability_subset
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR )
			{
				auto next = pNext->pNext;
				*reinterpret_cast< VkPhysicalDevicePortabilitySubsetFeaturesKHR * >( pNext ) = get( physicalDevice )->getPortabilitySubset();
				pNext->pNext = next;
			}
#endif
#if VK_EXT_inline_uniform_block
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT )
			{
				auto next = pNext->pNext;
				*reinterpret_cast< VkPhysicalDeviceInlineUniformBlockFeaturesEXT * >( pNext ) = get( physicalDevice )->getInlineUniformBlockFeatures();
				pNext->pNext = next;
			}
#endif

			pNext = reinterpret_cast< VkBaseOutStructure * >( pNext->pNext );
		}
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2 * pProperties )
	{
		pProperties->properties = get( physicalDevice )->getProperties();
		VkBaseOutStructure * pNext = reinterpret_cast< VkBaseOutStructure * >( pProperties->pNext );

		while ( pNext )
		{
#if VK_VERSION_1_2
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES )
			{
				auto next = pNext->pNext;
				*reinterpret_cast< VkPhysicalDeviceDriverProperties * >( pNext ) = get( physicalDevice )->getDriverProperties();
				pNext->pNext = next;
			}
#endif
#if VK_KHR_portability_subset
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR )
			{
				reinterpret_cast< VkPhysicalDevicePortabilitySubsetPropertiesKHR * >( pNext )->minVertexInputBindingStrideAlignment = 1u;
			}
#endif
#if VK_EXT_inline_uniform_block
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT )
			{
				auto next = pNext->pNext;
				*reinterpret_cast< VkPhysicalDeviceInlineUniformBlockPropertiesEXT * >( pNext ) = get( physicalDevice )->getInlineUniformBlockProperties();
				pNext->pNext = next;
			}
#endif

			pNext = reinterpret_cast< VkBaseOutStructure * >( pNext->pNext );
		}
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2 * pFormatProperties )
	{
		pFormatProperties->formatProperties = get( physicalDevice )->getFormatProperties( format );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2 * pImageFormatInfo,
		VkImageFormatProperties2 * pImageFormatProperties )
	{
		return get( physicalDevice )->getImageFormatProperties( pImageFormatInfo->format
			, pImageFormatInfo->type
			, pImageFormatInfo->tiling
			, pImageFormatInfo->usage
			, pImageFormatInfo->flags
			, pImageFormatProperties->imageFormatProperties );
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2 * pQueueFamilyProperties )
	{
		auto props = get( physicalDevice )->getQueueFamilyProperties();

		if ( pQueueFamilyProperties
			&& pQueueFamilyPropertyCount )
		{
			auto requested = *pQueueFamilyPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					pQueueFamilyProperties->queueFamilyProperties = prop;
					++pQueueFamilyProperties;
				}

				++index;
			}

			*pQueueFamilyPropertyCount = std::min( requested, uint32_t( props.size() ) );
		}

		if ( !pQueueFamilyPropertyCount )
		{
			return;
		}

		*pQueueFamilyPropertyCount = uint32_t( props.size() );
	}

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2 * pMemoryProperties )
	{
		pMemoryProperties->memoryProperties = get( physicalDevice )->getMemoryProperties();
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2 * pFormatInfo,
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties2 * pProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSparseImageFormatProperties2" );
	}

	void VKAPI_CALL vkTrimCommandPool(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolTrimFlags flags )
	{
	}

	void VKAPI_CALL vkGetDeviceQueue2(
		VkDevice device,
		const VkDeviceQueueInfo2* pQueueInfo,
		VkQueue* pQueue )
	{
		reportUnsupported( device, "vkGetDeviceQueue2" );
	}

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(
		VkDevice device,
		const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		return reportUnsupported( device, "vkCreateSamplerYcbcrConversion" );
	}

	void VKAPI_CALL vkDestroySamplerYcbcrConversion(
		VkDevice device,
		VkSamplerYcbcrConversion ycbcrConversion,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroySamplerYcbcrConversion" );
	}

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(
		VkDevice device,
		const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		return reportUnsupported( device, "vkCreateDescriptorUpdateTemplate" );
	}

	void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(
		VkDevice device,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroyDescriptorUpdateTemplate" );
	}

	void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(
		VkDevice device,
		VkDescriptorSet descriptorSet,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const void* pData )
	{
		reportUnsupported( device, "vkUpdateDescriptorSetWithTemplate" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
		VkExternalBufferProperties* pExternalBufferProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalBufferProperties" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
		VkExternalFenceProperties* pExternalFenceProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalFenceProperties" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
		VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalSemaphoreProperties" );
	}

	void VKAPI_CALL vkGetDescriptorSetLayoutSupport(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
		VkDescriptorSetLayoutSupport* pSupport )
	{
		reportUnsupported( device, "vkGetDescriptorSetLayoutSupport" );
	}

#endif
#pragma endregion
#pragma region VK_API_VERSION_1_2
#ifdef VK_API_VERSION_1_2

	VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCount(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectCount" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCount(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		VkBuffer countBuffer,
		VkDeviceSize countBufferOffset,
		uint32_t maxDrawCount,
		uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndexedIndirectCount" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2(
		VkDevice device,
		const VkRenderPassCreateInfo2 * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkRenderPass * pRenderPass )
	{
		return reportUnsupported( device, "vkCreateRenderPass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo * pRenderPassBegin,
		const VkSubpassBeginInfo * pSubpassBeginInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginRenderPass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2(
		VkCommandBuffer commandBuffer,
		const VkSubpassBeginInfo * pSubpassBeginInfo,
		const VkSubpassEndInfo * pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdNextSubpass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2(
		VkCommandBuffer commandBuffer,
		const VkSubpassEndInfo * pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdEndRenderPass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkResetQueryPool(
		VkDevice device,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount )
	{
		reportUnsupported( device, "vkResetQueryPool" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValue(
		VkDevice device,
		VkSemaphore semaphore,
		uint64_t * pValue )
	{
		return reportUnsupported( device, "vkGetSemaphoreCounterValue" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphores(
		VkDevice device,
		const VkSemaphoreWaitInfo * pWaitInfo,
		uint64_t timeout )
	{
		return reportUnsupported( device, "vkWaitSemaphores" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphore(
		VkDevice device,
		const VkSemaphoreSignalInfo * pSignalInfo )
	{
		return reportUnsupported( device, "vkSignalSemaphore" );
	}

	VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddress(
		VkDevice device,
		const VkBufferDeviceAddressInfo * pInfo )
	{
		reportUnsupported( device, "vkGetBufferDeviceAddress" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddress(
		VkDevice device,
		const VkBufferDeviceAddressInfo * pInfo )
	{
		reportUnsupported( device, "vkGetBufferOpaqueCaptureAddress" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddress(
		VkDevice device,
		const VkDeviceMemoryOpaqueCaptureAddressInfo * pInfo )
	{
		reportUnsupported( device, "vkGetDeviceMemoryOpaqueCaptureAddress" );
		return 0u;
	}

#endif
#pragma endregion
#pragma region VK_KHR_timeline_semaphore
#ifdef VK_KHR_timeline_semaphore

	VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValueKHR(
		VkDevice device,
		VkSemaphore semaphore,
		uint64_t * pValue )
	{
		return reportUnsupported( device, "vkGetSemaphoreCounterValueKHR" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphoresKHR(
		VkDevice device,
		const VkSemaphoreWaitInfoKHR * pWaitInfo,
		uint64_t timeout )
	{
		return reportUnsupported( device, "vkWaitSemaphoresKHR" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphoreKHR(
		VkDevice device,
		const VkSemaphoreSignalInfoKHR * pSignalInfo )
	{
		return reportUnsupported( device, "vkSignalSemaphoreKHR" );
	}
#endif
#pragma endregion
#pragma region VK_KHR_buffer_device_address
#ifdef VK_KHR_buffer_device_address

	VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressKHR(
		VkDevice device,
		const VkBufferDeviceAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetBufferDeviceAddressKHR" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddressKHR(
		VkDevice device,
		const VkBufferDeviceAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetBufferOpaqueCaptureAddressKHR" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddressKHR(
		VkDevice device,
		const VkDeviceMemoryOpaqueCaptureAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetDeviceMemoryOpaqueCaptureAddressKHR" );
		return 0u;
	}

#endif
#pragma endregion
#pragma region VK_KHR_surface
#ifdef VK_KHR_surface

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
		auto formats = get( surface )->getFormats();
		*pSurfaceFormatCount = uint32_t( formats.size() );

		if ( pSurfaceFormats )
		{
			for ( auto & format : formats )
			{
				*pSurfaceFormats = format;
				++pSurfaceFormats;
			}
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t* pPresentModeCount,
		VkPresentModeKHR* pPresentModes )
	{
		auto modes = get( surface )->getPresentModes();
		*pPresentModeCount = uint32_t( modes.size() );

		if ( pPresentModes )
		{
			for ( auto & mode : modes )
			{
				*pPresentModes = mode;
				++pPresentModes;
			}
		}

		return VK_SUCCESS;
	}

#endif
#pragma endregion
#pragma region VK_KHR_swapchain
#ifdef VK_KHR_swapchain

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
		auto result = get( swapchain )->getImages();
		*pSwapchainImageCount = uint32_t( result.size() );

		if ( pSwapchainImages )
		{
			for ( auto & image : result )
			{
				*pSwapchainImages = image;
				++pSwapchainImages;
			}
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

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(
		VkDevice device,
		VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities )
	{
		return reportUnsupported( device, "vkGetDeviceGroupPresentCapabilitiesKHR" );
	}

	VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(
		VkDevice device,
		VkSurfaceKHR surface,
		VkDeviceGroupPresentModeFlagsKHR* pModes )
	{
		return reportUnsupported( device, "vkGetDeviceGroupSurfacePresentModesKHR" );
	}

	VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t* pRectCount,
		VkRect2D* pRects )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDevicePresentRectanglesKHR" );
	}

	VkResult VKAPI_CALL vkAcquireNextImage2KHR(
		VkDevice device,
		const VkAcquireNextImageInfoKHR* pAcquireInfo,
		uint32_t* pImageIndex )
	{
		return reportUnsupported( device, "vkAcquireNextImage2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPropertiesKHR * pProperties )
	{
		auto props = get( physicalDevice )->getDisplayProperties();
		*pPropertyCount = uint32_t( props.size() );

		if ( pProperties )
		{
			for ( auto & prop : props )
			{
				*pProperties = prop;
				++pProperties;
			}
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPlanePropertiesKHR * pProperties )
	{
		auto props = get( physicalDevice )->getDisplayPlaneProperties();

		if ( pProperties
			&& pPropertyCount )
		{
			auto requested = *pPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					*pProperties = prop;
					++pProperties;
				}

				++index;
			}

			*pPropertyCount = std::min( requested, uint32_t( props.size() ) );

			if ( requested >= uint32_t( props.size() ) )
			{
				return VK_SUCCESS;
			}

			return VK_INCOMPLETE;
		}

		if ( !pPropertyCount )
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		*pPropertyCount = uint32_t( props.size() );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t planeIndex,
		uint32_t * pDisplayCount,
		VkDisplayKHR * pDisplays )
	{
		auto props = get( physicalDevice )->getDisplayPlaneSupportedDisplays( planeIndex );
		*pDisplayCount = uint32_t( props.size() );

		if ( pDisplays )
		{
			for ( auto & prop : props )
			{
				*pDisplays = prop;
				++pDisplays;
			}
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t * pPropertyCount,
		VkDisplayModePropertiesKHR * pProperties )
	{
		auto props = get( display )->getDisplayModeProperties();

		if ( pProperties
			&& pPropertyCount )
		{
			auto requested = *pPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					*pProperties = prop;
					++pProperties;
				}

				++index;
			}

			*pPropertyCount = std::min( requested, uint32_t( props.size() ) );

			if ( requested >= uint32_t( props.size() ) )
			{
				return VK_SUCCESS;
			}

			return VK_INCOMPLETE;
		}

		if ( !pPropertyCount )
		{
			return VK_ERROR_OUT_OF_HOST_MEMORY;
		}

		*pPropertyCount = uint32_t( props.size() );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkCreateDisplayModeKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		const VkDisplayModeCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDisplayModeKHR * pMode )
	{
		return allocate( *pMode
			, pAllocator
			, display
			, *pCreateInfo );
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayModeKHR mode,
		uint32_t planeIndex,
		VkDisplayPlaneCapabilitiesKHR * pCapabilities )
	{
		*pCapabilities = get( mode )->getDisplayPlaneCapabilities( planeIndex );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
	{
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_physical_device_properties2
#ifdef VK_KHR_get_physical_device_properties2

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2KHR * pFeatures )
	{
		pFeatures->features = get( physicalDevice )->getFeatures();
		VkBaseOutStructure * pNext = reinterpret_cast< VkBaseOutStructure * >( pFeatures->pNext );

		while ( pNext )
		{
#if VK_KHR_portability_subset
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR )
			{
				*reinterpret_cast< VkPhysicalDevicePortabilitySubsetFeaturesKHR * >( pNext ) = get( physicalDevice )->getPortabilitySubset();
			}
#endif
#if VK_EXT_inline_uniform_block
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT )
			{
				auto next = pNext->pNext;
				*reinterpret_cast< VkPhysicalDeviceInlineUniformBlockFeaturesEXT * >( pNext ) = get( physicalDevice )->getInlineUniformBlockFeatures();
				pNext->pNext = next;
			}
#endif

			pNext = reinterpret_cast< VkBaseOutStructure * >( pNext->pNext );
		}
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2KHR * pProperties )
	{
		pProperties->properties = get( physicalDevice )->getProperties();
		VkBaseOutStructure * pNext = reinterpret_cast< VkBaseOutStructure * >( pProperties->pNext );

		while ( pNext )
		{
#if VK_KHR_driver_properties
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR )
			{
				*reinterpret_cast< VkPhysicalDeviceDriverPropertiesKHR * >( pNext ) = get( physicalDevice )->getDriverProperties();
			}
#endif
#if VK_KHR_portability_subset
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_PROPERTIES_KHR )
			{
				reinterpret_cast< VkPhysicalDevicePortabilitySubsetPropertiesKHR * >( pNext )->minVertexInputBindingStrideAlignment = 1u;
			}
#endif
#if VK_EXT_inline_uniform_block
			if ( pNext->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT )
			{
				auto next = pNext->pNext;
				*reinterpret_cast< VkPhysicalDeviceInlineUniformBlockPropertiesEXT * >( pNext ) = get( physicalDevice )->getInlineUniformBlockProperties();
				pNext->pNext = next;
			}
#endif

			pNext = reinterpret_cast< VkBaseOutStructure * >( pNext->pNext );
		}
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2KHR * pFormatProperties )
	{
		pFormatProperties->formatProperties = get( physicalDevice )->getFormatProperties( format );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2KHR * pImageFormatInfo,
		VkImageFormatProperties2KHR * pImageFormatProperties )
	{
		return get( physicalDevice )->getImageFormatProperties( pImageFormatInfo->format
			, pImageFormatInfo->type
			, pImageFormatInfo->tiling
			, pImageFormatInfo->usage
			, pImageFormatInfo->flags
			, pImageFormatProperties->imageFormatProperties );
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2KHR * pQueueFamilyProperties )
	{
		auto props = get( physicalDevice )->getQueueFamilyProperties();

		if ( pQueueFamilyProperties
			&& pQueueFamilyPropertyCount )
		{
			auto requested = *pQueueFamilyPropertyCount;
			uint32_t index = 0u;

			for ( auto & prop : props )
			{
				if ( index < requested )
				{
					pQueueFamilyProperties->queueFamilyProperties = prop;
					++pQueueFamilyProperties;
				}

				++index;
			}

			*pQueueFamilyPropertyCount = std::min( requested, uint32_t( props.size() ) );
		}

		if ( !pQueueFamilyPropertyCount )
		{
			return;
		}

		*pQueueFamilyPropertyCount = uint32_t( props.size() );
	}

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2KHR * pMemoryProperties )
	{
		pMemoryProperties->memoryProperties = get( physicalDevice )->getMemoryProperties();
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2KHR * pFormatInfo,
		uint32_t* pPropertyCount,
		VkSparseImageFormatProperties2KHR* pProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSparseImageFormatProperties2" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(
		VkDevice device,
		uint32_t heapIndex,
		uint32_t localDeviceIndex,
		uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		reportUnsupported( device, "vkGetDeviceGroupPeerMemoryFeaturesKHR" );
	}

	void VKAPI_CALL vkCmdSetDeviceMaskKHR(
		VkCommandBuffer commandBuffer,
		uint32_t deviceMask )
	{
		reportUnsupported( commandBuffer, "vkCmdSetDeviceMaskKHR" );
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
		reportUnsupported( commandBuffer, "vkCmdDispatchBaseKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_maintenance1
#ifdef VK_KHR_maintenance1

	void VKAPI_CALL vkTrimCommandPoolKHR(
		VkDevice device,
		VkCommandPool commandPool,
		VkCommandPoolTrimFlagsKHR flags )
	{
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group_creation
#ifdef VK_KHR_device_group_creation

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(
		VkInstance instance,
		uint32_t* pPhysicalDeviceGroupCount,
		VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties )
	{
		return reportUnsupported( instance, "vkEnumeratePhysicalDeviceGroupsKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_capabilities
#ifdef VK_KHR_external_memory_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo,
		VkExternalBufferProperties* pExternalBufferProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalBufferPropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_fd
#ifdef VK_KHR_external_memory_fd

	VkResult VKAPI_CALL vkGetMemoryFdKHR(
		VkDevice device,
		const VkMemoryGetFdInfoKHR* pGetFdInfo,
		int* pFd )
	{
		return reportUnsupported( device, "vkGetMemoryFdKHR" );
	}

	VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(
		VkDevice device,
		VkExternalMemoryHandleTypeFlagBits handleType,
		int fd,
		VkMemoryFdPropertiesKHR* pMemoryFdProperties )
	{
		return reportUnsupported( device, "vkGetMemoryFdPropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_capabilities
#ifdef VK_KHR_external_semaphore_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
		VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_fd
#ifdef VK_KHR_external_semaphore_fd

	VkResult VKAPI_CALL vkImportSemaphoreFdKHR(
		VkDevice device,
		const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo )
	{
		return reportUnsupported( device, "vkImportSemaphoreFdKHR" );
	}

	VkResult VKAPI_CALL vkGetSemaphoreFdKHR(
		VkDevice device,
		const VkSemaphoreGetFdInfoKHR* pGetFdInfo,
		int* pFd )
	{
		return reportUnsupported( device, "vkGetSemaphoreFdKHR" );
	}

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
		const VkWriteDescriptorSet* pDescriptorWrites )
	{
		reportUnsupported( commandBuffer, "vkCmdPushDescriptorSetKHR" );
	}

	void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(
		VkCommandBuffer commandBuffer,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		VkPipelineLayout layout,
		uint32_t set,
		const void* pData )
	{
		reportUnsupported( commandBuffer, "vkCmdPushDescriptorSetWithTemplateKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_descriptor_update_template
#ifdef VK_KHR_descriptor_update_template

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(
		VkDevice device,
		const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		return reportUnsupported( device, "vkCreateDescriptorUpdateTemplateKHR" );
	}

	void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(
		VkDevice device,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroyDescriptorUpdateTemplateKHR" );
	}

	void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(
		VkDevice device,
		VkDescriptorSet descriptorSet,
		VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		const void* pData )
	{
		reportUnsupported( device, "vkUpdateDescriptorSetWithTemplateKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_create_renderpass2
#ifdef VK_KHR_create_renderpass2

	VkResult VKAPI_CALL vkCreateRenderPass2KHR(
		VkDevice device,
		const VkRenderPassCreateInfo2KHR* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkRenderPass* pRenderPass )
	{
		return reportUnsupported( device, "vkCreateRenderPass2KHR" );
	}

	void VKAPI_CALL vkCmdBeginRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo* pRenderPassBegin,
		const VkSubpassBeginInfoKHR* pSubpassBeginInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginRenderPass2KHR" );
	}

	void VKAPI_CALL vkCmdNextSubpass2KHR(
		VkCommandBuffer commandBuffer,
		const VkSubpassBeginInfoKHR* pSubpassBeginInfo,
		const VkSubpassEndInfoKHR* pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdNextSubpass2KHR" );
	}

	void VKAPI_CALL vkCmdEndRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		const VkSubpassEndInfoKHR* pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdEndRenderPass2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_shared_presentable_image
#ifdef VK_KHR_shared_presentable_image

	VkResult VKAPI_CALL vkGetSwapchainStatusKHR(
		VkDevice device,
		VkSwapchainKHR swapchain )
	{
		return reportUnsupported( device, "vkImportFenceFdKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_capabilities
#ifdef VK_KHR_external_fence_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo,
		VkExternalFenceProperties* pExternalFenceProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalFencePropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_fd
#ifdef VK_KHR_external_fence_fd

	VkResult VKAPI_CALL vkImportFenceFdKHR(
		VkDevice device,
		const VkImportFenceFdInfoKHR* pImportFenceFdInfo )
	{
		return reportUnsupported( device, "vkImportFenceFdKHR" );
	}

	VkResult VKAPI_CALL vkGetFenceFdKHR(
		VkDevice device,
		const VkFenceGetFdInfoKHR* pGetFdInfo,
		int* pFd )
	{
		return reportUnsupported( device, "vkGetFenceFdKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_surface_capabilities2
#ifdef VK_KHR_get_surface_capabilities2

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
		VkSurfaceCapabilities2KHR* pSurfaceCapabilities )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSurfaceCapabilities2KHR" );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
		uint32_t* pSurfaceFormatCount,
		VkSurfaceFormat2KHR* pSurfaceFormats )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSurfaceFormats2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_display_properties2
#ifdef VK_KHR_get_display_properties2

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkDisplayProperties2KHR* pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceDisplayProperties2KHR" );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkDisplayPlaneProperties2KHR* pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR" );
	}

	VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t* pPropertyCount,
		VkDisplayModeProperties2KHR* pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetDisplayModeProperties2KHR" );
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo,
		VkDisplayPlaneCapabilities2KHR* pCapabilities )
	{
		return reportUnsupported( physicalDevice, "vkGetDisplayPlaneCapabilities2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_memory_requirements2
#ifdef VK_KHR_get_memory_requirements2

	void VKAPI_CALL vkGetImageMemoryRequirements2KHR(
		VkDevice device,
		const VkImageMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageMemoryRequirements2KHR" );
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(
		VkDevice device,
		const VkBufferMemoryRequirementsInfo2* pInfo,
		VkMemoryRequirements2* pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetBufferMemoryRequirements2KHR" );
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(
		VkDevice device,
		const VkImageSparseMemoryRequirementsInfo2* pInfo,
		uint32_t* pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageSparseMemoryRequirements2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_sampler_ycbcr_conversion
#ifdef VK_KHR_sampler_ycbcr_conversion

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(
		VkDevice device,
		const VkSamplerYcbcrConversionCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		return reportUnsupported( device, "vkCreateSamplerYcbcrConversionKHR" );
	}

	void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(
		VkDevice device,
		VkSamplerYcbcrConversion ycbcrConversion,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroySamplerYcbcrConversionKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_bind_memory2
#ifdef VK_KHR_bind_memory2

	VkResult VKAPI_CALL vkBindBufferMemory2KHR(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo* pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			auto tmp = get( bindInfo.memory )->bindBuffer( bindInfo.buffer, bindInfo.memoryOffset );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
		}

		return result;
	}

	VkResult VKAPI_CALL vkBindImageMemory2KHR(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo* pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView ( pBindInfos, bindInfoCount ) )
		{
			auto tmp = get( bindInfo.memory )->bindImage( bindInfo.image, bindInfo.memoryOffset );
			result = VkResult( std::max< uint32_t >( tmp, result ) );
		}

		return result;
	}

#endif
#pragma endregion
#pragma region VK_KHR_maintenance3
#ifdef VK_KHR_maintenance3

	void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo* pCreateInfo,
		VkDescriptorSetLayoutSupport* pSupport )
	{
		reportUnsupported( device, "vkGetDescriptorSetLayoutSupportKHR" );
	}

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
		uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectCountKHR" );
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
		reportUnsupported( commandBuffer, "vkCmdDrawIndexedIndirectCountKHR" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_report
#ifdef VK_EXT_debug_report

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

#endif
#pragma endregion
#pragma region VK_EXT_debug_marker
#ifdef VK_EXT_debug_marker

	VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT(
		VkDevice device,
		const VkDebugMarkerObjectTagInfoEXT* pTagInfo )
	{
		return get( device )->debugMarkerSetObjectTag( *pTagInfo );
	}

	VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(
		VkDevice device,
		const VkDebugMarkerObjectNameInfoEXT* pNameInfo )
	{
		return get( device )->debugMarkerSetObjectName( *pNameInfo );
	}

	void VKAPI_CALL vkCmdDebugMarkerBeginEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT* pMarkerInfo )
	{
		return get( commandBuffer )->debugMarkerBegin( *pMarkerInfo );
	}

	void VKAPI_CALL vkCmdDebugMarkerEndEXT(
		VkCommandBuffer commandBuffer )
	{
		return get( commandBuffer )->debugMarkerEnd();
	}

	void VKAPI_CALL vkCmdDebugMarkerInsertEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT* pMarkerInfo )
	{
		return get( commandBuffer )->debugMarkerInsert( *pMarkerInfo );
	}

#endif
#pragma endregion
#pragma region VK_EXT_transform_feedback
#ifdef VK_EXT_transform_feedback

	void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstBinding,
		uint32_t bindingCount,
		const VkBuffer* pBuffers,
		const VkDeviceSize* pOffsets,
		const VkDeviceSize* pSizes )
	{
		reportUnsupported( commandBuffer, "vkCmdBindTransformFeedbackBuffersEXT" );
	}

	void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstCounterBuffer,
		uint32_t counterBufferCount,
		const VkBuffer* pCounterBuffers,
		const VkDeviceSize* pCounterBufferOffsets )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginTransformFeedbackEXT" );
	}

	void VKAPI_CALL vkCmdEndTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstCounterBuffer,
		uint32_t counterBufferCount,
		const VkBuffer* pCounterBuffers,
		const VkDeviceSize* pCounterBufferOffsets )
	{
		reportUnsupported( commandBuffer, "vkCmdEndTransformFeedbackEXT" );
	}

	void VKAPI_CALL vkCmdBeginQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		VkQueryControlFlags flags,
		uint32_t index )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginQueryIndexedEXT" );
	}

	void VKAPI_CALL vkCmdEndQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		VkQueryPool queryPool,
		uint32_t query,
		uint32_t index )
	{
		reportUnsupported( commandBuffer, "vkCmdEndQueryIndexedEXT" );
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
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectByteCountEXT" );
	}

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
		uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectCountAMD" );
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
		reportUnsupported( commandBuffer, "vkCmdDrawIndexedIndirectCountAMD" );
	}

#endif
#pragma endregion
#pragma region VK_AMD_shader_info
#ifdef VK_AMD_shader_info

	VkResult VKAPI_CALL vkGetShaderInfoAMD(
		VkDevice device,
		VkPipeline pipeline,
		VkShaderStageFlagBits shaderStage,
		VkShaderInfoTypeAMD infoType,
		size_t* pInfoSize,
		void* pInfo )
	{
		return reportUnsupported( device, "vkGetShaderInfoAMD" );
	}

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
		VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_conditional_rendering
#ifdef VK_EXT_conditional_rendering

	void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(
		VkCommandBuffer commandBuffer,
		const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginConditionalRenderingEXT" );
	}

	void VKAPI_CALL vkCmdEndConditionalRenderingEXT(
		VkCommandBuffer commandBuffer )
	{
		reportUnsupported( commandBuffer, "vkCmdEndConditionalRenderingEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NVX_device_generated_commands
#ifdef VK_NVX_device_generated_commands

	void VKAPI_CALL vkCmdProcessCommandsNVX(
		VkCommandBuffer commandBuffer,
		const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdProcessCommandsNVX" );
	}

	void VKAPI_CALL vkCmdReserveSpaceForCommandsNVX(
		VkCommandBuffer commandBuffer,
		const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdReserveSpaceForCommandsNVX" );
	}

	VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNVX(
		VkDevice device,
		const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout )
	{
		return reportUnsupported( device, "vkCreateIndirectCommandsLayoutNVX" );
	}

	void VKAPI_CALL vkDestroyIndirectCommandsLayoutNVX(
		VkDevice device,
		VkIndirectCommandsLayoutNVX indirectCommandsLayout,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroyIndirectCommandsLayoutNVX" );
	}

	VkResult VKAPI_CALL vkCreateObjectTableNVX(
		VkDevice device,
		const VkObjectTableCreateInfoNVX* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkObjectTableNVX* pObjectTable )
	{
		return reportUnsupported( device, "vkCreateObjectTableNVX" );
	}

	void VKAPI_CALL vkDestroyObjectTableNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroyObjectTableNVX" );
	}

	VkResult VKAPI_CALL vkRegisterObjectsNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		uint32_t objectCount,
		const VkObjectTableEntryNVX* const* ppObjectTableEntries,
		const uint32_t* pObjectIndices )
	{
		return reportUnsupported( device, "vkRegisterObjectsNVX" );
	}

	VkResult VKAPI_CALL vkUnregisterObjectsNVX(
		VkDevice device,
		VkObjectTableNVX objectTable,
		uint32_t objectCount,
		const VkObjectEntryTypeNVX* pObjectEntryTypes,
		const uint32_t* pObjectIndices )
	{
		return reportUnsupported( device, "vkUnregisterObjectsNVX" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(
		VkPhysicalDevice physicalDevice,
		VkDeviceGeneratedCommandsFeaturesNVX* pFeatures,
		VkDeviceGeneratedCommandsLimitsNVX* pLimits )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX" );
	}

#endif
#pragma endregion
#pragma region VK_NV_clip_space_w_scaling
#ifdef VK_NV_clip_space_w_scaling

	void VKAPI_CALL vkCmdSetViewportWScalingNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkViewportWScalingNV* pViewportWScalings )
	{
		reportUnsupported( commandBuffer, "vkCmdSetViewportWScalingNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_direct_mode_display
#ifdef VK_EXT_direct_mode_display

	VkResult VKAPI_CALL vkReleaseDisplayEXT(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display )
	{
		return reportUnsupported( physicalDevice, "vkReleaseDisplayEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_display_surface_counter
#ifdef VK_EXT_display_surface_counter

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkSurfaceCapabilities2EXT* pSurfaceCapabilities )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSurfaceCapabilities2EXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_display_control
#ifdef VK_EXT_display_control

	VkResult VKAPI_CALL vkDisplayPowerControlEXT(
		VkDevice device,
		VkDisplayKHR display,
		const VkDisplayPowerInfoEXT* pDisplayPowerInfo )
	{
		return reportUnsupported( device, "vkDisplayPowerControlEXT" );
	}

	VkResult VKAPI_CALL vkRegisterDeviceEventEXT(
		VkDevice device,
		const VkDeviceEventInfoEXT* pDeviceEventInfo,
		const VkAllocationCallbacks* pAllocator,
		VkFence* pFence )
	{
		return reportUnsupported( device, "vkRegisterDeviceEventEXT" );
	}

	VkResult VKAPI_CALL vkRegisterDisplayEventEXT(
		VkDevice device,
		VkDisplayKHR display,
		const VkDisplayEventInfoEXT* pDisplayEventInfo,
		const VkAllocationCallbacks* pAllocator,
		VkFence* pFence )
	{
		return reportUnsupported( device, "vkRegisterDisplayEventEXT" );
	}

	VkResult VKAPI_CALL vkGetSwapchainCounterEXT(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkSurfaceCounterFlagBitsEXT counter,
		uint64_t* pCounterValue )
	{
		return reportUnsupported( device, "vkGetSwapchainCounterEXT" );
	}

#endif
#pragma endregion
#pragma region VK_GOOGLE_display_timing
#ifdef VK_GOOGLE_display_timing

	VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(
		VkDevice device,
		VkSwapchainKHR swapchain,
		VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties )
	{
		return reportUnsupported( device, "vkGetRefreshCycleDurationGOOGLE" );
	}

	VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(
		VkDevice device,
		VkSwapchainKHR swapchain,
		uint32_t* pPresentationTimingCount,
		VkPastPresentationTimingGOOGLE* pPresentationTimings )
	{
		return reportUnsupported( device, "vkGetPastPresentationTimingGOOGLE" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_discard_rectangles
#ifdef VK_EXT_discard_rectangles

	void VKAPI_CALL vkCmdSetDiscardRectangleEXT(
		VkCommandBuffer commandBuffer,
		uint32_t firstDiscardRectangle,
		uint32_t discardRectangleCount,
		const VkRect2D* pDiscardRectangles )
	{
		reportUnsupported( commandBuffer, "vkCmdSetDiscardRectangleEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_hdr_metadata
#ifdef VK_EXT_hdr_metadata

	void VKAPI_CALL vkSetHdrMetadataEXT(
		VkDevice device,
		uint32_t swapchainCount,
		const VkSwapchainKHR* pSwapchains,
		const VkHdrMetadataEXT* pMetadata )
	{
		reportUnsupported( device, "vkSetHdrMetadataEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_utils
#ifdef VK_EXT_debug_utils

	VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(
		VkDevice device,
		const VkDebugUtilsObjectNameInfoEXT* pNameInfo )
	{
		return get( device )->setDebugUtilsObjectName( *pNameInfo );
	}

	VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(
		VkDevice device,
		const VkDebugUtilsObjectTagInfoEXT* pTagInfo )
	{
		return get( device )->setDebugUtilsObjectTag( *pTagInfo );
	}

	void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return get( queue )->beginDebugUtilsLabel( *pLabelInfo );
	}

	void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT(
		VkQueue queue )
	{
		return get( queue )->endDebugUtilsLabel();
	}

	void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return get( queue )->insertDebugUtilsLabel( *pLabelInfo );
	}

	void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return get( commandBuffer )->beginDebugUtilsLabel( *pLabelInfo );
	}

	void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer )
	{
		return get( commandBuffer )->endDebugUtilsLabel();
	}

	void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return get( commandBuffer )->insertDebugUtilsLabel( *pLabelInfo );
	}

	VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pMessenger )
	{
		assert( pMessenger );
		return allocate( *pMessenger
			, pAllocator
			, instance
			, *pCreateInfo );
		return VK_SUCCESS;
	}

	void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks* pAllocator )
	{
		deallocate( messenger, pAllocator );
	}

	void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(
		VkInstance instance,
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData )
	{
		return get( instance )->submitDebugUtilsMessenger( messageSeverity
			, messageTypes
			, *pCallbackData );
	}

#endif
#pragma endregion
#pragma region VK_EXT_sample_locations
#ifdef VK_EXT_sample_locations

	void VKAPI_CALL vkCmdSetSampleLocationsEXT(
		VkCommandBuffer commandBuffer,
		const VkSampleLocationsInfoEXT* pSampleLocationsInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdSetSampleLocationsEXT" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(
		VkPhysicalDevice physicalDevice,
		VkSampleCountFlagBits samples,
		VkMultisamplePropertiesEXT* pMultisampleProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceMultisamplePropertiesEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_image_drm_format_modifier
#ifdef VK_EXT_image_drm_format_modifier

	VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(
		VkDevice device,
		VkImage image,
		VkImageDrmFormatModifierPropertiesEXT* pProperties )
	{
		return reportUnsupported( device, "vkGetImageDrmFormatModifierPropertiesEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_validation_cache
#ifdef VK_EXT_validation_cache

	VkResult VKAPI_CALL vkCreateValidationCacheEXT(
		VkDevice device,
		const VkValidationCacheCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkValidationCacheEXT* pValidationCache )
	{
		return reportUnsupported( device, "vkCreateValidationCacheEXT" );
	}

	void VKAPI_CALL vkDestroyValidationCacheEXT(
		VkDevice device,
		VkValidationCacheEXT validationCache,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroyValidationCacheEXT" );
	}

	VkResult VKAPI_CALL vkMergeValidationCachesEXT(
		VkDevice device,
		VkValidationCacheEXT dstCache,
		uint32_t srcCacheCount,
		const VkValidationCacheEXT* pSrcCaches )
	{
		return reportUnsupported( device, "vkMergeValidationCachesEXT" );
	}

	VkResult VKAPI_CALL vkGetValidationCacheDataEXT(
		VkDevice device,
		VkValidationCacheEXT validationCache,
		size_t* pDataSize,
		void* pData )
	{
		return reportUnsupported( device, "vkGetValidationCacheDataEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NV_shading_rate_image
#ifdef VK_NV_shading_rate_image

	void VKAPI_CALL vkCmdBindShadingRateImageNV(
		VkCommandBuffer commandBuffer,
		VkImageView imageView,
		VkImageLayout imageLayout )
	{
		reportUnsupported( commandBuffer, "vkCmdBindShadingRateImageNV" );
	}

	void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstViewport,
		uint32_t viewportCount,
		const VkShadingRatePaletteNV* pShadingRatePalettes )
	{
		reportUnsupported( commandBuffer, "vkCmdSetViewportShadingRatePaletteNV" );
	}

	void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(
		VkCommandBuffer commandBuffer,
		VkCoarseSampleOrderTypeNV sampleOrderType,
		uint32_t customSampleOrderCount,
		const VkCoarseSampleOrderCustomNV* pCustomSampleOrders )
	{
		reportUnsupported( commandBuffer, "vkCmdSetCoarseSampleOrderNV" );
	}

#endif
#pragma endregion
#pragma region VK_NV_ray_tracing
#ifdef VK_NV_ray_tracing

	VkResult VKAPI_CALL vkCreateAccelerationStructureNV(
		VkDevice device,
		const VkAccelerationStructureCreateInfoNV* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkAccelerationStructureNV* pAccelerationStructure )
	{
		return reportUnsupported( device, "vkCreateAccelerationStructureNV" );
	}

	void VKAPI_CALL vkDestroyAccelerationStructureNV(
		VkDevice device,
		VkAccelerationStructureNV accelerationStructure,
		const VkAllocationCallbacks* pAllocator )
	{
		reportUnsupported( device, "vkDestroyAccelerationStructureNV" );
	}

	void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(
		VkDevice device,
		const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo,
		VkMemoryRequirements2KHR* pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetAccelerationStructureMemoryRequirementsNV" );
	}

	VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(
		VkDevice device,
		uint32_t bindInfoCount,
		const VkBindAccelerationStructureMemoryInfoNV* pBindInfos )
	{
		return reportUnsupported( device, "vkBindAccelerationStructureMemoryNV" );
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
		reportUnsupported( commandBuffer, "vkCmdBuildAccelerationStructureNV" );
	}

	void VKAPI_CALL vkCmdCopyAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		VkAccelerationStructureNV dst,
		VkAccelerationStructureNV src,
		VkCopyAccelerationStructureModeNV mode )
	{
		reportUnsupported( commandBuffer, "vkCmdCopyAccelerationStructureNV" );
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
		reportUnsupported( commandBuffer, "vkCmdTraceRaysNV" );
	}

	VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(
		VkDevice device,
		VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkRayTracingPipelineCreateInfoNV* pCreateInfos,
		const VkAllocationCallbacks* pAllocator,
		VkPipeline* pPipelines )
	{
		return reportUnsupported( device, "vkCreateRayTracingPipelinesNV" );
	}

	VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(
		VkDevice device,
		VkPipeline pipeline,
		uint32_t firstGroup,
		uint32_t groupCount,
		size_t dataSize,
		void* pData )
	{
		return reportUnsupported( device, "vkGetRayTracingShaderGroupHandlesNV" );
	}

	VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(
		VkDevice device,
		VkAccelerationStructureNV accelerationStructure,
		size_t dataSize,
		void* pData )
	{
		return reportUnsupported( device, "vkGetAccelerationStructureHandleNV" );
	}

	void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(
		VkCommandBuffer commandBuffer,
		uint32_t accelerationStructureCount,
		const VkAccelerationStructureNV* pAccelerationStructures,
		VkQueryType queryType,
		VkQueryPool queryPool,
		uint32_t firstQuery )
	{
		reportUnsupported( commandBuffer, "vkCmdWriteAccelerationStructuresPropertiesNV" );
	}

	VkResult VKAPI_CALL vkCompileDeferredNV(
		VkDevice device,
		VkPipeline pipeline,
		uint32_t shader )
	{
		return reportUnsupported( device, "vkCompileDeferredNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_external_memory_host
#ifdef VK_EXT_external_memory_host

	VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(
		VkDevice device,
		VkExternalMemoryHandleTypeFlagBits handleType,
		const void* pHostPointer,
		VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties )
	{
		return reportUnsupported( device, "vkGetMemoryHostPointerPropertiesEXT" );
	}

#endif
#pragma endregion
#pragma region VK_AMD_buffer_marker
#ifdef VK_AMD_buffer_marker

	void VKAPI_CALL vkCmdWriteBufferMarkerAMD(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlagBits pipelineStage,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		uint32_t marker )
	{
		reportUnsupported( commandBuffer, "vkCmdWriteBufferMarkerAMD" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_calibrated_timestamps
#ifdef VK_EXT_calibrated_timestamps

	VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(
		VkPhysicalDevice physicalDevice,
		uint32_t* pTimeDomainCount,
		VkTimeDomainEXT* pTimeDomains )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT" );
	}

	VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(
		VkDevice device,
		uint32_t timestampCount,
		const VkCalibratedTimestampInfoEXT* pTimestampInfos,
		uint64_t* pTimestamps,
		uint64_t* pMaxDeviation )
	{
		return reportUnsupported( device, "vkGetCalibratedTimestampsEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NV_mesh_shader
#ifdef VK_NV_mesh_shader

	void VKAPI_CALL vkCmdDrawMeshTasksNV(
		VkCommandBuffer commandBuffer,
		uint32_t taskCount,
		uint32_t firstTask )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksNV" );
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(
		VkCommandBuffer commandBuffer,
		VkBuffer buffer,
		VkDeviceSize offset,
		uint32_t drawCount,
		uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksIndirectNV" );
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
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksIndirectCountNV" );
	}

#endif
#pragma endregion
#pragma region VK_NV_scissor_exclusive
#ifdef VK_NV_scissor_exclusive

	void VKAPI_CALL vkCmdSetExclusiveScissorNV(
		VkCommandBuffer commandBuffer,
		uint32_t firstExclusiveScissor,
		uint32_t exclusiveScissorCount,
		const VkRect2D* pExclusiveScissors )
	{
		reportUnsupported( commandBuffer, "vkCmdSetExclusiveScissorNV" );
	}

#endif
#pragma endregion
#pragma region VK_NV_device_diagnostic_checkpoints
#ifdef VK_NV_device_diagnostic_checkpoints

	void VKAPI_CALL vkCmdSetCheckpointNV(
		VkCommandBuffer commandBuffer,
		const void* pCheckpointMarker )
	{
		reportUnsupported( commandBuffer, "vkCmdSetCheckpointNV" );
	}

	void VKAPI_CALL vkGetQueueCheckpointDataNV(
		VkQueue queue,
		uint32_t* pCheckpointDataCount,
		VkCheckpointDataNV* pCheckpointData )
	{
		reportUnsupported( queue, "vkGetQueueCheckpointDataNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_buffer_device_address
#ifdef VK_EXT_buffer_device_address

	VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(
		VkDevice device,
		const VkBufferDeviceAddressInfoEXT* pInfo )
	{
		return reportUnsupported( device, "vkGetBufferDeviceAddressEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NV_cooperative_matrix
#ifdef VK_NV_cooperative_matrix

	VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(
		VkPhysicalDevice physicalDevice,
		uint32_t* pPropertyCount,
		VkCooperativeMatrixPropertiesNV* pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef VK_USE_PLATFORM_ANDROID_KHR

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

#	endif
#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_FUCHSIA_imagepipe_surface
#	ifdef VK_USE_PLATFORM_FUCHSIA

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

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_MVK_ios_surface
#	ifdef __APPLE__

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

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#ifdef VK_MVK_macos_surface
#	ifdef __APPLE__

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

#	endif
#endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_NN_vi_surface
#	ifdef VK_USE_PLATFORM_VI_NN

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

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#ifdef VK_KHR_xcb_surface
#	ifdef __linux__

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

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#ifdef VK_KHR_xlib_surface
#	if __linux__
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

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#ifdef VK_KHR_wayland_surface
#	ifdef __linux__

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

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_KHR_win32_surface
#	ifdef _WIN32

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

#	endif
#endif
#pragma endregion
#pragma region VK_EXT_acquire_xlib_display
#ifdef VK_EXT_acquire_xlib_display
#	ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

	VkResult VKAPI_CALL vkAcquireXlibDisplayEXT(
		VkPhysicalDevice physicalDevice,
		Display* dpy,
		VkDisplayKHR display )
	{
		return reportUnsupported( physicalDevice, "vkAcquireXlibDisplayEXT" );
	}

	VkResult VKAPI_CALL vkGetRandROutputDisplayEXT(
		VkPhysicalDevice physicalDevice,
		Display* dpy,
		RROutput rrOutput,
		VkDisplayKHR* pDisplay )
	{
		return reportUnsupported( physicalDevice, "vkGetRandROutputDisplayEXT" );
	}

#	endif
#endif
#pragma endregion
}

namespace ashes::gl
{
	std::vector< VkExtensionProperties > const & getSupportedInstanceExtensions()
	{
		static std::vector< VkExtensionProperties > const extensions
		{
#if VK_KHR_surface
			VkExtensionProperties{ VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_SURFACE_SPEC_VERSION },
#endif
#	if _WIN32
			VkExtensionProperties{ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_SPEC_VERSION },
#	elif __linux__
			VkExtensionProperties{ VK_KHR_XLIB_SURFACE_EXTENSION_NAME, VK_KHR_XLIB_SURFACE_SPEC_VERSION },
			VkExtensionProperties{ VK_KHR_XCB_SURFACE_EXTENSION_NAME, VK_KHR_XCB_SURFACE_SPEC_VERSION },
#	elif __APPLE__
			VkExtensionProperties{ VK_MVK_MACOS_SURFACE_EXTENSION_NAME, VK_MVK_MACOS_SURFACE_SPEC_VERSION },
#	endif
#if VK_EXT_debug_report
			VkExtensionProperties{ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_SPEC_VERSION },
#endif
#if VK_EXT_debug_marker
			VkExtensionProperties{ VK_EXT_DEBUG_MARKER_EXTENSION_NAME, VK_EXT_DEBUG_MARKER_SPEC_VERSION },
#endif
#if VK_EXT_debug_utils
			VkExtensionProperties{ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_SPEC_VERSION },
#endif
#if VK_KHR_get_physical_device_properties2
			VkExtensionProperties{ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_SPEC_VERSION },
#endif
#if VK_KHR_display
			VkExtensionProperties{ VK_KHR_DISPLAY_EXTENSION_NAME, VK_KHR_DISPLAY_SPEC_VERSION },
#endif
#if VK_NV_glsl_shader
			VkExtensionProperties{ VK_NV_GLSL_SHADER_EXTENSION_NAME, VK_NV_GLSL_SHADER_SPEC_VERSION },
#endif
#if VK_KHR_portability_subset
			VkExtensionProperties{ VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, VK_KHR_PORTABILITY_SUBSET_SPEC_VERSION },
#endif
		};
		return extensions;
	}

	std::vector< VkLayerProperties > const & getInstanceLayerProperties()
	{
		static std::vector< VkLayerProperties > result;
		return result;
	}

	struct Library
	{
		AshPluginDescription description{ "gl"
			, "OpenGL renderer for Ashes"
			, nullptr
			, {}
			, {}
			, ASHPLUGIN_UNDEFINED };

		VkResult init( AshPluginMode mode )
		{
			auto validMode = ( description.mode == ASHPLUGIN_UNDEFINED || description.mode == mode );
			assert( validMode
				&& "ashesTestRenderer: Plugin mode has already been selected." );
			VkResult result = description.getInstanceProcAddr
				? ( validMode ? VK_SUCCESS : VK_ERROR_INCOMPATIBLE_DRIVER )
				: VK_ERROR_INITIALIZATION_FAILED;

			if ( result == VK_ERROR_INITIALIZATION_FAILED )
			{
				clearDebugFile();
				bool supported = false;
				gl::ExtensionsHandler extensions;

				try
				{
					gl::RenderWindow window{ MinMajor, MinMinor, "Gl4Init" };
					extensions.initialise();
					supported = extensions.getMajor() > MinMajor
						|| ( extensions.getMajor() == MinMajor && extensions.getMinor() >= MinMinor );
				}
				catch ( std::exception & exc )
				{
					std::cerr << exc.what() << std::endl;
				}

				description.getInstanceProcAddr = &vkGetInstanceProcAddr;
				description.features = extensions.getFeatures();
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_INSTANCE_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_DEVICE_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )
#include <ashes/ashes_functions_list.hpp>
				result = VK_SUCCESS;

				description.mode = mode;
				description.support.priority = 7u;
				description.support.supported = supported ? VK_TRUE : VK_FALSE;
			}

			return result;
		}
	};

	Library & getLibrary()
	{
		thread_local Library library;
		return library;
	}

	bool checkVersion( VkInstance instance
		, uint32_t version )
	{
		return Instance::getDefaultApiVersion() >= version;
	}

	bool checkVersionExt( VkInstance instance
		, uint32_t version
		, std::string_view extension )
	{
		return checkVersion( instance, version )
			&& get( instance )->hasExtension( extension.data() );
	}

	bool checkVersion( VkDevice device
		, uint32_t version )
	{
		return checkVersion( getInstance( device ), version );
	}

	bool checkVersionExt( VkDevice device
		, uint32_t version
		, std::string_view extension )
	{
		return checkVersion( getInstance( device ), version )
			&& get( device )->hasExtension( extension.data() );
	}

	using ObjectFunctions = std::map< std::string, PFN_vkVoidFunction >;

	ObjectFunctions const & getInstanceFunctions( VkInstance instance )
	{
		static std::map< VkInstance, ObjectFunctions > functions;
		auto it = functions.insert( { instance, {} } );

		if ( it.second )
		{
			if ( instance != VK_NULL_HANDLE )
			{
				it.first->second =
				{
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_INSTANCE_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_DEVICE_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_STATIC_LIB_DEVICE_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#include <ashes/ashes_functions_list.hpp>
				};
			}
			else
			{
				it.first->second =
				{
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
					{ "vk"#x, PFN_vkVoidFunction( vk##x ) },
#define VK_LIB_INSTANCE_FUNCTION( v, x )\
					{ "vk"#x, PFN_vkVoidFunction( vk##x ) },
#define VK_LIB_DEVICE_FUNCTION( v, x )\
					{ "vk"#x, PFN_vkVoidFunction( vk##x ) },
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )
#	define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )
#include <ashes/ashes_functions_list.hpp>
				};
			}
		}

		return it.first->second;
	}

	ObjectFunctions const & getDeviceFunctions( VkDevice device )
	{
		static std::map< VkDevice, ObjectFunctions > functions;
		auto it = functions.insert( { device, {} } );

		if ( it.second )
		{
			it.first->second =
			{
				{ "vkGetDeviceProcAddr", PFN_vkVoidFunction( vkGetDeviceProcAddr ) },
#define VK_LIB_DEVICE_FUNCTION( v, x )\
				{ "vk"#x, checkVersion( device, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )\
				{ "vk"#x, checkVersionExt( device, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_STATIC_LIB_DEVICE_FUNCTION_EXT( v, n, x )\
				{ "vk"#x, checkVersionExt( device, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#include <ashes/ashes_functions_list.hpp>
			};
		}

		return it.first->second;
	}

	PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance instance,
		const char* pName )
	{
		PFN_vkVoidFunction result{ nullptr };
		auto & functions = getInstanceFunctions( instance );
		auto it = functions.find( pName );

		if ( it != functions.end() )
		{
			result = it->second;
		}

		return result;
	}

	PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(
		VkDevice device,
		const char* pName )
	{
		PFN_vkVoidFunction result{ nullptr };
		auto & functions = getDeviceFunctions( device );
		auto it = functions.find( pName );

		if ( it != functions.end() )
		{
			result = it->second;
		}

		return result;
	}
}

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region ICD mode

	GlRenderer_API PFN_vkVoidFunction VKAPI_PTR vk_icdGetInstanceProcAddr( VkInstance instance
		, const char * name )
	{
		if ( ashes::gl::getLibrary().init( ASHPLUGIN_ICD ) == VK_SUCCESS )
		{
			return ashes::gl::vkGetInstanceProcAddr( instance, name );
		}

		return nullptr;
	}

	GlRenderer_API PFN_vkVoidFunction VKAPI_PTR vk_icdGetPhysicalDeviceProcAddr( VkInstance instance
		, const char * name )
	{
		if ( ashes::gl::getLibrary().init( ASHPLUGIN_ICD ) == VK_SUCCESS )
		{
			return ashes::gl::vkGetInstanceProcAddr( instance, name );
		}

		return nullptr;
	}

	GlRenderer_API VkResult VKAPI_PTR vk_icdNegotiateLoaderICDInterfaceVersion( uint32_t * pVersion )
	{
		auto result = ashes::gl::getLibrary().init( ASHPLUGIN_ICD );

		if ( result == VK_SUCCESS )
		{
			if ( !pVersion )
			{
				result = VK_ERROR_VALIDATION_FAILED_EXT;
			}
			else if ( *pVersion < 4 )
			{
				result = VK_ERROR_INCOMPATIBLE_DRIVER;
			}
			else
			{
				*pVersion = 4;
			}
		}

		return result;
	}

#pragma region VK_KHR_surface
#ifdef VK_KHR_surface

	GlRenderer_API void VKAPI_CALL vkDestroySurfaceKHR( VkInstance instance, VkSurfaceKHR surface, const  VkAllocationCallbacks * pAllocator )
	{
		return ashes::gl::vkDestroySurfaceKHR( instance, surface, pAllocator );
	}

	GlRenderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32 * pSupported )
	{
		return ashes::gl::vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported );
	}

	GlRenderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR * pSurfaceCapabilities )
	{
		return ashes::gl::vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, pSurfaceCapabilities );
	}

	GlRenderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t * pSurfaceFormatCount, VkSurfaceFormatKHR * pSurfaceFormats )
	{
		return ashes::gl::vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
	}

	GlRenderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t * pPresentModeCount, VkPresentModeKHR * pPresentModes )
	{
		return ashes::gl::vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, pPresentModes );
	}

#endif
#pragma endregion
#pragma region VK_KHR_swapchain
#ifdef VK_KHR_swapchain

	GlRenderer_API VkResult VKAPI_CALL vkCreateSwapchainKHR( VkDevice device, const  VkSwapchainCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSwapchainKHR * pSwapchain )
	{
		return ashes::gl::vkCreateSwapchainKHR( device, pCreateInfo, pAllocator, pSwapchain );
	}

#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	GlRenderer_API VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateDisplayPlaneSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef VK_USE_PLATFORM_ANDROID_KHR

	GlRenderer_API VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR( VkInstance instance, const  VkAndroidSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateAndroidSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_FUCHSIA_imagepipe_surface
#	ifdef VK_USE_PLATFORM_FUCHSIA

	GlRenderer_API VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA( VkInstance instance, const  VkImagePipeSurfaceCreateInfoFUCHSIA * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateImagePipeSurfaceFUCHSIA( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_MVK_ios_surface
#	ifdef VK_USE_PLATFORM_IOS_MVK

	GlRenderer_API VkResult VKAPI_CALL vkCreateIOSSurfaceMVK( VkInstance instance, const  VkIOSSurfaceCreateInfoMVK * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateIOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#	ifdef __APPLE__

	GlRenderer_API VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK( VkInstance instance, const  VkMacOSSurfaceCreateInfoMVK * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateMacOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_NN_vi_surface
#	ifdef VK_USE_PLATFORM_VI_NN

	GlRenderer_API VkResult VKAPI_CALL vkCreateViSurfaceNN( VkInstance instance, const  VkViSurfaceCreateInfoNN * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateViSurfaceNN( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#	ifdef __linux__

	GlRenderer_API VkResult VKAPI_CALL vkCreateXcbSurfaceKHR( VkInstance instance, const  VkXcbSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateXcbSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#	ifdef __linux__

	GlRenderer_API VkResult VKAPI_CALL vkCreateXlibSurfaceKHR( VkInstance instance, const  VkXlibSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateXlibSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
// #endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#	ifdef __linux__

	GlRenderer_API VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR( VkInstance instance, const  VkWaylandSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateWaylandSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_KHR_win32_surface
#	ifdef _WIN32

	GlRenderer_API VkResult VKAPI_CALL vkCreateWin32SurfaceKHR( VkInstance instance, const  VkWin32SurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::gl::vkCreateWin32SurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion

#pragma endregion
#pragma region Drop-in replacement mode

	GlRenderer_API VkResult VKAPI_PTR ashGetPluginDescription( AshPluginDescription * pDescription )
	{
		auto result = ashes::gl::getLibrary().init( ASHPLUGIN_DROPIN );

		if ( result == VK_SUCCESS )
		{
			*pDescription = ashes::gl::getLibrary().description;
		}

		return result;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif
