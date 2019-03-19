/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "TestRenderer/Core/TestDebugReportCallback.hpp"

#include "TestRenderer/Core/TestInstance.hpp"

namespace test_renderer
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
