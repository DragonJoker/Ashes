/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_WrapMode_HPP___
#define ___Renderer_WrapMode_HPP___
#pragma once

namespace renderer
{
	/**
	*\brief
	*	Enumération des modes de wrap.
	*/
	enum class WrapMode
	{
		eRepeat,
		eMirroredRepeat,
		eClampToEdge,
		eClampToBorder,
		eMirrorClampToEdge,
		Utils_EnumBounds( eRepeat )
	};
}

#endif
