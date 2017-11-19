#include "PanelOverlay.h"
#include "OverlayRenderer.h"

namespace render
{
	PanelOverlay::PanelOverlay()
		: Overlay( Overlay::Type::ePanel )
	{
		m_panelQuads.resize( 1 );
	}

	PanelOverlay::~PanelOverlay()
	{
	}

	void PanelOverlay::render( OverlayRenderer & renderer )const
	{
		renderer.drawPanel( *this );
	}

	void PanelOverlay::doUpdate()
	{
		if ( m_sizeChanged )
		{
			doUpdateBuffer();
			m_sizeChanged = false;
		}
	}

	void PanelOverlay::doUpdateBuffer()
	{
		int32_t centerL = 0;
		int32_t centerT = 0;
		int32_t centerR = m_size.x;
		int32_t centerB = m_size.y;

		Overlay::Vertex vertex0 = { { centerL, centerT }, { m_uv.x, m_uv.w } };
		Overlay::Vertex vertex1 = { { centerL, centerB }, { m_uv.x, m_uv.y } };
		Overlay::Vertex vertex2 = { { centerR, centerB }, { m_uv.z, m_uv.y } };
		Overlay::Vertex vertex3 = { { centerL, centerT }, { m_uv.x, m_uv.w } };
		Overlay::Vertex vertex4 = { { centerR, centerB }, { m_uv.z, m_uv.y } };
		Overlay::Vertex vertex5 = { { centerR, centerT }, { m_uv.z, m_uv.w } };

		auto & quad = m_panelQuads[0];
		quad[0] = vertex0;
		quad[1] = vertex1;
		quad[2] = vertex2;
		quad[3] = vertex3;
		quad[4] = vertex4;
		quad[5] = vertex5;
	}
}
