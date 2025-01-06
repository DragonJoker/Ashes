/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
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
			, ArrayView< VkImageSubresourceRange const > ranges
			, VkClearColorValue const & colour );
		ClearColourCommand( ClearColourCommand  const & rhs );
		ClearColourCommand & operator=( ClearColourCommand  const & rhs );
		ClearColourCommand( ClearColourCommand && rhs )noexcept = default;
		ClearColourCommand & operator=( ClearColourCommand && rhs )noexcept = default;
		~ClearColourCommand()noexcept override;

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_image;
		VkImageSubresourceRangeArray m_ranges;
		VkClearColorValue m_colour;
		VkImageViewArray m_views;
	};
}
