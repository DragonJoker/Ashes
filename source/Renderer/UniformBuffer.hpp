/**
*\file
*	UniformBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_UniformBuffer_HPP___
#define ___Renderer_UniformBuffer_HPP___
#pragma once

#include "BufferTarget.hpp"
#include "MemoryPropertyFlag.hpp"
#include "RenderingResources.hpp"

#include <VkLib/LogicalDevice.hpp>
#include <VkLib/UniformBuffer.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe wrappant un vk::UniformBuffer.
	*/
	template< typename T >
	class UniformBuffer
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
		inline UniformBuffer( Device const & device
			, uint32_t count
			, BufferTarget target
			, MemoryPropertyFlags flags );
		/**
		*\brief
		*	Récupère l'offset dans le buffer pour un nombre d'éléments donné.
		*\param[in] count
		*	Le nombre d'éléments.
		*\return
		*	L'offset réel.
		*/
		uint32_t getOffset( uint32_t count )const
		{
			return m_buffer->getOffset( count, sizeof( T ) );
		}
		/**
		*\return
		*	Le tampon.
		*/
		inline vk::UniformBuffer const & getUbo()const
		{
			return *m_buffer;
		}
		/**
		*\return
		*	Le tampon.
		*/
		inline vk::UniformBuffer & getUbo()
		{
			return *m_buffer;
		}
		/**
		*\return
		*	La n-ème instance des données.
		*/
		inline T const & getData( uint32_t index = 0 )const
		{
			return m_data[index];
		}
		/**
		*\return
		*	La n-ème instance des données.
		*/
		inline T & getData( uint32_t index = 0 )
		{
			return m_data[index];
		}
		/**
		*\return
		*	Les données.
		*/
		inline std::vector< T > const & getDatas( uint32_t index = 0 )const
		{
			return m_data;
		}
		/**
		*\return
		*	Les données.
		*/
		inline std::vector< T > & getDatas( uint32_t index = 0 )
		{
			return m_data;
		}

	private:
		Device const & m_device;
		vk::UniformBufferPtr m_buffer;
		std::vector< T > m_data;
	};
}

#include "UniformBuffer.inl"

#endif
