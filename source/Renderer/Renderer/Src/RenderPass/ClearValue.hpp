/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_ClearValue_HPP___
#define ___Renderer_ClearValue_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Specifies a clear colour value.
	*\~french
	*\brief
	*	Définit une couleur de vidage.
	*/
	union ClearColorValue
	{
		std::array< float, 4u > float32;
		std::array< int32_t, 4u > int32;
		std::array< uint32_t, 4u > uint32;
	};
	/**
	*\~english
	*\brief
	*	Specifies a clear depth/stencil value.
	*\~french
	*\brief
	*	Valeur de vidage profondeur/stencil.
	*/
	struct DepthStencilClearValue
	{
		float depth;
		uint32_t stencil;
	};
	/**
	*\brief
	*	Valeur de vidage, couleur ou profondeur/stencil.
	*/
	struct ClearValue
	{
		ClearValue();
		ClearValue( ClearColorValue const & colour );
		ClearValue( DepthStencilClearValue const & depthStencil );

		bool isColour()const
		{
			return m_isColour;
		}

		ClearColorValue const & colour()const
		{
			assert( m_isColour );
			return m_colour;
		}

		DepthStencilClearValue const & depthStencil()const
		{
			assert( !m_isColour );
			return m_depthStencil;
		}

	private:
		bool m_isColour{ false };
		ClearColorValue m_colour;
		DepthStencilClearValue m_depthStencil;
	};
}

#endif
