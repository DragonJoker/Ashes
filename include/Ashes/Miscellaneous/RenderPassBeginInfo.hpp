/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_RenderPassBeginInfo_HPP___
#define ___Ashes_RenderPassBeginInfo_HPP___
#pragma once

#include "Ashes/RenderPass/ClearValue.hpp"
#include "Ashes/Miscellaneous/Rect2D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies the render pass begin info.
	*\~french
	*\brief
	*	Définit les informations de début d'une passe de rendu.
	*/
	struct RenderPassBeginInfo
	{
		/**
		*\~english
		*\brief
		*	The render pass to begin.
		*\~french
		*\brief
		*	La passe de rendu.
		*/
		RenderPass const * renderPass;
		/**
		*\~english
		*\brief
		*	The framebuffer containing the attachments that are used with the render pass.
		*\~french
		*\brief
		*	Le tampon d'image affecté par le rendu.
		*/
		FrameBuffer const * framebuffer;
		/**
		*\~english
		*\brief
		*	The area affected by the render pass instance.
		*\~french
		*\brief
		*	L'aire affectée par la passe de rendu.
		*/
		Rect2D renderArea;
		/**
		*\~english
		*\brief
		*	The clear values for each attachment that needs to be cleared.
		*\~french
		*\brief
		*	Les valeurs de vidage, une par attache de la passe de rendu.
		*/
		ClearValueArray clearValues;
	};
}

#endif
