/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PipelineBindPoint_HPP___
#define ___Ashes_PipelineBindPoint_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Enumération des points d'attache de tampon de commandes.
	*/
	enum class PipelineBindPoint
		: uint32_t
	{
		eGraphics,
		eCompute,
	};
}

#endif
