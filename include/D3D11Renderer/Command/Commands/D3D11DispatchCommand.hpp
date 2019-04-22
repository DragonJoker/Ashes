/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
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
		DispatchCommand( Device const & device
			, uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		uint32_t m_groupCountX;
		uint32_t m_groupCountY;
		uint32_t m_groupCountZ;
	};
}
