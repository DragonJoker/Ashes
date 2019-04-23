/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	/**
	*\brief
	*	Classe de Semaphore.
	*\remarks
	*	Un sémaphore est un élément de synchronisation servant pour les files.
	*/
	class Semaphore
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*/
		explicit Semaphore( VkDevice device );
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
