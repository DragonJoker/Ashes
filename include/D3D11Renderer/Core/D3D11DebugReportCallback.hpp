/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/DebugReportCallback.hpp>

namespace d3d11_renderer
{
	class DebugReportCallback
		: public ashes::DebugReportCallback
	{
	public:
		DebugReportCallback( Instance const & instance
			, ashes::DebugReportCallbackCreateInfo createInfo );
		~DebugReportCallback();
	};
}
