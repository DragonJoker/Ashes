/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace gl_renderer
{
	enum GlConstantFormatBaseType
		: GLenum
	{
		GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_BYTE = 0x1401,
		GL_CONSTANT_FORMAT_BASE_TYPE_INT = 0x1404,
		GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_INT = 0x1405,
		GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT = 0x1406,
	};
	std::string getName( GlConstantFormatBaseType value );

	/**
	*\brief
	*	Convertit un ashes::ConstantFormat en GlConstantFormat.
	*\param[in] flags
	*	Le ashes::ConstantFormat.
	*\return
	*	Le GlConstantFormat.
	*/
	GlConstantFormatBaseType getType( ashes::ConstantFormat format );
	/**
	*\brief
	*	R�cup�re le nombre d'�l�ments du format donn�.
	*\param[in] flags
	*	Le ashes::ConstantFormat.
	*\return
	*	Le compte.
	*/
	uint32_t getCount( ashes::ConstantFormat format );
}
