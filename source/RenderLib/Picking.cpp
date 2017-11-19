#include "Picking.h"

#include "Billboard.h"
#include "Camera.h"
#include "Mesh.h"
#include "Object.h"
#include "Scene.h"
#include "Submesh.h"
#include "Viewport.h"

namespace render
{
	namespace
	{
		template< typename NodeT >
		struct NodeTraits;

		template<>
		struct NodeTraits< Object >
		{
			static inline void getNodeValue( RenderSubmeshVector::const_iterator it
				, Object *& object )
			{
				object = it->m_object.get();
			}
		};

		template<>
		struct NodeTraits< Billboard >
		{
			static inline void getNodeValue( BillboardArray::const_iterator it
				, Billboard *& billboard )
			{
				billboard = it->get();
			}
		};

		template< typename MapT, typename NodeT >
		inline void doPickFromList( MapT const & map
			, gl::IVec4 const & data
			, NodeT *& result )
		{
			using Traits = NodeTraits< NodeT >;
			uint8_t node( data.y );
			uint32_t index( data.z );
			assert( map[node].size() > index );
			Traits::getNodeValue( map[node].begin() + index, result );
		}

		static int constexpr PickingWidth = 50;
		static int constexpr PickingOffset = PickingWidth / 2;
	}

	Picking::Picking( gl::IVec2 const & size )
		: m_renderer{}
		, m_size{ size }
		, m_colour{ std::make_unique< gl::Texture >
			( gl::PixelFormat::eR8G8B8A8
			, size ) }
		, m_depth{ std::make_unique< gl::RenderBuffer >
			( gl::PixelFormat::eD16
			, size ) }
		, m_fbo{ std::make_unique< gl::FrameBuffer >() }
		, m_buffer( PickingWidth * PickingWidth )
	{
		m_renderer.initialise();
		m_fbo->bind();
		m_fbo->attach( *m_colour, gl::AttachmentPoint::eColour0 );
		m_fbo->attach( *m_depth, gl::AttachmentPoint::eDepth );
		m_fbo->unbind();
	}

	Picking::~Picking()
	{
		m_fbo->bind();
		m_fbo->detach( *m_colour, gl::AttachmentPoint::eColour0 );
		m_fbo->detach( *m_depth, gl::AttachmentPoint::eDepth );
		m_fbo->unbind();
	}

	Picking::NodeType Picking::pick( gl::IVec2 const & position
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
#if DEBUG_PICKING
		m_fbo->bind();
		m_fbo->clear( gl::RgbaColour{ 0, 0, 0, 1 } );
		m_renderer.draw( camera
			, zoomPercent
			, objects
			, billboards );
		m_fbo->unbind();
		return NodeType::eNone;
#else
		onUnpick();
		auto pixel = doFboPick( position
			, camera
			, zoomPercent
			, objects
			, billboards );
		return doPick( pixel, objects, billboards );
#endif
	}

	Picking::Pixel Picking::doFboPick( gl::IVec2 const & position
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		assert( position.x < m_size.x );
		assert( position.y < m_size.y );
		m_fbo->bind();
		camera.viewport().apply();
		m_fbo->clear( gl::RgbaColour{ 0, 0, 0, 1 } );
		m_renderer.draw( camera
			, zoomPercent
			, objects
			, billboards );
		m_fbo->unbind();
		memset( m_buffer.data(), 0xFF, m_buffer.size() * sizeof( Pixel ) );
		gl::IVec2 offset
		{
			m_size.x - position.x - PickingOffset,
			m_size.y - position.y - PickingOffset
		};
		m_fbo->bind();
		m_fbo->download( uint32_t( offset.x )
			, uint32_t( offset.y )
			, PickingWidth
			, PickingWidth
			, gl::PixelFormat::eR8G8B8A8
			, reinterpret_cast< uint8_t * >( m_buffer.data() ) );
		m_fbo->unbind();

		return m_buffer[( PickingOffset * PickingWidth ) + PickingOffset - 1];
	}

	Picking::NodeType Picking::doPick( Pixel const & pixel
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		NodeType ret{ NodeType::eNone };

		if ( pixel.r > 0 || pixel.g > 0 || pixel.b > 0 )
		{
			auto data = doUnpackPixel( pixel );

			if ( data.x == int( ObjectType::eObject ) )
			{
				Object * object{ nullptr };
				doPickFromList( objects
					, data
					, object );
				onObjectPicked( *object );
			}
			else if ( data.x == int( ObjectType::eBillboard ) )
			{
				Billboard * billboard{ nullptr };
				doPickFromList( billboards
					, data
					, billboard );
				onBillboardPicked( *billboard, uint32_t( data.w ) );
			}
		}

		return ret;
	}

	gl::IVec4 Picking::doUnpackPixel( Pixel pixel )
	{
		static constexpr uint8_t ObjectTypeMask{ ObjectMask | BillboardMask };
		static constexpr uint8_t NodeTypeMask{ 0x3F };
		static constexpr uint8_t NodeTypeOffset{ 0x02 };
		auto objectType = ( pixel.r >> 2 ) & ObjectTypeMask;
		auto nodeType = ( pixel.r & NodeTypeMask ) >> NodeTypeOffset;
		pixel.r &= 0x03;

		switch ( objectType )
		{
		case ObjectMask:
			objectType = int( ObjectType::eObject );
			return gl::IVec4{ objectType, nodeType, doUnpackObjectPixel( pixel ) };

		case BillboardMask:
			objectType = int( ObjectType::eBillboard );
			return gl::IVec4{ objectType, nodeType, doUnpackBillboardPixel( pixel ) };

		default:
			assert( false && "Unsupported object type" );
			return gl::IVec4{};
			break;
		}
	}

	gl::IVec2 Picking::doUnpackBillboardPixel( Pixel pixel )
	{
		auto index = ( uint32_t( pixel.r ) << 6 )
			| ( uint32_t( pixel.g & 0xFC ) >> 2 );
		auto instance = ( uint32_t( pixel.g & 0x03 ) << 16 )
			| ( uint32_t( pixel.b ) << 8 )
			| ( uint32_t( pixel.a ) );
		return gl::IVec2{ index, instance };
	}

	gl::IVec2 Picking::doUnpackObjectPixel( Pixel pixel )
	{
		auto index = ( uint32_t( pixel.r ) << 24 )
			| ( uint32_t( pixel.g ) << 16 )
			| ( uint32_t( pixel.b ) << 8 )
			| ( uint32_t( pixel.a ) );
		return gl::IVec2{ index, 0 };
	}
}
