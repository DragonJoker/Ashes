#include "OverlayRenderer.h"

#include "Texture.h"

#include <Renderer/Attribute.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayoutBinding.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/StagingBuffer.hpp>
#include <Renderer/VertexBuffer.hpp>

#include "BorderPanelOverlay.h"
#include "ElementsList.h"
#include "Material.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"
#include "UberShader.h"
#include "Viewport.h"

namespace render
{
	//*************************************************************************

	namespace
	{
		void doFillBuffers( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Overlay::QuadArray::const_iterator begin
			, uint32_t count
			, uint32_t & offset
			, renderer::VertexBuffer< Overlay::Quad > const & buffer )
		{
			stagingBuffer.copyVertexData( commandBuffer
				, reinterpret_cast< uint8_t const * const >( &( *begin ) )
				, uint32_t( count * sizeof( Overlay::Quad ) )
				, uint32_t( offset * sizeof( Overlay::Quad ) )
				, buffer
				, renderer::PipelineStageFlag::eVertexInput );
			offset += count;
		}

		std::vector< OverlayPtr > doSortPerZIndex( OverlayList const & overlays
			, utils::IVec2 const & size )
		{
			std::vector< OverlayPtr > result;

			for ( auto & overlay : overlays )
			{
				auto & ovLeftTop = overlay.second->position();
				auto ovRightBottom = ovLeftTop + overlay.second->size();

				if ( overlay.second->visible()
					&& ovLeftTop.x < size.x
					&& ovLeftTop.y < size.y
					&& ovRightBottom.x > 0
					&& ovRightBottom.y > 0 )
				{
					auto it = std::lower_bound( std::begin( result )
						, std::end( result )
						, overlay.second->index()
						, []( OverlayPtr const & res, uint32_t index )
					{
						return res->index() <= index;
					} );

					if ( it == std::end( result ) )
					{
						result.push_back( overlay.second );
					}
					else
					{
						result.insert( it, overlay.second );
					}
				}
			}

			return result;
		}

		OverlayNodeArray doCreatePanelNodes( renderer::Device const & device
			, renderer::RenderPass const & renderPass )
		{
			return
			{
				{
					OverlayNode{ device, renderPass, false, OpacityType::eOpaque, TextureFlag::eNone },
					OverlayNode{ device, renderPass, false, OpacityType::eOpaque, TextureFlag::eDiffuse },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eNone },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eOpacity },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaBlend, TextureFlag::eDiffuse | TextureFlag::eOpacity },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eNone },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eDiffuse },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eOpacity },
					OverlayNode{ device, renderPass, false, OpacityType::eAlphaTest, TextureFlag::eDiffuse | TextureFlag::eOpacity },
				}
			};
		}

		renderer::DescriptorSetLayout doCreateMaterialDescriptorLayout( renderer::Device const & device
			, TextureFlags flags )
		{
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			if ( checkFlag( flags, TextureFlag::eDiffuse ) )
			{
				// Diffuse texture.
				bindings.emplace_back( index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment );
			}

			if ( checkFlag( flags, TextureFlag::eOpacity ) )
			{
				// Opacity texture
				bindings.emplace_back( index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment );
			}

			return renderer::DescriptorSetLayout{ device, bindings };
		}

		renderer::DescriptorSet doCreateMaterialDescriptor( renderer::DescriptorSetPool const & pool
			, Material const & material )
		{
			renderer::DescriptorSet result{ pool };
			uint32_t index = 0u;

			if ( material.hasDiffuseMap() )
			{
				result.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
					, material.diffuseMap().texture()
					, material.diffuseMap().sampler() );
			}

			if ( material.hasOpacityMap() )
			{
				result.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
					, material.opacityMap().texture()
					, material.opacityMap().sampler() );
			}

			return result;
		}

		renderer::DescriptorSet doCreateMaterialDescriptor( renderer::DescriptorSetPool const & pool
			, Material const & material
			, Texture const & fontTexture )
		{
			renderer::DescriptorSet result{ pool };
			uint32_t index = 0u;

			if ( material.hasDiffuseMap() )
			{
				result.createBinding( renderer::DescriptorSetLayoutBinding{ index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment }
					, material.diffuseMap().texture()
					, material.diffuseMap().sampler() );
			}

			result.createBinding( renderer::DescriptorSetLayoutBinding{ index++
				, renderer::DescriptorType::eSampledImage
				, renderer::ShaderStageFlag::eFragment }
				, fontTexture.texture()
				, fontTexture.sampler() );

			return result;
		}

		renderer::DescriptorSetLayout doCreateUboDescriptorLayout( renderer::Device const & device )
		{
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( index++
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment );

			return renderer::DescriptorSetLayout{ device, bindings };
		}
	}

	//*************************************************************************

	OverlayNode::OverlayNode( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, bool text
		, OpacityType opacity
		, TextureFlags textures )
		: m_program{ UberShader::createShaderProgram( device
			, RenderType::eScene
			, textures
			, opacity
			, text ? ObjectType::eTextOverlay : ObjectType::ePanelOverlay ) }
		, m_pipelineLayout{ device, nullptr }
		, m_layout{ 0u }
		, m_overlayUbo{ device, 1u, renderer::BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal }
		, m_materialDescriptorLayout{ doCreateMaterialDescriptorLayout( device, textures ) }
		, m_materialDescriptorPool{ m_materialDescriptorLayout }
		, m_uboDescriptorLayout{ doCreateUboDescriptorLayout( device ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout }
		, m_uboDescriptor{ m_uboDescriptorPool }
	{
		m_pipeline = std::make_shared< renderer::Pipeline >( device
			, m_pipelineLayout
			, *m_program
			, renderer::VertexLayoutCRefArray
			{
				m_layout
			}
			, renderPass
			, renderer::PrimitiveTopology::eTriangleFan );

		m_layout.createAttribute< utils::Vec2 >( 0u
			, offsetof( TextOverlay::Vertex, position ) );

		if ( textures )
		{
			m_layout.createAttribute< utils::Vec2 >( 1u
				, offsetof( TextOverlay::Vertex, texture ) );
		}

		m_uboDescriptor.createBinding( renderer::DescriptorSetLayoutBinding{ 0u
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eVertex | renderer::ShaderStageFlag::eFragment }
			, m_overlayUbo );
	}

	//*************************************************************************

	OverlayRenderer::OverlayRenderer( renderer::Device const & device
		, renderer::RenderPass const & renderPass
		, uint32_t maxCharsPerBuffer )
		: m_device{ device }
		, m_renderPass{ renderPass }
		, m_panelNodes{ doCreatePanelNodes( device, renderPass ) }
		, m_textNode{ device, renderPass, true, OpacityType::eAlphaTest, TextureFlag::eOpacity }
		, m_panelBuffer{ renderer::makeVertexBuffer< Overlay::Quad >( device
			, 0u
			, uint32_t( 1u * sizeof( Overlay::Quad ) )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_borderBuffer{ renderer::makeVertexBuffer< Overlay::Quad >( device
			, 0u
			, uint32_t( 8u * sizeof( Overlay::Quad ) )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_maxCharsPerBuffer{ maxCharsPerBuffer }
		, m_viewport{ { 100, 100 } }
	{
	}

	OverlayRenderer::~OverlayRenderer()
	{
	}

	void OverlayRenderer::registerOverlay( PanelOverlay const & overlay )
	{
		auto it = m_panelOverlays.find( &overlay );

		if ( it == m_panelOverlays.end() )
		{
			auto & material = overlay.material();
			auto & node = m_panelNodes[size_t( UberShader::nodeType
				( material.opacityType()
				, material.textureFlags() ) )];
			m_panelOverlays.emplace( &overlay
				, doCreateMaterialDescriptor( node.m_materialDescriptorPool, material ) );
		}
	}

	void OverlayRenderer::registerOverlay( BorderPanelOverlay const & overlay )
	{
		auto it = m_panelOverlays.find( &overlay );

		if ( it == m_panelOverlays.end() )
		{
			auto & material = overlay.material();
			auto & node = m_panelNodes[size_t( UberShader::nodeType
				( material.opacityType()
				, material.textureFlags() ) )];
			m_panelOverlays.emplace( &overlay
				, doCreateMaterialDescriptor( node.m_materialDescriptorPool
					, material ) );
		}

		it = m_borderOverlays.find( &overlay );

		if ( it == m_borderOverlays.end() )
		{
			auto & material = overlay.borderMaterial();
			auto & node = m_panelNodes[size_t( UberShader::nodeType
				( material.opacityType()
				, material.textureFlags() ) )];
			m_borderOverlays.emplace( &overlay
				, doCreateMaterialDescriptor( node.m_materialDescriptorPool
					, material ) );
		}
	}

	void OverlayRenderer::registerOverlay( TextOverlay const & overlay )
	{
		auto it = m_textOverlays.find( &overlay );

		if ( it == m_textOverlays.end() )
		{
			auto & material = overlay.material();
			auto & node = m_panelNodes[size_t( UberShader::nodeType
				( material.opacityType()
				, material.textureFlags() ) )];
			m_textOverlays.emplace( &overlay
				, doCreateMaterialDescriptor( node.m_materialDescriptorPool
					, material
					, overlay.fontTexture().texture() ) );
		}
	}

	void OverlayRenderer::unregisterOverlay( PanelOverlay const & overlay )
	{
		auto it = m_panelOverlays.find( &overlay );

		if ( it != m_panelOverlays.end() )
		{
			m_panelOverlays.erase( it );
		}
	}

	void OverlayRenderer::unregisterOverlay( BorderPanelOverlay const & overlay )
	{
		auto it = m_panelOverlays.find( &overlay );

		if ( it != m_panelOverlays.end() )
		{
			m_panelOverlays.erase( it );
		}

		it = m_borderOverlays.find( &overlay );

		if ( it != m_borderOverlays.end() )
		{
			m_borderOverlays.erase( it );
		}
	}

	void OverlayRenderer::unregisterOverlay( TextOverlay const & overlay )
	{
		auto it = m_textOverlays.find( &overlay );

		if ( it != m_textOverlays.end() )
		{
			m_textOverlays.erase( it );
		}
	}

	void OverlayRenderer::draw( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, OverlayList const & overlays )
	{
		auto & size = m_viewport.size();
		auto sorted = doSortPerZIndex( overlays, size );

		for ( auto & overlay : sorted )
		{
			overlay->render( stagingBuffer
				, commandBuffer
				, *this );
		}
	}

	void OverlayRenderer::beginRender( utils::IVec2 const & size )
	{
		if ( m_viewport.size() != size )
		{
			m_sizeChanged = true;
			m_viewport.resize( size );
			m_viewport.ortho( 0.0f
				, float( size.x )
				, 0.0f
				, float( size.y )
				, 0.0f
				, 1000.0f );
			m_transform = m_viewport.transform();
		}
	}

	void OverlayRenderer::endRender()
	{
		m_sizeChanged = false;
	}

	void OverlayRenderer::drawPanel( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, PanelOverlay const & overlay )
	{
		auto & material = overlay.material();
		auto & node = m_panelNodes[size_t( UberShader::nodeType
			( material.opacityType()
			, material.textureFlags() ) )];
		stagingBuffer.copyVertexData( commandBuffer
			, overlay.panelVertex()
			, *m_panelBuffer
			, renderer::PipelineStageFlag::eVertexInput );
		commandBuffer.bindPipeline( *node.m_pipeline );
		doDrawBuffer( stagingBuffer
			, commandBuffer
			, *m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, node
			, m_panelOverlays.find( &overlay )->second );
	}

	void OverlayRenderer::drawBorderPanel( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, BorderPanelOverlay const & overlay )
	{
		auto & material = overlay.material();
		auto & node = m_panelNodes[size_t( UberShader::nodeType
			( material.opacityType()
			, material.textureFlags() ) )];
		stagingBuffer.copyVertexData( commandBuffer
			, overlay.panelVertex()
			, *m_panelBuffer
			, renderer::PipelineStageFlag::eVertexInput );
		stagingBuffer.copyVertexData( commandBuffer
			, overlay.borderVertex()
			, *m_borderBuffer
			, renderer::PipelineStageFlag::eVertexInput );
		commandBuffer.bindPipeline( *node.m_pipeline );
		doDrawBuffer( stagingBuffer
			, commandBuffer
			, *m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, node
			, m_panelOverlays.find( &overlay )->second );
		doDrawBuffer( stagingBuffer
			, commandBuffer
			, *m_borderBuffer
			, 8u
			, overlay.transform()
			, overlay.borderMaterial()
			, node
			, m_borderOverlays.find( &overlay )->second );
	}

	void OverlayRenderer::drawText( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, TextOverlay const & overlay )
	{
		uint32_t offset{ 0u };
		auto & quads = overlay.textVertex();
		auto count = uint32_t( quads.size() );
		auto it = quads.cbegin();
		uint32_t index{ 0u };
		std::vector< renderer::VertexBuffer< Overlay::Quad > const * > buffers;

		while ( count > m_maxCharsPerBuffer )
		{
			buffers.push_back( &doFillTextPart( stagingBuffer
				, commandBuffer
				, count
				, offset
				, it
				, index ) );
			count -= m_maxCharsPerBuffer;
		}

		if ( count > 0 )
		{
			buffers.push_back( &doFillTextPart( stagingBuffer
				, commandBuffer
				, count
				, offset
				, it
				, index ) );
		}

		count = uint32_t( quads.size() );

		for ( auto & buffer : buffers )
		{
			doDrawBuffer( stagingBuffer
				, commandBuffer
				, *buffer
				, std::min( count, m_maxCharsPerBuffer )
				, overlay.transform()
				, overlay.material()
				, m_textNode
				, m_textOverlays.find( &overlay )->second );
			count -= m_maxCharsPerBuffer;
		}
	}

	renderer::VertexBuffer< Overlay::Quad > const & OverlayRenderer::doCreateTextBuffer( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer )
	{
		auto buffer = renderer::makeVertexBuffer< TextOverlay::Quad >( m_device
			, 0u
			, m_maxCharsPerBuffer
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
		m_textBuffers.push_back( std::move( buffer ) );
		return *m_textBuffers.back();
	}

	void OverlayRenderer::doDrawBuffer( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, renderer::VertexBuffer< Overlay::Quad > const & buffer
		, uint32_t count
		, utils::Mat4 const & transform
		, Material const & material
		, OverlayNode & node
		, renderer::DescriptorSet const & descriptor )
	{
		node.m_overlayUbo.getData().colour = utils::Vec4{ material.ambient(), material.opacity() };
		node.m_overlayUbo.getData().modelProj = m_transform * transform;
		stagingBuffer.copyUniformData( commandBuffer
			, node.m_overlayUbo.getDatas()
			, node.m_overlayUbo
			, renderer::PipelineStageFlag::eVertexShader | renderer::PipelineStageFlag::eFragmentShader );
		commandBuffer.bindVertexBuffer( buffer
			, 0u );
		commandBuffer.bindDescriptorSet( descriptor
			, node.m_pipelineLayout );
		commandBuffer.draw( count * 6u
			, 1u
			, 0u
			, 0u );
	}

	renderer::VertexBuffer< Overlay::Quad > const & OverlayRenderer::doFillTextPart( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, uint32_t count
		, uint32_t & offset
		, TextOverlay::QuadArray::const_iterator & it
		, uint32_t & index )
	{
		renderer::VertexBuffer< Overlay::Quad > const * buffer{ nullptr };

		if ( offset + count > m_maxCharsPerBuffer )
		{
			buffer = &doCreateTextBuffer( stagingBuffer, commandBuffer );
			offset = 0u;
			++index;
		}
		else
		{
			buffer = m_textBuffers[index].get();
		}

		count = std::min( count, m_maxCharsPerBuffer );
		doFillBuffers( stagingBuffer
			, commandBuffer
			, it
			, count
			, offset
			, *buffer );
		it += count;

		return *buffer;
	}
}
