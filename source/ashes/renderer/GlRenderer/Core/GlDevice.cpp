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
#include "Core/GlInstance.hpp"
#include "Core/GlSwapChain.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Miscellaneous/GlDummyIndexBuffer.hpp"
#include "Miscellaneous/GlQueryPool.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"
#include "Sync/GlEvent.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"

#include "ashesgl_api.hpp"

#include <iostream>
#include <cstring>

namespace ashes::gl
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
			case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT:
			case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT:
			case VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT:
				if ( reinterpret_cast< gl::IcdObject const * >( object )->hasInternal() )
				{
					result = reinterpret_cast< gl::IcdObject const * >( object )->getInternal();
				}
				else
				{
					result = GL_INVALID_INDEX;
				}
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
			case VK_OBJECT_TYPE_IMAGE:
			case VK_OBJECT_TYPE_SAMPLER:
			case VK_OBJECT_TYPE_FRAMEBUFFER:
				if ( reinterpret_cast< gl::IcdObject const * >( object )->hasInternal() )
				{
					result = reinterpret_cast< gl::IcdObject const * >( object )->getInternal();
				}
				else
				{
					result = GL_INVALID_INDEX;
				}
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
		}

		template< typename T >
		T getSubresourceValue( T value, uint32_t mipLevel )
		{
			return T( value >> mipLevel );
		}

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
				texelBlockExtent.width = getSize( format );
			}

			return texelBlockExtent;
		}

		void doCheckEnabledExtensions( VkPhysicalDevice physicalDevice
			, StringArray const & extensions )
		{
			auto available = get( physicalDevice )->enumerateExtensionProperties( nullptr );

			for ( auto & extension : extensions )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&extension]( VkExtensionProperties const & lookup )
					{
						return lookup.extensionName == extension;
					} ) )
				{
					throw ExtensionNotPresentException{ extension };
				}
			}
		}

		void doCheckEnabledFeature( VkBool32 available
			, VkBool32 enabled )
		{
			if ( enabled && !available )
			{
				throw ashes::Exception{ VK_ERROR_FEATURE_NOT_PRESENT
					, "Enabled feature is not supported" };
			}
		}

		void doCheckEnabledFeatures( VkPhysicalDevice physicalDevice
			, VkPhysicalDeviceFeatures const & enabledFeatures )
		{
			auto available = get( physicalDevice )->getFeatures();
			doCheckEnabledFeature( available.robustBufferAccess, enabledFeatures.robustBufferAccess );
			doCheckEnabledFeature( available.fullDrawIndexUint32, enabledFeatures.fullDrawIndexUint32 );
			doCheckEnabledFeature( available.imageCubeArray, enabledFeatures.imageCubeArray );
			doCheckEnabledFeature( available.independentBlend, enabledFeatures.independentBlend );
			doCheckEnabledFeature( available.geometryShader, enabledFeatures.geometryShader );
			doCheckEnabledFeature( available.tessellationShader, enabledFeatures.tessellationShader );
			doCheckEnabledFeature( available.sampleRateShading, enabledFeatures.sampleRateShading );
			doCheckEnabledFeature( available.dualSrcBlend, enabledFeatures.dualSrcBlend );
			doCheckEnabledFeature( available.logicOp, enabledFeatures.logicOp );
			doCheckEnabledFeature( available.multiDrawIndirect, enabledFeatures.multiDrawIndirect );
			doCheckEnabledFeature( available.drawIndirectFirstInstance, enabledFeatures.drawIndirectFirstInstance );
			doCheckEnabledFeature( available.depthClamp, enabledFeatures.depthClamp );
			doCheckEnabledFeature( available.depthBiasClamp, enabledFeatures.depthBiasClamp );
			doCheckEnabledFeature( available.fillModeNonSolid, enabledFeatures.fillModeNonSolid );
			doCheckEnabledFeature( available.depthBounds, enabledFeatures.depthBounds );
			doCheckEnabledFeature( available.wideLines, enabledFeatures.wideLines );
			doCheckEnabledFeature( available.largePoints, enabledFeatures.largePoints );
			doCheckEnabledFeature( available.alphaToOne, enabledFeatures.alphaToOne );
			doCheckEnabledFeature( available.multiViewport, enabledFeatures.multiViewport );
			doCheckEnabledFeature( available.samplerAnisotropy, enabledFeatures.samplerAnisotropy );
			doCheckEnabledFeature( available.textureCompressionETC2, enabledFeatures.textureCompressionETC2 );
			doCheckEnabledFeature( available.textureCompressionASTC_LDR, enabledFeatures.textureCompressionASTC_LDR );
			doCheckEnabledFeature( available.textureCompressionBC, enabledFeatures.textureCompressionBC );
			doCheckEnabledFeature( available.occlusionQueryPrecise, enabledFeatures.occlusionQueryPrecise );
			doCheckEnabledFeature( available.pipelineStatisticsQuery, enabledFeatures.pipelineStatisticsQuery );
			doCheckEnabledFeature( available.vertexPipelineStoresAndAtomics, enabledFeatures.vertexPipelineStoresAndAtomics );
			doCheckEnabledFeature( available.fragmentStoresAndAtomics, enabledFeatures.fragmentStoresAndAtomics );
			doCheckEnabledFeature( available.shaderTessellationAndGeometryPointSize, enabledFeatures.shaderTessellationAndGeometryPointSize );
			doCheckEnabledFeature( available.shaderImageGatherExtended, enabledFeatures.shaderImageGatherExtended );
			doCheckEnabledFeature( available.shaderStorageImageExtendedFormats, enabledFeatures.shaderStorageImageExtendedFormats );
			doCheckEnabledFeature( available.shaderStorageImageMultisample, enabledFeatures.shaderStorageImageMultisample );
			doCheckEnabledFeature( available.shaderStorageImageReadWithoutFormat, enabledFeatures.shaderStorageImageReadWithoutFormat );
			doCheckEnabledFeature( available.shaderStorageImageWriteWithoutFormat, enabledFeatures.shaderStorageImageWriteWithoutFormat );
			doCheckEnabledFeature( available.shaderUniformBufferArrayDynamicIndexing, enabledFeatures.shaderUniformBufferArrayDynamicIndexing );
			doCheckEnabledFeature( available.shaderSampledImageArrayDynamicIndexing, enabledFeatures.shaderSampledImageArrayDynamicIndexing );
			doCheckEnabledFeature( available.shaderStorageBufferArrayDynamicIndexing, enabledFeatures.shaderStorageBufferArrayDynamicIndexing );
			doCheckEnabledFeature( available.shaderStorageImageArrayDynamicIndexing, enabledFeatures.shaderStorageImageArrayDynamicIndexing );
			doCheckEnabledFeature( available.shaderClipDistance, enabledFeatures.shaderClipDistance );
			doCheckEnabledFeature( available.shaderCullDistance, enabledFeatures.shaderCullDistance );
			doCheckEnabledFeature( available.shaderFloat64, enabledFeatures.shaderFloat64 );
			doCheckEnabledFeature( available.shaderInt64, enabledFeatures.shaderInt64 );
			doCheckEnabledFeature( available.shaderInt16, enabledFeatures.shaderInt16 );
			doCheckEnabledFeature( available.shaderResourceResidency, enabledFeatures.shaderResourceResidency );
			doCheckEnabledFeature( available.shaderResourceMinLod, enabledFeatures.shaderResourceMinLod );
			doCheckEnabledFeature( available.sparseBinding, enabledFeatures.sparseBinding );
			doCheckEnabledFeature( available.sparseResidencyBuffer, enabledFeatures.sparseResidencyBuffer );
			doCheckEnabledFeature( available.sparseResidencyImage2D, enabledFeatures.sparseResidencyImage2D );
			doCheckEnabledFeature( available.sparseResidencyImage3D, enabledFeatures.sparseResidencyImage3D );
			doCheckEnabledFeature( available.sparseResidency2Samples, enabledFeatures.sparseResidency2Samples );
			doCheckEnabledFeature( available.sparseResidency4Samples, enabledFeatures.sparseResidency4Samples );
			doCheckEnabledFeature( available.sparseResidency8Samples, enabledFeatures.sparseResidency8Samples );
			doCheckEnabledFeature( available.sparseResidency16Samples, enabledFeatures.sparseResidency16Samples );
			doCheckEnabledFeature( available.sparseResidencyAliased, enabledFeatures.sparseResidencyAliased );
			doCheckEnabledFeature( available.variableMultisampleRate, enabledFeatures.variableMultisampleRate );
			doCheckEnabledFeature( available.inheritedQueries, enabledFeatures.inheritedQueries );

		}

		bool doHasEnabledExtensions( StringArray const & available
			, ashes::ArrayView< char const * const > const & extensions )
		{
			try
			{
				for ( auto & extension : extensions )
				{
					if ( available.end() == std::find_if( available.begin()
						, available.end()
						, [&extension]( std::string const & lookup )
						{
							return lookup == std::string{ extension };
						} ) )
					{
						throw ExtensionNotPresentException{ extension };
					}
				}
				return true;
			}
			catch ( ExtensionNotPresentException & )
			{
				return false;
			}
		}
	}

	Device::Device( VkAllocationCallbacks const * callbacks
		, VkInstance instance
		, VkPhysicalDevice physicalDevice
		, VkDeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_physicalDevice{ physicalDevice }
		, m_callbacks{ callbacks }
		, m_createInfos{ std::move( createInfos ) }
		, m_enabledFeatures{ m_createInfos.pEnabledFeatures ? *m_createInfos.pEnabledFeatures : get( m_physicalDevice )->getFeatures() }
		, m_dyState{ VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT }
	{
		for ( auto & name : ashes::makeArrayView( m_createInfos.ppEnabledExtensionNames, m_createInfos.enabledExtensionCount ) )
		{
			m_enabledExtensions.emplace_back( name );
		}

		m_currentContext = get( m_instance )->registerDevice( get( this ) );
		doCheckEnabledExtensions( m_physicalDevice
			, m_enabledExtensions );
		doCheckEnabledFeatures( m_physicalDevice
			, m_enabledFeatures );
		doInitialiseQueues();
		doInitialiseContextDependent();
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
					deallocateNA( queue );
				}
			}

			doCleanupContextDependent();

			if ( m_dummyIndexed.indexMemory )
			{
				deallocate( m_dummyIndexed.indexMemory
					, getAllocationCallbacks() );
				deallocate( m_dummyIndexed.indexBuffer
					, getAllocationCallbacks() );
			}
		}

		reportRegisteredObjects();
		get( m_instance )->unregisterDevice( get( this ) );
	}

	void Device::cleanupContextDependent( Context const & context )
	{
		if ( &context == m_currentContext )
		{
			doCleanupContextDependent();
		}
	}

	void Device::cleanupBlitSrcFbo()
	{
		if ( m_blitFbos[0] )
		{
			deallocate( m_blitFbos[0]
				, getAllocationCallbacks() );
			m_blitFbos[0] = nullptr;
		}
	}

	void Device::cleanupBlitDstFbo()
	{
		if ( m_blitFbos[1] )
		{
			deallocate( m_blitFbos[1]
				, getAllocationCallbacks() );
			m_blitFbos[1] = nullptr;
		}
	}

	bool Device::hasExtension( std::string_view extension )const
	{
		char const * const version = extension.data();
		return doHasEnabledExtensions( m_enabledExtensions
			, ashes::makeArrayView( &version, 1u ) );
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
		auto target = convert( get( this )
			, get( image )->getType()
			, get( image )->getArrayLayers()
			, get( image )->getCreateFlags() );
		glLogCall( context
			, glBindTexture
			, target
			, get( image )->getInternal() );
		int w = 0;
		int h = 0;
		int d = 0;
		context->glGetTexLevelParameteriv( target, int( subresource.mipLevel ), GL_TEXTURE_WIDTH, &w );
		context->glGetTexLevelParameteriv( target, int( subresource.mipLevel ), GL_TEXTURE_HEIGHT, &h );
		context->glGetTexLevelParameteriv( target, int( subresource.mipLevel ), GL_TEXTURE_DEPTH, &d );
		glLogCall( context
			, glBindTexture
			, target
			, 0 );
		auto extent = getTexelBlockExtent( get( image )->getFormatVk() );
		layout.rowPitch = extent.width * getAligned( std::max( w, 1 ), extent.width );
		layout.arrayPitch = layout.rowPitch * getAligned( std::max( h, 1 ), extent.height );
		layout.depthPitch = layout.arrayPitch;
		layout.offset = subresource.arrayLayer * layout.arrayPitch;
		layout.size = layout.arrayPitch * std::max( d, 1 );
	}

	VkResult Device::waitIdle()const
	{
		if ( m_currentContext )
		{
			auto context = getContext();
			glLogEmptyCall( context
				, glFinish );
		}

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

		if ( nameInfo.objectType != VK_OBJECT_TYPE_FENCE )
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

	void Device::submitDebugUtilsMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const & callbackData )const
	{
		get( m_instance )->submitDebugUtilsMessenger( messageSeverity
			, messageTypes
			, callbackData );
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
#if VK_EXT_debug_report

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

#endif

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

	void Device::link( VkSurfaceKHR surface )
	{
		try
		{
			doCleanupContextDependent();

			if ( !m_ownContext )
			{
				m_ownContext = Context::create( m_instance
					, surface );
				m_currentContext = m_ownContext.get();
				get( m_instance )->registerContext( *m_ownContext );
			}

			doInitialiseContextDependent();
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

	void Device::unlink( VkSurfaceKHR surface )
	{
		doCleanupContextDependent();
	}

	ContextLock Device::getContext()const
	{
		assert( m_currentContext );
		return { *m_currentContext, get( this ) };
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
			allocateNA( queue
				, get( this )
				, it->second.createInfo );
			it->second.queues.emplace_back( queue );
		}
	}

	void Device::doInitialiseDummy()const
	{
		if ( !m_dummyIndexed.indexBuffer )
		{
			auto context = getContext();
			auto count = uint32_t( sizeof( gl::dummyIndex ) / sizeof( gl::dummyIndex[0] ) );
			allocate( m_dummyIndexed.indexBuffer
				, getAllocationCallbacks()
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
			auto deduced = deduceMemoryType( get( this )
				, requirements.memoryTypeBits
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
			allocate( m_dummyIndexed.indexMemory
				, getAllocationCallbacks()
				, get( this )
				, VkMemoryAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } );
			get( m_dummyIndexed.indexMemory )->bindBuffer( m_dummyIndexed.indexBuffer, 0u );
			auto memory = get( m_dummyIndexed.indexMemory );
			uint8_t * buffer{ nullptr };
			auto size = count * sizeof( uint32_t );

			if ( memory->lock( context, 0u, size, 0u, reinterpret_cast< void ** >( &buffer ) ) == VK_SUCCESS )
			{
				std::memcpy( buffer, gl::dummyIndex, size );
				memory->flush( context, 0, size );
				memory->unlock( context );
			}

			m_dummyIndexed.geometryBuffers = std::make_unique< GeometryBuffers >( get( this )
				, VboBindings{}
				, BufferObjectBinding{ indexBuffer->getInternal(), 0u, m_dummyIndexed.indexBuffer }
				, VkPipelineVertexInputStateCreateInfo{}
				, InputsLayout{}
				, VK_INDEX_TYPE_UINT32 );
			m_dummyIndexed.geometryBuffers->initialise( context );
		}
	}

	void Device::doInitialiseContextDependent()
	{
		auto lock = getContext();
		allocate( m_blitFbos[0]
			, getAllocationCallbacks()
			, get( this )
			, GL_INVALID_INDEX );
		allocate( m_blitFbos[1]
			, getAllocationCallbacks()
			, get( this )
			, GL_INVALID_INDEX );
		allocate( m_sampler
			, getAllocationCallbacks()
			, get( this )
			, VkSamplerCreateInfo
			{
				VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
				nullptr,
				0u,
				VK_FILTER_NEAREST,
				VK_FILTER_NEAREST,
				VK_SAMPLER_MIPMAP_MODE_NEAREST,
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
				VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
				0.0f,
				VK_FALSE,
				1.0f,
				VK_FALSE,
				VK_COMPARE_OP_ALWAYS,
				0.0f,
				1.0f,
				VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
				VK_FALSE,
			} );
	}

	void Device::doCleanupContextDependent()
	{
		if ( m_sampler )
		{
			deallocate( m_sampler
				, getAllocationCallbacks() );
			m_sampler = nullptr;
		}

		cleanupBlitSrcFbo();
		cleanupBlitDstFbo();
	}

	Device * Device::getDevice( VkDevice device )
	{
		return get( device );
	}

#if VK_EXT_debug_utils || VK_EXT_debug_marker

	void Device::doRegisterObject( uint64_t object
		, uint32_t objectType
		, std::string const & typeName )const
	{
		std::stringstream stream;
		stream.imbue( std::locale{ "C" } );
		stream << "Created " << typeName
			<< " [0x" << std::hex << std::setw( 8u ) << std::setfill( '0' ) << object << "]";
		m_allocated.emplace( object
			, ObjectAllocation{
				typeName
			} );
	}

	void Device::doUnregisterObject( uint64_t object )const
	{
		auto it = m_allocated.find( object );
		assert( it != m_allocated.end() );
		m_allocated.erase( it );
	}

	void Device::doReportRegisteredObjects()const
	{
		for ( auto & alloc : m_allocated )
		{
			std::stringstream stream;
			stream << "Leaked [" << alloc.second.type << "]";
			logError( stream.str().c_str() );
		}
	}

#endif

	bool has420PackExtensions( VkDevice device )
	{
		return has420PackExtensions( get( device )->getPhysicalDevice() );
	}

	bool hasCopyImage( VkDevice device )
	{
		return hasCopyImage( get( device )->getPhysicalDevice() );
	}

	bool hasProgramPipelines( VkDevice device )
	{
		return hasProgramPipelines( get( device )->getPhysicalDevice() );
	}

	bool hasSamplerAnisotropy( VkDevice device )
	{
		return hasSamplerAnisotropy( get( device )->getPhysicalDevice() );
	}

	bool hasTextureStorage( VkDevice device )
	{
		return hasTextureStorage( get( device )->getPhysicalDevice() );
	}

	bool hasTextureViews( VkDevice device )
	{
		return hasTextureViews( get( device )->getPhysicalDevice() );
	}

	bool hasViewportArrays( VkDevice device )
	{
		return hasViewportArrays( get( device )->getPhysicalDevice() );
	}

	bool hasProgramInterfaceQuery( VkDevice device )
	{
		return hasProgramInterfaceQuery( get( device )->getPhysicalDevice() );
	}
}
