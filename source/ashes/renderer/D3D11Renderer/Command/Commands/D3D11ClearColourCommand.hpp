/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
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
