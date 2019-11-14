/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl4
{
	enum GlTextureType
		: GLenum
	{
		GL_TEXTURE_1D = 0x0DE0,
		GL_TEXTURE_2D = 0x0DE1,
		GL_TEXTURE_3D = 0x806F,
		GL_TEXTURE_1D_ARRAY = 0x8C18,
		GL_TEXTURE_2D_ARRAY = 0x8C1A,
		GL_TEXTURE_2D_MULTISAMPLE = 0x9100,
		GL_TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102,
	};
	std::string getName( GlTextureType value );
	/**
	*\brief
	*	Convertit un VkImageType en GlTextureType.
	*\param[in] mode
	*	Le VkImageType.
	*\return
	*	Le GlTextureType.
	*/
	GlTextureType convert( VkImageType const & mode
		, uint32_t layerCount );
}
