#include "Debug.h"

#include "Billboard.h"
#include "BillboardBuffer.h"
#include "Scene.h"
#include "TextOverlay.h"

#include <iomanip>
#include <sstream>

namespace render
{
	namespace
	{
		static std::string const Time = "Debug_Time";
		static std::string const Fps = "Debug_FPS";
		static std::string const Version = "Debug_Version";
		static std::string const BillboardCount = "Debug_BillboardCount";
		static std::string const BufferCount = "Debug_BufferCount";
	}

	Debug::Debug( bool enable
		, render::Scene & scene
		, render::FontLoader & loader )
		: m_enabled{ enable }
	{
		if ( m_enabled )
		{
			render::FontPtr font = std::make_unique< render::Font >( "Arial"
				, 32 );
			render::loadFont( loader, *font );
			m_fontTexture = std::make_unique< render::FontTexture >
				( std::move( font ) );

			auto material = scene.materials().findElement( "FullAlphaWhite" );

			m_scene = &scene;
			m_version = std::make_shared< TextOverlay >();
			m_version->position( { 0, 0 } );
			m_version->material( material );
			m_version->caption( gl::OpenGL::getVersion() );
			m_version->fontTexture( *m_fontTexture );
			m_scene->overlays().addElement( Version, m_version );

			m_time = std::make_shared< TextOverlay >();
			m_time->position( { 0, 40 } );
			m_time->material( material );
			m_time->fontTexture( *m_fontTexture );
			m_scene->overlays().addElement( Time, m_time );

			m_fps = std::make_shared< TextOverlay >();
			m_fps->position( { 0, 80 } );
			m_fps->material( material );
			m_fps->fontTexture( *m_fontTexture );
			m_scene->overlays().addElement( Fps, m_fps );

			m_billboardCount = std::make_shared< TextOverlay >();
			m_billboardCount->position( { 0, 120 } );
			m_billboardCount->material( material );
			m_billboardCount->fontTexture( *m_fontTexture );
			m_scene->overlays().addElement( BillboardCount, m_billboardCount );

			m_buffersCount = std::make_shared< TextOverlay >();
			m_buffersCount->position( { 0, 160 } );
			m_buffersCount->material( material );
			m_buffersCount->fontTexture( *m_fontTexture );
			m_scene->overlays().addElement( BufferCount, m_buffersCount );
		}
	}

	Debug::~Debug()
	{
		if ( m_enabled )
		{
			m_time.reset();
			m_fps.reset();
			m_buffersCount.reset();
			m_billboardCount.reset();
			m_version.reset();
			m_scene->overlays().removeElement( BufferCount );
			m_scene->overlays().removeElement( BillboardCount );
			m_scene->overlays().removeElement( Version );
			m_scene->overlays().removeElement( Time );
			m_fontTexture.reset();
		}
	}

	void Debug::start()
	{
		if ( m_enabled )
		{
			m_startTime = Clock::now();
		}
	}

	void Debug::end()
	{
		if ( m_enabled )
		{
			auto end = Clock::now();
			auto duration = std::chrono::duration_cast< std::chrono::microseconds >( end - m_startTime );
			{
				std::stringstream stream;
				stream << std::fixed << std::setprecision( 2 ) << duration.count() / 1000.0 << "ms";
				m_time->caption( "Time: " + stream.str() );
			}
			{
				std::stringstream stream;
				stream << std::fixed << std::setprecision( 2 ) << 1000.0 / ( duration.count() / 1000.0 );
				m_fps->caption( "Fps: " + stream.str() );
			}
		}
	}

	void Debug::count( BillboardArray const & billboards
		, BillboardList const & buffers )
	{
		if ( m_enabled )
		{
			{
				auto billboardsCount = 0u;

				for ( auto & billboard : billboards )
				{
					billboardsCount += billboard->buffer().count();
				}
				{
					std::stringstream stream;
					stream << billboardsCount;
					m_billboardCount->caption( "Billboards: " + stream.str() );
				}
			}
			{
				auto buffersCount = 0u;

				for ( auto & billboard : buffers )
				{
					buffersCount += billboard.second->count() ? 1 : 0;
				}
				{
					std::stringstream stream;
					stream << buffersCount;
					m_buffersCount->caption( "Buffers: " + stream.str() );
				}
			}
		}
	}
}
