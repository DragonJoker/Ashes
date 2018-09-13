/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_QueueFlag_HPP___
#define ___Ashes_QueueFlag_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des types de file.
	*/
	enum class QueueFlag
		: uint32_t
	{
		eGraphics = 0x00000001,
		eCompute = 0x00000002,
		eTransfer = 0x00000004,
		eSparseBinding = 0x00000008,
	};
	Renderer_ImplementFlag( QueueFlag )
}

#endif
