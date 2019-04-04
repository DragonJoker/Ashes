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
			, VkIndexType type );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		uint32_t m_indexCount;
		uint32_t m_instCount;
		size_t m_firstIndex;
		uint32_t m_vertexOffset;
		uint32_t m_firstInstance;
		GlPrimitiveTopology m_mode;
		GlIndexType m_type;
		uint32_t m_size;
	};
}
