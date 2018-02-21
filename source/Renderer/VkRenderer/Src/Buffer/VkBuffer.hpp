/**
*\file
*	RenderingResources.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Buffer_HPP___
#define ___VkRenderer_Buffer_HPP___
#pragma once

#include "Miscellaneous/VkMemoryStorage.hpp"

#include <Buffer/Buffer.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Classe regroupant les ressources de rendu nécessaires au dessin d'une image.
	*/
	class Buffer
		: public renderer::BufferBase
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
		*\param[in] memoryFlags
		*	Les indicateurs de mémoire du tampon.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] size
		*	The buffer data size.
		*\param[in] target
		*	The buffer usage flags.
		*\param[in] memoryFlags
		*	The buffer memory flags.
		*/
		Buffer( Device const & device
			, uint32_t size
			, renderer::BufferTargets target
			, renderer::MemoryPropertyFlags memoryFlags );
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
		*\copydoc	renderer::BufferBase::invalidate
		*/
		void invalidate( uint32_t offset
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
		*\~french
		*\brief
		*	Opérateur de conversion implicite vers VkBuffer.
		*\~english
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const &( )const
		{
			return m_buffer;
		}

	private:
		Device const & m_device;
		uint32_t m_size{ 0u };
		VkBuffer m_buffer{ VK_NULL_HANDLE };
		BufferStoragePtr m_storage;
		mutable VkAccessFlags m_currentAccessMask{ VK_ACCESS_MEMORY_WRITE_BIT };
	};
}

#endif
