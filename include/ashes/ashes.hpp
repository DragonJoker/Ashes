/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_CPP_H___
#define ___Ashes_CPP_H___
#pragma once

#include "ashes.h"

#include "common/ArrayView.hpp"
#include "common/Optional.hpp"
#include "common/FlagCombination.hpp"
#include "common/Signal.hpp"

#if defined( MemoryBarrier )
#	undef MemoryBarrier
#endif

namespace ashes
{
	using ByteArray = std::vector< uint8_t >;
	using UInt16Array = std::vector< uint16_t >;
	using UInt32Array = std::vector< uint32_t >;
	using UInt64Array = std::vector< uint64_t >;
	using FloatArray = std::vector< float >;
	using StringArray = std::vector< std::string >;
	using CharPtrArray = std::vector< char const * >;

	static VkDeviceSize constexpr WholeSize = ~( 0ull );

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

	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< LibType > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( element );
		}

		return result;
	}

	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::reference_wrapper< LibType > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( element.get() );
		}

		return result;
	}

	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::unique_ptr< LibType > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( *element );
		}

		return result;
	}

	template< typename VkType, typename LibType >
	inline std::vector< VkType > makeVkArray( std::vector< std::reference_wrapper< LibType const > > const & input )
	{
		std::vector< VkType > result;
		result.reserve( input.size() );

		for ( auto const & element : input )
		{
			result.emplace_back( VkType( element.get() ) );
		}

		return result;
	}

	inline CharPtrArray convert( StringArray const & values )
	{
		CharPtrArray result;

		for ( auto & value : values )
		{
			result.push_back( value.c_str() );
		}

		return result;
	}

	template< typename VkType, typename LibType >
	std::vector< VkType > convert( std::vector< LibType > const & values )
	{
		std::vector< VkType > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( convert( value ) );
		}

		return result;
	}

	template< typename VkType, typename LibType >
	std::vector< VkType > convert( std::vector< std::reference_wrapper< LibType const > > const & values )
	{
		std::vector< VkType > result;
		result.reserve( values.size() );

		for ( auto & value : values )
		{
			result.emplace_back( convert( value.get() ) );
		}

		return result;
	}

	template< typename VkType, typename ItType >
	std::vector< VkType > convert( ItType begin, ItType end )
	{
		std::vector< VkType > result;
		result.reserve( std::distance( begin, end ) );

		for ( auto it = begin; it != end; ++it )
		{
			result.emplace_back( convert( *it ) );
		}

		return result;
	}

	inline VkClearValue makeClearValue( VkClearColorValue v )
	{
		VkClearValue result;
		result.color = std::move( v );
		return result;
	}

	inline VkClearValue makeClearValue( VkClearDepthStencilValue v )
	{
		VkClearValue result;
		result.depthStencil = std::move( v );
		return result;
	}

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
	using VkDeviceQueueCreateInfoArray = std::vector< VkDeviceQueueCreateInfo >;
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
	using VkLayerPropertiesArray = std::vector< VkLayerProperties >;
	using VkMemoryBarrierArray = std::vector< VkMemoryBarrier >;
	using VkPipelineCacheArray = std::vector< VkPipelineCache >;
	using VkPipelineColorBlendAttachmentStateArray = std::vector< VkPipelineColorBlendAttachmentState >;
	using VkPipelineShaderStageCreateInfoArray = std::vector< VkPipelineShaderStageCreateInfo >;
	using VkPipelineStageFlagsArray = std::vector< VkPipelineStageFlags >;
	using VkPhysicalDeviceArray = std::vector< VkPhysicalDevice >;
	using VkPresentModeArrayKHR = std::vector< VkPresentModeKHR >;
	using VkPushConstantRangeArray = std::vector< VkPushConstantRange >;
	using VkQueueFamilyPropertiesArray = std::vector< VkQueueFamilyProperties >;
	using VkResultArray = std::vector< VkResult >;
	using VkSemaphoreArray = std::vector< VkSemaphore >;
	using VkScissorArray = std::vector< VkRect2D >;
	using VkShaderModuleArray = std::vector< VkShaderModule >;
	using VkSpecializationMapEntryArray = std::vector< VkSpecializationMapEntry >;
	using VkSubmitInfoArray = std::vector< VkSubmitInfo >;
	using VkSubpassDependencyArray = std::vector< VkSubpassDependency >;
	using VkSubpassDescriptionArray = std::vector< VkSubpassDescription >;
	using VkSurfaceFormatArrayKHR = std::vector< VkSurfaceFormatKHR >;
	using VkSwapchainArrayKHR = std::vector< VkSwapchainKHR >;
	using VkVertexInputAttributeDescriptionArray = std::vector< VkVertexInputAttributeDescription >;
	using VkVertexInputBindingDescriptionArray = std::vector< VkVertexInputBindingDescription >;
	using VkViewportArray = std::vector< VkViewport >;
	using VkWriteDescriptorSetArray = std::vector< VkWriteDescriptorSet >;

#if defined( VK_USE_PLATFORM_ANDROID_KHR )

	using VkSurfaceCreateInfoKHR = VkAndroidSurfaceCreateInfoKHR;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_KHR_android_surface";

#elif defined( VK_USE_PLATFORM_FUCHSIA )

	using VkSurfaceCreateInfoKHR = VkImagePipeSurfaceCreateInfoFUCHSIA;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_FUCHSIA_imagepipe_surface";

#elif defined( VK_USE_PLATFORM_IOS_MVK )

	using VkSurfaceCreateInfoKHR = VkIOSSurfaceCreateInfoMVK;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_MVK_ios_surface";

#elif defined( VK_USE_PLATFORM_MACOS_MVK )

	using VkSurfaceCreateInfoKHR = VkMacOSSurfaceCreateInfoMVK;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_MVK_macos_surface";

#elif defined( VK_USE_PLATFORM_VI_NN )

	using VkSurfaceCreateInfoKHR = VkViSurfaceCreateInfoNN;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_NN_vi_surface";

#elif defined( VK_USE_PLATFORM_XCB_KHR )

	using VkSurfaceCreateInfoKHR = VkXcbSurfaceCreateInfoKHR;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_KHR_xcb_surface";

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

	using VkSurfaceCreateInfoKHR = VkXlibSurfaceCreateInfoKHR;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_KHR_xlib_surface";

#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )

	using VkSurfaceCreateInfoKHR = VkWaylandSurfaceCreateInfoKHR;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_KHR_wayland_surface";

#elif defined( VK_USE_PLATFORM_WIN32_KHR )

	using VkSurfaceCreateInfoKHR = VkWin32SurfaceCreateInfoKHR;
	static std::string const KHR_PLATFORM_SURFACE_EXTENSION_NAME = "VK_KHR_win32_surface";

#endif

	template< typename Type, typename ... Params >
	inline std::vector< Type > makeVector( Type const * ptr
		, size_t count
		, Params && ... params )
	{
		std::vector< Type > result;

		if ( ptr )
		{
			for ( auto & elem : makeArrayView( ptr, count ) )
			{
				result.push_back( deepCopy( elem, std::forward< Params && >( params )... ) );
			}
		}

		return result;
	}

	template< typename Type, typename ... Params >
	inline std::vector< Type > makeVector( Type const * ptr
		, uint32_t count
		, Params && ... params )
	{
		std::vector< Type > result;

		if ( ptr )
		{
			for ( auto & elem : makeArrayView( ptr, count ) )
			{
				result.push_back( deepCopy( elem, std::forward< Params && >( params )... ) );
			}
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

	inline VkSpecializationMapEntry deepCopy( VkSpecializationMapEntry const & rhs )
	{
		return rhs;
	}

	inline uint8_t deepCopy( uint8_t const & rhs )
	{
		return rhs;
	}

	inline VkSpecializationInfo deepCopy( VkSpecializationInfo const & rhs
		, VkSpecializationMapEntryArray & entries
		, ByteArray & data )
	{
		VkSpecializationInfo result{ rhs };
		entries = makeVector( result.pMapEntries, result.mapEntryCount );
		data = makeVector( reinterpret_cast< uint8_t const * >( result.pData ), result.dataSize );
		result.pData = data.data();
		result.pMapEntries = entries.data();
		return result;
	}

	inline VkPipelineShaderStageCreateInfo deepCopy( VkPipelineShaderStageCreateInfo const & rhs
		, Optional< VkSpecializationInfo > & specializationInfo
		, VkSpecializationMapEntryArray & entries
		, ByteArray & data )
	{
		VkPipelineShaderStageCreateInfo result{ rhs };

		if ( result.pSpecializationInfo )
		{
			specializationInfo = deepCopy( *result.pSpecializationInfo
				, entries
				, data );
			result.pSpecializationInfo = &specializationInfo.value();
		}

		return result;
	}

	inline VkPipelineColorBlendAttachmentState deepCopy( VkPipelineColorBlendAttachmentState const & rhs )
	{
		return rhs;
	}

	inline VkPipelineColorBlendStateCreateInfo deepCopy( VkPipelineColorBlendStateCreateInfo const & rhs
		, VkPipelineColorBlendAttachmentStateArray & attachments )
	{
		VkPipelineColorBlendStateCreateInfo result = rhs;
		attachments = makeVector( rhs.pAttachments, rhs.attachmentCount );
		result.pAttachments = attachments.data();
		return result;
	}

	inline VkStencilOpState deepCopy( VkStencilOpState const & rhs )
	{
		return rhs;
	}

	inline VkPipelineDepthStencilStateCreateInfo deepCopy( VkPipelineDepthStencilStateCreateInfo const & rhs )
	{
		return rhs;
	}

	inline VkDynamicState deepCopy( VkDynamicState const & rhs )
	{
		return rhs;
	}

	inline VkPipelineDynamicStateCreateInfo deepCopy( VkPipelineDynamicStateCreateInfo const & rhs
		, VkDynamicStateArray & states )
	{
		VkPipelineDynamicStateCreateInfo result = rhs;
		states = makeVector( rhs.pDynamicStates, rhs.dynamicStateCount );
		result.pDynamicStates = states.data();
		return result;
	}

	inline VkPipelineInputAssemblyStateCreateInfo deepCopy( VkPipelineInputAssemblyStateCreateInfo const & rhs )
	{
		return rhs;
	}

	inline VkPipelineMultisampleStateCreateInfo deepCopy( VkPipelineMultisampleStateCreateInfo const & rhs )
	{
		return rhs;
	}

	inline VkPipelineRasterizationStateCreateInfo deepCopy( VkPipelineRasterizationStateCreateInfo const & rhs )
	{
		return rhs;
	}

	inline VkPipelineTessellationStateCreateInfo deepCopy( VkPipelineTessellationStateCreateInfo const & rhs )
	{
		return rhs;
	}

	inline VkVertexInputAttributeDescription deepCopy( VkVertexInputAttributeDescription const & rhs )
	{
		return rhs;
	}

	inline VkVertexInputBindingDescription deepCopy( VkVertexInputBindingDescription const & rhs )
	{
		return rhs;
	}

	inline VkPipelineVertexInputStateCreateInfo deepCopy( VkPipelineVertexInputStateCreateInfo const & rhs
		, VkVertexInputBindingDescriptionArray & bindings
		, VkVertexInputAttributeDescriptionArray & attributes )
	{
		VkPipelineVertexInputStateCreateInfo result = rhs;
		bindings = makeVector( rhs.pVertexBindingDescriptions, rhs.vertexBindingDescriptionCount );
		attributes = makeVector( rhs.pVertexAttributeDescriptions, rhs.vertexAttributeDescriptionCount );
		result.pVertexBindingDescriptions = bindings.data();
		result.pVertexAttributeDescriptions = attributes.data();
		return result;
	}

	inline VkViewport deepCopy( VkViewport const & rhs )
	{
		return rhs;
	}

	inline VkRect2D deepCopy( VkRect2D const & rhs )
	{
		return rhs;
	}

	inline VkPipelineViewportStateCreateInfo deepCopy( VkPipelineViewportStateCreateInfo const & rhs
		, VkViewportArray & viewports
		, VkScissorArray & scissors )
	{
		VkPipelineViewportStateCreateInfo result = rhs;

		if ( rhs.pViewports )
		{
			viewports = makeVector( rhs.pViewports, rhs.viewportCount );
		}

		if ( rhs.pScissors )
		{
			scissors = makeVector( rhs.pScissors, rhs.scissorCount );
		}

		result.pViewports = viewports.data();
		result.pScissors = scissors.data();
		return result;
	}
}

#endif
