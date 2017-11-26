#include "Pipeline.hpp"

#include "DepthStencilState.hpp"
#include "Device.hpp"
#include "MultisampleState.hpp"
#include "PipelineLayout.hpp"
#include "RenderingResources.hpp"
#include "RenderPass.hpp"
#include "Scissor.hpp"
#include "ShaderProgram.hpp"
#include "TessellationState.hpp"
#include "VertexLayout.hpp"
#include "Viewport.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	namespace
	{
		vk::VertexLayoutCRefArray convert( VertexLayoutCRefArray const & layouts )
		{
			vk::VertexLayoutCRefArray result;
			result.reserve( layouts.size() );

			for ( auto & layout : layouts )
			{
				result.emplace_back( *layout.get().getLayout() );
			}

			return result;
		}
	}

	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, ShaderProgram const & program
		, VertexLayoutCRefArray const & vertexLayouts
		, RenderPass const & renderPass
		, PrimitiveTopology topology
		, RasterisationState const & rasterisationState
		, ColourBlendState const & colourBlendState )
		: m_pipeline{ device.getDevice().createPipeline( layout.getLayout()
			, program.getProgram()
			, convert( vertexLayouts )
			, renderPass.getRenderPass()
			, convert( topology )
			, rasterisationState.getState()
			, colourBlendState.getState() ) }
	{
	}

	Pipeline & Pipeline::finish()
	{
		m_pipeline->finish();
		return *this;
	}

	Pipeline & Pipeline::multisampleState( MultisampleState const & state )
	{
		m_pipeline->multisampleState( state.getState() );
		return *this;
	}

	inline Pipeline & Pipeline::depthStencilState( DepthStencilState const & state )
	{
		m_pipeline->depthStencilState( state.getState() );
		return *this;
	}

	inline Pipeline & Pipeline::tessellationState( TessellationState const & state )
	{
		m_pipeline->tessellationState( state.getState() );
		return *this;
	}

	inline Pipeline & Pipeline::viewport( Viewport const & viewport )
	{
		m_pipeline->viewport( viewport.getViewport() );
		return *this;
	}

	inline Pipeline & Pipeline::scissor( Scissor const & scissor )
	{
		m_pipeline->scissor( scissor.getScissor() );
		return *this;
	}
}
