/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

namespace ashes::gl3
{
	enum GlTextureType
		: GLenum
	{
		GL_TEXTURE_1D = 0x0DE0,
		GL_TEXTURE_2D = 0x0DE1,
		GL_TEXTURE_3D = 0x806F,
		GL_TEXTURE_CUBE = 0x8513,
		GL_TEXTURE_1D_ARRAY = 0x8C18,
		GL_TEXTURE_2D_ARRAY = 0x8C1A,
		GL_TEXTURE_CUBE_ARRAY = 0x9009,
		GL_TEXTURE_2D_MULTISAMPLE = 0x9100,
		GL_TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102,
		GL_TEXTURE_CUBE_POSITIVE_X = 0x8515,
		GL_TEXTURE_CUBE_NEGATIVE_X = 0x8516,
		GL_TEXTURE_CUBE_POSITIVE_Y = 0x8517,
		GL_TEXTURE_CUBE_NEGATIVE_Y = 0x8518,
		GL_TEXTURE_CUBE_POSITIVE_Z = 0x8519,
		GL_TEXTURE_CUBE_NEGATIVE_Z = 0x851A,
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
		, uint32_t layerCount
		, VkImageCreateFlags flags );
}
