/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Core/DebugReportCallback.hpp"

namespace ashes
{
	DebugReportCallback::DebugReportCallback( Instance const & instance
		, DebugReportCallbackCreateInfo createInfo )
		: m_instance{ instance }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	DebugReportCallback::~DebugReportCallback()
	{
	}
}
