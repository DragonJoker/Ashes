/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "RasterisationState.hpp"

namespace renderer
{
	RasterisationState::RasterisationState( RasterisationStateFlags flags
		, bool depthClampEnable
		, bool rasterizerDiscardEnable
		, PolygonMode polygonMode
		, CullModeFlags cullMode
		, FrontFace frontFace
		, bool depthBiasEnable
		, float depthBiasConstantFactor
		, float depthBiasClamp
		, float depthBiasSlopeFactor
		, float lineWidth )
		: m_state
		{
			flags,
			depthClampEnable,
			rasterizerDiscardEnable,
			convert( polygonMode ),
			convert( cullMode ),
			convert( frontFace ),
			depthBiasEnable,
			depthBiasConstantFactor,
			depthBiasClamp,
			depthBiasSlopeFactor,
			lineWidth
		}
	{
	}
}
