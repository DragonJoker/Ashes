#include "SceneRenderer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Material.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"
#include "Texture.h"

#include <Renderer/ColourBlendState.hpp>
#include <Renderer/ColourBlendStateAttachment.hpp>
#include <Renderer/DepthStencilState.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/ImageMemoryBarrier.hpp>
#include <Renderer/MultisampleState.hpp>
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
		renderer::DescriptorSetLayoutPtr doCreateDescriptorLayout( renderer::Device const & device
			, ObjectType type
			, TextureFlags textures )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( UberShader::UboMatrixBinding
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex );
			bindings.emplace_back( UberShader::UboMaterialBinding
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eFragment );

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

			if ( checkFlag( textures, TextureFlag::eDiffuse ) )
			{
				bindings.emplace_back( UberShader::TextureDiffuseBinding
					, renderer::DescriptorType::eCombinedImageSampler
					, renderer::ShaderStageFlag::eFragment );
			}

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

	SceneRenderer::RenderNode::RenderNode( renderer::Device const & device
		, renderer::DescriptorSetLayoutPtr && layout
		, renderer::ShaderProgramPtr && program )
		: m_program{ std::move( program ) }
		, m_mtxUbo{ std::make_unique< renderer::UniformBuffer< MatrixUbo > >( device
			, MaxObjectsCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_matUbo{ std::make_unique< renderer::UniformBuffer< MaterialUbo > >( device
			, MaxObjectsCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_descriptorLayout{ std::move( layout ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( MaxObjectsCount ) }
	{
	}

	//*************************************************************************

	SceneRenderer::ObjectNode::ObjectNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayoutPtr && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_posLayout{ renderer::makeLayout< renderer::Vec3 >( device, 0u ) }
		, m_nmlLayout{ renderer::makeLayout< renderer::Vec3 >( device, 1u ) }
		, m_texLayout{ renderer::makeLayout< renderer::Vec2 >( device, 2u ) }
	{
		m_posLayout->createAttribute< renderer::Vec3 >( 0u, 0u );
		m_nmlLayout->createAttribute< renderer::Vec3 >( 1u, 0u );
		m_texLayout->createAttribute< renderer::Vec2 >( 2u, 0u );
		m_pipelineLayout = device.createPipelineLayout( *m_descriptorLayout );
		m_pipeline = device.createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_posLayout, *m_nmlLayout, *m_texLayout }
			, renderPass
			, renderer::PrimitiveTopology::eTriangleList
			, renderer::RasterisationState{}
			, doCreateBlendState( type ) );
		m_pipeline->multisampleState( renderer::MultisampleState{} )
			.depthStencilState( renderer::DepthStencilState{} )
			.finish();
	}

	//*************************************************************************

	SceneRenderer::BillboardNode::BillboardNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayoutPtr && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_billboardUbo{ std::make_unique< renderer::UniformBuffer< BillboardUbo > >( device
			, MaxObjectsCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_layout{ renderer::makeLayout< BillboardBuffer::Vertex >( device, 0u ) }
	{
		m_layout->createAttribute< renderer::Vec3 >( 0u, offsetof( BillboardData, center ) );
		m_layout->createAttribute< renderer::Vec2 >( 1u, offsetof( BillboardData, scale ) );
		m_layout->createAttribute< renderer::Vec2 >( 2u, offsetof( BillboardBuffer::Vertex, texture ) );
		m_pipelineLayout = device.createPipelineLayout( *m_descriptorLayout );
		m_pipeline = device.createPipeline( *m_pipelineLayout
			, *m_program
			, { *m_layout }
			, renderPass
			, renderer::PrimitiveTopology::eTriangleList
			, renderer::RasterisationState{
				0u,
				false,
				false,
				renderer::PolygonMode::eFill,
				renderer::CullModeFlag::eNone
			}
			, doCreateBlendState( type ) );
		m_pipeline->multisampleState( renderer::MultisampleState{} )
			.depthStencilState( renderer::DepthStencilState{} )
			.finish();
	}

	//*************************************************************************

	SceneRenderer::PolyLineNode::PolyLineNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, renderer::DescriptorSetLayoutPtr && layout
		, renderer::ShaderProgramPtr && program
		, NodeType type )
		: RenderNode{ device, std::move( layout ), std::move( program ) }
		, m_lineUbo{ std::make_unique< renderer::UniformBuffer< LineUbo > >( device
			, MaxObjectsCount
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_layout{ renderer::makeLayout< PolyLine::Vertex >( device, 0u ) }
	{
		m_layout->createAttribute< renderer::Vec3 >( 0u
			, offsetof( PolyLine::Vertex, m_position ) );
		m_layout->createAttribute< renderer::Vec2 >( 1u
			, offsetof( PolyLine::Vertex, m_normal ) );
		m_pipelineLayout = device.createPipelineLayout( *m_descriptorLayout );
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

	SceneRenderer::SceneRenderer( renderer::Device const & device
		, renderer::RenderPass const & renderPass )
		: m_device{ device }
		, m_renderPass{ renderPass }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer( false ) }
	{
	}

	void SceneRenderer::initialise()
	{
		uint32_t i = 0u;
		for ( auto & node : m_objectNodes )
		{
			node = std::make_unique< ObjectNode >( m_device
				, m_renderPass
				, doCreateDescriptorLayout( m_device
					, ObjectType::eObject
					, UberShader::textureFlags( NodeType( i ) ) )
				, UberShader::createShaderProgram( m_device
					, RenderType::eScene
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eObject )
				, NodeType( i ) );
			++i;
		}

		i = 0u;
		for ( auto & node : m_billboardNodes )
		{
			node = std::make_unique< BillboardNode >( m_device
				, m_renderPass
				, doCreateDescriptorLayout( m_device
					, ObjectType::eBillboard
					, UberShader::textureFlags( NodeType( i ) ) )
				, UberShader::createShaderProgram( m_device
					, RenderType::eScene
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eBillboard )
				, NodeType( i ) );
			++i;
		}

		m_lineNode = std::make_unique< PolyLineNode >( m_device
			, m_renderPass
			, doCreateDescriptorLayout( m_device
				, ObjectType::ePolyLine
				, TextureFlag::eNone )
			, UberShader::createShaderProgram( m_device
				, RenderType::eScene
				, TextureFlag::eNone
				, OpacityType::eOpaque
				, ObjectType::ePolyLine )
			, NodeType::eOpaqueNoTex );
	}

	void SceneRenderer::cleanup()
	{
		for ( auto & node : m_objectNodes )
		{
			node.reset();
		}

		for ( auto & node : m_billboardNodes )
		{
			node.reset();
		}

		m_lineNode.reset();
	}

	void SceneRenderer::draw( renderer::FrameBuffer const & frameBuffer
		, Camera const & camera
		, float zoomScale
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, RenderPolyLineArray const & lines )const
	{
		if ( m_commandBuffer->begin( renderer::CommandBufferUsageFlag::eRenderPassContinue
			, m_renderPass
			, 0u
			, frameBuffer ) )
		{
			draw( *m_commandBuffer
				, camera
				, zoomScale
				, objects
				, billboards
				, lines );
			m_commandBuffer->end();
		}
	}

	void SceneRenderer::draw( renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomScale
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, RenderPolyLineArray const & lines )const
	{
		doRenderObjects( commandBuffer
			, camera
			, NodeType::eOpaqueNoTex
			, *m_objectNodes[size_t( NodeType::eOpaqueNoTex )]
			, objects[size_t( NodeType::eOpaqueNoTex )] );
		doRenderObjects( commandBuffer
			, camera
			, NodeType::eOpaqueDiff
			, *m_objectNodes[size_t( NodeType::eOpaqueDiff )]
			, objects[size_t( NodeType::eOpaqueDiff )] );
		doRenderBillboards( commandBuffer
			, camera
			, NodeType::eOpaqueNoTex
			, *m_billboardNodes[size_t( NodeType::eOpaqueNoTex )]
			, billboards[size_t( NodeType::eOpaqueNoTex )] );
		doRenderBillboards( commandBuffer
			, camera
			, NodeType::eOpaqueDiff
			, *m_billboardNodes[size_t( NodeType::eOpaqueDiff )]
			, billboards[size_t( NodeType::eOpaqueDiff )] );
		doRenderLines( commandBuffer
			, camera
			, zoomScale
			, *m_lineNode
			, lines[size_t( NodeType::eOpaqueNoTex )] );
		doRenderTransparent( commandBuffer
			, camera
			, NodeType::eAlphaTest
			, OpacityType::eAlphaTest
			, objects
			, billboards
			, lines );
		doRenderTransparent( commandBuffer
			, camera
			, NodeType::eAlphaBlend
			, OpacityType::eAlphaBlend
			, objects
			, billboards
			, lines );
	}

	void SceneRenderer::doRenderTransparent( renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, NodeType type
		, OpacityType opacity
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, RenderPolyLineArray const & lines )const
	{
		size_t nodeType{ size_t( type )
			+ size_t( TransparentNodeType::eNoTex ) };
		doRenderObjects( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderObjects( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderObjects( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderObjects( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eNoTex );
		doRenderBillboards( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderBillboards( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderBillboards( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderBillboards( commandBuffer
			, camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
	}

	void SceneRenderer::doRenderObjects( renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, NodeType type
		, ObjectNode & node
		, RenderSubmeshVector const & objects )const
	{
		if ( !objects.empty() )
		{
			commandBuffer.bindPipeline( *node.m_pipeline );
			commandBuffer.setViewport( camera.viewport().viewport() );
			commandBuffer.setScissor( camera.viewport().scissor() );

			for ( auto & object : objects )
			{
				if ( object.m_object->visible() )
				{
					if ( object.m_material->hasDiffuseMap() )
					{
						commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
							, renderer::PipelineStageFlag::eFragmentShader
							, object.m_material->diffuseMap().texture().makeShaderInputResource() );
					}

					if ( object.m_material->hasOpacityMap() )
					{
						commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
							, renderer::PipelineStageFlag::eFragmentShader
							, object.m_material->opacityMap().texture().makeShaderInputResource() );
					}

					commandBuffer.bindDescriptorSet( *object.m_descriptor
						, *node.m_pipelineLayout );
					commandBuffer.bindGeometryBuffers( *object.m_vao );
					//node.m_scale->bind();
					commandBuffer.drawIndexed( object.m_submesh->getIndexCount()
						, 1u
						, 0u
						, 0u
						, 0u );
				}
			}
		}
	}

	void SceneRenderer::doRenderBillboards( renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, NodeType type
		, BillboardNode & node
		, RenderBillboardVector const & billboards )const
	{
		if ( !billboards.empty() )
		{
			commandBuffer.bindPipeline( *node.m_pipeline );
			commandBuffer.setViewport( camera.viewport().viewport() );
			commandBuffer.setScissor( camera.viewport().scissor() );

			for ( auto & billboard : billboards )
			{
				if ( billboard.m_billboard->visible()
					&& billboard.m_billboard->buffer().count() )
				{
					if ( billboard.m_billboard->material().hasDiffuseMap() )
					{
						commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
							, renderer::PipelineStageFlag::eFragmentShader
							, billboard.m_billboard->material().diffuseMap().texture().makeShaderInputResource() );
					}

					if ( billboard.m_billboard->material().hasOpacityMap() )
					{
						commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
							, renderer::PipelineStageFlag::eFragmentShader
							, billboard.m_billboard->material().opacityMap().texture().makeShaderInputResource() );
					}

					commandBuffer.bindDescriptorSet( *billboard.m_descriptor
						, *node.m_pipelineLayout );
					commandBuffer.bindGeometryBuffers( *billboard.m_vao );
					commandBuffer.draw( billboard.m_billboard->buffer().count() * 6
						, 1u
						, 0u
						, 0u );
				}
			}
		}
	}

	void SceneRenderer::doRenderLines( renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomScale
		, PolyLineNode & node
		, RenderPolyLineVector const & lines )const
	{
		if ( !lines.empty() )
		{
			//commandBuffer.bindPipeline( *node.m_pipeline );
			//commandBuffer.setViewport( camera.viewport().viewport() );
			//commandBuffer.setScissor( camera.viewport().scissor() );

			//for ( auto & line : lines )
			//{
			//	if ( line.m_line->visible()
			//		&& line.m_line->count() )
			//	{
			//		commandBuffer.bindDescriptorSet( line.m_descriptor
			//			, *node.m_pipelineLayout );
			//		m_commandBuffer.bindVertexBuffer( line.m_line->buffer()
			//			, 0u );
			//		commandBuffer.draw( line.m_line->count() * 6
			//			, 1u
			//			, 0u
			//			, 0u );
			//	}
			//}
		}
	}

	//*************************************************************************
}
