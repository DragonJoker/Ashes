/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/ColourBlendStateAttachment.hpp>

namespace renderer
{
	/**
	*\brief
	*	Wrapper de ColourBlendStateAttachment.
	*\~english
	*\brief
	*	ColourBlendStateAttachment wrapper.
	*/
	class ColourBlendStateAttachment
	{
	public:
		ColourBlendStateAttachment( bool blendEnable = false
			, BlendFactor srcColorBlendFactor = BlendFactor::eOne
			, BlendFactor dstColorBlendFactor = BlendFactor::eZero
			, BlendOp colorBlendOp = BlendOp::eAdd
			, BlendFactor srcAlphaBlendFactor = BlendFactor::eOne
			, BlendFactor dstAlphaBlendFactor = BlendFactor::eZero
			, BlendOp alphaBlendOp = BlendOp::eAdd
			, ColourComponentFlags colorWriteMask = ColourComponentFlag::eR
				| ColourComponentFlag::eG
				| ColourComponentFlag::eB
				| ColourComponentFlag::eA );
		/*
		*\~french
		*\return
		*	Le vk::ColourBlendAttachment.
		*/
		inline vk::ColourBlendStateAttachment const & getAttachment()const
		{
			return m_state;
		}

	private:
		vk::ColourBlendStateAttachment m_state;
	};
}
