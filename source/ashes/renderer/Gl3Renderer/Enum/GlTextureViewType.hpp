/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl3
{
	/**
	*\brief
	*	Convertit un VkSamplerAddressMode en GlTextureType.
	*\param[in] mode
	*	Le VkSamplerAddressMode.
	*\return
	*	Le GlTextureType.
	*/
	GlTextureType convert( VkImageViewType const & mode
		, VkSampleCountFlagBits samples );
	GlTextureType convert( VkImageViewType const & mode
		, VkSampleCountFlagBits samples
		, uint32_t layers );
}
