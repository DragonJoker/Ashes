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
#include "Core/GlSurface.hpp"
#include "Core/GlContext.hpp"
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

#include <AshesRenderer/Helper/ColourBlendState.hpp>
#include <AshesRenderer/Helper/DepthStencilState.hpp>
#include <AshesRenderer/Helper/InputAssemblyState.hpp>
#include <AshesRenderer/Helper/MultisampleState.hpp>
#include <AshesRenderer/Helper/RasterisationState.hpp>
#include <AshesRenderer/Helper/TessellationState.hpp>

#include <iostream>

namespace ashes::gl4
{
	namespace
	{
		GLuint getObjectName( GlDebugReportObjectType const & value
			, uint64_t object )
		{
			GLuint result = GL_INVALID_INDEX;

			switch ( value )
			{
			case GlDebugReportObjectType::eBuffer:
				result = reinterpret_cast< Buffer const * >( object )->getInternal();
				break;
			case GlDebugReportObjectType::eTexture:
				result = reinterpret_cast< Image const * >( object )->getInternal();
				break;
			case GlDebugReportObjectType::eQuery:
				result = *reinterpret_cast< QueryPool const * >( object )->begin();
				break;
			case GlDebugReportObjectType::eShaderModule:
				result = reinterpret_cast< ShaderModule const * >( object )->getInternal();
				break;
			case GlDebugReportObjectType::eSampler:
				result = reinterpret_cast< Sampler const * >( object )->getInternal();
				break;
			case GlDebugReportObjectType::eFrameBuffer:
				result = reinterpret_cast< Framebuffer const * >( object )->getInternal();
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
			GLuint buf = 0u;

			for ( auto it = state.pAttachments;
				it != state.pAttachments + state.attachmentCount;
				++it )
			{
				auto & blendState = *it;

				if ( blendState.blendEnable )
				{
					blend = true;
					glLogCall( context
						, glBlendEquationSeparatei
						, buf
						, convert( blendState.colorBlendOp )
						, convert( blendState.alphaBlendOp ) );
					glLogCall( context
						, glBlendFuncSeparatei
						, buf
						, convert( blendState.srcColorBlendFactor )
						, convert( blendState.dstColorBlendFactor )
						, convert( blendState.srcAlphaBlendFactor )
						, convert( blendState.dstAlphaBlendFactor ) );
				}

				++buf;
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
					, convertCullModeFlags( state.cullMode ) );
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
					glLogCall( context, glEnable, GL_SAMPLE_ALPHA_TO_ONE );
				}
				else
				{
					glLogCall( context, glDisable, GL_SAMPLE_ALPHA_TO_ONE );
				}
			}
			else
			{
				glLogCall( context, glDisable, GL_MULTISAMPLE );
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
			if ( state.patchControlPoints )
			{
				glLogCall( context
					, glPatchParameteri
					, GL_PATCH_VERTICES
					, int( state.patchControlPoints ) );
			}
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

		uint32_t getTexelBlockByteSize( VkExtent3D const & texelBlockExtent
			, VkFormat format )
		{
			uint32_t texelBlockSize;

			if ( !isDepthStencilFormat( format ) )
			{
				texelBlockSize = getSize( texelBlockExtent, format );
			}
			else
			{
				texelBlockSize = texelBlockExtent.width;
			}

			return texelBlockSize;
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
		, m_cbStateAttachments{ 1u, getColourBlendStateAttachment() }
		, m_cbState{ getColourBlendState( m_cbStateAttachments ) }
		, m_dsState{ getDepthStencilState() }
		, m_msState{ getMultisampleState() }
		, m_rsState{ getRasterisationState() }
		, m_tsState{ getTessellationState() }
		, m_iaState{ getInputAssemblyState() }
	{
		//m_timestampPeriod = 1;
		doCreateQueues();
		auto lock = getContext();
		doInitialiseContext( lock );

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
		VkDeviceMemory vkMemory;
		allocate( vkMemory
			, nullptr
			, get( this )
			, VkMemoryAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, requirements.size, deduced } );
		get( vkMemory )->bindToBuffer( m_dummyIndexed.indexBuffer, 0u );
		auto memory = get( vkMemory );
		uint8_t * buffer{ nullptr };
		memory->lock( 0u, count, 0u, reinterpret_cast< void ** >( &buffer ) );

		if ( buffer )
		{
			std::copy( dummyIndex, dummyIndex + count, buffer );
			memory->flush( 0, count );
			memory->unlock();
		}

		m_dummyIndexed.geometryBuffers = std::make_unique< GeometryBuffers >( get( this )
			, VboBindings{}
			, BufferObjectBinding{ indexBuffer->getInternal(), 0u, m_dummyIndexed.indexBuffer }
			, VkPipelineVertexInputStateCreateInfo{}
			, VK_INDEX_TYPE_UINT32 );
		m_dummyIndexed.geometryBuffers->initialise();

		lock->glGenFramebuffers( 2, m_blitFbos );
	}

	Device::~Device()
	{
		if ( m_ownContext )
		{
			auto context = getContext();
			context->glDeleteFramebuffers( 2, m_blitFbos );
			m_dummyIndexed.geometryBuffers.reset();
			deallocate( m_dummyIndexed.indexBuffer, nullptr );
		}
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

	void Device::debugMarkerSetObjectName( VkDebugMarkerObjectNameInfoEXT const & nameInfo )const
	{
#if !defined( NDEBUG )
		auto context = getContext();

		if ( nameInfo.objectType == VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT )
		{
			glLogCall( context
				, glObjectPtrLabel
				, get( VkFence( nameInfo.object ) )->getInternal()
				, GLsizei( strlen( nameInfo.pObjectName ) )
				, nameInfo.pObjectName );
		}
		else
		{
			auto objectType = convert( nameInfo.objectType );

			if ( objectType != GlDebugReportObjectType::eUnknown )
			{
				auto name = getObjectName( objectType, nameInfo.object );

				if ( name != GL_INVALID_INDEX )
				{
					glLogCall( context
						, glObjectLabel
						, GLenum( objectType )
						, name
						, GLsizei( strlen( nameInfo.pObjectName ) )
						, nameInfo.pObjectName );
				}
			}
		}
#endif
	}

	VkQueue Device::getQueue( uint32_t familyIndex
		, uint32_t index )const
	{
		auto it = m_queues.find( familyIndex );

		if ( it == m_queues.end() )
		{
			throw Exception{ VK_ERROR_INCOMPATIBLE_DRIVER
				, "Couldn't find family index within created queues" };
		}

		if ( it->second.second <= index )
		{
			throw Exception{ VK_ERROR_INCOMPATIBLE_DRIVER
				, "Couldn't find queue with wanted index within its family" };
		}

		VkQueue result;
		allocate( result
			, nullptr
			, get( this )
			, it->second.first
			, index );
		return result;
	}

	VkResult Device::waitIdle()const
	{
		auto context = getContext();
		glLogCall( context
			, glFinish );
		return VK_SUCCESS;
	}

	void Device::registerContext( VkSurfaceKHR surface )const
	{
		auto context = Context::create( m_instance, surface, nullptr );
		ContextLock lock{ *context };
		doInitialiseContext( lock );

		if ( !m_ownContext )
		{
			m_ownContext = std::move( context );
			m_currentContext = m_ownContext.get();
		}
	}

	void Device::unregisterContext( VkSurfaceKHR surface )const
	{
	}

	void Device::swapBuffers()const
	{
		getContext()->swapBuffers();
	}

	void Device::doInitialiseContext( ContextLock const & context )const
	{
		glLogCall( context
			, glEnable
			, GL_TEXTURE_CUBE_MAP_SEAMLESS );

		doApply( context, m_cbState );
		doApply( context, m_dsState );
		doApply( context, m_msState );
		doApply( context, m_rsState );
		doApply( context, m_iaState );

		if ( get( m_physicalDevice )->getFeatures().tessellationShader )
		{
			doApply( context, m_tsState );
		}
	}

	void Device::doCreateQueues()
	{
		for ( auto itQueue = m_createInfos.pQueueCreateInfos;
			itQueue != m_createInfos.pQueueCreateInfos + m_createInfos.queueCreateInfoCount;
			++itQueue )
		{
			auto & queueCreateInfo = *itQueue;
			auto it = m_queues.emplace( queueCreateInfo.queueFamilyIndex
				, VkQueueCreateCount{ queueCreateInfo, 0u } ).first;
			it->second.second++;
		}
	}
}
