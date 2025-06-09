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
	*	Commande de copie du contenu d'une image dans un tampon.
	*/
	class CopyImageToBufferCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] copyInfo
		*	Les informations de copie.
		*\param[in] src
		*	L'image source.
		*\param[in] dst
		*	Le tampon destination.
		*/
		CopyImageToBufferCommand( VkDevice device
			, ArrayView< VkBufferImageCopy const > const & copyInfo
			, VkImage src
			, VkBuffer dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		void applyOne( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkSubresourceLayout const & srcLayout
			, D3D11_BOX const & dstBox )const;
		void doMapCopy( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkSubresourceLayout const & srcLayout
			, D3D11_BOX const & dstBox
			, VkFormat format
			, VkDeviceMemory src
			, VkDeviceMemory dst )const;
		void doCopyToStaging( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkImage src
			, VkImage staging )const;
		void doCopyFromStaging( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkBuffer staging
			, VkBuffer dst
			, D3D11_BOX const & dstBox )const;

	private:
		VkImage m_src;
		VkBuffer m_dst;
		VkBufferImageCopyArray m_copyInfo;
		DXGI_FORMAT m_format;
		std::vector< VkSubresourceLayout > m_srcLayouts;
		std::vector< D3D11_BOX > m_dstBoxes;
		bool m_srcMappable;
		bool m_dstMappable;
	};
}
