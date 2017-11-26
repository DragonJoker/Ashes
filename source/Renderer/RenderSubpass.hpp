/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/RenderSubpass.hpp>

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une sous passe de rendu.
	*\~english
	*\brief
	*	Describes a render subpass.
	*/
	class RenderSubpass
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] formats
		*	Les formats des attaches voulues pour la passe.
		*\param[in] neededState
		*	L'état voulu pour l'exécution de cette sous passe.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] formats
		*	The attachments pixels formats.
		*\param[in] neededState
		*	The state wanted for this subpass execution.
		*/
		RenderSubpass( Device const & device
			, std::vector< utils::PixelFormat > const & formats
			, RenderSubpassState const & neededState );
		/**
		*\return
		*	La vk::RenderSubpass.
		*/
		inline vk::RenderSubpass const & getRenderSubpass()const
		{
			return m_subPass;
		}

	private:
		vk::RenderSubpass m_subPass;
	};
}
