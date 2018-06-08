/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/Device.hpp"

#include "Buffer/Buffer.hpp"
#include "Core/Renderer.hpp"
#include "Core/SwapChain.hpp"
#include "Image/Sampler.hpp"
#include "Miscellaneous/MemoryRequirements.hpp"
#include "Pipeline/PipelineLayout.hpp"
#include "Pipeline/VertexInputState.hpp"
#include "RenderPass/RenderPass.hpp"
#include "RenderPass/RenderPassCreateInfo.hpp"
#include "RenderPass/RenderSubpass.hpp"
#include "RenderPass/RenderSubpassState.hpp"
#include "Utils/CallStack.hpp"

namespace renderer
{
	Device::Device( Renderer const & renderer
		, PhysicalDevice const & gpu
		, Connection const & connection )
		: m_renderer{ renderer }
		, m_gpu{ gpu }
	{
	}

	Device::~Device()
	{
		reportRegisteredObjects();
	}

	void Device::enable()const
	{
		doEnable();
		onEnabled( *this );
	}

	void Device::disable()const
	{
		onDisabled( *this );
		doDisable();
	}

	std::array< float, 16u > Device::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_renderer.frustum( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		return m_renderer.perspective( radiansFovY, aspect, zNear, zFar );
	}

	std::array< float, 16u > Device::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_renderer.ortho( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		return m_renderer.infinitePerspective( radiansFovY, aspect, zNear );
	}

	BufferBasePtr Device::createBuffer( uint32_t size
		, BufferTargets target
		, MemoryPropertyFlags flags )const
	{
		auto result = createBuffer( size, target );
		result->bindMemory( allocateMemory( result->getMemoryRequirements(), flags ) );
		return result;
	}

	TexturePtr Device::createTexture( ImageCreateInfo const & createInfo
		, MemoryPropertyFlags flags )const
	{
		auto result = createTexture( createInfo );
		result->bindMemory( allocateMemory( result->getMemoryRequirements(), flags ) );
		return result;
	}

	RenderPassPtr Device::createRenderPass( AttachmentDescriptionArray const & attaches
		, RenderSubpassPtrArray && subpasses
		, RenderSubpassState const & initialState
		, RenderSubpassState const & finalState )const
	{
		RenderPassCreateInfo createInfo;
		createInfo.attachments = attaches;
		createInfo.subpasses.reserve( subpasses.size() );
		createInfo.dependencies.reserve( 1 + subpasses.size() );
		auto pipelineStage = initialState.pipelineStage;
		auto access = initialState.access;
		uint32_t subpassIndex = ExternalSubpass;

		for ( auto const & subpass : subpasses )
		{
			auto const & state = subpass->getNeededState();

			if ( pipelineStage != state.pipelineStage
				|| access != state.access )
			{
				createInfo.dependencies.push_back(
				{
					subpassIndex,                                    // srcSubpass
					subpassIndex + 1,                                // dstSubpass
					pipelineStage,                                   // srcStageMask
					state.pipelineStage,                             // dstStageMask
					access,                                          // srcAccessMask
					state.access,                                    // dstAccessMask
					DependencyFlag::eByRegion                        // dependencyFlags
				} );
			}

			pipelineStage = state.pipelineStage;
			access = state.access;
			++subpassIndex;
			createInfo.subpasses.push_back( subpass->getDescription() );
		}

		if ( pipelineStage != finalState.pipelineStage
			|| access != finalState.access )
		{
			createInfo.dependencies.push_back(
			{
				subpassIndex,                                        // srcSubpass
				ExternalSubpass,                                     // dstSubpass
				pipelineStage,                                       // srcStageMask
				finalState.pipelineStage,                            // dstStageMask
				access,                                              // srcAccessMask
				finalState.access,                                   // dstAccessMask
				DependencyFlag::eByRegion                            // dependencyFlags
			} );
		}

		return createRenderPass( createInfo );
	}

	ClipDirection Device::getClipDirection()const
	{
		return m_renderer.getClipDirection();
	}

	PipelineLayoutPtr Device::createPipelineLayout()const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, PushConstantRangeCRefArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, PushConstantRangeCRefArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( PushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, PushConstantRangeCRefArray{ pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout
		, PushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, PushConstantRangeCRefArray{ pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayoutCRefArray const & layouts )const
	{
		return createPipelineLayout( layouts
			, PushConstantRangeCRefArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( PushConstantRangeCRefArray const & pushConstantRanges )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, PushConstantRangeCRefArray{ pushConstantRanges } );
	}

	SamplerPtr Device::createSampler( WrapMode wrapS
		, WrapMode wrapT
		, WrapMode wrapR
		, Filter minFilter
		, Filter magFilter
		, MipmapMode mipFilter
		, float minLod
		, float maxLod
		, float lodBias
		, BorderColour borderColour
		, float maxAnisotropy
		, CompareOp compareOp )const
	{
		return createSampler(
		{
			minFilter,
			magFilter,
			mipFilter,
			wrapR,
			wrapT,
			wrapS,
			lodBias,
			maxAnisotropy != 1.0f,
			maxAnisotropy,
			compareOp != CompareOp::eAlways,
			compareOp,
			minLod,
			maxLod,
			borderColour,
			false
		} );
	}

#ifndef NDEBUG
	void Device::doRegisterObject( char const * const type, void * object )const
	{
		auto hash = std::hash< void const * >{}( object );
		std::stringstream stream;
		stream << Debug::Backtrace{ 20, 4 };
		m_allocated.emplace( hash
			, ObjectAllocation{
				std::string{ type },
				stream.str()
			} );
	}

	void Device::doUnregisterObject( void * object )const
	{
		auto hash = std::hash< void * >{}( object );
		auto it = m_allocated.find( hash );
		assert( it != m_allocated.end() );
		m_allocated.erase( it );
	}

	void Device::doReportRegisteredObjects()const
	{
		for ( auto & alloc : m_allocated )
		{
			std::stringstream stream;
			stream << "Leaked [" << alloc.second.type << "], allocation stack:\n";
			stream << alloc.second.callstack;
			Logger::logError( stream );
		}
	}

#endif
}
