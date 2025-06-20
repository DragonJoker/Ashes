#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
#include <ashes/ashes.h>

#include "Core/D3D11Device.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include "ashesd3d11_api.hpp"

#include <ashes/common/Exception.hpp>

#include <cstring>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <iostream>
#pragma warning( pop )

#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

namespace ashes::D3D11_NAMESPACE
{
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
		VkInstance * pInstance )
	{
		assert( pInstance );
		return allocate( *pInstance
			, pAllocator
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyInstance(
		VkInstance instance,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( instance, pAllocator );
	}

	VkResult VKAPI_CALL vkEnumeratePhysicalDevices(
		VkInstance instance,
		uint32_t * pPhysicalDeviceCount,
		VkPhysicalDevice * pPhysicalDevices )
	{
		VkResult result = VK_SUCCESS;

		try
		{
			auto gpus = get( instance )->enumeratePhysicalDevices();
			*pPhysicalDeviceCount = uint32_t( gpus.size() );

			if ( pPhysicalDevices )
			{
				for ( auto & gpu : gpus )
				{
					*pPhysicalDevices = gpu;
					++pPhysicalDevices;
				}
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
		VkPhysicalDeviceFeatures * pFeatures )
	{
		*pFeatures = get( physicalDevice )->getFeatures();
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties * pFormatProperties )
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
		VkImageFormatProperties * pImageFormatProperties )
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
		VkPhysicalDeviceProperties * pProperties )
	{
		*pProperties = get( physicalDevice )->getProperties();
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties * pQueueFamilyProperties )
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
		VkPhysicalDeviceMemoryProperties * pMemoryProperties )
	{
		*pMemoryProperties = get( physicalDevice )->getMemoryProperties();
	}

	VkResult VKAPI_CALL vkCreateDevice(
		VkPhysicalDevice physicalDevice,
		const VkDeviceCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDevice * pDevice )
	{
		auto & gpu = *get( physicalDevice );
		auto instance = gpu.getInstance();
		assert( pDevice );
		return allocate( *pDevice
			, pAllocator
			, instance
			, physicalDevice
			, pAllocator
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDevice(
		VkDevice device,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( device, pAllocator );
	}

	VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(
		const char * pLayerName,
		uint32_t * pPropertyCount,
		VkExtensionProperties * pProperties )
	{
		auto & props = getSupportedInstanceExtensions( pLayerName );
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

	VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const char * pLayerName,
		uint32_t * pPropertyCount,
		VkExtensionProperties * pProperties )
	{
		auto props = get( physicalDevice )->enumerateExtensionProperties();
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

	VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(
		uint32_t * pPropertyCount,
		VkLayerProperties * pProperties )
	{
		auto & props = getInstanceLayerProperties();
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

	VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkLayerProperties * pProperties )
	{
		auto props = get( physicalDevice )->enumerateLayerProperties();
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

	void VKAPI_CALL vkGetDeviceQueue(
		VkDevice device,
		uint32_t queueFamilyIndex,
		uint32_t queueIndex,
		VkQueue * pQueue )
	{
		assert( pQueue );
		*pQueue = get( device )->getQueue( queueFamilyIndex
			, queueIndex );
	}

	VkResult VKAPI_CALL vkQueueSubmit(
		VkQueue queue,
		uint32_t submitCount,
		const VkSubmitInfo * pSubmits,
		[[maybe_unused]] VkFence fence )
	{
		return get( queue )->submit( makeArrayView( pSubmits, submitCount ) );
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
		const VkMemoryAllocateInfo * pAllocateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDeviceMemory * pMemory )
	{
		assert( pMemory );
		return allocate( *pMemory
			, pAllocator
			, device
			, *pAllocateInfo );
	}

	void VKAPI_CALL vkFreeMemory(
		[[maybe_unused]] VkDevice device,
		VkDeviceMemory memory,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( memory, pAllocator );
	}

	VkResult VKAPI_CALL vkMapMemory(
		[[maybe_unused]] VkDevice device,
		VkDeviceMemory memory,
		VkDeviceSize offset,
		VkDeviceSize size,
		VkMemoryMapFlags flags,
		void ** ppData )
	{
		return get( memory )->lock( offset
			, size
			, flags
			, ppData );
	}

	void VKAPI_CALL vkUnmapMemory(
		VkDevice device,
		VkDeviceMemory memory )
	{
		get( memory )->unlock( get( device )->getImmediateContext() );
	}

	VkResult VKAPI_CALL vkFlushMappedMemoryRanges(
		[[maybe_unused]] VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange * pMemoryRanges )
	{
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
		{
			result = get( pMemoryRanges->memory )->flush( get( device )->getImmediateContext()
				, pMemoryRanges->offset
				, pMemoryRanges->size );
			++pMemoryRanges;
		}

		return result;
	}

	VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges(
		[[maybe_unused]] VkDevice device,
		uint32_t memoryRangeCount,
		const VkMappedMemoryRange * pMemoryRanges )
	{
		VkResult result = VK_SUCCESS;

		for ( uint32_t i = 0u; i < memoryRangeCount; ++i )
		{
			result = get( pMemoryRanges->memory )->invalidate( get( device )->getImmediateContext()
				, pMemoryRanges->offset
				, pMemoryRanges->size );
			++pMemoryRanges;
		}

		return result;
	}

	void VKAPI_CALL vkGetDeviceMemoryCommitment(
		[[maybe_unused]] VkDevice device,
		[[maybe_unused]] VkDeviceMemory memory,
		VkDeviceSize * pCommittedMemoryInBytes )
	{
		*pCommittedMemoryInBytes = 0;
	}

	VkResult VKAPI_CALL vkBindBufferMemory(
		[[maybe_unused]] VkDevice device,
		VkBuffer buffer,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset )
	{
		return get( buffer )->bindMemory( memory, memoryOffset );
	}

	VkResult VKAPI_CALL vkBindImageMemory(
		[[maybe_unused]] VkDevice device,
		VkImage image,
		VkDeviceMemory memory,
		VkDeviceSize memoryOffset )
	{
		return get( image )->bindMemory( memory, memoryOffset );
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements(
		[[maybe_unused]] VkDevice device,
		VkBuffer buffer,
		VkMemoryRequirements * pMemoryRequirements )
	{
		*pMemoryRequirements = get( buffer )->getMemoryRequirements();
	}

	void VKAPI_CALL vkGetImageMemoryRequirements(
		[[maybe_unused]] VkDevice device,
		VkImage image,
		VkMemoryRequirements * pMemoryRequirements )
	{
		*pMemoryRequirements = get( image )->getMemoryRequirements();
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements(
		[[maybe_unused]] VkDevice device,
		VkImage image,
		uint32_t * pSparseMemoryRequirementCount,
		VkSparseImageMemoryRequirements * pSparseMemoryRequirements )
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
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties * pProperties )
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
			for ( auto const & prop : props )
			{
				*pProperties = prop;
				++pProperties;
			}
		}
	}

	VkResult VKAPI_CALL vkQueueBindSparse(
		VkQueue queue,
		[[maybe_unused]] uint32_t bindInfoCount,
		[[maybe_unused]] const VkBindSparseInfo * pBindInfo,
		[[maybe_unused]] VkFence fence )
	{
		return get( queue )->bindSparse();
	}

	VkResult VKAPI_CALL vkCreateFence(
		VkDevice device,
		[[maybe_unused]] const VkFenceCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkFence * pFence )
	{
		assert( pFence );
		return allocate( *pFence
			, pAllocator
			, device );
	}

	void VKAPI_CALL vkDestroyFence(
		[[maybe_unused]] VkDevice device,
		VkFence fence,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( fence, pAllocator );
	}

	VkResult VKAPI_CALL vkResetFences(
		[[maybe_unused]] VkDevice device,
		uint32_t fenceCount,
		const VkFence * pFences )
	{
		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			get( *pFences )->reset();
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetFenceStatus(
		[[maybe_unused]] VkDevice device,
		VkFence fence )
	{
		return get( fence )->getStatus();
	}

	VkResult VKAPI_CALL vkWaitForFences(
		[[maybe_unused]] VkDevice device,
		uint32_t fenceCount,
		const VkFence * pFences,
		[[maybe_unused]] VkBool32 waitAll,
		[[maybe_unused]] uint64_t timeout )
	{
		for ( uint32_t i = 0u; i < fenceCount; ++i )
		{
			get( *pFences )->wait();
		}

		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkCreateSemaphore(
		VkDevice device,
		[[maybe_unused]] const VkSemaphoreCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSemaphore * pSemaphore )
	{
		assert( pSemaphore );
		return allocate( *pSemaphore
			, pAllocator
			, device );
	}

	void VKAPI_CALL vkDestroySemaphore(
		[[maybe_unused]] VkDevice device,
		VkSemaphore semaphore,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( semaphore, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateEvent(
		VkDevice device,
		[[maybe_unused]] const VkEventCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkEvent * pEvent )
	{
		assert( pEvent );
		return allocate( *pEvent
			, pAllocator
			, device );
	}

	void VKAPI_CALL vkDestroyEvent(
		[[maybe_unused]] VkDevice device,
		VkEvent event,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( event, pAllocator );
	}

	VkResult VKAPI_CALL vkGetEventStatus(
		[[maybe_unused]] VkDevice device,
		VkEvent event )
	{
		return get( event )->getStatus();
	}

	VkResult VKAPI_CALL vkSetEvent(
		[[maybe_unused]] VkDevice device,
		VkEvent event )
	{
		return get( event )->set();
	}

	VkResult VKAPI_CALL vkResetEvent(
		[[maybe_unused]] VkDevice device,
		VkEvent event )
	{
		return get( event )->reset();
	}

	VkResult VKAPI_CALL vkCreateQueryPool(
		VkDevice device,
		const VkQueryPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkQueryPool * pQueryPool )
	{
		assert( pQueryPool );
		return allocate( *pQueryPool
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyQueryPool(
		[[maybe_unused]] VkDevice device,
		VkQueryPool queryPool,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( queryPool, pAllocator );
	}

	VkResult VKAPI_CALL vkGetQueryPoolResults(
		[[maybe_unused]] VkDevice device,
		VkQueryPool queryPool,
		uint32_t firstQuery,
		uint32_t queryCount,
		size_t dataSize,
		void * pData,
		VkDeviceSize stride,
		VkQueryResultFlags flags )
	{
		return get( queryPool )->getResults( firstQuery
			, queryCount
			, stride
			, flags
			, dataSize
			, pData );
	}

	VkResult VKAPI_CALL vkCreateBuffer(
		VkDevice device,
		const VkBufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBuffer * pBuffer )
	{
		assert( pBuffer );
		return allocate( *pBuffer
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyBuffer(
		[[maybe_unused]] VkDevice device,
		VkBuffer buffer,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( buffer, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateBufferView(
		VkDevice device,
		const VkBufferViewCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkBufferView * pView )
	{
		assert( pView );
		return allocate( *pView
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyBufferView(
		[[maybe_unused]] VkDevice device,
		VkBufferView bufferView,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( bufferView, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateImage(
		VkDevice device,
		const VkImageCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkImage * pImage )
	{
		assert( pImage );
		return allocate( *pImage
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyImage(
		[[maybe_unused]] VkDevice device,
		VkImage image,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( image, pAllocator );
	}

	void VKAPI_CALL vkGetImageSubresourceLayout(
		VkDevice device,
		VkImage image,
		const VkImageSubresource * pSubresource,
		VkSubresourceLayout * pLayout )
	{
		get( device )->getImageSubresourceLayout( image
			, *pSubresource
			, *pLayout );
	}

	VkResult VKAPI_CALL vkCreateImageView(
		VkDevice device,
		const VkImageViewCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkImageView * pView )
	{
		assert( pView );
		return allocate( *pView
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyImageView(
		[[maybe_unused]] VkDevice device,
		VkImageView imageView,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( imageView, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateShaderModule(
		VkDevice device,
		const VkShaderModuleCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkShaderModule * pShaderModule )
	{
		assert( pShaderModule );
		return allocate( *pShaderModule
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyShaderModule(
		[[maybe_unused]] VkDevice device,
		VkShaderModule shaderModule,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( shaderModule, pAllocator );
	}

	VkResult VKAPI_CALL vkCreatePipelineCache(
		VkDevice device,
		const VkPipelineCacheCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkPipelineCache * pPipelineCache )
	{
		assert( pPipelineCache );
		return allocate( *pPipelineCache
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyPipelineCache(
		[[maybe_unused]] VkDevice device,
		VkPipelineCache pipelineCache,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( pipelineCache, pAllocator );
	}

	VkResult VKAPI_CALL vkGetPipelineCacheData(
		[[maybe_unused]] VkDevice device,
		VkPipelineCache pipelineCache,
		size_t * pDataSize,
		void * pData )
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
		[[maybe_unused]] VkDevice device,
		VkPipelineCache dstCache,
		[[maybe_unused]] uint32_t srcCacheCount,
		[[maybe_unused]] const VkPipelineCache * pSrcCaches )
	{
		return get( dstCache )->merge();
	}

	VkResult VKAPI_CALL vkCreateGraphicsPipelines(
		VkDevice device,
		[[maybe_unused]] VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkGraphicsPipelineCreateInfo * pCreateInfos,
		const VkAllocationCallbacks * pAllocator,
		VkPipeline * pPipelines )
	{
		assert( pPipelines );
		VkResult result = VK_SUCCESS;

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
		[[maybe_unused]] VkPipelineCache pipelineCache,
		uint32_t createInfoCount,
		const VkComputePipelineCreateInfo * pCreateInfos,
		const VkAllocationCallbacks * pAllocator,
		VkPipeline * pPipelines )
	{
		assert( pPipelines );
		VkResult result = VK_SUCCESS;

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
		[[maybe_unused]] VkDevice device,
		VkPipeline pipeline,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( pipeline, pAllocator );
	}

	VkResult VKAPI_CALL vkCreatePipelineLayout(
		VkDevice device,
		const VkPipelineLayoutCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkPipelineLayout * pPipelineLayout )
	{
		assert( pPipelineLayout );
		return allocate( *pPipelineLayout
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyPipelineLayout(
		[[maybe_unused]] VkDevice device,
		VkPipelineLayout pipelineLayout,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( pipelineLayout, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateSampler(
		VkDevice device,
		const VkSamplerCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSampler * pSampler )
	{
		assert( pSampler );
		return allocate( *pSampler
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroySampler(
		[[maybe_unused]] VkDevice device,
		VkSampler sampler,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( sampler, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateDescriptorSetLayout(
		VkDevice device,
		const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorSetLayout * pSetLayout )
	{
		assert( pSetLayout );
		return allocate( *pSetLayout
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDescriptorSetLayout(
		[[maybe_unused]] VkDevice device,
		VkDescriptorSetLayout descriptorSetLayout,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( descriptorSetLayout, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateDescriptorPool(
		VkDevice device,
		const VkDescriptorPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDescriptorPool * pDescriptorPool )
	{
		assert( pDescriptorPool );
		return allocate( *pDescriptorPool
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDescriptorPool(
		[[maybe_unused]] VkDevice device,
		VkDescriptorPool descriptorPool,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( descriptorPool, pAllocator );
	}

	VkResult VKAPI_CALL vkResetDescriptorPool(
		[[maybe_unused]] VkDevice device,
		VkDescriptorPool descriptorPool,
		[[maybe_unused]] VkDescriptorPoolResetFlags flags )
	{
		return get( descriptorPool )->reset();
	}

	VkResult VKAPI_CALL vkAllocateDescriptorSets(
		VkDevice device,
		const VkDescriptorSetAllocateInfo * pAllocateInfo,
		VkDescriptorSet * pDescriptorSets )
	{
		VkResult result = VK_SUCCESS;
		auto itLayout = pAllocateInfo->pSetLayouts;

		for ( auto itSet = pDescriptorSets;
			itSet != pDescriptorSets + pAllocateInfo->descriptorSetCount;
			++itLayout, ++itSet )
		{
			if ( result == VK_SUCCESS )
			{
				result = allocateNA( *itSet
					, device
					, pAllocateInfo->descriptorPool
					, *itLayout );
			}
		}

		return result;
	}

	VkResult VKAPI_CALL vkFreeDescriptorSets(
		[[maybe_unused]] VkDevice device,
		VkDescriptorPool descriptorPool,
		uint32_t descriptorSetCount,
		const VkDescriptorSet * pDescriptorSets )
	{
		return get( descriptorPool )->freeDescriptors( makeArrayView( pDescriptorSets, descriptorSetCount ) );
	}

	void VKAPI_CALL vkUpdateDescriptorSets(
		[[maybe_unused]] VkDevice device,
		uint32_t descriptorWriteCount,
		const VkWriteDescriptorSet * pDescriptorWrites,
		uint32_t descriptorCopyCount,
		const VkCopyDescriptorSet * pDescriptorCopies )
	{
		for ( auto & write : makeArrayView( pDescriptorWrites, descriptorWriteCount ) )
		{
			get( write.dstSet )->update( write );
		}

		for ( auto & copy : makeArrayView( pDescriptorCopies, descriptorCopyCount ) )
		{
			get( copy.dstSet )->update();
		}
	}

	VkResult VKAPI_CALL vkCreateFramebuffer(
		VkDevice device,
		const VkFramebufferCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkFramebuffer * pFramebuffer )
	{
		assert( pFramebuffer );
		return allocate( *pFramebuffer
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyFramebuffer(
		[[maybe_unused]] VkDevice device,
		VkFramebuffer framebuffer,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( framebuffer, pAllocator );
	}

	VkResult VKAPI_CALL vkCreateRenderPass(
		VkDevice device,
		const VkRenderPassCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkRenderPass * pRenderPass )
	{
		assert( pRenderPass );
		return allocate( *pRenderPass
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyRenderPass(
		[[maybe_unused]] VkDevice device,
		VkRenderPass renderPass,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( renderPass, pAllocator );
	}

	void VKAPI_CALL vkGetRenderAreaGranularity(
		[[maybe_unused]] VkDevice device,
		VkRenderPass renderPass,
		VkExtent2D * pGranularity )
	{
		*pGranularity = get( renderPass )->getRenderAreaGranularity();
	}

	VkResult VKAPI_CALL vkCreateCommandPool(
		VkDevice device,
		const VkCommandPoolCreateInfo * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkCommandPool * pCommandPool )
	{
		assert( pCommandPool );
		return allocate( *pCommandPool
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyCommandPool(
		[[maybe_unused]] VkDevice device,
		VkCommandPool commandPool,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( commandPool, pAllocator );
	}

	VkResult VKAPI_CALL vkResetCommandPool(
		[[maybe_unused]] VkDevice device,
		VkCommandPool commandPool,
		[[maybe_unused]] VkCommandPoolResetFlags flags )
	{
		return get( commandPool )->reset();
	}

	VkResult VKAPI_CALL vkAllocateCommandBuffers(
		VkDevice device,
		const VkCommandBufferAllocateInfo * pAllocateInfo,
		VkCommandBuffer * pCommandBuffers )
	{
		VkResult result = VK_SUCCESS;

		for ( auto it = pCommandBuffers;
			it != pCommandBuffers + pAllocateInfo->commandBufferCount;
			++it )
		{
			auto & cb = *it;

			if ( result == VK_SUCCESS )
			{
				result = allocateNA( cb
					, device
					, pAllocateInfo->commandPool
					, pAllocateInfo->level == VK_COMMAND_BUFFER_LEVEL_PRIMARY );
			}
		}

		return result;
	}

	void VKAPI_CALL vkFreeCommandBuffers(
		[[maybe_unused]] VkDevice device,
		VkCommandPool commandPool,
		uint32_t commandBufferCount,
		const VkCommandBuffer * pCommandBuffers )
	{
		get( commandPool )->freeCommands( makeArrayView( pCommandBuffers, commandBufferCount ) );
	}

	VkResult VKAPI_CALL vkBeginCommandBuffer(
		VkCommandBuffer commandBuffer,
		const VkCommandBufferBeginInfo * pBeginInfo )
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
		[[maybe_unused]] VkCommandBufferResetFlags flags )
	{
		return get( commandBuffer )->reset();
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
		const VkViewport * pViewports )
	{
		get( commandBuffer )->setViewport( firstViewport
			, makeArrayView( pViewports, viewportCount ) );
	}

	void VKAPI_CALL vkCmdSetScissor(
		VkCommandBuffer commandBuffer,
		uint32_t firstScissor,
		uint32_t scissorCount,
		const VkRect2D * pScissors )
	{
		get( commandBuffer )->setScissor( firstScissor
			, makeArrayView( pScissors, scissorCount ) );
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
		get( commandBuffer )->setDepthBounds( minDepthBounds
			, maxDepthBounds );
	}

	void VKAPI_CALL vkCmdSetStencilCompareMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t compareMask )
	{
		get( commandBuffer )->setStencilCompareMask( faceMask
			, compareMask );
	}

	void VKAPI_CALL vkCmdSetStencilWriteMask(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t writeMask )
	{
		get( commandBuffer )->setStencilWriteMask( faceMask
			, writeMask );
	}

	void VKAPI_CALL vkCmdSetStencilReference(
		VkCommandBuffer commandBuffer,
		VkStencilFaceFlags faceMask,
		uint32_t reference )
	{
		get( commandBuffer )->setStencilReference( faceMask
			, reference );
	}

	void VKAPI_CALL vkCmdBindDescriptorSets(
		VkCommandBuffer commandBuffer,
		VkPipelineBindPoint pipelineBindPoint,
		VkPipelineLayout layout,
		uint32_t firstSet,
		uint32_t descriptorSetCount,
		const VkDescriptorSet * pDescriptorSets,
		uint32_t dynamicOffsetCount,
		const uint32_t * pDynamicOffsets )
	{
		get( commandBuffer )->bindDescriptorSets( pipelineBindPoint
			, layout
			, firstSet
			, makeArrayView( pDescriptorSets, descriptorSetCount )
			, makeArrayView( pDynamicOffsets, dynamicOffsetCount ) );
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
		const VkBuffer * pBuffers,
		const VkDeviceSize * pOffsets )
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
			, uint32_t( vertexOffset )
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
		get( commandBuffer )->dispatch( groupCountX
			, groupCountY
			, groupCountZ );
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
		const VkBufferCopy * pRegions )
	{
		get( commandBuffer )->copyBuffer( srcBuffer
			, dstBuffer
			, makeArrayView( pRegions, regionCount ) );
	}

	void VKAPI_CALL vkCmdCopyImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		[[maybe_unused]] VkImageLayout srcImageLayout,
		VkImage dstImage,
		[[maybe_unused]] VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageCopy * pRegions )
	{
		get( commandBuffer )->copyImage( srcImage
			, dstImage
			, makeArrayView( pRegions, regionCount ) );
	}

	void VKAPI_CALL vkCmdBlitImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		[[maybe_unused]] VkImageLayout srcImageLayout,
		VkImage dstImage,
		[[maybe_unused]] VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageBlit * pRegions,
		VkFilter filter )
	{
		get( commandBuffer )->blitImage( srcImage
			, dstImage
			, makeArrayView( pRegions, regionCount )
			, filter );
	}

	void VKAPI_CALL vkCmdCopyBufferToImage(
		VkCommandBuffer commandBuffer,
		VkBuffer srcBuffer,
		VkImage dstImage,
		[[maybe_unused]] VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkBufferImageCopy * pRegions )
	{
		get( commandBuffer )->copyToImage( srcBuffer
			, dstImage
			, makeArrayView( pRegions, regionCount ) );
	}

	void VKAPI_CALL vkCmdCopyImageToBuffer(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		[[maybe_unused]] VkImageLayout srcImageLayout,
		VkBuffer dstBuffer,
		uint32_t regionCount,
		const VkBufferImageCopy * pRegions )
	{
		get( commandBuffer )->copyToBuffer( srcImage
			, dstBuffer
			, makeArrayView( pRegions, regionCount ) );
	}

	void VKAPI_CALL vkCmdUpdateBuffer(
		VkCommandBuffer commandBuffer,
		VkBuffer dstBuffer,
		VkDeviceSize dstOffset,
		VkDeviceSize dataSize,
		const void * pData )
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
		[[maybe_unused]] VkImageLayout imageLayout,
		const VkClearColorValue * pColor,
		uint32_t rangeCount,
		const VkImageSubresourceRange * pRanges )
	{
		get( commandBuffer )->clearColorImage( image
			, *pColor
			, makeArrayView( pRanges, rangeCount ) );
	}

	void VKAPI_CALL vkCmdClearDepthStencilImage(
		VkCommandBuffer commandBuffer,
		VkImage image,
		[[maybe_unused]] VkImageLayout imageLayout,
		const VkClearDepthStencilValue * pDepthStencil,
		uint32_t rangeCount,
		const VkImageSubresourceRange * pRanges )
	{
		get( commandBuffer )->clearDepthStencilImage( image
			, *pDepthStencil
			, makeArrayView( pRanges, rangeCount ) );
	}

	void VKAPI_CALL vkCmdClearAttachments(
		VkCommandBuffer commandBuffer,
		uint32_t attachmentCount,
		const VkClearAttachment * pAttachments,
		uint32_t rectCount,
		const VkClearRect * pRects )
	{
		get( commandBuffer )->clearAttachments( makeArrayView( pAttachments, attachmentCount )
		, makeArrayView( pRects, rectCount ) );
	}

	void VKAPI_CALL vkCmdResolveImage(
		VkCommandBuffer commandBuffer,
		VkImage srcImage,
		[[maybe_unused]] VkImageLayout srcImageLayout,
		VkImage dstImage,
		[[maybe_unused]] VkImageLayout dstImageLayout,
		uint32_t regionCount,
		const VkImageResolve * pRegions )
	{
		get( commandBuffer )->resolveImage( srcImage
			, dstImage
			, makeArrayView( pRegions, regionCount ) );
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
		const VkEvent * pEvents,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		[[maybe_unused]] uint32_t memoryBarrierCount,
		[[maybe_unused]] const VkMemoryBarrier * pMemoryBarriers,
		uint32_t bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier * pBufferMemoryBarriers,
		uint32_t imageMemoryBarrierCount,
		const VkImageMemoryBarrier * pImageMemoryBarriers )
	{
		get( commandBuffer )->waitEvents( { pEvents, pEvents + eventCount }
			, srcStageMask
			, dstStageMask
			, makeArrayView( pBufferMemoryBarriers, bufferMemoryBarrierCount )
			, makeArrayView( pImageMemoryBarriers, imageMemoryBarrierCount ) );
	}

	void VKAPI_CALL vkCmdPipelineBarrier(
		VkCommandBuffer commandBuffer,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		[[maybe_unused]] VkDependencyFlags dependencyFlags,
		uint32_t memoryBarrierCount,
		const VkMemoryBarrier * pMemoryBarriers,
		uint32_t bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier * pBufferMemoryBarriers,
		uint32_t imageMemoryBarrierCount,
		const VkImageMemoryBarrier * pImageMemoryBarriers )
	{
		get( commandBuffer )->pipelineBarrier( srcStageMask
			, dstStageMask
			, makeArrayView( pMemoryBarriers, memoryBarrierCount )
			, makeArrayView( pBufferMemoryBarriers, bufferMemoryBarrierCount )
			, makeArrayView( pImageMemoryBarriers, imageMemoryBarrierCount ) );
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
		const void * pValues )
	{
		get( commandBuffer )->pushConstants( layout
			, stageFlags
			, offset
			, size
			, pValues );
	}

	void VKAPI_CALL vkCmdBeginRenderPass(
		VkCommandBuffer commandBuffer,
		const VkRenderPassBeginInfo * pRenderPassBegin,
		[[maybe_unused]] VkSubpassContents contents )
	{
		get( commandBuffer )->beginRenderPass( *pRenderPassBegin );
	}

	void VKAPI_CALL vkCmdNextSubpass(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkSubpassContents contents )
	{
		get( commandBuffer )->nextSubpass();
	}

	void VKAPI_CALL vkCmdEndRenderPass(
		VkCommandBuffer commandBuffer )
	{
		get( commandBuffer )->endRenderPass();
	}

	void VKAPI_CALL vkCmdExecuteCommands(
		VkCommandBuffer commandBuffer,
		uint32_t commandBufferCount,
		const VkCommandBuffer * pCommandBuffers )
	{
		get( commandBuffer )->executeCommands( makeArrayView( pCommandBuffers, commandBufferCount ) );
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
		[[maybe_unused]] VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo * pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			if ( result == VK_SUCCESS )
			{
				result = get( bindInfo.buffer )->bindMemory( bindInfo.memory
					, bindInfo.memoryOffset );
			}
		}

		return result;
	}

	VkResult VKAPI_CALL vkBindImageMemory2(
		[[maybe_unused]] VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo * pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			if ( result == VK_SUCCESS )
			{
				result = get( bindInfo.image )->bindMemory( bindInfo.memory
					, bindInfo.memoryOffset );
			}
		}

		return result;
	}

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(
		VkDevice device,
		[[maybe_unused]] uint32_t heapIndex,
		[[maybe_unused]] uint32_t localDeviceIndex,
		[[maybe_unused]] uint32_t remoteDeviceIndex,
		VkPeerMemoryFeatureFlags * pPeerMemoryFeatures )
	{
		reportUnsupported( device, "vkGetDeviceGroupPeerMemoryFeatures" );
		*pPeerMemoryFeatures = VkPeerMemoryFeatureFlags{};
	}

	void VKAPI_CALL vkCmdSetDeviceMask(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t deviceMask )
	{
		reportUnsupported( commandBuffer, "vkCmdSetDeviceMask" );
	}

	void VKAPI_CALL vkCmdDispatchBase(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t baseGroupX,
		[[maybe_unused]] uint32_t baseGroupY,
		[[maybe_unused]] uint32_t baseGroupZ,
		[[maybe_unused]] uint32_t groupCountX,
		[[maybe_unused]] uint32_t groupCountY,
		[[maybe_unused]] uint32_t groupCountZ )
	{
		reportUnsupported( commandBuffer, "vkCmdDispatchBase" );
	}

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups(
		VkInstance instance,
		[[maybe_unused]] uint32_t * pPhysicalDeviceGroupCount,
		[[maybe_unused]] VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties )
	{
		return reportUnsupported( instance, "vkEnumeratePhysicalDeviceGroups" );
	}

	void VKAPI_CALL vkGetImageMemoryRequirements2(
		VkDevice device,
		[[maybe_unused]] const VkImageMemoryRequirementsInfo2 * pInfo,
		[[maybe_unused]] VkMemoryRequirements2 * pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageMemoryRequirements2" );
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements2(
		VkDevice device,
		[[maybe_unused]] const VkBufferMemoryRequirementsInfo2 * pInfo,
		[[maybe_unused]] VkMemoryRequirements2 * pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetBufferMemoryRequirements2" );
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements2(
		VkDevice device,
		[[maybe_unused]] const VkImageSparseMemoryRequirementsInfo2 * pInfo,
		[[maybe_unused]] uint32_t * pSparseMemoryRequirementCount,
		[[maybe_unused]] VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageSparseMemoryRequirements2" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2 * pFeatures )
	{
		*pFeatures = get( physicalDevice )->getFeatures2();
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2 * pProperties )
	{
		*pProperties = get( physicalDevice )->getProperties2();
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2 * pFormatProperties )
	{
		*pFormatProperties = get( physicalDevice )->getFormatProperties2( format );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2 * pImageFormatInfo,
		VkImageFormatProperties2 * pImageFormatProperties )
	{
		return get( physicalDevice )->getImageFormatProperties2( *pImageFormatInfo, *pImageFormatProperties );
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2 * pQueueFamilyProperties )
	{
		auto props = get( physicalDevice )->getQueueFamilyProperties2();
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

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2 * pMemoryProperties )
	{
		*pMemoryProperties = get( physicalDevice )->getMemoryProperties2();
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2 * pFormatInfo,
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties2 * pProperties )
	{
		std::vector< VkSparseImageFormatProperties2 > props;
		get( physicalDevice )->getSparseImageFormatProperties2( *pFormatInfo, props );
		*pPropertyCount = uint32_t( props.size() );

		if ( pProperties )
		{
			for ( auto & prop : props )
			{
				( *pProperties ) = prop;
				++pProperties;
			}
		}
	}

	void VKAPI_CALL vkTrimCommandPool(
		[[maybe_unused]] VkDevice device,
		[[maybe_unused]] VkCommandPool commandPool,
		[[maybe_unused]] VkCommandPoolTrimFlags flags )
	{
	}

	void VKAPI_CALL vkGetDeviceQueue2(
		VkDevice device,
		[[maybe_unused]] const VkDeviceQueueInfo2 * pQueueInfo,
		[[maybe_unused]] VkQueue * pQueue )
	{
		reportUnsupported( device, "vkGetDeviceQueue2" );
	}

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(
		VkDevice device,
		[[maybe_unused]] const VkSamplerYcbcrConversionCreateInfo * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkSamplerYcbcrConversion * pYcbcrConversion )
	{
		return reportUnsupported( device, "vkCreateSamplerYcbcrConversion" );
	}

	void VKAPI_CALL vkDestroySamplerYcbcrConversion(
		VkDevice device,
		[[maybe_unused]] VkSamplerYcbcrConversion ycbcrConversion,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroySamplerYcbcrConversion" );
	}

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(
		VkDevice device,
		[[maybe_unused]] const VkDescriptorUpdateTemplateCreateInfo * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkDescriptorUpdateTemplate * pDescriptorUpdateTemplate )
	{
		return reportUnsupported( device, "vkCreateDescriptorUpdateTemplate" );
	}

	void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(
		VkDevice device,
		[[maybe_unused]] VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyDescriptorUpdateTemplate" );
	}

	void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(
		VkDevice device,
		[[maybe_unused]] VkDescriptorSet descriptorSet,
		[[maybe_unused]] VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		[[maybe_unused]] const void * pData )
	{
		reportUnsupported( device, "vkUpdateDescriptorSetWithTemplate" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceExternalBufferInfo * pExternalBufferInfo,
		[[maybe_unused]] VkExternalBufferProperties * pExternalBufferProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalBufferProperties" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceExternalFenceInfo * pExternalFenceInfo,
		[[maybe_unused]] VkExternalFenceProperties * pExternalFenceProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalFenceProperties" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceExternalSemaphoreInfo * pExternalSemaphoreInfo,
		[[maybe_unused]] VkExternalSemaphoreProperties * pExternalSemaphoreProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalSemaphoreProperties" );
	}

	void VKAPI_CALL vkGetDescriptorSetLayoutSupport(
		VkDevice device,
		[[maybe_unused]] const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		[[maybe_unused]] VkDescriptorSetLayoutSupport * pSupport )
	{
		reportUnsupported( device, "vkGetDescriptorSetLayoutSupport" );
	}

#endif
#pragma endregion
#pragma region VK_API_VERSION_1_2
#ifdef VK_API_VERSION_1_2

	VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirectCount(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectCount" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirectCount(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndexedIndirectCount" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkCreateRenderPass2(
		VkDevice device,
		[[maybe_unused]] const VkRenderPassCreateInfo2 * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkRenderPass * pRenderPass )
	{
		return reportUnsupported( device, "vkCreateRenderPass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdBeginRenderPass2(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkRenderPassBeginInfo * pRenderPassBegin,
		[[maybe_unused]] const VkSubpassBeginInfo * pSubpassBeginInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginRenderPass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdNextSubpass2(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkSubpassBeginInfo * pSubpassBeginInfo,
		[[maybe_unused]] const VkSubpassEndInfo * pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdNextSubpass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkCmdEndRenderPass2(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkSubpassEndInfo * pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdEndRenderPass2" );
	}

	VKAPI_ATTR void VKAPI_CALL vkResetQueryPool(
		VkDevice device,
		[[maybe_unused]] VkQueryPool queryPool,
		[[maybe_unused]] uint32_t firstQuery,
		[[maybe_unused]] uint32_t queryCount )
	{
		reportUnsupported( device, "vkResetQueryPool" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkGetSemaphoreCounterValue(
		VkDevice device,
		[[maybe_unused]] VkSemaphore semaphore,
		[[maybe_unused]] uint64_t * pValue )
	{
		return reportUnsupported( device, "vkGetSemaphoreCounterValue" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphores(
		VkDevice device,
		[[maybe_unused]] const VkSemaphoreWaitInfo * pWaitInfo,
		[[maybe_unused]] uint64_t timeout )
	{
		return reportUnsupported( device, "vkWaitSemaphores" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphore(
		VkDevice device,
		[[maybe_unused]] const VkSemaphoreSignalInfo * pSignalInfo )
	{
		return reportUnsupported( device, "vkSignalSemaphore" );
	}

	VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddress(
		VkDevice device,
		[[maybe_unused]] const VkBufferDeviceAddressInfo * pInfo )
	{
		reportUnsupported( device, "vkGetBufferDeviceAddress" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddress(
		VkDevice device,
		[[maybe_unused]] const VkBufferDeviceAddressInfo * pInfo )
	{
		reportUnsupported( device, "vkGetBufferOpaqueCaptureAddress" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddress(
		VkDevice device,
		[[maybe_unused]] const VkDeviceMemoryOpaqueCaptureAddressInfo * pInfo )
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
		[[maybe_unused]] VkSemaphore semaphore,
		[[maybe_unused]] uint64_t * pValue )
	{
		return reportUnsupported( device, "vkGetSemaphoreCounterValueKHR" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkWaitSemaphoresKHR(
		VkDevice device,
		[[maybe_unused]] const VkSemaphoreWaitInfoKHR * pWaitInfo,
		[[maybe_unused]] uint64_t timeout )
	{
		return reportUnsupported( device, "vkWaitSemaphoresKHR" );
	}

	VKAPI_ATTR VkResult VKAPI_CALL vkSignalSemaphoreKHR(
		VkDevice device,
		[[maybe_unused]] const VkSemaphoreSignalInfoKHR * pSignalInfo )
	{
		return reportUnsupported( device, "vkSignalSemaphoreKHR" );
	}
#endif
#pragma endregion
#pragma region VK_KHR_buffer_device_address
#ifdef VK_KHR_buffer_device_address

	VKAPI_ATTR VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressKHR(
		VkDevice device,
		[[maybe_unused]] const VkBufferDeviceAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetBufferDeviceAddressKHR" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetBufferOpaqueCaptureAddressKHR(
		VkDevice device,
		[[maybe_unused]] const VkBufferDeviceAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetBufferOpaqueCaptureAddressKHR" );
		return 0u;
	}

	VKAPI_ATTR uint64_t VKAPI_CALL vkGetDeviceMemoryOpaqueCaptureAddressKHR(
		VkDevice device,
		[[maybe_unused]] const VkDeviceMemoryOpaqueCaptureAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetDeviceMemoryOpaqueCaptureAddressKHR" );
		return 0u;
	}

#endif
#pragma endregion
#pragma region VK_KHR_surface
#ifdef VK_KHR_surface

	void VKAPI_CALL vkDestroySurfaceKHR(
		[[maybe_unused]] VkInstance instance,
		VkSurfaceKHR surface,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( surface, pAllocator );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t queueFamilyIndex,
		VkSurfaceKHR surface,
		VkBool32 * pSupported )
	{
		*pSupported = get( surface )->getSupport( physicalDevice );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		VkSurfaceCapabilitiesKHR * pSurfaceCapabilities )
	{
		*pSurfaceCapabilities = get( surface )->getCapabilities( physicalDevice );
		return VK_SUCCESS;
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR(
		VkPhysicalDevice physicalDevice,
		VkSurfaceKHR surface,
		uint32_t * pSurfaceFormatCount,
		VkSurfaceFormatKHR * pSurfaceFormats )
	{
		auto formats = get( surface )->getFormats( physicalDevice );
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
		uint32_t * pPresentModeCount,
		VkPresentModeKHR * pPresentModes )
	{
		auto modes = get( surface )->getPresentModes( physicalDevice );
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
		const VkSwapchainCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSwapchainKHR * pSwapchain )
	{
		assert( pSwapchain );
		return allocate( *pSwapchain
			, pAllocator
			, device
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroySwapchainKHR(
		[[maybe_unused]] VkDevice device,
		VkSwapchainKHR swapchain,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( swapchain, pAllocator );
	}

	VkResult VKAPI_CALL vkGetSwapchainImagesKHR(
		[[maybe_unused]] VkDevice device,
		VkSwapchainKHR swapchain,
		uint32_t * pSwapchainImageCount,
		VkImage * pSwapchainImages )
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
		[[maybe_unused]] VkDevice device,
		VkSwapchainKHR swapchain,
		[[maybe_unused]] uint64_t timeout,
		[[maybe_unused]] VkSemaphore semaphore,
		[[maybe_unused]] VkFence fence,
		uint32_t * pImageIndex )
	{
		return get( swapchain )->acquireNextImage( *pImageIndex );
	}

	VkResult VKAPI_CALL vkQueuePresentKHR(
		VkQueue queue,
		const VkPresentInfoKHR * pPresentInfo )
	{
		return get( queue )->present( *pPresentInfo );
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR(
		VkDevice device,
		[[maybe_unused]] VkDeviceGroupPresentCapabilitiesKHR * pDeviceGroupPresentCapabilities )
	{
		return reportUnsupported( device, "vkGetDeviceGroupPresentCapabilitiesKHR" );
	}

	VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR(
		VkDevice device,
		[[maybe_unused]] VkSurfaceKHR surface,
		[[maybe_unused]] VkDeviceGroupPresentModeFlagsKHR * pModes )
	{
		return reportUnsupported( device, "vkGetDeviceGroupSurfacePresentModesKHR" );
	}

	VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkSurfaceKHR surface,
		[[maybe_unused]] uint32_t * pRectCount,
		[[maybe_unused]] VkRect2D * pRects )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDevicePresentRectanglesKHR" );
	}

	VkResult VKAPI_CALL vkAcquireNextImage2KHR(
		VkDevice device,
		[[maybe_unused]] const VkAcquireNextImageInfoKHR * pAcquireInfo,
		[[maybe_unused]] uint32_t * pImageIndex )
	{
		return reportUnsupported( device, "vkAcquireNextImage2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkDisplayPropertiesKHR * pProperties )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceDisplayPropertiesKHR" );
#else
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
#endif
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkDisplayPlanePropertiesKHR * pProperties )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR" );
#else
		auto props = get( physicalDevice )->getDisplayPlaneProperties();
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
#endif
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t planeIndex,
		[[maybe_unused]] uint32_t * pDisplayCount,
		[[maybe_unused]] VkDisplayKHR * pDisplays )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( physicalDevice, "vkGetDisplayPlaneSupportedDisplaysKHR" );
#else
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
#endif
	}

	VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkDisplayKHR display,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkDisplayModePropertiesKHR * pProperties )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( physicalDevice, "vkGetDisplayModePropertiesKHR" );
#else
		auto props = get( display )->getDisplayModeProperties();
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
#endif
	}

	VkResult VKAPI_CALL vkCreateDisplayModeKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkDisplayKHR display,
		[[maybe_unused]] const VkDisplayModeCreateInfoKHR * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkDisplayModeKHR * pMode )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( physicalDevice, "vkCreateDisplayModeKHR" );
#else
		return allocate( *pMode
			, pAllocator
			, display
			, *pCreateInfo );
#endif
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkDisplayModeKHR mode,
		[[maybe_unused]] uint32_t planeIndex,
		[[maybe_unused]] VkDisplayPlaneCapabilitiesKHR * pCapabilities )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( physicalDevice, "vkGetDisplayPlaneCapabilitiesKHR" );
#else
		*pCapabilities = get( mode )->getDisplayPlaneCapabilities();
		return VK_SUCCESS;
#endif
	}

	VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		[[maybe_unused]] const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkSurfaceKHR * pSurface )
	{
#if defined( Ashes_D3D11_XBox )
		return reportUnsupported( instance, "vkCreateDisplayPlaneSurfaceKHR" );
#else
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
#endif
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_physical_device_properties2
#ifdef VK_KHR_get_physical_device_properties2

	void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceFeatures2KHR * pFeatures )
	{
		*pFeatures = get( physicalDevice )->getFeatures2();
	}

	void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties2KHR * pProperties )
	{
		*pProperties = get( physicalDevice )->getProperties2();
	}

	void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkFormat format,
		VkFormatProperties2KHR * pFormatProperties )
	{
		*pFormatProperties = get( physicalDevice )->getFormatProperties2( format );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceImageFormatInfo2KHR * pImageFormatInfo,
		VkImageFormatProperties2KHR * pImageFormatProperties )
	{
		return get( physicalDevice )->getImageFormatProperties2( *pImageFormatInfo
			, *pImageFormatProperties );
	}

	void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pQueueFamilyPropertyCount,
		VkQueueFamilyProperties2KHR * pQueueFamilyProperties )
	{
		auto props = get( physicalDevice )->getQueueFamilyProperties2();
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

	void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR(
		VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceMemoryProperties2KHR * pMemoryProperties )
	{
		*pMemoryProperties = get( physicalDevice )->getMemoryProperties2();
	}

	void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR(
		VkPhysicalDevice physicalDevice,
		const VkPhysicalDeviceSparseImageFormatInfo2KHR * pFormatInfo,
		uint32_t * pPropertyCount,
		VkSparseImageFormatProperties2KHR * pProperties )
	{
		std::vector< VkSparseImageFormatProperties2KHR > props;
		get( physicalDevice )->getSparseImageFormatProperties2( *pFormatInfo, props );
		*pPropertyCount = uint32_t( props.size() );

		if ( pProperties )
		{
			for ( auto & prop : props )
			{
				( *pProperties ) = prop;
				++pProperties;
			}
		}
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR(
		VkDevice device,
		[[maybe_unused]] uint32_t heapIndex,
		[[maybe_unused]] uint32_t localDeviceIndex,
		[[maybe_unused]] uint32_t remoteDeviceIndex,
		[[maybe_unused]] VkPeerMemoryFeatureFlags * pPeerMemoryFeatures )
	{
		reportUnsupported( device, "vkGetDeviceGroupPeerMemoryFeaturesKHR" );
	}

	void VKAPI_CALL vkCmdSetDeviceMaskKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t deviceMask )
	{
		reportUnsupported( commandBuffer, "vkCmdSetDeviceMaskKHR" );
	}

	void VKAPI_CALL vkCmdDispatchBaseKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t baseGroupX,
		[[maybe_unused]] uint32_t baseGroupY,
		[[maybe_unused]] uint32_t baseGroupZ,
		[[maybe_unused]] uint32_t groupCountX,
		[[maybe_unused]] uint32_t groupCountY,
		[[maybe_unused]] uint32_t groupCountZ )
	{
		reportUnsupported( commandBuffer, "vkCmdDispatchBaseKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_maintenance1
#ifdef VK_KHR_maintenance1

	void VKAPI_CALL vkTrimCommandPoolKHR(
		VkDevice device,
		[[maybe_unused]] VkCommandPool commandPool,
		[[maybe_unused]] VkCommandPoolTrimFlagsKHR flags )
	{
		reportUnsupported( device, "vkTrimCommandPoolKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group_creation
#ifdef VK_KHR_device_group_creation

	VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR(
		VkInstance instance,
		[[maybe_unused]] uint32_t * pPhysicalDeviceGroupCount,
		[[maybe_unused]] VkPhysicalDeviceGroupProperties * pPhysicalDeviceGroupProperties )
	{
		return reportUnsupported( instance, "vkEnumeratePhysicalDeviceGroupsKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_capabilities
#ifdef VK_KHR_external_memory_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceExternalBufferInfo * pExternalBufferInfo,
		[[maybe_unused]] VkExternalBufferProperties * pExternalBufferProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalBufferPropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_fd
#ifdef VK_KHR_external_memory_fd

	VkResult VKAPI_CALL vkGetMemoryFdKHR(
		VkDevice device,
		[[maybe_unused]] const VkMemoryGetFdInfoKHR * pGetFdInfo,
		[[maybe_unused]] int * pFd )
	{
		return reportUnsupported( device, "vkGetMemoryFdKHR" );
	}

	VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR(
		VkDevice device,
		[[maybe_unused]] VkExternalMemoryHandleTypeFlagBits handleType,
		[[maybe_unused]] int fd,
		[[maybe_unused]] VkMemoryFdPropertiesKHR * pMemoryFdProperties )
	{
		return reportUnsupported( device, "vkGetMemoryFdPropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_capabilities
#ifdef VK_KHR_external_semaphore_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceExternalSemaphoreInfo * pExternalSemaphoreInfo,
		[[maybe_unused]] VkExternalSemaphoreProperties * pExternalSemaphoreProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_fd
#ifdef VK_KHR_external_semaphore_fd

	VkResult VKAPI_CALL vkImportSemaphoreFdKHR(
		VkDevice device,
		[[maybe_unused]] const VkImportSemaphoreFdInfoKHR * pImportSemaphoreFdInfo )
	{
		return reportUnsupported( device, "vkImportSemaphoreFdKHR" );
	}

	VkResult VKAPI_CALL vkGetSemaphoreFdKHR(
		VkDevice device,
		[[maybe_unused]] const VkSemaphoreGetFdInfoKHR * pGetFdInfo,
		[[maybe_unused]] int * pFd )
	{
		return reportUnsupported( device, "vkGetSemaphoreFdKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_push_descriptor
#ifdef VK_KHR_push_descriptor

	void VKAPI_CALL vkCmdPushDescriptorSetKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkPipelineBindPoint pipelineBindPoint,
		[[maybe_unused]] VkPipelineLayout layout,
		[[maybe_unused]] uint32_t set,
		[[maybe_unused]] uint32_t descriptorWriteCount,
		[[maybe_unused]] const VkWriteDescriptorSet * pDescriptorWrites )
	{
		reportUnsupported( commandBuffer, "vkCmdPushDescriptorSetKHR" );
	}

	void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		[[maybe_unused]] VkPipelineLayout layout,
		[[maybe_unused]] uint32_t set,
		[[maybe_unused]] const void * pData )
	{
		reportUnsupported( commandBuffer, "vkCmdPushDescriptorSetWithTemplateKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_descriptor_update_template
#ifdef VK_KHR_descriptor_update_template

	VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR(
		VkDevice device,
		[[maybe_unused]] const VkDescriptorUpdateTemplateCreateInfo * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkDescriptorUpdateTemplate * pDescriptorUpdateTemplate )
	{
		return reportUnsupported( device, "vkCreateDescriptorUpdateTemplateKHR" );
	}

	void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR(
		VkDevice device,
		[[maybe_unused]] VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyDescriptorUpdateTemplateKHR" );
	}

	void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR(
		VkDevice device,
		[[maybe_unused]] VkDescriptorSet descriptorSet,
		[[maybe_unused]] VkDescriptorUpdateTemplate descriptorUpdateTemplate,
		[[maybe_unused]] const void * pData )
	{
		reportUnsupported( device, "vkUpdateDescriptorSetWithTemplateKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_create_renderpass2
#ifdef VK_KHR_create_renderpass2

	VkResult VKAPI_CALL vkCreateRenderPass2KHR(
		VkDevice device,
		[[maybe_unused]] const VkRenderPassCreateInfo2KHR * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkRenderPass * pRenderPass )
	{
		return reportUnsupported( device, "vkCreateRenderPass2KHR" );
	}

	void VKAPI_CALL vkCmdBeginRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkRenderPassBeginInfo * pRenderPassBegin,
		[[maybe_unused]] const VkSubpassBeginInfoKHR * pSubpassBeginInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginRenderPass2KHR" );
	}

	void VKAPI_CALL vkCmdNextSubpass2KHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkSubpassBeginInfoKHR * pSubpassBeginInfo,
		[[maybe_unused]] const VkSubpassEndInfoKHR * pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdNextSubpass2KHR" );
	}

	void VKAPI_CALL vkCmdEndRenderPass2KHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkSubpassEndInfoKHR * pSubpassEndInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdEndRenderPass2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_shared_presentable_image
#ifdef VK_KHR_shared_presentable_image

	VkResult VKAPI_CALL vkGetSwapchainStatusKHR(
		VkDevice device,
		[[maybe_unused]] VkSwapchainKHR swapchain )
	{
		return reportUnsupported( device, "vkImportFenceFdKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_capabilities
#ifdef VK_KHR_external_fence_capabilities

	void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceExternalFenceInfo * pExternalFenceInfo,
		[[maybe_unused]] VkExternalFenceProperties * pExternalFenceProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalFencePropertiesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_fd
#ifdef VK_KHR_external_fence_fd

	VkResult VKAPI_CALL vkImportFenceFdKHR(
		VkDevice device,
		[[maybe_unused]] const VkImportFenceFdInfoKHR * pImportFenceFdInfo )
	{
		return reportUnsupported( device, "vkImportFenceFdKHR" );
	}

	VkResult VKAPI_CALL vkGetFenceFdKHR(
		VkDevice device,
		[[maybe_unused]] const VkFenceGetFdInfoKHR * pGetFdInfo,
		[[maybe_unused]] int * pFd )
	{
		return reportUnsupported( device, "vkGetFenceFdKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_surface_capabilities2
#ifdef VK_KHR_get_surface_capabilities2

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
		[[maybe_unused]] VkSurfaceCapabilities2KHR * pSurfaceCapabilities )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSurfaceCapabilities2KHR" );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkPhysicalDeviceSurfaceInfo2KHR * pSurfaceInfo,
		[[maybe_unused]] uint32_t * pSurfaceFormatCount,
		[[maybe_unused]] VkSurfaceFormat2KHR * pSurfaceFormats )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSurfaceFormats2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_display_properties2
#ifdef VK_KHR_get_display_properties2

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkDisplayProperties2KHR * pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceDisplayProperties2KHR" );
	}

	VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkDisplayPlaneProperties2KHR * pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR" );
	}

	VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkDisplayKHR display,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkDisplayModeProperties2KHR * pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetDisplayModeProperties2KHR" );
	}

	VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] const VkDisplayPlaneInfo2KHR * pDisplayPlaneInfo,
		[[maybe_unused]] VkDisplayPlaneCapabilities2KHR * pCapabilities )
	{
		return reportUnsupported( physicalDevice, "vkGetDisplayPlaneCapabilities2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_memory_requirements2
#ifdef VK_KHR_get_memory_requirements2

	void VKAPI_CALL vkGetImageMemoryRequirements2KHR(
		VkDevice device,
		[[maybe_unused]] const VkImageMemoryRequirementsInfo2 * pInfo,
		[[maybe_unused]] VkMemoryRequirements2 * pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageMemoryRequirements2KHR" );
	}

	void VKAPI_CALL vkGetBufferMemoryRequirements2KHR(
		VkDevice device,
		[[maybe_unused]] const VkBufferMemoryRequirementsInfo2 * pInfo,
		[[maybe_unused]] VkMemoryRequirements2 * pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetBufferMemoryRequirements2KHR" );
	}

	void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR(
		VkDevice device,
		[[maybe_unused]] const VkImageSparseMemoryRequirementsInfo2 * pInfo,
		[[maybe_unused]] uint32_t * pSparseMemoryRequirementCount,
		[[maybe_unused]] VkSparseImageMemoryRequirements2 * pSparseMemoryRequirements )
	{
		reportUnsupported( device, "vkGetImageSparseMemoryRequirements2KHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_sampler_ycbcr_conversion
#ifdef VK_KHR_sampler_ycbcr_conversion

	VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR(
		VkDevice device,
		[[maybe_unused]] const VkSamplerYcbcrConversionCreateInfo * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkSamplerYcbcrConversion * pYcbcrConversion )
	{
		return reportUnsupported( device, "vkCreateSamplerYcbcrConversionKHR" );
	}

	void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR(
		VkDevice device,
		[[maybe_unused]] VkSamplerYcbcrConversion ycbcrConversion,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroySamplerYcbcrConversionKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_bind_memory2
#ifdef VK_KHR_bind_memory2

	VkResult VKAPI_CALL vkBindBufferMemory2KHR(
		[[maybe_unused]] VkDevice device,
		uint32_t bindInfoCount,
		const VkBindBufferMemoryInfo * pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			if ( result == VK_SUCCESS )
			{
				result = get( bindInfo.buffer )->bindMemory( bindInfo.memory
					, bindInfo.memoryOffset );
			}
		}

		return result;
	}

	VkResult VKAPI_CALL vkBindImageMemory2KHR(
		[[maybe_unused]] VkDevice device,
		uint32_t bindInfoCount,
		const VkBindImageMemoryInfo * pBindInfos )
	{
		VkResult result = VK_SUCCESS;

		for ( auto & bindInfo : makeArrayView( pBindInfos, bindInfoCount ) )
		{
			if ( result == VK_SUCCESS )
			{
				result = get( bindInfo.image )->bindMemory( bindInfo.memory
					, bindInfo.memoryOffset );
			}
		}

		return result;
	}

#endif
#pragma endregion
#pragma region VK_KHR_maintenance3
#ifdef VK_KHR_maintenance3

	void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR(
		VkDevice device,
		[[maybe_unused]] const VkDescriptorSetLayoutCreateInfo * pCreateInfo,
		[[maybe_unused]] VkDescriptorSetLayoutSupport * pSupport )
	{
		reportUnsupported( device, "vkGetDescriptorSetLayoutSupportKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_draw_indirect_count
#ifdef VK_KHR_draw_indirect_count

	void VKAPI_CALL vkCmdDrawIndirectCountKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectCountKHR" );
	}

	void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndexedIndirectCountKHR" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_report
#ifdef VK_EXT_debug_report

	VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDebugReportCallbackEXT * pCallback )
	{
		assert( pCallback );
		return allocate( *pCallback
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
		[[maybe_unused]] VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks * pAllocator )
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
		const char * pLayerPrefix,
		const char * pMessage )
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
		const VkDebugMarkerObjectTagInfoEXT * pTagInfo )
	{
		return get( device )->debugMarkerSetObjectTag( *pTagInfo );
	}

	VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT(
		VkDevice device,
		const VkDebugMarkerObjectNameInfoEXT * pNameInfo )
	{
		return get( device )->debugMarkerSetObjectName( *pNameInfo );
	}

	void VKAPI_CALL vkCmdDebugMarkerBeginEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugMarkerMarkerInfoEXT * pMarkerInfo )
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
		const VkDebugMarkerMarkerInfoEXT * pMarkerInfo )
	{
		return get( commandBuffer )->debugMarkerInsert( *pMarkerInfo );
	}

#endif
#pragma endregion
#pragma region VK_EXT_transform_feedback
#ifdef VK_EXT_transform_feedback

	void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstBinding,
		[[maybe_unused]] uint32_t bindingCount,
		[[maybe_unused]] const VkBuffer * pBuffers,
		[[maybe_unused]] const VkDeviceSize * pOffsets,
		[[maybe_unused]] const VkDeviceSize * pSizes )
	{
		reportUnsupported( commandBuffer, "vkCmdBindTransformFeedbackBuffersEXT" );
	}

	void VKAPI_CALL vkCmdBeginTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstCounterBuffer,
		[[maybe_unused]] uint32_t counterBufferCount,
		[[maybe_unused]] const VkBuffer * pCounterBuffers,
		[[maybe_unused]] const VkDeviceSize * pCounterBufferOffsets )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginTransformFeedbackEXT" );
	}

	void VKAPI_CALL vkCmdEndTransformFeedbackEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstCounterBuffer,
		[[maybe_unused]] uint32_t counterBufferCount,
		[[maybe_unused]] const VkBuffer * pCounterBuffers,
		[[maybe_unused]] const VkDeviceSize * pCounterBufferOffsets )
	{
		reportUnsupported( commandBuffer, "vkCmdEndTransformFeedbackEXT" );
	}

	void VKAPI_CALL vkCmdBeginQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkQueryPool queryPool,
		[[maybe_unused]] uint32_t query,
		[[maybe_unused]] VkQueryControlFlags flags,
		[[maybe_unused]] uint32_t index )
	{
		reportUnsupported( commandBuffer, "vkCmdBeginQueryIndexedEXT" );
	}

	void VKAPI_CALL vkCmdEndQueryIndexedEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkQueryPool queryPool,
		[[maybe_unused]] uint32_t query,
		[[maybe_unused]] uint32_t index )
	{
		reportUnsupported( commandBuffer, "vkCmdEndQueryIndexedEXT" );
	}

	void VKAPI_CALL vkCmdDrawIndirectByteCountEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t instanceCount,
		[[maybe_unused]] uint32_t firstInstance,
		[[maybe_unused]] VkBuffer counterBuffer,
		[[maybe_unused]] VkDeviceSize counterBufferOffset,
		[[maybe_unused]] uint32_t counterOffset,
		[[maybe_unused]] uint32_t vertexStride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectByteCountEXT" );
	}

#endif
#pragma endregion
#pragma region VK_AMD_draw_indirect_count
#ifdef VK_AMD_draw_indirect_count

	void VKAPI_CALL vkCmdDrawIndirectCountAMD(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndirectCountAMD" );
	}

	void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawIndexedIndirectCountAMD" );
	}

#endif
#pragma endregion
#pragma region VK_AMD_shader_info
#ifdef VK_AMD_shader_info

	VkResult VKAPI_CALL vkGetShaderInfoAMD(
		VkDevice device,
		[[maybe_unused]] VkPipeline pipeline,
		[[maybe_unused]] VkShaderStageFlagBits shaderStage,
		[[maybe_unused]] VkShaderInfoTypeAMD infoType,
		[[maybe_unused]] size_t * pInfoSize,
		[[maybe_unused]] void * pInfo )
	{
		return reportUnsupported( device, "vkGetShaderInfoAMD" );
	}

#endif
#pragma endregion
#pragma region VK_NV_external_memory_capabilities
#ifdef VK_NV_external_memory_capabilities

	VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkFormat format,
		[[maybe_unused]] VkImageType type,
		[[maybe_unused]] VkImageTiling tiling,
		[[maybe_unused]] VkImageUsageFlags usage,
		[[maybe_unused]] VkImageCreateFlags flags,
		[[maybe_unused]] VkExternalMemoryHandleTypeFlagsNV externalHandleType,
		[[maybe_unused]] VkExternalImageFormatPropertiesNV * pExternalImageFormatProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_conditional_rendering
#ifdef VK_EXT_conditional_rendering

	void VKAPI_CALL vkCmdBeginConditionalRenderingEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkConditionalRenderingBeginInfoEXT * pConditionalRenderingBegin )
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
		[[maybe_unused]] const VkCmdProcessCommandsInfoNVX * pProcessCommandsInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdProcessCommandsNVX" );
	}

	void VKAPI_CALL vkCmdReserveSpaceForCommandsNVX(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkCmdReserveSpaceForCommandsInfoNVX * pReserveSpaceInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdReserveSpaceForCommandsNVX" );
	}

	VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNVX(
		VkDevice device,
		[[maybe_unused]] const VkIndirectCommandsLayoutCreateInfoNVX * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkIndirectCommandsLayoutNVX * pIndirectCommandsLayout )
	{
		return reportUnsupported( device, "vkCreateIndirectCommandsLayoutNVX" );
	}

	void VKAPI_CALL vkDestroyIndirectCommandsLayoutNVX(
		VkDevice device,
		[[maybe_unused]] VkIndirectCommandsLayoutNVX indirectCommandsLayout,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyIndirectCommandsLayoutNVX" );
	}

	VkResult VKAPI_CALL vkCreateObjectTableNVX(
		VkDevice device,
		[[maybe_unused]] const VkObjectTableCreateInfoNVX * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkObjectTableNVX * pObjectTable )
	{
		return reportUnsupported( device, "vkCreateObjectTableNVX" );
	}

	void VKAPI_CALL vkDestroyObjectTableNVX(
		VkDevice device,
		[[maybe_unused]] VkObjectTableNVX objectTable,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyObjectTableNVX" );
	}

	VkResult VKAPI_CALL vkRegisterObjectsNVX(
		VkDevice device,
		[[maybe_unused]] VkObjectTableNVX objectTable,
		[[maybe_unused]] uint32_t objectCount,
		[[maybe_unused]] const VkObjectTableEntryNVX * const * ppObjectTableEntries,
		[[maybe_unused]] const uint32_t * pObjectIndices )
	{
		return reportUnsupported( device, "vkRegisterObjectsNVX" );
	}

	VkResult VKAPI_CALL vkUnregisterObjectsNVX(
		VkDevice device,
		[[maybe_unused]] VkObjectTableNVX objectTable,
		[[maybe_unused]] uint32_t objectCount,
		[[maybe_unused]] const VkObjectEntryTypeNVX * pObjectEntryTypes,
		[[maybe_unused]] const uint32_t * pObjectIndices )
	{
		return reportUnsupported( device, "vkUnregisterObjectsNVX" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkDeviceGeneratedCommandsFeaturesNVX * pFeatures,
		[[maybe_unused]] VkDeviceGeneratedCommandsLimitsNVX * pLimits )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX" );
	}

#endif
#pragma endregion
#pragma region VK_NV_clip_space_w_scaling
#ifdef VK_NV_clip_space_w_scaling

	void VKAPI_CALL vkCmdSetViewportWScalingNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstViewport,
		[[maybe_unused]] uint32_t viewportCount,
		[[maybe_unused]] const VkViewportWScalingNV * pViewportWScalings )
	{
		reportUnsupported( commandBuffer, "vkCmdSetViewportWScalingNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_direct_mode_display
#ifdef VK_EXT_direct_mode_display

	VkResult VKAPI_CALL vkReleaseDisplayEXT(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkDisplayKHR display )
	{
		return reportUnsupported( physicalDevice, "vkReleaseDisplayEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_display_surface_counter
#ifdef VK_EXT_display_surface_counter

	VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] VkSurfaceKHR surface,
		[[maybe_unused]] VkSurfaceCapabilities2EXT * pSurfaceCapabilities )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceSurfaceCapabilities2EXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_display_control
#ifdef VK_EXT_display_control

	VkResult VKAPI_CALL vkDisplayPowerControlEXT(
		VkDevice device,
		[[maybe_unused]] VkDisplayKHR display,
		[[maybe_unused]] const VkDisplayPowerInfoEXT * pDisplayPowerInfo )
	{
		return reportUnsupported( device, "vkDisplayPowerControlEXT" );
	}

	VkResult VKAPI_CALL vkRegisterDeviceEventEXT(
		VkDevice device,
		[[maybe_unused]] const VkDeviceEventInfoEXT * pDeviceEventInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkFence * pFence )
	{
		return reportUnsupported( device, "vkRegisterDeviceEventEXT" );
	}

	VkResult VKAPI_CALL vkRegisterDisplayEventEXT(
		VkDevice device,
		[[maybe_unused]] VkDisplayKHR display,
		[[maybe_unused]] const VkDisplayEventInfoEXT * pDisplayEventInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkFence * pFence )
	{
		return reportUnsupported( device, "vkRegisterDisplayEventEXT" );
	}

	VkResult VKAPI_CALL vkGetSwapchainCounterEXT(
		VkDevice device,
		[[maybe_unused]] VkSwapchainKHR swapchain,
		[[maybe_unused]] VkSurfaceCounterFlagBitsEXT counter,
		[[maybe_unused]] uint64_t * pCounterValue )
	{
		return reportUnsupported( device, "vkGetSwapchainCounterEXT" );
	}

#endif
#pragma endregion
#pragma region VK_GOOGLE_display_timing
#ifdef VK_GOOGLE_display_timing

	VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE(
		VkDevice device,
		[[maybe_unused]] VkSwapchainKHR swapchain,
		[[maybe_unused]] VkRefreshCycleDurationGOOGLE * pDisplayTimingProperties )
	{
		return reportUnsupported( device, "vkGetRefreshCycleDurationGOOGLE" );
	}

	VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE(
		VkDevice device,
		[[maybe_unused]] VkSwapchainKHR swapchain,
		[[maybe_unused]] uint32_t * pPresentationTimingCount,
		[[maybe_unused]] VkPastPresentationTimingGOOGLE * pPresentationTimings )
	{
		return reportUnsupported( device, "vkGetPastPresentationTimingGOOGLE" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_discard_rectangles
#ifdef VK_EXT_discard_rectangles

	void VKAPI_CALL vkCmdSetDiscardRectangleEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstDiscardRectangle,
		[[maybe_unused]] uint32_t discardRectangleCount,
		[[maybe_unused]] const VkRect2D * pDiscardRectangles )
	{
		reportUnsupported( commandBuffer, "vkCmdSetDiscardRectangleEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_hdr_metadata
#ifdef VK_EXT_hdr_metadata

	void VKAPI_CALL vkSetHdrMetadataEXT(
		VkDevice device,
		[[maybe_unused]] uint32_t swapchainCount,
		[[maybe_unused]] const VkSwapchainKHR * pSwapchains,
		[[maybe_unused]] const VkHdrMetadataEXT * pMetadata )
	{
		reportUnsupported( device, "vkSetHdrMetadataEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_utils
#ifdef VK_EXT_debug_utils

	VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT(
		VkDevice device,
		const VkDebugUtilsObjectNameInfoEXT * pNameInfo )
	{
		return get( device )->setDebugUtilsObjectName( *pNameInfo );
	}

	VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT(
		VkDevice device,
		const VkDebugUtilsObjectTagInfoEXT * pTagInfo )
	{
		return get( device )->setDebugUtilsObjectTag( *pTagInfo );
	}

	void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT(
		VkQueue queue,
		const VkDebugUtilsLabelEXT * pLabelInfo )
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
		const VkDebugUtilsLabelEXT * pLabelInfo )
	{
		return get( queue )->insertDebugUtilsLabel( *pLabelInfo );
	}

	void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(
		VkCommandBuffer commandBuffer,
		const VkDebugUtilsLabelEXT * pLabelInfo )
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
		const VkDebugUtilsLabelEXT * pLabelInfo )
	{
		return get( commandBuffer )->insertDebugUtilsLabel( *pLabelInfo );
	}

	VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDebugUtilsMessengerEXT * pMessenger )
	{
		assert( pMessenger );
		return allocate( *pMessenger
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
		[[maybe_unused]] VkInstance instance,
		VkDebugUtilsMessengerEXT messenger,
		const VkAllocationCallbacks * pAllocator )
	{
		deallocate( messenger, pAllocator );
	}

	void VKAPI_CALL vkSubmitDebugUtilsMessageEXT(
		VkInstance instance,
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData )
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
		[[maybe_unused]] const VkSampleLocationsInfoEXT * pSampleLocationsInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdSetSampleLocationsEXT" );
	}

	void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT(
		VkPhysicalDevice physicalDevice,
		VkSampleCountFlagBits samples,
		[[maybe_unused]] VkMultisamplePropertiesEXT * pMultisampleProperties )
	{
		reportUnsupported( physicalDevice, "vkGetPhysicalDeviceMultisamplePropertiesEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_image_drm_format_modifier
#ifdef VK_EXT_image_drm_format_modifier

	VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT(
		VkDevice device,
		[[maybe_unused]] VkImage image,
		[[maybe_unused]] VkImageDrmFormatModifierPropertiesEXT * pProperties )
	{
		return reportUnsupported( device, "vkGetImageDrmFormatModifierPropertiesEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_validation_cache
#ifdef VK_EXT_validation_cache

	VkResult VKAPI_CALL vkCreateValidationCacheEXT(
		VkDevice device,
		[[maybe_unused]] const VkValidationCacheCreateInfoEXT * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkValidationCacheEXT * pValidationCache )
	{
		return reportUnsupported( device, "vkCreateValidationCacheEXT" );
	}

	void VKAPI_CALL vkDestroyValidationCacheEXT(
		VkDevice device,
		[[maybe_unused]] VkValidationCacheEXT validationCache,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyValidationCacheEXT" );
	}

	VkResult VKAPI_CALL vkMergeValidationCachesEXT(
		VkDevice device,
		[[maybe_unused]] VkValidationCacheEXT dstCache,
		[[maybe_unused]] uint32_t srcCacheCount,
		[[maybe_unused]] const VkValidationCacheEXT * pSrcCaches )
	{
		return reportUnsupported( device, "vkMergeValidationCachesEXT" );
	}

	VkResult VKAPI_CALL vkGetValidationCacheDataEXT(
		VkDevice device,
		[[maybe_unused]] VkValidationCacheEXT validationCache,
		[[maybe_unused]] size_t * pDataSize,
		[[maybe_unused]] void * pData )
	{
		return reportUnsupported( device, "vkGetValidationCacheDataEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NV_shading_rate_image
#ifdef VK_NV_shading_rate_image

	void VKAPI_CALL vkCmdBindShadingRateImageNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkImageView imageView,
		[[maybe_unused]] VkImageLayout imageLayout )
	{
		reportUnsupported( commandBuffer, "vkCmdBindShadingRateImageNV" );
	}

	void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstViewport,
		[[maybe_unused]] uint32_t viewportCount,
		[[maybe_unused]] const VkShadingRatePaletteNV * pShadingRatePalettes )
	{
		reportUnsupported( commandBuffer, "vkCmdSetViewportShadingRatePaletteNV" );
	}

	void VKAPI_CALL vkCmdSetCoarseSampleOrderNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkCoarseSampleOrderTypeNV sampleOrderType,
		[[maybe_unused]] uint32_t customSampleOrderCount,
		[[maybe_unused]] const VkCoarseSampleOrderCustomNV * pCustomSampleOrders )
	{
		reportUnsupported( commandBuffer, "vkCmdSetCoarseSampleOrderNV" );
	}

#endif
#pragma endregion
#pragma region VK_NV_ray_tracing
#ifdef VK_NV_ray_tracing

	VkResult VKAPI_CALL vkCreateAccelerationStructureNV(
		VkDevice device,
		[[maybe_unused]] const VkAccelerationStructureCreateInfoNV * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkAccelerationStructureNV * pAccelerationStructure )
	{
		return reportUnsupported( device, "vkCreateAccelerationStructureNV" );
	}

	void VKAPI_CALL vkDestroyAccelerationStructureNV(
		VkDevice device,
		[[maybe_unused]] VkAccelerationStructureNV accelerationStructure,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyAccelerationStructureNV" );
	}

	void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV(
		VkDevice device,
		[[maybe_unused]] const VkAccelerationStructureMemoryRequirementsInfoNV * pInfo,
		[[maybe_unused]] VkMemoryRequirements2KHR * pMemoryRequirements )
	{
		reportUnsupported( device, "vkGetAccelerationStructureMemoryRequirementsNV" );
	}

	VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV(
		VkDevice device,
		[[maybe_unused]] uint32_t bindInfoCount,
		[[maybe_unused]] const VkBindAccelerationStructureMemoryInfoNV * pBindInfos )
	{
		return reportUnsupported( device, "vkBindAccelerationStructureMemoryNV" );
	}

	void VKAPI_CALL vkCmdBuildAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkAccelerationStructureInfoNV * pInfo,
		[[maybe_unused]] VkBuffer instanceData,
		[[maybe_unused]] VkDeviceSize instanceOffset,
		[[maybe_unused]] VkBool32 update,
		[[maybe_unused]] VkAccelerationStructureNV dst,
		[[maybe_unused]] VkAccelerationStructureNV src,
		[[maybe_unused]] VkBuffer scratch,
		[[maybe_unused]] VkDeviceSize scratchOffset )
	{
		reportUnsupported( commandBuffer, "vkCmdBuildAccelerationStructureNV" );
	}

	void VKAPI_CALL vkCmdCopyAccelerationStructureNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkAccelerationStructureNV dst,
		[[maybe_unused]] VkAccelerationStructureNV src,
		[[maybe_unused]] VkCopyAccelerationStructureModeNV mode )
	{
		reportUnsupported( commandBuffer, "vkCmdCopyAccelerationStructureNV" );
	}

	void VKAPI_CALL vkCmdTraceRaysNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer raygenShaderBindingTableBuffer,
		[[maybe_unused]] VkDeviceSize raygenShaderBindingOffset,
		[[maybe_unused]] VkBuffer missShaderBindingTableBuffer,
		[[maybe_unused]] VkDeviceSize missShaderBindingOffset,
		[[maybe_unused]] VkDeviceSize missShaderBindingStride,
		[[maybe_unused]] VkBuffer hitShaderBindingTableBuffer,
		[[maybe_unused]] VkDeviceSize hitShaderBindingOffset,
		[[maybe_unused]] VkDeviceSize hitShaderBindingStride,
		[[maybe_unused]] VkBuffer callableShaderBindingTableBuffer,
		[[maybe_unused]] VkDeviceSize callableShaderBindingOffset,
		[[maybe_unused]] VkDeviceSize callableShaderBindingStride,
		[[maybe_unused]] uint32_t width,
		[[maybe_unused]] uint32_t height,
		[[maybe_unused]] uint32_t depth )
	{
		reportUnsupported( commandBuffer, "vkCmdTraceRaysNV" );
	}

	VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV(
		VkDevice device,
		[[maybe_unused]] VkPipelineCache pipelineCache,
		[[maybe_unused]] uint32_t createInfoCount,
		[[maybe_unused]] const VkRayTracingPipelineCreateInfoNV * pCreateInfos,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkPipeline * pPipelines )
	{
		return reportUnsupported( device, "vkCreateRayTracingPipelinesNV" );
	}

	VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV(
		VkDevice device,
		[[maybe_unused]] VkPipeline pipeline,
		[[maybe_unused]] uint32_t firstGroup,
		[[maybe_unused]] uint32_t groupCount,
		[[maybe_unused]] size_t dataSize,
		[[maybe_unused]] void * pData )
	{
		return reportUnsupported( device, "vkGetRayTracingShaderGroupHandlesNV" );
	}

	VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV(
		VkDevice device,
		[[maybe_unused]] VkAccelerationStructureNV accelerationStructure,
		[[maybe_unused]] size_t dataSize,
		[[maybe_unused]] void * pData )
	{
		return reportUnsupported( device, "vkGetAccelerationStructureHandleNV" );
	}

	void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t accelerationStructureCount,
		[[maybe_unused]] const VkAccelerationStructureNV * pAccelerationStructures,
		[[maybe_unused]] VkQueryType queryType,
		[[maybe_unused]] VkQueryPool queryPool,
		[[maybe_unused]] uint32_t firstQuery )
	{
		reportUnsupported( commandBuffer, "vkCmdWriteAccelerationStructuresPropertiesNV" );
	}

	VkResult VKAPI_CALL vkCompileDeferredNV(
		VkDevice device,
		[[maybe_unused]] VkPipeline pipeline,
		[[maybe_unused]] uint32_t shader )
	{
		return reportUnsupported( device, "vkCompileDeferredNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_external_memory_host
#ifdef VK_EXT_external_memory_host

	VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT(
		VkDevice device,
		[[maybe_unused]] VkExternalMemoryHandleTypeFlagBits handleType,
		[[maybe_unused]] const void * pHostPointer,
		[[maybe_unused]] VkMemoryHostPointerPropertiesEXT * pMemoryHostPointerProperties )
	{
		return reportUnsupported( device, "vkGetMemoryHostPointerPropertiesEXT" );
	}

#endif
#pragma endregion
#pragma region VK_AMD_buffer_marker
#ifdef VK_AMD_buffer_marker

	void VKAPI_CALL vkCmdWriteBufferMarkerAMD(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkPipelineStageFlagBits pipelineStage,
		[[maybe_unused]] VkBuffer dstBuffer,
		[[maybe_unused]] VkDeviceSize dstOffset,
		[[maybe_unused]] uint32_t marker )
	{
		reportUnsupported( commandBuffer, "vkCmdWriteBufferMarkerAMD" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_calibrated_timestamps
#ifdef VK_EXT_calibrated_timestamps

	VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t * pTimeDomainCount,
		[[maybe_unused]] VkTimeDomainEXT * pTimeDomains )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT" );
	}

	VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT(
		VkDevice device,
		[[maybe_unused]] uint32_t timestampCount,
		[[maybe_unused]] const VkCalibratedTimestampInfoEXT * pTimestampInfos,
		[[maybe_unused]] uint64_t * pTimestamps,
		[[maybe_unused]] uint64_t * pMaxDeviation )
	{
		return reportUnsupported( device, "vkGetCalibratedTimestampsEXT" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_mesh_shader
#ifdef VK_EXT_mesh_shader

	void VKAPI_CALL vkCmdDrawMeshTasksEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t groupCountX,
		[[maybe_unused]] uint32_t groupCountY,
		[[maybe_unused]] uint32_t groupCountZ )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksEXT" );
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] uint32_t drawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksIndirectEXT" );
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountEXT(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksIndirectCountEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NV_mesh_shader
#ifdef VK_NV_mesh_shader

	void VKAPI_CALL vkCmdDrawMeshTasksNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t taskCount,
		[[maybe_unused]] uint32_t firstTask )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksNV" );
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] uint32_t drawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksIndirectNV" );
	}

	void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] VkBuffer buffer,
		[[maybe_unused]] VkDeviceSize offset,
		[[maybe_unused]] VkBuffer countBuffer,
		[[maybe_unused]] VkDeviceSize countBufferOffset,
		[[maybe_unused]] uint32_t maxDrawCount,
		[[maybe_unused]] uint32_t stride )
	{
		reportUnsupported( commandBuffer, "vkCmdDrawMeshTasksIndirectCountNV" );
	}

#endif
#pragma endregion
#pragma region VK_NV_scissor_exclusive
#ifdef VK_NV_scissor_exclusive

	void VKAPI_CALL vkCmdSetExclusiveScissorNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t firstExclusiveScissor,
		[[maybe_unused]] uint32_t exclusiveScissorCount,
		[[maybe_unused]] const VkRect2D * pExclusiveScissors )
	{
		reportUnsupported( commandBuffer, "vkCmdSetExclusiveScissorNV" );
	}

#endif
#pragma endregion
#pragma region VK_NV_device_diagnostic_checkpoints
#ifdef VK_NV_device_diagnostic_checkpoints

	void VKAPI_CALL vkCmdSetCheckpointNV(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const void * pCheckpointMarker )
	{
		reportUnsupported( commandBuffer, "vkCmdSetCheckpointNV" );
	}

	void VKAPI_CALL vkGetQueueCheckpointDataNV(
		VkQueue queue,
		[[maybe_unused]] uint32_t * pCheckpointDataCount,
		[[maybe_unused]] VkCheckpointDataNV * pCheckpointData )
	{
		reportUnsupported( queue, "vkGetQueueCheckpointDataNV" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_buffer_device_address
#ifdef VK_EXT_buffer_device_address

	VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT(
		VkDevice device,
		[[maybe_unused]] const VkBufferDeviceAddressInfoEXT * pInfo )
	{
		return reportUnsupported( device, "vkGetBufferDeviceAddressEXT" );
	}

#endif
#pragma endregion
#pragma region VK_NV_cooperative_matrix
#ifdef VK_NV_cooperative_matrix

	VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t * pPropertyCount,
		[[maybe_unused]] VkCooperativeMatrixPropertiesNV * pProperties )
	{
		return reportUnsupported( physicalDevice, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_deferred_host_operations
#ifdef VK_KHR_deferred_host_operations

	VkResult VKAPI_CALL vkCreateDeferredOperationKHR(
		VkDevice device,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkDeferredOperationKHR * pDeferredOperation )
	{
		return reportUnsupported( device, "vkCreateDeferredOperationKHR" );
	}

	void VKAPI_CALL vkDestroyDeferredOperationKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR operation,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyDeferredOperationKHR" );
	}

	uint32_t VKAPI_CALL vkGetDeferredOperationMaxConcurrencyKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR operation )
	{
		reportUnsupported( device, "vkGetDeferredOperationMaxConcurrencyKHR" );
		return 0u;
	}

	VkResult VKAPI_CALL vkGetDeferredOperationResultKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR operation )
	{
		return reportUnsupported( device, "vkGetDeferredOperationResultKHR" );
	}

	VkResult VKAPI_CALL vkDeferredOperationJoinKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR operation )
	{
		return reportUnsupported( device, "vkDeferredOperationJoinKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_ray_tracing_pipeline
#ifdef VK_KHR_ray_tracing_pipeline

	void VKAPI_CALL vkCmdTraceRaysKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pRaygenShaderBindingTable,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pMissShaderBindingTable,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pHitShaderBindingTable,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pCallableShaderBindingTable,
		[[maybe_unused]] uint32_t width,
		[[maybe_unused]] uint32_t height,
		[[maybe_unused]] uint32_t depth )
	{
		reportUnsupported( commandBuffer, "vkCmdTraceRaysKHR" );
	}

	VkResult VKAPI_CALL vkCreateRayTracingPipelinesKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR deferredOperation,
		[[maybe_unused]] VkPipelineCache pipelineCache,
		[[maybe_unused]] uint32_t createInfoCount,
		[[maybe_unused]] const VkRayTracingPipelineCreateInfoKHR * pCreateInfos,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkPipeline * pPipelines )
	{
		return reportUnsupported( device, "vkCreateRayTracingPipelinesKHR" );
	}

	VkResult VKAPI_CALL vkGetRayTracingCaptureReplayShaderGroupHandlesKHR(
		VkDevice device,
		[[maybe_unused]] VkPipeline pipeline,
		[[maybe_unused]] uint32_t firstGroup,
		[[maybe_unused]] uint32_t groupCount,
		[[maybe_unused]] size_t dataSize,
		[[maybe_unused]] void * pData )
	{
		return reportUnsupported( device, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR" );
	}

	void VKAPI_CALL vkCmdTraceRaysIndirectKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pRaygenShaderBindingTable,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pMissShaderBindingTable,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pHitShaderBindingTable,
		[[maybe_unused]] const VkStridedDeviceAddressRegionKHR * pCallableShaderBindingTable,
		[[maybe_unused]] VkDeviceAddress indirectDeviceAddress )
	{
		reportUnsupported( commandBuffer, "vkCmdTraceRaysIndirectKHR" );
	}

	VkDeviceSize VKAPI_CALL vkGetRayTracingShaderGroupStackSizeKHR(
		VkDevice device,
		[[maybe_unused]] VkPipeline pipeline,
		[[maybe_unused]] uint32_t group,
		[[maybe_unused]] VkShaderGroupShaderKHR groupShader )
	{
		reportUnsupported( device, "vkGetRayTracingShaderGroupStackSizeKHR" );
		return 0u;
	}

	void VKAPI_CALL vkCmdSetRayTracingPipelineStackSizeKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t pipelineStackSize )
	{
		reportUnsupported( commandBuffer, "vkCmdSetRayTracingPipelineStackSizeKHR" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_acceleration_structure
#ifdef VK_KHR_acceleration_structure

	VkResult VKAPI_CALL vkCreateAccelerationStructureKHR(
		VkDevice device,
		[[maybe_unused]] const VkAccelerationStructureCreateInfoKHR * pCreateInfo,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator,
		[[maybe_unused]] VkAccelerationStructureKHR * pAccelerationStructure )
	{
		return reportUnsupported( device, "vkCreateAccelerationStructureKHR" );
	}

	void VKAPI_CALL vkDestroyAccelerationStructureKHR(
		VkDevice device,
		[[maybe_unused]] VkAccelerationStructureKHR accelerationStructure,
		[[maybe_unused]] const VkAllocationCallbacks * pAllocator )
	{
		reportUnsupported( device, "vkDestroyAccelerationStructureKHR" );
	}

	void VKAPI_CALL vkCmdBuildAccelerationStructuresKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t infoCount,
		[[maybe_unused]] const VkAccelerationStructureBuildGeometryInfoKHR * pInfos,
		[[maybe_unused]] const VkAccelerationStructureBuildRangeInfoKHR * const * ppBuildRangeInfos )
	{
		reportUnsupported( commandBuffer, "vkCmdBuildAccelerationStructuresKHR" );
	}

	void VKAPI_CALL vkCmdBuildAccelerationStructuresIndirectKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t infoCount,
		[[maybe_unused]] const VkAccelerationStructureBuildGeometryInfoKHR * pInfos,
		[[maybe_unused]] const VkDeviceAddress * pIndirectDeviceAddresses,
		[[maybe_unused]] const uint32_t * pIndirectStrides,
		[[maybe_unused]] const uint32_t * const * ppMaxPrimitiveCounts )
	{
		reportUnsupported( commandBuffer, "vkCmdBuildAccelerationStructuresIndirectKHR" );
	}

	VkResult VKAPI_CALL vkBuildAccelerationStructuresKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR deferredOperation,
		[[maybe_unused]] uint32_t infoCount,
		[[maybe_unused]] const VkAccelerationStructureBuildGeometryInfoKHR * pInfos,
		[[maybe_unused]] const VkAccelerationStructureBuildRangeInfoKHR * const * ppBuildRangeInfos )
	{
		return reportUnsupported( device, "vkBuildAccelerationStructuresKHR" );
	}

	VkResult VKAPI_CALL vkCopyAccelerationStructureKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR deferredOperation,
		[[maybe_unused]] const VkCopyAccelerationStructureInfoKHR * pInfo )
	{
		return reportUnsupported( device, "vkCopyAccelerationStructureKHR" );
	}

	VkResult VKAPI_CALL vkCopyAccelerationStructureToMemoryKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR deferredOperation,
		[[maybe_unused]] const VkCopyAccelerationStructureToMemoryInfoKHR * pInfo )
	{
		return reportUnsupported( device, "vkCopyAccelerationStructureToMemoryKHR" );
	}

	VkResult VKAPI_CALL vkCopyMemoryToAccelerationStructureKHR(
		VkDevice device,
		[[maybe_unused]] VkDeferredOperationKHR deferredOperation,
		[[maybe_unused]] const VkCopyMemoryToAccelerationStructureInfoKHR * pInfo )
	{
		return reportUnsupported( device, "vkCopyMemoryToAccelerationStructureKHR" );
	}

	VkResult VKAPI_CALL vkWriteAccelerationStructuresPropertiesKHR(
		VkDevice device,
		[[maybe_unused]] uint32_t accelerationStructureCount,
		[[maybe_unused]] const VkAccelerationStructureKHR * pAccelerationStructures,
		[[maybe_unused]] VkQueryType queryType,
		[[maybe_unused]] size_t dataSize,
		[[maybe_unused]] void * pData,
		[[maybe_unused]] size_t stride )
	{
		return reportUnsupported( device, "vkWriteAccelerationStructuresPropertiesKHR" );
	}

	void VKAPI_CALL vkCmdCopyAccelerationStructureKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkCopyAccelerationStructureInfoKHR * pInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdCopyAccelerationStructureKHR" );
	}

	void VKAPI_CALL vkCmdCopyAccelerationStructureToMemoryKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkCopyAccelerationStructureToMemoryInfoKHR * pInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdCopyAccelerationStructureToMemoryKHR" );
	}

	void VKAPI_CALL vkCmdCopyMemoryToAccelerationStructureKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] const VkCopyMemoryToAccelerationStructureInfoKHR * pInfo )
	{
		reportUnsupported( commandBuffer, "vkCmdCopyMemoryToAccelerationStructureKHR" );
	}

	VkDeviceAddress VKAPI_CALL vkGetAccelerationStructureDeviceAddressKHR(
		VkDevice device,
		[[maybe_unused]] const VkAccelerationStructureDeviceAddressInfoKHR * pInfo )
	{
		reportUnsupported( device, "vkGetAccelerationStructureDeviceAddressKHR" );
		return 0u;
	}

	void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesKHR(
		VkCommandBuffer commandBuffer,
		[[maybe_unused]] uint32_t accelerationStructureCount,
		[[maybe_unused]] const VkAccelerationStructureKHR * pAccelerationStructures,
		[[maybe_unused]] VkQueryType queryType,
		[[maybe_unused]] VkQueryPool queryPool,
		[[maybe_unused]] uint32_t firstQuery )
	{
		reportUnsupported( commandBuffer, "vkCmdWriteAccelerationStructuresPropertiesKHR" );
	}

	void VKAPI_CALL vkGetDeviceAccelerationStructureCompatibilityKHR(
		VkDevice device,
		[[maybe_unused]] const VkAccelerationStructureVersionInfoKHR * pVersionInfo,
		[[maybe_unused]] VkAccelerationStructureCompatibilityKHR * pCompatibility )
	{
		reportUnsupported( device, "vkGetDeviceAccelerationStructureCompatibilityKHR" );
	}

	void VKAPI_CALL vkGetAccelerationStructureBuildSizesKHR(
		VkDevice device,
		[[maybe_unused]] VkAccelerationStructureBuildTypeKHR buildType,
		[[maybe_unused]] const VkAccelerationStructureBuildGeometryInfoKHR * pBuildInfo,
		[[maybe_unused]] const uint32_t * pMaxPrimitiveCounts,
		[[maybe_unused]] VkAccelerationStructureBuildSizesInfoKHR * pSizeInfo )
	{
		reportUnsupported( device, "vkGetAccelerationStructureBuildSizesKHR" );
	}

#endif
#pragma endregion
#pragma region VK_EXT_device_fault
#ifdef VK_EXT_device_fault

	void VKAPI_CALL vkGetDeviceFaultInfoEXT(
		VkDevice device,
		[[maybe_unused]] VkDeviceFaultCountsEXT * pFaultCounts,
		[[maybe_unused]] VkDeviceFaultInfoEXT * pFaultInfo )
	{
		reportUnsupported( device, "vkGetDeviceFaultInfoEXT" );
	}

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef VK_USE_PLATFORM_ANDROID_KHR

	VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR(
		VkInstance instance,
		const VkAndroidSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
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
		const VkImagePipeSurfaceCreateInfoFUCHSIA * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
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
#	ifdef VK_USE_PLATFORM_IOS_MVK

	VkResult VKAPI_CALL vkCreateIOSSurfaceMVK(
		VkInstance instance,
		const VkIOSSurfaceCreateInfoMVK * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
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
#	ifdef VK_USE_PLATFORM_MACOS_MVK

	VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK(
		VkInstance instance,
		const VkMacOSSurfaceCreateInfoMVK * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
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
		const VkViSurfaceCreateInfoNN * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
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
		const VkXcbSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t queueFamilyIndex,
		[[maybe_unused]] xcb_connection_t * connection,
		[[maybe_unused]] xcb_visualid_t visual_id )
	{
		return get( physicalDevice )->getPresentationSupport();
	}

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
		VkSurfaceKHR * pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t queueFamilyIndex,
		[[maybe_unused]] Display * dpy,
		[[maybe_unused]] VisualID visualID )
	{
		return get( physicalDevice )->getPresentationSupport();
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#ifdef VK_KHR_wayland_surface
#	ifdef VK_USE_PLATFORM_WAYLAND_KHR

	VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR(
		VkInstance instance,
		const VkWaylandSurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t queueFamilyIndex,
		[[maybe_unused]] struct wl_display * display )
	{
		return get( physicalDevice )->getPresentationSupport();
	}

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
		VkSurfaceKHR * pSurface )
	{
		assert( pSurface );
		return allocate( *pSurface
			, pAllocator
			, instance
			, *pCreateInfo );
	}

	VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] uint32_t queueFamilyIndex )
	{
		return get( physicalDevice )->getPresentationSupport();
	}

#	endif
#endif
#pragma endregion
#pragma region VK_EXT_acquire_xlib_display
#ifdef VK_EXT_acquire_xlib_display
#	ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

	VkResult VKAPI_CALL vkAcquireXlibDisplayEXT(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] Display * dpy,
		[[maybe_unused]] VkDisplayKHR display )
	{
		return reportUnsupported( physicalDevice, "vkAcquireXlibDisplayEXT" );
	}

	VkResult VKAPI_CALL vkGetRandROutputDisplayEXT(
		VkPhysicalDevice physicalDevice,
		[[maybe_unused]] Display * dpy,
		[[maybe_unused]] RROutput rrOutput,
		[[maybe_unused]] VkDisplayKHR * pDisplay )
	{
		return reportUnsupported( physicalDevice, "vkGetRandROutputDisplayEXT" );
	}

#	endif
#endif
#pragma endregion
}

namespace ashes::D3D11_NAMESPACE
{
	static std::vector< VkExtensionProperties > const SupportedInstanceExtensions
	{
#if VK_KHR_surface
		VkExtensionProperties{ VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_SURFACE_SPEC_VERSION },
#endif
#if VK_KHR_swapchain
		VkExtensionProperties{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SWAPCHAIN_SPEC_VERSION },
#endif
#if VK_KHR_win32_surface
		VkExtensionProperties{ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_SPEC_VERSION },
#endif
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
#if VK_KHR_display && !defined( Ashes_D3D11_XBox )
		VkExtensionProperties{ VK_KHR_DISPLAY_EXTENSION_NAME, VK_KHR_DISPLAY_SPEC_VERSION },
#endif
#if VK_KHR_portability_subset
		VkExtensionProperties{ VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, VK_KHR_PORTABILITY_SUBSET_SPEC_VERSION },
#endif
	};
	static std::vector< VkLayerProperties > const InstanceLayerProperties{};

	std::vector< VkExtensionProperties > const & getSupportedInstanceExtensions( const char * pLayerName )
	{
		return SupportedInstanceExtensions;
	}

	std::vector< VkLayerProperties > const & getInstanceLayerProperties()
	{
		return InstanceLayerProperties;
	}

	struct Library
	{
#if defined( Ashes_D3D11_XBox )
		AshPluginDescription description{ "xbox"
			, "XBox renderer for Ashes"
			, nullptr
			, {}
			, {}
			, ASHPLUGIN_UNDEFINED };
#else
		AshPluginDescription description{ "d3d11"
			, "Direct3D 11 renderer for Ashes"
			, nullptr
			, {}
			, {}
			, ASHPLUGIN_UNDEFINED };
#endif

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
				description.getInstanceProcAddr = &vkGetInstanceProcAddr;
				description.features =
				{
					true, // hasTexBufferRange
					true, // hasImageTexture
					true, // hasBaseInstance
					true, // hasClearTexImage
					true, // hasComputeShaders
					false, // hasStorageBuffers
					true, // supportsPersistentMapping
				};
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_INSTANCE_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_PHYSDEVICE_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_DEVICE_FUNCTION( v, x )\
				description.functions.x = vk##x;
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )
#define VK_LIB_PHYSDEVICE_FUNCTION_EXT( v, n, x )
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )
#include <ashes/ashes_functions_list.hpp>
				result = VK_SUCCESS;

				description.mode = mode;
				description.support.priority = 6u;
				description.support.supported = VkBool32( getSupportedFeatureLevel() >= D3D_FEATURE_LEVEL_11_1
					? VK_TRUE
					: VK_FALSE );
			}

			return result;
		}
	};
}

namespace ashes::D3D11_NAMESPACE
{
	Library & getLibrary()
	{
		thread_local Library library;
		return library;
	}

	bool checkVersion( VkInstance instance
		, uint32_t version )
	{
		return get( instance )->getApiVersion() >= version;
	}
	
	bool checkVersionExt( VkInstance instance
		, uint32_t version
		, std::string_view extension )
	{
		return checkVersion( instance, version )
			&& get( instance )->hasExtension( extension );
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
			&& get( device )->hasExtension( extension );
	}

	using InstanceFunctions = std::map< std::string, PFN_vkVoidFunction, std::less<> >;
	using PhysicalDeviceFunctions = std::map< std::string, PFN_vkVoidFunction, std::less<> >;
	using DeviceFunctions = std::map< std::string, PFN_vkVoidFunction, std::less<> >;

#pragma warning( push )
#pragma warning( disable: 4191 )

	InstanceFunctions const & getFunctions( VkInstance instance )
	{
		static std::map< VkInstance, InstanceFunctions > functions;
		auto [it, res] = functions.try_emplace( instance );

		if ( res )
		{
			if ( instance != nullptr )
			{
				it->second =
				{
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_INSTANCE_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_PHYSDEVICE_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_PHYSDEVICE_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_DEVICE_FUNCTION( v, x )\
					{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )\
					{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#include <ashes/ashes_functions_list.hpp>
				};
			}
			else
			{
				it->second =
				{
#define VK_LIB_GLOBAL_FUNCTION( v, x )\
					{ "vk"#x, PFN_vkVoidFunction( vk##x ) },
#define VK_LIB_INSTANCE_FUNCTION( v, x )
#define VK_LIB_PHYSDEVICE_FUNCTION( v, x )
#define VK_LIB_DEVICE_FUNCTION( v, x )
#define VK_LIB_GLOBAL_FUNCTION_EXT( v, n, x )
#define VK_LIB_INSTANCE_FUNCTION_EXT( v, n, x )
#define VK_LIB_PHYSDEVICE_FUNCTION_EXT( v, n, x )
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )
#include <ashes/ashes_functions_list.hpp>
				};
			}
		}

		return it->second;
	}

	PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(
		VkInstance instance,
		const char* pName )
	{
		PFN_vkVoidFunction result{ nullptr };
		auto & functions = getFunctions( instance );

		if ( auto it = functions.find( pName );
			it != functions.end() )
		{
			result = it->second;
		}

		return result;
	}

	PFN_vkVoidFunction VKAPI_CALL vkGetPhysicalDeviceProcAddr(
		VkInstance instance,
		const char* pName )
	{
		PFN_vkVoidFunction result{ nullptr };
		static PhysicalDeviceFunctions functions
		{
#define VK_LIB_PHYSDEVICE_FUNCTION( v, x )\
			{ "vk"#x, checkVersion( instance, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_PHYSDEVICE_FUNCTION_EXT( v, n, x )\
			{ "vk"#x, checkVersionExt( instance, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#include <ashes/ashes_functions_list.hpp>
		};

		if ( auto it = functions.find( pName );
			it != functions.end() )
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
		static DeviceFunctions functions
		{
			{ "vkGetDeviceProcAddr", PFN_vkVoidFunction( vkGetDeviceProcAddr ) },
#define VK_LIB_DEVICE_FUNCTION( v, x )\
			{ "vk"#x, checkVersion( device, v ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#define VK_LIB_DEVICE_FUNCTION_EXT( v, n, x )\
			{ "vk"#x, checkVersionExt( device, v, n ) ? PFN_vkVoidFunction( vk##x ) : PFN_vkVoidFunction( nullptr ) },
#include <ashes/ashes_functions_list.hpp>
		};

		if ( auto it = functions.find( pName );
			it != functions.end() )
		{
			result = it->second;
		}

		return result;
	}

#pragma warning( pop )
}

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region ICD mode

	D3D11Renderer_API VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_icdGetInstanceProcAddr( VkInstance instance
		, const char * name )
	{
		if ( ashes::D3D11_NAMESPACE::getLibrary().init( ASHPLUGIN_ICD ) == VK_SUCCESS )
		{
			return ashes::D3D11_NAMESPACE::vkGetInstanceProcAddr( instance, name );
		}

		return nullptr;
	}

	D3D11Renderer_API PFN_vkVoidFunction VKAPI_CALL vk_icdGetPhysicalDeviceProcAddr( VkInstance instance,
		const char * name )
	{
		if ( ashes::D3D11_NAMESPACE::getLibrary().init( ASHPLUGIN_ICD ) == VK_SUCCESS )
		{
			return ashes::D3D11_NAMESPACE::vkGetPhysicalDeviceProcAddr( instance, name );
		}

		return nullptr;
	}

	D3D11Renderer_API VKAPI_ATTR VkResult VKAPI_CALL vk_icdNegotiateLoaderICDInterfaceVersion( uint32_t * pVersion )
	{
		auto result = ashes::D3D11_NAMESPACE::getLibrary().init( ASHPLUGIN_ICD );

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

	D3D11Renderer_API void VKAPI_CALL vkDestroySurfaceKHR( VkInstance instance, VkSurfaceKHR surface, const  VkAllocationCallbacks * pAllocator )
	{
		return ashes::D3D11_NAMESPACE::vkDestroySurfaceKHR( instance, surface, pAllocator );
	}

	D3D11Renderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32 * pSupported )
	{
		return ashes::D3D11_NAMESPACE::vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported );
	}

	D3D11Renderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR * pSurfaceCapabilities )
	{
		return ashes::D3D11_NAMESPACE::vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, pSurfaceCapabilities );
	}

	D3D11Renderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t * pSurfaceFormatCount, VkSurfaceFormatKHR * pSurfaceFormats )
	{
		return ashes::D3D11_NAMESPACE::vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
	}

	D3D11Renderer_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t * pPresentModeCount, VkPresentModeKHR * pPresentModes )
	{
		return ashes::D3D11_NAMESPACE::vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, pPresentModes );
	}

#endif
#pragma endregion
#pragma region VK_KHR_swapchain
#ifdef VK_KHR_swapchain

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateSwapchainKHR( VkDevice device, const  VkSwapchainCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSwapchainKHR * pSwapchain )
	{
		return ashes::D3D11_NAMESPACE::vkCreateSwapchainKHR( device, pCreateInfo, pAllocator, pSwapchain );
	}

#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateDisplayPlaneSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef VK_USE_PLATFORM_ANDROID_KHR

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR( VkInstance instance, const  VkAndroidSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateAndroidSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_FUCHSIA_imagepipe_surface
#	ifdef VK_USE_PLATFORM_FUCHSIA

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA( VkInstance instance, const  VkImagePipeSurfaceCreateInfoFUCHSIA * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateImagePipeSurfaceFUCHSIA( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_MVK_ios_surface
#	ifdef VK_USE_PLATFORM_IOS_MVK

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateIOSSurfaceMVK( VkInstance instance, const  VkIOSSurfaceCreateInfoMVK * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateIOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#	ifdef __APPLE__

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK( VkInstance instance, const  VkMacOSSurfaceCreateInfoMVK * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateMacOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_NN_vi_surface
#	ifdef VK_USE_PLATFORM_VI_NN

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateViSurfaceNN( VkInstance instance, const  VkViSurfaceCreateInfoNN * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateViSurfaceNN( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#	ifdef __linux__

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateXcbSurfaceKHR( VkInstance instance, const  VkXcbSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateXcbSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#	ifdef __linux__

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateXlibSurfaceKHR( VkInstance instance, const  VkXlibSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateXlibSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#	ifdef __linux__

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR( VkInstance instance, const  VkWaylandSurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateWaylandSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_KHR_win32_surface
#	ifdef _WIN32

	D3D11Renderer_API VkResult VKAPI_CALL vkCreateWin32SurfaceKHR( VkInstance instance, const  VkWin32SurfaceCreateInfoKHR * pCreateInfo, const  VkAllocationCallbacks * pAllocator, VkSurfaceKHR * pSurface )
	{
		return ashes::D3D11_NAMESPACE::vkCreateWin32SurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion

#pragma endregion
#pragma region Drop-in replacement mode

	D3D11Renderer_API VkResult VKAPI_PTR ashGetPluginDescription( AshPluginDescription * pDescription )
	{
		auto result = ashes::D3D11_NAMESPACE::getLibrary().init( ASHPLUGIN_DROPIN );

		if ( result == VK_SUCCESS )
		{
			*pDescription = ashes::D3D11_NAMESPACE::getLibrary().description;
		}

		return result;
	}

#pragma endregion

#ifdef __cplusplus
}
#endif
