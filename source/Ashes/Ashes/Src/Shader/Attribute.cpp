/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Shader/Attribute.hpp"

namespace ashes
{
	Attribute::Attribute( VertexLayout const & layout
		, Format format
		, uint32_t location
		, uint32_t offset
		, std::string const & semanticName
		, uint32_t semanticIndex )
		: layout{ layout }
		, format{ format }
		, location{ location }
		, offset{ offset }
		, semanticName{ semanticName }
		, semanticIndex{ semanticIndex }
	{
	}
}
