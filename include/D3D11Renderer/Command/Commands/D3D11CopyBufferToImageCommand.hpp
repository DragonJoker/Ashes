/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace d3d11_renderer
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
		CopyBufferToImageCommand( Device const & device
			, ashes::BufferImageCopyArray const & copyInfo
			, ashes::BufferBase const & src
			, ashes::Image const & dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		void applyOne( Context const & context
			, ashes::BufferImageCopy const & copyInfo
			, D3D11_BOX const & srcBox
			, ashes::SubresourceLayout const & dstLayout )const;
		void doMapCopy( ashes::BufferImageCopy const & copyInfo
			, D3D11_BOX const & srcBox
			, ashes::SubresourceLayout const & dstLayout
			, ashes::BufferBase const & src
			, ashes::Image const & dst )const;
		void doCopyToStaging( Context const & context
			, ashes::BufferImageCopy const & copyInfo
			, ashes::BufferBase const & src
			, ashes::BufferBase const & staging
			, D3D11_BOX const & srcBox )const;
		void doCopyFromStaging( Context const & context
			, ashes::BufferImageCopy const & copyInfo
			, ashes::Image const & staging
			, ashes::Image const & dst )const;

	private:
		Buffer const & m_src;
		Image const & m_dst;
		ashes::BufferImageCopyArray m_copyInfo;
		DXGI_FORMAT m_format;
		std::vector< D3D11_BOX > m_srcBoxes;
		std::vector< ashes::SubresourceLayout > m_dstLayouts;
		bool m_srcMappable;
		bool m_dstMappable;
	};
}
