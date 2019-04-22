/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Image/ImageView.hpp>
#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace ashes::d3d11
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
		CopyImageToBufferCommand( Device const & device
			, ashes::VkBufferImageCopyArray const & copyInfo
			, ashes::Image const & src
			, ashes::BufferBase const & dst );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		void applyOne( Context const & context
			, ashes::BufferImageCopy const & copyInfo
			, ashes::SubresourceLayout const & srcLayout
			, D3D11_BOX const & dstBox )const;
		void doMapCopy( ashes::BufferImageCopy const & copyInfo
			, ashes::SubresourceLayout const & srcLayout
			, D3D11_BOX const & dstBox
			, ashes::Image const & src
			, ashes::BufferBase const & dst )const;
		void doCopyToStaging( Context const & context
			, ashes::BufferImageCopy const & copyInfo
			, ashes::Image const & src
			, ashes::Image const & staging )const;
		void doCopyFromStaging( Context const & context
			, ashes::BufferImageCopy const & copyInfo
			, ashes::BufferBase const & staging
			, ashes::BufferBase const & dst
			, D3D11_BOX const & dstBox )const;

	private:
		Image const & m_src;
		Buffer const & m_dst;
		ashes::VkBufferImageCopyArray m_copyInfo;
		DXGI_FORMAT m_format;
		std::vector< ashes::SubresourceLayout > m_srcLayouts;
		std::vector< D3D11_BOX > m_dstBoxes;
		bool m_srcMappable;
		bool m_dstMappable;
	};
}
