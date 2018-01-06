#include "VkPipeline.hpp"

#include "VkDepthStencilState.hpp"
#include "VkDevice.hpp"
#include "VkMultisampleState.hpp"
#include "VkPipelineLayout.hpp"
#include "VkRenderingResources.hpp"
#include "VkRenderPass.hpp"
#include "VkScissor.hpp"
#include "VkShaderProgram.hpp"
#include "VkTessellationState.hpp"
#include "VkVertexLayout.hpp"
#include "VkViewport.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace vk_renderer
{
	namespace
	{
		vk::VertexLayoutCRefArray convert( renderer::VertexLayoutCRefArray const & layouts )
		{
			vk::VertexLayoutCRefArray result;
			result.reserve( layouts.size() );

			for ( auto & layout : layouts )
			{
				result.emplace_back( *static_cast< VertexLayout const & >( layout.get() ).getLayout() );
			}

			return result;
		}
	}

	Pipeline::Pipeline( renderer::Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::ShaderProgram const & program
		, renderer::VertexLayoutCRefArray const & vertexLayouts
		, renderer::RenderPass const & renderPass
		, renderer::PrimitiveTopology topology
		, renderer::RasterisationState const & rasterisationState
		, renderer::ColourBlendState const & colourBlendState )
		: renderer::Pipeline{ device
			, layout
			, program
			, vertexLayouts
			, renderPass
			, topology
			, rasterisationState
			, colourBlendState }
		, m_pipeline{ static_cast< Device const & >( device ).getDevice().createPipeline( static_cast< PipelineLayout const & >( layout ).getLayout()
			, static_cast< ShaderProgram const & >( program ).getProgram()
			, convert( vertexLayouts )
			, static_cast< RenderPass const & >( renderPass ).getRenderPass()
			, convert( topology )
			, convert( rasterisationState )
			, convert( colourBlendState ) ) }
	{
	}

	renderer::Pipeline & Pipeline::finish()
	{
		m_pipeline->finish();
		return *this;
	}

	renderer::Pipeline & Pipeline::multisampleState( renderer::MultisampleState const & state )
	{
		m_pipeline->multisampleState( convert( state ) );
		return *this;
	}

	renderer::Pipeline & Pipeline::depthStencilState( renderer::DepthStencilState const & state )
	{
		m_pipeline->depthStencilState( convert( state ) );
		return *this;
	}

	renderer::Pipeline & Pipeline::tessellationState( renderer::TessellationState const & state )
	{
		m_pipeline->tessellationState( convert( state ) );
		return *this;
	}

	renderer::Pipeline & Pipeline::viewport( renderer::Viewport const & viewport )
	{
		m_pipeline->viewport( convert( viewport ) );
		return *this;
	}

	renderer::Pipeline & Pipeline::scissor( renderer::Scissor const & scissor )
	{
		m_pipeline->scissor( convert( scissor ) );
		return *this;
	}
}
