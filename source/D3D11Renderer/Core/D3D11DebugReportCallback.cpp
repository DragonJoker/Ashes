/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "D3D11Renderer/Core/D3D11DebugReportCallback.hpp"

#include "D3D11Renderer/Core/D3D11Instance.hpp"

namespace d3d11_renderer
{
	DebugReportCallback::DebugReportCallback( Instance const & instance
		, ashes::DebugReportCallbackCreateInfo createInfo )
		: ashes::DebugReportCallback{ instance, std::move( createInfo ) }
	{
	}

	DebugReportCallback::~DebugReportCallback()
	{
	}
}
