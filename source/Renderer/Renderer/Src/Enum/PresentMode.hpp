/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_PresentMode_HPP___
#define ___Renderer_PresentMode_HPP___
#pragma once

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Surface presentation mode.
	*\~french
	*\brief
	*	Mode de présentation de la surface.
	*/
	enum class PresentMode
		: uint32_t
	{
		eImmediate = 0,
		eMailbox = 1,
		eFifo = 2,
		eFifoRelaxed = 3,
		eSharedDemandRefresh = 1000111000,
		eSharedContinuousRefresh = 1000111001,
		Renderer_EnumBoundsEx( eImmediate, eFifoRelaxed )
	};
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
	*/
	inline std::string getName( PresentMode value )
	{
		switch ( value )
		{
		case PresentMode::eImmediate:
			return "immediate";

		case PresentMode::eMailbox:
			return "mailbox";

		case PresentMode::eFifo:
			return "fifo";

		case PresentMode::eFifoRelaxed:
			return "fifo_relaxed";

		case PresentMode::eSharedDemandRefresh:
			return "shared_demand_refresh";

		case PresentMode::eSharedContinuousRefresh:
			return "shared_continuous_refresh";

		default:
			assert( false && "Unsupported PresentMode." );
			throw std::runtime_error{ "Unsupported PresentMode" };
		}

		return 0;
	}
}

#endif
