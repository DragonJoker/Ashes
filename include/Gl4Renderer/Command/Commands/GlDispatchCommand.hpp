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
	*	Commande de distribution de travail.
	*/
	class DispatchCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] groupCountX, groupCountY, groupCountZ
		*	Le nombre de groupes de travail locaux à distribuer dans les dimensions Xy, Y, et Z.
		*/
		DispatchCommand( VkDevice device
			, uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		uint32_t m_groupCountX;
		uint32_t m_groupCountY;
		uint32_t m_groupCountZ;
	};
}
