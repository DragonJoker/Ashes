/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
{
	/**
	*\brief
	*	Classe de base d'une commande.
	*/
	class MemoryBarrierCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		MemoryBarrierCommand( Device const & device
			, ashes::PipelineStageFlags after
			, ashes::PipelineStageFlags before
			, ashes::MemoryBarrierArray const & memoryBarriers
			, ashes::BufferMemoryBarrierArray const & bufferBarriers
			, ashes::ImageMemoryBarrierArray const & imageBarriers );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;
	};
}
