/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorSet_HPP___
#define ___AshesPP_DescriptorSet_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include "ashespp/Buffer/UniformBuffer.hpp"

#include <list>

namespace ashes
{
	struct WriteDescriptorSet
	{
		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, uint32_t descriptorCount
			, VkDescriptorType descriptorType )
			: vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, descriptorCount, descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
		}

		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, VkDescriptorType descriptorType
			, VkDescriptorImageInfoArray imageInfos )
			: imageInfo{ std::move( imageInfos ) }
			, vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, uint32_t( this->imageInfo.size() ), descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
		}

		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, VkDescriptorType descriptorType
			, VkDescriptorBufferInfoArray bufferInfos )
			: bufferInfo{ std::move( bufferInfos ) }
			, vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, uint32_t( this->bufferInfo.size() ), descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
		}

		WriteDescriptorSet( uint32_t dstBinding
			, uint32_t dstArrayElement
			, VkDescriptorType descriptorType
			, VkDescriptorBufferInfoArray bufferInfos
			, VkBufferViewArray viewInfos )
			: bufferInfo{ std::move( bufferInfos ) }
			, texelBufferView{ std::move( viewInfos ) }
			, vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, nullptr, dstBinding, dstArrayElement, uint32_t( this->bufferInfo.size() ), descriptorType, nullptr, nullptr, nullptr }
			, needsUpdate{ true }
		{
			assert( bufferInfo.size() == texelBufferView.size() );
		}

		WriteDescriptorSet( VkDescriptorSet set
			, uint32_t dstBinding
			, uint32_t dstArrayElement
			, uint32_t descriptorCount
			, VkDescriptorType descriptorType
			, VkDescriptorImageInfo const * pimageInfo
			, VkDescriptorBufferInfo const * pbufferInfo
			, VkBufferView const * ptexelBufferView )
			: vk{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, set, dstBinding, dstArrayElement, descriptorCount, descriptorType, pimageInfo, pbufferInfo, ptexelBufferView }
			, needsUpdate{ false }
		{
		}

		void update( VkDescriptorSet descriptorSet )const
		{
			if ( needsUpdate )
			{
				vk.dstSet = descriptorSet;
				vk.pImageInfo = imageInfo.data();
				vk.pBufferInfo = bufferInfo.data();
				vk.pTexelBufferView = texelBufferView.data();
			}
		}

		operator VkWriteDescriptorSet const &()const
		{
			return vk;
		}

		inline VkWriteDescriptorSet const * operator->()const
		{
			return &vk;
		}

		inline VkWriteDescriptorSet * operator->()
		{
			return &vk;
		}

		VkDescriptorImageInfoArray imageInfo;
		VkDescriptorBufferInfoArray bufferInfo;
		VkBufferViewArray texelBufferView;

	private:
		mutable VkWriteDescriptorSet vk{};
		bool needsUpdate;
	};
	/**
	*\brief
	*	A descriptor set.
	*/
	class DescriptorSet
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
			, uint32_t bindingPoint );
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
			, uint32_t bindingPoint );
		/**
		*\brief
		*	Destructor.
		*/
		~DescriptorSet();
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
		void setBindings( VkWriteDescriptorSetArray bindings );
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
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createSizedBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createSizedBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createSizedDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createSizedDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		inline void createDynamicBinding( VkDescriptorSetLayoutBinding const & layoutBinding
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
		void update()const;
		/**
		*\return
		*	The binding point for the set.
		*/
		inline uint32_t getBindingPoint()const
		{
			return m_bindingPoint;
		}
		/**
		*\return
		*	The descriptor at given index.
		*/
		inline WriteDescriptorSet const & getBinding( uint32_t index )const
		{
			assert( index < m_writes.size() );
			return m_writes[index];
		}
		/**
		*\return
		*	The descriptor at given index.
		*/
		inline WriteDescriptorSet & getBinding( uint32_t index )
		{
			assert( index < m_writes.size() );
			return m_writes[index];
		}
		/**
		*\return
		*	The descriptor layout this descriptor set is created from.
		*/
		inline DescriptorSetLayout const & getLayout()
		{
			return m_layout;
		}
		/**
		*\return
		*	Tells if the set is empty.
		*/
		inline bool empty()const
		{
			return m_writes.empty();
		}
		/**
		*\brief
		*	VkDescriptorSet implicit cast operator.
		*/
		inline operator VkDescriptorSet const & ()const
		{
			return m_internal;
		}

	protected:
		WriteDescriptorSetArray m_writes;

	private:
		Device const & m_device;
		DescriptorPool const & m_pool;
		DescriptorSetLayout const & m_layout;
		uint32_t m_bindingPoint;
		VkDescriptorSet m_internal{ VK_NULL_HANDLE };
		std::list< VkDescriptorImageInfo > m_imageBindings;
		std::list< VkDescriptorBufferInfo > m_bufferBindings;
		std::list< VkBufferView > m_bufferViews;
	};
}

#endif
