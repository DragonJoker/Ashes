#include "Pipeline.hpp"

#include "RenderingResources.hpp"
#include "VertexLayout.hpp"

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
				result.push_back( *layout.get().getLayout() );
			}

			return result;
		}
	}

	Pipeline::Pipeline( RenderingResources const & resources
		, vk::PipelineLayout const & layout
		, std::string const & vertexShaderFile
		, std::string const & fragmentShaderFile
		, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
		, vk::RenderPass const & renderPass
		, PrimitiveTopology topology )
		: m_pipeline{ resources.getDevice().createPipeline( layout
			, vertexShaderFile
			, fragmentShaderFile
			, convert( vertexLayouts )
			, renderPass
			, convert( topology ) ) }
	{
	}

	Pipeline::Pipeline( RenderingResources const & resources
		, vk::PipelineLayout const & layout
		, std::string const & vertexShaderFile
		, std::string const & fragmentShaderFile
		, std::vector< std::reference_wrapper< VertexLayout const > > const & vertexLayouts
		, vk::Viewport const & viewport
		, vk::RenderPass const & renderPass
		, PrimitiveTopology topology )
		: m_pipeline{ resources.getDevice().createPipeline( layout
			, vertexShaderFile
			, fragmentShaderFile
			, convert( vertexLayouts )
			, viewport
			, renderPass
			, convert( topology ) ) }
	{
	}
}
