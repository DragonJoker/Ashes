#include "PickingRenderer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"
#include "Texture.h"

#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/RenderingResources.hpp>
#include <Renderer/RenderPass.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/VertexLayout.hpp>

#include <algorithm>

namespace render
{
	//*************************************************************************

	namespace
	{
		renderer::DescriptorSetLayout doCreateUboDescriptorLayout( renderer::Device const & device
			, ObjectType type )
		{
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( 0u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex );
			bindings.emplace_back( 1u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eFragment );

			if ( type == ObjectType::eBillboard
				|| type == ObjectType::ePolyLine )
			{
				bindings.emplace_back( 0u
					, renderer::DescriptorType::eUniformBuffer
					, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment );
			}

			return renderer::DescriptorSetLayout{ device, bindings };
		}
	}

	//*************************************************************************

	PickingRenderer::RenderNode::RenderNode( renderer::Device const & device
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program )
		: m_program{ std::move( program ) }
		, m_mtxUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_pickUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_uboDescriptorLayout{ std::move( layout ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout }
		, m_uboDescriptor{ m_uboDescriptorPool.createDescriptorSet() }
	{
		m_uboDescriptor.createBinding( { 0u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment }
			, m_mtxUbo );
		m_uboDescriptor.createBinding( { 1u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment }
			, m_pickUbo );
	}

	//*************************************************************************

	PickingRenderer::ObjectNode::ObjectNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_pipelineLayout{ device, nullptr }
		, m_posLayout{ 0u }
		, m_nmlLayout{ 1u }
		, m_texLayout{ 2u }
	{
		m_pipeline = std::make_shared< renderer::Pipeline >( device
			, m_pipelineLayout
			, *m_program
			, renderer::VertexLayoutCRefArray
			{
				m_posLayout,
				m_nmlLayout,
				m_texLayout
			}
			, renderPass
			, renderer::PrimitiveTopology::eTriangleList );
		m_posLayout.createAttribute< utils::Vec3 >( 0u, 0u );
		m_nmlLayout.createAttribute< utils::Vec3 >( 1u, 0u );
		m_texLayout.createAttribute< utils::Vec2 >( 2u, 0u );
	}

	//*************************************************************************

	PickingRenderer::BillboardNode::BillboardNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayout && layout
		, renderer::ShaderProgramPtr && program )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_pipelineLayout{ device, nullptr }
		, m_layout{ 0u }
		, m_billboardUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
	{
		m_pipeline = std::make_shared< renderer::Pipeline >( device
			, m_pipelineLayout
			, *m_program
			, renderer::VertexLayoutCRefArray
			{
				m_layout,
			}
			, renderPass
			, renderer::PrimitiveTopology::eTriangleFan );
		m_layout.createAttribute< utils::Vec3 >( 0u
			, offsetof( BillboardData, center ) );
		m_layout.createAttribute< utils::Vec2 >( 1u
			, offsetof( BillboardData, scale ) );
		m_layout.createAttribute< utils::Vec2 >( 2u
			, offsetof( BillboardBuffer::Vertex, texture ) );
		m_layout.createAttribute< float >( 3u
			, offsetof( BillboardBuffer::Vertex, id ) );
		m_uboDescriptor.createBinding( { 2u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eFragment }
			, m_billboardUbo );
	}

	//*************************************************************************

	PickingRenderer::PickingRenderer( renderer::Device const & device
		, renderer::RenderPass const & renderPass )
		: m_device{ device }
		, m_renderPass{ renderPass }
	{
	}

	void PickingRenderer::initialise()
	{
		uint32_t i = 0u;
		for ( auto & node : m_objectNodes )
		{
			node = std::make_unique< ObjectNode >( m_device
				, m_renderPass
				, doCreateUboDescriptorLayout( m_device
					, ObjectType::eObject )
				, UberShader::createShaderProgram( m_device
					, RenderType::ePicking
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eObject ) );
			++i;
		}

		i = 0u;
		for ( auto & node : m_billboardNodes )
		{
			node = std::make_unique< BillboardNode >( m_device
				, m_renderPass
				, doCreateUboDescriptorLayout( m_device
					, ObjectType::eBillboard )
				, UberShader::createShaderProgram( m_device
					, RenderType::ePicking
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eBillboard ) );
			++i;
		}
	}

	void PickingRenderer::cleanup()
	{
		for ( auto & node : m_objectNodes )
		{
			node.reset();
		}

		for ( auto & node : m_billboardNodes )
		{
			node.reset();
		}
	}

	void PickingRenderer::draw( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		doRenderObjects( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType::eOpaqueNoTex
			, *m_objectNodes[size_t( NodeType::eOpaqueNoTex )]
			, objects[size_t( NodeType::eOpaqueNoTex )] );
		doRenderObjects( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType::eOpaqueDiff
			, *m_objectNodes[size_t( NodeType::eOpaqueDiff )]
			, objects[size_t( NodeType::eOpaqueDiff )] );
		doRenderBillboards( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType::eOpaqueNoTex
			, *m_billboardNodes[size_t( NodeType::eOpaqueNoTex )]
			, billboards[size_t( NodeType::eOpaqueNoTex )] );
		doRenderBillboards( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType::eOpaqueDiff
			, *m_billboardNodes[size_t( NodeType::eOpaqueDiff )]
			, billboards[size_t( NodeType::eOpaqueDiff )] );
		doRenderTransparent( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType::eAlphaTest
			, OpacityType::eAlphaTest
			, objects
			, billboards );
		doRenderTransparent( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType::eAlphaBlend
			, OpacityType::eAlphaBlend
			, objects
			, billboards );
	}

	void PickingRenderer::doRenderTransparent( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomPercent
		, NodeType type
		, OpacityType opacity
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		size_t nodeType{ size_t( type )
			+ size_t( TransparentNodeType::eNoTex ) };
		doRenderObjects( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderObjects( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderObjects( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderObjects( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eNoTex );
		doRenderBillboards( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderBillboards( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderBillboards( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderBillboards( stagingBuffer
			, commandBuffer
			, camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
	}

	void PickingRenderer::doRenderObjects( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomPercent
		, NodeType type
		, ObjectNode & node
		, RenderSubmeshVector const & objects )const
	{
		if ( !objects.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			//node.m_scale->value( m_objectScale.value( zoomPercent ) );
			node.m_pickUbo.getData().drawIndex = ObjectMask | int( type );
			commandBuffer.bindPipeline( *node.m_pipeline );
			commandBuffer.setViewport( camera.viewport().viewport() );
			uint32_t id{ 0u };

			for ( auto & object : objects )
			{
				if ( object.m_object->visible() )
				{
					node.m_mtxUbo.getData().model = object.m_object->transform();
					node.m_pickUbo.getData().nodeIndex = id;
					stagingBuffer.copyUniformData( commandBuffer
						, node.m_mtxUbo.getDatas()
						, node.m_mtxUbo
						, renderer::PipelineStageFlag::eVertexShader );
					stagingBuffer.copyUniformData( commandBuffer
						, node.m_pickUbo.getDatas()
						, node.m_pickUbo
						, renderer::PipelineStageFlag::eVertexShader | renderer::PipelineStageFlag::eFragmentShader );
					commandBuffer.bindDescriptorSet( object.m_materialDescriptor
						, node.m_pipelineLayout );
					commandBuffer.bindVertexBuffers( { std::ref( static_cast< renderer::VertexBufferBase const & >( object.m_mesh->getPositions() ) )
							, std::ref( static_cast< renderer::VertexBufferBase const & >( object.m_mesh->getNormals() ) )
							, std::ref( static_cast< renderer::VertexBufferBase const & >( object.m_mesh->getTexCoords() ) ) }
						, { 0u, 0u, 0u } );
					commandBuffer.bindIndexBuffer( object.m_submesh->getIbo()
						, 0u
						, renderer::IndexType::eUInt16 );
					//node.m_scale->bind();
					commandBuffer.draw( object.m_submesh->getIndexCount()
						, 1u
						, 0u
						, 0u );
				}

				++id;
			}
		}
	}

	void PickingRenderer::doRenderBillboards( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomPercent
		, NodeType type
		, BillboardNode & node
		, RenderBillboardVector const & billboards )const
	{
		if ( !billboards.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			utils::Vec3 const & position = camera.position();
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			node.m_billboardUbo.getData().camera = position;
			node.m_billboardUbo.getData().dimensions = utils::Vec2{ 30.0, 30.0 };
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_billboardUbo.getDatas()
				, node.m_billboardUbo
				, renderer::PipelineStageFlag::eFragmentShader );
			node.m_pickUbo.getData().drawIndex = ObjectMask | int( type );
			commandBuffer.bindPipeline( *node.m_pipeline );
			commandBuffer.setViewport( camera.viewport().viewport() );
			uint32_t id{ 0u };
			commandBuffer.bindDescriptorSet( node.m_uboDescriptor
				, node.m_pipelineLayout );

			for ( auto & billboard : billboards )
			{
				if ( billboard.m_billboard->visible()
					&& billboard.m_billboard->buffer().count() )
				{
					node.m_mtxUbo.getData().model = billboard.m_billboard->transform();
					node.m_pickUbo.getData().nodeIndex = id;
					stagingBuffer.copyUniformData( commandBuffer
						, node.m_mtxUbo.getDatas()
						, node.m_mtxUbo
						, renderer::PipelineStageFlag::eVertexShader );
					stagingBuffer.copyUniformData( commandBuffer
						, node.m_pickUbo.getDatas()
						, node.m_pickUbo
						, renderer::PipelineStageFlag::eVertexShader | renderer::PipelineStageFlag::eFragmentShader );
					commandBuffer.bindDescriptorSet( billboard.m_materialDescriptor
						, node.m_pipelineLayout );
					commandBuffer.bindVertexBuffer( billboard.m_billboard->buffer().vbo()
						, 0u );
					commandBuffer.draw( billboard.m_billboard->buffer().count() * 6
						, 1u
						, 0u
						, 0u );
				}

				++id;
			}
		}
	}

	//*************************************************************************
}
