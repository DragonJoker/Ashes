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

#include "ashesgl3_api.hpp"

#include <iostream>
#include <cstring>

namespace ashes::gl3
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

		void doApply( ContextLock const & context
			, VkPipelineColorBlendStateCreateInfo const & state )
		{
			if ( state.logicOpEnable )
			{
				glLogCall( context
					, glLogicOp
					, convert( state.logicOp ) );
			}

			auto & blendConstants = state.blendConstants;
			glLogCall( context
				, glBlendColor
				, blendConstants[0]
				, blendConstants[1]
				, blendConstants[2]
				, blendConstants[3] );

			bool blend = false;

			if ( context->hasBlendEquationSeparatei_40() )
			{
				uint32_t buf = 0;

				for ( auto & blendState : ashes::makeArrayView( state.pAttachments, state.attachmentCount ) )
				{
					if ( blendState.blendEnable )
					{
						blend = true;
						glLogCall( context
							, glBlendEquationSeparatei_40
							, buf
							, convert( blendState.colorBlendOp )
							, convert( blendState.alphaBlendOp ) );
						glLogCall( context
							, glBlendFuncSeparatei_40
							, buf
							, convert( blendState.srcColorBlendFactor )
							, convert( blendState.dstColorBlendFactor )
							, convert( blendState.srcAlphaBlendFactor )
							, convert( blendState.dstAlphaBlendFactor ) );
					}

					++buf;
				}
			}
			else
			{
				auto count = std::count_if( state.pAttachments
					, state.pAttachments + state.attachmentCount
					, []( VkPipelineColorBlendAttachmentState const & attach )
					{
						return attach.blendEnable;
					} );
				blend = count > 0;

				if ( count > 1 )
				{
					std::cerr << "Separate blend equations are not available." << std::endl;
				}

				if ( blend )
				{
					auto it = std::find_if( state.pAttachments
						, state.pAttachments + state.attachmentCount
						, []( VkPipelineColorBlendAttachmentState const & attach )
						{
							return attach.blendEnable;
						} );
					glLogCall( context
						, glBlendEquationSeparate
						, convert( it->colorBlendOp )
						, convert( it->alphaBlendOp ) );
					glLogCall( context
						, glBlendFuncSeparate
						, convert( it->srcColorBlendFactor )
						, convert( it->dstColorBlendFactor )
						, convert( it->srcAlphaBlendFactor )
						, convert( it->dstAlphaBlendFactor ) );
				}
			}

			if ( blend )
			{
				glLogCall( context
					, glEnable
					, GL_BLEND );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_BLEND );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineRasterizationStateCreateInfo const & state )
		{
			if ( state.cullMode != VK_CULL_MODE_NONE )
			{
				glLogCall( context
					, glEnable
					, GL_CULL_FACE );
				glLogCall( context
					, glCullFace
					, convert( state.cullMode ) );
				glLogCall( context
					, glFrontFace
					, convert( state.frontFace ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_CULL_FACE );
			}

			glLogCall( context
				, glPolygonMode
				, GL_CULL_MODE_FRONT_AND_BACK
				, convert( state.polygonMode ) );

			if ( state.depthBiasEnable )
			{
				switch ( state.polygonMode )
				{
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_FILL );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_LINE );
					break;

				case VK_POLYGON_MODE_POINT:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_POINT );
					break;
				}

				glLogCall( context
					, glPolygonOffsetClampEXT
					, state.depthBiasConstantFactor
					, state.depthBiasSlopeFactor
					, state.depthBiasClamp );
			}
			else
			{
				switch ( state.polygonMode )
				{
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_FILL );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_LINE );
					break;

				case VK_POLYGON_MODE_POINT:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_POINT );
					break;
				}
			}

			if ( state.depthClampEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_CLAMP );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_CLAMP );
			}

			if ( state.rasterizerDiscardEnable )
			{
				glLogCall( context
					, glEnable
					, GL_RASTERIZER_DISCARD );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_RASTERIZER_DISCARD );
			}

			glLogCall( context
				, glLineWidth
				, state.lineWidth );
		}

		void doApply( ContextLock const & context
			, VkPipelineMultisampleStateCreateInfo const & state )
		{
			if ( state.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT )
			{
				glLogCall( context
					, glEnable
					, GL_MULTISAMPLE );

				if ( state.alphaToCoverageEnable )
				{
					glLogCall( context
						, glEnable
						, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}

				if ( state.alphaToOneEnable )
				{
					glLogCall( context
						, glEnable
						, GL_SAMPLE_ALPHA_TO_ONE );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_SAMPLE_ALPHA_TO_ONE );
				}
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_MULTISAMPLE );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineDepthStencilStateCreateInfo const & state )
		{
			if ( state.depthWriteEnable )
			{
				glLogCall( context
					, glDepthMask
					, GL_TRUE );
			}
			else
			{
				glLogCall( context
					, glDepthMask
					, GL_FALSE );
			}

			if ( state.depthTestEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_TEST );
				glLogCall( context
					, glDepthFunc
					, convert( state.depthCompareOp ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_TEST );
			}

			if ( state.stencilTestEnable )
			{
				glLogCall( context
					, glEnable
					, GL_STENCIL_TEST );
				glLogCall( context
					, glStencilMaskSeparate
					, GL_CULL_MODE_BACK
					, state.back.writeMask );
				glLogCall( context
					, glStencilFuncSeparate
					, GL_CULL_MODE_BACK
					, convert( state.back.compareOp )
					, state.back.reference
					, state.back.compareMask );
				glLogCall( context
					, glStencilOpSeparate
					, GL_CULL_MODE_BACK
					, convert( state.back.failOp )
					, convert( state.back.depthFailOp )
					, convert( state.back.passOp ) );
				glLogCall( context
					, glStencilMaskSeparate
					, GL_CULL_MODE_FRONT
					, state.front.writeMask );
				glLogCall( context
					, glStencilFuncSeparate
					, GL_CULL_MODE_FRONT
					, convert( state.front.compareOp )
					, state.front.reference
					, state.front.compareMask );
				glLogCall( context
					, glStencilOpSeparate
					, GL_CULL_MODE_FRONT
					, convert( state.front.failOp )
					, convert( state.front.depthFailOp )
					, convert( state.front.passOp ) );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_STENCIL_TEST );
			}

			if ( state.depthBoundsTestEnable )
			{
				glLogCall( context
					, glEnable
					, GL_DEPTH_CLAMP );
				glLogCall( context
					, glDepthRange
					, state.minDepthBounds
					, state.maxDepthBounds );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_DEPTH_CLAMP );
			}
		}

		void doApply( ContextLock const & context
			, VkPipelineTessellationStateCreateInfo const & state )
		{
		}

		void doApply( ContextLock const & context
			, VkPipelineInputAssemblyStateCreateInfo const & state )
		{
			if ( state.topology == VK_PRIMITIVE_TOPOLOGY_POINT_LIST )
			{
				glLogCall( context
					, glEnable
					, GL_PROGRAM_POINT_SIZE );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_PROGRAM_POINT_SIZE );
			}

			if ( state.primitiveRestartEnable )
			{
				glLogCall( context
					, glEnable
					, GL_PRIMITIVE_RESTART );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_PRIMITIVE_RESTART );
			}
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

		void doCheckEnabledExtensions( ashes::ArrayView< char const * const > const & extensions )
		{
			auto & available = getSupportedDeviceExtensions();

			for ( auto & extension : extensions )
			{
				if ( available.end() == std::find_if( available.begin()
					, available.end()
					, [&extension]( VkExtensionProperties const & lookup )
					{
						return lookup.extensionName == std::string{ extension };
					} ) )
				{
					throw ashes::Exception{ VK_ERROR_EXTENSION_NOT_PRESENT, extension };
				}
			}
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
		doCheckEnabledExtensions( ashes::makeArrayView( m_createInfos.ppEnabledExtensionNames, m_createInfos.enabledExtensionCount ) );
		doInitialiseQueues();
		auto lock = getContext();
		allocate( m_blitFbos[0]
			, nullptr
			, get( this )
			, GL_INVALID_INDEX );
		allocate( m_blitFbos[1]
			, nullptr
			, get( this )
			, GL_INVALID_INDEX );
		allocate( m_sampler
			, nullptr
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

			deallocate( m_sampler, nullptr );
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
		auto target = convert( get( image )->getType(), get( image )->getArrayLayers(), get( image )->getCreateFlags() );
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

	void Device::doInitialiseDummy()const
	{
		if ( !m_dummyIndexed.indexBuffer )
		{
			auto context = getContext();
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
				std::memcpy( buffer, dummyIndex, size );
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
}
