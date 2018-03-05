/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Core/Device.hpp"

#include "Core/Renderer.hpp"
#include "Core/SwapChain.hpp"
#include "Pipeline/PipelineLayout.hpp"
#include "Pipeline/VertexInputState.hpp"
#include "RenderPass/RenderSubpass.hpp"

namespace renderer
{
	Device::Device( Renderer const & renderer
		, PhysicalDevice const & gpu
		, Connection const & connection )
		: m_renderer{ renderer }
		, m_gpu{ gpu }
	{
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

	Mat4 Device::infinitePerspective( Angle fovy
		, float aspect
		, float zNear )const
	{
		float const range = tan( float( fovy ) / float( 2 ) ) * zNear;
		float const left = -range * aspect;
		float const right = range * aspect;
		float const bottom = -range;
		float const top = range;

		Mat4 result{ float{ 0 } };
		result[0][0] = ( float( 2 ) * zNear ) / ( right - left );
		result[1][1] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[2][2] = -float( 1 );
		result[2][3] = -float( 1 );
		result[3][2] = -float( 2 ) * zNear;
		return result;
	}

	RenderSubpassPtr Device::createRenderSubpass( PipelineBindPoint pipelineBindPoint
		, RenderSubpassState const & state
		, RenderSubpassAttachmentArray const & colourAttaches )const
	{
		return createRenderSubpass( pipelineBindPoint
			, state
			, RenderSubpassAttachmentArray{}
			, colourAttaches
			, RenderSubpassAttachmentArray{}
			, nullptr
			, UInt32Array{} );
	}

	RenderSubpassPtr Device::createRenderSubpass( PipelineBindPoint pipelineBindPoint
		, RenderSubpassState const & state
		, RenderSubpassAttachmentArray const & colourAttaches
		, RenderSubpassAttachment const & depthAttach )const
	{
		return createRenderSubpass( pipelineBindPoint
			, state
			, RenderSubpassAttachmentArray{}
			, colourAttaches
			, RenderSubpassAttachmentArray{}
			, &depthAttach
			, UInt32Array{} );
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
}
