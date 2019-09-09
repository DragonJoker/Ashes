/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_UniformBuffer_HPP___
#define ___AshesPP_UniformBuffer_HPP___
#pragma once

#include "ashespp/Buffer/Buffer.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	A uniform variables buffer.
	*\~french
	*\brief
	*	Représente un tampon de variables uniformes.
	*/
	class UniformBufferBase
	{
	public:
		/**
		*\~english
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The number of instances of the data.
		*\param[in] size
		*	The size of an instance, in bytes.
		*\param[in] usage
		*	The buffer usage flags.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'instance des données.
		*\param[in] size
		*	La taille d'une instance des données, en octets.
		*\param[in] usage
		*	Les indicateurs d'utilisation du tampon.
		*/
		UniformBufferBase( Device const & device
			, VkDeviceSize count
			, VkDeviceSize size
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~UniformBufferBase();
		/**
		*\~english
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*\~french
		*\brief
		*	Lie ce tampon à l'objet mémoire donné.
		*\param[in] memory
		*	L'object mémoire de périphérique.
		*/
		void bindMemory( DeviceMemoryPtr memory );
		/**
		*\~english
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		VkMemoryRequirements getMemoryRequirements()const;
		/**
		*\~english
		*\brief
		*	Retrieves the aligned size for an element.
		*\param[in] size
		*	The size of an element.
		*\return
		*	The aligned size.
		*\~french
		*\brief
		*	Récupère la taille alignée pour un élément.
		*\param[in] size
		*	La taille d'un élément.
		*\return
		*	La taille alignée.
		*/
		VkDeviceSize getAlignedSize( VkDeviceSize size )const;
		/**
		*\~english
		*\return
		*	The GPU buffer.
		*\~french
		*\return
		*	Le tampon GPU.
		*/
		inline BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}
		/**
		*\~english
		*\return
		*	The GPU buffer.
		*\~french
		*\return
		*	Le tampon GPU.
		*/
		inline BufferBase & getBuffer()
		{
			return *m_buffer;
		}
		/**
		*\~english
		*\return
		*	The size of one element in the buffer.
		*\~french
		*\return
		*	La taille d'une instance des données du tampon.
		*/
		inline VkDeviceSize getElementSize()const
		{
			return m_size;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkBuffer.
		*\~english
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const & ()const
		{
			return *m_buffer;
		}

	protected:
		Device const & m_device;
		VkDeviceSize m_count;
		VkDeviceSize m_size;
		BufferBasePtr m_buffer;
	};
	/**
	*\~french
	*\brief
	*	UniformBuffer creation helper function.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The number of instances of the data.
	*\param[in] size
	*	The size of an instance, in bytes.
	*\param[in] usage
	*	The buffer usage flags.
	*\return
	*	The created buffer.
	*\~french
	*\brief
	*	Fonction d'aide à la création d'un UniformBuffer.
	*\param[in] device
	*	Le périphérique logique.
	*\param[in] count
	*	Le nombre d'instance des données.
	*\param[in] size
	*	La taille d'une instance des données, en octets.
	*\param[in] usage
	*	Les indicateurs d'utilisation du tampon.
	*\return
	*	Le tampon créé.
	*/
	inline UniformBufferBasePtr makeUniformBufferBase( Device const & device
		, VkDeviceSize count
		, VkDeviceSize size
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< UniformBufferBase >( device
			, count
			, size
			, usage
			, std::move( sharingMode ) );
	}
	/**
	*\~french
	*\brief
	*	A template class wrapping a UniformBufferBase.
	*\~french
	*\brief
	*	Classe template wrappant un UniformBufferBase.
	*/
	template< typename T >
	class UniformBuffer
	{
	public:
		/**
		*\~english
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] count
		*	The number of instances of the data.
		*\param[in] usage
		*	The buffer usage flags.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] count
		*	Le nombre d'instance des données.
		*\param[in] usage
		*	Les indicateurs d'utilisation du tampon.
		*/
		inline UniformBuffer( Device const & device
			, VkDeviceSize count
			, VkBufferUsageFlags usage
			, QueueShare sharingMode = {} );
		/**
		*\~english
		*\brief
		*	Binds this buffer to given device memory object.
		*\param[in] memory
		*	The memory object.
		*\~french
		*\brief
		*	Lie ce tampon à l'objet mémoire donné.
		*\param[in] memory
		*	L'object mémoire de périphérique.
		*/
		inline void bindMemory( DeviceMemoryPtr memory )
		{
			m_ubo.bindMemory( std::move( memory ) );
		}
		/**
		*\~english
		*\return
		*	The memory requirements for this buffer.
		*\~french
		*\return
		*	Les exigences mémoire pour ce tampon.
		*/
		inline VkMemoryRequirements getMemoryRequirements()const
		{
			return m_ubo.getMemoryRequirements();
		}
		/**
		*\~english
		*\return
		*	The N-th instance of the data.
		*\~french
		*\return
		*	La n-ème instance des données.
		*/
		inline T const & getData( VkDeviceSize index = 0 )const
		{
			return m_data[index];
		}
		/**
		*\~english
		*\return
		*	The N-th instance of the data.
		*\~french
		*\return
		*	La n-ème instance des données.
		*/
		inline T & getData( VkDeviceSize index = 0 )
		{
			return m_data[index];
		}
		/**
		*\~english
		*\return
		*	The data.
		*\~french
		*\return
		*	Les données.
		*/
		inline std::vector< T > const & getDatas( VkDeviceSize index = 0 )const
		{
			return m_data;
		}
		/**
		*\~english
		*\return
		*	The data.
		*\~french
		*\return
		*	Les données.
		*/
		inline std::vector< T > & getDatas( VkDeviceSize index = 0 )
		{
			return m_data;
		}
		/**
		*\~english
		*\brief
		*	Retrieves the aligned size for an element.
		*\return
		*	The aligned size.
		*\~french
		*\brief
		*	Récupère la taille alignée pour un élément.
		*\return
		*	La taille alignée.
		*/
		inline VkDeviceSize getAlignedSize()const
		{
			return m_ubo.getAlignedSize( sizeof( T ) );
		}
		/**
		*\~english
		*\return
		*	The GPU buffer.
		*\~french
		*\return
		*	Le tampon GPU.
		*/
		inline UniformBufferBase const & getUbo()const
		{
			return m_ubo;
		}
		/**
		*\~english
		*\return
		*	The GPU buffer.
		*\~french
		*\return
		*	Le tampon GPU.
		*/
		inline UniformBufferBase & getUbo()
		{
			return m_ubo;
		}
		/**
		*\~english
		*\brief
		*	Uploads the buffer data to VRAM
		*\param[in] offset
		*	The offset in elements from which buffer memory is mapped.
		*\param[in] range
		*	The number of elements to map.
		*\~french
		*\brief
		*	Met en VRAM les données du tampon.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] range
		*	Le nombre d'éléments à mapper.
		*/
		inline void upload( VkDeviceSize offset = 0u
			, VkDeviceSize range = 1u )
		{
			assert( range + offset <= m_data.size() );
			auto size = getAlignedSize();

			if ( auto buffer = m_ubo.getBuffer().lock( offset * size
				, range * size
				, 0u ) )
			{
				for ( auto i = 0u; i < range; ++i )
				{
					std::memcpy( buffer, &m_data[offset + i], sizeof( T ) );
					buffer += size;
				}

				m_ubo.getBuffer().flush( offset * size, range * size );
				m_ubo.getBuffer().unlock();
			}
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkBuffer.
		*\~english
		*\brief
		*	VkBuffer implicit cast operator.
		*/
		inline operator VkBuffer const & ()const
		{
			return m_ubo;
		}

	private:
		UniformBufferBase m_ubo;
		std::vector< T > m_data;
	};
	/**
	*\~french
	*\brief
	*	UniformBuffer creation helper function.
	*\param[in] device
	*	The logical device.
	*\param[in] count
	*	The number of instances of the data.
	*\param[in] usage
	*	The buffer usage flags.
	*\return
	*	The created buffer.
	*\~french
	*\brief
	*	Fonction d'aide à la création d'un UniformBuffer.
	*\param[in] device
	*	Le périphérique logique.
	*\param[in] count
	*	La nombre d'éléments dans le tampon.
	*\param[in] usage
	*	Les indicateurs d'utilisation du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	inline UniformBufferPtr< T > makeUniformBuffer( Device const & device
		, VkDeviceSize count
		, VkBufferUsageFlags usage
		, QueueShare sharingMode = {} )
	{
		return std::make_unique< UniformBuffer< T > >( device
			, count
			, usage
			, std::move( sharingMode ) );
	}
}

#include "ashespp/Buffer/UniformBuffer.inl"

#endif
