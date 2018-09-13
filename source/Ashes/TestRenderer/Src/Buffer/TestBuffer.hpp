/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_Buffer_HPP___
#define ___TestRenderer_Buffer_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Buffer/Buffer.hpp>

namespace test_renderer
{
	/**
	*\~french
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class Buffer
		: public ashes::BufferBase
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] size
		*	The buffer data size.
		*\param[in] target
		*	The buffer usage flags.
		*/
		Buffer( Device const & device
			, uint32_t size
			, ashes::BufferTargets target );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~Buffer();
		/**
		*\copydoc	ashes::BufferBase::getMemoryRequirements
		*/
		ashes::MemoryRequirements getMemoryRequirements()const override;

	private:
		void doBindMemory()override;
	};
}

#endif
