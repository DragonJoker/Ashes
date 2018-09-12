/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <Miscellaneous/BufferImageCopy.hpp>

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
			, renderer::BufferImageCopyArray const & copyInfo
			, renderer::BufferBase const & src
			, renderer::Texture const & dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		void applyOne( ID3D11DeviceContext * context
			, renderer::BufferImageCopy const & copyInfo
			, D3D11_BOX const & srcBox )const;

	private:
		Buffer const & m_src;
		Texture const & m_dst;
		renderer::BufferImageCopyArray m_copyInfo;
		DXGI_FORMAT m_format;
		std::vector< D3D11_BOX > m_srcBoxes;
	};
}
