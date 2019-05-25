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
				result = GL_INVALID_INDEX;
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

		UInt32Array convert( std::string const & value )
		{
			auto size = getAlignedSize( value.size(), sizeof( uint32_t ) );
			UInt32Array result;
			result.resize( size / sizeof( uint32_t ) );
			std::memcpy( result.data(), value.data(), value.size() );
			return result;
		}

		UInt32Array getVertexShader()
		{
			static std::string const glsl
			{
				R"(#version 420

vec4 points[4] = vec4[4](
	vec4( -1.0, -1.0, 1.0, 1.0 ),
	vec4( -1.0,  1.0, 1.0, 1.0 ),
	vec4(  1.0,  1.0, 1.0, 1.0 ),
	vec4(  1.0, -1.0, 1.0, 1.0 ),
);

layout( location = 0 ) out vec2 vtx_texture;

void main()
{
	vec4 position = points[gl_VertexID % 4];
	vtx_texture = ( position.xy + 1.0 ) / 2.0;
	gl_Position = position;
}
)"
			};
			return convert( glsl );
		}

		UInt32Array getPixelShader()
		{
			static std::string const glsl
			{
				R"(#version 420

layout( binding = 0 ) uniform sampler2D mapColour;

layout( location = 0 ) in vec2 vtx_texture;

layout( location = 0 ) out vec4 pxl_colour;

void main()
{
	pxl_colour = texture( mapColour, vtx_texture );
}
)"
			};
			return convert( glsl );
		}

		std::unique_ptr< ShaderProgram > doCreateShaderProgram( VkDevice device
			, ContextLock & context
			, ContextState const & state )
		{
			VkPipelineShaderStageCreateInfoArray shaderStages;
			UInt32Array vtx = getVertexShader();
			UInt32Array pxl = getPixelShader();
			VkShaderModule vtxModule;
			allocate( vtxModule
				, nullptr
				, device
				, VkShaderModuleCreateInfo
				{
					VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					nullptr,
					0u,
					uint32_t( vtx.size() * sizeof( uint32_t ) ),
					vtx.data()
				} );
			VkShaderModule pxlModule;
			allocate( pxlModule
				, nullptr
				, device
				, VkShaderModuleCreateInfo
				{
					VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					nullptr,
					0u,
					uint32_t( vtx.size() * sizeof( uint32_t ) ),
					vtx.data()
				} );

			shaderStages.push_back( VkPipelineShaderStageCreateInfo
				{
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					nullptr,
					0u,
					VK_SHADER_STAGE_VERTEX_BIT,
					vtxModule,
					"main",
					nullptr,
				} );
			shaderStages.push_back( VkPipelineShaderStageCreateInfo
				{
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					nullptr,
					0u,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					pxlModule,
					"main",
					nullptr,
				} );

			context->apply( context, *get( device ), state );
			auto result = std::make_unique< ShaderProgram >( device
				, nullptr
				, std::move( shaderStages )
				, true );
			result->link( context );
			return result;
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
		, m_rtocContextState
		{
			getDefaultColorBlendState( m_cbStateAttachments ),
			std::nullopt,
			std::nullopt,
			std::nullopt,
			{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_FALSE },
			{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, nullptr, 0u, 1, nullptr, 1u, nullptr },
			{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, nullptr, 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, GL_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f },
			{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0u, uint32_t( m_dyState.size() ), m_dyState.data() },
		}
		, m_rtocCommands{ get( this ), VK_COMMAND_BUFFER_LEVEL_PRIMARY }
	{
		//m_timestampPeriod = 1;
		doInitialiseQueues();
		auto lock = getContext();
		doInitialiseDummy( lock );
		GLuint fbos[2];
		lock->glGenFramebuffers( 2, fbos );
		allocate( m_blitFbos[0]
			, nullptr
			, get( this )
			, fbos[0] );
		allocate( m_blitFbos[1]
			, nullptr
			, get( this )
			, fbos[1] );
		doInitialiseRtoc( lock );
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

		if ( it->second.queues.size() <= index )
		{
			throw Exception{ VK_ERROR_INCOMPATIBLE_DRIVER
				, "Couldn't find queue with wanted index within its family" };
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
			std::cerr << exc.what() << std::endl;
			throw;
		}
	}

	void Device::unregisterContext( VkSurfaceKHR surface )const
	{
	}

	GLuint Device::getRtocProgram()const
	{
		assert( m_rtocProgram );
		return m_rtocProgram->getProgram();
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

	void Device::doInitialiseRtoc( ContextLock & context )
	{
		//m_rtocProgram = doCreateShaderProgram( get( this )
		//	, context
		//	, m_rtocContextState );
		//m_rtocCommands.begin( );
	}
}
