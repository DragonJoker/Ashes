#include "BorderPanelOverlay.h"

#include "Material.h"
#include "OverlayRenderer.h"

namespace render
{
	BorderPanelOverlay::BorderPanelOverlay()
		: Overlay( Overlay::Type::eBorderPanel )
	{
		m_panelQuads.resize( 1 );
		m_borderQuads.resize( 8 );
	}

	BorderPanelOverlay::~BorderPanelOverlay()
	{
	}

	void BorderPanelOverlay::render( OverlayRenderer & renderer )const
	{
		renderer.drawBorderPanel( *this );
	}

	void BorderPanelOverlay::doUpdate()
	{
		if ( m_sizeChanged )
		{
			doUpdateBuffer();
			m_sizeChanged = false;
		}
	}

	void BorderPanelOverlay::doUpdateBuffer()
	{
		int32_t centerL = 0;
		int32_t centerT = 0;
		int32_t centerR = m_size.x;
		int32_t centerB = m_size.y;

		if ( m_borderPosition == BorderPosition::eInternal )
		{
			centerL += m_borderSize.x;
			centerT += m_borderSize.y;
			centerR -= m_borderSize.z;
			centerB -= m_borderSize.w;
		}
		else if ( m_borderPosition == BorderPosition::eMiddle )
		{
			centerL += m_borderSize.x / 2;
			centerT += m_borderSize.y / 2;
			centerR -= m_borderSize.z / 2;
			centerB -= m_borderSize.w / 2;
		}

		int32_t borderL = centerL - m_borderSize.x;
		int32_t borderT = centerT - m_borderSize.y;
		int32_t borderR = centerR + m_borderSize.z;
		int32_t borderB = centerB + m_borderSize.w;

		auto borderUvLL = m_borderOuterUv.x;
		auto borderUvTT = m_borderOuterUv.y;
		auto borderUvML = m_borderInnerUv.x;
		auto borderUvMT = m_borderInnerUv.y;
		auto borderUvMR = m_borderInnerUv.z;
		auto borderUvMB = m_borderInnerUv.w;
		auto borderUvRR = m_borderOuterUv.z;
		auto borderUvBB = m_borderOuterUv.w;
		{
			// Center
			Overlay::Vertex vertex0 = { { centerL, centerT }, { m_uv[0], m_uv[3] } };
			Overlay::Vertex vertex1 = { { centerL, centerB }, { m_uv[0], m_uv[1] } };
			Overlay::Vertex vertex2 = { { centerR, centerB }, { m_uv[2], m_uv[1] } };
			Overlay::Vertex vertex3 = { { centerL, centerT }, { m_uv[0], m_uv[3] } };
			Overlay::Vertex vertex4 = { { centerR, centerB }, { m_uv[2], m_uv[1] } };
			Overlay::Vertex vertex5 = { { centerR, centerT }, { m_uv[2], m_uv[3] } };
			auto & quad = m_panelQuads[0];
			quad[0] = vertex0;
			quad[1] = vertex1;
			quad[2] = vertex2;
			quad[3] = vertex3;
			quad[4] = vertex4;
			quad[5] = vertex5;
		}
		uint32_t index = 0;
		{
			// Corner Top Left
			Overlay::Vertex cornerTL0 = { { borderL, borderT }, { borderUvLL, borderUvTT } };
			Overlay::Vertex cornerTL1 = { { borderL, centerT }, { borderUvLL, borderUvMT } };
			Overlay::Vertex cornerTL2 = { { centerL, centerT }, { borderUvML, borderUvMT } };
			Overlay::Vertex cornerTL3 = { { borderL, borderT }, { borderUvLL, borderUvTT } };
			Overlay::Vertex cornerTL4 = { { centerL, centerT }, { borderUvML, borderUvMT } };
			Overlay::Vertex cornerTL5 = { { centerL, borderT }, { borderUvML, borderUvTT } };
			auto & quad = m_borderQuads[index++];
			quad[0] = cornerTL0;
			quad[1] = cornerTL1;
			quad[2] = cornerTL2;
			quad[3] = cornerTL3;
			quad[4] = cornerTL4;
			quad[5] = cornerTL5;
		}
		{
			// Border Top
			Overlay::Vertex borderT0 = { { centerL, borderT }, { borderUvML, borderUvTT } };
			Overlay::Vertex borderT1 = { { centerL, centerT }, { borderUvML, borderUvMT } };
			Overlay::Vertex borderT2 = { { centerR, centerT }, { borderUvMR, borderUvMT } };
			Overlay::Vertex borderT3 = { { centerL, borderT }, { borderUvML, borderUvTT } };
			Overlay::Vertex borderT4 = { { centerR, centerT }, { borderUvMR, borderUvMT } };
			Overlay::Vertex borderT5 = { { centerR, borderT }, { borderUvMR, borderUvTT } };
			auto & quad = m_borderQuads[index++];
			quad[0] = borderT0;
			quad[1] = borderT1;
			quad[2] = borderT2;
			quad[3] = borderT3;
			quad[4] = borderT4;
			quad[5] = borderT5;
		}
		{
			// Corner Top Right
			Overlay::Vertex cornerTR0 = { { centerR, borderT }, { borderUvMR, borderUvTT } };
			Overlay::Vertex cornerTR1 = { { centerR, centerT }, { borderUvMR, borderUvMT } };
			Overlay::Vertex cornerTR2 = { { borderR, centerT }, { borderUvRR, borderUvMT } };
			Overlay::Vertex cornerTR3 = { { centerR, borderT }, { borderUvMR, borderUvTT } };
			Overlay::Vertex cornerTR4 = { { borderR, centerT }, { borderUvRR, borderUvMT } };
			Overlay::Vertex cornerTR5 = { { borderR, borderT }, { borderUvRR, borderUvTT } };
			auto & quad = m_borderQuads[index++];
			quad[0] = cornerTR0;
			quad[1] = cornerTR1;
			quad[2] = cornerTR2;
			quad[3] = cornerTR3;
			quad[4] = cornerTR4;
			quad[5] = cornerTR5;
		}
		{
			// Border Left
			Overlay::Vertex borderL0 = { { borderL, centerT }, { borderUvLL, borderUvMT } };
			Overlay::Vertex borderL1 = { { borderL, centerB }, { borderUvLL, borderUvMB } };
			Overlay::Vertex borderL2 = { { centerL, centerB }, { borderUvML, borderUvMB } };
			Overlay::Vertex borderL3 = { { borderL, centerT }, { borderUvLL, borderUvMT } };
			Overlay::Vertex borderL4 = { { centerL, centerB }, { borderUvML, borderUvMB } };
			Overlay::Vertex borderL5 = { { centerL, centerT }, { borderUvML, borderUvMT } };
			auto & quad = m_borderQuads[index++];
			quad[0] = borderL0;
			quad[1] = borderL1;
			quad[2] = borderL2;
			quad[3] = borderL3;
			quad[4] = borderL4;
			quad[5] = borderL5;
		}
		{
			// Border Right
			Overlay::Vertex borderR0 = { { centerR, centerT }, { borderUvMR, borderUvMT } };
			Overlay::Vertex borderR1 = { { centerR, centerB }, { borderUvMR, borderUvMB } };
			Overlay::Vertex borderR2 = { { borderR, centerB }, { borderUvRR, borderUvMB } };
			Overlay::Vertex borderR3 = { { centerR, centerT }, { borderUvMR, borderUvMT } };
			Overlay::Vertex borderR4 = { { borderR, centerB }, { borderUvRR, borderUvMB } };
			Overlay::Vertex borderR5 = { { borderR, centerT }, { borderUvRR, borderUvMT } };
			auto & quad = m_borderQuads[index++];
			quad[0] = borderR0;
			quad[1] = borderR1;
			quad[2] = borderR2;
			quad[3] = borderR3;
			quad[4] = borderR4;
			quad[5] = borderR5;
		}
		{
			// Corner Bottom Left
			Overlay::Vertex cornerBL0 = { { borderL, centerB }, { borderUvLL, borderUvMB } };
			Overlay::Vertex cornerBL1 = { { borderL, borderB }, { borderUvLL, borderUvBB } };
			Overlay::Vertex cornerBL2 = { { centerL, borderB }, { borderUvML, borderUvBB } };
			Overlay::Vertex cornerBL3 = { { borderL, centerB }, { borderUvLL, borderUvMB } };
			Overlay::Vertex cornerBL4 = { { centerL, borderB }, { borderUvML, borderUvBB } };
			Overlay::Vertex cornerBL5 = { { centerL, centerB }, { borderUvML, borderUvMB } };
			auto & quad = m_borderQuads[index++];
			quad[0] = cornerBL0;
			quad[1] = cornerBL1;
			quad[2] = cornerBL2;
			quad[3] = cornerBL3;
			quad[4] = cornerBL4;
			quad[5] = cornerBL5;
		}
		{
			// Border Bottom
			Overlay::Vertex borderB0 = { { centerL, centerB }, { borderUvML, borderUvMB } };
			Overlay::Vertex borderB1 = { { centerL, borderB }, { borderUvML, borderUvBB } };
			Overlay::Vertex borderB2 = { { centerR, borderB }, { borderUvMR, borderUvBB } };
			Overlay::Vertex borderB3 = { { centerL, centerB }, { borderUvML, borderUvMB } };
			Overlay::Vertex borderB4 = { { centerR, borderB }, { borderUvMR, borderUvBB } };
			Overlay::Vertex borderB5 = { { centerR, centerB }, { borderUvMR, borderUvMB } };
			auto & quad = m_borderQuads[index++];
			quad[0] = borderB0;
			quad[1] = borderB1;
			quad[2] = borderB2;
			quad[3] = borderB3;
			quad[4] = borderB4;
			quad[5] = borderB5;
		}
		{
			// Corner Bottom Right
			Overlay::Vertex cornerBR0 = { { centerR, centerB }, { borderUvMR, borderUvMB } };
			Overlay::Vertex cornerBR1 = { { centerR, borderB }, { borderUvMR, borderUvBB } };
			Overlay::Vertex cornerBR2 = { { borderR, borderB }, { borderUvRR, borderUvBB } };
			Overlay::Vertex cornerBR3 = { { centerR, centerB }, { borderUvMR, borderUvMB } };
			Overlay::Vertex cornerBR4 = { { borderR, borderB }, { borderUvRR, borderUvBB } };
			Overlay::Vertex cornerBR5 = { { borderR, centerB }, { borderUvRR, borderUvMB } };
			auto & quad = m_borderQuads[index++];
			quad[0] = cornerBR0;
			quad[1] = cornerBR1;
			quad[2] = cornerBR2;
			quad[3] = cornerBR3;
			quad[4] = cornerBR4;
			quad[5] = cornerBR5;
		}
	}
}
