#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlPrimitiveTopology value )
	{
		switch ( value )
		{
		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_POINT_LIST:
			return "GL_POINTS";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_LINE_LIST:
			return "GL_LINES";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_LINE_LOOP:
			return "GL_LINE_LOOP";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_LINE_STRIP:
			return "GL_LINE_STRIP";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
			return "GL_TRIANGLES";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
			return "GL_TRIANGLE_STRIP";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
			return "GL_TRIANGLE_FAN";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_LINE_LIST_ADJACENCY:
			return "GL_LINES_ADJACENCY";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJACENCY:
			return "GL_LINE_STRIP_ADJACENCY";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJACENCY:
			return "GL_TRIANGLES_ADJACENCY";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_ADJACENCY:
			return " GL_TRIANGLE_STRIP_ADJACENCY";

		case gl_renderer::GL_PRIMITIVE_TOPOLOGY_PATCH_LIST:
			return "GL_PATCHES";

		default:
			assert( false && "Unsupported GlPrimitiveTopology" );
			return "GlPrimitiveTopology_UNKNOWN";
		}
	}

	GlPrimitiveTopology convert( ashes::PrimitiveTopology const & topology )
	{
		switch ( topology )
		{
		case ashes::PrimitiveTopology::ePointList:
			return GL_PRIMITIVE_TOPOLOGY_POINT_LIST;

		case ashes::PrimitiveTopology::eLineList:
			return GL_PRIMITIVE_TOPOLOGY_LINE_LIST;

		case ashes::PrimitiveTopology::eLineStrip:
			return GL_PRIMITIVE_TOPOLOGY_LINE_STRIP;

		case ashes::PrimitiveTopology::eTriangleList:
			return GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		case ashes::PrimitiveTopology::eTriangleStrip:
			return GL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

		case ashes::PrimitiveTopology::eTriangleFan:
			return GL_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

		case ashes::PrimitiveTopology::eLineListWithAdjacency:
			return GL_PRIMITIVE_TOPOLOGY_LINE_LIST_ADJACENCY;

		case ashes::PrimitiveTopology::eLineStripWithAdjacency:
			return GL_PRIMITIVE_TOPOLOGY_LINE_STRIP_ADJACENCY;

		case ashes::PrimitiveTopology::eTriangleListWithAdjacency:
			return GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_ADJACENCY;

		case ashes::PrimitiveTopology::eTriangleStripWithAdjacency:
			return GL_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_ADJACENCY;

		case ashes::PrimitiveTopology::ePatchList:
			return GL_PRIMITIVE_TOPOLOGY_PATCH_LIST;

		default:
			assert( false );
			return GL_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}
}
