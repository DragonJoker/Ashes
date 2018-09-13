/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_SubpassDependency_HPP___
#define ___VkRenderer_SubpassDependency_HPP___
#pragma once

#include <RenderPass/SubpassDependency.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SubpassDependency en VkSubpassDependency.
	*\param[in] mode
	*	Le ashes::SubpassDependency.
	*\return
	*	Le VkSubpassDependency.
	*/
	VkSubpassDependency convert( ashes::SubpassDependency const & value );
}

#endif
