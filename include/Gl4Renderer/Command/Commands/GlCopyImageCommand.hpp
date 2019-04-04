/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Commande de copie du contenu d'une image dans une autre.
	*/
	class CopyImageCommand
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
		*	L'image destination.
		*/
		CopyImageCommand( VkDevice device
			, VkImageCopy copyInfo
			, VkImage src
			, VkImage dst );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_src;
		VkImage m_dst;
		VkImageCopy m_copyInfo;
		GlInternal m_srcInternal;
		GlFormat m_srcFormat;
		GlType m_srcType;
		GlTextureType m_srcTarget;
		GlInternal m_dstInternal;
		GlFormat m_dstFormat;
		GlType m_dstType;
		GlTextureType m_dstTarget;
	};
}
