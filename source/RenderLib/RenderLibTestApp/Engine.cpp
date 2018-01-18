#include "stdafx.h"

#include "Engine.h"
#include "resource.h"

#include <DesktopUtils/FontLoader.h>
#include <DesktopUtils/ObjLoader.h>
#include <DesktopUtils/Log.h>

#include <RenderLib/Billboard.h>
#include <RenderLib/Object.h>
#include <RenderLib/PanelOverlay.h>
#include <RenderLib/PolyLine.h>
#include <RenderLib/Scene.h>

#include <array>
#include <iostream>
#include <string>

Engine::Engine()
	: MsWindow{}
{
	std::string const appName = "RenderLibTestApp";
	m_cout = new utils::LogStreambuf< utils::InfoLogStreambufTraits >( appName
		, std::cout );
	m_cerr = new utils::LogStreambuf< utils::ErrorLogStreambufTraits >( appName
		, std::cerr );
	m_clog = new utils::LogStreambuf< utils::DebugLogStreambufTraits >( appName
		, std::clog );
}

Engine::~Engine()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Engine::onDraw()
{
	if ( m_window )
	{
		auto start = std::chrono::high_resolution_clock::now();
		m_onDraw();
		m_window->beginFrame();
		m_window->update();
		m_window->updateOverlays();
		m_window->updateUbos();
		m_window->draw();
		m_window->endFrame();
		auto end = std::chrono::high_resolution_clock::now();
		doUpdateFps( std::chrono::duration_cast< std::chrono::microseconds >( end - start ) );
	}
}

void Engine::onCreate()
{
	doInitialise3DElements();
}

void Engine::onDestroy()
{
	doCleanup3DElements();
}

void Engine::onMinimise()
{
}

void Engine::onRestore( renderer::IVec2 const & event )
{
	if ( m_window )
	{
		m_window->resize( event );
	}
}

void Engine::onResize( renderer::IVec2 const & event )
{
	if ( m_window )
	{
		m_window->resize( event );
	}
}

void Engine::onMouseMove( utils::MouseEvent const & event )
{
	if ( event.ldown )
	{
		auto diff = event.position - m_mouse;
		m_window->state().velocity( renderer::Vec2{ diff } );
		m_mouse = event.position;
	}
}

void Engine::onLButtonDown( utils::MouseEvent const & event )
{
	m_mouse = event.position;
}

void Engine::onLButtonClick( utils::MouseEvent const & event )
{
	if ( m_window )
	{
		m_window->pick( event.position );
	}
}

void Engine::onLButtonDblClick( utils::MouseEvent const & event )
{
	m_window->state().reset();
}

void Engine::onMouseWheel( utils::MouseEvent const & event )
{
	m_window->state().zoomVelocity( float( event.delta ) );
}

void Engine::onObjectPicked( render::Object & object )
{
	if ( m_picked )
	{
		m_pickedObject = &object;
		doUpdatePicked( object );
	}
}

void Engine::onBillboardPicked( render::Billboard & billboard, uint32_t index )
{
	if ( m_picked )
	{
		m_pickedBillboard = &billboard;
		doUpdatePicked( billboard, index );
	}
}

void Engine::onUnpick()
{
	if ( m_picked )
	{
		m_picked->show( false );
	}
}

void Engine::doInitialise3DElements()
{
	auto width = size().x;
	auto height = size().y;
	float ratio = float( width ) / height;

	// Initialise the render window
	utils::FontLoader loader{ "arial.ttf" };
	m_window = std::make_unique< render::RenderWindow >( *m_device
		, renderer::IVec2{ width, height }
		, loader
		, false );
	m_window->viewport().fovY( utils::Angle{ 45.0_degrees } );

	// Initialise the scene
	auto & scene = m_window->scene();
	scene.backgroundColour( renderer::RgbaColour{ 0.5, 0.5, 0.5, 1.0 } );

	// Populate the scene
	//std::string content = utils::getFileTextContent( "tex_cube.obj" );

	//if ( !content.empty() )
	//{
	//	render::ObjectPtr object = utils::loadObjFile( m_window->getDevice()
	//		, m_window->getStagingBuffer()
	//		, m_window->getDefaultResources().getCommandBuffer()
	//		, "Cube"
	//		, content
	//		, scene.materials()
	//		, scene.textures()
	//		, scene.meshes() );
	//	object->moveTo( renderer::Vec3{ 0.0, 0.0, 52.0 } );
	//	scene.add( object );
	//}

	auto texture = scene.textures().findElement( "texture.bmp" );

	if ( !texture )
	{
		texture = std::make_shared< render::Texture >( *m_device );
		render::loadTexture( m_window->getStagingBuffer()
			, m_window->getDefaultResources().getCommandBuffer()
			, utils::getFileBinaryContent( "texture.bmp" )
			, *texture );
		scene.textures().addElement( "texture.bmp", texture );
	}

	auto opacity = scene.textures().findElement( "halo.bmp" );

	if ( !opacity )
	{
		opacity = std::make_shared< render::Texture >( *m_device );
		render::loadTexture( m_window->getStagingBuffer()
			, m_window->getDefaultResources().getCommandBuffer()
			, utils::getFileBinaryContent( "halo.bmp" )
			, *opacity );
		scene.textures().addElement( "halo.bmp", opacity );
	}
///*
//	auto pickedMat = std::make_shared< render::Material >();
//	pickedMat->opacityMap( opacity );
//	pickedMat->ambient( renderer::RgbColour{ 0.0, 0.0, 0.5 } );
//	pickedMat->diffuse( renderer::RgbColour{ 0.0, 0.0, 0.5 } );
//	scene.materials().addElement( "picked", pickedMat );
//
//	auto pickedBuffers = std::make_shared< render::BillboardBuffer >( *m_device
//		, false );
//	pickedBuffers->add( { -1000.0f, renderer::Vec3{ 0, 0, 0 }, renderer::Vec2{ 1, 1 } } );
//	scene.addBillboardBuffer( "picked", pickedBuffers );
//	m_picked = std::make_shared< render::Billboard >( "picked", *pickedBuffers );
//	m_picked->material( pickedMat );
//	m_picked->show( false );
//	scene.add( m_picked );
//*/
	auto billboardMat = std::make_shared< render::Material >();
	//billboardMat->diffuseMap( texture );
	//billboardMat->opacityMap( opacity );
	billboardMat->ambient( renderer::RgbColour{ 1.0, 0.0, 0.5 } );
	billboardMat->diffuse( renderer::RgbColour{ 1.0, 0.0, 0.5 } );
	billboardMat->emissive( renderer::RgbColour{ 1.0, 0.0, 0.5 } );
	scene.materials().addElement( "billboard", billboardMat );
	auto billboardBuffer = std::make_shared< render::BillboardBuffer >( *m_device
		, false );
	billboardBuffer->add( { -100.0f, renderer::Vec3{ 1, 0, 0 }, renderer::Vec2{ 1, 1 } } );
	//billboardBuffer->add( { -100.0f, renderer::Vec3{ 0, 1, 0 }, renderer::Vec2{ 1, 0.5 } } );
	//billboardBuffer->add( { -100.0f, renderer::Vec3{ -1, 0, 0 }, renderer::Vec2{ 0.5, 1 } } );
	//billboardBuffer->add( { -100.0f, renderer::Vec3{ 0, -1, 0 }, renderer::Vec2{ 1.5, 1.5 } } );
	scene.addBillboardBuffer( "billboard", billboardBuffer );
	billboardBuffer->initialise();
	auto billboard = std::make_shared< render::Billboard >( "billboard", *billboardBuffer );
	billboard->dimensions( renderer::IVec2{ 1, 1 } );
	billboard->moveTo( renderer::Vec3{ 0, 0, 50 } );
	billboard->scale( renderer::Vec3{ 1.5, 1.5, 1.5 } );
	billboard->material( billboardMat );
	scene.add( billboard );
///*
//	auto starsMat = std::make_shared< render::Material >();
//	starsMat->opacityMap( opacity );
//	starsMat->ambient( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	starsMat->diffuse( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	starsMat->emissive( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	starsMat->alphaTest( true );
//	scene.materials().addElement( "stars", starsMat );
//	auto halosMat = std::make_shared< render::Material >();
//	halosMat->opacityMap( opacity );
//	halosMat->ambient( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	halosMat->diffuse( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	halosMat->emissive( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	scene.materials().addElement( "halos", halosMat );
//	auto starsBuffers = std::make_shared< render::BillboardBuffer >( *m_device
//		, false );
//	starsBuffers->add( { 50.0f, renderer::Vec3{ -1, 1, 0 }, renderer::Vec2{ 1, 1 } } );
//	starsBuffers->add( { 50.0f, renderer::Vec3{ 1, 1, 0 }, renderer::Vec2{ 1, 0.5 } } );
//	starsBuffers->add( { 50.0f, renderer::Vec3{ 1, -1, 0 }, renderer::Vec2{ 0.5, 1 } } );
//	starsBuffers->add( { 50.0f, renderer::Vec3{ -1, -1, 0 }, renderer::Vec2{ 1.5, 1.5 } } );
//	scene.addBillboardBuffer( "stars", starsBuffers );
//	auto stars = std::make_shared< render::Billboard >( "stars", *starsBuffers );
//	stars->dimensions( renderer::IVec2{ 1, 1 } );
//	stars->moveTo( renderer::Vec3{ 0, 0, 50 } );
//	stars->material( starsMat );
//	scene.add( stars );
//	auto halos = std::make_shared< render::Billboard >( "halos", *starsBuffers );
//	halos->dimensions( renderer::IVec2{ 2, 2 } );
//	halos->moveTo( renderer::Vec3{ 0, 0, 50 } );
//	halos->material( halosMat );
//	scene.add( halos );
//
//	auto linesMat = std::make_shared< render::Material >();
//	linesMat->ambient( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	linesMat->diffuse( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	linesMat->emissive( renderer::RgbColour{ 1.0, 1.0, 0.5 } );
//	scene.materials().addElement( "lines", linesMat );
//	auto lines = std::make_shared< render::PolyLine >( "lines" );
//	lines->add( { renderer::Vec3{ -1, 1, 0 }, renderer::Vec3{ 1, 1, 0 } } );
//	lines->add( { renderer::Vec3{ 1, 1, 0 }, renderer::Vec3{ 1, -1, 0 } } );
//	lines->add( { renderer::Vec3{ 1, -1, 0 }, renderer::Vec3{ -1, -1, 0 } } );
//	lines->add( { renderer::Vec3{ -1, -1, 0 }, renderer::Vec3{ -1, 1, 0 } } );
//	lines->moveTo( renderer::Vec3{ 0, 0, 50 } );
//	lines->material( linesMat );
//	scene.add( lines );
//*/
	//{
	//	auto content = utils::getFileBinaryContent( "arial.ttf" );
	//	render::FontPtr font = std::make_unique< render::Font >( "Arial", 32u );
	//	utils::FontLoader loader{ "arial.ttf" };
	//	render::loadFont( loader, *font );
	//	m_fontTexture = std::make_unique< render::FontTexture >( *m_device
	//		, m_window->getStagingBuffer()
	//		, m_window->getDefaultResources().getCommandBuffer()
	//		, std::move( font ) );
	//}
	//auto coinMat = doCreateOverlayMaterial( "coin", renderer::RgbColour{ 0, 1, 0 }, 1 );
	//auto coin = std::make_shared< render::TextOverlay >();
	//coin->position( renderer::IVec2{ 200, 200 } );
	//coin->material( coinMat );
	//coin->caption( "coin !!" );
	//coin->fontTexture( *m_fontTexture );
	//scene.addOverlay( "coin", coin );

	//auto glopMat = doCreateOverlayMaterial( "glop", renderer::RgbColour{ 1, 0, 0 }, 1 );
	//auto glop = std::make_shared< render::TextOverlay >();
	//glop->position( renderer::IVec2{ 400, 300 } );
	//glop->material( glopMat );
	//glop->caption( "glop !" );
	//glop->fontTexture( *m_fontTexture );
	//scene.addOverlay( "glop", glop );

	//auto miaouMat = doCreateOverlayMaterial( "miaou", renderer::RgbColour{ 0, 1, 0 }, 1.0 );
	//miaouMat->alphaTest( true );
	//miaouMat->diffuseMap( texture );
	//miaouMat->opacityMap( opacity );
	//auto miaou = std::make_shared< render::PanelOverlay >();
	//miaou->position( renderer::IVec2{ 100, 100 } );
	//miaou->size( renderer::IVec2{ 200, 200 } );
	//miaou->material( miaouMat );
	//scene.addOverlay( "miaou", miaou );

	//auto bizuizuiMat = doCreateOverlayMaterial( "bizuizui", renderer::RgbColour{ 0, 0, 1 }, 1.0 );
	//bizuizuiMat->diffuseMap( texture );
	//bizuizuiMat->opacityMap( opacity );
	//auto bizuizui = std::make_shared< render::PanelOverlay >();
	//bizuizui->position( renderer::IVec2{ 400, 100 } );
	//bizuizui->size( renderer::IVec2{ 200, 200 } );
	//bizuizui->material( bizuizuiMat );
	//scene.addOverlay( "bizuizui", bizuizui );

	//auto trucPanelMat = doCreateOverlayMaterial( "trucPanel", renderer::RgbColour{ 0, 0, 1 }, 1.0 );
	//auto trucBorderMat = doCreateOverlayMaterial( "trucBorder", renderer::RgbColour{ 1, 0, 1 }, 1.0 );
	//auto truc = std::make_shared< render::BorderPanelOverlay >();
	//truc->position( renderer::IVec2{ 700, 100 } );
	//truc->size( renderer::IVec2{ 200, 200 } );
	//truc->borderSize( renderer::IVec4{ 20, 20, 20, 20 } );
	//truc->material( trucPanelMat );
	//truc->borderMaterial( trucBorderMat );
	//scene.addOverlay( "truc", truc );

	//auto bidulePanelMat = doCreateOverlayMaterial( "bidulePanel", renderer::RgbColour{ 0, 1, 1 }, 1.0 );
	//auto biduleBorderMat = doCreateOverlayMaterial( "biduleBorder", renderer::RgbColour{ 1, 1, 0 }, 1.0 );
	//auto bidule = std::make_shared< render::BorderPanelOverlay >();
	//bidule->position( renderer::IVec2{ 1000, 100 } );
	//bidule->size( renderer::IVec2{ 200, 200 } );
	//bidule->borderSize( renderer::IVec4{ 20, 20, 20, 20 } );
	//bidule->material( bidulePanelMat );
	//bidule->borderMaterial( biduleBorderMat );
	//scene.addOverlay( "bidule", bidule );

	//m_onObjectPicked = m_window->picking().onObjectPicked.connect
	//	( std::bind( &Engine::onObjectPicked
	//		, this
	//		, std::placeholders::_1 ) );
	//m_onBillboardPicked = m_window->picking().onBillboardPicked.connect
	//	( std::bind( &Engine::onBillboardPicked
	//		, this
	//		, std::placeholders::_1
	//		, std::placeholders::_2 ) );
	//m_onUnpick = m_window->picking().onUnpick.connect
	//	( std::bind( &Engine::onUnpick
	//		, this ) );
}

void Engine::doCleanup3DElements()
{
	m_onDrawConnection.disconnect();
	onUnpick();
	m_picked.reset();
	m_onObjectPicked.disconnect();
	m_onBillboardPicked.disconnect();
	m_fontTexture.reset();
	m_window.reset();
}

render::MaterialPtr Engine::doCreateOverlayMaterial( std::string const & name
	, renderer::RgbColour const & colour
	, float opacity )
{
	render::MaterialPtr result = m_window->scene().materials().findElement( name );

	if ( !result )
	{
		result = std::make_unique< render::Material >();
		result->ambient( colour );
		result->diffuse( colour );
		result->specular( colour );
		result->emissive( colour );
		result->opacity( opacity );
		m_window->scene().materials().addElement( name, result );
	}

	return result;
}

void Engine::doUpdatePicked( render::Movable const & movable )
{
	m_picked->rescale( movable.scale() );
	m_picked->reorient( movable.orientation() );
	m_picked->update();
	m_picked->show( true );
}

void Engine::doUpdatePicked( render::Object const & object )
{
	m_picked->moveTo( object.position() - renderer::Vec3{ 0, 0, object.boundaries().z + 0.1 } );
	doUpdatePicked( static_cast< render::Movable const & >( object ) );
	m_picked->dimensions( renderer::IVec2{ utils::toVec2( object.boundaries() ) } );
	m_picked->buffer().at( 0u, { -1000.0f, renderer::Vec3{ 0, 0, 0 }, renderer::Vec2{ 1, 1 } } );
}

void Engine::doUpdatePicked( render::Billboard const & billboard
	, uint32_t index )
{
	if ( &billboard == m_picked.get() )
	{
		m_picked->show( false );
	}
	else
	{
		m_picked->moveTo( billboard.position() - renderer::Vec3{ 0, 0, 0.2 } );
		doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
		m_picked->dimensions( billboard.dimensions() );
		auto data = billboard.buffer()[index];
		auto scale = 0.1f + m_window->state().zoomBounds().percent( m_window->state().zoom() );
		m_picked->buffer().at( 0u
			, { -1000.0f, data.center, renderer::Vec2{ scale, scale } } );
	}
}
