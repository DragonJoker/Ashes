/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlDevice.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Buffer/GlGeometryBuffers.hpp"
#include "Command/GlCommandPool.hpp"
#include "Command/GlQueue.hpp"
#include "Core/GlContextLock.hpp"
#include "Core/GlSurface.hpp"
#include "Core/GlDummyIndexBuffer.hpp"
#include "Core/GlInstance.hpp"
#include "Core/GlSwapChain.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Miscellaneous/GlQueryPool.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"
#include "Sync/GlEvent.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"

#include "ashesgl4_api.hpp"

#include <iostream>
#include <cstring>

namespace ashes::gl4
{
	namespace
	{
		GLuint getObjectName( VkDebugReportObjectTypeEXT const & value
			, uint64_t object )
		{
			GLuint result = 0;

			switch ( value )
			{
			case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT:
				result = reinterpret_cast< Buffer const * >( object )->getInternal();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
				result = reinterpret_cast< Image const * >( object )->getInternal();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT:
				result = *reinterpret_cast< QueryPool const * >( object )->begin();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT:
				result = reinterpret_cast< Sampler const * >( object )->getInternal();
				break;
			case VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT:
				result = reinterpret_cast< Framebuffer const * >( object )->getInternal();
				break;
			default:
				result = GL_INVALID_INDEX;
				break;
			}

			return result;
		}

		GLuint getObjectName( VkObjectType const & value
			, uint64_t object )
		{
			GLuint result = 0;

			switch ( value )
			{
			case VK_OBJECT_TYPE_BUFFER:
				result = reinterpret_cast< Buffer const * >( object )->getInternal();
				break;
			case VK_OBJECT_TYPE_IMAGE:
				result = reinterpret_cast< Image const * >( object )->getInternal();
				break;
			case VK_OBJECT_TYPE_QUERY_POOL:
				result = *reinterpret_cast< QueryPool const * >( object )->begin();
				break;
			case VK_OBJECT_TYPE_SAMPLER:
				result = reinterpret_cast< Sampler const * >( object )->getInternal();
				break;
			case VK_OBJECT_TYPE_FRAMEBUFFER:
				result = reinterpret_cast< Framebuffer const * >( object )->getInternal();
				break;
			default:
				result = GL_INVALID_INDEX;
				break;
			}

			return result;
		}

		template< typename T, typename U >
		T getAligned( T value, U align )
		{
			return T( ( value + align - 1 ) & ~( align - 1 ) );
		};

		template< typename T >
		T getSubresourceValue( T value, uint32_t mipLevel )
		{
			return T( value >> mipLevel );
		};

		VkExtent3D getTexelBlockExtent( VkFormat format )
		{
			VkExtent3D texelBlockExtent{ 1u, 1u, 1u };

			if ( isCompressedFormat( format ) )
			{
				auto extent = getMinimalExtent2D( format );
				texelBlockExtent.width = extent.width;
				texelBlockExtent.height = extent.height;
			}
			else
			{
				texelBlockExtent.width = 1u;
			}

			return texelBlockExtent;
		}

		uint32_t getTexelBlockByteSize( VkExtent3D const & texelBlockExtent
			, VkFormat format )
		{
			VkDeviceSize texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return uint32_t( texelBlockSize );
		}
	}

	Device::Device( VkInstance instance
		, VkPhysicalDevice gpu
		, Context & context
		, VkDeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_physicalDevice{ gpu }
		, m_createInfos{ std::move( createInfos ) }
		, m_currentContext{ &context }
		, m_dyState{ VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT }
	{
		//m_timestampPeriod = 1;
		doInitialiseQueues();
		auto lock = getContext();
		doInitialiseDummy( lock );
		allocate( m_blitFbos[0]
			, nullptr
			, get( this )
			, GL_INVALID_INDEX );
		allocate( m_blitFbos[1]
			, nullptr
			, get( this )
			, GL_INVALID_INDEX );
	}

	Device::~Device()
	{
		if ( m_currentContext )
		{
			auto context = getContext();

			for ( auto creates : m_queues )
			{
				for ( auto queue : creates.second.queues )
				{
					deallocate( queue, nullptr );
				}
			}

			deallocate( m_blitFbos[0], nullptr );
			deallocate( m_blitFbos[1], nullptr );
			deallocate( m_dummyIndexed.indexMemory, nullptr );
			deallocate( m_dummyIndexed.indexBuffer, nullptr );
		}
	}

	VkPhysicalDeviceLimits const & Device::getLimits()const
	{
		return get( m_physicalDevice )->getProperties().limits;
	}

	void Device::getImageSubresourceLayout( VkImage image
		, VkImageSubresource const & subresource
		, VkSubresourceLayout & layout )const
	{
		auto context = getContext();
		auto target = convert( get( image )->getType(), get( image )->getArrayLayers() );
		glLogCall( context
			, glBindTexture
			, target
			, get( image )->getInternal() );
		int w = 0;
		int h = 0;
		int d = 0;
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_WIDTH, &w );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_HEIGHT, &h );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_DEPTH, &d );
		glLogCall( context
			, glBindTexture
			, target
			, 0 );
		auto extent = getTexelBlockExtent( get( image )->getFormat() );
		auto byteSize = getTexelBlockByteSize( extent, get( image )->getFormat() );
		layout.rowPitch = getAligned( std::max( w, 1 ), extent.width );
		layout.arrayPitch = layout.rowPitch * getAligned( std::max( h, 1 ), extent.width );
		layout.depthPitch = layout.arrayPitch;
		layout.offset = subresource.arrayLayer * layout.arrayPitch;
		layout.size = layout.arrayPitch * std::max( d, 1 );
	}

	VkResult Device::waitIdle()const
	{
		auto context = getContext();
		glLogCall( context
			, glFinish );
		return VK_SUCCESS;
	}

#if VK_EXT_debug_utils

	VkResult Device::setDebugUtilsObjectName( VkDebugUtilsObjectNameInfoEXT const & nameInfo )const
	{
		auto context = getContext();
		bool isOk = ( context->m_glObjectPtrLabel || context->m_glObjectLabel );

		if ( !isOk )
		{
			get( m_instance )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "OpenGL"
				, "Either debug utils layer is not enabled or functions are not available on your GPU" );
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		if ( nameInfo.objectType == VK_OBJECT_TYPE_FENCE )
		{
			if ( context->m_glObjectPtrLabel )
			{
				isOk = glLogCall( context
					, glObjectPtrLabel
					, get( VkFence( nameInfo.objectHandle ) )->getInternal()
					, GLsizei( strlen( nameInfo.pObjectName ) )
					, nameInfo.pObjectName );
			}
		}
		else
		{
			if ( context->m_glObjectLabel )
			{
				auto name = getObjectName( nameInfo.objectType, nameInfo.objectHandle );

				if ( name != GL_INVALID_INDEX )
				{
					isOk = glLogCall( context
						, glObjectLabel
						, GLenum( convert( nameInfo.objectType ) )
						, name
						, GLsizei( strlen( nameInfo.pObjectName ) )
						, nameInfo.pObjectName );
				}
			}
		}

		return isOk
			? VK_SUCCESS
			: VK_ERROR_INCOMPATIBLE_DRIVER;
	}

	VkResult Device::setDebugUtilsObjectTag( VkDebugUtilsObjectTagInfoEXT const & tagInfo )const
	{
		return VK_SUCCESS;
	}

#endif
#if VK_EXT_debug_marker

	VkResult Device::debugMarkerSetObjectTag( VkDebugMarkerObjectTagInfoEXT const & tagInfo )const
	{
		return VK_SUCCESS;
	}

	VkResult Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
		auto context = getContext();
		bool isOk = ( context->m_glObjectPtrLabel || context->m_glObjectLabel );

		if ( !isOk )
		{
			get( m_instance )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "OpenGL"
				, "Either debug marker layer is not enabled or functions are not available on your GPU" );
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		if ( nameInfo.objectType == VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT )
		{
			if ( context->m_glObjectPtrLabel )
			{
				isOk = glLogCall( context
					, glObjectPtrLabel
					, get( VkFence( nameInfo.object ) )->getInternal()
					, GLsizei( strlen( nameInfo.pObjectName ) )
					, nameInfo.pObjectName );
			}
		}
		else
		{
			if ( context->m_glObjectLabel )
			{
				auto name = getObjectName( nameInfo.objectType, nameInfo.object );

				if ( name != GL_INVALID_INDEX )
				{
					isOk = glLogCall( context
						, glObjectLabel
						, GLenum( convert( nameInfo.objectType ) )
						, name
						, GLsizei( strlen( nameInfo.pObjectName ) )
						, nameInfo.pObjectName );
				}
			}
		}

		return isOk
			? VK_SUCCESS
			: VK_ERROR_INCOMPATIBLE_DRIVER;
	}

#endif

	void Device::reportMessage( VkDebugReportFlagsEXT flags
		, VkDebugReportObjectTypeEXT objectType
		, uint64_t object
		, size_t location
		, int32_t messageCode
		, const char * pLayerPrefix
		, const char * pMessage )
	{
		get( m_instance )->reportMessage( flags
			, objectType
			, object
			, location
			, messageCode
			, pLayerPrefix
			, pMessage );
	}

	VkQueue Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			get( m_instance )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "OpenGL"
				, "Couldn't find family index within created queues" );
			return VK_NULL_HANDLE;
		}

		if ( it->second.queues.size() <= index )
		{
			get( m_instance )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "OpenGL"
				, "Couldn't find queue with wanted index within its family" );
			return VK_NULL_HANDLE;
		}

		return it->second.queues[index];
	}

	void Device::swapBuffers()const
	{
		getContext()->swapBuffers();
	}

	void Device::registerContext( VkSurfaceKHR surface )const
	{
		try
		{
			auto context = Context::create( m_instance, surface, nullptr );
			ContextLock lock{ *context };

			if ( !m_ownContext )
			{
				m_ownContext = std::move( context );
				m_currentContext = m_ownContext.get();
				get( m_instance )->registerContext( *m_ownContext );
			}
		}
		catch ( std::exception & exc )
		{
			get( m_instance )->reportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "OpenGL"
				, exc.what() );
			throw;
		}
	}

	void Device::unregisterContext( VkSurfaceKHR surface )const
	{
	}

	void Device::doInitialiseQueues()
	{
		for ( auto itQueue = m_createInfos.pQueueCreateInfos;
			itQueue != m_createInfos.pQueueCreateInfos + m_createInfos.queueCreateInfoCount;
			++itQueue )
		{
			auto & queueCreateInfo = *itQueue;
			auto it = m_queues.emplace( queueCreateInfo.queueFamilyIndex
				, QueueCreates{ queueCreateInfo, {} } ).first;

			VkQueue queue;
			allocate( queue
				, nullptr
				, get( this )
				, it->second.createInfo
				, uint32_t( it->second.queues.size() ) );
			it->second.queues.emplace_back( queue );
		}
	}

	void Device::doInitialiseDummy( ContextLock & context )
	{
		auto count = uint32_t( sizeof( dummyIndex ) / sizeof( dummyIndex[0] ) );
		allocate( m_dummyIndexed.indexBuffer
			, nullptr
			, get( this )
			, VkBufferCreateInfo
			{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,
				0u,
				count * sizeof( uint32_t ),
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0u,
				nullptr,
			} );
		auto indexBuffer = get( m_dummyIndexed.indexBuffer );
		auto requirements = indexBuffer->getMemoryRequirements();
		auto deduced = deduceMemoryType( requirements.memoryTypeBits
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
		allocate( m_dummyIndexed.indexMemory
			, nullptr
			, get( this )
			, VkMemoryAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } );
		get( m_dummyIndexed.indexMemory )->bindToBuffer( m_dummyIndexed.indexBuffer, 0u );
		auto memory = get( m_dummyIndexed.indexMemory );
		uint8_t * buffer{ nullptr };
		auto size = count * sizeof( uint32_t );

		if ( memory->lock( context, 0u, size, 0u, reinterpret_cast< void ** >( &buffer ) ) == VK_SUCCESS )
		{
			std::copy( dummyIndex, dummyIndex + size, buffer );
			memory->flush( context, 0, size );
			memory->unlock( context );
		}

		m_dummyIndexed.geometryBuffers = std::make_unique< GeometryBuffers >( get( this )
			, VboBindings{}
			, BufferObjectBinding{ indexBuffer->getInternal(), 0u, m_dummyIndexed.indexBuffer }
			, VkPipelineVertexInputStateCreateInfo{}
			, InputLayout{}
			, VK_INDEX_TYPE_UINT32 );
		m_dummyIndexed.geometryBuffers->initialise( context );
	}
}
