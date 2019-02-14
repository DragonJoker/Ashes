/**
*\file
*	UniformBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_UniformBuffer_HPP___
#define ___GlRenderer_UniformBuffer_HPP___
#pragma once

#include "Gl3Renderer/Core/GlDevice.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Tampon de variables uniformes.
	*/
	class UniformBuffer
		: public ashes::UniformBufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'instance des données.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		UniformBuffer( Device const & device
			, uint32_t count
			, uint32_t size
			, ashes::BufferTargets target
			, ashes::MemoryPropertyFlags flags );
		/**
		*\copydoc		ashes::UniformBuffer::getAlignedSize
		*/
		uint32_t getAlignedSize( uint32_t size )const override;

	private:
		Device const & m_device;
	};
}

#endif
