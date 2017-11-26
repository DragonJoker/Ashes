/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/RenderPass.hpp>

namespace renderer
{
	/**
	*\brief
	*	Description d'une passe de rendu (pouvant contenir plusieurs sous-passes).
	*/
	class RenderPass
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] formats
		*	Les formats des attaches voulues pour la passe.
		*\param[in] subpasses
		*	Les sous passes (au moins 1 nécéssaire).
		*\param[in] initialState
		*	L'état voulu en début de passe.
		*\param[in] finalState
		*	L'état voulu en fin de passe.
		*\param[in] clear
		*	Dit si l'on veut vider le contenu des images au chargement de la passe.
		*\param[in] samplesCount
		*	Le nombre d'échantillons (pour le multisampling).
		*/
		RenderPass( Device const & device
			, std::vector< utils::PixelFormat > const & formats
			, RenderSubpassArray const & subpasses
			, RenderPassState const & initialState
			, RenderPassState const & finalState
			, bool clear = true
			, SampleCountFlag samplesCount = SampleCountFlag::e1 );
		/**
		*\brief
		*	Destructeur.
		*/
		~RenderPass();
		/**
		*\return
		*	La vk::RenderPass.
		*/
		inline vk::RenderPass const & getRenderPass()const
		{
			return m_renderPass;
		}

	private:
		vk::RenderPass m_renderPass;
	};
}
