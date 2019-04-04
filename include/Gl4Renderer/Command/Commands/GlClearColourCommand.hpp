/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include "Gl4Renderer/Enum/GlFormat.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Commande de vidage d'une image.
	*/
	class ClearColourCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] image
		*	L'image à vider.
		*\param[in] colour
		*	La couleur de vidage.
		*/
		ClearColourCommand( VkDevice device
			, VkImage image
			, VkImageLayout imageLayout
			, VkClearColorValue value
			, VkImageSubresourceRangeArray ranges );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_image;
		VkClearColorValue m_colour;
		VkImageSubresourceRangeArray m_ranges;
		GlInternal m_internal;
		GlFormat m_format;
		GlType m_type;
	};
}
