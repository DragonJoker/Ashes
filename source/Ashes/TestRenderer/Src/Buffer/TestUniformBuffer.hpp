/**
*\file
*	UniformBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_UniformBuffer_HPP___
#define ___TestRenderer_UniformBuffer_HPP___
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Buffer/UniformBuffer.hpp>

namespace test_renderer
{
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
		UniformBuffer( ashes::Device const & device
			, uint32_t count
			, uint32_t size
			, ashes::BufferTargets target
			, ashes::MemoryPropertyFlags flags );
		/**
		*\copydoc		ashes::UniformBuffer::getAlignedSize
		*/
		uint32_t getAlignedSize( uint32_t size )const override;
	};
}

#endif
