/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_AttributeFormat_HPP___
#define ___Renderer_AttributeFormat_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\brief
	*	Liste des formats d'attributs supportés.
	*/
	enum class AttributeFormat
	{
		eFloat,
		eVec2f,
		eVec3f,
		eVec4f,
		eInt,
		eVec2i,
		eVec3i,
		eVec4i
	};
}

#endif
