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
	*	Convertit un ashes::MipmapMode en VkSamplerMipmapMode.
	*\param[in] mode
	*	Le ashes::MipmapMode.
	*\return
	*	Le VkSamplerMipmapMode.
	*/
	VkSamplerMipmapMode convert( ashes::MipmapMode const & mode );
}
