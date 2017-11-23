#include "Pipeline.hpp"

#include "Device.hpp"
#include "PipelineLayout.hpp"
#include "RenderingResources.hpp"
#include "Scissor.hpp"
#include "ShaderProgram.hpp"
#include "VertexLayout.hpp"
#include "Viewport.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	namespace
	{
		std::vector< std::reference_wrapper< vk::VertexLayout const > > convert( std::vector< std::reference_wrapper< VertexLayout const > > const & layouts )
		{
			std::vector< std::reference_wrapper< vk::VertexLayout const > > result;
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
		, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
		, vk::RenderPass const & renderPass
		, PrimitiveTopology topology )
		: m_pipeline{ device.getDevice().createPipeline( layout.getLayout()
			, program.getProgram()
			, convert( vertexLayouts )
			, renderPass
			, convert( topology ) ) }
	{
	}

	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, ShaderProgram const & program
		, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
		, Viewport const & viewport
		, Scissor const & scissor
		, vk::RenderPass const & renderPass
		, PrimitiveTopology topology )
		: m_pipeline{ device.getDevice().createPipeline( layout.getLayout()
			, program.getProgram()
			, convert( vertexLayouts )
			, viewport.getViewport()
			, scissor.getScissor()
			, renderPass
			, convert( topology ) ) }
	{
	}
}
