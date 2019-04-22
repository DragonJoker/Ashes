/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

#include <Ashes/Miscellaneous/BufferCopy.hpp>

namespace ashes::d3d11
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
			, ashes::BufferCopy const & copyInfo
			, ashes::BufferBase const & src
			, ashes::BufferBase const & dst );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		Buffer const & m_src;
		Buffer const & m_dst;
		UINT m_dstOffset;
		D3D11_BOX m_srcBox;
	};
}
