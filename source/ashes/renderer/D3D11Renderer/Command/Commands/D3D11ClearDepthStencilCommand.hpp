/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	/**
	*\brief
	*	Commande de vidage d'une image.
	*/
	class ClearDepthStencilCommand
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
		ClearDepthStencilCommand( VkDevice device
			, VkImage image
			, ArrayView< VkImageSubresourceRange const > ranges
			, VkClearDepthStencilValue value );
		ClearDepthStencilCommand( ClearDepthStencilCommand const & rhs );
		~ClearDepthStencilCommand()override;

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_image;
		VkImageSubresourceRangeArray m_ranges;
		VkClearDepthStencilValue m_value;
		UINT m_flags;
		VkImageViewArray m_views;
	};
}
