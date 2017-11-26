#include "Scene.h"

#include "Billboard.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"

#include <Utils/ExponentialRange.hpp>
#include <Utils/LogarithmicRange.hpp>
#include <Utils/StringUtils.hpp>

//#include <Renderer/ShaderProgram.hpp>
//#include <Renderer/GlUniform.h>
//#include <Renderer/OpenGL.h>

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

	Scene::Scene( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, utils::IVec2 const & size )
		: RenderableContainer{ device, renderPass }
		, m_camera{ size }
	{
		auto material = m_materials.findElement( "FullAlphaWhite" );

		if ( !material )
		{
			renderer::RgbColour white{ 1, 1, 1 };
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

	void Scene::draw( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer )const
	{
		if ( m_cameraChanged )
		{
			auto percent = m_state.zoomBounds().invpercent( m_state.zoom() );
			m_cameraChanged = false;
			// Apply frustum culling to billboards.
			for ( auto & billboard : billboards() )
			{
				billboard->cull( stagingBuffer
					, commandBuffer
					, m_camera
					, 2.0f - 2.0f * percent );
			}
		}

		auto percent = m_state.zoomBounds().percent( m_state.zoom() );
		doDraw( stagingBuffer
			, commandBuffer
			, m_camera
			, 2.0f * percent + ( 1.0f - percent ) / 100.0f );
	}

	void Scene::resize( utils::IVec2 const & size )noexcept
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

	void Scene::addOverlay( std::string const & name
		, PanelOverlayPtr overlay )
	{
	}

	void Scene::addOverlay( std::string const & name
		, BorderPanelOverlayPtr overlay )
	{
	}

	void Scene::addOverlay( std::string const & name
		, TextOverlayPtr overlay )
	{
	}

	void Scene::removeOverlay( std::string const & name )
	{
	}

	void Scene::onMovableChanged( Movable & movable )
	{
		m_changedMovables.push_back( &movable );
	}

	void Scene::doUpdateBillboards()
	{
		static float constexpr offset = 1.0f;
		static utils::LogarithmicRange< 2, float > threshRange{ 0 + offset, 1 + offset };
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

		m_newBillboardBuffers.clear();
	}
}
