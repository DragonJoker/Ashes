/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::WrapMode en VkSamplerAddressMode.
	*\param[in] mode
	*	Le ashes::WrapMode.
	*\return
	*	Le VkSamplerAddressMode.
	*/
	VkSamplerAddressMode convert( ashes::WrapMode const & mode );
}
