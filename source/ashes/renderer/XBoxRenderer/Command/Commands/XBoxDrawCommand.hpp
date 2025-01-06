/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"

namespace ashes::xbox
{
	/**
	*\brief
	*	Commande de dessin non index�.
	*/
	class DrawCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] vtxCount
		*	Nombre de sommets.
		*\param[in] instCount
		*	Nombre d'instances.
		*\param[in] firstVertex
		*	Index du premier sommet.
		*\param[in] firstInstance
		*	Index de la premi�re instance.
		*/
		DrawCommand( VkDevice device
			, uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance
			, VkPrimitiveTopology mode
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		uint32_t m_vtxCount;
		uint32_t m_instCount;
		uint32_t m_firstVertex;
		uint32_t m_firstInstance;
		D3D11_PRIMITIVE_TOPOLOGY m_mode;
		VbosBindingArray m_vbos;
	};
}
