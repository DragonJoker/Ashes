/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Util/ArrayView.hpp"

#include <ashes/ashes.h>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined( __GNUG__ )
#	define ASHES_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if ASHES_COMPILER_VERSION < 40900
#		error "Unsupported version of GCC"
#	elif ASHES_COMPILER_VERSION < 70400
#		include <experimental/optional>
namespace ashes
{
	template< typename T >
	using Optional = std::experimental::optional< T >;
	using std::experimental::nullopt;
}
#	else
#		include <optional>
namespace ashes
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	endif
#else
#	include <optional>
namespace ashes
{
	template< typename T >
	using Optional = std::optional< T >;
	using std::nullopt;
}
#	if defined( MemoryBarrier )
#		undef MemoryBarrier
#	endif
#endif

#ifndef NDEBUG
#	define declareDebugVariable( Type, Name, Value )\
	mutable Type Name{ Value }
#	define setDebugValue( Name, Value )\
	Name = Value;
#	define assertDebugValue( Name, Value )\
	assert( Name == Value )
#else
#	define declareDebugVariable( Type, Name, Value )
#	define setDebugValue( Name, Value )
#	define assertDebugValue( Name, Value )
#endif

namespace ashes
{
	inline constexpr uint32_t getMajor( uint32_t version )
	{
		return ( ( uint32_t )( version ) >> 22 );
	}

	inline constexpr uint32_t getMinor( uint32_t version )
	{
		return ( ( ( uint32_t )( version ) >> 12 ) & 0x3ff );
	}

	inline constexpr uint32_t getPatch( uint32_t version )
	{
		return ( ( uint32_t )( version ) & 0xfff );
	}

	inline constexpr uint32_t makeVersion( uint32_t major
		, uint32_t minor
		, uint32_t patch )
	{
		return ( ( ( major ) << 22 ) | ( ( minor ) << 12 ) | ( patch ) );
	}

	inline uint32_t checkFlag( uint32_t a, uint32_t b )
	{
		return ( a & b ) == b;
	}

	template< typename Type, typename ... Params >
	inline Type deepCopy( Type const & rhs
		, Params && ... params )
	{
		return rhs;
	}

	template< typename Type, typename ... Params >
	inline std::vector< Type > makeVector( Type const * ptr
		, size_t count
		, Params && ... params )
	{
		std::vector< Type > result;

		for ( auto & elem : makeArrayView( ptr, count ) )
		{
			result.push_back( deepCopy( elem, std::forward< Params && >( params )... ) );
		}

		return result;
	}

	template< typename Type, typename ... Params >
	inline std::vector< Type > makeVector( Type const * ptr
		, uint32_t count
		, Params && ... params )
	{
		std::vector< Type > result;

		for ( auto & elem : makeArrayView( ptr, count ) )
		{
			result.push_back( deepCopy( elem, std::forward< Params && >( params )... ) );
		}

		return result;
	}

	template< typename Type, typename ... Params >
	inline Optional< Type > makeOptional( Type const * ptr
		, Params && ... params )
	{
		return ptr
			? Optional< Type >{ deepCopy( *ptr, std::forward< Params && >( params )... ) }
			: nullopt;
	}

	using StringArray = std::vector< std::string >;
	using UInt32Array = std::vector< uint32_t >;
	using UInt64Array = std::vector< uint64_t >;

	using VkQueueCreateCount = std::pair< VkDeviceQueueCreateInfo, uint32_t >;
	using VkQueueCreateCountMap = std::map< uint32_t, VkQueueCreateCount >;

	using VkAttachmentDescriptionArray = std::vector< VkAttachmentDescription >;
	using VkAttachmentReferenceArray = std::vector< VkAttachmentReference >;
	using VkBufferArray = std::vector< VkBuffer >;
	using VkBufferCopyArray = std::vector< VkBufferCopy >;
	using VkBufferImageCopyArray = std::vector< VkBufferImageCopy >;
	using VkBufferMemoryBarrierArray = std::vector< VkBufferMemoryBarrier >;
	using VkClearAttachmentArray = std::vector< VkClearAttachment >;
	using VkClearRectArray = std::vector< VkClearRect >;
	using VkClearValueArray = std::vector< VkClearValue >;
	using VkCommandBufferArray = std::vector< VkCommandBuffer >;
	using VkDescriptorPoolSizeArray = std::vector< VkDescriptorPoolSize >;
	using VkDescriptorSetArray = std::vector< VkDescriptorSet >;
	using VkDescriptorSetLayoutArray = std::vector< VkDescriptorSetLayout >;
	using VkDescriptorSetLayoutBindingArray = std::vector< VkDescriptorSetLayoutBinding >;
	using VkDeviceSizeArray = std::vector< VkDeviceSize >;
	using VkDynamicStateArray = std::vector< VkDynamicState >;
	using VkEventArray = std::vector< VkEvent >;
	using VkExtensionPropertiesArray = std::vector< VkExtensionProperties >;
	using VkImageArray = std::vector< VkImage >;
	using VkImageBlitArray = std::vector< VkImageBlit >;
	using VkImageCopyArray = std::vector< VkImageCopy >;
	using VkImageMemoryBarrierArray = std::vector< VkImageMemoryBarrier >;
	using VkImageSubresourceRangeArray = std::vector< VkImageSubresourceRange >;
	using VkImageViewArray = std::vector< VkImageView >;
	using VkMemoryBarrierArray = std::vector< VkMemoryBarrier >;
	using VkPipelineColorBlendAttachmentStateArray = std::vector< VkPipelineColorBlendAttachmentState >;
	using VkPipelineShaderStageCreateInfoArray = std::vector< VkPipelineShaderStageCreateInfo >;
	using VkPhysicalDeviceArray = std::vector< VkPhysicalDevice >;
	using VkPresentModeArrayKHR = std::vector< VkPresentModeKHR >;
	using VkPushConstantRangeArray = std::vector< VkPushConstantRange >;
	using VkResultArray = std::vector< VkResult >;
	using VkSemaphoreArray = std::vector< VkSemaphore >;
	using VkScissorArray = std::vector< VkRect2D >;
	using VkShaderModuleArray = std::vector< VkShaderModule >;
	using VkSubmitInfoArray = std::vector< VkSubmitInfo >;
	using VkSubpassDependencyArray = std::vector< VkSubpassDependency >;
	using VkSubpassDescriptionArray = std::vector< VkSubpassDescription >;
	using VkSurfaceFormatArrayKHR = std::vector< VkSurfaceFormatKHR >;
	using VkSwapchainArrayKHR = std::vector< VkSwapchainKHR >;
	using VkVertexInputAttributeDescriptionArray = std::vector< VkVertexInputAttributeDescription >;
	using VkVertexInputBindingDescriptionArray = std::vector< VkVertexInputBindingDescription >;
	using VkViewportArray = std::vector< VkViewport >;
	using VkWriteDescriptorSetArray = std::vector< VkWriteDescriptorSet >;
}
