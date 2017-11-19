#include "BillboardBuffer.h"

#include "Camera.h"

namespace render
{
	namespace
	{
		//*********************************************************************

		class StorageES2
			: public BillboardBuffer::Storage
		{
		public:
			StorageES2( uint32_t size )
				: m_data( size )
			{
				m_vbo = gl::makeBuffer< BillboardBuffer::Quad >
					( gl::BufferTarget::eArrayBuffer, m_data );
			}

			BillboardBuffer::Quad * lock()override
			{
				return m_data.data();
			}

			void unlock()override
			{
				m_vbo->bind();
				m_vbo->upload( 0u, m_vbo->count(), m_data.data() );
				m_vbo->unbind();
			}

		private:
			//! Les données du stockage.
			std::vector< BillboardBuffer::Quad > m_data;
		};

		//*********************************************************************

		class StorageES3
			: public BillboardBuffer::Storage
		{
		public:
			StorageES3( uint32_t size )
			{
				m_vbo = gl::makeBuffer< BillboardBuffer::Quad >
					( gl::BufferTarget::eArrayBuffer );
				m_vbo->bind();
				m_vbo->resize( size );
				m_vbo->unbind();
			}

			BillboardBuffer::Quad * lock()override
			{
				m_vbo->bind();
				return m_vbo->lock( 0u, m_vbo->count(), GL_MAP_WRITE_BIT );
			}

			void unlock()override
			{
				m_vbo->unlock();
				m_vbo->unbind();
			}
		};

		//*********************************************************************
	}

	BillboardBuffer::BillboardBuffer( bool scale )
		: m_scale{ scale }
	{
	}

	void BillboardBuffer::initialise()
	{
		if ( gl::OpenGL::checkSupport( gl::FeatureLevel::eGLES3 ) )
		{
			m_visible = std::make_unique< StorageES3 >( uint32_t( m_buffer.size() ) );
		}
		else
		{
			m_visible = std::make_unique< StorageES2 >( uint32_t( m_buffer.size() ) );
		}
	}

	void BillboardBuffer::cleanup()
	{
		m_visible.reset();
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

	void BillboardBuffer::cull( Camera const & camera
		, gl::Vec3 const & position
		, float scale )
	{
		auto buffer = m_visible->lock();

		if ( buffer )
		{
			auto count = 0u;
			std::for_each( std::begin( m_buffer )
				, std::begin( m_buffer ) + m_unculled
				, [&count
					, &camera
					, &position
					, &scale
					, &buffer
					, this]( Quad const & quad )
			{
				if ( m_scale )
				{
					if ( camera.visible( quad[0].data.center + position ) )
					{
						auto & visible = buffer[count];
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
						auto & visible = buffer[count];
						visible = quad;
					}

					count++;
				}
			} );
			m_count = count;
		}

		m_visible->unlock();
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

	//*************************************************************************
}
