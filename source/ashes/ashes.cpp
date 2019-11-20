#include "ashes_plugin.hpp"

#include <algorithm>

PluginLibrary g_library;

namespace details
{
	namespace
	{
		bool endsWith( std::string const & value
			, std::string const & lookup )
		{
			auto it = value.find( lookup );
			bool result = it != std::string::npos;

			if ( result )
			{
				result = ( it + lookup.size() ) == value.size();
			}

			return result;
		}

		std::string const & getSharedLibExt()
		{
#if defined( NDEBUG )
#	if defined( _WIN32 )
			static std::string result{ R"(.dll)" };
#	else
			static std::string result{ R"(.so)" };
#	endif
#else
#	if defined( _WIN32 )
			static std::string result{ R"(d.dll)" };
#	else
			static std::string result{ R"(d.so)" };
#	endif
#endif
			return result;
		}

		bool isAshesPlugin( std::string const & filePath )
		{
			return endsWith( filePath, "Renderer" + getSharedLibExt() );
		}

		uint32_t getPriority( Plugin const & plugin )
		{
			return plugin.description.support.priority;
		}
	}

	Plugin * findFirstSupportedPlugin( PluginArray & plugins )
	{
		Plugin * result{ nullptr };

		for ( auto & lookup : plugins )
		{
			if ( lookup.description.support.supported )
			{
				if ( !result
					|| getPriority( lookup ) > getPriority( *result ) )
				{
					result = &lookup;
				}
			}
		}

		return result;
	}

	PluginArray listPlugins()
	{
		PluginArray result;
		ashes::StringArray files;

		if ( ashes::listDirectoryFiles( ashes::getExecutableDirectory(), files, false ) )
		{
			for ( auto & file : files )
			{
				if ( isAshesPlugin( file ) )
				{
					try
					{
						result.emplace_back( std::make_unique< ashes::DynamicLibrary >( file ) );
					}
					catch ( std::exception & exc )
					{
						// Prevent useless noisy message
						std::clog << exc.what() << std::endl;
					}
				}
			}
		}

		return result;
	}
}

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

	Ashes_API void VKAPI_PTR ashEnumeratePluginsDescriptions( uint32_t * count
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

	Ashes_API VkResult VKAPI_PTR ashSelectPlugin( AshPluginDescription description )
	{
		auto result = g_library.init();

		if ( result == VK_SUCCESS )
		{
			result = g_library.selectDesc( description );
		}

		return result;
	}

	Ashes_API VkResult VKAPI_PTR ashGetCurrentPluginDescription( AshPluginDescription * description )
	{
		auto result = g_library.init();

		if ( result == VK_SUCCESS )
		{
			*description = g_library.getSelectedDesc();
		}

		return result;
	}

#pragma region Vulkan 1.0
#ifdef VK_VERSION_1_0
	Ashes_API PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr( VkDevice device, const char* pName )
	{
		return g_library.getSelectedDesc().functions.GetDeviceProcAddr( device, pName );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateInstance( const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance )
	{
		return g_library.getSelectedDesc().functions.CreateInstance( pCreateInfo, pAllocator, pInstance );
	}

	Ashes_API void VKAPI_CALL vkDestroyInstance( VkInstance instance, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyInstance( instance, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumeratePhysicalDevices( VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice*  pPhysicalDevices )
	{
		return g_library.getSelectedDesc().functions.EnumeratePhysicalDevices( instance, pPhysicalDeviceCount, pPhysicalDevices );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFeatures( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceFeatures( physicalDevice, pFeatures );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties*  pFormatProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceFormatProperties( physicalDevice, format, pFormatProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceImageFormatProperties( physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceProperties( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceProperties( physicalDevice, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties( VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties*  pQueueFamilyProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceQueueFamilyProperties( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties( VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceMemoryProperties( physicalDevice, pMemoryProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDevice( VkPhysicalDevice physicalDevice, const  VkDeviceCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDevice* pDevice )
	{
		return g_library.getSelectedDesc().functions.CreateDevice( physicalDevice, pCreateInfo, pAllocator, pDevice );
	}

	Ashes_API void VKAPI_CALL vkDestroyDevice( VkDevice device, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyDevice( device, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties( const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties )
	{
		return g_library.getSelectedDesc().functions.EnumerateInstanceExtensionProperties( pLayerName, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateDeviceExtensionProperties( VkPhysicalDevice physicalDevice, const  char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties )
	{
		return g_library.getSelectedDesc().functions.EnumerateDeviceExtensionProperties( physicalDevice, pLayerName, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties( uint32_t* pPropertyCount, VkLayerProperties* pProperties )
	{
		return g_library.getSelectedDesc().functions.EnumerateInstanceLayerProperties( pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties( VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkLayerProperties*  pProperties )
	{
		return g_library.getSelectedDesc().functions.EnumerateDeviceLayerProperties( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceQueue( VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue )
	{
		return g_library.getSelectedDesc().functions.GetDeviceQueue( device, queueFamilyIndex, queueIndex, pQueue );
	}

	Ashes_API VkResult VKAPI_CALL vkQueueSubmit( VkQueue queue, uint32_t submitCount, const  VkSubmitInfo* pSubmits, VkFence fence )
	{
		return g_library.getSelectedDesc().functions.QueueSubmit( queue, submitCount, pSubmits, fence );
	}

	Ashes_API VkResult VKAPI_CALL vkQueueWaitIdle( VkQueue queue )
	{
		return g_library.getSelectedDesc().functions.QueueWaitIdle( queue );
	}

	Ashes_API VkResult VKAPI_CALL vkDeviceWaitIdle( VkDevice device )
	{
		return g_library.getSelectedDesc().functions.DeviceWaitIdle( device );
	}

	Ashes_API VkResult VKAPI_CALL vkAllocateMemory( VkDevice device, const  VkMemoryAllocateInfo* pAllocateInfo, const  VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory )
	{
		return g_library.getSelectedDesc().functions.AllocateMemory( device, pAllocateInfo, pAllocator, pMemory );
	}

	Ashes_API void VKAPI_CALL vkFreeMemory( VkDevice device, VkDeviceMemory memory, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.FreeMemory( device, memory, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkMapMemory( VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData )
	{
		return g_library.getSelectedDesc().functions.MapMemory( device, memory, offset, size, flags, ppData );
	}

	Ashes_API void VKAPI_CALL vkUnmapMemory( VkDevice device, VkDeviceMemory memory )
	{
		return g_library.getSelectedDesc().functions.UnmapMemory( device, memory );
	}

	Ashes_API VkResult VKAPI_CALL vkFlushMappedMemoryRanges( VkDevice device, uint32_t memoryRangeCount, const  VkMappedMemoryRange* pMemoryRanges )
	{
		return g_library.getSelectedDesc().functions.FlushMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
	}

	Ashes_API VkResult VKAPI_CALL vkInvalidateMappedMemoryRanges( VkDevice device, uint32_t memoryRangeCount, const  VkMappedMemoryRange* pMemoryRanges )
	{
		return g_library.getSelectedDesc().functions.InvalidateMappedMemoryRanges( device, memoryRangeCount, pMemoryRanges );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceMemoryCommitment( VkDevice device, VkDeviceMemory memory, VkDeviceSize*  pCommittedMemoryInBytes )
	{
		return g_library.getSelectedDesc().functions.GetDeviceMemoryCommitment( device, memory, pCommittedMemoryInBytes );
	}

	Ashes_API VkResult VKAPI_CALL vkBindBufferMemory( VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset )
	{
		return g_library.getSelectedDesc().functions.BindBufferMemory( device, buffer, memory, memoryOffset );
	}

	Ashes_API VkResult VKAPI_CALL vkBindImageMemory( VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset )
	{
		return g_library.getSelectedDesc().functions.BindImageMemory( device, image, memory, memoryOffset );
	}

	Ashes_API void VKAPI_CALL vkGetBufferMemoryRequirements( VkDevice device, VkBuffer buffer, VkMemoryRequirements*  pMemoryRequirements )
	{
		return g_library.getSelectedDesc().functions.GetBufferMemoryRequirements( device, buffer, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageMemoryRequirements( VkDevice device, VkImage image, VkMemoryRequirements*  pMemoryRequirements )
	{
		return g_library.getSelectedDesc().functions.GetImageMemoryRequirements( device, image, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageSparseMemoryRequirements( VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements )
	{
		return g_library.getSelectedDesc().functions.GetImageSparseMemoryRequirements( device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties( VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceSparseImageFormatProperties( physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkQueueBindSparse( VkQueue queue, uint32_t bindInfoCount, const  VkBindSparseInfo* pBindInfo, VkFence fence )
	{
		return g_library.getSelectedDesc().functions.QueueBindSparse( queue, bindInfoCount, pBindInfo, fence );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateFence( VkDevice device, const  VkFenceCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkFence* pFence )
	{
		return g_library.getSelectedDesc().functions.CreateFence( device, pCreateInfo, pAllocator, pFence );
	}

	Ashes_API void VKAPI_CALL vkDestroyFence( VkDevice device, VkFence fence, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyFence( device, fence, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkResetFences( VkDevice device, uint32_t fenceCount, const  VkFence* pFences )
	{
		return g_library.getSelectedDesc().functions.ResetFences( device, fenceCount, pFences );
	}

	Ashes_API VkResult VKAPI_CALL vkGetFenceStatus( VkDevice device, VkFence fence )
	{
		return g_library.getSelectedDesc().functions.GetFenceStatus( device, fence );
	}

	Ashes_API VkResult VKAPI_CALL vkWaitForFences( VkDevice device, uint32_t fenceCount, const  VkFence* pFences, VkBool32 waitAll, uint64_t timeout )
	{
		return g_library.getSelectedDesc().functions.WaitForFences( device, fenceCount, pFences, waitAll, timeout );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateSemaphore( VkDevice device, const  VkSemaphoreCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore )
	{
		return g_library.getSelectedDesc().functions.CreateSemaphore( device, pCreateInfo, pAllocator, pSemaphore );
	}

	Ashes_API void VKAPI_CALL vkDestroySemaphore( VkDevice device, VkSemaphore semaphore, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroySemaphore( device, semaphore, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateEvent( VkDevice device, const  VkEventCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkEvent* pEvent )
	{
		return g_library.getSelectedDesc().functions.CreateEvent( device, pCreateInfo, pAllocator, pEvent );
	}

	Ashes_API void VKAPI_CALL vkDestroyEvent( VkDevice device, VkEvent event, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyEvent( device, event, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetEventStatus( VkDevice device, VkEvent event )
	{
		return g_library.getSelectedDesc().functions.GetEventStatus( device, event );
	}

	Ashes_API VkResult VKAPI_CALL vkSetEvent( VkDevice device, VkEvent event )
	{
		return g_library.getSelectedDesc().functions.SetEvent( device, event );
	}

	Ashes_API VkResult VKAPI_CALL vkResetEvent( VkDevice device, VkEvent event )
	{
		return g_library.getSelectedDesc().functions.ResetEvent( device, event );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateQueryPool( VkDevice device, const  VkQueryPoolCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool )
	{
		return g_library.getSelectedDesc().functions.CreateQueryPool( device, pCreateInfo, pAllocator, pQueryPool );
	}

	Ashes_API void VKAPI_CALL vkDestroyQueryPool( VkDevice device, VkQueryPool queryPool, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyQueryPool( device, queryPool, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetQueryPoolResults( VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags )
	{
		return g_library.getSelectedDesc().functions.GetQueryPoolResults( device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateBuffer( VkDevice device, const  VkBufferCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer )
	{
		return g_library.getSelectedDesc().functions.CreateBuffer( device, pCreateInfo, pAllocator, pBuffer );
	}

	Ashes_API void VKAPI_CALL vkDestroyBuffer( VkDevice device, VkBuffer buffer, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyBuffer( device, buffer, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateBufferView( VkDevice device, const  VkBufferViewCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkBufferView* pView )
	{
		return g_library.getSelectedDesc().functions.CreateBufferView( device, pCreateInfo, pAllocator, pView );
	}

	Ashes_API void VKAPI_CALL vkDestroyBufferView( VkDevice device, VkBufferView bufferView, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyBufferView( device, bufferView, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateImage( VkDevice device, const  VkImageCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkImage* pImage )
	{
		return g_library.getSelectedDesc().functions.CreateImage( device, pCreateInfo, pAllocator, pImage );
	}

	Ashes_API void VKAPI_CALL vkDestroyImage( VkDevice device, VkImage image, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyImage( device, image, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkGetImageSubresourceLayout( VkDevice device, VkImage image, const  VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout )
	{
		return g_library.getSelectedDesc().functions.GetImageSubresourceLayout( device, image, pSubresource, pLayout );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateImageView( VkDevice device, const  VkImageViewCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkImageView* pView )
	{
		return g_library.getSelectedDesc().functions.CreateImageView( device, pCreateInfo, pAllocator, pView );
	}

	Ashes_API void VKAPI_CALL vkDestroyImageView( VkDevice device, VkImageView imageView, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyImageView( device, imageView, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateShaderModule( VkDevice device, const  VkShaderModuleCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule )
	{
		return g_library.getSelectedDesc().functions.CreateShaderModule( device, pCreateInfo, pAllocator, pShaderModule );
	}

	Ashes_API void VKAPI_CALL vkDestroyShaderModule( VkDevice device, VkShaderModule shaderModule, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyShaderModule( device, shaderModule, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreatePipelineCache( VkDevice device, const  VkPipelineCacheCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache )
	{
		return g_library.getSelectedDesc().functions.CreatePipelineCache( device, pCreateInfo, pAllocator, pPipelineCache );
	}

	Ashes_API void VKAPI_CALL vkDestroyPipelineCache( VkDevice device, VkPipelineCache pipelineCache, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyPipelineCache( device, pipelineCache, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPipelineCacheData( VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData )
	{
		return g_library.getSelectedDesc().functions.GetPipelineCacheData( device, pipelineCache, pDataSize, pData );
	}

	Ashes_API VkResult VKAPI_CALL vkMergePipelineCaches( VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const  VkPipelineCache* pSrcCaches )
	{
		return g_library.getSelectedDesc().functions.MergePipelineCaches( device, dstCache, srcCacheCount, pSrcCaches );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateGraphicsPipelines( VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const  VkGraphicsPipelineCreateInfo* pCreateInfos, const  VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines )
	{
		return g_library.getSelectedDesc().functions.CreateGraphicsPipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateComputePipelines( VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const  VkComputePipelineCreateInfo* pCreateInfos, const  VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines )
	{
		return g_library.getSelectedDesc().functions.CreateComputePipelines( device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines );
	}

	Ashes_API void VKAPI_CALL vkDestroyPipeline( VkDevice device, VkPipeline pipeline, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyPipeline( device, pipeline, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreatePipelineLayout( VkDevice device, const  VkPipelineLayoutCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout )
	{
		return g_library.getSelectedDesc().functions.CreatePipelineLayout( device, pCreateInfo, pAllocator, pPipelineLayout );
	}

	Ashes_API void VKAPI_CALL vkDestroyPipelineLayout( VkDevice device, VkPipelineLayout pipelineLayout, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyPipelineLayout( device, pipelineLayout, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateSampler( VkDevice device, const  VkSamplerCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSampler* pSampler )
	{
		return g_library.getSelectedDesc().functions.CreateSampler( device, pCreateInfo, pAllocator, pSampler );
	}

	Ashes_API void VKAPI_CALL vkDestroySampler( VkDevice device, VkSampler sampler, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroySampler( device, sampler, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorSetLayout( VkDevice device, const  VkDescriptorSetLayoutCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout )
	{
		return g_library.getSelectedDesc().functions.CreateDescriptorSetLayout( device, pCreateInfo, pAllocator, pSetLayout );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorSetLayout( VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyDescriptorSetLayout( device, descriptorSetLayout, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorPool( VkDevice device, const  VkDescriptorPoolCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool )
	{
		return g_library.getSelectedDesc().functions.CreateDescriptorPool( device, pCreateInfo, pAllocator, pDescriptorPool );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorPool( VkDevice device, VkDescriptorPool descriptorPool, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyDescriptorPool( device, descriptorPool, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkResetDescriptorPool( VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags  flags )
	{
		return g_library.getSelectedDesc().functions.ResetDescriptorPool( device, descriptorPool, flags );
	}

	Ashes_API VkResult VKAPI_CALL vkAllocateDescriptorSets( VkDevice device, const  VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets )
	{
		return g_library.getSelectedDesc().functions.AllocateDescriptorSets( device, pAllocateInfo, pDescriptorSets );
	}

	Ashes_API VkResult VKAPI_CALL vkFreeDescriptorSets( VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const  VkDescriptorSet* pDescriptorSets )
	{
		return g_library.getSelectedDesc().functions.FreeDescriptorSets( device, descriptorPool, descriptorSetCount, pDescriptorSets );
	}

	Ashes_API void VKAPI_CALL vkUpdateDescriptorSets( VkDevice device, uint32_t descriptorWriteCount, const  VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const  VkCopyDescriptorSet* pDescriptorCopies )
	{
		return g_library.getSelectedDesc().functions.UpdateDescriptorSets( device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateFramebuffer( VkDevice device, const  VkFramebufferCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer )
	{
		return g_library.getSelectedDesc().functions.CreateFramebuffer( device, pCreateInfo, pAllocator, pFramebuffer );
	}

	Ashes_API void VKAPI_CALL vkDestroyFramebuffer( VkDevice device, VkFramebuffer framebuffer, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyFramebuffer( device, framebuffer, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateRenderPass( VkDevice device, const  VkRenderPassCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass )
	{
		return g_library.getSelectedDesc().functions.CreateRenderPass( device, pCreateInfo, pAllocator, pRenderPass );
	}

	Ashes_API void VKAPI_CALL vkDestroyRenderPass( VkDevice device, VkRenderPass renderPass, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyRenderPass( device, renderPass, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkGetRenderAreaGranularity( VkDevice device, VkRenderPass renderPass, VkExtent2D*  pGranularity )
	{
		return g_library.getSelectedDesc().functions.GetRenderAreaGranularity( device, renderPass, pGranularity );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateCommandPool( VkDevice device, const  VkCommandPoolCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool )
	{
		return g_library.getSelectedDesc().functions.CreateCommandPool( device, pCreateInfo, pAllocator, pCommandPool );
	}

	Ashes_API void VKAPI_CALL vkDestroyCommandPool( VkDevice device, VkCommandPool commandPool, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyCommandPool( device, commandPool, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkResetCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags  flags )
	{
		return g_library.getSelectedDesc().functions.ResetCommandPool( device, commandPool, flags );
	}

	Ashes_API VkResult VKAPI_CALL vkAllocateCommandBuffers( VkDevice device, const  VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers )
	{
		return g_library.getSelectedDesc().functions.AllocateCommandBuffers( device, pAllocateInfo, pCommandBuffers );
	}

	Ashes_API void VKAPI_CALL vkFreeCommandBuffers( VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const  VkCommandBuffer* pCommandBuffers )
	{
		return g_library.getSelectedDesc().functions.FreeCommandBuffers( device, commandPool, commandBufferCount, pCommandBuffers );
	}

	Ashes_API VkResult VKAPI_CALL vkBeginCommandBuffer( VkCommandBuffer commandBuffer, const  VkCommandBufferBeginInfo* pBeginInfo )
	{
		return g_library.getSelectedDesc().functions.BeginCommandBuffer( commandBuffer, pBeginInfo );
	}

	Ashes_API VkResult VKAPI_CALL vkEndCommandBuffer( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().functions.EndCommandBuffer( commandBuffer );
	}

	Ashes_API VkResult VKAPI_CALL vkResetCommandBuffer( VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags )
	{
		return g_library.getSelectedDesc().functions.ResetCommandBuffer( commandBuffer, flags );
	}

	Ashes_API void VKAPI_CALL vkCmdBindPipeline( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline  pipeline )
	{
		return g_library.getSelectedDesc().functions.CmdBindPipeline( commandBuffer, pipelineBindPoint, pipeline );
	}

	Ashes_API void VKAPI_CALL vkCmdSetViewport( VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const  VkViewport* pViewports )
	{
		return g_library.getSelectedDesc().functions.CmdSetViewport( commandBuffer, firstViewport, viewportCount, pViewports );
	}

	Ashes_API void VKAPI_CALL vkCmdSetScissor( VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const  VkRect2D* pScissors )
	{
		return g_library.getSelectedDesc().functions.CmdSetScissor( commandBuffer, firstScissor, scissorCount, pScissors );
	}

	Ashes_API void VKAPI_CALL vkCmdSetLineWidth( VkCommandBuffer commandBuffer, float lineWidth )
	{
		return g_library.getSelectedDesc().functions.CmdSetLineWidth( commandBuffer, lineWidth );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDepthBias( VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor )
	{
		return g_library.getSelectedDesc().functions.CmdSetDepthBias( commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor );
	}

	Ashes_API void VKAPI_CALL vkCmdSetBlendConstants( VkCommandBuffer commandBuffer, const  float blendConstants[4] )
	{
		return g_library.getSelectedDesc().functions.CmdSetBlendConstants( commandBuffer, blendConstants );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDepthBounds( VkCommandBuffer commandBuffer, float minDepthBounds, float  maxDepthBounds )
	{
		return g_library.getSelectedDesc().functions.CmdSetDepthBounds( commandBuffer, minDepthBounds, maxDepthBounds );
	}

	Ashes_API void VKAPI_CALL vkCmdSetStencilCompareMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t  compareMask )
	{
		return g_library.getSelectedDesc().functions.CmdSetStencilCompareMask( commandBuffer, faceMask, compareMask );
	}

	Ashes_API void VKAPI_CALL vkCmdSetStencilWriteMask( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t  writeMask )
	{
		return g_library.getSelectedDesc().functions.CmdSetStencilWriteMask( commandBuffer, faceMask, writeMask );
	}

	Ashes_API void VKAPI_CALL vkCmdSetStencilReference( VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t  reference )
	{
		return g_library.getSelectedDesc().functions.CmdSetStencilReference( commandBuffer, faceMask, reference );
	}

	Ashes_API void VKAPI_CALL vkCmdBindDescriptorSets( VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const  VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const  uint32_t* pDynamicOffsets )
	{
		return g_library.getSelectedDesc().functions.CmdBindDescriptorSets( commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets );
	}

	Ashes_API void VKAPI_CALL vkCmdBindIndexBuffer( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType )
	{
		return g_library.getSelectedDesc().functions.CmdBindIndexBuffer( commandBuffer, buffer, offset, indexType );
	}

	Ashes_API void VKAPI_CALL vkCmdBindVertexBuffers( VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const  VkBuffer* pBuffers, const  VkDeviceSize* pOffsets )
	{
		return g_library.getSelectedDesc().functions.CmdBindVertexBuffers( commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets );
	}

	Ashes_API void VKAPI_CALL vkCmdDraw( VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance )
	{
		return g_library.getSelectedDesc().functions.CmdDraw( commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndexed( VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance )
	{
		return g_library.getSelectedDesc().functions.CmdDrawIndexed( commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().functions.CmdDrawIndirect( commandBuffer, buffer, offset, drawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDrawIndexedIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride )
	{
		return g_library.getSelectedDesc().functions.CmdDrawIndexedIndirect( commandBuffer, buffer, offset, drawCount, stride );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatch( VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ )
	{
		return g_library.getSelectedDesc().functions.CmdDispatch( commandBuffer, groupCountX, groupCountY, groupCountZ );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatchIndirect( VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize  offset )
	{
		return g_library.getSelectedDesc().functions.CmdDispatchIndirect( commandBuffer, buffer, offset );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyBuffer( VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const  VkBufferCopy* pRegions )
	{
		return g_library.getSelectedDesc().functions.CmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyImage( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkImageCopy* pRegions )
	{
		return g_library.getSelectedDesc().functions.CmdCopyImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdBlitImage( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkImageBlit* pRegions, VkFilter filter )
	{
		return g_library.getSelectedDesc().functions.CmdBlitImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyBufferToImage( VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkBufferImageCopy* pRegions )
	{
		return g_library.getSelectedDesc().functions.CmdCopyBufferToImage( commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyImageToBuffer( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const  VkBufferImageCopy* pRegions )
	{
		return g_library.getSelectedDesc().functions.CmdCopyImageToBuffer( commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdUpdateBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const  void* pData )
	{
		return g_library.getSelectedDesc().functions.CmdUpdateBuffer( commandBuffer, dstBuffer, dstOffset, dataSize, pData );
	}

	Ashes_API void VKAPI_CALL vkCmdFillBuffer( VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data )
	{
		return g_library.getSelectedDesc().functions.CmdFillBuffer( commandBuffer, dstBuffer, dstOffset, size, data );
	}

	Ashes_API void VKAPI_CALL vkCmdClearColorImage( VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const  VkClearColorValue* pColor, uint32_t rangeCount, const  VkImageSubresourceRange* pRanges )
	{
		return g_library.getSelectedDesc().functions.CmdClearColorImage( commandBuffer, image, imageLayout, pColor, rangeCount, pRanges );
	}

	Ashes_API void VKAPI_CALL vkCmdClearDepthStencilImage( VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const  VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const  VkImageSubresourceRange* pRanges )
	{
		return g_library.getSelectedDesc().functions.CmdClearDepthStencilImage( commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges );
	}

	Ashes_API void VKAPI_CALL vkCmdClearAttachments( VkCommandBuffer commandBuffer, uint32_t attachmentCount, const  VkClearAttachment* pAttachments, uint32_t rectCount, const  VkClearRect* pRects )
	{
		return g_library.getSelectedDesc().functions.CmdClearAttachments( commandBuffer, attachmentCount, pAttachments, rectCount, pRects );
	}

	Ashes_API void VKAPI_CALL vkCmdResolveImage( VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const  VkImageResolve* pRegions )
	{
		return g_library.getSelectedDesc().functions.CmdResolveImage( commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions );
	}

	Ashes_API void VKAPI_CALL vkCmdSetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags  stageMask )
	{
		return g_library.getSelectedDesc().functions.CmdSetEvent( commandBuffer, event, stageMask );
	}

	Ashes_API void VKAPI_CALL vkCmdResetEvent( VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags  stageMask )
	{
		return g_library.getSelectedDesc().functions.CmdResetEvent( commandBuffer, event, stageMask );
	}

	Ashes_API void VKAPI_CALL vkCmdWaitEvents( VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers )
	{
		return g_library.getSelectedDesc().functions.CmdWaitEvents( commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers );
	}

	Ashes_API void VKAPI_CALL vkCmdPipelineBarrier( VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const  VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const  VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const  VkImageMemoryBarrier* pImageMemoryBarriers )
	{
		return g_library.getSelectedDesc().functions.CmdPipelineBarrier( commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags )
	{
		return g_library.getSelectedDesc().functions.CmdBeginQuery( commandBuffer, queryPool, query, flags );
	}

	Ashes_API void VKAPI_CALL vkCmdEndQuery( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t  query )
	{
		return g_library.getSelectedDesc().functions.CmdEndQuery( commandBuffer, queryPool, query );
	}

	Ashes_API void VKAPI_CALL vkCmdResetQueryPool( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount )
	{
		return g_library.getSelectedDesc().functions.CmdResetQueryPool( commandBuffer, queryPool, firstQuery, queryCount );
	}

	Ashes_API void VKAPI_CALL vkCmdWriteTimestamp( VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query )
	{
		return g_library.getSelectedDesc().functions.CmdWriteTimestamp( commandBuffer, pipelineStage, queryPool, query );
	}

	Ashes_API void VKAPI_CALL vkCmdCopyQueryPoolResults( VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags )
	{
		return g_library.getSelectedDesc().functions.CmdCopyQueryPoolResults( commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags );
	}

	Ashes_API void VKAPI_CALL vkCmdPushConstants( VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const  void* pValues )
	{
		return g_library.getSelectedDesc().functions.CmdPushConstants( commandBuffer, layout, stageFlags, offset, size, pValues );
	}

	Ashes_API void VKAPI_CALL vkCmdBeginRenderPass( VkCommandBuffer commandBuffer, const  VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents )
	{
		return g_library.getSelectedDesc().functions.CmdBeginRenderPass( commandBuffer, pRenderPassBegin, contents );
	}

	Ashes_API void VKAPI_CALL vkCmdNextSubpass( VkCommandBuffer commandBuffer, VkSubpassContents contents )
	{
		return g_library.getSelectedDesc().functions.CmdNextSubpass( commandBuffer, contents );
	}

	Ashes_API void VKAPI_CALL vkCmdEndRenderPass( VkCommandBuffer commandBuffer )
	{
		return g_library.getSelectedDesc().functions.CmdEndRenderPass( commandBuffer );
	}

	Ashes_API void VKAPI_CALL vkCmdExecuteCommands( VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const  VkCommandBuffer* pCommandBuffers )
	{
		return g_library.getSelectedDesc().functions.CmdExecuteCommands( commandBuffer, commandBufferCount, pCommandBuffers );
	}

#endif
#pragma endregion
#pragma region Vulkan 1.1
#ifdef VK_VERSION_1_1

	Ashes_API VkResult VKAPI_PTR vkEnumerateInstanceVersion( uint32_t * version )
	{
		return g_library.getSelectedDesc().functions.EnumerateInstanceVersion( version );
	}

	Ashes_API VkResult VKAPI_CALL vkBindBufferMemory2( VkDevice device, uint32_t bindInfoCount, const  VkBindBufferMemoryInfo* pBindInfos )
	{
		return g_library.getSelectedDesc().functions.BindBufferMemory2( device, bindInfoCount, pBindInfos );
	}

	Ashes_API VkResult VKAPI_CALL vkBindImageMemory2( VkDevice device, uint32_t bindInfoCount, const  VkBindImageMemoryInfo* pBindInfos )
	{
		return g_library.getSelectedDesc().functions.BindImageMemory2( device, bindInfoCount, pBindInfos );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures( VkDevice device, uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures )
	{
		return g_library.getSelectedDesc().functions.GetDeviceGroupPeerMemoryFeatures( device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures );
	}

	Ashes_API void VKAPI_CALL vkCmdSetDeviceMask( VkCommandBuffer commandBuffer, uint32_t deviceMask )
	{
		return g_library.getSelectedDesc().functions.CmdSetDeviceMask( commandBuffer, deviceMask );
	}

	Ashes_API void VKAPI_CALL vkCmdDispatchBase( VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ )
	{
		return g_library.getSelectedDesc().functions.CmdDispatchBase( commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ );
	}

	Ashes_API VkResult VKAPI_CALL vkEnumeratePhysicalDeviceGroups( VkInstance instance, uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties*  pPhysicalDeviceGroupProperties )
	{
		return g_library.getSelectedDesc().functions.EnumeratePhysicalDeviceGroups( instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties );
	}

	Ashes_API void VKAPI_CALL vkGetImageMemoryRequirements2( VkDevice device, const  VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().functions.GetImageMemoryRequirements2( device, pInfo, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetBufferMemoryRequirements2( VkDevice device, const  VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements )
	{
		return g_library.getSelectedDesc().functions.GetBufferMemoryRequirements2( device, pInfo, pMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetImageSparseMemoryRequirements2( VkDevice device, const  VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements )
	{
		return g_library.getSelectedDesc().functions.GetImageSparseMemoryRequirements2( device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFeatures2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceFeatures2( physicalDevice, pFeatures );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceProperties2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceProperties2( physicalDevice, pProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2( VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2*  pFormatProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceFormatProperties2( physicalDevice, format, pFormatProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceImageFormatProperties2( physicalDevice, pImageFormatInfo, pImageFormatProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2( VkPhysicalDevice physicalDevice, uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2*  pQueueFamilyProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceQueueFamilyProperties2( physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2( VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceMemoryProperties2( physicalDevice, pMemoryProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceSparseImageFormatProperties2( physicalDevice, pFormatInfo, pPropertyCount, pProperties );
	}

	Ashes_API void VKAPI_CALL vkTrimCommandPool( VkDevice device, VkCommandPool commandPool, VkCommandPoolTrimFlags  flags )
	{
		return g_library.getSelectedDesc().functions.TrimCommandPool( device, commandPool, flags );
	}

	Ashes_API void VKAPI_CALL vkGetDeviceQueue2( VkDevice device, const  VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue )
	{
		return g_library.getSelectedDesc().functions.GetDeviceQueue2( device, pQueueInfo, pQueue );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion( VkDevice device, const  VkSamplerYcbcrConversionCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSamplerYcbcrConversion* pYcbcrConversion )
	{
		return g_library.getSelectedDesc().functions.CreateSamplerYcbcrConversion( device, pCreateInfo, pAllocator, pYcbcrConversion );
	}

	Ashes_API void VKAPI_CALL vkDestroySamplerYcbcrConversion( VkDevice device, VkSamplerYcbcrConversion ycbcrConversion, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroySamplerYcbcrConversion( device, ycbcrConversion, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate( VkDevice device, const  VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate )
	{
		return g_library.getSelectedDesc().functions.CreateDescriptorUpdateTemplate( device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate );
	}

	Ashes_API void VKAPI_CALL vkDestroyDescriptorUpdateTemplate( VkDevice device, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroyDescriptorUpdateTemplate( device, descriptorUpdateTemplate, pAllocator );
	}

	Ashes_API void VKAPI_CALL vkUpdateDescriptorSetWithTemplate( VkDevice device, VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const  void* pData )
	{
		return g_library.getSelectedDesc().functions.UpdateDescriptorSetWithTemplate( device, descriptorSet, descriptorUpdateTemplate, pData );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceExternalBufferProperties( physicalDevice, pExternalBufferInfo, pExternalBufferProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceExternalFenceProperties( physicalDevice, pExternalFenceInfo, pExternalFenceProperties );
	}

	Ashes_API void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties( VkPhysicalDevice physicalDevice, const  VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceExternalSemaphoreProperties( physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties );
	}

	Ashes_API void VKAPI_CALL vkGetDescriptorSetLayoutSupport( VkDevice device, const  VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport )
	{
		return g_library.getSelectedDesc().functions.GetDescriptorSetLayoutSupport( device, pCreateInfo, pSupport );
	}

#endif
#pragma endregion
#pragma region VK_KHR_surface
#ifdef VK_KHR_surface

	Ashes_API void VKAPI_CALL vkDestroySurfaceKHR( VkInstance instance, VkSurfaceKHR surface, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroySurfaceKHR( instance, surface, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceSurfaceSupportKHR( physicalDevice, queueFamilyIndex, surface, pSupported );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceCapabilitiesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR*  pSurfaceCapabilities )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, pSurfaceCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfaceFormatsKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceSurfacePresentModesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, pPresentModeCount, pPresentModes );
	}

#endif
#pragma endregion
#pragma region VK_KHR_swapchain
#ifdef VK_KHR_swapchain
	Ashes_API VkResult VKAPI_CALL vkCreateSwapchainKHR( VkDevice device, const  VkSwapchainCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain )
	{
		return g_library.getSelectedDesc().functions.CreateSwapchainKHR( device, pCreateInfo, pAllocator, pSwapchain );
	}

	Ashes_API void VKAPI_CALL vkDestroySwapchainKHR( VkDevice device, VkSwapchainKHR swapchain, const  VkAllocationCallbacks* pAllocator )
	{
		return g_library.getSelectedDesc().functions.DestroySwapchainKHR( device, swapchain, pAllocator );
	}

	Ashes_API VkResult VKAPI_CALL vkGetSwapchainImagesKHR( VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages )
	{
		return g_library.getSelectedDesc().functions.GetSwapchainImagesKHR( device, swapchain, pSwapchainImageCount, pSwapchainImages );
	}

	Ashes_API VkResult VKAPI_CALL vkAcquireNextImageKHR( VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex )
	{
		return g_library.getSelectedDesc().functions.AcquireNextImageKHR( device, swapchain, timeout, semaphore, fence, pImageIndex );
	}

	Ashes_API VkResult VKAPI_CALL vkQueuePresentKHR( VkQueue queue, const  VkPresentInfoKHR* pPresentInfo )
	{
		return g_library.getSelectedDesc().functions.QueuePresentKHR( queue, pPresentInfo );
	}

#	ifdef VK_API_VERSION_1_1

	Ashes_API VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR( VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities )
	{
		return g_library.getSelectedDesc().functions.GetDeviceGroupPresentCapabilitiesKHR( device, pDeviceGroupPresentCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR( VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR*  pModes )
	{
		return g_library.getSelectedDesc().functions.GetDeviceGroupSurfacePresentModesKHR( device, surface, pModes );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDevicePresentRectanglesKHR( physicalDevice, surface, pRectCount, pRects );
	}

	Ashes_API VkResult VKAPI_CALL vkAcquireNextImage2KHR( VkDevice device, const  VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex )
	{
		return g_library.getSelectedDesc().functions.AcquireNextImage2KHR( device, pAcquireInfo, pImageIndex );
	}

#	endif

#endif
#pragma endregion
#pragma region VK_KHR_device_group
#if defined( VK_KHR_device_group )
#	ifndef VK_API_VERSION_1_1

	Ashes_API VkResult VKAPI_CALL vkGetDeviceGroupPresentCapabilitiesKHR( VkDevice device, VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities )
	{
		return g_library.getSelectedDesc().functions.GetDeviceGroupPresentCapabilitiesKHR( device, pDeviceGroupPresentCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDeviceGroupSurfacePresentModesKHR( VkDevice device, VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR*  pModes )
	{
		return g_library.getSelectedDesc().functions.GetDeviceGroupSurfacePresentModesKHR( device, surface, pModes );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDevicePresentRectanglesKHR( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDevicePresentRectanglesKHR( physicalDevice, surface, pRectCount, pRects );
	}

	Ashes_API VkResult VKAPI_CALL vkAcquireNextImage2KHR( VkDevice device, const  VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex )
	{
		return g_library.getSelectedDesc().functions.AcquireNextImage2KHR( device, pAcquireInfo, pImageIndex );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_display
#ifdef VK_KHR_display

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPropertiesKHR * pProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceDisplayPropertiesKHR( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetPhysicalDeviceDisplayPlanePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t * pPropertyCount,
		VkDisplayPlanePropertiesKHR * pProperties )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceDisplayPlanePropertiesKHR( physicalDevice, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayPlaneSupportedDisplaysKHR(
		VkPhysicalDevice physicalDevice,
		uint32_t planeIndex,
		uint32_t * pDisplayCount,
		VkDisplayKHR * pDisplays )
	{
		return g_library.getSelectedDesc().functions.GetDisplayPlaneSupportedDisplaysKHR( physicalDevice, planeIndex, pDisplayCount, pDisplays );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayModePropertiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		uint32_t * pPropertyCount,
		VkDisplayModePropertiesKHR * pProperties )
	{
		return g_library.getSelectedDesc().functions.GetDisplayModePropertiesKHR( physicalDevice, display, pPropertyCount, pProperties );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDisplayModeKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayKHR display,
		const VkDisplayModeCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkDisplayModeKHR * pMode )
	{
		return g_library.getSelectedDesc().functions.CreateDisplayModeKHR( physicalDevice, display, pCreateInfo, pAllocator, pMode );
	}

	Ashes_API VkResult VKAPI_CALL vkGetDisplayPlaneCapabilitiesKHR(
		VkPhysicalDevice physicalDevice,
		VkDisplayModeKHR mode,
		uint32_t planeIndex,
		VkDisplayPlaneCapabilitiesKHR * pCapabilities )
	{
		return g_library.getSelectedDesc().functions.GetDisplayPlaneCapabilitiesKHR( physicalDevice, mode, planeIndex, pCapabilities );
	}

	Ashes_API VkResult VKAPI_CALL vkCreateDisplayPlaneSurfaceKHR(
		VkInstance instance,
		const VkDisplaySurfaceCreateInfoKHR * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkSurfaceKHR * pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateDisplayPlaneSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#endif
#pragma endregion
#pragma region VK_KHR_android_surface
#ifdef VK_KHR_android_surface
#	ifdef VK_USE_PLATFORM_ANDROID_KHR

	Ashes_API VkResult VKAPI_CALL vkCreateAndroidSurfaceKHR( VkInstance instance, const  VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateAndroidSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_FUCHSIA_imagepipe_surface
#ifdef VK_FUCHSIA_imagepipe_surface
#	ifdef VK_USE_PLATFORM_FUCHSIA

	Ashes_API VkResult VKAPI_CALL vkCreateImagePipeSurfaceFUCHSIA( VkInstance instance, const  VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateImagePipeSurfaceFUCHSIA( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_ios_surface
#ifdef VK_MVK_ios_surface
#	ifdef VK_USE_PLATFORM_IOS_MVK

	Ashes_API VkResult VKAPI_CALL vkCreateIOSSurfaceMVK( VkInstance instance, const  VkIOSSurfaceCreateInfoMVK* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateIOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_MVK_macos_surface
#ifdef VK_MVK_macos_surface
#	ifdef VK_USE_PLATFORM_MACOS_MVK

	Ashes_API VkResult VKAPI_CALL vkCreateMacOSSurfaceMVK( VkInstance instance, const  VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateMacOSSurfaceMVK( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_NN_vi_surface
#ifdef VK_NN_vi_surface
#	ifdef VK_USE_PLATFORM_VI_NN

	Ashes_API VkResult VKAPI_CALL vkCreateViSurfaceNN( VkInstance instance, const  VkViSurfaceCreateInfoNN* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateViSurfaceNN( instance, pCreateInfo, pAllocator, pSurface );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_KHR_xcb_surface
#	ifdef __linux__

	Ashes_API VkResult VKAPI_CALL vkCreateXcbSurfaceKHR( VkInstance instance, const  VkXcbSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateXcbSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceXcbPresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceXcbPresentationSupportKHR( physicalDevice, queueFamilyIndex, connection, visual_id );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_xlib_surface
#	ifdef __linux__

	Ashes_API VkResult VKAPI_CALL vkCreateXlibSurfaceKHR( VkInstance instance, const  VkXlibSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateXlibSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceXlibPresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceXlibPresentationSupportKHR( physicalDevice, queueFamilyIndex, dpy, visualID );
	}

#	endif
// #endif
#pragma endregion
#pragma region VK_KHR_wayland_surface
#	ifdef __linux__

	Ashes_API VkResult VKAPI_CALL vkCreateWaylandSurfaceKHR( VkInstance instance, const  VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateWaylandSurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceWaylandPresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceWaylandPresentationSupportKHR( physicalDevice, queueFamilyIndex, display );
	}

#	endif
#pragma endregion
#pragma region VK_KHR_win32_surface
#ifdef VK_KHR_win32_surface
#	ifdef _WIN32

	Ashes_API VkResult VKAPI_CALL vkCreateWin32SurfaceKHR( VkInstance instance, const  VkWin32SurfaceCreateInfoKHR* pCreateInfo, const  VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface )
	{
		return g_library.getSelectedDesc().functions.CreateWin32SurfaceKHR( instance, pCreateInfo, pAllocator, pSurface );
	}

	Ashes_API VkBool32 VKAPI_CALL vkGetPhysicalDeviceWin32PresentationSupportKHR( VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex )
	{
		return g_library.getSelectedDesc().functions.GetPhysicalDeviceWin32PresentationSupportKHR( physicalDevice, queueFamilyIndex );
	}

#	endif
#endif
#pragma endregion
#pragma region VK_EXT_acquire_xlib_display
#ifdef VK_EXT_acquire_xlib_display
#	ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

	Ashes_API VkResult VKAPI_CALL vkAcquireXlibDisplayEXT( VkPhysicalDevice physicalDevice, Display* dpy, VkDisplayKHR  display )
	{
		return g_library.getSelectedDesc().functions.AcquireXlibDisplayEXT( physicalDevice, dpy, display );
	}

	Ashes_API VkResult VKAPI_CALL vkGetRandROutputDisplayEXT( VkPhysicalDevice physicalDevice, Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay )
	{
		return g_library.getSelectedDesc().functions.GetRandROutputDisplayEXT( physicalDevice, dpy, rrOutput, pDisplay );
	}

#	endif
#endif
#pragma endregion
}
