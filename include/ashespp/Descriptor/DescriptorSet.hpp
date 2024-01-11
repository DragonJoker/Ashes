/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorSet_HPP___
#define ___AshesPP_DescriptorSet_HPP___
#pragma once

#include "ashespp/Descriptor/WriteDescriptorSet.hpp"

#include "ashespp/Buffer/UniformBuffer.hpp"

#include <list>

namespace ashes
{
	/**
	*\brief
	*	A descriptor set.
	*/
	class DescriptorSet
		: public VkObject
	{
	public:
		/**
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
			, uint32_t bindingPoint
			, void const * pNext = nullptr );
		/**
		*\brief
		*	Constructor.
		*\param[in] pool
		*	The parent pool.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*/
		DescriptorSet( Device const & device
			, std::string const & debugName
			, DescriptorPool const & pool
			, DescriptorSetLayout const & layout
			, uint32_t bindingPoint
			, void const * pNext = nullptr );
		/**
		*\brief
		*	Destructor.
		*/
		~DescriptorSet()noexcept;
		/**
		*\brief
		*	Sets the descriptor set's attaches.
		*\param[in] bindings
		*	The bindings.
		*/
		void setBindings( WriteDescriptorSetArray bindings );
		/**
		*\brief
		*	Sets the descriptor set's attaches.
		*\param[in] bindings
		*	The bindings.
		*/
		void setBindings( VkWriteDescriptorSetArray const & bindings );
		/**
		*\brief
		*	Updates some descriptor set's attaches.
		*\param[in] bindings
		*	The bindings.
		*/
		void updateBindings( WriteDescriptorSetArray const & bindings )const noexcept;
		/**
		*\brief
		*	Updates some descriptor set's attaches.
		*\param[in] bindings
		*	The bindings.
		*/
		void updateBindings( VkWriteDescriptorSetArray const & bindings )const noexcept;
		/**
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
			, VkImageView view
			, VkSampler sampler
			, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			, uint32_t index = 0u );
		/**
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
			, VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			, uint32_t index = 0u );
		/**
		*\brief
		*	Creates a sampler binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] sampler
		*	The sampler.
		*\param[in] index
		*	The array index.
		*/
		void createSamplerBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, VkSampler sampler
			, uint32_t index = 0u );
		/**
		*\brief
		*	Creates a sampler binding.
		*\param[in] layoutBinding
		*	The layout binding.
		*\param[in] sampler
		*	The sampler.
		*\param[in] index
		*	The array index.
		*/
		void createSamplerBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Sampler const & sampler
			, uint32_t index = 0u );
		/**
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
			, VkImageView view
			, VkImageLayout layout
			, uint32_t index = 0u );
		/**
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
			, VkImageView view
			, uint32_t index = 0u );
		/**
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
			, UniformBuffer const & uniformBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
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
			, VkBuffer buffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
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
			, VkBuffer buffer
			, VkBufferView view
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
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
			, UniformBuffer const & uniformBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index );
		/**
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
			, VkBuffer buffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u );
		/**
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
		void createSizedBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer const & uniformBuffer
			, uint32_t elemSize
			, uint32_t offset
			, uint32_t range
			, uint32_t index )
		{
			createBinding( layoutBinding
				, uniformBuffer.getBuffer()
				, uint32_t( offset * uniformBuffer.getAlignedSize( elemSize ) )
				, uint32_t( range * uniformBuffer.getAlignedSize( elemSize ) )
				, index );
		}
		/**
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
		void createSizedBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer const & uniformBuffer
			, uint32_t offset = 0u
			, uint32_t range = 1u
			, uint32_t index = 0u )
		{
			createSizedBinding( layoutBinding
				, uniformBuffer
				, uint32_t( uniformBuffer.getElementSize() )
				, offset
				, range
				, index );
		}
		/**
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
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Buffer< T > const & storageBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u )
		{
			createBinding( layoutBinding
				, storageBuffer.getBuffer()
				, uint32_t( offset * sizeof( T ) )
				, uint32_t( range * sizeof( T ) )
				, index );
		}
		/**
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
			, UniformBuffer const & buffer
			, BufferView const & view
			, uint32_t index = 0u )
		{
			createBinding( layoutBinding
				, buffer.getBuffer()
				, view
				, index );
		}
		/**
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
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		void createSizedDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer const & uniformBuffer
			, uint32_t elemSize
			, uint32_t offset = 0u
			, uint32_t range = 1u
			, uint32_t index = 0u )
		{
			createDynamicBinding( layoutBinding
				, uniformBuffer
				, uint32_t( offset * uniformBuffer.getAlignedSize( elemSize ) )
				, uint32_t( range * uniformBuffer.getAlignedSize( elemSize ) )
				, index );
		}
		/**
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
		void createSizedDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, UniformBuffer const & uniformBuffer
			, uint32_t offset = 0u
			, uint32_t range = 1u
			, uint32_t index = 0u )
		{
			createSizedDynamicBinding( layoutBinding
				, uniformBuffer
				, uint32_t( uniformBuffer.getElementSize() )
				, offset
				, range
				, index );
		}
		/**
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
		void createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
			, Buffer< T > const & storageBuffer
			, uint32_t offset
			, uint32_t range
			, uint32_t index = 0u )
		{
			createDynamicBinding( layoutBinding
				, storageBuffer.getBuffer()
				, uint32_t( offset * sizeof( T ) )
				, uint32_t( range * sizeof( T ) )
				, index );
		}
		/**
		*\brief
		*	Updates all the bindings in the descriptor set.
		*/
		void update()const noexcept;
		/**
		*\return
		*	The binding point for the set.
		*/
		uint32_t getBindingPoint()const noexcept
		{
			return m_bindingPoint;
		}
		/**
		*\return
		*	The descriptor at given index.
		*/
		WriteDescriptorSet const & getBinding( uint32_t index )const noexcept
		{
			assert( index < m_writes.size() );
			return m_writes[index];
		}
		/**
		*\return
		*	The descriptor at given index.
		*/
		WriteDescriptorSet & getBinding( uint32_t index )noexcept
		{
			assert( index < m_writes.size() );
			return m_writes[index];
		}
		/**
		*\return
		*	The descriptor layout this descriptor set is created from.
		*/
		DescriptorSetLayout const & getLayout()const noexcept
		{
			return m_layout;
		}
		/**
		*\return
		*	Tells if the set is empty.
		*/
		bool empty()const noexcept
		{
			return m_writes.empty();
		}
		/**
		*\brief
		*	VkDescriptorSet implicit cast operator.
		*/
		operator VkDescriptorSet const & ()const noexcept
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		DescriptorPool const & m_pool;
		DescriptorSetLayout const & m_layout;
		uint32_t m_bindingPoint;
		VkDescriptorSet m_internal{};
		std::list< VkDescriptorImageInfo > m_imageBindings;
		std::list< VkDescriptorBufferInfo > m_bufferBindings;
		std::list< VkBufferView > m_bufferViews;
		WriteDescriptorSetArray m_writes;
	};
}

#endif
