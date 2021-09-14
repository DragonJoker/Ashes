/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlPhysicalDevice.hpp"

#include "Core/GlInstance.hpp"
#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlScreenHelpers.hpp"

#include "ashesgl_api.hpp"

#include <ashes/common/Hash.hpp>

#if defined( __linux__ )
#	include <X11/Xlib.h>
#elif defined( _WIN32 )
#	include <gl/GL.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iterator>
#include <stdexcept>

#ifdef max
#	undef max
#endif
#ifdef GL_TEXTURE_2D
#undef GL_TEXTURE_2D
#endif
#ifdef GL_SUBPIXEL_BITS
#undef GL_SUBPIXEL_BITS
#endif
#ifdef GL_MAX_TEXTURE_SIZE
#undef GL_MAX_TEXTURE_SIZE
#endif
#ifdef GL_MAX_VIEWPORT_DIMS
#undef GL_MAX_VIEWPORT_DIMS
#endif
#ifdef GL_POINT_SIZE_RANGE
#undef GL_POINT_SIZE_RANGE
#endif
#ifdef GL_POINT_SIZE_GRANULARITY
#undef GL_POINT_SIZE_GRANULARITY
#endif
#ifdef GL_LINE_WIDTH_GRANULARITY
#undef GL_LINE_WIDTH_GRANULARITY
#endif

namespace ashes::gl
{
	namespace
	{
		size_t makeKey( VkFormat format
			, VkImageType type
			, VkImageTiling tiling
			, VkImageUsageFlags usage
			, VkImageCreateFlags flags )
		{
			auto hash = std::hash< VkFormat >{}( format );
			hashCombine( hash, type );
			hashCombine( hash, tiling );
			hashCombine( hash, usage );
			hashCombine( hash, flags );
			return hash;
		}

		uint32_t doGetVendorID( std::string vendorName )
		{
			uint32_t result = 0u;
			std::transform( vendorName.begin()
				, vendorName.end()
				, vendorName.begin()
				, []( char c )
				{
					return char( ::tolower( c ) );
				} );

			if ( vendorName.find( "nvidia" ) != std::string::npos )
			{
				result = 0x10DE;
			}
			else if ( vendorName.find( "intel" ) != std::string::npos )
			{
				result = 0x8086;
			}
			else if ( vendorName.find( "amd" ) != std::string::npos )
			{
				result = 0x1002;
			}
			else if ( vendorName.find( "arm" ) != std::string::npos )
			{
				result = 0x13B5;
			}

			return result;
		}
	}

	PhysicalDevice::PhysicalDevice( VkInstance instance )
		: m_instance{ instance }
	{
		doInitialise();
	}

	std::vector< VkLayerProperties > PhysicalDevice::enumerateLayerProperties()const
	{
		return {};
	}

	uint32_t PhysicalDevice::getMemoryTypeBits( VkMemoryPropertyFlags properties )const
	{
		uint32_t result{};
		auto & memoryProperties = getMemoryProperties();

		for ( auto i = 0u; i < memoryProperties.memoryTypeCount; ++i )
		{
			if ( ( memoryProperties.memoryTypes[i].propertyFlags & properties ) == properties )
			{
				result |= ( 0x1 << i );
			}
		}

		return result;
	}

	uint32_t PhysicalDevice::getMemoryTypeBits( VkMemoryPropertyFlags properties1
		, VkMemoryPropertyFlags properties2 )const
	{
		uint32_t result = getMemoryTypeBits( properties1 );
		result |= getMemoryTypeBits( properties2 );
		return result;
	}

	std::vector < VkExtensionProperties > PhysicalDevice::enumerateExtensionProperties( const char * layerName )const
	{
		static std::vector< VkExtensionProperties > const extensions
		{
#if VK_KHR_swapchain
			VkExtensionProperties{ VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_SWAPCHAIN_SPEC_VERSION },
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
#if VK_EXT_inline_uniform_block
			VkExtensionProperties{ VK_EXT_INLINE_UNIFORM_BLOCK_EXTENSION_NAME, VK_EXT_INLINE_UNIFORM_BLOCK_SPEC_VERSION },
#endif
#if VK_KHR_maintenance1
			VkExtensionProperties{ VK_KHR_MAINTENANCE1_EXTENSION_NAME, VK_KHR_MAINTENANCE1_SPEC_VERSION },
#endif
#if VK_KHR_portability_subset
			VkExtensionProperties{ VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, VK_KHR_PORTABILITY_SUBSET_SPEC_VERSION },
#endif
		};
		return extensions;
	}

	VkPhysicalDeviceProperties const & PhysicalDevice::getProperties()const
	{
		return m_properties;
	}

	VkPhysicalDeviceMemoryProperties const & PhysicalDevice::getMemoryProperties()const
	{
		return m_memoryProperties;
	}

	VkPhysicalDeviceFeatures const & PhysicalDevice::getFeatures()const
	{
		return m_features;
	}

	std::vector< VkQueueFamilyProperties > const & PhysicalDevice::getQueueFamilyProperties()const
	{
		return m_queueProperties;
	}

	VkFormatProperties const & PhysicalDevice::getFormatProperties( VkFormat fmt )const
	{
		auto iresult = m_formatProperties.insert( { fmt, VkFormatProperties{} } );

		if ( iresult.second )
		{
			auto & properties = iresult.first->second;

			if ( !find( ARB_internalformat_query2 ) )
			{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wextra"
				properties.optimalTilingFeatures = VkFormatFeatureFlags( VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT
					| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
					| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
					| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
					| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
					| VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT
					| VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT
					| VkFormatFeatureFlagBits( ashes::isDepthOrStencilFormat( fmt ) ? VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT : 0u )
					| VK_FORMAT_FEATURE_BLIT_SRC_BIT
					| VK_FORMAT_FEATURE_BLIT_DST_BIT
					| VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT
					| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
					| VK_FORMAT_FEATURE_TRANSFER_DST_BIT );
#pragma GCC diagnostic pop
				properties.linearTilingFeatures = properties.optimalTilingFeatures;
			}
			else
			{
				ContextLock context{ get( m_instance )->getCurrentContext() };
				assert( context->m_glGetInternalformativ );
				auto internal = getInternalFormat( fmt );

				if ( isCompressedFormat( fmt ) )
				{
					if ( isBCFormat( fmt ) )
					{
						internal = ( m_features.textureCompressionBC
							? internal
							: GL_INTERNAL_UNSUPPORTED );
					}
					else if ( isEACFormat( fmt ) || isETC2Format( fmt ) )
					{
						internal = ( m_features.textureCompressionETC2
							? internal
							: GL_INTERNAL_UNSUPPORTED );
					}
					else if ( isASTCFormat( fmt ) )
					{
						internal = ( m_features.textureCompressionASTC_LDR
							? internal
							: GL_INTERNAL_UNSUPPORTED );
					}
				}

				if ( internal != GL_INTERNAL_UNSUPPORTED )
				{
					GlFormatPropertyResult support = GL_FORMAT_PROPERTY_UNSUPPORTED;
					glLogCall( context, glGetInternalformativ
						, GL_TEXTURE_2D
						, internal
						, GL_FORMAT_PROPERTY_IS_SUPPORTED
						, 1
						, reinterpret_cast< GLint * >( &support ) );

					if ( support == GL_FORMAT_PROPERTY_SUPPORTED )
					{
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_READ_PIXELS
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
						{
							if ( isSRGBFormat( fmt ) )
							{
								support = GL_FORMAT_PROPERTY_UNSUPPORTED;
								glLogCall( context, glGetInternalformativ
									, GL_TEXTURE_2D
									, internal
									, GL_FORMAT_PROPERTY_SRGB_READ
									, 1
									, reinterpret_cast< GLint * >( &support ) );

								if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
								{
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
								}
							}
							else
							{
								properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
							}
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_RENDERABLE
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
						{
							if ( isSRGBFormat( fmt ) )
							{
								support = GL_FORMAT_PROPERTY_UNSUPPORTED;
								glLogCall( context, glGetInternalformativ
									, GL_TEXTURE_2D
									, internal
									, GL_FORMAT_PROPERTY_SRGB_WRITE
									, 1
									, reinterpret_cast< GLint * >( &support ) );

								if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
								{
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
								}
							}
							else
							{
								properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
								properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
							}
						}
#endif
						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_RENDERABLE
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
						{
							if ( isDepthStencilFormat( fmt ) )
							{
								support = GL_FORMAT_PROPERTY_UNSUPPORTED;
								glLogCall( context, glGetInternalformativ
									, GL_TEXTURE_2D
									, internal
									, GL_FORMAT_PROPERTY_DEPTH_RENDERABLE
									, 1
									, reinterpret_cast< GLint * >( &support ) );

								if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
								{
									support = GL_FORMAT_PROPERTY_UNSUPPORTED;
									glLogCall( context, glGetInternalformativ
										, GL_TEXTURE_2D
										, internal
										, GL_FORMAT_PROPERTY_STENCIL_RENDERABLE
										, 1
										, reinterpret_cast< GLint * >( &support ) );

									if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
									{
										properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
										properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
										properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
#endif
									}
								}
							}
							else if ( isDepthFormat( fmt ) )
							{
								support = GL_FORMAT_PROPERTY_UNSUPPORTED;
								glLogCall( context, glGetInternalformativ
									, GL_TEXTURE_2D
									, internal
									, GL_FORMAT_PROPERTY_DEPTH_RENDERABLE
									, 1
									, reinterpret_cast< GLint * >( &support ) );

								if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
								{
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
#endif
								}
							}
							else if ( isStencilFormat( fmt ) )
							{
								support = GL_FORMAT_PROPERTY_UNSUPPORTED;
								glLogCall( context, glGetInternalformativ
									, GL_TEXTURE_2D
									, internal
									, GL_FORMAT_PROPERTY_STENCIL_RENDERABLE
									, 1
									, reinterpret_cast< GLint * >( &support ) );

								if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
								{
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
#endif
								}
							}
							else
							{
								support = GL_FORMAT_PROPERTY_UNSUPPORTED;
								glLogCall( context, glGetInternalformativ
									, GL_TEXTURE_2D
									, internal
									, GL_FORMAT_PROPERTY_COLOR_RENDERABLE
									, 1
									, reinterpret_cast< GLint * >( &support ) );

								if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
								{
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
									properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_DST_BIT;
#endif
									support = GL_FORMAT_PROPERTY_UNSUPPORTED;
									glLogCall( context, glGetInternalformativ
										, GL_TEXTURE_2D
										, internal
										, GL_FORMAT_PROPERTY_BLEND
										, 1
										, reinterpret_cast< GLint * >( &support ) );

									if ( support != GL_FORMAT_PROPERTY_UNSUPPORTED )
									{
										properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
									}
								}
							}
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_FRAGMENT_TEXTURE
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_BLIT_SRC_BIT;
#endif
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_FILTER
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_SHADER_IMAGE_LOAD
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_SHADER_IMAGE_ATOMIC
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT;
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_BUFFER
							, internal
							, GL_FORMAT_PROPERTY_FRAGMENT_TEXTURE
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.bufferFeatures |= VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT;
							properties.bufferFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
							properties.bufferFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_BUFFER
							, internal
							, GL_FORMAT_PROPERTY_SHADER_IMAGE_LOAD
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.bufferFeatures |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT;
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_BUFFER
							, internal
							, GL_FORMAT_PROPERTY_SHADER_IMAGE_ATOMIC
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.bufferFeatures |= VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT;
						}

#if defined( VK_KHR_maintenance ) || defined( VK_API_VERSION_1_1 )
						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_2D
							, internal
							, GL_FORMAT_PROPERTY_READ_PIXELS
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
							properties.optimalTilingFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
						}

						support = GL_FORMAT_PROPERTY_UNSUPPORTED;
						glLogCall( context, glGetInternalformativ
							, GL_TEXTURE_BUFFER
							, internal
							, GL_FORMAT_PROPERTY_READ_PIXELS
							, 1
							, reinterpret_cast< GLint * >( &support ) );

						if ( support != 0 )
						{
							properties.bufferFeatures |= VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
							properties.bufferFeatures |= VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
						}
#endif
					}

					if ( !isCompressedFormat( fmt ) )
					{
						GlType dataType = getType( fmt );

						switch ( dataType )
						{
						case ashes::gl::GL_TYPE_I8:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_UI8:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_I16:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_UI16:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_I32:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_UI32:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_F32:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_F16:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_US_4_4_4_4:
							break;
						case ashes::gl::GL_TYPE_US_5_5_5_1:
							break;
						case ashes::gl::GL_TYPE_UI_8_8_8_8:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_UI_8_8_8_8_REV:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_UI_2_10_10_10_REV:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						case ashes::gl::GL_TYPE_US_5_6_5:
							break;
						case ashes::gl::GL_TYPE_UI_24_8:
							break;
						case ashes::gl::GL_TYPE_32F_UI24_8:
							break;
						case ashes::gl::GL_UI_5_9_9_9_REV:
							break;
						case ashes::gl::GL_UI_10F_11F_11F_REV:
							properties.bufferFeatures |= VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT;
							break;
						default:
							break;
						}
					}
				}

				properties.linearTilingFeatures = properties.optimalTilingFeatures;
			}
		}

		return iresult.first->second;
	}

	VkResult PhysicalDevice::getImageFormatProperties( VkFormat format
		, VkImageType type
		, VkImageTiling tiling
		, VkImageUsageFlags usage
		, VkImageCreateFlags flags
		, VkImageFormatProperties & imageProperties )const
	{
		auto pair = m_imageFormatProperties.insert( { makeKey( format
				, type
				, VkImageTiling{}
				, VkImageUsageFlags{}
				, VkImageCreateFlags{} )
			, {} } );

		if ( pair.second )
		{
			auto & imageFormatProperties = pair.first->second.first;
			pair.first->second.second = VK_ERROR_FORMAT_NOT_SUPPORTED;
			imageFormatProperties = {};
			ContextLock context{ get( m_instance )->getCurrentContext() };
			auto internal = getInternalFormat( format );

			if ( isCompressedFormat( format ) )
			{
				if ( isBCFormat( format ) )
				{
					internal = ( m_features.textureCompressionBC
						? internal
						: GL_INTERNAL_UNSUPPORTED );
				}
				else if ( isEACFormat( format ) || isETC2Format( format ) )
				{
					internal = ( m_features.textureCompressionETC2
						? internal
						: GL_INTERNAL_UNSUPPORTED );
				}
				else if ( isASTCFormat( format ) )
				{
					internal = ( m_features.textureCompressionASTC_LDR
						? internal
						: GL_INTERNAL_UNSUPPORTED );
				}
			}

			if ( context->m_glGetInternalformativ
				&& internal != GL_INTERNAL_UNSUPPORTED )
			{
				auto gltype = convert( get( this ), type, 1u, 0u );
				GlFormatPropertyResult support = GL_FORMAT_PROPERTY_UNSUPPORTED;
				glLogCall( context
					, glGetInternalformativ
					, gltype
					, internal
					, GL_FORMAT_PROPERTY_IS_SUPPORTED
					, 1
					, reinterpret_cast< GLint * >( &support ) );

				if ( support == GL_FORMAT_PROPERTY_SUPPORTED )
				{
					GLint w = 0;
					glLogCall( context
						, glGetInternalformativ
						, gltype
						, internal
						, GL_FORMAT_PROPERTY_MAX_WIDTH
						, 1
						, &w );
					imageFormatProperties.maxExtent.width = uint32_t( w );
					imageFormatProperties.maxExtent.height = 1u;
					imageFormatProperties.maxExtent.depth = 1u;
					imageFormatProperties.sampleCounts = VK_SAMPLE_COUNT_1_BIT;

					if ( type == VK_IMAGE_TYPE_2D )
					{
						GLint h = 0;
						glLogCall( context
							, glGetInternalformativ
							, gltype
							, internal
							, GL_FORMAT_PROPERTY_MAX_HEIGHT
							, 1
							, &h );
						imageFormatProperties.maxExtent.height = uint32_t( h );
					}
					else if ( type == VK_IMAGE_TYPE_3D )
					{
						GLint h = 0;
						glLogCall( context
							, glGetInternalformativ
							, GL_TEXTURE_3D
							, internal
							, GL_FORMAT_PROPERTY_MAX_HEIGHT
							, 1
							, &h );
						imageFormatProperties.maxExtent.height = uint32_t( h );

						GLint d = 0;
						glLogCall( context
							, glGetInternalformativ
							, GL_TEXTURE_3D
							, internal
							, GL_FORMAT_PROPERTY_MAX_DEPTH
							, 1
							, &d );
						imageFormatProperties.maxExtent.depth = uint32_t( d );
					}

					if ( gltype == GL_TEXTURE_2D )
					{
						GLint n = 0;
						glLogCall( context
							, glGetInternalformativ
							, GL_TEXTURE_2D_MULTISAMPLE
							, internal
							, GL_FORMAT_PROPERTY_NUM_SAMPLE_COUNTS
							, 1
							, &n );

						std::vector< GLint > samples;
						samples.resize( size_t( n ) );
						glLogCall( context
							, glGetInternalformativ
							, GL_TEXTURE_2D_MULTISAMPLE
							, internal
							, GL_FORMAT_PROPERTY_SAMPLES
							, GLsizei( samples.size() )
							, samples.data() );

						for ( auto sample : samples )
						{
							imageFormatProperties.sampleCounts |= VkSampleCountFlagBits( sample );
						}
					}

					GLint t = 0;
					glLogCall( context
						, glGetInternalformativ
						, gltype
						, internal
						, GL_FORMAT_PROPERTY_TEXEL_SIZE
						, 1
						, &t );

					gltype = convert( get( this ), type, 2u, 0u );
					GLint l = 0;
					glLogCall( context
						, glGetInternalformativ
						, gltype
						, internal
						, GL_FORMAT_PROPERTY_MAX_LAYERS
						, 1
						, &l );
					imageFormatProperties.maxArrayLayers = std::max( 1u, uint32_t( l ) );

					if ( imageFormatProperties.maxExtent.width > 0u )
					{
						imageFormatProperties.maxMipLevels = getMaxMipCount( imageFormatProperties.maxExtent );
						imageFormatProperties.maxResourceSize = getLevelsSize( imageFormatProperties.maxExtent
							, format
							, 0u
							, imageFormatProperties.maxMipLevels
							, uint32_t( getBlockSize( format ).size ) );
						pair.first->second.second = VK_SUCCESS;
					}
					else
					{
						imageFormatProperties.maxMipLevels = 0u;
						pair.first->second.second = VK_ERROR_FORMAT_NOT_SUPPORTED;
					}
				}
			}
		}

		imageProperties = pair.first->second.first;
		return pair.first->second.second;
	}

	VkResult PhysicalDevice::getSparseImageFormatProperties( VkFormat format
		, VkImageType type
		, VkSampleCountFlagBits samples
		, VkImageUsageFlags usage
		, VkImageTiling tiling
		, std::vector< VkSparseImageFormatProperties > & sparseImageFormatProperties )const
	{
		return VK_ERROR_FORMAT_NOT_SUPPORTED;
	}

#if VK_VERSION_1_2

	VkPhysicalDeviceDriverProperties const & PhysicalDevice::getDriverProperties()const
	{
		return m_driverProperties;
	}

#elif VK_KHR_driver_properties

	VkPhysicalDeviceDriverPropertiesKHR const & PhysicalDevice::getDriverProperties()const
	{
		return m_driverProperties;
	}

#endif
#ifdef VK_KHR_display

	std::vector< VkDisplayPropertiesKHR > const & PhysicalDevice::getDisplayProperties()const
	{
		return m_displays;
	}

	std::vector< VkDisplayPlanePropertiesKHR > PhysicalDevice::getDisplayPlaneProperties()const
	{
		return m_displayPlanes;
	}

	std::vector< VkDisplayKHR > PhysicalDevice::getDisplayPlaneSupportedDisplays( uint32_t planeIndex )const
	{
		std::vector< VkDisplayKHR > result
		{
			m_displayPlanes[planeIndex].currentDisplay
		};
		return result;
	}

#endif

	bool PhysicalDevice::find( VkExtensionProperties const & name )const
	{
		return get( m_instance )->getExtensions().find( name );
	}

	bool PhysicalDevice::findAny( VkExtensionPropertiesArray const & names )const
	{
		return get( m_instance )->getExtensions().findAny( names );
	}

	bool PhysicalDevice::findAll( VkExtensionPropertiesArray const & names )const
	{
		return get( m_instance )->getExtensions().findAll( names );
	}

	int PhysicalDevice::getMajor()const
	{
		return int( get( m_instance )->getExtensions().getMajor() );
	}

	int PhysicalDevice::getMinor()const
	{
		return int( get( m_instance )->getExtensions().getMinor() );
	}

	void PhysicalDevice::doInitialise()
	{
		m_glFeatures.has420PackExtensions = find( ARB_shading_language_420pack );
		m_glFeatures.hasCopyImage = find( ARB_copy_image );
		m_glFeatures.hasProgramPipelines = find( ARB_separate_shader_objects );
		m_glFeatures.hasTextureStorage = findAll( { ARB_texture_storage, ARB_texture_storage_multisample } );
		m_glFeatures.hasTextureViews = find( ARB_texture_view );
		m_glFeatures.hasViewportArrays = find( ARB_viewport_array );
		m_glFeatures.hasProgramInterfaceQuery = find( ARB_program_interface_query );

		ContextLock context{ get( m_instance )->getCurrentContext() };
		doInitialiseMemoryProperties( context );
		doInitialiseFeatures( context );
		doInitialiseProperties( context );
		doInitialiseQueueProperties( context );
		doInitialiseDisplayProperties( context );
		doInitialisePortability( context );
		doInitialiseDriverProperties( context );
		doInitialiseInlineUniformBlock( context );
	}

	void PhysicalDevice::doInitialiseFeatures( ContextLock & context )
	{
		m_features.robustBufferAccess = find( KHR_robustness );
		m_features.fullDrawIndexUint32 = false;
		m_features.imageCubeArray = find( ARB_texture_cube_map_array );
		m_features.independentBlend = findAny( { ARB_draw_buffers_blend, EXT_draw_buffers2 } );
		m_features.geometryShader = find( ARB_geometry_shader4 );
		m_features.tessellationShader = find( ARB_tessellation_shader );
		m_features.sampleRateShading = find( ARB_sample_shading );
		m_features.dualSrcBlend = find( ARB_blend_func_extended );
		m_features.logicOp = true;
		m_features.multiDrawIndirect = findAll( { ARB_multi_draw_indirect, ARB_draw_indirect } );
		m_features.drawIndirectFirstInstance = findAll( { ARB_base_instance, ARB_draw_instanced } );
		m_features.depthClamp = find( ARB_depth_clamp );
		m_features.depthBiasClamp = find( ARB_polygon_offset_clamp );
		m_features.fillModeNonSolid = true;
		m_features.depthBounds = true;
		GLint range[2];
		glLogCall( context, glGetIntegerv
			, GL_VALUE_NAME_ALIASED_LINE_WIDTH_RANGE
			, range );
		m_features.wideLines = ( range[1] >= 1 );
		glLogCall( context, glGetIntegerv
			, GL_VALUE_NAME_SMOOTH_LINE_WIDTH_RANGE
			, range );
		m_features.wideLines &= ( range[1] >= 1 );
		m_features.largePoints = true;
		m_features.alphaToOne = true;
		m_features.multiViewport = find( ARB_viewport_array );
		m_features.samplerAnisotropy = find( ARB_texture_filter_anisotropic );
		m_features.textureCompressionETC2 = false;
		m_features.textureCompressionASTC_LDR = findAny( { KHR_texture_compression_astc_ldr, KHR_texture_compression_astc_hdr } );
		m_features.textureCompressionBC = findAll( { ARB_texture_compression, EXT_texture_compression_s3tc, EXT_texture_sRGB } );
		m_features.occlusionQueryPrecise = true;
		m_features.pipelineStatisticsQuery = find( ARB_pipeline_statistics_query );
		m_features.vertexPipelineStoresAndAtomics = find( ARB_shader_atomic_counters );
		m_features.fragmentStoresAndAtomics = m_features.vertexPipelineStoresAndAtomics;
		m_features.shaderTessellationAndGeometryPointSize = m_features.tessellationShader && m_features.geometryShader;
		m_features.shaderImageGatherExtended = findAll( { ARB_texture_gather, ARB_gpu_shader5 } );
		m_features.shaderStorageImageExtendedFormats = find( ARB_shader_image_load_store );
		m_features.shaderStorageImageMultisample = find( ARB_shader_image_load_store );
		m_features.shaderStorageImageReadWithoutFormat = find( EXT_shader_image_load_formatted );
		m_features.shaderStorageImageWriteWithoutFormat = find( ARB_shader_image_load_store );
		m_features.shaderUniformBufferArrayDynamicIndexing = find( ARB_gpu_shader5 );
		m_features.shaderSampledImageArrayDynamicIndexing = find( ARB_gpu_shader5 );
		m_features.shaderStorageBufferArrayDynamicIndexing = find( ARB_shader_storage_buffer_object );
		m_features.shaderStorageImageArrayDynamicIndexing = find( ARB_shader_image_load_store );
		m_features.shaderClipDistance = true;
		m_features.shaderCullDistance = find( ARB_cull_distance );
		m_features.shaderFloat64 = find( ARB_gpu_shader_fp64 );
		m_features.shaderInt64 = find( ARB_gpu_shader_int64 );
		m_features.shaderInt16 = false;
		m_features.shaderResourceResidency = false;// find( ARB_sparse_texture2 );
		m_features.shaderResourceMinLod = false;// find( ARB_sparse_texture_clamp );
		m_features.sparseBinding = false;// findAll( { ARB_sparse_buffer, ARB_sparse_texture2 } );
		m_features.sparseResidencyBuffer = false;// find( ARB_sparse_buffer );
		m_features.sparseResidencyImage2D = false;// find( ARB_sparse_texture2 );
		m_features.sparseResidencyImage3D = false;// find( ARB_sparse_texture2 );
		m_features.sparseResidency2Samples = false;// find( ARB_sparse_texture2 );
		m_features.sparseResidency4Samples = false;// find( ARB_sparse_texture2 );
		m_features.sparseResidency8Samples = false;// find( ARB_sparse_texture2 );
		m_features.sparseResidency16Samples = false;// find( ARB_sparse_texture2 );
		m_features.sparseResidencyAliased = false;
		m_features.variableMultisampleRate = true;
		m_features.inheritedQueries = true;
	}

	void PhysicalDevice::doInitialiseProperties( ContextLock & context )
	{
		m_properties.apiVersion = get( m_instance )->getApiVersion();
		m_properties.deviceID = 0u;
		strncpy( m_properties.deviceName
			, reinterpret_cast< char const * >( context->glGetString( GL_INFO_RENDERER ) )
			, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE - 1 );
		strncat( m_properties.deviceName
			, " (gl)"
			, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE - 1 );
		std::memset( m_properties.pipelineCacheUUID, 0u, sizeof( m_properties.pipelineCacheUUID ) );
		m_properties.vendorID = doGetVendorID( reinterpret_cast< char const * >( context->glGetString( GL_INFO_VENDOR ) ) );
		m_properties.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		m_properties.driverVersion = 0;
		uint32_t shaderStages = 3;

		if ( m_features.tessellationShader )
		{
			shaderStages += 2;
		}

		if ( m_features.geometryShader )
		{
			shaderStages++;
		}

		context.getValue( GL_VALUE_NAME_MAX_TEXTURE_SIZE, m_properties.limits.maxImageDimension1D, 16384u );
		context.getValue( GL_VALUE_NAME_MAX_TEXTURE_SIZE, m_properties.limits.maxImageDimension2D, 16384u );
		context.getValue( GL_VALUE_NAME_MAX_3D_TEXTURE_SIZE, m_properties.limits.maxImageDimension3D, 2048u );
		context.getValue( GL_VALUE_NAME_MAX_CUBE_MAP_TEXTURE_SIZE, m_properties.limits.maxImageDimensionCube, 16384u );
		context.getValue( GL_VALUE_NAME_MAX_ARRAY_TEXTURE_LAYERS, m_properties.limits.maxImageArrayLayers, 2048u );
		context.getValue( GL_VALUE_NAME_MAX_TEXTURE_BUFFER_SIZE, m_properties.limits.maxTexelBufferElements, 65536u );
		context.getValue( GL_VALUE_NAME_MAX_UNIFORM_BLOCK_SIZE, m_properties.limits.maxUniformBufferRange, 65536u );
		context.getValue( GL_VALUE_NAME_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, m_properties.limits.maxStorageBufferRange, 134217728u );
		context.getValue( GL_VALUE_NAME_MAX_UNIFORM_BLOCK_SIZE, m_properties.limits.maxPushConstantsSize, 16384u );
		m_properties.limits.maxMemoryAllocationCount = NonAvailable< uint32_t >;
		m_properties.limits.maxSamplerAllocationCount = NonAvailable< uint32_t >;
		m_properties.limits.bufferImageGranularity = DefaultAlign< VkDeviceSize >;
		m_properties.limits.sparseAddressSpaceSize = NonAvailable< uint64_t >;
		m_properties.limits.maxBoundDescriptorSets = NonAvailable< uint32_t >;
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorSamplers, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_UNIFORM_BINDINGS, m_properties.limits.maxPerStageDescriptorUniformBuffers, 84u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_SHADER_STORAGE_BINDINGS, m_properties.limits.maxPerStageDescriptorStorageBuffers, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorSampledImages, 8u );
		context.getValue( GL_VALUE_NAME_MAX_IMAGE_UNITS, m_properties.limits.maxPerStageDescriptorStorageImages, 8u );
		m_properties.limits.maxPerStageDescriptorInputAttachments = NonAvailable< uint32_t >;
		m_properties.limits.maxPerStageResources = NonAvailable< uint32_t >;
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxDescriptorSetSamplers, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_UNIFORM_BINDINGS, m_properties.limits.maxDescriptorSetUniformBuffers, 84u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_UNIFORM_BINDINGS, m_properties.limits.maxDescriptorSetUniformBuffersDynamic, 84u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_SHADER_STORAGE_BINDINGS, m_properties.limits.maxDescriptorSetStorageBuffers, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_SHADER_STORAGE_BINDINGS, m_properties.limits.maxDescriptorSetStorageBuffersDynamic, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_TEXTURE_IMAGE_UNITS, m_properties.limits.maxDescriptorSetSampledImages, 8u );
		context.getValue( GL_VALUE_NAME_MAX_IMAGE_UNITS, m_properties.limits.maxDescriptorSetStorageImages, 8u ) *= shaderStages;
		m_properties.limits.maxDescriptorSetInputAttachments = NonAvailable< uint32_t >;
		context.getValue( GL_VALUE_NAME_MAX_VERTEX_ATTRIBS, m_properties.limits.maxVertexInputAttributes, 16u );
		context.getValue( GL_VALUE_NAME_MAX_VERTEX_ATTRIB_BINDINGS, m_properties.limits.maxVertexInputBindings, 16u );
		context.getValue( GL_VALUE_NAME_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET, m_properties.limits.maxVertexInputAttributeOffset, 2047u );
		context.getValue( GL_VALUE_NAME_MAX_VERTEX_ATTRIB_STRIDE, m_properties.limits.maxVertexInputBindingStride, 2048u );
		context.getValue( GL_VALUE_NAME_MAX_VERTEX_OUTPUT_COMPONENTS, m_properties.limits.maxVertexOutputComponents, 64u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_GEN_LEVEL, m_properties.limits.maxTessellationGenerationLevel, 64u );
		context.getValue( GL_VALUE_NAME_MAX_PATCH_VERTICES, m_properties.limits.maxTessellationPatchSize, 32u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_CONTROL_INPUT_COMPONENTS, m_properties.limits.maxTessellationControlPerVertexInputComponents, 128u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_CONTROL_OUTPUT_COMPONENTS, m_properties.limits.maxTessellationControlPerVertexOutputComponents, 128u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_PATCH_COMPONENTS, m_properties.limits.maxTessellationControlPerPatchOutputComponents, 120u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_CONTROL_OUTPUT_COMPONENTS, m_properties.limits.maxTessellationControlTotalOutputComponents, 4096u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_EVALUATION_INPUT_COMPONENTS, m_properties.limits.maxTessellationEvaluationInputComponents, 128u );
		context.getValue( GL_VALUE_NAME_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS, m_properties.limits.maxTessellationEvaluationOutputComponents, 128u );
		context.getValue( GL_VALUE_NAME_MAX_GEOMETRY_SHADER_INVOCATIONS, m_properties.limits.maxGeometryShaderInvocations, 32u );
		context.getValue( GL_VALUE_NAME_MAX_GEOMETRY_INPUT_COMPONENTS, m_properties.limits.maxGeometryInputComponents, 64u );
		context.getValue( GL_VALUE_NAME_MAX_GEOMETRY_OUTPUT_COMPONENTS, m_properties.limits.maxGeometryOutputComponents, 128u );
		context.getValue( GL_VALUE_NAME_MAX_GEOMETRY_OUTPUT_VERTICES, m_properties.limits.maxGeometryOutputVertices, 256u );
		context.getValue( GL_VALUE_NAME_MAX_GEOMETRY_OUTPUT_COMPONENTS, m_properties.limits.maxGeometryTotalOutputComponents, 1024u );
		context.getValue( GL_VALUE_NAME_MAX_FRAGMENT_INPUT_COMPONENTS, m_properties.limits.maxFragmentInputComponents, 128u );
		context.getValue( GL_VALUE_NAME_MAX_DRAW_BUFFERS, m_properties.limits.maxFragmentOutputAttachments, 8u );
		context.getValue( GL_VALUE_NAME_MAX_DUAL_SOURCE_DRAW_BUFFERS, m_properties.limits.maxFragmentDualSrcAttachments, 1u );
		context.getValue( GL_VALUE_NAME_MAX_COLOR_ATTACHMENTS, m_properties.limits.maxFragmentCombinedOutputResources, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMPUTE_SHARED_MEMORY_SIZE, m_properties.limits.maxComputeSharedMemorySize, 32768u );
		context.getValuesI( GL_VALUE_NAME_MAX_COMPUTE_WORK_GROUP_COUNT, m_properties.limits.maxComputeWorkGroupCount, { 65535u, 65535u, 65535u } );
		context.getValue( GL_VALUE_NAME_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, m_properties.limits.maxComputeWorkGroupInvocations, 1024u );
		context.getValuesI( GL_VALUE_NAME_MAX_COMPUTE_WORK_GROUP_SIZE, m_properties.limits.maxComputeWorkGroupSize, { 1024u, 1024u, 64u } );
		context.getValue( GL_VALUE_NAME_SUBPIXEL_BITS, m_properties.limits.subPixelPrecisionBits, 4u );
		m_properties.limits.subTexelPrecisionBits = m_properties.limits.subPixelPrecisionBits;
		m_properties.limits.mipmapPrecisionBits = NonAvailable< uint32_t >;
		m_properties.limits.maxDrawIndexedIndexValue = NonAvailable< uint32_t >;
		m_properties.limits.maxDrawIndirectCount = NonAvailable< uint32_t >;
		context.getValue( GL_VALUE_NAME_MAX_TEXTURE_LOD_BIAS, m_properties.limits.maxSamplerLodBias, 2.0f );
		context.getValue( GL_VALUE_NAME_MAX_TEXTURE_MAX_ANISOTROPY, m_properties.limits.maxSamplerAnisotropy );
		context.getValue( GL_VALUE_NAME_MAX_VIEWPORTS, m_properties.limits.maxViewports, 16u );
		context.getValues( GL_VALUE_NAME_MAX_VIEWPORT_DIMS, m_properties.limits.maxViewportDimensions );
		m_properties.limits.viewportBoundsRange[0] = -2.0f * float( std::max( m_properties.limits.maxViewportDimensions[0], m_properties.limits.maxViewportDimensions[1] ) );
		m_properties.limits.viewportBoundsRange[1] = 2.0f * float( std::max( m_properties.limits.maxViewportDimensions[0], m_properties.limits.maxViewportDimensions[1] ) );
		m_properties.limits.viewportSubPixelBits = m_properties.limits.subPixelPrecisionBits;
		m_properties.limits.minMemoryMapAlignment = 64u;
		context.getValue( GL_VALUE_NAME_TEXTURE_BUFFER_OFFSET_ALIGNMENT, m_properties.limits.minTexelBufferOffsetAlignment );
		context.getValue( GL_VALUE_NAME_UNIFORM_BUFFER_OFFSET_ALIGNMENT, m_properties.limits.minUniformBufferOffsetAlignment );
		m_properties.limits.minStorageBufferOffsetAlignment = 1u;
		context.getValue( GL_VALUE_NAME_MIN_PROGRAM_TEXEL_OFFSET, m_properties.limits.minTexelOffset );
		m_properties.limits.minTexelOffset = std::min( m_properties.limits.minTexelOffset, -8 );
		context.getValue( GL_VALUE_NAME_MAX_PROGRAM_TEXEL_OFFSET, m_properties.limits.maxTexelOffset, 7u );
		context.getValue( GL_VALUE_NAME_MIN_PROGRAM_TEXTURE_GATHER_OFFSET, m_properties.limits.minTexelGatherOffset );
		m_properties.limits.minTexelGatherOffset = std::min( m_properties.limits.minTexelGatherOffset, -8 );
		context.getValue( GL_VALUE_NAME_MAX_PROGRAM_TEXTURE_GATHER_OFFSET, m_properties.limits.maxTexelGatherOffset, 7u );
		m_properties.limits.subPixelInterpolationOffsetBits = 4u;
		m_properties.limits.minInterpolationOffset = -0.5f;
		m_properties.limits.maxInterpolationOffset = float( 0.5f - ( 1.0f / pow( 2.0f, float( m_properties.limits.subPixelInterpolationOffsetBits ) ) ) );
		context.getValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_WIDTH, m_properties.limits.maxFramebufferWidth, 16384u );
		context.getValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_HEIGHT, m_properties.limits.maxFramebufferHeight, 16384u );
		context.getValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_LAYERS, m_properties.limits.maxFramebufferLayers, 2048u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_SAMPLES, m_properties.limits.framebufferColorSampleCounts, 5u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_SAMPLES, m_properties.limits.framebufferDepthSampleCounts, 5u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_SAMPLES, m_properties.limits.framebufferStencilSampleCounts, 5u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_FRAMEBUFFER_SAMPLES, m_properties.limits.framebufferNoAttachmentsSampleCounts, 5u );
		context.getValue( GL_VALUE_NAME_MAX_COLOR_ATTACHMENTS, m_properties.limits.maxColorAttachments, 8u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_COLOR_TEXTURE_SAMPLES, m_properties.limits.sampledImageColorSampleCounts, 1u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_INTEGER_SAMPLES, m_properties.limits.sampledImageIntegerSampleCounts, 1u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_DEPTH_TEXTURE_SAMPLES, m_properties.limits.sampledImageDepthSampleCounts, 1u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_DEPTH_TEXTURE_SAMPLES, m_properties.limits.sampledImageStencilSampleCounts, 1u );
		context.getBitfieldValue( GL_VALUE_NAME_MAX_COLOR_TEXTURE_SAMPLES, m_properties.limits.storageImageSampleCounts, 1u );
		context.getValue( GL_VALUE_NAME_MAX_SAMPLE_MASK_WORDS, m_properties.limits.maxSampleMaskWords, 1u );
		m_properties.limits.timestampComputeAndGraphics = true;
		m_properties.limits.timestampPeriod = 1;
		context.getValue( GL_VALUE_NAME_MAX_CLIP_DISTANCES, m_properties.limits.maxClipDistances, 8u );
		context.getValue( GL_VALUE_NAME_MAX_CULL_DISTANCES, m_properties.limits.maxCullDistances, 8u );
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_CLIP_AND_CULL_DISTANCES, m_properties.limits.maxCombinedClipAndCullDistances, 8u );
		m_properties.limits.discreteQueuePriorities = 2u;
		context.getValues( GL_VALUE_NAME_POINT_SIZE_RANGE, m_properties.limits.pointSizeRange );
		context.getValues( GL_VALUE_NAME_ALIASED_LINE_WIDTH_RANGE, m_properties.limits.lineWidthRange );
		context.getValue( GL_VALUE_NAME_POINT_SIZE_GRANULARITY, m_properties.limits.pointSizeGranularity );
		context.getValue( GL_VALUE_NAME_LINE_WIDTH_GRANULARITY, m_properties.limits.lineWidthGranularity );
		m_properties.limits.strictLines = true;
		m_properties.limits.standardSampleLocations = false;
		m_properties.limits.optimalBufferCopyOffsetAlignment = DefaultAlign< VkDeviceSize >;
		m_properties.limits.optimalBufferCopyRowPitchAlignment = DefaultAlign< VkDeviceSize >;
		m_properties.limits.nonCoherentAtomSize = 64ull;

		m_properties.sparseProperties.residencyAlignedMipSize = false;
		m_properties.sparseProperties.residencyNonResidentStrict = false;
		m_properties.sparseProperties.residencyStandard2DBlockShape = false;
		m_properties.sparseProperties.residencyStandard2DMultisampleBlockShape = false;
		m_properties.sparseProperties.residencyStandard3DBlockShape = false;
	}

	void PhysicalDevice::doInitialiseMemoryProperties( ContextLock & context )
	{
		VkDeviceSize memSizeNV{};
		uint32_t memSizeAMD[4]{};
		context.getValue( GL_VALUE_NAME_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, memSizeNV );

		if ( !memSizeNV )
		{
			context.getValues( GL_VALUE_NAME_TEXTURE_FREE_MEMORY_ATI, memSizeAMD );

			if ( memSizeAMD[0] )
			{
				memSizeNV = memSizeAMD[0] + memSizeAMD[2];
			}
			else
			{
				memSizeNV = ~( 0ull );
			}
		}
		else
		{
			// GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX returns size in kilobytes.
			memSizeNV *= 1024;
		}

		// Emulate one device local heap
		m_memoryProperties.memoryHeaps[m_memoryProperties.memoryHeapCount++] = { memSizeNV, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT };
		// and one host visible heap
		m_memoryProperties.memoryHeaps[m_memoryProperties.memoryHeapCount++] = { memSizeNV, 0u };

		// Emulate few combinations of device local memory types
		m_memoryProperties.memoryTypes[m_memoryProperties.memoryTypeCount++] = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0u };
		m_memoryProperties.memoryTypes[m_memoryProperties.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1u };
		m_memoryProperties.memoryTypes[m_memoryProperties.memoryTypeCount++] = { VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, 1u };
	}

	void PhysicalDevice::doInitialiseQueueProperties( ContextLock & context )
	{
		m_queueProperties.push_back(
			{
				0xFF,
				1u,
				64u,
				{
					1u,
					1u,
					1u,
				}
			} );
	}

	void PhysicalDevice::doInitialiseDisplayProperties( ContextLock & context )
	{
#ifdef VK_KHR_display

		auto count = getScreenCount();

		for ( uint32_t i = 0u; i < count; ++i )
		{
			std::string name;
			VkDisplayPropertiesKHR displayProps{};
			std::vector< VkDisplayModeParametersKHR > displayModesParams;
			getScreenDesc( i, name, displayProps, displayModesParams );
			m_displayNames.emplace_back( std::move( name ) );
			displayProps.displayName = m_displayNames.back().c_str();
			allocate( displayProps.display
				, nullptr
				, get( this )
				, displayProps
				, i
				, displayModesParams );
			m_displays.push_back( displayProps );
		}

		for ( auto & display : m_displays )
		{
			m_displayPlanes.push_back( { display.display, 0u } );
		}

#endif
	}

	void PhysicalDevice::doInitialiseDriverProperties( ContextLock & context )
	{
#if VK_VERSION_1_1
		m_driverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES;
#elif VK_KHR_get_physical_device_properties2
		m_driverProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR;
#endif
#if VK_VERSION_1_1 || VK_KHR_get_physical_device_properties2
		auto & extensions = get( m_instance )->getExtensions();
		m_driverProperties.pNext = nullptr;
		m_driverProperties.driverID = VkDriverId( 0x12345678 );
		strcpy( m_driverProperties.driverName, "GlRenderer" );
		snprintf( m_driverProperties.driverInfo
			, sizeof( m_driverProperties.driverInfo )
			, "Ashes OpenGL %d.%d Renderer"
			, int( extensions.getMajor() )
			, int( extensions.getMinor() ) );
		m_driverProperties.conformanceVersion.major = uint8_t( Instance::getDefaultMajor() );
		m_driverProperties.conformanceVersion.minor = uint8_t( Instance::getDefaultMinor() );
		m_driverProperties.conformanceVersion.subminor = 0;
		m_driverProperties.conformanceVersion.patch = 0;
#endif
	}

	void PhysicalDevice::doInitialisePortability( ContextLock & context )
	{
#if VK_KHR_portability_subset

		auto & extensions = get( m_instance )->getExtensions();
		m_portabilityFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PORTABILITY_SUBSET_FEATURES_KHR;
		m_portabilityFeatures.pNext = nullptr;
		m_portabilityFeatures.constantAlphaColorBlendFactors = VK_TRUE;
		m_portabilityFeatures.events = VK_FALSE;
		m_portabilityFeatures.imageViewFormatReinterpretation = VK_TRUE;
		m_portabilityFeatures.imageViewFormatSwizzle = VK_TRUE;
		m_portabilityFeatures.imageView2DOn3DImage = VK_FALSE;
		m_portabilityFeatures.multisampleArrayImage = ( extensions.find( ARB_texture_multisample )
			? VK_TRUE
			: VK_FALSE );
		m_portabilityFeatures.mutableComparisonSamplers = VK_TRUE;
		m_portabilityFeatures.pointPolygons = VK_TRUE;
		m_portabilityFeatures.samplerMipLodBias = VK_TRUE;
		m_portabilityFeatures.separateStencilMaskRef = VK_TRUE;
		m_portabilityFeatures.shaderSampleRateInterpolationFunctions = ( ( extensions.getShaderVersion() >= 400 )
			? m_features.sampleRateShading
			: VK_FALSE );
		m_portabilityFeatures.tessellationIsolines = m_features.tessellationShader;
		m_portabilityFeatures.tessellationPointMode = m_features.tessellationShader;
		m_portabilityFeatures.triangleFans = VK_TRUE;
		m_portabilityFeatures.vertexAttributeAccessBeyondStride = VK_FALSE;

#endif
	}

	void PhysicalDevice::doInitialiseInlineUniformBlock( ContextLock & context )
	{
#if VK_EXT_inline_uniform_block

		m_inlineUniformBlockFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_FEATURES_EXT;
		m_inlineUniformBlockFeatures.pNext = nullptr;
		m_inlineUniformBlockFeatures.descriptorBindingInlineUniformBlockUpdateAfterBind = VK_FALSE;
		m_inlineUniformBlockFeatures.inlineUniformBlock = VK_TRUE;

		m_inlineUniformBlockProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INLINE_UNIFORM_BLOCK_PROPERTIES_EXT;
		m_inlineUniformBlockProperties.pNext = nullptr;
		context.getValue( GL_VALUE_NAME_MAX_COMBINED_UNIFORM_BLOCKS, m_inlineUniformBlockProperties.maxDescriptorSetInlineUniformBlocks );
		context.getValue( GL_VALUE_NAME_MAX_UNIFORM_BLOCK_SIZE, m_inlineUniformBlockProperties.maxInlineUniformBlockSize );
		context.getValue( GL_VALUE_NAME_MAX_FRAGMENT_UNIFORM_BLOCKS, m_inlineUniformBlockProperties.maxPerStageDescriptorInlineUniformBlocks );
		m_inlineUniformBlockProperties.maxDescriptorSetUpdateAfterBindInlineUniformBlocks = 4u;
		m_inlineUniformBlockProperties.maxPerStageDescriptorUpdateAfterBindInlineUniformBlocks = 4u;

#endif
	}

	bool has420PackExtensions( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().has420PackExtensions != 0;
	}

	bool hasCopyImage( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().hasCopyImage != 0;
	}

	bool hasProgramPipelines( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().hasProgramPipelines != 0;
	}

	bool hasSamplerAnisotropy( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getFeatures().samplerAnisotropy != 0;
	}

	bool hasTextureStorage( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().hasTextureStorage != 0;
	}

	bool hasTextureViews( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().hasTextureViews != 0;
	}

	bool hasViewportArrays( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().hasViewportArrays != 0;
	}

	bool hasProgramInterfaceQuery( VkPhysicalDevice physicalDevice )
	{
		return get( physicalDevice )->getGlFeatures().hasProgramInterfaceQuery != 0;
	}
}
