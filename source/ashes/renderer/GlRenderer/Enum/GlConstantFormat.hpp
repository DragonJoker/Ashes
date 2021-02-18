/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

namespace ashes::gl
{
	enum GlConstantFormatBaseType
	{
		GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_BYTE = 0x1401,
		GL_CONSTANT_FORMAT_BASE_TYPE_INT = 0x1404,
		GL_CONSTANT_FORMAT_BASE_TYPE_UNSIGNED_INT = 0x1405,
		GL_CONSTANT_FORMAT_BASE_TYPE_FLOAT = 0x1406,
	};
	std::string getName( GlConstantFormatBaseType value );
	inline std::string toString( GlConstantFormatBaseType value ) { return getName( value ); }

	/**
	*\brief
	*	Convertit un ConstantFormat en GlConstantFormat.
	*\param[in] flags
	*	Le ConstantFormat.
	*\return
	*	Le GlConstantFormat.
	*/
	GlConstantFormatBaseType getType( ConstantFormat format );
	/**
	*\brief
	*	R�cup�re le nombre d'éléments du format donné.
	*\param[in] flags
	*	Le ConstantFormat.
	*\return
	*	Le compte.
	*/
	uint32_t getCount( ConstantFormat format );
}
