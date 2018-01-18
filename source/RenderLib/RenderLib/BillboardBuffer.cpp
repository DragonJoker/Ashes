#include "BillboardBuffer.h"

#include "Camera.h"

#include <Renderer/Buffer/StagingBuffer.hpp>
#include <Renderer/Buffer/VertexBuffer.hpp>
#include <Renderer/Pipeline/VertexLayout.hpp>

namespace render
{
	BillboardBuffer::BillboardBuffer( renderer::Device const & device
		, bool scale )
		: m_device{ device }
		, m_scale{ scale }
	{
	}

	void BillboardBuffer::initialise()
	{
		m_vbo = renderer::makeVertexBuffer< BillboardBuffer::Quad >( m_device
			, uint32_t( m_buffer.size() )
			, renderer::BufferTarget::eTransferDst
			, renderer::MemoryPropertyFlag::eDeviceLocal );
	}

	void BillboardBuffer::cleanup()
	{
		m_vbo.reset();
	}

	void BillboardBuffer::update( float threshold )
	{
		auto it = std::find_if( std::begin( m_buffer )
			, std::end( m_buffer )
			, [&threshold]( auto const & element )
			{
				return element[0].data.magnitude > threshold;
			} );

		if ( it != std::end( m_buffer ) )
		{
			m_unculled = uint32_t( std::distance( std::begin( m_buffer )
				, it ) );
		}
		else
		{
			m_unculled = uint32_t( m_buffer.size() );
		}
	}

	void BillboardBuffer::cull( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, Camera const & camera
		, renderer::Vec3 const & position
		, float scale )
	{
		assert( m_vbo && "Billboard VBO is not initialised." );
		m_visible.resize( m_buffer.size() );
		auto count = 0u;
		std::for_each( std::begin( m_buffer )
			, std::begin( m_buffer ) + m_unculled
			, [&count
				, &camera
				, &position
				, &scale
				, this]( Quad const & quad )
			{
				if ( m_scale )
				{
					if ( camera.visible( quad[0].data.center + position ) )
					{
						auto & visible = m_visible[count];
						visible = quad;

						visible[0].data.scale *= scale;
						visible[1].data.scale *= scale;
						visible[2].data.scale *= scale;
						visible[3].data.scale *= scale;
						visible[4].data.scale *= scale;
						visible[5].data.scale *= scale;

						count++;
					}
				}
				else
				{
					if ( camera.visible( quad[0].data.center + position ) )
					{
						auto & visible = m_visible[count];
						visible = quad;
					}

					count++;
				}
			} );

		stagingBuffer.copyVertexData( commandBuffer
			, m_visible
			, *m_vbo
			, renderer::PipelineStageFlag::eVertexInput );
		m_count = count;
	}

	void BillboardBuffer::remove( uint32_t index )
	{
		assert( index < m_buffer.size() );
		m_buffer.erase( m_buffer.begin() + index );
	}

	void BillboardBuffer::add( BillboardData const & data )
	{
		float fid{ float( m_buffer.size() ) };
		auto quad = Quad
		{
			{
				Vertex{ data,{ -0.5, -0.5 }, fid },
				Vertex{ data,{ +0.5, +0.5 }, fid },
				Vertex{ data,{ +0.5, -0.5 }, fid },
				Vertex{ data,{ -0.5, -0.5 }, fid },
				Vertex{ data,{ -0.5, +0.5 }, fid },
				Vertex{ data,{ +0.5, +0.5 }, fid },
			}
		};
		m_buffer.push_back( quad );
	}

	void BillboardBuffer::add( BillboardDataArray const & datas )
	{
		for ( auto & data : datas )
		{
			add( data );
		}
	}

	BillboardData const & BillboardBuffer::at( uint32_t index )const
	{
		assert( index < m_buffer.size() );
		return m_buffer[index][0].data;
	}

	void BillboardBuffer::at( uint32_t index, BillboardData const & data )
	{
		assert( index < m_buffer.size() );

		for ( auto & in : m_buffer[index] )
		{
			in.data = data;
		}

		onBillboardBufferChanged( *this );
	}
}
