/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_PolygonMode_HPP___
#define ___Renderer_PolygonMode_HPP___
#pragma once

namespace renderer
{
	/**
	*\brief
	*	Modes d'affichage des polygones.
	*/
	enum class PolygonMode
		: uint32_t
	{
		eFill,
		eLine,
		ePoint,
	};
}

#endif
