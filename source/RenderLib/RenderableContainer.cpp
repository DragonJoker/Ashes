#include "RenderableContainer.h"

#include "Billboard.h"
#include "Object.h"
#include "PolyLine.h"
#include "Submesh.h"

#include <Renderer/DescriptorSetLayoutBinding.hpp>

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

		renderer::DescriptorSetLayout doCreateMaterialDescriptorLayout( renderer::Device const & device
			, NodeType node )
		{
			uint32_t index = 0u;
			std::vector< renderer::DescriptorSetLayoutBinding > bindings;

			bindings.emplace_back( index++
				, renderer::DescriptorType::eUniformBuffer
				, renderer::ShaderStageFlag::eFragment );

			if ( node == NodeType::eOpaqueDiff
				|| node == NodeType::eAlphaBlendDiff
				|| node == NodeType::eAlphaBlendOpaDiff
				|| node == NodeType::eAlphaTestDiff
				|| node == NodeType::eAlphaTestOpaDiff )
			{
				// Diffuse texture.
				bindings.emplace_back( index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment );
			}

			if ( node == NodeType::eAlphaBlendOpa
				|| node == NodeType::eAlphaBlendOpaDiff
				|| node == NodeType::eAlphaTestOpa
				|| node == NodeType::eAlphaTestOpaDiff )
			{
				// Opacity texture
				bindings.emplace_back( index++
					, renderer::DescriptorType::eSampledImage
					, renderer::ShaderStageFlag::eFragment );
			}

			return renderer::DescriptorSetLayout{ device, bindings };
		}
	}

	RenderableContainer::RenderableContainer( renderer::Device const & device )
		: m_renderer{ device }
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
		m_descriptorLayouts.clear();
	}

	void RenderableContainer::doDraw( renderer::RenderingResources const & resources
		, Camera const & camera
		, float zoomScale )const
	{
		m_renderer.draw( resources
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
			auto node = UberShader::nodeType( material->opacityType()
				, material->textureFlags() );
			m_renderObjects[size_t( node )].emplace_back( doFindDescriptorLayout( node ).pool
				, object->mesh()
				, *mshit
				, material
				, object );
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
		auto node = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		m_renderBillboards[size_t( node )].emplace_back( doFindDescriptorLayout( node ).pool
			, billboard );
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
		auto node = UberShader::nodeType( material.opacityType()
			, material.textureFlags() );
		m_renderLines[size_t( node )].emplace_back( doFindDescriptorLayout( node ).pool
			, lines );
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

	RenderableContainer::DescriptorLayoutPool const & RenderableContainer::doFindDescriptorLayout( NodeType node )
	{
		auto it = m_descriptorLayouts.find( size_t( node ) );

		if ( it == m_descriptorLayouts.end() )
		{
			it = m_descriptorLayouts.emplace( size_t( node )
				, DescriptorLayoutPool{ doCreateMaterialDescriptorLayout( m_device, node ) } ).first;
		}

		return it->second;
	}
}
