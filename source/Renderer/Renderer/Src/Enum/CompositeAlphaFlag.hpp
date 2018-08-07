/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_CompositeAlphaFlag_HPP___
#define ___Renderer_CompositeAlphaFlag_HPP___
#pragma once

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Alpha composite flags.
	*\~french
	*\brief
	*	Indicateurs de composition alpha.
	*/
	enum class CompositeAlphaFlag
		: uint32_t
	{
		eOpaque = 0x00000001,
		ePreMultiplied = 0x00000002,
		ePostMultiplied = 0x00000004,
		eInherit = 0x00000008,
	};
	Renderer_ImplementFlag( CompositeAlphaFlag )
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
	inline std::string getName( CompositeAlphaFlags value )
	{
		std::string result;
		std::string sep;

		if ( checkFlag( value, CompositeAlphaFlag::eOpaque ) )
		{
			result += sep + "opaque";
			sep = " | ";
		}

		if ( checkFlag( value, CompositeAlphaFlag::ePreMultiplied ) )
		{
			result += sep + "pre_multiplied";
			sep = " | ";
		}

		if ( checkFlag( value, CompositeAlphaFlag::ePostMultiplied ) )
		{
			result += sep + "post_multiplied";
			sep = " | ";
		}

		if ( checkFlag( value, CompositeAlphaFlag::eInherit ) )
		{
			result += sep + "inherit";
			sep = " | ";
		}

		return result;
	}
}

#endif
