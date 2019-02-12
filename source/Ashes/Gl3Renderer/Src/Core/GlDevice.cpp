/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlDevice.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Buffer/GlBufferView.hpp"
#include "Buffer/GlGeometryBuffers.hpp"
#include "Buffer/GlUniformBuffer.hpp"
#include "Command/GlCommandPool.hpp"
#include "Command/GlQueue.hpp"
#include "Core/GlConnection.hpp"
#include "Core/GlContext.hpp"
#include "Core/GlDummyIndexBuffer.hpp"
#include "Core/GlRenderer.hpp"
#include "Core/GlSwapChain.hpp"
#include "Descriptor/GlDescriptorPool.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Image/GlSampler.hpp"
#include "Image/GlStagingTexture.hpp"
#include "Image/GlTexture.hpp"
#include "Image/GlTextureView.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "Miscellaneous/GlQueryPool.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderModule.hpp"
#include "Sync/GlEvent.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"

#include <Image/ImageSubresource.hpp>
#include <Image/SubresourceLayout.hpp>
#include <RenderPass/RenderPassCreateInfo.hpp>

#include <algorithm>
#include <iostream>

namespace gl_renderer
{
	namespace
	{
		GLuint getObjectName( GlDebugReportObjectType const & value
			, void const * object )
		{
			GLuint result = GL_INVALID_INDEX;

			switch ( value )
			{
			case GlDebugReportObjectType::eBuffer:
				result = reinterpret_cast< Buffer const * >( object )->getBuffer();
				break;
			case GlDebugReportObjectType::eTexture:
				result = reinterpret_cast< Texture const * >( object )->getImage();
				break;
			case GlDebugReportObjectType::eQuery:
				result = *reinterpret_cast< QueryPool const * >( object )->begin();
				break;
			case GlDebugReportObjectType::eShaderModule:
				result = reinterpret_cast< ShaderModule const * >( object )->getShader();
				break;
			case GlDebugReportObjectType::eSampler:
				result = reinterpret_cast< Sampler const * >( object )->getSampler();
				break;
			case GlDebugReportObjectType::eFrameBuffer:
				result = reinterpret_cast< FrameBuffer const * >( object )->getFrameBuffer();
				break;
			}

			return result;
		}

		void doApply( ContextLock const & context
			, ashes::ColourBlendState const & state )
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

				for ( auto & blendState : state.attachs )
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
				auto count = std::count_if( state.attachs.begin()
					, state.attachs.end()
					, []( ashes::ColourBlendStateAttachment const & attach )
					{
						return attach.blendEnable;
					} );
				blend = count > 0;

				if ( count > 1 )
				{
					ashes::Logger::logWarning( "Separate blend equations are not available." );
				}

				if ( blend )
				{
					auto it = std::find_if( state.attachs.begin()
						, state.attachs.end()
						, []( ashes::ColourBlendStateAttachment const & attach )
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
			, ashes::RasterisationState const & state )
		{
			if ( state.cullMode != ashes::CullModeFlag::eNone )
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
				case ashes::PolygonMode::eFill:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_FILL );
					break;

				case ashes::PolygonMode::eLine:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_LINE );
					break;

				case ashes::PolygonMode::ePoint:
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
				case ashes::PolygonMode::eFill:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_FILL );
					break;

				case ashes::PolygonMode::eLine:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_LINE );
					break;

				case ashes::PolygonMode::ePoint:
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

			if ( state.rasteriserDiscardEnable )
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
			, ashes::MultisampleState const & state )
		{
			if ( state.rasterisationSamples != ashes::SampleCountFlag::e1 )
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
			, ashes::DepthStencilState const & state )
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
			, ashes::TessellationState const & state )
		{
			if ( state.patchControlPoints )
			{
				glLogCall( context
					, glPatchParameteri_ARB
					, GL_PATCH_VERTICES
					, int( state.patchControlPoints ) );
			}
		}

		void doApply( ContextLock const & context
			, ashes::InputAssemblyState const & state )
		{
			if ( state.topology == ashes::PrimitiveTopology::ePointList )
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
	}

	Device::Device( Renderer const & renderer
		, PhysicalDevice const & gpu
		, ashes::ConnectionPtr && connection )
		: ashes::Device{ renderer, gpu, *connection }
		, m_renderer{ renderer }
		, m_context{ Context::create( gpu, *connection, nullptr ) }
		, m_rsState{}
	{
		auto context = getContext();
		//glLogCall( context
		//	, glClipControl
		//	, GL_UPPER_LEFT
		//	, GL_ZERO_TO_ONE );
		glLogCall( context
			, glEnable
			, GL_TEXTURE_CUBE_MAP_SEAMLESS );

		m_timestampPeriod = 1;
		m_presentQueue = std::make_unique< Queue >( *this );
		m_computeQueue = std::make_unique< Queue >( *this );
		m_graphicsQueue = std::make_unique< Queue >( *this );
		m_presentCommandPool = std::make_unique< CommandPool >( *this, 0u );
		m_computeCommandPool = std::make_unique< CommandPool >( *this, 0u );
		m_graphicsCommandPool = std::make_unique< CommandPool >( *this, 0u );

		doApply( context, m_cbState );
		doApply( context, m_dsState );
		doApply( context, m_msState );
		doApply( context, m_rsState );
		doApply( context, m_iaState );

		if ( m_gpu.getFeatures().tessellationShader )
		{
			doApply( context, m_tsState );
		}

		auto count = uint32_t( sizeof( dummyIndex ) / sizeof( dummyIndex[0] ) );
		m_dummyIndexed.indexBuffer = ashes::makeBuffer< uint32_t >( *this
			, count
			, ashes::BufferTarget::eIndexBuffer
			, ashes::MemoryPropertyFlag::eHostVisible );

		if ( auto * buffer = m_dummyIndexed.indexBuffer->lock( 0u
			, count
			, ashes::MemoryMapFlag::eWrite ) )
		{
			std::copy( dummyIndex, dummyIndex + count, buffer );
			m_dummyIndexed.indexBuffer->flush( 0, count );
			m_dummyIndexed.indexBuffer->unlock();
		}

		auto & indexBuffer = static_cast< Buffer const & >( m_dummyIndexed.indexBuffer->getBuffer() );
		m_dummyIndexed.geometryBuffers = std::make_unique< GeometryBuffers >( *this
			, VboBindings{}
			, BufferObjectBinding{ indexBuffer.getBuffer(), 0u, &indexBuffer }
			, ashes::VertexInputState{}
			, ashes::IndexType::eUInt32 );
		m_dummyIndexed.geometryBuffers->initialise();

		context->glGenFramebuffers( 2, m_blitFbos );
	}

	Device::~Device()
	{
		{
			auto context = getContext();
			context->glDeleteFramebuffers( 2, m_blitFbos );
			m_dummyIndexed.geometryBuffers.reset();
			m_dummyIndexed.indexBuffer.reset();
		}
		m_context.reset();

		m_graphicsCommandPool.reset();
		m_graphicsQueue.reset();
		m_presentCommandPool.reset();
		m_presentQueue.reset();
		m_computeCommandPool.reset();
		m_computeQueue.reset();
	}

	ashes::StagingTexturePtr Device::createStagingTexture( ashes::Format format
		, ashes::Extent2D const & extent )const
	{
		return std::make_unique< StagingTexture >( *this, format, extent );
	}

	ashes::RenderPassPtr Device::createRenderPass( ashes::RenderPassCreateInfo createInfo )const
	{
		return std::make_unique< RenderPass >( *this, std::move( createInfo ) );
	}

	ashes::PipelineLayoutPtr Device::createPipelineLayout( ashes::DescriptorSetLayoutCRefArray const & setLayouts
		, ashes::PushConstantRangeArray const & pushConstantRanges )const
	{
		return std::make_unique< PipelineLayout >( *this
			, setLayouts
			, pushConstantRanges );
	}

	ashes::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( ashes::DescriptorSetLayoutBindingArray && bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	ashes::DescriptorPoolPtr Device::createDescriptorPool( ashes::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::DescriptorPoolSizeArray poolSizes )const
	{
		return std::make_unique< DescriptorPool >( *this, flags, maxSets, poolSizes );
	}

	ashes::DeviceMemoryPtr Device::allocateMemory( ashes::MemoryRequirements const & requirements
		, ashes::MemoryPropertyFlags flags )const
	{
		return std::make_unique< DeviceMemory >( *this
			, requirements
			, flags );
	}

	ashes::TexturePtr Device::createTexture( ashes::ImageCreateInfo const & createInfo )const
	{
		return std::make_unique< Texture >( *this, createInfo );
	}

	void Device::getImageSubresourceLayout( ashes::Texture const & image
		, ashes::ImageSubresource const & subresource
		, ashes::SubresourceLayout & layout )const
	{
		auto & gltex = static_cast< Texture const & >( image );
		auto context = getContext();
		auto target = convert( gltex.getType(), gltex.getLayerCount(), gltex.getFlags() );
		glLogCall( context
			, glBindTexture
			, target
			, gltex.getImage() );
		int w = 0;
		int h = 0;
		int d = 0;
		int red = 0;
		int green = 0;
		int blue = 0;
		int alpha = 0;
		int depth = 0;
		int stencil = 0;
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_WIDTH, &w );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_HEIGHT, &h );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_DEPTH, &d );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_RED_SIZE, &red );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_GREEN_SIZE, &green );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_BLUE_SIZE, &blue );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_ALPHA_SIZE, &alpha );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_DEPTH_SIZE, &depth );
		context->glGetTexLevelParameteriv( target, subresource.mipLevel, GL_TEXTURE_STENCIL_SIZE, &stencil );
		layout.rowPitch = 0u;
		layout.arrayPitch = 0u;
		layout.depthPitch = 0u;
		layout.size = std::max( w, 1 )  * std::max( d, 1 ) * std::max( h, 1 ) * ( red + green + blue + alpha + depth + stencil );
		layout.offset = 0;
		glLogCall( context
			, glBindTexture
			, target
			, 0 );
	}

	ashes::SamplerPtr Device::createSampler( ashes::SamplerCreateInfo const & createInfo )const
	{
		return std::make_unique< Sampler >( *this, createInfo );
	}

	ashes::BufferBasePtr Device::createBuffer( uint32_t size
		, ashes::BufferTargets target )const
	{
		return std::make_unique< Buffer >( *this
			, size
			, target );
	}

	ashes::BufferViewPtr Device::createBufferView( ashes::BufferBase const & buffer
		, ashes::Format format
		, uint32_t offset
		, uint32_t range )const
	{
		return std::make_unique< BufferView >( *this
			, static_cast< Buffer const & >( buffer )
			, format
			, offset
			, range );
	}

	ashes::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, ashes::BufferTargets target
		, ashes::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< UniformBuffer >( *this
			, count
			, size
			, target
			, memoryFlags );
	}

	ashes::SwapChainPtr Device::createSwapChain( ashes::Extent2D const & size )const
	{
		ashes::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not create the swap chain:\n" } + exc.what() );
		}
		catch ( ... )
		{
			ashes::Logger::logError( "Could not create the swap chain:\nUnknown error" );
		}

		return result;
	}

	ashes::SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	ashes::FencePtr Device::createFence( ashes::FenceCreateFlags flags )const
	{
		return std::make_unique< Fence >( *this, flags );
	}

	ashes::EventPtr Device::createEvent()const
	{
		return std::make_unique< Event >( *this );
	}

	ashes::CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	ashes::ShaderModulePtr Device::createShaderModule( ashes::ShaderStageFlag stage )const
	{
		return std::make_shared< ShaderModule >( *this, stage );
	}

	ashes::QueryPoolPtr Device::createQueryPool( ashes::QueryType type
		, uint32_t count
		, ashes::QueryPipelineStatisticFlags pipelineStatistics )const
	{
		return std::make_unique< QueryPool >( *this
			, type
			, count
			, pipelineStatistics );
	}

	void Device::debugMarkerSetObjectName( ashes::DebugMarkerObjectNameInfo const & nameInfo )const
	{
#if !defined( NDEBUG )
		auto context = getContext();

		if ( nameInfo.objectType == ashes::DebugReportObjectType::eFence )
		{
			glLogCall( context
				, glObjectPtrLabel
				, reinterpret_cast< Fence const * >( nameInfo.object )->getSync()
				, GLsizei( nameInfo.objectName.size() )
				, nameInfo.objectName.c_str() );
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
						, GLsizei( nameInfo.objectName.size() )
						, nameInfo.objectName.c_str() );
				}
			}
		}
#endif
	}

	void Device::waitIdle()const
	{
		auto context = getContext();
		glLogCall( context
			, glFinish );
	}

	void Device::swapBuffers()const
	{
		getContext()->swapBuffers();
	}
}
