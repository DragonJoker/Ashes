/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	enum class SampleCountFlag
		: uint32_t
	{
		e1 = 0x00000001,
		e2 = 0x00000002,
		e4 = 0x00000004,
		e8 = 0x00000008,
		e16 = 0x00000010,
		e32 = 0x00000020,
		e64 = 0x00000040,
	};
	VkLib_ImplementFlag( SampleCountFlag )
	/**
	*\brief
	*	Convertit un renderer::SampleCountFlag en VkSampleCountFlagBits.
	*\param[in] flags
	*	Le renderer::SampleCountFlag.
	*\return
	*	Le VkSampleCountFlagBits.
	*/
	VkSampleCountFlagBits convert( SampleCountFlag const & flags );
}
