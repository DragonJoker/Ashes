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

#include <common/Exception.hpp>

namespace ashes::gl4
{
	template< typename T >
	static constexpr T NonAvailable = std::numeric_limits< T >::max();

	template< typename VkType  >
	struct VkTypeTraits;
	
	template< typename Type  >
	struct TypeTraits;

#define VK_IMPLEMENT_HANDLE(object)\
	template<>\
	struct VkTypeTraits< Vk##object >\
	{\
		using Type = object;\
	};\
	template<>\
	struct TypeTraits< object >\
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
	typename VkTypeTraits< VkType >::Type * get( VkType vkValue )
	{
		using Type = typename VkTypeTraits< VkType >::Type;
		return ( ( Type * )vkValue );
	}

	template< typename Type >
	typename TypeTraits< Type >::VkType get( Type * vkValue )
	{
		using VkType = typename TypeTraits< Type >::VkType;
		return VkType( vkValue );
	}

	template< typename Type >
	typename TypeTraits< Type >::VkType get( Type const * vkValue )
	{
		using VkType = typename TypeTraits< Type >::VkType;
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
			using Type = typename VkTypeTraits< VkType >::Type;
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
	std::vector< VkExtensionProperties > const & getSupportedDeviceExtensions();
}
