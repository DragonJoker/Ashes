#include "Pipeline.hpp"

#include "Device.hpp"
#include "PipelineLayout.hpp"
#include "RenderingResources.hpp"
#include "RenderPass.hpp"
#include "Scissor.hpp"
#include "ShaderProgram.hpp"
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
		, PrimitiveTopology topology )
		: m_pipeline{ device.getDevice().createPipeline( layout.getLayout()
			, program.getProgram()
			, convert( vertexLayouts )
			, renderPass.getRenderPass()
			, convert( topology ) ) }
	{
	}

	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, ShaderProgram const & program
		, VertexLayoutCRefArray const & vertexLayouts
		, Viewport const & viewport
		, Scissor const & scissor
		, RenderPass const & renderPass
		, PrimitiveTopology topology )
		: m_pipeline{ device.getDevice().createPipeline( layout.getLayout()
			, program.getProgram()
			, convert( vertexLayouts )
			, viewport.getViewport()
			, scissor.getScissor()
			, renderPass.getRenderPass()
			, convert( topology ) ) }
	{
	}
}
