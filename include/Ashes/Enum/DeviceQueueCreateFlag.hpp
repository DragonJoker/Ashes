/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceQueueCreateFlag_HPP___
#define ___Ashes_DeviceQueueCreateFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des indicateurs de création de file.
	*/
	enum class DeviceQueueCreateFlag
		: int32_t
	{
		eNone = 0x00000000,
		eProtected = 0x00000001,
	};
	Ashes_ImplementFlag( DeviceQueueCreateFlag )
}

#endif
