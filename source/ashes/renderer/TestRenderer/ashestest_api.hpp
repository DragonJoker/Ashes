/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Command/TestCommandBuffer.hpp"
#include "Command/TestCommandPool.hpp"
#include "Command/TestQueue.hpp"
#include "Core/TestDebugReportCallback.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestDisplay.hpp"
#include "Core/TestDisplayMode.hpp"
#include "Core/TestInstance.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestSurface.hpp"
#include "Core/TestSwapChain.hpp"
#include "Descriptor/TestDescriptorPool.hpp"
#include "Descriptor/TestDescriptorSet.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Miscellaneous/TestDeviceMemory.hpp"
#include "Miscellaneous/TestQueryPool.hpp"
#include "Image/TestImage.hpp"
#include "Image/TestImageView.hpp"
#include "Image/TestSampler.hpp"
#include "Pipeline/TestPipeline.hpp"
#include "Pipeline/TestPipelineCache.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "RenderPass/TestFrameBuffer.hpp"
#include "RenderPass/TestRenderPass.hpp"
#include "Shader/TestShaderModule.hpp"
#include "Sync/TestEvent.hpp"
#include "Sync/TestFence.hpp"
#include "Sync/TestSemaphore.hpp"

#include <ashes/common/Exception.hpp>
#include <ashes/common/VkTypeTraits.hpp>

#include <iostream>

namespace ashes::test
{
	template< typename T >
	static constexpr T NonAvailable = std::numeric_limits< T >::max();

	template< typename VkType  >
	struct VkTestTypeTraits;
	
	template< typename Type  >
	struct TestVkTypeTraits;

#define VK_IMPLEMENT_HANDLE(object)\
	template<>\
	struct VkTestTypeTraits< Vk##object >\
	{\
		using Type = object;\
	};\
	template<>\
	struct TestVkTypeTraits< object >\
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
	VK_IMPLEMENT_HANDLE( SurfaceKHR );
	VK_IMPLEMENT_HANDLE( SwapchainKHR );

	VK_IMPLEMENT_HANDLE( SamplerYcbcrConversion );
	VK_IMPLEMENT_HANDLE( DescriptorUpdateTemplate );
	VK_IMPLEMENT_HANDLE( DisplayKHR );
	VK_IMPLEMENT_HANDLE( DisplayModeKHR );
	VK_IMPLEMENT_HANDLE( DebugReportCallbackEXT );
	VK_IMPLEMENT_HANDLE( ObjectTableNVX );
	VK_IMPLEMENT_HANDLE( IndirectCommandsLayoutNVX );
	VK_IMPLEMENT_HANDLE( DebugUtilsMessengerEXT );
	VK_IMPLEMENT_HANDLE( ValidationCacheEXT );

	template< typename VkType >
	typename VkTestTypeTraits< VkType >::Type * get( VkType vkValue )
	{
		using Type = typename VkTestTypeTraits< VkType >::Type;
		return ( ( Type * )vkValue );
	}

	template< typename Type >
	typename TestVkTypeTraits< Type >::VkType get( Type * vkValue )
	{
		using VkType = typename TestVkTypeTraits< Type >::VkType;
		return VkType( vkValue );
	}

	template< typename Type >
	typename TestVkTypeTraits< Type >::VkType get( Type const * vkValue )
	{
		using VkType = typename TestVkTypeTraits< Type >::VkType;
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
			using Type = typename VkTestTypeTraits< VkType >::Type;
			vkValue = VkType( new Type{ std::forward< Params && >( params )... } );
			result = VK_SUCCESS;
		}
		catch ( Exception & exc )
		{
			std::cerr << exc.what() << std::endl;
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

	inline bool operator==( VkOffset2D const & lhs, VkOffset2D const & rhs )
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y;
	}

	inline bool operator!=( VkOffset2D const & lhs, VkOffset2D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkOffset3D const & lhs, VkOffset3D const & rhs )
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.z == rhs.z;
	}

	inline bool operator!=( VkOffset3D const & lhs, VkOffset3D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkExtent2D const & lhs, VkExtent2D const & rhs )
	{
		return lhs.width == rhs.width
			&& lhs.height == rhs.height;
	}

	inline bool operator!=( VkExtent2D const & lhs, VkExtent2D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkExtent3D const & lhs, VkExtent3D const & rhs )
	{
		return lhs.width == rhs.width
			&& lhs.height == rhs.height
			&& lhs.depth == rhs.depth;
	}

	inline bool operator!=( VkExtent3D const & lhs, VkExtent3D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkRect2D const & lhs, VkRect2D const & rhs )
	{
		return lhs.offset == rhs.offset
			&& lhs.extent == rhs.extent;
	}

	inline bool operator!=( VkRect2D const & lhs, VkRect2D const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkViewport const & lhs, VkViewport const & rhs )
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y
			&& lhs.width == rhs.width
			&& lhs.height == rhs.height
			&& lhs.minDepth == rhs.minDepth
			&& lhs.maxDepth == rhs.maxDepth;
	}

	inline bool operator!=( VkViewport const & lhs, VkViewport const & rhs )
	{
		return !( lhs == rhs );
	}

	inline bool operator==( VkAttachmentDescription const & lhs, VkAttachmentDescription const & rhs )
	{
		return lhs.flags == rhs.flags
			&& lhs.format == rhs.format
			&& lhs.samples == rhs.samples
			&& lhs.loadOp == rhs.loadOp
			&& lhs.storeOp == rhs.storeOp
			&& lhs.stencilLoadOp == rhs.stencilLoadOp
			&& lhs.stencilStoreOp == rhs.stencilStoreOp
			&& lhs.initialLayout == rhs.initialLayout
			&& lhs.finalLayout == rhs.finalLayout;
	}

	inline bool operator!=( VkAttachmentDescription const & lhs, VkAttachmentDescription const & rhs )
	{
		return !( lhs == rhs );
	}

	std::vector< VkExtensionProperties > const & getSupportedInstanceExtensions();

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

	template< typename VkObject >
	inline void reportError( VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
		VkInstance instance = getInstance( object );
#if VK_EXT_debug_utils
		{
			VkDebugUtilsObjectNameInfoEXT objectName
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
				nullptr,
				ashes::VkTypeTraits< VkObject >::UtilsValue,
				uint64_t( object ),
				ashes::VkTypeTraits< VkObject >::getName().c_str(),
			};
			get( instance )->onSubmitDebugUtilsMessenger( VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
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
		{
			std::string text = errorName + ": " + name;
			get( instance )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, ashes::VkTypeTraits< VkObject >::ReportValue
				, uint64_t( object )
				, 0u
				, result
				, "OpenGL3"
				, text.c_str() );
		}
#endif
	}

	template< typename VkObject >
	inline void reportWarning( VkObject object
		, VkResult result
		, std::string const & errorName
		, std::string const & name )
	{
		VkInstance instance = getInstance( object );
#if VK_EXT_debug_utils
		{
			VkDebugUtilsObjectNameInfoEXT objectNameInfo
			{
				VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
				nullptr,
				ashes::VkTypeTraits< VkObject >::UtilsValue,
				uint64_t( object ),
				ashes::VkTypeTraits< VkObject >::getName().c_str(),
			};
			get( instance )->onSubmitDebugUtilsMessenger( VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
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
					&objectNameInfo,
				} );
		}
#endif
#if VK_EXT_debug_report
		{
			std::string text = errorName + ": " + name;
			get( instance )->onReportMessage( VK_DEBUG_REPORT_WARNING_BIT_EXT
				, ashes::VkTypeTraits< VkObject >::ReportValue
				, uint64_t( object )
				, 0u
				, result
				, "OpenGL3"
				, text.c_str() );
		}
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
}
