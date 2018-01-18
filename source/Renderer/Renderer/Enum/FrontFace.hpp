/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_FrontFace_HPP___
#define ___Renderer_FrontFace_HPP___
#pragma once

namespace renderer
{
	/**
	*\brief
	*	Sens dans lequel les faces sont considérées comme faisant face à la caméra.
	*/
	enum class FrontFace
		: uint32_t
	{
		eCounterClockwise,
		eClockwise,
	};
}

#endif
