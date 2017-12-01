#include "Picking.h"

#include "Billboard.h"
#include "Camera.h"
#include "Mesh.h"
#include "Object.h"
#include "Scene.h"
#include "Submesh.h"
#include "Viewport.h"

#include <Renderer/RenderPass.hpp>
#include <Renderer/RenderPassState.hpp>
#include <Renderer/RenderSubpass.hpp>
#include <Renderer/RenderSubpassState.hpp>

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
			static inline void getNodeValue( RenderBillboardVector::const_iterator it
				, Billboard *& billboard )
			{
				billboard = it->m_billboard.get();
			}
		};

		template< typename MapT, typename NodeT >
		inline void doPickFromList( MapT const & map
			, utils::IVec4 const & data
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

		std::vector< utils::PixelFormat > doGetPixelFormats()
		{
			return { utils::PixelFormat::eR8G8B8A8, utils::PixelFormat::eD16 };
		}

		renderer::RenderSubpassState doGetSubpassState()
		{
			return { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite };
		}

		renderer::RenderPassState doGetColourPassState()
		{
			return { renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::AccessFlag::eColourAttachmentWrite
				, { renderer::ImageLayout::eColourAttachmentOptimal
					, renderer::ImageLayout::eDepthStencilAttachmentOptimal } };
		}
	}

	Picking::Picking( renderer::Device const & device
		, utils::IVec2 const & size )
		: m_renderPass{ std::make_unique< renderer::RenderPass >( device
			, doGetPixelFormats()
			, renderer::RenderSubpassArray{ { device, doGetPixelFormats(), doGetSubpassState() } }
			, doGetColourPassState()
			, doGetColourPassState() ) }
		, m_renderer{ device, *m_renderPass }
		, m_size{ size }
		, m_colour{ std::make_shared< renderer::Texture >( device ) }
		, m_depth{ std::make_shared< renderer::Texture >( device ) }
		, m_buffer( PickingWidth * PickingWidth )
		, m_stagingBuffer{ std::make_shared< renderer::StagingBuffer >( device ) }
		, m_commandPool{ std::make_unique< renderer::CommandPool >( device
			, device.getGraphicsQueue().getFamilyIndex() ) }
		, m_commandBuffer{ std::make_unique< renderer::CommandBuffer >( device
			, *m_commandPool ) }
	{
		m_colour->setImage( utils::PixelFormat::eR8G8B8A8, size, renderer::ImageUsageFlag::eColourAttachment );
		m_depth->setImage( utils::PixelFormat::eD16, size, renderer::ImageUsageFlag::eDepthStencilAttachment );
		m_frameBuffer = std::make_shared< renderer::FrameBuffer >( *m_renderPass
			, size
			, renderer::TextureCRefArray{ *m_colour, *m_depth } );
		m_renderer.initialise();
	}

	Picking::~Picking()
	{
	}

	Picking::NodeType Picking::pick( renderer::Queue const & queue
		, utils::IVec2 const & position
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
#if DEBUG_PICKING
		m_frameBuffer->bind();
		m_frameBuffer->clear( renderer::RgbaColour{ 0, 0, 0, 1 } );
		m_renderer.draw( camera
			, zoomPercent
			, objects
			, billboards );
		m_frameBuffer->unbind();
		return NodeType::eNone;
#else
		onUnpick();
		auto pixel = doFboPick( *m_commandBuffer
			, queue
			, position
			, camera
			, zoomPercent
			, objects
			, billboards );
		return doPick( pixel, objects, billboards );
#endif
	}

	Picking::Pixel Picking::doFboPick( renderer::CommandBuffer const & commandBuffer
		, renderer::Queue const & queue
		, utils::IVec2 const & position
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		assert( position.x < m_size.x );
		assert( position.y < m_size.y );
		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, {
					renderer::ClearValue{ utils::RgbaColour{ 0, 0, 0, 1 } },
					renderer::ClearValue{ renderer::DepthStencilClearValue{ 1.0f, 0 } }
				}
				, renderer::SubpassContents::eInline );
			commandBuffer.setViewport( camera.viewport().viewport() );
			m_renderer.draw( *m_stagingBuffer
				, commandBuffer
				, camera
				, zoomPercent
				, objects
				, billboards );
			commandBuffer.endRenderPass();

			queue.submit( commandBuffer, nullptr );
		}

		memset( m_buffer.data(), 0xFF, m_buffer.size() * sizeof( Pixel ) );
		utils::IVec2 offset
		{
			m_size.x - position.x - PickingOffset,
			m_size.y - position.y - PickingOffset
		};
		m_frameBuffer->download( queue
			, 0u
			, uint32_t( offset.x )
			, uint32_t( offset.y )
			, PickingWidth
			, PickingWidth
			, utils::PixelFormat::eR8G8B8A8
			, reinterpret_cast< uint8_t * >( m_buffer.data() ) );

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

	utils::IVec4 Picking::doUnpackPixel( Pixel pixel )
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
			return utils::IVec4{ objectType, nodeType, doUnpackObjectPixel( pixel ) };

		case BillboardMask:
			objectType = int( ObjectType::eBillboard );
			return utils::IVec4{ objectType, nodeType, doUnpackBillboardPixel( pixel ) };

		default:
			assert( false && "Unsupported object type" );
			return utils::IVec4{};
			break;
		}
	}

	utils::IVec2 Picking::doUnpackBillboardPixel( Pixel pixel )
	{
		auto index = ( uint32_t( pixel.r ) << 6 )
			| ( uint32_t( pixel.g & 0xFC ) >> 2 );
		auto instance = ( uint32_t( pixel.g & 0x03 ) << 16 )
			| ( uint32_t( pixel.b ) << 8 )
			| ( uint32_t( pixel.a ) );
		return utils::IVec2{ index, instance };
	}

	utils::IVec2 Picking::doUnpackObjectPixel( Pixel pixel )
	{
		auto index = ( uint32_t( pixel.r ) << 24 )
			| ( uint32_t( pixel.g ) << 16 )
			| ( uint32_t( pixel.b ) << 8 )
			| ( uint32_t( pixel.a ) );
		return utils::IVec2{ index, 0 };
	}
}
