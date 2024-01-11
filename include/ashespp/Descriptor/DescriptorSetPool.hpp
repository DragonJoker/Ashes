/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorSetPool_HPP___
#define ___AshesPP_DescriptorSetPool_HPP___
#pragma once

#include "ashespp/Descriptor/DescriptorPool.hpp"

#include <vector>

namespace ashes
{
	/**
	*\brief
	*	Descriptor set pool helper.
	*/
	class DescriptorSetPool
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] layout
		*	The layout from which the pool will be created.
		*\param[in] maxSets
		*	The maximum sets count that can be created from this pool.
		*\param[in] automaticFree
		*	Tells if the pool automatically frees the sets it has allocated, during its own destruction.
		*/
		DescriptorSetPool( Device const & device
			, DescriptorSetLayout const & layout
			, uint32_t maxSets
			, bool automaticFree );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] layout
		*	The layout from which the pool will be created.
		*\param[in] maxSets
		*	The maximum sets count that can be created from this pool.
		*\param[in] automaticFree
		*	Tells if the pool automatically frees the sets it has allocated, during its own destruction.
		*/
		DescriptorSetPool( Device const & device
			, std::string const & debugName
			, DescriptorSetLayout const & layout
			, uint32_t maxSets
			, bool automaticFree );
		/**
		*\brief
		*	Creates a descriptor set matching the layout defined for this pool.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*\return
		*	The created descriptor set.
		*/
		DescriptorSetPtr createDescriptorSet( uint32_t bindingPoint = 0u )const;
		/**
		*\brief
		*	Creates a descriptor set matching the layout defined for this pool.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*\return
		*	The created descriptor set.
		*/
		DescriptorSetPtr createDescriptorSet( std::string const & debugName
			, uint32_t bindingPoint = 0u )const;
		void freeDescriptorSet( DescriptorSetPtr set )const;
		/**
		*\return
		*	The descriptor set layout.
		*/
		inline DescriptorSetLayout const & getLayout()const noexcept
		{
			return m_layout;
		}
		/**
		*\return
		*	The descriptor set layout.
		*/
		inline DescriptorPool const & getPool()const noexcept
		{
			return *m_pool;
		}
		/**
		*\brief
		*	VkDescriptorPool implicit cast operator.
		*/
		inline operator VkDescriptorPool const & ()const noexcept
		{
			return *m_pool;
		}

	private:
		DescriptorSetLayout const & m_layout;
		DescriptorPoolPtr m_pool;
	};
}

#endif
