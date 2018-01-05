/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlDevice.hpp"

#include "GlBuffer.hpp"
#include "GlCommandPool.hpp"
#include "GlConnection.hpp"
#include "GlContext.hpp"
#include "GlDescriptorSetLayout.hpp"
#include "GlGeometryBuffers.hpp"
#include "GlPipeline.hpp"
#include "GlPipelineLayout.hpp"
#include "GlQueue.hpp"
#include "GlRenderer.hpp"
#include "GlRenderPass.hpp"
#include "GlRenderSubpass.hpp"
#include "GlSampler.hpp"
#include "GlSemaphore.hpp"
#include "GlShaderProgram.hpp"
#include "GlStagingBuffer.hpp"
#include "GlSwapChain.hpp"
#include "GlTexture.hpp"
#include "GlUniformBuffer.hpp"
#include "GlVertexLayout.hpp"

namespace gl_renderer
{
	Device::Device( renderer::Renderer const & renderer
		, renderer::ConnectionPtr && connection )
		: renderer::Device{ renderer, *connection }
		, m_context{ Context::create( std::move( connection ) ) }
	{
		m_presentQueue = std::make_unique< Queue >();
		m_graphicsQueue = std::make_unique< Queue >();
		m_presentCommandPool = std::make_unique< CommandPool >( *this, 0u );
		m_graphicsCommandPool = std::make_unique< CommandPool >( *this, 0u );
	}

	renderer::RenderPassPtr Device::createRenderPass( std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassPtrArray const & subpasses
		, renderer::RenderPassState const & initialState
		, renderer::RenderPassState const & finalState
		, bool clear
		, renderer::SampleCountFlag samplesCount )const
	{
		return std::make_unique< RenderPass >( *this
			, formats
			, subpasses
			, initialState
			, finalState
			, clear
			, samplesCount );
	}

	renderer::RenderSubpassPtr Device::createRenderSubpass( std::vector< utils::PixelFormat > const & formats
		, renderer::RenderSubpassState const & neededState )const
	{
		return std::make_unique< RenderSubpass >( *this
			, formats
			, neededState );
	}

	renderer::VertexLayoutPtr Device::createVertexLayout( uint32_t bindingSlot
		, uint32_t stride )const
	{
		return std::make_unique< VertexLayout >( bindingSlot
			, stride );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferBase const & vbo
		, uint64_t vboOffset
		, renderer::VertexLayout const & layout )const
	{
		return std::make_unique< GeometryBuffers >( vbo
			, vboOffset
			, layout );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferBase const & vbo
		, uint64_t vboOffset
		, renderer::VertexLayout const & layout
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )const
	{
		return std::make_unique< GeometryBuffers >( vbo
			, vboOffset
			, layout
			, ibo
			, iboOffset
			, type );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > vboOffsets
		, renderer::VertexLayoutCRefArray const & layouts )const
	{
		return std::make_unique< GeometryBuffers >( vbos
			, vboOffsets
			, layouts );
	}

	renderer::GeometryBuffersPtr Device::createGeometryBuffers( renderer::VertexBufferCRefArray const & vbos
		, std::vector< uint64_t > vboOffsets
		, renderer::VertexLayoutCRefArray const & layouts
		, renderer::BufferBase const & ibo
		, uint64_t iboOffset
		, renderer::IndexType type )const
	{
		return std::make_unique< GeometryBuffers >( vbos
			, vboOffsets
			, layouts
			, ibo
			, iboOffset
			, type );
	}

	renderer::PipelineLayoutPtr Device::createPipelineLayout()const
	{
		return std::make_unique< PipelineLayout >( *this
			, nullptr );
	}

	renderer::PipelineLayoutPtr Device::createPipelineLayout( renderer::DescriptorSetLayout const & layout )const
	{
		return std::make_unique< PipelineLayout >( *this
			, &layout );
	}

	renderer::PipelinePtr Device::createPipeline( renderer::PipelineLayout const & layout
		, renderer::ShaderProgram const & program
		, renderer::VertexLayoutCRefArray const & vertexLayouts
		, renderer::RenderPass const & renderPass
		, renderer::PrimitiveTopology topology
		, renderer::RasterisationState const & rasterisationState
		, renderer::ColourBlendState const & colourBlendState )const
	{
		return std::make_shared< Pipeline >( *this
			, layout
			, program
			, vertexLayouts
			, renderPass
			, topology
			, rasterisationState
			, colourBlendState );
	}

	renderer::DescriptorSetLayoutPtr Device::createDescriptorSetLayout( renderer::DescriptorSetLayoutBindingArray && bindings )const
	{
		return std::make_unique< DescriptorSetLayout >( *this, std::move( bindings ) );
	}

	renderer::TexturePtr Device::createTexture()const
	{
		return std::make_shared< Texture >( *this );
	}

	renderer::SamplerPtr Device::createSampler( renderer::WrapMode wrapS
		, renderer::WrapMode wrapT
		, renderer::WrapMode wrapR
		, renderer::Filter minFilter
		, renderer::Filter magFilter
		, renderer::MipmapMode mipFilter )const
	{
		return std::make_unique< Sampler >( *this
			, wrapS
			, wrapT
			, wrapR
			, minFilter
			, magFilter
			, mipFilter );
	}

	renderer::BufferBasePtr Device::createBuffer( uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< BufferBase >( *this
			, size
			, target
			, memoryFlags );
	}

	renderer::StagingBufferPtr Device::createStagingBuffer( uint32_t size )const
	{
		return std::make_shared< StagingBuffer >( *this, size );
	}

	renderer::UniformBufferBasePtr Device::createUniformBuffer( uint32_t count
		, uint32_t size
		, renderer::BufferTargets target
		, renderer::MemoryPropertyFlags memoryFlags )const
	{
		return std::make_unique< UniformBufferBase >( *this
			, count
			, size
			, target
			, memoryFlags );
	}

	renderer::SwapChainPtr Device::createSwapChain( utils::IVec2 const & size )const
	{
		renderer::SwapChainPtr result;

		try
		{
			result = std::make_unique< SwapChain >( *this, size );
		}
		catch ( std::exception & exc )
		{
			std::cerr << "Could not create the swap chain:\n" << exc.what() << std::endl;
		}
		catch ( ... )
		{
			std::cerr << "Could not create the swap chain:\nUnknown error" << std::endl;
		}

		return result;
	}

	renderer::SemaphorePtr Device::createSemaphore()const
	{
		return std::make_unique< Semaphore >( *this );
	}

	renderer::CommandPoolPtr Device::createCommandPool( uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags const & flags )const
	{
		return std::make_unique< CommandPool >( *this
			, queueFamilyIndex
			, flags );
	}

	renderer::ShaderProgramPtr Device::createShaderProgram()const
	{
		return std::make_unique< ShaderProgram >( *this );
	}

	void Device::waitIdle()const
	{
		glFinish();
	}

	void Device::swapBuffers()const
	{
		m_context->swapBuffers();
	}
}
