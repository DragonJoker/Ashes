#include "RenderableContainer.h"

#include "Billboard.h"
#include "Camera.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"

#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/VertexLayout.hpp>

#include <algorithm>

namespace render
{
	namespace
	{
		template< typename ObjectType >
		void doUpdate( std::vector< ObjectType * > & objects )
		{
			auto end = std::unique( objects.begin()
				, objects.end() );
			auto it = objects.begin();

			while ( it != end )
			{
				( *it )->update();
				++it;
			}

			objects.clear();
		}
	}

	RenderableContainer::RenderableContainer( renderer::Device const & device
		, renderer::RenderPass const & renderPass )
		: m_renderer{ device, renderPass }
		, m_device{ device }
	{
		m_renderer.initialise();
	}

	RenderableContainer::~RenderableContainer()
	{
		doCleanup();
	}

	void RenderableContainer::doCleanup()
	{
		for ( auto & objects : m_renderObjects )
		{
			objects.clear();
		}

		for ( auto & billboards : m_renderBillboards )
		{
			billboards.clear();
		}

		for ( auto & lines : m_renderLines )
		{
			lines.clear();
		}

		m_lines.clear();
		m_objects.clear();
		m_billboards.clear();
		m_renderer.cleanup();
	}

	void RenderableContainer::doUpdate( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomScale )
	{
		size_t i = 0;

		for ( auto & objects : m_renderObjects )
		{
			doUpdate( stagingBuffer
				, commandBuffer
				, camera
				, m_renderer.getObjectNode( NodeType( i++ ) )
				, objects );
		}

		i = 0;

		for ( auto & billboards : m_renderBillboards )
		{
			doUpdate( stagingBuffer
				, commandBuffer
				, camera
				, m_renderer.getBillboardNode( NodeType( i++ ) )
				, billboards );
		}

		i = 0;

		for ( auto & lines : m_renderLines )
		{
			doUpdate( stagingBuffer
				, commandBuffer
				, camera
				, zoomScale
				, m_renderer.getPolyLineNode()
				, lines );
		}
	}

	void RenderableContainer::doDraw( renderer::FrameBuffer const & frameBuffer
		, Camera const & camera
		, float zoomScale )const
	{
		m_renderer.draw( frameBuffer
			, camera
			, zoomScale
			, m_renderObjects
			, m_renderBillboards
			, m_renderLines );
	}

	void RenderableContainer::doDraw( renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomScale )const
	{
		m_renderer.draw( commandBuffer
			, camera
			, zoomScale
			, m_renderObjects
			, m_renderBillboards
			, m_renderLines );
	}

	void RenderableContainer::doAdd( ObjectPtr object )
	{
		assert( object && "Null object given" );

		if ( std::find( m_objects.begin()
			, m_objects.end()
			, object ) != m_objects.end() )
		{
			assert( false && "Object has already been added" );
			return;
		}

		m_objects.push_back( object );
		auto mshit = object->mesh()->begin();
		auto mshitend = object->mesh()->end();
		auto mtlit = object->materials().begin();

		while ( mshit != mshitend )
		{
			auto material = ( *mtlit );
			auto nodeType = UberShader::nodeType( material->opacityType()
				, material->textureFlags() );
			auto & node = m_renderer.getObjectNode( nodeType );
			m_renderObjects[size_t( nodeType )].emplace_back( *node.m_descriptorPool
				, object->mesh()
				, *mshit
				, material
				, object
				, uint32_t( m_renderObjects[size_t( nodeType )].size() )
				, *node.m_mtxUbo
				, *node.m_matUbo );
			++mshit;
			++mtlit;
		}
	}

	void RenderableContainer::doRemove( ObjectPtr object )
	{
		auto it = std::find( m_objects.begin(), m_objects.end(), object );

		if ( it == m_objects.end() )
		{
			assert( false && "Object not found in the list" );
			return;
		}

		m_objects.erase( it );
		auto mshit = object->mesh()->begin();
		auto mshitend = object->mesh()->end();
		auto mtlit = object->materials().begin();

		while ( mshit != mshitend )
		{
			auto material = ( *mtlit );
			auto submesh = ( *mshit );
			size_t flags = size_t( UberShader::nodeType( material->opacityType()
				, material->textureFlags() ) );
			auto itr = std::find_if( m_renderObjects[flags].begin()
				, m_renderObjects[flags].end()
				, [submesh, material]( RenderSubmesh & lookup )
			{
				return lookup.m_submesh == submesh
					&& lookup.m_material == material;
			} );

			if ( itr == m_renderObjects[flags].end() )
			{
				assert( false && "Object not found in the list" );
				return;
			}

			m_renderObjects[flags].erase( itr );
			++mshit;
			++mtlit;
		}
	}

	void RenderableContainer::doAdd( BillboardPtr billboard )
	{
		assert( billboard && "Null billboard given" );

		if ( std::find( m_billboards.begin()
			, m_billboards.end()
			, billboard ) != m_billboards.end() )
		{
			assert( false && "Billboard has already been added" );
			return;
		}

		m_billboards.push_back( billboard );
		auto & material = billboard->material();
		auto nodeType = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		auto & node = m_renderer.getBillboardNode( nodeType );
		m_renderBillboards[size_t( nodeType )].emplace_back( *node.m_descriptorPool
			, billboard
			, uint32_t( m_renderBillboards[size_t( nodeType )].size() )
			, *node.m_mtxUbo
			, *node.m_matUbo
			, *node.m_billboardUbo );
	}

	void RenderableContainer::doRemove( BillboardPtr billboard )
	{
		auto it = std::find( m_billboards.begin()
			, m_billboards.end()
			, billboard );

		if ( it == m_billboards.end() )
		{
			assert( false && "Billboard not found in the list" );
			return;
		}

		m_billboards.erase( it );
		auto flags = size_t( UberShader::nodeType( billboard->material().opacityType()
			, billboard->material().textureFlags() ) );
		auto itr = std::find_if( m_renderBillboards[flags].begin()
			, m_renderBillboards[flags].end()
			, [billboard]( RenderBillboard & lookup )
		{
			return lookup.m_billboard == billboard;
		} );

		if ( itr == m_renderBillboards[flags].end() )
		{
			assert( false && "Billboard not found in the list" );
			return;
		}

		m_renderBillboards[flags].erase( itr );
	}

	void RenderableContainer::doAdd( PolyLinePtr lines )
	{
		assert( lines && "Null PolyLine given" );

		if ( std::find( m_lines.begin()
			, m_lines.end()
			, lines ) != m_lines.end() )
		{
			assert( false && "PolyLine has already been added" );
			return;
		}

		m_lines.push_back( lines );
		auto & material = lines->material();
		auto nodeType = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		auto & node = m_renderer.getPolyLineNode();
		m_renderLines[size_t( nodeType )].emplace_back( *node.m_descriptorPool
			, lines
			, uint32_t( m_renderLines[size_t( nodeType )].size() )
			, *node.m_mtxUbo
			, *node.m_matUbo
			, *node.m_lineUbo );
	}

	void RenderableContainer::doRemove( PolyLinePtr lines )
	{
		auto it = std::find( m_lines.begin()
			, m_lines.end()
			, lines );

		if ( it == m_lines.end() )
		{
			assert( false && "PolyLine not found in the list" );
			return;
		}

		m_lines.erase( it );
		auto flags = size_t( UberShader::nodeType( lines->material().opacityType()
			, lines->material().textureFlags() ) );
		auto itr = std::find_if( m_renderLines[flags].begin()
			, m_renderLines[flags].end()
			, [lines]( RenderPolyLine & lookup )
		{
			return lookup.m_line == lines;
		} );

		if ( itr == m_renderLines[flags].end() )
		{
			assert( false && "PolyLine not found in the list" );
			return;
		}

		m_renderLines[flags].erase( itr );
	}


	void RenderableContainer::doUpdate( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, SceneRenderer::ObjectNode & node
		, RenderSubmeshVector const & objects )const
	{
		if ( !objects.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			uint32_t index = 0u;

			for ( auto & object : objects )
			{
				if ( object.m_object->visible() )
				{
					auto & mtxData = node.m_mtxUbo->getData( index );
					mtxData.projection = projection;
					mtxData.view = view;
					mtxData.model = object.m_object->transform();
					auto & matData = node.m_matUbo->getData( index );
					matData.ambient = object.m_material->ambient();
					matData.diffuse = object.m_material->diffuse();
					matData.specular = object.m_material->specular();
					matData.emissive = object.m_material->emissive();
					matData.exponent = object.m_material->exponent();
					matData.opacity = object.m_material->opacity();
				}

				++index;
			}

			stagingBuffer.copyUniformData( commandBuffer
				, node.m_mtxUbo->getDatas()
				, *node.m_mtxUbo
				, renderer::PipelineStageFlag::eVertexShader );
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_matUbo->getDatas()
				, *node.m_matUbo
				, renderer::PipelineStageFlag::eFragmentShader );
		}
	}

	void RenderableContainer::doUpdate( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, SceneRenderer::BillboardNode & node
		, RenderBillboardVector const & billboards )const
	{
		if ( !billboards.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			utils::Vec3 const & position = camera.position();
			uint32_t index = 0u;

			for ( auto & billboard : billboards )
			{
				if ( billboard.m_billboard->visible()
					&& billboard.m_billboard->buffer().count() )
				{
					auto & mtxData = node.m_mtxUbo->getData( index );
					mtxData.projection = projection;
					mtxData.view = view;
					mtxData.model = billboard.m_billboard->transform();
					auto & matData = node.m_matUbo->getData( index );
					matData.ambient = billboard.m_billboard->material().ambient();
					matData.diffuse = billboard.m_billboard->material().diffuse();
					matData.specular = billboard.m_billboard->material().specular();
					matData.emissive = billboard.m_billboard->material().emissive();
					matData.exponent = billboard.m_billboard->material().exponent();
					matData.opacity = billboard.m_billboard->material().opacity();
					auto & billboardData = node.m_billboardUbo->getData( index );
					billboardData.camera = position;
					billboardData.dimensions = utils::Vec2{ billboard.m_billboard->dimensions() };
				}

				++index;
			}

			stagingBuffer.copyUniformData( commandBuffer
				, node.m_mtxUbo->getDatas()
				, *node.m_mtxUbo
				, renderer::PipelineStageFlag::eVertexShader );
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_matUbo->getDatas()
				, *node.m_matUbo
				, renderer::PipelineStageFlag::eFragmentShader );
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_billboardUbo->getDatas()
				, *node.m_billboardUbo
				, renderer::PipelineStageFlag::eVertexShader | renderer::PipelineStageFlag::eFragmentShader );
		}
	}

	void RenderableContainer::doUpdate( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, float zoomScale
		, SceneRenderer::PolyLineNode & node
		, RenderPolyLineVector const & lines )const
	{
		if ( !lines.empty() )
		{
			utils::Mat4 const & projection = camera.projection();
			utils::Mat4 const & view = camera.view();
			utils::Vec3 const & position = camera.position();
			uint32_t index = 0u;

			for ( auto & line : lines )
			{
				if ( line.m_line->visible()
					&& line.m_line->count() )
				{
					auto & mtxData = node.m_mtxUbo->getData( index );
					mtxData.projection = projection;
					mtxData.view = view;
					mtxData.model = line.m_line->transform();
					auto & matData = node.m_matUbo->getData( index );
					matData.ambient = line.m_line->material().ambient();
					matData.diffuse = line.m_line->material().diffuse();
					matData.specular = line.m_line->material().specular();
					matData.emissive = line.m_line->material().emissive();
					matData.exponent = line.m_line->material().exponent();
					matData.opacity = line.m_line->material().opacity();
					auto & lineData = node.m_lineUbo->getData( index );
					lineData.lineScale = zoomScale;
					lineData.camera = position;
					lineData.lineWidth = line.m_line->width();
					lineData.lineFeather = line.m_line->feather();
				}

				++index;
			}

			stagingBuffer.copyUniformData( commandBuffer
				, node.m_mtxUbo->getDatas()
				, *node.m_mtxUbo
				, renderer::PipelineStageFlag::eVertexShader );
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_lineUbo->getDatas()
				, *node.m_lineUbo
				, renderer::PipelineStageFlag::eVertexShader | renderer::PipelineStageFlag::eFragmentShader );
			stagingBuffer.copyUniformData( commandBuffer
				, node.m_matUbo->getDatas()
				, *node.m_matUbo
				, renderer::PipelineStageFlag::eFragmentShader );
		}
	}
}
