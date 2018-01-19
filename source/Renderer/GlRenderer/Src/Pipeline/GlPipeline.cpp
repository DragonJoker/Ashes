#include "Pipeline/GlPipeline.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Core/GlRenderingResources.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderProgram.hpp"
#include "Pipeline/GlVertexLayout.hpp"

namespace gl_renderer
{
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
		, m_layout{ layout }
		, m_program{ program }
		, m_cbState{ colourBlendState }
		, m_rsState{ rasterisationState }
	{
	}

	renderer::Pipeline & Pipeline::finish()
	{
		return *this;
	}

	renderer::Pipeline & Pipeline::multisampleState( renderer::MultisampleState const & state )
	{
		m_msState = std::make_unique< renderer::MultisampleState >( state );
		return *this;
	}

	renderer::Pipeline & Pipeline::depthStencilState( renderer::DepthStencilState const & state )
	{
		m_dsState = std::make_unique< renderer::DepthStencilState >( state );
		return *this;
	}

	renderer::Pipeline & Pipeline::tessellationState( renderer::TessellationState const & state )
	{
		m_tsState = std::make_unique< renderer::TessellationState >( state );
		return *this;
	}

	renderer::Pipeline & Pipeline::viewport( renderer::Viewport const & viewport )
	{
		m_viewport = std::make_unique< renderer::Viewport >( viewport );
		return *this;
	}

	renderer::Pipeline & Pipeline::scissor( renderer::Scissor const & scissor )
	{
		m_scissor = std::make_unique< renderer::Scissor >( scissor );
		return *this;
	}
}
