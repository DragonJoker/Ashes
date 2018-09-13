/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

#include <Miscellaneous/BufferCopy.hpp>

namespace d3d11_renderer
{
	/**
	*\brief
	*	Commande copie d'un vertex buffer.
	*/
	class CopyBufferCommand
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
		*	Le tampon destination.
		*/
		CopyBufferCommand( Device const & device
			, renderer::BufferCopy const & copyInfo
			, renderer::BufferBase const & src
			, renderer::BufferBase const & dst );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		Buffer const & m_src;
		Buffer const & m_dst;
		UINT m_dstOffset;
		D3D11_BOX m_srcBox;
	};
}
