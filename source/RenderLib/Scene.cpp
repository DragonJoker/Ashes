#include "Scene.h"

#include "Billboard.h"
#include "ExponentialRange.h"
#include "LogarithmicRange.h"
#include "Object.h"
#include "PolyLine.h"
#include "StringUtils.h"
#include "Submesh.h"

#include <GlLib/GlShaderProgram.h>
#include <GlLib/GlUniform.h>
#include <GlLib/OpenGL.h>

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

	Scene::Scene( gl::IVec2 const & size )
		: RenderableContainer{}
		, m_camera{ size }
	{
		auto material = m_materials.findElement( "FullAlphaWhite" );

		if ( !material )
		{
			gl::RgbColour white{ 1, 1, 1 };
			material = std::make_shared< Material >();
			material->ambient( white );
			material->diffuse( white );
			material->specular( white );
			material->emissive( white );
			material->opacity( 1.0f );
			m_materials.addElement( "FullAlphaWhite", material );
		}
	}

	Scene::~Scene()
	{
		for ( auto & billboard : m_billboardsBuffers )
		{
			billboard.second->cleanup();
		}

		m_materials.clear();
		m_meshes.clear();
		m_textures.clear();
		doCleanup();
	}

	void Scene::update()
	{
		m_cameraChanged |= m_camera.update();
		doUpdate( m_changedMovables );
		doUpdateBillboards();
		m_cameraChanged = false;
	}

	void Scene::updateOverlays()
	{
		for ( auto & overlay : overlays() )
		{
			overlay.second->update();
		}
	}

	void Scene::draw()const
	{
		m_camera.viewport().apply();
		auto percent = m_state.zoomBounds().percent( m_state.zoom() );
		doDraw( m_camera
			, 2.0f * percent + ( 1.0f - percent ) / 100.0f );
	}

	void Scene::resize( gl::IVec2 const & size )noexcept
	{
		m_camera.resize( size );
	}

	void Scene::add( ObjectPtr object )
	{
		assert( object && "Null object given" );
		doAdd( object );
		m_onMovableChanged[object.get()] = object->onMovableChanged.connect
			( std::bind( &Scene::onMovableChanged
				, this
				, std::placeholders::_1 ) );
		m_changedMovables.push_back( object.get() );
	}

	void Scene::remove( ObjectPtr object )
	{
		doRemove( object );
		m_onMovableChanged[object.get()].disconnect();
	}

	void Scene::add( BillboardPtr billboard )
	{
		assert( billboard && "Null billboard given" );
		doAdd( billboard );
		m_onMovableChanged[billboard.get()] = billboard->onMovableChanged.connect
			( std::bind( &Scene::onMovableChanged
				, this
				, std::placeholders::_1 ) );
		m_changedMovables.push_back( billboard.get() );
	}

	void Scene::remove( BillboardPtr billboard )
	{
		doRemove( billboard );
		m_onMovableChanged[billboard.get()].disconnect();
	}

	void Scene::add( PolyLinePtr lines )
	{
		assert( lines && "Null PolyLine given" );
		doAdd( lines );
		m_onMovableChanged[lines.get()] = lines->onMovableChanged.connect
			( std::bind( &Scene::onMovableChanged
				, this
				, std::placeholders::_1 ) );
		m_changedMovables.push_back( lines.get() );
	}

	void Scene::remove( PolyLinePtr lines )
	{
		doRemove( lines );
		m_onMovableChanged[lines.get()].disconnect();
	}

	void Scene::addBillboardBuffer( std::string const & name
		, BillboardBufferPtr buffer )
	{
		m_billboardsBuffers.addElement( name, buffer );
		m_newBillboardBuffers.push_back( buffer );
	}

	void Scene::onMovableChanged( Movable & movable )
	{
		m_changedMovables.push_back( &movable );
	}

	void Scene::doUpdateBillboards()
	{
		static float constexpr offset = 1.0f;
		static LogarithmicRange< 2, float > threshRange{ 0 + offset, 1 + offset };
		auto percent = m_state.zoomBounds().invpercent( m_state.zoom() );
		m_currentThreshold = m_threshold.range().value( percent );
		// First, initialise the billboards that need to be.
		for ( auto & billboard : m_newBillboardBuffers )
		{
			billboard->initialise();
		}

		if ( m_currentThreshold != m_previousThreshold )
		{
			// Threshold has changed, so update all the billboards buffers.
			m_previousThreshold = m_currentThreshold;
			m_cameraChanged = true;

			for ( auto & billboard : m_billboardsBuffers )
			{
				billboard.second->update( m_previousThreshold );
			}
		}
		else
		{
			// Threshold has not changed, so update the new billboards buffers.
			for ( auto & billboard : m_newBillboardBuffers )
			{
				billboard->update( m_previousThreshold );
			}
		}

		if ( m_cameraChanged )
		{
			m_cameraChanged = false;
			// Apply frustum culling to billboards.
			for ( auto & billboard : billboards() )
			{
				billboard->cull( m_camera
					, 2.0f - 2.0f * percent );
			}
		}

		m_newBillboardBuffers.clear();
	}
}
