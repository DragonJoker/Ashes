#include "ashes_plugin.hpp"

thread_local PluginLibrary g_library;

extern "C"
{
	Ashes_API PFN_vkVoidFunction VKAPI_PTR vkGetInstanceProcAddr( VkInstance instance
		, const char * name )
	{
		if ( g_library.init() == VK_SUCCESS )
		{
			return g_library.getSelectedDesc().getInstanceProcAddr( instance, name );
		}

		return nullptr;
	}

	Ashes_API void VKAPI_PTR ashGetPluginsDescriptions( uint32_t * count
		, AshPluginDescription * pDescriptions )
	{
		if ( g_library.init() == VK_SUCCESS )
		{
			*count = uint32_t( g_library.plugins.size() );

			if ( pDescriptions )
			{
				for ( auto & plugin : g_library.plugins )
				{
					*pDescriptions = plugin.description;
					++pDescriptions;
				}
			}
		}
	}

	Ashes_API VkResult VKAPI_PTR ashGetCurrentPluginDescription( AshPluginDescription * description )
	{
		auto result = g_library.init();

		if ( result == VK_SUCCESS )
		{
			*description = g_library.selectedPugin->description;
		}

		return result;
	}

#pragma region Vulkan 1.0
#ifdef VK_VERSION_1_0
	Ashes_API PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr( VkDevice device, const char* pName )
	{
		return g_library.getSelectedDesc().GetDeviceProcAddr( device, pName );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateInstance( const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance )
	{
		return g_library.getSelectedDesc().CreateInstance( pCreateInfo, pAllocator, pInstance );
	}

	Ashes_API void VKAPI_CALL vkDestroyInstance( VkInstance instance, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyInstance( instance, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumeratePhysicalDevices( VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice*  pPhysicalDevices )
	{
		return g_library.getSelectedDesc().EnumeratePhysicalDevices( instance, pPhysicalDeviceCount, pPhysicalDevices );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFeatures( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceFeatures( physicalDevice, pFeatures );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties*  pFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceFormatProperties( physicalDevice, format, pFormatProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceImageFormatProperties( physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceProperties( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceProperties( physicalDevice, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties( VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties*  pQueueFamilyProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceQueueFamilyProperties( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties( VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceMemoryProperties( physicalDevice, pMemoryProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDevice( VkPhysicalDevice physicalDevice, const  VkDeviceCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDevice* pDevice )
	{
		return g_library.getSelectedDesc().CreateDevice( physicalDevice, pCreateInfo, pAllocator, pDevice );
	}

	Ashes_API void VKAPI_CALL vkDestroyDevice( VkDevice device, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDevice( device, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties )
	{
		return g_library.getSelectedDesc().EnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties( VkPhysicalDevice physicalDevice, const  char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties )
	{
		return g_library.getSelectedDesc().EnumerateDeviceExtensionProperties( physicalDevice, pLayerName, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties( uint32_t* pPropertyCount, VkLayerProperties* pProperties )
	{
		return g_library.getSelectedDesc().EnumerateInstanceLayerProperties( pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties*  pProperties )
	{
		return g_library.getSelectedDesc().EnumerateDeviceLayerProperties( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceQueue( VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue )
	{
		return g_library.getSelectedDesc().GetDeviceQueue( device, queueFamilyIndex, queueIndex, pQueue );
	}

	Ashes_API VkResult VKAPI_CALL vkQueueSubmit( VkQueue queue, uint32_t submitCount, const  VkSubmitInfo* pSubmits, VkFence fence )
	{
		return g_library.getSelectedDesc().QueueSubmit( queue, submitCount, pSubmits, fence );
	}

	Ashes_API VkResult VKAPI_CALL vkQueueWaitIdle( VkQueue queue )
	{
		return g_library.getSelectedDesc().QueueWaitIdle( queue );
	}

	Ashes_API VkResult VKAPI_CALL vkDeviceWaitIdle( VkDevice device )
	{
		return g_library.getSelectedDesc().DeviceWaitIdle( device );
	}

	Ashes_API VkResult VKAPI_CALL vkAllocateMemory( VkDevice device, const  VkMemoryAllocateInfo* pAllocateInfo, const  VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory )
	{
		return g_library.getSelectedDesc().AllocateMemory( device, pAllocateInfo, pAllocator, pMemory );
	}

	Ashes_API void VKAPI_CALL vkFreeMemory( VkDevice device, VkDeviceMemory memory, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().FreeMemory( device, memory, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkMapMemory( VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData )
	{
		return g_library.getSelectedDesc().MapMemory( device, memory, offset, size, flags, ppData );
	}

	Ashes_API void VKAPI_CALL vkUnmapMemory( VkDevice device, VkDeviceMemory memory )
	{
		return g_library.getSelectedDesc().UnmapMemory( device, memory );
	}

	Ashes_API VkResult VKAPI_CALL vkFlushMappedMemoryRanges( VkDevice device, uint32_t memoryRangeCount, const  VkMappedMemoryRange* pMemoryRanges )
	{
		return g_library.getSelectedDesc().FlushMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
	}

	Ashes_API VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges( VkDevice device, uint32_t memoryRangeCount, const  VkMappedMemoryRange* pMemoryRanges )
	{
		return g_library.getSelectedDesc().InvalidateMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceMemoryCommitment( VkDevice device, VkDeviceMemory memory, VkDeviceSize*  pCommittedMemoryInBytes )
	{
		return g_library.getSelectedDesc().GetDeviceMemoryCommitment( device, memory, pCommittedMemoryInBytes );
	}

	Ashes_API VkResult VKAPI_CALL vkBindBufferMemory( VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset )
	{
		return g_library.getSelectedDesc().BindBufferMemory( device, buffer, memory, memoryOffset );
	}

	Ashes_API VkResult VKAPI_CALL vkBindImageMemory( VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset )
	{
		return g_library.getSelectedDesc().BindImageMemory( device, image, memory, memoryOffset );
	}

	Ashes_API void VKAPI_CALL vkGetBufferMemoryRequirements( VkDevice device, VkBuffer buffer, VkMemoryRequirements*  pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetBufferMemoryRequirements( device, buffer, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageMemoryRequirements( VkDevice device, VkImage image, VkMemoryRequirements*  pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetImageMemoryRequirements( device, image, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageSparseMemoryRequirements( VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetImageSparseMemoryRequirements( device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkQueueBindSparse( VkQueue queue, uint32_t bindInfoCount, const  VkBindSparseInfo* pBindInfo, VkFence fence )
	{
		return g_library.getSelectedDesc().QueueBindSparse( queue, bindInfoCount, pBindInfo, fence );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateFence( VkDevice device, const  VkFenceCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkFence* pFence )
	{
		return g_library.getSelectedDesc().CreateFence( device, pCreateInfo, pAllocator, pFence );
	}

	Ashes_API void VKAPI_CALL vkDestroyFence( VkDevice device, VkFence fence, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyFence( device, fence, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkResetFences( VkDevice device, uint32_t fenceCount, const  VkFence* pFences )
	{
		return g_library.getSelectedDesc().ResetFences( device, fenceCount, pFences );
	}

	Ashes_API VkResult VKAPI_CALL vkGetFenceStatus( VkDevice device, VkFence fence )
	{
		return g_library.getSelectedDesc().GetFenceStatus( device, fence );
	}

	Ashes_API VkResult VKAPI_CALL vkWaitForFences( VkDevice device, uint32_t fenceCount, const  VkFence* pFences, VkBool32 waitAll, uint64_t timeout )
	{
		return g_library.getSelectedDesc().WaitForFences( device, fenceCount, pFences, waitAll, timeout );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateSemaphore( VkDevice device, const  VkSemaphoreCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore )
	{
		return g_library.getSelectedDesc().CreateSemaphore( device, pCreateInfo, pAllocator, pSemaphore );
	}

	Ashes_API void VKAPI_CALL vkDestroySemaphore( VkDevice device, VkSemaphore semaphore, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroySemaphore( device, semaphore, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateEvent( VkDevice device, const  VkEventCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkEvent* pEvent )
	{
		return g_library.getSelectedDesc().CreateEvent( device, pCreateInfo, pAllocator, pEvent );
	}

	Ashes_API void VKAPI_CALL vkDestroyEvent( VkDevice device, VkEvent event, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyEvent( device, event, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetEventStatus( VkDevice device, VkEvent event )
	{
		return g_library.getSelectedDesc().GetEventStatus( device, event );
	}

	Ashes_API VkResult VKAPI_CALL vkSetEvent( VkDevice device, VkEvent event )
	{
		return g_library.getSelectedDesc().SetEvent( device, event );
	}

	Ashes_API VkResult VKAPI_CALL vkResetEvent( VkDevice device, VkEvent event )
	{
		return g_library.getSelectedDesc().ResetEvent( device, event );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateQueryPool( VkDevice device, const  VkQueryPoolCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool )
	{
		return g_library.getSelectedDesc().CreateQueryPool( device, pCreateInfo, pAllocator, pQueryPool );
	}

	Ashes_API void VKAPI_CALL vkDestroyQueryPool( VkDevice device, VkQueryPool queryPool, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyQueryPool( device, queryPool, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetQueryPoolResults( VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags )
	{
		return g_library.getSelectedDesc().GetQueryPoolResults( device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateBuffer( VkDevice device, const  VkBufferCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer )
	{
		return g_library.getSelectedDesc().CreateBuffer( device, pCreateInfo, pAllocator, pBuffer );
	}

	Ashes_API void VKAPI_CALL vkDestroyBuffer( VkDevice device, VkBuffer buffer, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyBuffer( device, buffer, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateBufferView( VkDevice device, const  VkBufferViewCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkBufferView* pView )
	{
		return g_library.getSelectedDesc().CreateBufferView( device, pCreateInfo, pAllocator, pView );
	}

	Ashes_API void VKAPI_CALL vkDestroyBufferView( VkDevice device, VkBufferView bufferView, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyBufferView( device, bufferView, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateImage( VkDevice device, const  VkImageCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkImage* pImage )
	{
		return g_library.getSelectedDesc().CreateImage( device, pCreateInfo, pAllocator, pImage );
	}

	Ashes_API void VKAPI_CALL vkDestroyImage( VkDevice device, VkImage image, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyImage( device, image, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkGetImageSubresourceLayout( VkDevice device, VkImage image, const  VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout )
	{
		return g_library.getSelectedDesc().GetImageSubresourceLayout( device, image, pSubresource, pLayout );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateImageView( VkDevice device, const  VkImageViewCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkImageView* pView )
	{
		return g_library.getSelectedDesc().CreateImageView( device, pCreateInfo, pAllocator, pView );
	}

	Ashes_API void VKAPI_CALL vkDestroyImageView( VkDevice device, VkImageView imageView, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyImageView( device, imageView, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateShaderModule( VkDevice device, const  VkShaderModuleCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule )
	{
		return g_library.getSelectedDesc().CreateShaderModule( device, pCreateInfo, pAllocator, pShaderModule );
	}

	Ashes_API void VKAPI_CALL vkDestroyShaderModule( VkDevice device, VkShaderModule shaderModule, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyShaderModule( device, shaderModule, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreatePipelineCache( VkDevice device, const  VkPipelineCacheCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache )
	{
		return g_library.getSelectedDesc().CreatePipelineCache( device, pCreateInfo, pAllocator, pPipelineCache );
	}

	Ashes_API void VKAPI_CALL vkDestroyPipelineCache( VkDevice device, VkPipelineCache pipelineCache, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyPipelineCache( device, pipelineCache, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPipelineCacheData( VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData )
	{
		return g_library.getSelectedDesc().GetPipelineCacheData( device, pipelineCache, pDataSize, pData );
	}

	Ashes_API VkResult VKAPI_CALL vkMergePipelineCaches( VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const  VkPipelineCache* pSrcCaches )
	{
		return g_library.getSelectedDesc().MergePipelineCaches( device, dstCache, srcCacheCount, pSrcCaches );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateGraphicsPipelines( VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const  VkGraphicsPipelineCreateInfo* pCreateInfos, const  VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines )
	{
		return g_library.getSelectedDesc().CreateGraphicsPipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateComputePipelines( VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const  VkComputePipelineCreateInfo* pCreateInfos, const  VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines )
	{
		return g_library.getSelectedDesc().CreateComputePipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
	}

	Ashes_API void VKAPI_CALL vkDestroyPipeline( VkDevice device, VkPipeline pipeline, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyPipeline( device, pipeline, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreatePipelineLayout( VkDevice device, const  VkPipelineLayoutCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout )
	{
		return g_library.getSelectedDesc().CreatePipelineLayout( device, pCreateInfo, pAllocator, pPipelineLayout );
	}

	Ashes_API void VKAPI_CALL vkDestroyPipelineLayout( VkDevice device, VkPipelineLayout pipelineLayout, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyPipelineLayout( device, pipelineLayout, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateSampler( VkDevice device, const  VkSamplerCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSampler* pSampler )
	{
		return g_library.getSelectedDesc().CreateSampler( device, pCreateInfo, pAllocator, pSampler );
	}

	Ashes_API void VKAPI_CALL vkDestroySampler( VkDevice device, VkSampler sampler, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroySampler( device, sampler, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorSetLayout( VkDevice device, const  VkDescriptorSetLayoutCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout )
	{
		return g_library.getSelectedDesc().CreateDescriptorSetLayout( device, pCreateInfo, pAllocator, pSetLayout );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorSetLayout( VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDescriptorSetLayout( device, descriptorSetLayout, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorPool( VkDevice device, const  VkDescriptorPoolCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool )
	{
		return g_library.getSelectedDesc().CreateDescriptorPool( device, pCreateInfo, pAllocator, pDescriptorPool );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorPool( VkDevice device, VkDescriptorPool descriptorPool, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDescriptorPool( device, descriptorPool, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkResetDescriptorPool( VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags  flags )
	{
		return g_library.getSelectedDesc().ResetDescriptorPool( device, descriptorPool, flags );
	}

	Ashes_API VkResult VKAPI_CALL vkAllocateDescriptorSets( VkDevice device, const  VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets )
	{
		return g_library.getSelectedDesc().AllocateDescriptorSets( device, pAllocateInfo, pDescriptorSets );
	}

	Ashes_API VkResult VKAPI_CALL vkFreeDescriptorSets( VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const  VkDescriptorSet* pDescriptorSets )
	{
		return g_library.getSelectedDesc().FreeDescriptorSets( device, descriptorPool, descriptorSetCount, pDescriptorSets );
	}

	Ashes_API void VKAPI_CALL vkUpdateDescriptorSets( VkDevice device, uint32_t descriptorWriteCount, const  VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const  VkCopyDescriptorSet* pDescriptorCopies )
	{
		return g_library.getSelectedDesc().UpdateDescriptorSets( device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateFramebuffer( VkDevice device, const  VkFramebufferCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer )
	{
		return g_library.getSelectedDesc().CreateFramebuffer( device, pCreateInfo, pAllocator, pFramebuffer );
	}

	Ashes_API void VKAPI_CALL vkDestroyFramebuffer( VkDevice device, VkFramebuffer framebuffer, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyFramebuffer( device, framebuffer, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateRenderPass( VkDevice device, const  VkRenderPassCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass )
	{
		return g_library.getSelectedDesc().CreateRenderPass( device, pCreateInfo, pAllocator, pRenderPass );
	}

	Ashes_API void VKAPI_CALL vkDestroyRenderPass( VkDevice device, VkRenderPass renderPass, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyRenderPass( device, renderPass, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkGetRenderAreaGranularity( VkDevice device, VkRenderPass renderPass, VkExtent2D*  pGranularity )
	{
		return g_library.getSelectedDesc().GetRenderAreaGranularity( device, renderPass, pGranularity );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateCommandPool( VkDevice device, const  VkCommandPoolCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool )
	{
		return g_library.getSelectedDesc().CreateCommandPool( device, pCreateInfo, pAllocator, pCommandPool );
	}

	Ashes_API void VKAPI_CALL vkDestroyCommandPool( VkDevice device, VkCommandPool commandPool, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyCommandPool( device, commandPool, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkResetCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags  flags )
	{
		return g_library.getSelectedDesc().ResetCommandPool( device, commandPool, flags );
	}

	Ashes_API VkResult VKAPI_CALL vkAllocateCommandBuffers( VkDevice device, const  VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers )
	{
		return g_library.getSelectedDesc().AllocateCommandBuffers( device, pAllocateInfo, pCommandBuffers );
	}

	Ashes_API void VKAPI_CALL vkFreeCommandBuffers( VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const  VkCommandBuffer* pCommandBuffers )
	{
		return g_library.getSelectedDesc().FreeCommandBuffers( device, commandPool, commandBufferCount, pCommandBuffers );
	}

	Ashes_API VkResult VKAPI_CALL vkBeginCommandBuffer( VkCommandBuffer commandBuffer, const  VkCommandBufferBeginInfo* pBeginInfo )
	{
		return g_library.getSelectedDesc().BeginCommandBuffer( commandBuffer, pBeginInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkEndCommandBuffer( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().EndCommandBuffer( commandBuffer );
	}

	Ashes_API VkResult VKAPI_CALL vkResetCommandBuffer( VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags )
	{
		return g_library.getSelectedDesc().ResetCommandBuffer( commandBuffer, flags );
	}

	Ashes_API void VKAPI_CALL vkCmdBindPipeline( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline  pipeline )
	{
		return g_library.getSelectedDesc().CmdBindPipeline( commandBuffer, pipelineBindPoint, pipeline );
	}

	Ashes_API void VKAPI_CALL vkCmdSetViewport( VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const  VkViewport* pViewports )
	{
		return g_library.getSelectedDesc().CmdSetViewport( commandBuffer, firstViewport, viewportCount, pViewports );
	}

	Ashes_API void VKAPI_CALL vkCmdSetScissor( VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const  VkRect2D* pScissors )
	{
		return g_library.getSelectedDesc().CmdSetScissor( commandBuffer, firstScissor, scissorCount, pScissors );
	}

	Ashes_API void VKAPI_CALL vkCmdSetLineWidth( VkCommandBuffer commandBuffer, float lineWidth )
	{
		return g_library.getSelectedDesc().CmdSetLineWidth( commandBuffer, lineWidth );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDepthBias( VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor )
	{
		return g_library.getSelectedDesc().CmdSetDepthBias( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
	}

	Ashes_API void VKAPI_CALL vkCmdSetBlendConstants( VkCommandBuffer commandBuffer, const  float blendConstants[4] )
	{
		return g_library.getSelectedDesc().CmdSetBlendConstants( commandBuffer, blendConstants );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDepthBounds( VkCommandBuffer commandBuffer, float minDepthBounds, float  maxDepthBounds )
	{
		return g_library.getSelectedDesc().CmdSetDepthBounds( commandBuffer, minDepthBounds, maxDepthBounds );
	}

	Ashes_API void VKAPI_CALL vkCmdSetStencilCompareMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t  compareMask )
	{
		return g_library.getSelectedDesc().CmdSetStencilCompareMask( commandBuffer, faceMask, compareMask );
	}

	Ashes_API void VKAPI_CALL vkCmdSetStencilWriteMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t  writeMask )
	{
		return g_library.getSelectedDesc().CmdSetStencilWriteMask( commandBuffer, faceMask, writeMask );
	}

	Ashes_API void VKAPI_CALL vkCmdSetStencilReference( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t  reference )
	{
		return g_library.getSelectedDesc().CmdSetStencilReference( commandBuffer, faceMask, reference );
	}

	Ashes_API void VKAPI_CALL vkCmdBindDescriptorSets( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const  VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const  uint32_t* pDynamicOffsets )
	{
		return g_library.getSelectedDesc().CmdBindDescriptorSets( commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets );
	}

	Ashes_API void VKAPI_CALL vkCmdBindIndexBuffer( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType )
	{
		return g_library.getSelectedDesc().CmdBindIndexBuffer( commandBuffer, buffer, offset, indexType );
	}

	Ashes_API void VKAPI_CALL vkCmdBindVertexBuffers( VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const  VkBuffer* pBuffers, const  VkDeviceSize* pOffsets )
	{
		return g_library.getSelectedDesc().CmdBindVertexBuffers( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets );
	}

	Ashes_API void VKAPI_CALL vkCmdDraw( VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance )
	{
		return g_library.getSelectedDesc().CmdDraw( commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndexed( VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance )
	{
		return g_library.getSelectedDesc().CmdDrawIndexed( commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawIndirect( commandBuffer, buffer, offset, drawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndexedIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawIndexedIndirect( commandBuffer, buffer, offset, drawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatch( VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ )
	{
		return g_library.getSelectedDesc().CmdDispatch( commandBuffer, groupCountX, groupCountY, groupCountZ );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatchIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize  offset )
	{
		return g_library.getSelectedDesc().CmdDispatchIndirect( commandBuffer, buffer, offset );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyBuffer( VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const  VkBufferCopy* pRegions )
	{
		return g_library.getSelectedDesc().CmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyImage( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkImageCopy* pRegions )
	{
		return g_library.getSelectedDesc().CmdCopyImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdBlitImage( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkImageBlit* pRegions, VkFilter filter )
	{
		return g_library.getSelectedDesc().CmdBlitImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyBufferToImage( VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkBufferImageCopy* pRegions )
	{
		return g_library.getSelectedDesc().CmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyImageToBuffer( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const  VkBufferImageCopy* pRegions )
	{
		return g_library.getSelectedDesc().CmdCopyImageToBuffer( commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdUpdateBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const  void* pData )
	{
		return g_library.getSelectedDesc().CmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, pData );
	}

	Ashes_API void VKAPI_CALL vkCmdFillBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data )
	{
		return g_library.getSelectedDesc().CmdFillBuffer( commandBuffer, dstBuffer, dstOffset, size, data );
	}

	Ashes_API void VKAPI_CALL vkCmdClearColorImage( VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const  VkClearColorValue* pColor, uint32_t rangeCount, const  VkImageSubresourceRange* pRanges )
	{
		return g_library.getSelectedDesc().CmdClearColorImage( commandBuffer, image, imageLayout, pColor, rangeCount, pRanges );
	}

	Ashes_API void VKAPI_CALL vkCmdClearDepthStencilImage( VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const  VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const  VkImageSubresourceRange* pRanges )
	{
		return g_library.getSelectedDesc().CmdClearDepthStencilImage( commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges );
	}

	Ashes_API void VKAPI_CALL vkCmdClearAttachments( VkCommandBuffer commandBuffer, uint32_t attachmentCount, const  VkClearAttachment* pAttachments, uint32_t rectCount, const  VkClearRect* pRects )
	{
		return g_library.getSelectedDesc().CmdClearAttachments( commandBuffer, attachmentCount, pAttachments, rectCount, pRects );
	}

	Ashes_API void VKAPI_CALL vkCmdResolveImage( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkImageResolve* pRegions )
	{
		return g_library.getSelectedDesc().CmdResolveImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdSetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags  stageMask )
	{
		return g_library.getSelectedDesc().CmdSetEvent( commandBuffer, event, stageMask );
	}

	Ashes_API void VKAPI_CALL vkCmdResetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags  stageMask )
	{
		return g_library.getSelectedDesc().CmdResetEvent( commandBuffer, event, stageMask );
	}

	Ashes_API void VKAPI_CALL vkCmdWaitEvents( VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers )
	{
		return g_library.getSelectedDesc().CmdWaitEvents( commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers );
	}

	Ashes_API void VKAPI_CALL vkCmdPipelineBarrier( VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const  VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const  VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const  VkImageMemoryBarrier* pImageMemoryBarriers )
	{
		return g_library.getSelectedDesc().CmdPipelineBarrier( commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags )
	{
		return g_library.getSelectedDesc().CmdBeginQuery( commandBuffer, queryPool, query, flags );
	}

	Ashes_API void VKAPI_CALL vkCmdEndQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t  query )
	{
		return g_library.getSelectedDesc().CmdEndQuery( commandBuffer, queryPool, query );
	}

	Ashes_API void VKAPI_CALL vkCmdResetQueryPool( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount )
	{
		return g_library.getSelectedDesc().CmdResetQueryPool( commandBuffer, queryPool, firstQuery, queryCount );
	}

	Ashes_API void VKAPI_CALL vkCmdWriteTimestamp( VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query )
	{
		return g_library.getSelectedDesc().CmdWriteTimestamp( commandBuffer, pipelineStage, queryPool, query );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyQueryPoolResults( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags )
	{
		return g_library.getSelectedDesc().CmdCopyQueryPoolResults( commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags );
	}

	Ashes_API void VKAPI_CALL vkCmdPushConstants( VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const  void* pValues )
	{
		return g_library.getSelectedDesc().CmdPushConstants( commandBuffer, layout, stageFlags, offset, size, pValues );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginRenderPass( VkCommandBuffer commandBuffer, const  VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents )
	{
		return g_library.getSelectedDesc().CmdBeginRenderPass( commandBuffer, pRenderPassBegin, contents );
	}

	Ashes_API void VKAPI_CALL vkCmdNextSubpass( VkCommandBuffer commandBuffer, VkSubpassContents contents )
	{
		return g_library.getSelectedDesc().CmdNextSubpass( commandBuffer, contents );
	}

	Ashes_API void VKAPI_CALL vkCmdEndRenderPass( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().CmdEndRenderPass( commandBuffer );
	}

	Ashes_API void VKAPI_CALL vkCmdExecuteCommands( VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const  VkCommandBuffer* pCommandBuffers )
	{
		return g_library.getSelectedDesc().CmdExecuteCommands( commandBuffer, commandBufferCount, pCommandBuffers );
	}

#endif
#pragma endregion
#pragma region Vulkan 1.1
#ifdef VK_VERSION_1_1

	Ashes_API VkResult VKAPI_PTR vkEnumerateInstanceVersion( uint32_t * version )
	{
		return g_library.getSelectedDesc().EnumerateInstanceVersion( version );
	}

	Ashes_API VkResult VKAPI_CALL vkBindBufferMemory2( VkDevice device, uint32_t bindInfoCount, const  VkBindBufferMemoryInfo* pBindInfos )
	{
		return g_library.getSelectedDesc().BindBufferMemory2( device, bindInfoCount, pBindInfos );
	}

	Ashes_API VkResult VKAPI_CALL vkBindImageMemory2( VkDevice device, uint32_t bindInfoCount, const  VkBindImageMemoryInfo* pBindInfos )
	{
		return g_library.getSelectedDesc().BindImageMemory2( device, bindInfoCount, pBindInfos );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures( VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		return g_library.getSelectedDesc().GetDeviceGroupPeerMemoryFeatures( device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDeviceMask( VkCommandBuffer commandBuffer, uint32_t deviceMask )
	{
		return g_library.getSelectedDesc().CmdSetDeviceMask( commandBuffer, deviceMask );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatchBase( VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ )
	{
		return g_library.getSelectedDesc().CmdDispatchBase( commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups( VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties*  pPhysicalDeviceGroupProperties )
	{
		return g_library.getSelectedDesc().EnumeratePhysicalDeviceGroups( instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties );
	}

	Ashes_API void VKAPI_CALL vkGetImageMemoryRequirements2( VkDevice device, const  VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetImageMemoryRequirements2( device, pInfo, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetBufferMemoryRequirements2( VkDevice device, const  VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetBufferMemoryRequirements2( device, pInfo, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageSparseMemoryRequirements2( VkDevice device, const  VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetImageSparseMemoryRequirements2( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFeatures2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceFeatures2( physicalDevice, pFeatures );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceProperties2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceProperties2( physicalDevice, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2( VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2*  pFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceFormatProperties2( physicalDevice, format, pFormatProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceImageFormatProperties2( physicalDevice, pImageFormatInfo, pImageFormatProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2( VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2*  pQueueFamilyProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceQueueFamilyProperties2( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceMemoryProperties2( physicalDevice, pMemoryProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSparseImageFormatProperties2( physicalDevice, pFormatInfo, pPropertyCount, pProperties );
	}

	Ashes_API void VKAPI_CALL vkTrimCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags  flags )
	{
		return g_library.getSelectedDesc().TrimCommandPool( device, commandPool, flags );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceQueue2( VkDevice device, const  VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue )
	{
		return g_library.getSelectedDesc().GetDeviceQueue2( device, pQueueInfo, pQueue );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion( VkDevice device, const  VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		return g_library.getSelectedDesc().CreateSamplerYcbcrConversion( device, pCreateInfo, pAllocator, pYcbcrConversion );
	}

	Ashes_API void VKAPI_CALL vkDestroySamplerYcbcrConversion( VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroySamplerYcbcrConversion( device, ycbcrConversion, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate( VkDevice device, const  VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		return g_library.getSelectedDesc().CreateDescriptorUpdateTemplate( device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorUpdateTemplate( VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDescriptorUpdateTemplate( device, descriptorUpdateTemplate, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkUpdateDescriptorSetWithTemplate( VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const  void* pData )
	{
		return g_library.getSelectedDesc().UpdateDescriptorSetWithTemplate( device, descriptorSet, descriptorUpdateTemplate, pData );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalBufferProperties( physicalDevice, pExternalBufferInfo, pExternalBufferProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalFenceProperties( physicalDevice, pExternalFenceInfo, pExternalFenceProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalSemaphoreProperties( physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties );
	}

	Ashes_API void VKAPI_CALL vkGetDescriptorSetLayoutSupport( VkDevice device, const  VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport )
	{
		return g_library.getSelectedDesc().GetDescriptorSetLayoutSupport( device, pCreateInfo, pSupport );
	}

#endif
#pragma endregion
#pragma region VK_KHR_surface
#ifdef VK_KHR_surface

	Ashes_API void VKAPI_CALL vkDestroySurfaceKHR( VkInstance instance, VkSurfaceKHR surface, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroySurfaceKHR( instance, surface, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR*  pSurfaceCapabilities )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, pSurfaceCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, pPresentModes );
	}

#endif
#pragma endregion
#pragma region VK_KHR_swapchain
#ifdef VK_KHR_swapchain

	Ashes_API VkResult VKAPI_CALL vkCreateSwapchainKHR( VkDevice device, const  VkSwapchainCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain )
	{
		return g_library.getSelectedDesc().CreateSwapchainKHR( device, pCreateInfo, pAllocator, pSwapchain );
	}

	Ashes_API void VKAPI_CALL vkDestroySwapchainKHR( VkDevice device, VkSwapchainKHR swapchain, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroySwapchainKHR( device, swapchain, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetSwapchainImagesKHR( VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages )
	{
		return g_library.getSelectedDesc().GetSwapchainImagesKHR( device, swapchain, pSwapchainImageCount, pSwapchainImages );
	}

	Ashes_API VkResult VKAPI_CALL vkAcquireNextImageKHR( VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex )
	{
		return g_library.getSelectedDesc().AcquireNextImageKHR( device, swapchain, timeout, semaphore, fence, pImageIndex );
	}

	Ashes_API VkResult VKAPI_CALL vkQueuePresentKHR( VkQueue queue, const  VkPresentInfoKHR* pPresentInfo )
	{
		return g_library.getSelectedDesc().QueuePresentKHR( queue, pPresentInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR( VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities )
	{
		return g_library.getSelectedDesc().GetDeviceGroupPresentCapabilitiesKHR( device, pDeviceGroupPresentCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR( VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR*  pModes )
	{
		return g_library.getSelectedDesc().GetDeviceGroupSurfacePresentModesKHR( device, surface, pModes );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects )
	{
		return g_library.getSelectedDesc().GetPhysicalDevicePresentRectanglesKHR( physicalDevice, surface, pRectCount, pRects );
	}

	Ashes_API VkResult VKAPI_CALL vkAcquireNextImage2KHR( VkDevice device, const  VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex )
	{
		return g_library.getSelectedDesc().AcquireNextImage2KHR( device, pAcquireInfo, pImageIndex );
	}

#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPropertiesKHR*  pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceDisplayPropertiesKHR( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR*  pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR( VkPhysicalDevice physicalDevice, uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays )
	{
		return g_library.getSelectedDesc().GetDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, pDisplayCount, pDisplays );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR( VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties )
	{
		return g_library.getSelectedDesc().GetDisplayModePropertiesKHR( physicalDevice, display, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDisplayModeKHR( VkPhysicalDevice physicalDevice, VkDisplayKHR display, const  VkDisplayModeCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode )
	{
		return g_library.getSelectedDesc().CreateDisplayModeKHR( physicalDevice, display, pCreateInfo, pAllocator, pMode );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR( VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities )
	{
		return g_library.getSelectedDesc().GetDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, pCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR( VkInstance instance, const  VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateDisplayPlaneSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_physical_device_properties2
#ifdef VK_KHR_get_physical_device_properties2

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFeatures2KHR( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceFeatures2KHR( physicalDevice, pFeatures );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceProperties2KHR( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceProperties2KHR( physicalDevice, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2KHR( VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2*  pFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceFormatProperties2KHR( physicalDevice, format, pFormatProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2KHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceImageFormatProperties2KHR( physicalDevice, pImageFormatInfo, pImageFormatProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2KHR( VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2*  pQueueFamilyProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceQueueFamilyProperties2KHR( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2KHR( VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceMemoryProperties2KHR( physicalDevice, pMemoryProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2KHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSparseImageFormatProperties2KHR( physicalDevice, pFormatInfo, pPropertyCount, pProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#ifdef VK_KHR_device_group

	Ashes_API void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeaturesKHR( VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		return g_library.getSelectedDesc().GetDeviceGroupPeerMemoryFeaturesKHR( device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDeviceMaskKHR( VkCommandBuffer commandBuffer, uint32_t deviceMask )
	{
		return g_library.getSelectedDesc().CmdSetDeviceMaskKHR( commandBuffer, deviceMask );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatchBaseKHR( VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ )
	{
		return g_library.getSelectedDesc().CmdDispatchBaseKHR( commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
	}

#endif
#pragma endregion
#pragma region VK_KHR_maintenance1
#ifdef VK_KHR_maintenance1

	Ashes_API void VKAPI_CALL vkTrimCommandPoolKHR( VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags  flags )
	{
		return g_library.getSelectedDesc().TrimCommandPoolKHR( device, commandPool, flags );
	}

#endif
#pragma endregion
#pragma region VK_KHR_device_group_creation
#ifdef VK_KHR_device_group_creation

	Ashes_API VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroupsKHR( VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties*  pPhysicalDeviceGroupProperties )
	{
		return g_library.getSelectedDesc().EnumeratePhysicalDeviceGroupsKHR( instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_capabilities
#ifdef VK_KHR_external_memory_capabilities

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalBufferPropertiesKHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalBufferPropertiesKHR( physicalDevice, pExternalBufferInfo, pExternalBufferProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_memory_fd
#ifdef VK_KHR_external_memory_fd

	Ashes_API VkResult VKAPI_CALL vkGetMemoryFdKHR( VkDevice device, const  VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd )
	{
		return g_library.getSelectedDesc().GetMemoryFdKHR( device, pGetFdInfo, pFd );
	}

	Ashes_API VkResult VKAPI_CALL vkGetMemoryFdPropertiesKHR( VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties )
	{
		return g_library.getSelectedDesc().GetMemoryFdPropertiesKHR( device, handleType, fd, pMemoryFdProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_capabilities
#ifdef VK_KHR_external_semaphore_capabilities

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphorePropertiesKHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalSemaphorePropertiesKHR( physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_semaphore_fd
#ifdef VK_KHR_external_semaphore_fd

	Ashes_API VkResult VKAPI_CALL vkImportSemaphoreFdKHR( VkDevice device, const  VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo )
	{
		return g_library.getSelectedDesc().ImportSemaphoreFdKHR( device, pImportSemaphoreFdInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkGetSemaphoreFdKHR( VkDevice device, const  VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd )
	{
		return g_library.getSelectedDesc().GetSemaphoreFdKHR( device, pGetFdInfo, pFd );
	}

#endif
#pragma endregion
#pragma region VK_KHR_push_descriptor
#ifdef VK_KHR_push_descriptor

	Ashes_API void VKAPI_CALL vkCmdPushDescriptorSetKHR( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const  VkWriteDescriptorSet* pDescriptorWrites )
	{
		return g_library.getSelectedDesc().CmdPushDescriptorSetKHR( commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites );
	}

	Ashes_API void VKAPI_CALL vkCmdPushDescriptorSetWithTemplateKHR( VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const  void* pData )
	{
		return g_library.getSelectedDesc().CmdPushDescriptorSetWithTemplateKHR( commandBuffer, descriptorUpdateTemplate, layout, set, pData );
	}

#endif
#pragma endregion
#pragma region VK_KHR_descriptor_update_template
#ifdef VK_KHR_descriptor_update_template

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplateKHR( VkDevice device, const  VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		return g_library.getSelectedDesc().CreateDescriptorUpdateTemplateKHR( device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorUpdateTemplateKHR( VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDescriptorUpdateTemplateKHR( device, descriptorUpdateTemplate, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkUpdateDescriptorSetWithTemplateKHR( VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const  void* pData )
	{
		return g_library.getSelectedDesc().UpdateDescriptorSetWithTemplateKHR( device, descriptorSet, descriptorUpdateTemplate, pData );
	}

#endif
#pragma endregion
#pragma region VK_KHR_create_renderpass2
#ifdef VK_KHR_create_renderpass2

	Ashes_API VkResult VKAPI_CALL vkCreateRenderPass2KHR( VkDevice device, const  VkRenderPassCreateInfo2KHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass )
	{
		return g_library.getSelectedDesc().CreateRenderPass2KHR( device, pCreateInfo, pAllocator, pRenderPass );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginRenderPass2KHR( VkCommandBuffer commandBuffer, const  VkRenderPassBeginInfo* pRenderPassBegin, const  VkSubpassBeginInfoKHR* pSubpassBeginInfo )
	{
		return g_library.getSelectedDesc().CmdBeginRenderPass2KHR( commandBuffer, pRenderPassBegin, pSubpassBeginInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdNextSubpass2KHR( VkCommandBuffer commandBuffer, const  VkSubpassBeginInfoKHR* pSubpassBeginInfo, const  VkSubpassEndInfoKHR* pSubpassEndInfo )
	{
		return g_library.getSelectedDesc().CmdNextSubpass2KHR( commandBuffer, pSubpassBeginInfo, pSubpassEndInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdEndRenderPass2KHR( VkCommandBuffer commandBuffer, const  VkSubpassEndInfoKHR* pSubpassEndInfo )
	{
		return g_library.getSelectedDesc().CmdEndRenderPass2KHR( commandBuffer, pSubpassEndInfo );
	}

#endif
#pragma endregion
#pragma region VK_KHR_shared_presentable_image
#ifdef VK_KHR_shared_presentable_image

	Ashes_API VkResult VKAPI_CALL vkGetSwapchainStatusKHR( VkDevice device, VkSwapchainKHR swapchain )
	{
		return g_library.getSelectedDesc().GetSwapchainStatusKHR( device, swapchain );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_capabilities
#ifdef VK_KHR_external_fence_capabilities

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalFencePropertiesKHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalFencePropertiesKHR( physicalDevice, pExternalFenceInfo, pExternalFenceProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_external_fence_fd
#ifdef VK_KHR_external_fence_fd

	Ashes_API VkResult VKAPI_CALL vkImportFenceFdKHR( VkDevice device, const  VkImportFenceFdInfoKHR* pImportFenceFdInfo )
	{
		return g_library.getSelectedDesc().ImportFenceFdKHR( device, pImportFenceFdInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkGetFenceFdKHR( VkDevice device, const  VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd )
	{
		return g_library.getSelectedDesc().GetFenceFdKHR( device, pGetFdInfo, pFd );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_surface_capabilities2
#ifdef VK_KHR_get_surface_capabilities2

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2KHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfaceCapabilities2KHR( physicalDevice, pSurfaceInfo, pSurfaceCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormats2KHR( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfaceFormats2KHR( physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_display_properties2
#ifdef VK_KHR_get_display_properties2

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayProperties2KHR( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayProperties2KHR*  pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceDisplayProperties2KHR( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlaneProperties2KHR( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR*  pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceDisplayPlaneProperties2KHR( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayModeProperties2KHR( VkPhysicalDevice physicalDevice, VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties )
	{
		return g_library.getSelectedDesc().GetDisplayModeProperties2KHR( physicalDevice, display, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayPlaneCapabilities2KHR( VkPhysicalDevice physicalDevice, const  VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities )
	{
		return g_library.getSelectedDesc().GetDisplayPlaneCapabilities2KHR( physicalDevice, pDisplayPlaneInfo, pCapabilities );
	}

#endif
#pragma endregion
#pragma region VK_KHR_get_memory_requirements2
#ifdef VK_KHR_get_memory_requirements2

	Ashes_API void VKAPI_CALL vkGetImageMemoryRequirements2KHR( VkDevice device, const  VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetImageMemoryRequirements2KHR( device, pInfo, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetBufferMemoryRequirements2KHR( VkDevice device, const  VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetBufferMemoryRequirements2KHR( device, pInfo, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageSparseMemoryRequirements2KHR( VkDevice device, const  VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetImageSparseMemoryRequirements2KHR( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
	}

#endif
#pragma endregion
#pragma region VK_KHR_sampler_ycbcr_conversion
#ifdef VK_KHR_sampler_ycbcr_conversion

	Ashes_API VkResult VKAPI_CALL vkCreateSamplerYcbcrConversionKHR( VkDevice device, const  VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		return g_library.getSelectedDesc().CreateSamplerYcbcrConversionKHR( device, pCreateInfo, pAllocator, pYcbcrConversion );
	}

	Ashes_API void VKAPI_CALL vkDestroySamplerYcbcrConversionKHR( VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroySamplerYcbcrConversionKHR( device, ycbcrConversion, pAllocator );
	}

#endif
#pragma endregion
#pragma region VK_KHR_bind_memory2
#ifdef VK_KHR_bind_memory2

	Ashes_API VkResult VKAPI_CALL vkBindBufferMemory2KHR( VkDevice device, uint32_t bindInfoCount, const  VkBindBufferMemoryInfo* pBindInfos )
	{
		return g_library.getSelectedDesc().BindBufferMemory2KHR( device, bindInfoCount, pBindInfos );
	}

	Ashes_API VkResult VKAPI_CALL vkBindImageMemory2KHR( VkDevice device, uint32_t bindInfoCount, const  VkBindImageMemoryInfo* pBindInfos )
	{
		return g_library.getSelectedDesc().BindImageMemory2KHR( device, bindInfoCount, pBindInfos );
	}

#endif
#pragma endregion
#pragma region VK_KHR_maintenance3
#ifdef VK_KHR_maintenance3

	Ashes_API void VKAPI_CALL vkGetDescriptorSetLayoutSupportKHR( VkDevice device, const  VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport )
	{
		return g_library.getSelectedDesc().GetDescriptorSetLayoutSupportKHR( device, pCreateInfo, pSupport );
	}

#endif
#pragma endregion
#pragma region VK_KHR_draw_indirect_count
#ifdef VK_KHR_draw_indirect_count

	Ashes_API void VKAPI_CALL vkCmdDrawIndirectCountKHR( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawIndirectCountKHR( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndexedIndirectCountKHR( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawIndexedIndirectCountKHR( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_report
#ifdef VK_EXT_debug_report

	Ashes_API VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT( VkInstance instance, const  VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback )
	{
		return g_library.getSelectedDesc().CreateDebugReportCallbackEXT( instance, pCreateInfo, pAllocator, pCallback );
	}

	Ashes_API void VKAPI_CALL vkDestroyDebugReportCallbackEXT( VkInstance instance, VkDebugReportCallbackEXT callback, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDebugReportCallbackEXT( instance, callback, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkDebugReportMessageEXT( VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const  char* pLayerPrefix, const  char* pMessage )
	{
		return g_library.getSelectedDesc().DebugReportMessageEXT( instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_marker
#ifdef VK_EXT_debug_marker

	Ashes_API VkResult VKAPI_CALL vkDebugMarkerSetObjectTagEXT( VkDevice device, const  VkDebugMarkerObjectTagInfoEXT* pTagInfo )
	{
		return g_library.getSelectedDesc().DebugMarkerSetObjectTagEXT( device, pTagInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkDebugMarkerSetObjectNameEXT( VkDevice device, const  VkDebugMarkerObjectNameInfoEXT* pNameInfo )
	{
		return g_library.getSelectedDesc().DebugMarkerSetObjectNameEXT( device, pNameInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdDebugMarkerBeginEXT( VkCommandBuffer commandBuffer, const  VkDebugMarkerMarkerInfoEXT* pMarkerInfo )
	{
		return g_library.getSelectedDesc().CmdDebugMarkerBeginEXT( commandBuffer, pMarkerInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdDebugMarkerEndEXT( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().CmdDebugMarkerEndEXT( commandBuffer );
	}

	Ashes_API void VKAPI_CALL vkCmdDebugMarkerInsertEXT( VkCommandBuffer commandBuffer, const  VkDebugMarkerMarkerInfoEXT* pMarkerInfo )
	{
		return g_library.getSelectedDesc().CmdDebugMarkerInsertEXT( commandBuffer, pMarkerInfo );
	}

#endif
#pragma endregion
#pragma region VK_EXT_transform_feedback
#ifdef VK_EXT_transform_feedback

	Ashes_API void VKAPI_CALL vkCmdBindTransformFeedbackBuffersEXT( VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const  VkBuffer* pBuffers, const  VkDeviceSize* pOffsets, const  VkDeviceSize* pSizes )
	{
		return g_library.getSelectedDesc().CmdBindTransformFeedbackBuffersEXT( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginTransformFeedbackEXT( VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const  VkBuffer* pCounterBuffers, const  VkDeviceSize* pCounterBufferOffsets )
	{
		return g_library.getSelectedDesc().CmdBeginTransformFeedbackEXT( commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets );
	}

	Ashes_API void VKAPI_CALL vkCmdEndTransformFeedbackEXT( VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const  VkBuffer* pCounterBuffers, const  VkDeviceSize* pCounterBufferOffsets )
	{
		return g_library.getSelectedDesc().CmdEndTransformFeedbackEXT( commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginQueryIndexedEXT( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index )
	{
		return g_library.getSelectedDesc().CmdBeginQueryIndexedEXT( commandBuffer, queryPool, query, flags, index );
	}

	Ashes_API void VKAPI_CALL vkCmdEndQueryIndexedEXT( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index )
	{
		return g_library.getSelectedDesc().CmdEndQueryIndexedEXT( commandBuffer, queryPool, query, index );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndirectByteCountEXT( VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride )
	{
		return g_library.getSelectedDesc().CmdDrawIndirectByteCountEXT( commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride );
	}

#endif
#pragma endregion
#pragma region VK_AMD_draw_indirect_count
#ifdef VK_AMD_draw_indirect_count

	Ashes_API void VKAPI_CALL vkCmdDrawIndirectCountAMD( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawIndirectCountAMD( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndexedIndirectCountAMD( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawIndexedIndirectCountAMD( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
	}

#endif
#pragma endregion
#pragma region VK_AMD_shader_info
#ifdef VK_AMD_shader_info

	Ashes_API VkResult VKAPI_CALL vkGetShaderInfoAMD( VkDevice device, VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo )
	{
		return g_library.getSelectedDesc().GetShaderInfoAMD( device, pipeline, shaderStage, infoType, pInfoSize, pInfo );
	}

#endif
#pragma endregion
#pragma region VK_NV_external_memory_capabilities
#ifdef VK_NV_external_memory_capabilities

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceExternalImageFormatPropertiesNV( VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceExternalImageFormatPropertiesNV( physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties );
	}

#endif
#pragma endregion
#pragma region VK_EXT_conditional_rendering
#ifdef VK_EXT_conditional_rendering

	Ashes_API void VKAPI_CALL vkCmdBeginConditionalRenderingEXT( VkCommandBuffer commandBuffer, const  VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin )
	{
		return g_library.getSelectedDesc().CmdBeginConditionalRenderingEXT( commandBuffer, pConditionalRenderingBegin );
	}

	Ashes_API void VKAPI_CALL vkCmdEndConditionalRenderingEXT( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().CmdEndConditionalRenderingEXT( commandBuffer );
	}

#endif
#pragma endregion
#pragma region VK_NVX_device_generated_commands
#ifdef VK_NVX_device_generated_commands

	Ashes_API void VKAPI_CALL vkCmdProcessCommandsNVX( VkCommandBuffer commandBuffer, const  VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo )
	{
		return g_library.getSelectedDesc().CmdProcessCommandsNVX( commandBuffer, pProcessCommandsInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdReserveSpaceForCommandsNVX( VkCommandBuffer commandBuffer, const  VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo )
	{
		return g_library.getSelectedDesc().CmdReserveSpaceForCommandsNVX( commandBuffer, pReserveSpaceInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateIndirectCommandsLayoutNVX( VkDevice device, const  VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout )
	{
		return g_library.getSelectedDesc().CreateIndirectCommandsLayoutNVX( device, pCreateInfo, pAllocator, pIndirectCommandsLayout );
	}

	Ashes_API void VKAPI_CALL vkDestroyIndirectCommandsLayoutNVX( VkDevice device, VkIndirectCommandsLayoutNVX indirectCommandsLayout, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyIndirectCommandsLayoutNVX( device, indirectCommandsLayout, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateObjectTableNVX( VkDevice device, const  VkObjectTableCreateInfoNVX* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable )
	{
		return g_library.getSelectedDesc().CreateObjectTableNVX( device, pCreateInfo, pAllocator, pObjectTable );
	}

	Ashes_API void VKAPI_CALL vkDestroyObjectTableNVX( VkDevice device, VkObjectTableNVX objectTable, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyObjectTableNVX( device, objectTable, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkRegisterObjectsNVX( VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices )
	{
		return g_library.getSelectedDesc().RegisterObjectsNVX( device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices );
	}

	Ashes_API VkResult VKAPI_CALL vkUnregisterObjectsNVX( VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount, const  VkObjectEntryTypeNVX* pObjectEntryTypes, const  uint32_t* pObjectIndices )
	{
		return g_library.getSelectedDesc().UnregisterObjectsNVX( device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX( VkPhysicalDevice physicalDevice, VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX*  pLimits )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceGeneratedCommandsPropertiesNVX( physicalDevice, pFeatures, pLimits );
	}

#endif
#pragma endregion
#pragma region VK_NV_clip_space_w_scaling
#ifdef VK_NV_clip_space_w_scaling

	Ashes_API void VKAPI_CALL vkCmdSetViewportWScalingNV( VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const  VkViewportWScalingNV* pViewportWScalings )
	{
		return g_library.getSelectedDesc().CmdSetViewportWScalingNV( commandBuffer, firstViewport, viewportCount, pViewportWScalings );
	}

#endif
#pragma endregion
#pragma region VK_EXT_direct_mode_display
#ifdef VK_EXT_direct_mode_display

	Ashes_API VkResult VKAPI_CALL vkReleaseDisplayEXT( VkPhysicalDevice physicalDevice, VkDisplayKHR display )
	{
		return g_library.getSelectedDesc().ReleaseDisplayEXT( physicalDevice, display );
	}

#endif
#pragma endregion
#pragma region VK_EXT_display_surface_counter
#ifdef VK_EXT_display_surface_counter

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilities2EXT( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT*  pSurfaceCapabilities )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceSurfaceCapabilities2EXT( physicalDevice, surface, pSurfaceCapabilities );
	}

#endif
#pragma endregion
#pragma region VK_EXT_display_control
#ifdef VK_EXT_display_control

	Ashes_API VkResult VKAPI_CALL vkDisplayPowerControlEXT( VkDevice device, VkDisplayKHR display, const  VkDisplayPowerInfoEXT* pDisplayPowerInfo )
	{
		return g_library.getSelectedDesc().DisplayPowerControlEXT( device, display, pDisplayPowerInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkRegisterDeviceEventEXT( VkDevice device, const  VkDeviceEventInfoEXT* pDeviceEventInfo, const  VkAllocationCallbacks* pAllocator, VkFence* pFence )
	{
		return g_library.getSelectedDesc().RegisterDeviceEventEXT( device, pDeviceEventInfo, pAllocator, pFence );
	}

	Ashes_API VkResult VKAPI_CALL vkRegisterDisplayEventEXT( VkDevice device, VkDisplayKHR display, const  VkDisplayEventInfoEXT* pDisplayEventInfo, const  VkAllocationCallbacks* pAllocator, VkFence* pFence )
	{
		return g_library.getSelectedDesc().RegisterDisplayEventEXT( device, display, pDisplayEventInfo, pAllocator, pFence );
	}

	Ashes_API VkResult VKAPI_CALL vkGetSwapchainCounterEXT( VkDevice device, VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue )
	{
		return g_library.getSelectedDesc().GetSwapchainCounterEXT( device, swapchain, counter, pCounterValue );
	}

#endif
#pragma endregion
#pragma region VK_GOOGLE_display_timing
#ifdef VK_GOOGLE_display_timing

	Ashes_API VkResult VKAPI_CALL vkGetRefreshCycleDurationGOOGLE( VkDevice device, VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE*  pDisplayTimingProperties )
	{
		return g_library.getSelectedDesc().GetRefreshCycleDurationGOOGLE( device, swapchain, pDisplayTimingProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPastPresentationTimingGOOGLE( VkDevice device, VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings )
	{
		return g_library.getSelectedDesc().GetPastPresentationTimingGOOGLE( device, swapchain, pPresentationTimingCount, pPresentationTimings );
	}

#endif
#pragma endregion
#pragma region VK_EXT_discard_rectangles
#ifdef VK_EXT_discard_rectangles

	Ashes_API void VKAPI_CALL vkCmdSetDiscardRectangleEXT( VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const  VkRect2D* pDiscardRectangles )
	{
		return g_library.getSelectedDesc().CmdSetDiscardRectangleEXT( commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles );
	}

#endif
#pragma endregion
#pragma region VK_EXT_hdr_metadata
#ifdef VK_EXT_hdr_metadata

	Ashes_API void VKAPI_CALL vkSetHdrMetadataEXT( VkDevice device, uint32_t swapchainCount, const  VkSwapchainKHR* pSwapchains, const  VkHdrMetadataEXT* pMetadata )
	{
		return g_library.getSelectedDesc().SetHdrMetadataEXT( device, swapchainCount, pSwapchains, pMetadata );
	}

#endif
#pragma endregion
#pragma region VK_EXT_debug_utils
#ifdef VK_EXT_debug_utils

	Ashes_API VkResult VKAPI_CALL vkSetDebugUtilsObjectNameEXT( VkDevice device, const  VkDebugUtilsObjectNameInfoEXT* pNameInfo )
	{
		return g_library.getSelectedDesc().SetDebugUtilsObjectNameEXT( device, pNameInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkSetDebugUtilsObjectTagEXT( VkDevice device, const  VkDebugUtilsObjectTagInfoEXT* pTagInfo )
	{
		return g_library.getSelectedDesc().SetDebugUtilsObjectTagEXT( device, pTagInfo );
	}

	Ashes_API void VKAPI_CALL vkQueueBeginDebugUtilsLabelEXT( VkQueue queue, const  VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return g_library.getSelectedDesc().QueueBeginDebugUtilsLabelEXT( queue, pLabelInfo );
	}

	Ashes_API void VKAPI_CALL vkQueueEndDebugUtilsLabelEXT( VkQueue queue )
	{
		return g_library.getSelectedDesc().QueueEndDebugUtilsLabelEXT( queue );
	}

	Ashes_API void VKAPI_CALL vkQueueInsertDebugUtilsLabelEXT( VkQueue queue, const  VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return g_library.getSelectedDesc().QueueInsertDebugUtilsLabelEXT( queue, pLabelInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT( VkCommandBuffer commandBuffer, const  VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return g_library.getSelectedDesc().CmdBeginDebugUtilsLabelEXT( commandBuffer, pLabelInfo );
	}

	Ashes_API void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().CmdEndDebugUtilsLabelEXT( commandBuffer );
	}

	Ashes_API void VKAPI_CALL vkCmdInsertDebugUtilsLabelEXT( VkCommandBuffer commandBuffer, const  VkDebugUtilsLabelEXT* pLabelInfo )
	{
		return g_library.getSelectedDesc().CmdInsertDebugUtilsLabelEXT( commandBuffer, pLabelInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance instance, const  VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger )
	{
		return g_library.getSelectedDesc().CreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pMessenger );
	}

	Ashes_API void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT messenger, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyDebugUtilsMessengerEXT( instance, messenger, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkSubmitDebugUtilsMessageEXT( VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const  VkDebugUtilsMessengerCallbackDataEXT* pCallbackData )
	{
		return g_library.getSelectedDesc().SubmitDebugUtilsMessageEXT( instance, messageSeverity, messageTypes, pCallbackData );
	}

#endif
#pragma endregion
#pragma region VK_EXT_sample_locations
#ifdef VK_EXT_sample_locations

	Ashes_API void VKAPI_CALL vkCmdSetSampleLocationsEXT( VkCommandBuffer commandBuffer, const  VkSampleLocationsInfoEXT* pSampleLocationsInfo )
	{
		return g_library.getSelectedDesc().CmdSetSampleLocationsEXT( commandBuffer, pSampleLocationsInfo );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceMultisamplePropertiesEXT( VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT*  pMultisampleProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceMultisamplePropertiesEXT( physicalDevice, samples, pMultisampleProperties );
	}

#endif
#pragma endregion
#pragma region VK_EXT_image_drm_format_modifier
#ifdef VK_EXT_image_drm_format_modifier

	Ashes_API VkResult VKAPI_CALL vkGetImageDrmFormatModifierPropertiesEXT( VkDevice device, VkImage image, VkImageDrmFormatModifierPropertiesEXT*  pProperties )
	{
		return g_library.getSelectedDesc().GetImageDrmFormatModifierPropertiesEXT( device, image, pProperties );
	}

#endif
#pragma endregion
#pragma region VK_EXT_validation_cache
#ifdef VK_EXT_validation_cache

	Ashes_API VkResult VKAPI_CALL vkCreateValidationCacheEXT( VkDevice device, const  VkValidationCacheCreateInfoEXT* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkValidationCacheEXT* pValidationCache )
	{
		return g_library.getSelectedDesc().CreateValidationCacheEXT( device, pCreateInfo, pAllocator, pValidationCache );
	}

	Ashes_API void VKAPI_CALL vkDestroyValidationCacheEXT( VkDevice device, VkValidationCacheEXT validationCache, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyValidationCacheEXT( device, validationCache, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkMergeValidationCachesEXT( VkDevice device, VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const  VkValidationCacheEXT* pSrcCaches )
	{
		return g_library.getSelectedDesc().MergeValidationCachesEXT( device, dstCache, srcCacheCount, pSrcCaches );
	}

	Ashes_API VkResult VKAPI_CALL vkGetValidationCacheDataEXT( VkDevice device, VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData )
	{
		return g_library.getSelectedDesc().GetValidationCacheDataEXT( device, validationCache, pDataSize, pData );
	}

#endif
#pragma endregion
#pragma region VK_NV_shading_rate_image
#ifdef VK_NV_shading_rate_image

	Ashes_API void VKAPI_CALL vkCmdBindShadingRateImageNV( VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout  imageLayout )
	{
		return g_library.getSelectedDesc().CmdBindShadingRateImageNV( commandBuffer, imageView, imageLayout );
	}

	Ashes_API void VKAPI_CALL vkCmdSetViewportShadingRatePaletteNV( VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const  VkShadingRatePaletteNV* pShadingRatePalettes )
	{
		return g_library.getSelectedDesc().CmdSetViewportShadingRatePaletteNV( commandBuffer, firstViewport, viewportCount, pShadingRatePalettes );
	}

	Ashes_API void VKAPI_CALL vkCmdSetCoarseSampleOrderNV( VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const  VkCoarseSampleOrderCustomNV* pCustomSampleOrders )
	{
		return g_library.getSelectedDesc().CmdSetCoarseSampleOrderNV( commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders );
	}

#endif
#pragma endregion
#pragma region VK_NV_ray_tracing
#ifdef VK_NV_ray_tracing

	Ashes_API VkResult VKAPI_CALL vkCreateAccelerationStructureNV( VkDevice device, const  VkAccelerationStructureCreateInfoNV* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkAccelerationStructureNV* pAccelerationStructure )
	{
		return g_library.getSelectedDesc().CreateAccelerationStructureNV( device, pCreateInfo, pAllocator, pAccelerationStructure );
	}

	Ashes_API void VKAPI_CALL vkDestroyAccelerationStructureNV( VkDevice device, VkAccelerationStructureNV accelerationStructure, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().DestroyAccelerationStructureNV( device, accelerationStructure, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkGetAccelerationStructureMemoryRequirementsNV( VkDevice device, const  VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().GetAccelerationStructureMemoryRequirementsNV( device, pInfo, pMemoryRequirements );
	}

	Ashes_API VkResult VKAPI_CALL vkBindAccelerationStructureMemoryNV( VkDevice device, uint32_t bindInfoCount, const  VkBindAccelerationStructureMemoryInfoNV* pBindInfos )
	{
		return g_library.getSelectedDesc().BindAccelerationStructureMemoryNV( device, bindInfoCount, pBindInfos );
	}

	Ashes_API void VKAPI_CALL vkCmdBuildAccelerationStructureNV( VkCommandBuffer commandBuffer, const  VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset )
	{
		return g_library.getSelectedDesc().CmdBuildAccelerationStructureNV( commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyAccelerationStructureNV( VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeNV mode )
	{
		return g_library.getSelectedDesc().CmdCopyAccelerationStructureNV( commandBuffer, dst, src, mode );
	}

	Ashes_API void VKAPI_CALL vkCmdTraceRaysNV( VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth )
	{
		g_library.getSelectedDesc().CmdTraceRaysNV( commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateRayTracingPipelinesNV( VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const  VkRayTracingPipelineCreateInfoNV* pCreateInfos, const  VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines )
	{
		return g_library.getSelectedDesc().CreateRayTracingPipelinesNV( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
	}

	Ashes_API VkResult VKAPI_CALL vkGetRayTracingShaderGroupHandlesNV( VkDevice device, VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData )
	{
		return g_library.getSelectedDesc().GetRayTracingShaderGroupHandlesNV( device, pipeline, firstGroup, groupCount, dataSize, pData );
	}

	Ashes_API VkResult VKAPI_CALL vkGetAccelerationStructureHandleNV( VkDevice device, VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData )
	{
		return g_library.getSelectedDesc().GetAccelerationStructureHandleNV( device, accelerationStructure, dataSize, pData );
	}

	Ashes_API void VKAPI_CALL vkCmdWriteAccelerationStructuresPropertiesNV( VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const  VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery )
	{
		return g_library.getSelectedDesc().CmdWriteAccelerationStructuresPropertiesNV( commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery );
	}

	Ashes_API VkResult VKAPI_CALL vkCompileDeferredNV( VkDevice device, VkPipeline pipeline, uint32_t  shader )
	{
		return g_library.getSelectedDesc().CompileDeferredNV( device, pipeline, shader );
	}

#endif
#pragma endregion
#pragma region VK_EXT_external_memory_host
#ifdef VK_EXT_external_memory_host

	Ashes_API VkResult VKAPI_CALL vkGetMemoryHostPointerPropertiesEXT( VkDevice device, VkExternalMemoryHandleTypeFlagBits handleType, const  void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties )
	{
		return g_library.getSelectedDesc().GetMemoryHostPointerPropertiesEXT( device, handleType, pHostPointer, pMemoryHostPointerProperties );
	}

#endif
#pragma endregion
#pragma region VK_AMD_buffer_marker
#ifdef VK_AMD_buffer_marker

	Ashes_API void VKAPI_CALL vkCmdWriteBufferMarkerAMD( VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker )
	{
		return g_library.getSelectedDesc().CmdWriteBufferMarkerAMD( commandBuffer, pipelineStage, dstBuffer, dstOffset, marker );
	}

#endif
#pragma endregion
#pragma region VK_EXT_calibrated_timestamps
#ifdef VK_EXT_calibrated_timestamps

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceCalibrateableTimeDomainsEXT( VkPhysicalDevice physicalDevice, uint32_t* pTimeDomainCount, VkTimeDomainEXT*  pTimeDomains )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceCalibrateableTimeDomainsEXT( physicalDevice, pTimeDomainCount, pTimeDomains );
	}

	Ashes_API VkResult VKAPI_CALL vkGetCalibratedTimestampsEXT( VkDevice device, uint32_t timestampCount, const  VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation )
	{
		return g_library.getSelectedDesc().GetCalibratedTimestampsEXT( device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation );
	}

#endif
#pragma endregion
#pragma region VK_NV_mesh_shader
#ifdef VK_NV_mesh_shader

	Ashes_API void VKAPI_CALL vkCmdDrawMeshTasksNV( VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t  firstTask )
	{
		return g_library.getSelectedDesc().CmdDrawMeshTasksNV( commandBuffer, taskCount, firstTask );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawMeshTasksIndirectNV( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawMeshTasksIndirectNV( commandBuffer, buffer, offset, drawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawMeshTasksIndirectCountNV( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().CmdDrawMeshTasksIndirectCountNV( commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride );
	}

#endif
#pragma endregion
#pragma region VK_NV_scissor_exclusive
#ifdef VK_NV_scissor_exclusive

	Ashes_API void VKAPI_CALL vkCmdSetExclusiveScissorNV( VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const  VkRect2D* pExclusiveScissors )
	{
		return g_library.getSelectedDesc().CmdSetExclusiveScissorNV( commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors );
	}

#endif
#pragma endregion
#pragma region VK_NV_device_diagnostic_checkpoints
#ifdef VK_NV_device_diagnostic_checkpoints

	Ashes_API void VKAPI_CALL vkCmdSetCheckpointNV( VkCommandBuffer commandBuffer, const  void* pCheckpointMarker )
	{
		return g_library.getSelectedDesc().CmdSetCheckpointNV( commandBuffer, pCheckpointMarker );
	}

	Ashes_API void VKAPI_CALL vkGetQueueCheckpointDataNV( VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV*  pCheckpointData )
	{
		return g_library.getSelectedDesc().GetQueueCheckpointDataNV( queue, pCheckpointDataCount, pCheckpointData );
	}

#endif
#pragma endregion
#pragma region VK_EXT_buffer_device_address
#ifdef VK_EXT_buffer_device_address

	Ashes_API VkDeviceAddress VKAPI_CALL vkGetBufferDeviceAddressEXT( VkDevice device, const  VkBufferDeviceAddressInfoEXT* pInfo )
	{
		return g_library.getSelectedDesc().GetBufferDeviceAddressEXT( device, pInfo );
	}

#endif
#pragma endregion
#pragma region VK_NV_cooperative_matrix
#ifdef VK_NV_cooperative_matrix

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceCooperativeMatrixPropertiesNV( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV*  pProperties )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceCooperativeMatrixPropertiesNV( physicalDevice, pPropertyCount, pProperties );
	}

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef VK_USE_PLATFORM_ANDROID_KHR

	Ashes_API VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR( VkInstance instance, const  VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateAndroidSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_FUCHSIA_imagepipe_surface
#	ifdef VK_USE_PLATFORM_FUCHSIA

	Ashes_API VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA( VkInstance instance, const  VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateImagePipeSurfaceFUCHSIA( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_MVK_ios_surface
#	ifdef VK_USE_PLATFORM_IOS_MVK

	Ashes_API VkResult VKAPI_CALL vkCreateIOSSurfaceMVK( VkInstance instance, const  VkIOSSurfaceCreateInfoMVK* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateIOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#ifdef VK_MVK_macos_surface
#	ifdef VK_USE_PLATFORM_MACOS_MVK

	Ashes_API VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK( VkInstance instance, const  VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateMacOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_NN_vi_surface
#	ifdef VK_USE_PLATFORM_VI_NN

	Ashes_API VkResult VKAPI_CALL vkCreateViSurfaceNN( VkInstance instance, const  VkViSurfaceCreateInfoNN* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateViSurfaceNN( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#ifdef VK_KHR_xcb_surface
#	ifdef VK_USE_PLATFORM_XCB_KHR

	Ashes_API VkResult VKAPI_CALL vkCreateXcbSurfaceKHR( VkInstance instance, const  VkXcbSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateXcbSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#ifdef VK_KHR_xlib_surface
#	ifdef VK_USE_PLATFORM_XLIB_KHR

	Ashes_API VkResult VKAPI_CALL vkCreateXlibSurfaceKHR( VkInstance instance, const  VkXlibSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateXlibSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#ifdef VK_KHR_wayland_surface
#	ifdef VK_USE_PLATFORM_WAYLAND_KHR

	Ashes_API VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR( VkInstance instance, const  VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateWaylandSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_KHR_win32_surface
#	ifdef VK_USE_PLATFORM_WIN32_KHR

	Ashes_API VkResult VKAPI_CALL vkCreateWin32SurfaceKHR( VkInstance instance, const  VkWin32SurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().CreateWin32SurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex )
	{
		return g_library.getSelectedDesc().GetPhysicalDeviceWin32PresentationSupportKHR( physicalDevice, queueFamilyIndex );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_EXT_acquire_xlib_display
#ifdef VK_EXT_acquire_xlib_display
#	ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

	Ashes_API VkResult VKAPI_CALL vkAcquireXlibDisplayEXT( VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR  display )
	{
		return g_library.getSelectedDesc().AcquireXlibDisplayEXT( physicalDevice, dpy, display );
	}

	Ashes_API VkResult VKAPI_CALL vkGetRandROutputDisplayEXT( VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay )
	{
		return g_library.getSelectedDesc().GetRandROutputDisplayEXT( physicalDevice, dpy, rrOutput, pDisplay );
	}

#	endif
#endif
#pragma endregion
}
