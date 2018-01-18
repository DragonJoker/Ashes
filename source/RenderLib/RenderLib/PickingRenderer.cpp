#include "PickingRenderer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"
#include "Texture.h"

#include <Renderer/Pipeline/DepthStencilState.hpp>
#include <Renderer/Descriptor/DescriptorSet.hpp>
#include <Renderer/Descriptor/DescriptorSetLayout.hpp>
#include <Renderer/Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Renderer/Descriptor/DescriptorSetPool.hpp>
#include <Renderer/Pipeline/MultisampleState.hpp>
#include <Renderer/Pipeline/PipelineLayout.hpp>
#include <Renderer/Core/RenderingResources.hpp>
#include <Renderer/RenderPass/RenderPass.hpp>
#include <Renderer/Buffer/StagingBuffer.hpp>

#include <algorithm>

namespace render
{
	//*************************************************************************

	namespace
	{
		renderer::DescriptorSetLayoutPtr doCreateUboDescriptorLayout( renderer::Device const & device
			, ObjectType type
			, TextureFlags textures )
		{
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( UberShader::UboMatrixBinding
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex );

			if ( type == ObjectType::eBillboard )
			{
				bindings.emplace_back( UberShader::UboBillboardBinding
					, renderer::DescriptorType::eUniformBuffer
					, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment );
			}
			else if ( type == ObjectType::ePolyLine )
			{
				bindings.emplace_back( UberShader::UboPolyLineBinding
					, renderer::DescriptorType::eUniformBuffer
					, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment );
			}

			bindings.emplace_back( UberShader::UboPickingBinding
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eFragment );

			if ( checkFlag( textures, TextureFlag::eOpacity ) )
			{
				bindings.emplace_back( UberShader::TextureOpacityBinding
					, renderer::DescriptorType::eCombinedImageSampler
					, renderer::ShaderStageFlag::eFragment );
			}

			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		renderer::ColourBlendState doCreateBlendState( NodeType type )
		{
			renderer::ColourBlendState result;

			switch ( UberShader::opacityType( type ) )
			{
			case OpacityType::eAlphaBlend:
				result.addAttachment( renderer::ColourBlendStateAttachment
				{
					true,
					renderer::BlendFactor::eSrcAlpha,
					renderer::BlendFactor::eInvSrcAlpha,
					renderer::BlendOp::eAdd,
					renderer::BlendFactor::eSrcAlpha,
					renderer::BlendFactor::eInvSrcAlpha,
					renderer::BlendOp::eAdd
				} );
				break;

			default:
				result.addAttachment( renderer::ColourBlendStateAttachment{} );
			}

			return result;
		}
	}

	//*************************************************************************

	PickingRenderer::RenderNode::RenderNode( renderer::Device const & device
		, renderer::DescriptorSetLayoutPtr && layout
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
		, m_descriptorLayout{ std::move( layout ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( MaxObjectsCount ) }
		, m_descriptor{ m_descriptorPool->createDescriptorSet() }
	{
		m_descriptor->createBinding( m_descriptorPool->getLayout().getBinding( UberShader::UboMatrixBinding )
			, m_mtxUbo
			, 0u );
		m_descriptor->createBinding( m_descriptorPool->getLayout().getBinding( UberShader::UboPickingBinding )
			, m_pickUbo
			, 0u );
	}

	//*************************************************************************

	PickingRenderer::ObjectNode::ObjectNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayoutPtr && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_posLayout{ renderer::makeLayout< renderer::Vec3 > ( device, 0u ) }
		, m_texLayout{ renderer::makeLayout< renderer::Vec2 >( device, 1u ) }
	{
		m_posLayout->createAttribute< renderer::Vec3 >( 0u, 0u );
		m_texLayout->createAttribute< renderer::Vec2 >( 1u, 0u );
		m_pipelineLayout = device.createPipelineLayout( *m_descriptorLayout );
		m_pipeline = device.createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_posLayout, *m_texLayout }
			, renderPass
			, renderer::PrimitiveTopology::eTriangleList
			, renderer::RasterisationState{}
			, doCreateBlendState( type ) );
		m_pipeline->multisampleState( renderer::MultisampleState{} )
			.depthStencilState( renderer::DepthStencilState{} )
			.finish();
	}

	//*************************************************************************

	PickingRenderer::BillboardNode::BillboardNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayoutPtr && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_billboardUbo{ device
			, 1u
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_layout{ renderer::makeLayout< BillboardBuffer::Vertex >( device, 0u ) }
	{
		m_layout->createAttribute< renderer::Vec3 >( 0u, offsetof( BillboardData, center ) );
		m_layout->createAttribute< renderer::Vec2 >( 1u, offsetof( BillboardData, scale ) );
		m_layout->createAttribute< renderer::Vec2 >( 2u, offsetof( BillboardBuffer::Vertex, texture ) );
		m_layout->createAttribute< float >( 3u, offsetof( BillboardBuffer::Vertex, id ) );
		m_pipelineLayout = device.createPipelineLayout( *m_descriptorLayout );
		m_descriptor->createBinding( m_descriptorPool->getLayout().getBinding( UberShader::UboBillboardBinding )
			, m_billboardUbo
			, 0u );
		m_pipeline = device.createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_layout }
			, renderPass
			, renderer::PrimitiveTopology::eTriangleFan
			, renderer::RasterisationState{}
			, doCreateBlendState( type ) );
		m_pipeline->multisampleState( renderer::MultisampleState{} )
			.depthStencilState( renderer::DepthStencilState{} )
			.finish();
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
			auto type = NodeType( i );
			auto textures = UberShader::textureFlags( type );
			auto opacity = UberShader::opacityType( type );
			node = std::make_unique< ObjectNode >( m_device
				, m_renderPass
				, doCreateUboDescriptorLayout( m_device
					, ObjectType::eObject
					, textures )
				, UberShader::createShaderProgram( m_device
					, RenderType::ePicking
					, textures
					, opacity
					, ObjectType::eObject )
				, type );
			++i;
		}

		i = 0u;
		for ( auto & node : m_billboardNodes )
		{
			auto type = NodeType( i );
			auto textures = UberShader::textureFlags( type );
			auto opacity = UberShader::opacityType( type );
			node = std::make_unique< BillboardNode >( m_device
				, m_renderPass
				, doCreateUboDescriptorLayout( m_device
					, ObjectType::eBillboard
					, textures )
				, UberShader::createShaderProgram( m_device
					, RenderType::ePicking
					, textures
					, opacity
					, ObjectType::eBillboard )
				, type );
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
			renderer::Mat4 const & projection = camera.projection();
			renderer::Mat4 const & view = camera.view();
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
					commandBuffer.bindDescriptorSet( *object.m_descriptor
						, *node.m_pipelineLayout );
					commandBuffer.bindGeometryBuffers( *object.m_vao );
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
			renderer::Mat4 const & projection = camera.projection();
			renderer::Mat4 const & view = camera.view();
			renderer::Vec3 const & position = camera.position();
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			node.m_billboardUbo.getData().camera = position;
			node.m_billboardUbo.getData().dimensions = renderer::Vec2{ 30.0, 30.0 };
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_billboardUbo.getDatas()
				, node.m_billboardUbo
				, renderer::PipelineStageFlag::eFragmentShader );
			node.m_pickUbo.getData().drawIndex = ObjectMask | int( type );
			commandBuffer.bindPipeline( *node.m_pipeline );
			commandBuffer.setViewport( camera.viewport().viewport() );
			uint32_t id{ 0u };
			commandBuffer.bindDescriptorSet( *node.m_descriptor
				, *node.m_pipelineLayout );

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
					commandBuffer.bindGeometryBuffers( *billboard.m_vao );
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
