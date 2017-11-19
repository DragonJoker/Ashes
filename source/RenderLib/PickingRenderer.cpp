#include "PickingRenderer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"

#include <GlLib/OpenGL.h>

#include <algorithm>

namespace render
{
	//*************************************************************************

	namespace
	{
		template< typename T >
		void doSetUniformValue( gl::Uniform< T > const & uniform
			, T const & value )
		{
			if ( uniform.valid() )
			{
				uniform.value( value );
			}
		}

		template< typename T >
		void doBindAttribute( gl::Attribute< T > const & attribute )
		{
			if ( attribute.valid() )
			{
				attribute.bind();
			}
		}

		template< typename T >
		void doUnbindAttribute( gl::Attribute< T > const & attribute )
		{
			if ( attribute.valid() )
			{
				attribute.unbind();
			}
		}

		template< typename T >
		void doBindAttribBuffer( gl::Buffer< T > const & buffer
			, gl::Attribute< T > const & attribute )noexcept
		{
			if ( attribute.valid() )
			{
				buffer.bind();
				attribute.bind();
			}
		}
		template< typename T >
		void doUnbindAttribBuffer( gl::Buffer< T > const & buffer
			, gl::Attribute< T > const & attribute )noexcept
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
		}
	}

	//*************************************************************************

	PickingRenderer::RenderNode::RenderNode( gl::ShaderProgramPtr && program )
		: m_program{ std::move( program ) }
		, m_mtxUbo{ "Matrices", 0u, *m_program }
		, m_mtxProjection{ &m_mtxUbo.createUniform< gl::Mat4 >( "mtxProjection" ) }
		, m_mtxView{ &m_mtxUbo.createUniform< gl::Mat4 >( "mtxView" ) }
		, m_mtxModel{ &m_mtxUbo.createUniform< gl::Mat4 >( "mtxModel" ) }
		, m_mapOpacity{ gl::makeUniform< int >( "mapOpacity", *m_program ) }
		, m_pickUbo{ "Picking", 1u, *m_program }
		, m_drawIndex{ &m_pickUbo.createUniform< int >( "drawIndex" ) }
		, m_nodeIndex{ &m_pickUbo.createUniform< int >( "nodeIndex" ) }
	{
		m_mtxUbo.initialise();
		m_pickUbo.initialise();
	}

	//*************************************************************************

	PickingRenderer::ObjectNode::ObjectNode( gl::ShaderProgramPtr && program )
		: RenderNode{ std::move( program ) }
		, m_position{ m_program->createAttribute< gl::Vec3 >( "position" ) }
		, m_normal{ m_program->createAttribute< gl::Vec3 >( "normal" ) }
		, m_texture{ m_program->createAttribute< gl::Vec2 >( "texture" ) }
		, m_scale{ gl::makeUniform< float >( "scale", *m_program ) }
	{
	}

	//*************************************************************************

	PickingRenderer::BillboardNode::BillboardNode( gl::ShaderProgramPtr && program )
		: RenderNode{ std::move( program ) }
		, m_billboardUbo{ "Billboard", 2u, *m_program }
		, m_dimensions{ &m_billboardUbo.createUniform< gl::Vec2 >( "dimensions" ) }
		, m_camera{ &m_billboardUbo.createUniform< gl::Vec3 >( "camera" ) }
		, m_position{ m_program->createAttribute< gl::Vec3 >( "position"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardData, center ) ) }
		, m_scale{ m_program->createAttribute< gl::Vec2 >( "scale"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardData, scale ) ) }
		, m_texture{ m_program->createAttribute< gl::Vec2 >( "texture"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardBuffer::Vertex, texture ) ) }
		, m_id{ m_program->createAttribute< float >( "id"
			, sizeof( BillboardBuffer::Vertex )
			, offsetof( BillboardBuffer::Vertex, id ) ) }
	{
		m_billboardUbo.initialise();
	}

	//*************************************************************************

	PickingRenderer::PickingRenderer()
		: m_pipelineOpaque{ false, true, true, false }
	{
	}

	void PickingRenderer::initialise()
	{
		uint32_t i = 0u;
		for ( auto & node : m_objectNodes )
		{
			node = std::make_unique< ObjectNode >
				( UberShader::createShaderProgram( RenderType::ePicking
					, UberShader::textureFlags( NodeType( i ) )
					, UberShader::opacityType( NodeType( i ) )
					, ObjectType::eObject ) );
			++i;
		}

		i = 0u;
		for ( auto & node : m_billboardNodes )
		{
			node = std::make_unique< BillboardNode >
				( UberShader::createShaderProgram( RenderType::ePicking
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

	void PickingRenderer::draw( Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		m_pipelineOpaque.apply();
		doRenderObjects( camera
			, zoomPercent
			, NodeType::eOpaqueNoTex
			, *m_objectNodes[size_t( NodeType::eOpaqueNoTex )]
			, objects[size_t( NodeType::eOpaqueNoTex )] );
		doRenderObjects( camera
			, zoomPercent
			, NodeType::eOpaqueDiff
			, *m_objectNodes[size_t( NodeType::eOpaqueDiff )]
			, objects[size_t( NodeType::eOpaqueDiff )] );
		doRenderBillboards( camera
			, zoomPercent
			, NodeType::eOpaqueNoTex
			, *m_billboardNodes[size_t( NodeType::eOpaqueNoTex )]
			, billboards[size_t( NodeType::eOpaqueNoTex )] );
		doRenderBillboards( camera
			, zoomPercent
			, NodeType::eOpaqueDiff
			, *m_billboardNodes[size_t( NodeType::eOpaqueDiff )]
			, billboards[size_t( NodeType::eOpaqueDiff )] );
		doRenderTransparent( camera
			, zoomPercent
			, NodeType::eAlphaTest
			, OpacityType::eAlphaTest
			, objects
			, billboards );
		doRenderTransparent( camera
			, zoomPercent
			, NodeType::eAlphaBlend
			, OpacityType::eAlphaBlend
			, objects
			, billboards );
	}

	void PickingRenderer::doRenderTransparent( Camera const & camera
		, float zoomPercent
		, NodeType type
		, OpacityType opacity
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		size_t nodeType{ size_t( type )
			+ size_t( TransparentNodeType::eNoTex ) };
		doRenderObjects( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderObjects( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderObjects( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderObjects( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_objectNodes[nodeType]
			, objects[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eNoTex );
		doRenderBillboards( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eDiff );
		doRenderBillboards( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpa );
		doRenderBillboards( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
		nodeType = size_t( type )
			+ size_t( TransparentNodeType::eOpaDiff );
		doRenderBillboards( camera
			, zoomPercent
			, NodeType( nodeType )
			, *m_billboardNodes[nodeType]
			, billboards[nodeType] );
	}

	void PickingRenderer::doRenderObjects( Camera const & camera
		, float zoomPercent
		, NodeType type
		, ObjectNode const & node
		, RenderSubmeshVector const & objects )const
	{
		if ( !objects.empty() )
		{
			gl::Mat4 const & projection = camera.projection();
			gl::Mat4 const & view = camera.view();
			node.m_program->bind();
			node.m_mtxProjection->value( projection );
			node.m_mtxView->value( view );
			node.m_scale->value( m_objectScale.value( zoomPercent ) );
			node.m_drawIndex->value( ObjectMask | int( type ) );
			uint32_t id{ 0u };

			for ( auto & object : objects )
			{
				if ( object.m_object->visible() )
				{
					node.m_mtxModel->value(
						object.m_object->transform() );
					node.m_nodeIndex->value( id );
					doBindMaterial( node, *object.m_material );
					node.m_mtxUbo.bind( 0u );
					node.m_pickUbo.bind( 1u );
					node.m_scale->bind();
					object.m_submesh->bind( node.m_position.get()
						, node.m_normal.get()
						, node.m_texture.get() );
					object.m_submesh->draw();
					object.m_submesh->unbind( node.m_position.get()
						, node.m_normal.get()
						, node.m_texture.get() );
					doUnbindMaterial( node, *object.m_material );
				}

				++id;
			}

			node.m_program->unbind();
		}
	}

	void PickingRenderer::doRenderBillboards( Camera const & camera
		, float zoomPercent
		, NodeType type
		, BillboardNode const & node
		, BillboardArray const & billboards )const
	{
		if ( !billboards.empty() )
		{
			gl::Mat4 const & projection = camera.projection();
			gl::Mat4 const & view = camera.view();
			gl::Vec3 const & position = camera.position();
			node.m_program->bind();
			node.m_mtxProjection->value( projection );
			node.m_mtxView->value( view );
			node.m_camera->value( position );
			node.m_dimensions->value( { 30.0, 30.0 } );
			node.m_drawIndex->value( BillboardMask | int( type ) );
			uint32_t id{ 0u };

			for ( auto & billboard : billboards )
			{
				if ( billboard->visible()
					&& billboard->buffer().count() )
				{
					node.m_mtxModel->value( billboard->transform() );
					node.m_nodeIndex->value( id );
					doBindMaterial( node, billboard->material() );
					node.m_mtxUbo.bind( 0u );
					node.m_pickUbo.bind( 1u );
					node.m_billboardUbo.bind( 2u );
					billboard->buffer().vbo().bind();
					node.m_position->bind();
					node.m_scale->bind();
					node.m_texture->bind();
					node.m_id->bind();
					glCheckError( glDrawArrays
						, GL_TRIANGLES
						, 0
						, billboard->buffer().count() * 6 );
					node.m_id->unbind();
					node.m_texture->unbind();
					node.m_scale->unbind();
					node.m_position->unbind();
					billboard->buffer().vbo().unbind();
					doUnbindMaterial( node, billboard->material() );
				}

				++id;
			}

			node.m_program->unbind();
		}
	}

	//*************************************************************************
}
