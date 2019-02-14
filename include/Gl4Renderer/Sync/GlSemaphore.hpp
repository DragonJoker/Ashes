/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Sync/Semaphore.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Classe de Semaphore.
	*\remarks
	*	Un sémaphore est un élément de synchronisation servant pour les files.
	*/
	class Semaphore
		: public ashes::Semaphore
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*/
		explicit Semaphore( ashes::Device const & device );
		/**
		*\brief
		*	Conversion implicite vers VkSemaphore.
		*/
		inline GLuint getSemaphore()const
		{
			return m_semaphore;
		}

	private:
		GLuint m_semaphore;
	};
}
