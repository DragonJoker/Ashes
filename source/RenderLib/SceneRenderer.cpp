#include "SceneRenderer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"

#include <algorithm>

namespace render
{
	//*************************************************************************

	namespace
	{
		template< typename T >
		void doBindAttribute( renderer::Attribute< T > const & attribute )
		{
			if ( attribute.valid() )
			{
				attribute.bind();
			}
		}

		template< typename T >
		void doUnbindAttribute( renderer::Attribute< T > const & attribute )
		{
			if ( attribute.valid() )
			{
				attribute.unbind();
			}
		}

		template< typename T >
		void doBindAttribBuffer( renderer::VertexBuffer< T > const & buffer
			, renderer::Attribute< T > const & attribute )noexcept
		{
			if ( attribute.valid() )
			{
				buffer.bind();
				attribute.bind();
			}
		}

		template< typename T >
		void doUnbindAttribBuffer( renderer::VertexBuffer< T > const & buffer
			, renderer::Attribute< T > const & attribute )noexcept
		{
			if ( attribute.valid() )
			{
				attribute.unbind();
				buffer.unbind();
			}
		}

		template< typename NodesType >
		void doBindMaterial( NodesType const & nodes
			, Material const & material )
		{
			nodes.m_matUbo.getData().ambient = material.ambient();
			nodes.m_matUbo.getData().diffuse = material.diffuse();
			nodes.m_matUbo.getData().specular = material.specular();
			nodes.m_matUbo.getData().emissive = material.emissive();
			nodes.m_matUbo.getData().exponent = material.exponent();
			nodes.m_matUbo.getData().opacity = material.opacity();

			if ( material.hasDiffuseMap()
				&& nodes.m_mapDiffuse->valid() )
			{
				nodes.m_mapDiffuse->value( 0 );
				nodes.m_mapDiffuse->bind();
				material.diffuseMap().bind( 0 );
			}

			if ( material.hasOpacityMap()
				&& nodes.m_mapOpacity->valid() )
			{
				nodes.m_mapOpacity->value( 1 );
				nodes.m_mapOpacity->bind();
				material.opacityMap().bind( 1 );
			}
		}

		template< typename NodesType >
		void doUnbindMaterial( NodesType const & nodes
			, Material const & material )
		{
			if ( material.hasOpacityMap()
				&& nodes.m_mapOpacity->valid() )
			{
				material.opacityMap().unbind( 1 );
			}

			if ( material.hasDiffuseMap()
				&& nodes.m_mapDiffuse->valid() )
			{
				material.diffuseMap().unbind( 0 );
			}
		}
	}

	//*************************************************************************

	SceneRenderer::RenderNode::RenderNode( renderer::RenderingResources const & resources
		, renderer::ShaderProgramPtr && program )
		: m_program{ std::move( program ) }
		, m_mtxUbo{ resources, 1u, BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_matUbo{ resources, 1u, BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_mapDiffuse{ renderer::makeUniform< int >( "mapDiffuse", *m_program ) }
		, m_mapOpacity{ renderer::makeUniform< int >( "mapOpacity", *m_program ) }
	{
		m_mtxUbo.initialise();
		m_matUbo.initialise();
	}

	//*************************************************************************

	SceneRenderer::ObjectNode::ObjectNode( renderer::RenderingResources const & resources
		, renderer::ShaderProgramPtr && program )
		: RenderNode{ std::move( program ) }
		, m_position{ m_program->createAttribute< renderer::Vec3 >( "position" ) }
		, m_normal{ m_program->createAttribute< renderer::Vec3 >( "normal" ) }
		, m_texture{ m_program->createAttribute< renderer::Vec2 >( "texture" ) }
	{
	}

	//*************************************************************************

	SceneRenderer::BillboardNode::BillboardNode( renderer::RenderingResources const & resources
		, renderer::ShaderProgramPtr && program )
		: RenderNode{ std::move( program ) }
		, m_billboardUbo{ resources, 1u, BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_position{ m_program->createAttribute< renderer::Vec3 >( "position"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardData, center ) ) }
		, m_scale{ m_program->createAttribute< renderer::Vec2 >( "scale"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardData, scale ) ) }
		, m_texture{ m_program->createAttribute< renderer::Vec2 >( "texture"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardBuffer::Vertex, texture ) ) }
	{
		m_billboardUbo.initialise();
	}

	//*************************************************************************

	SceneRenderer::PolyLineNode::PolyLineNode( renderer::RenderingResources const & resources
		, renderer::ShaderProgramPtr && program )
		: RenderNode{ std::move( program ) }
		, m_lineUbo{ resources, 1u, BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_position{ m_program->createAttribute< renderer::Vec3 >( "position"
			, sizeof( PolyLine::Vertex )
			, offsetof( PolyLine::Vertex, m_position ) ) }
		, m_normal{ m_program->createAttribute< renderer::Vec3 >( "normal"
			, sizeof( PolyLine::Vertex )
			, offsetof( PolyLine::Vertex, m_normal ) ) }
	{
		m_lineUbo.initialise();
	}

	//*************************************************************************

	SceneRenderer::SceneRenderer( renderer::RenderingResources const & resources )
		: m_resources{ resources }
		, m_pipelineOpaque{ true, true, true, false }
		, m_pipelineAlphaBlend{ false, true, false, true }
	{
	}

	void SceneRenderer::initialise()
	{
		uint32_t i = 0u;
		for ( auto & node : m_objectNodes )
		{
			node = std::make_unique< ObjectNode >
				( m_resources
					, UberShader::createShaderProgram( RenderType::eScene
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eObject ) );
			++i;
		}

		i = 0u;
		for ( auto & node : m_billboardNodes )
		{
			node = std::make_unique< BillboardNode >
				( m_resources
					, UberShader::createShaderProgram( RenderType::eScene
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eBillboard ) );
			++i;
		}

		m_lineNode = std::make_unique< PolyLineNode >
			( m_resources
				, UberShader::createShaderProgram( RenderType::eScene
				, TextureFlag::eNone
				, OpacityType::eOpaque
				, ObjectType::ePolyLine ) );
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

	void SceneRenderer::draw( Camera const & camera
		, float zoomScale
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, PolyLineArray const & lines )const
	{
		m_pipelineOpaque.apply();
		doRenderObjects( camera
			, NodeType::eOpaqueNoTex
			, *m_objectNodes[size_t( NodeType::eOpaqueNoTex )]
			, objects[size_t( NodeType::eOpaqueNoTex )] );
		doRenderObjects( camera
			, NodeType::eOpaqueDiff
			, *m_objectNodes[size_t( NodeType::eOpaqueDiff )]
			, objects[size_t( NodeType::eOpaqueDiff )] );
		doRenderBillboards( camera
			, NodeType::eOpaqueNoTex
			, *m_billboardNodes[size_t( NodeType::eOpaqueNoTex )]
			, billboards[size_t( NodeType::eOpaqueNoTex )] );
		doRenderBillboards( camera
			, NodeType::eOpaqueDiff
			, *m_billboardNodes[size_t( NodeType::eOpaqueDiff )]
			, billboards[size_t( NodeType::eOpaqueDiff )] );
		m_pipelineAlphaBlend.apply();
		doRenderLines( camera
			, zoomScale
			, *m_lineNode
			, lines );
		m_pipelineOpaque.apply();
		doRenderTransparent( camera
			, NodeType::eAlphaTest
			, OpacityType::eAlphaTest
			, objects
			, billboards
			, lines );
		m_pipelineAlphaBlend.apply();
		doRenderTransparent( camera
			, NodeType::eAlphaBlend
			, OpacityType::eAlphaBlend
			, objects
			, billboards
			, lines );
	}

	void SceneRenderer::doRenderTransparent( Camera const & camera
		, NodeType type
		, OpacityType opacity
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards
		, PolyLineArray const & lines )const
	{
		size_t nodeType{ size_t( type )
			+ size_t( TransparentNodeType::eNoTex ) };
		doRenderObjects( camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderObjects( camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderObjects( camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderObjects( camera
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eNoTex );
		doRenderBillboards( camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderBillboards( camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderBillboards( camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderBillboards( camera
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
	}

	void SceneRenderer::doRenderObjects( Camera const & camera
		, NodeType type
		, ObjectNode const & node
		, RenderSubmeshVector const & objects )const
	{
		if ( !objects.empty() )
		{
			renderer::Mat4 const & projection = camera.projection();
			renderer::Mat4 const & view = camera.view();
			node.m_program->bind();
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;

			for ( auto & object : objects )
			{
				if ( object.m_object->visible() )
				{
					node.m_mtxUbo.getData().model = object.m_object->transform();
					m_resources.copyUniformData( node.m_mtxUbo.getDatas()
						, node.m_mtxUbo );
					doBindMaterial( node, *object.m_material );
					m_resources.copyUniformData( node.m_matUbo.getDatas()
						, node.m_matUbo );
					node.m_mtxUbo.bind( 0u );
					node.m_matUbo.bind( 1u );
					object.m_submesh->bind( node.m_position.get()
						, node.m_normal.get()
						, node.m_texture.get() );
					object.m_submesh->draw();
					object.m_submesh->unbind( node.m_position.get()
						, node.m_normal.get()
						, node.m_texture.get() );
					doUnbindMaterial( node, *object.m_material );
				}
			}

			node.m_program->unbind();
		}
	}

	void SceneRenderer::doRenderBillboards( Camera const & camera
		, NodeType type
		, BillboardNode const & node
		, BillboardArray const & billboards )const
	{
		if ( !billboards.empty() )
		{
			renderer::Mat4 const & projection = camera.projection();
			renderer::Mat4 const & view = camera.view();
			renderer::Vec3 const & position = camera.position();
			node.m_program->bind();
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			node.m_billboardUbo.getData().camera = position;

			for ( auto & billboard : billboards )
			{
				if ( billboard->visible()
					&& billboard->buffer().count() )
				{
					node.m_mtxUbo.getData().model = billboard->transform();
					m_resources.copyUniformData( node.m_mtxUbo.getDatas()
						, node.m_mtxUbo );
					node.m_billboardUbo.getData().dimensions = renderer::Vec2{ billboard->dimensions() };
					m_resources.copyUniformData( node.m_billboardUbo.getDatas()
						, node.m_billboardUbo );
					doBindMaterial( node, billboard->material() );
					m_resources.copyUniformData( node.m_matUbo.getDatas()
						, node.m_matUbo );
					node.m_mtxUbo.bind( 0u );
					node.m_matUbo.bind( 1u );
					node.m_billboardUbo.bind( 2u );
					billboard->buffer().vbo().bind();
					node.m_position->bind();
					node.m_scale->bind();
					node.m_texture->bind();
					glCheckError( glDrawArrays
						, GL_TRIANGLES
						, 0
						, billboard->buffer().count() * 6 );
					node.m_texture->unbind();
					node.m_scale->unbind();
					node.m_position->unbind();
					billboard->buffer().vbo().unbind();
					doUnbindMaterial( node, billboard->material() );
				}
			}

			node.m_program->unbind();
		}
	}

	void SceneRenderer::doRenderLines( Camera const & camera
		, float zoomScale
		, PolyLineNode const & node
		, PolyLineArray const & lines )const
	{
		if ( !lines.empty() )
		{
			renderer::Mat4 const & projection = camera.projection();
			renderer::Mat4 const & view = camera.view();
			renderer::Vec3 const & position = camera.position();
			node.m_program->bind();
			node.m_mtxUbo.getData().projection = projection;
			node.m_mtxUbo.getData().view = view;
			node.m_lineUbo.getData().lineScale = zoomScale;
			node.m_lineUbo.getData().camera = position;

			for ( auto & line : lines )
			{
				if ( line->visible()
					&& line->count() )
				{
					node.m_mtxUbo.getData().model = line->transform();
					m_resources.copyUniformData( node.m_mtxUbo.getDatas()
						, node.m_mtxUbo );
					node.m_lineUbo.getData().lineWidth = line->width();
					node.m_lineUbo.getData().lineFeather = line->feather();
					m_resources.copyUniformData( node.m_lineUbo.getDatas()
						, node.m_lineUbo );
					doBindMaterial( node, line->material() );
					m_resources.copyUniformData( node.m_matUbo.getDatas()
						, node.m_matUbo );
					node.m_mtxUbo.bind( 0u );
					node.m_matUbo.bind( 1u );
					node.m_lineUbo.bind( 2u );
					node.m_position->bind( line->buffer().data()->data() );
					node.m_normal->bind( line->buffer().data()->data() );
					glCheckError( glDrawArrays
						, GL_LINES
						, 0
						, GLsizei( line->buffer().size() * 6 ) );
					node.m_normal->unbind();
					node.m_position->unbind();
					doUnbindMaterial( node, line->material() );
				}
			}

			node.m_program->unbind();
		}
	}

	//*************************************************************************
}
