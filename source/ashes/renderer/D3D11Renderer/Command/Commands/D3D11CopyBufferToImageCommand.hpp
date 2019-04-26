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
	*	Commande de copie du contenu d'un tampon dans une image.
	*/
	class CopyBufferToImageCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] copyInfo
		*	Les informations de copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*/
		CopyBufferToImageCommand( VkDevice device
			, VkBufferImageCopyArray const & copyInfo
			, VkBuffer src
			, VkImage dst );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		void applyOne( Context const & context
			, VkBufferImageCopy const & copyInfo
			, D3D11_BOX const & srcBox
			, VkSubresourceLayout const & dstLayout )const;
		void doMapCopy( VkBufferImageCopy const & copyInfo
			, D3D11_BOX const & srcBox
			, VkSubresourceLayout const & dstLayout
			, VkFormat format
			, VkDeviceMemory src
			, VkDeviceMemory dst )const;
		void doCopyToStaging( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkBuffer src
			, VkBuffer staging
			, D3D11_BOX const & srcBox )const;
		void doCopyFromStaging( Context const & context
			, VkBufferImageCopy const & copyInfo
			, VkImage staging
			, VkImage dst )const;

	private:
		VkBuffer m_src;
		VkImage m_dst;
		VkBufferImageCopyArray m_copyInfo;
		DXGI_FORMAT m_format;
		std::vector< D3D11_BOX > m_srcBoxes;
		std::vector< VkSubresourceLayout > m_dstLayouts;
		bool m_srcMappable;
		bool m_dstMappable;
	};
}
