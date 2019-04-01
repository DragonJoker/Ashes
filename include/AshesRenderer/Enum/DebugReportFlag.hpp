/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DebugReportFlag_HPP___
#define ___Ashes_DebugReportFlag_HPP___
#pragma once

namespace ashes
{
	enum class DebugReportFlag
		: uint32_t
	{
		eInformation = 0x00000001,
		eWarning = 0x00000002,
		ePerformanceWarning = 0x00000004,
		eError = 0x00000008,
		eDebug = 0x00000010,
	};
	Ashes_ImplementFlag( DebugReportFlag )
}

#endif
