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
		DrawCommand( Device const & device
			, uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance
			, VkPrimitiveTopology mode
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		uint32_t m_vtxCount;
		uint32_t m_instCount;
		uint32_t m_firstVertex;
		uint32_t m_firstInstance;
		D3D11_PRIMITIVE_TOPOLOGY m_mode;
		VbosBindingArray m_vbos;
	};
}
