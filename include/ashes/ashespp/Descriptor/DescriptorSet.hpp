/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorSet_HPP___
#define ___AshesPP_DescriptorSet_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

#include <list>

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Set de descripteurs.
	*\~english
	*\brief
	*	A descriptor set.
	*/
	class DescriptorSet
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] pool
		*	Le pool parent.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] pool
		*	The parent pool.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*/
		DescriptorSet( Device const & device
			, DescriptorPool const & pool
			, DescriptorSetLayout const & layout
			, uint32_t bindingPoint );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~DescriptorSet();
		/**
		*\~french
		*\brief
		*	Définit les attaches du set de descripteurs.
		*\param[in] bindings
		*	Les attaches.
		*\~english
		*\brief
		*	Sets the descriptor set's attaches.
		*\param[in] bindings
		*	The bindings.
		*/
		void setBindings( VkWriteDescriptorSetArray bindings );
		/**
		*\~french
		*\brief
		*	Crée une attache de type image et échantillonneur combinés.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\param[in] sampler
		*	L'échantillonneur.
		*\param[in] layout
		*	Le layout de l'image voulu.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a combined image and sampler binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] view
		*	The image.
		*\param[in] sampler
		*	The sampler.
		*\param[in] layout
		*	The wanted image layout.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, ImageView const & view
			, Sampler const & sampler
			, VkImageLayout layout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type échantillonneur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] sampler
		*	L'échantillonneur.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a sampler binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] sampler
		*	The sampler.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Sampler const & sampler
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type image échantillonée.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\param[in] layout
		*	Le layout de l'image.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a sampled image binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] view
		*	The image.
		*\param[in] layout
		*	The image layout.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, ImageView const & view
			, VkImageLayout layout
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type image de stockage.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] view
		*	L'image.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a storage image binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] view
		*	The image.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, ImageView const & view
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon uniforme.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] uniformBuffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a uniform buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] uniformBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBufferBase const & uniformBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de stockage ou uniforme.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] buffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a storage or uniform buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] buffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, BufferBase const & buffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de texels.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] buffer
		*	Le tampon.
		*\param[in] view
		*	La vue sur le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a texel buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] buffer
		*	The buffer.
		*\param[in] view
		*	The view to the buffer.
		*\param[in] index
		*	The array index.
		*/
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, BufferBase const & buffer
			, BufferView const & view
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon uniforme dynamique.
		*\remarks
		*	Permet de spécifier un offset supplémentaire au moment du binding du descripteur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] uniformBuffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a dynamic uniform buffer binding.
		*\remarks
		*	Allow specification of an additional offset at descriptor's binding time.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] uniformBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		void createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBufferBase const & uniformBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de stockage ou uniforme dynamique.
		*\remarks
		*	Permet de spécifier un offset supplémentaire au moment du binding du descripteur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] buffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a dynamic storage or uniform buffer binding.
		*\remarks
		*	Allow specification of an additional offset at descriptor's binding time.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] buffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		void createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, BufferBase const & buffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de variables uniformes.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] uniformBuffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a uniform buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] uniformBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		template< typename T >
		inline void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer< T > const & uniformBuffer
			, uint32_t offset = 0u
			, uint32_t range = 1u
			, uint32_t index = 0u )
		{
			createBinding( layoutBinding
				, uniformBuffer.getUbo()
				, offset * uniformBuffer.getAlignedSize()
				, range * uniformBuffer.getAlignedSize()
				//, range * sizeof( T )
				, index );
		}
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de stockage.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] storageBuffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a storage buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] storageBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		template< typename T >
		inline void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Buffer< T > const & storageBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u )
		{
			createBinding( layoutBinding
				, storageBuffer.getBuffer()
				, offset * uint32_t( sizeof( T ) )
				, range * uint32_t( sizeof( T ) )
				, index );
		}
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de texels.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] buffer
		*	Le tampon.
		*\param[in] view
		*	La vue sur le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a texel buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] buffer
		*	The buffer.
		*\param[in] view
		*	The view to the buffer.
		*\param[in] index
		*	The array index.
		*/
		template< typename T >
		inline void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer< T > const & buffer
			, BufferView const & view
			, uint32_t index = 0u )
		{
			createBinding( layoutBinding
				, buffer.getUbo()
				, view
				, index );
		}
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de texels.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] buffer
		*	Le tampon.
		*\param[in] view
		*	La vue sur le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a texel buffer binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] buffer
		*	The buffer.
		*\param[in] view
		*	The view to the buffer.
		*\param[in] index
		*	The array index.
		*/
		template< typename T >
		inline void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Buffer< T > const & buffer
			, BufferView const & view
			, uint32_t index = 0u )
		{
			createBinding( layoutBinding
				, buffer.getBuffer()
				, view
				, index );
		}
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de variables uniformes dynamique.
		*\remarks
		*	Permet de spécifier un offset supplémentaire au moment du binding du descripteur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] uniformBuffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a dynamic uniform buffer binding.
		*\remarks
		*	Allow specification of an additional offset at descriptor's binding time.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] uniformBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		template< typename T >
		inline void createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer< T > const & uniformBuffer
			, uint32_t offset = 0u
			, uint32_t range = 1u
			, uint32_t index = 0u )
		{
			createDynamicBinding( layoutBinding
				, uniformBuffer.getUbo()
				, offset * uniformBuffer.getAlignedSize()
				, range * uniformBuffer.getAlignedSize()
				//, range * sizeof( T )
				, index );
		}
		/**
		*\~french
		*\brief
		*	Crée une attache de type tampon de stockage dynamique.
		*\remarks
		*	Permet de spécifier un offset supplémentaire au moment du binding du descripteur.
		*\param[in] layoutBinding
		*	L'attache de layout.
		*\param[in] storageBuffer
		*	Le tampon.
		*\param[in] offset
		*	Le décalage de l'attache dans le tampon.
		*\param[in] range
		*	Le décompte des données pouvant être lues depuis l'attache dans le tampon.
		*\param[in] index
		*	L'indice dans le tableau.
		*\~english
		*\brief
		*	Creates a dynamic storage buffer binding.
		*\remarks
		*	Allow specification of an additional offset at descriptor's binding time.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] storageBuffer
		*	The buffer.
		*\param[in] offset
		*	The attach's offset in the buffer.
		*\param[in] range
		*	The amount of data that can be read from the buffer.
		*\param[in] index
		*	The array index.
		*/
		template< typename T >
		inline void createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Buffer< T > const & storageBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u )
		{
			createDynamicBinding( layoutBinding
				, storageBuffer.getBuffer()
				, offset * uint32_t( sizeof( T ) )
				, range * uint32_t( sizeof( T ) )
				, index );
		}
		/**
		*\~french
		*\brief
		*	Met à jour toutes les attaches du set de descripteurs.
		*\~english
		*\brief
		*	Updates all the bindings in the descriptor set.
		*/
		void update()const;
		/**
		*\~french
		*\return
		*	Le point d'attache du set.
		*\~english
		*\return
		*	The binding point for the set.
		*/
		inline uint32_t getBindingPoint()const
		{
			return m_bindingPoint;
		}
		/**
		*\~french
		*\return
		*	Le descripteur à l'indice donné.
		*\~english
		*\return
		*	The descriptor at given index.
		*/
		inline VkWriteDescriptorSet const & getBinding( uint32_t index )const
		{
			assert( index < m_writes.size() );
			return m_writes[index];
		}
		/**
		*\~french
		*\return
		*	Le descripteur à l'indice donné.
		*\~english
		*\return
		*	The descriptor at given index.
		*/
		inline VkWriteDescriptorSet & getBinding( uint32_t index )
		{
			assert( index < m_writes.size() );
			return m_writes[index];
		}
		/**
		*\~french
		*\return
		*	Dit si l'ensemble est vide.
		*\~english
		*\return
		*	Tells if the set is empty.
		*/
		inline bool empty()const
		{
			return m_writes.empty();
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkDescriptorSet.
		*\~english
		*\brief
		*	VkDescriptorSet implicit cast operator.
		*/
		inline operator VkDescriptorSet const & ()const
		{
			return m_internal;
		}

	protected:
		VkWriteDescriptorSetArray m_writes;

	private:
		Device const & m_device;
		DescriptorPool const & m_pool;
		uint32_t m_bindingPoint;
		VkDescriptorSet m_internal{ VK_NULL_HANDLE };
		std::list< VkDescriptorImageInfo > m_imageBindings;
		std::list< VkDescriptorBufferInfo > m_bufferBindings;
		std::list< VkBufferView > m_bufferViews;
	};
}

#endif
