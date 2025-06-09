/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::D3D11_NAMESPACE
{
	/**
	*\brief
	*	Commande de dessin index�.
	*/
	class DrawIndexedCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] indexCount
		*	Nombre d'indices.
		*\param[in] instCount
		*	Nombre d'instances.
		*\param[in] firstIndex
		*	Index du premier indice.
		*\param[in] vertexOffset
		*	La valeur ajout�e � l'indice du sommet avant d'indexer le tampon de sommets.
		*\param[in] firstInstance
		*	Index de la premi�re instance.
		*/
		DrawIndexedCommand( VkDevice device
			, uint32_t indexCount
			, uint32_t instCount
			, uint32_t firstIndex
			, uint32_t vertexOffset
			, uint32_t firstInstance
			, VkPrimitiveTopology mode
			, VkIndexType type
			, VbosBindingArray const & vbos );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		uint32_t m_indexCount;
		uint32_t m_instCount;
		uint32_t m_firstIndex;
		uint32_t m_vertexOffset;
		uint32_t m_firstInstance;
		D3D11_PRIMITIVE_TOPOLOGY m_mode;
		DXGI_FORMAT m_type;
		uint32_t m_size;
		VbosBindingArray m_vbos;
	};
}
