#include "OverlayCategory.h"

#include "BorderPanelOverlay.h"
#include "OverlayRenderer.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"

#include <GlLib/Transform.h>

namespace render
{
	Overlay::Overlay( Type type )
		: m_type( type )
	{
	}

	Overlay::~Overlay()
	{
	}

	void Overlay::update()
	{
		if ( m_positionChanged )
		{
			doUpdatePosition();
		}

		doUpdate();
	}

	void Overlay::doUpdatePosition()
	{
		if ( m_positionChanged )
		{
			m_transform = gl::Mat4{ 1 };
			m_transform = gl::translate( m_transform
				, gl::Vec3{ m_position.x, m_position.y, 0 } );
			m_positionChanged = false;
		}
	}
}
