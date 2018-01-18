/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_CullModeFlag_HPP___
#define ___Renderer_CullModeFlag_HPP___
#pragma once

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class CullModeFlag
		: uint32_t
	{
		eNone = 0x00000000,
		eFront = 0x00000001,
		eBack = 0x00000002,
		eFrontAndBack = 0x00000003,
	};
	Utils_ImplementFlag( CullModeFlag )
}

#endif
