/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Image/TextureView.hpp>
#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace d3d11_renderer
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
			, ashes::BufferImageCopyArray const & copyInfo
			, ashes::Texture const & src
			, ashes::BufferBase const & dst );
		CopyImageToBufferCommand( CopyImageToBufferCommand const & rhs );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		void applyOne( ashes::BufferImageCopy const & copyInfo
			, TextureView const & view )const;

	private:
		Texture const & m_src;
		Buffer const & m_dst;
		ashes::BufferImageCopyArray m_copyInfo;
		DXGI_FORMAT m_format;
		std::vector< TextureViewPtr > m_views;
	};
}
