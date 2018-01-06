#include "PanelOverlay.h"
#include "OverlayRenderer.h"

namespace render
{
	PanelOverlay::PanelOverlay()
		: Overlay( Overlay::Type::ePanel )
	{
	}

	PanelOverlay::~PanelOverlay()
	{
	}

	void PanelOverlay::render( renderer::CommandBuffer const & commandBuffer
		, OverlayRenderer const & renderer )const
	{
		renderer.drawPanel( commandBuffer
			, *this );
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

		m_quad[0] = vertex0;
		m_quad[1] = vertex1;
		m_quad[2] = vertex2;
		m_quad[3] = vertex3;
		m_quad[4] = vertex4;
		m_quad[5] = vertex5;
	}
}
