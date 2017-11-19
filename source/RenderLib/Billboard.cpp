#include "Billboard.h"

#include "BillboardBuffer.h"

namespace render
{
	Billboard::Billboard( std::string const & name, BillboardBuffer & buffer )
		: Movable{ name }
		, m_buffer{ buffer }
	{
	}

	void Billboard::cull( Camera const & camera
		, float scale )
	{
		m_buffer.cull( camera, position(), scale );
	}
}
