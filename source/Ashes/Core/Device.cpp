/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/Device.hpp"

#include "Ashes/Buffer/Buffer.hpp"
#include "Ashes/Core/Instance.hpp"
#include "Ashes/Core/SwapChain.hpp"
#include "Ashes/Image/Sampler.hpp"
#include "Ashes/Miscellaneous/MemoryRequirements.hpp"
#include "Ashes/Pipeline/PipelineLayout.hpp"
#include "Ashes/Pipeline/VertexInputState.hpp"
#include "Ashes/RenderPass/RenderPass.hpp"
#include "Ashes/RenderPass/RenderPassCreateInfo.hpp"
#include "Ashes/RenderPass/RenderSubpass.hpp"
#include "Ashes/RenderPass/RenderSubpassState.hpp"
#include "Ashes/Utils/CallStack.hpp"

namespace ashes
{
	Device::Device( Instance const & instance
		, PhysicalDevice const & gpu
		, Surface const & surface
		, DeviceCreateInfo createInfos )
		: m_instance{ instance }
		, m_gpu{ gpu }
		, m_surface{ surface }
		, m_createInfos{ std::move( createInfos ) }
	{
	}

	Device::~Device()
	{
		reportRegisteredObjects();
	}

	std::array< float, 16u > Device::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_instance.frustum( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		return m_instance.perspective( radiansFovY, aspect, zNear, zFar );
	}

	std::array< float, 16u > Device::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		return m_instance.ortho( left, right, bottom, top, zNear, zFar );
	}

	std::array< float, 16u > Device::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		return m_instance.infinitePerspective( radiansFovY, aspect, zNear );
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
		, RenderSubpassPtrArray subpasses
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
		return m_instance.getClipDirection();
	}

	PipelineLayoutPtr Device::createPipelineLayout()const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, PushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, PushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( PushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, PushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayout const & layout
		, PushConstantRange const & pushConstantRange )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{ layout }
			, PushConstantRangeArray{ 1u, pushConstantRange } );
	}

	PipelineLayoutPtr Device::createPipelineLayout( DescriptorSetLayoutCRefArray const & layouts )const
	{
		return createPipelineLayout( layouts
			, PushConstantRangeArray{} );
	}

	PipelineLayoutPtr Device::createPipelineLayout( PushConstantRangeArray const & pushConstantRanges )const
	{
		return createPipelineLayout( DescriptorSetLayoutCRefArray{}
			, pushConstantRanges );
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
		stream << callstack::Backtrace{ 20, 4 };
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
