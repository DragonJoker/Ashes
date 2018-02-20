/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Buffer_HPP___
#define ___GlRenderer_Buffer_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Buffer/Buffer.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class Buffer
		: public renderer::BufferBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'éléments du tampon.
		*\param[in] target
		*	Les indicateurs d'utilisation du tampon.
		*\param[in] flags
		*	Les indicateurs de mémoire du tampon.
		*/
		Buffer( renderer::Device const & device
			, uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags flags );
		/**
		*\brief
		*	Destructeur.
		*/
		~Buffer();
		/**
		*\copydoc	renderer::BufferBase::lock
		*/
		uint8_t * lock( uint32_t offset
			, uint32_t size
			, renderer::MemoryMapFlags flags )const override;
		/**
		*\copydoc	renderer::BufferBase::flush
		*/
		void flush( uint32_t offset
			, uint32_t size )const override;
		/**
		*\copydoc	renderer::BufferBase::unlock
		*/
		void unlock()const override;
		/**
		*\copydoc	renderer::BufferBase::makeTransferDestination
		*/
		renderer::BufferMemoryBarrier makeTransferDestination()const override;
		/**
		*\copydoc	renderer::BufferBase::makeTransferSource
		*/
		renderer::BufferMemoryBarrier makeTransferSource()const override;
		/**
		*\copydoc	renderer::BufferBase::makeVertexShaderInputResource
		*/
		renderer::BufferMemoryBarrier makeVertexShaderInputResource()const override;
		/**
		*\copydoc	renderer::BufferBase::makeUniformBufferInput
		*/
		renderer::BufferMemoryBarrier makeUniformBufferInput()const override;
		/**
		*\copydoc	renderer::BufferBase::makeMemoryTransitionBarrier
		*/
		renderer::BufferMemoryBarrier makeMemoryTransitionBarrier( renderer::AccessFlags dstAccess )const override;
		/**
		*\return
		*	Le tampon.
		*/
		inline GLuint getBuffer()const
		{
			assert( m_name != GL_INVALID_INDEX );
			return m_name;
		}
		/**
		*\return
		*	La cible du tampon.
		*/
		inline GlBufferTarget getTarget()const
		{
			return m_target;
		}

	private:
		GLuint m_name{ GL_INVALID_INDEX };
		GlBufferTarget m_target;
		mutable GlBufferTarget m_copyTarget;
	};
}

#endif
