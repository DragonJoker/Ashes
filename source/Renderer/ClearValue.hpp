/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <Utils/Vec4.hpp>

namespace renderer
{
	/**
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
		ClearValue( utils::RgbaColour const & colour );
		ClearValue( DepthStencilClearValue const & depthStencil );

		utils::RgbaColour const & colour()
		{
			assert( m_isColour );
			return m_colour;
		}

		DepthStencilClearValue const & depthStencil()
		{
			assert( !m_isColour );
			return m_depthStencil;
		}

	private:
		friend VkClearValue convert( ClearValue const & value );
		bool m_isColour{ false };
		utils::RgbaColour m_colour;
		DepthStencilClearValue m_depthStencil;
	};
	/**
	*\brief
	*	Convertit un renderer::ClearValue en VkClearValue.
	*\param[in] value
	*	Le renderer::ClearValue.
	*\return
	*	Le VkClearValue.
	*/
	VkClearValue convert( ClearValue const & value );
}
