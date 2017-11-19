#include "RenderableContainer.h"

#include "Billboard.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"

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

	RenderableContainer::RenderableContainer()
		: m_renderer{}
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

		m_lines.clear();
		m_objects.clear();
		m_billboards.clear();
		m_renderer.cleanup();
	}

	void RenderableContainer::doDraw( Camera const & camera
		, float zoomScale )const
	{
		m_renderer.draw( camera
			, zoomScale
			, m_renderObjects
			, m_renderBillboards
			, m_lines );
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
			RenderSubmesh objMesh;
			objMesh.m_material = *mtlit;
			objMesh.m_submesh = *mshit;
			objMesh.m_mesh = object->mesh();
			objMesh.m_object = object;
			auto opacity = objMesh.m_material->opacityType();
			auto textures = objMesh.m_material->textureFlags();
			size_t flags = size_t( UberShader::nodeType( opacity, textures ) );
			m_renderObjects[flags].push_back( objMesh );
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
			RenderSubmesh objMesh;
			objMesh.m_material = *mtlit;
			objMesh.m_submesh = *mshit;
			objMesh.m_mesh = object->mesh();
			objMesh.m_object = object;
			auto opacity = objMesh.m_material->opacityType();
			auto textures = objMesh.m_material->textureFlags();
			size_t flags = size_t( UberShader::nodeType( opacity, textures ) );
			auto itr = std::find_if( m_renderObjects[flags].begin()
				, m_renderObjects[flags].end()
				, [&objMesh]( RenderSubmesh & submesh )
				{
					return submesh.m_submesh == objMesh.m_submesh
						&& submesh.m_material == objMesh.m_material;
				} );

			if ( itr == m_renderObjects[flags].end() )
			{
				assert( false && "Billboard not found in the list" );
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

		auto opacity = billboard->material().opacityType();
		auto textures = billboard->material().textureFlags();
		size_t flags = size_t( UberShader::nodeType( opacity, textures ) );
		m_renderBillboards[flags].push_back( billboard );
		m_billboards.push_back( billboard );
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
		auto opacity = billboard->material().opacityType();
		auto textures = billboard->material().textureFlags();
		size_t flags = size_t( UberShader::nodeType( opacity, textures ) );
		auto itr = std::find( m_renderBillboards[flags].begin()
			, m_renderBillboards[flags].end()
			, billboard );

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
	}
}
