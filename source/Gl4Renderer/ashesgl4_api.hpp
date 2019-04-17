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

#include <AshesRenderer/Util/Exception.hpp>

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
}
