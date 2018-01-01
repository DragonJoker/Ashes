#include "GlPrimitiveTopology.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::PrimitiveTopology const & topology )
	{
		switch ( topology )
		{
		case renderer::PrimitiveTopology::ePointList:
			return GL_POINTS;

		case renderer::PrimitiveTopology::eLineList:
			return GL_LINES;

		case renderer::PrimitiveTopology::eLineStrip:
			return GL_LINE_STRIP;

		case renderer::PrimitiveTopology::eTriangleList:
			return GL_TRIANGLES;

		case renderer::PrimitiveTopology::eTriangleStrip:
			return GL_TRIANGLE_STRIP;

		case renderer::PrimitiveTopology::eTriangleFan:
			return GL_TRIANGLE_FAN;

		case renderer::PrimitiveTopology::eLineListWithAdjacency:
			return GL_LINES_ADJACENCY;

		case renderer::PrimitiveTopology::eLineStripWithAdjacency:
			return GL_LINE_STRIP_ADJACENCY;

		case renderer::PrimitiveTopology::eTriangleListWithAdjacency:
			return GL_TRIANGLES_ADJACENCY;

		case renderer::PrimitiveTopology::eTriangleStripWithAdjacency:
			return GL_TRIANGLE_STRIP_ADJACENCY;

		case renderer::PrimitiveTopology::ePatchList:
			return GL_PATCHES;

		default:
			assert( false );
			return GL_TRIANGLES;
		}
	}
}
