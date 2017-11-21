#include "PolyLine.h"

namespace render
{
	PolyLine::PolyLine( std::string const & name
		, float width
		, float feather )
		: Movable{ name }
		, m_width{ width }
		, m_feather{ feather }
	{
	}

	void PolyLine::remove( uint32_t index )
	{
		assert( index < m_buffer.size() );
		m_buffer.erase( m_buffer.begin() + index );
	}

	void PolyLine::add( Points const & line )
	{
		static constexpr utils::Vec3 down{ 0, 0, -1 };
		utils::Vec3 slope{ renderer::normalize( line.m_b - line.m_a ) };
		auto normal = renderer::cross( slope, down );
		m_buffer.push_back( LineQuad
		{
			{
				Vertex{ line.m_a, normal },
				Vertex{ line.m_a, -normal },
				Vertex{ line.m_b, -normal },
				Vertex{ line.m_a, normal },
				Vertex{ line.m_b, -normal },
				Vertex{ line.m_b, normal },
			}
		});
	}

	void PolyLine::add( LineArray const & lines )
	{
		for ( auto & line : lines )
		{
			add( line );
		}
	}
}
