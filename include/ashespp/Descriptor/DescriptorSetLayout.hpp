/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorSetLayout_HPP___
#define ___AshesPP_DescriptorSetLayout_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	A descriptor set layout.
	*/
	class DescriptorSetLayout
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] bindings
		*	The layout bindings.
		*/
		DescriptorSetLayout( Device const & device
			, VkDescriptorSetLayoutBindingArray bindings
			, VkDescriptorBindingFlags flags = {} );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] bindings
		*	The layout bindings.
		*/
		DescriptorSetLayout( Device const & device
			, std::string debugName
			, VkDescriptorSetLayoutBindingArray bindings
			, VkDescriptorBindingFlags flags = {} );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] bindings
		*	The layout bindings.
		*/
		DescriptorSetLayout( Device const & device
			, std::string debugName
			, VkDescriptorSetLayoutCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~DescriptorSetLayout();
		/**
		*\return
		*	The descriptor set attach at given binding point and index.
		*\param[in] point
		*	The binding point.
		*\param[in] index
		*	The binding index, for array descriptors.
		*/
		VkDescriptorSetLayoutBinding const & getBinding( uint32_t point
			, uint32_t index = 0u )const;
		/**
		*\return
		*	The descriptor set attaches.
		*/
		inline VkDescriptorSetLayoutBindingArray const & getBindings()const
		{
			return m_bindings;
		}
		/**
		*\brief
		*	Creates a pool for the descriptor sets using this layout.
		*\param[in] maxSets
		*	The maximum descriptor sets taht can be created from the pool.
		*\param[in] automaticFree
		*	Tells if the pool will destroy all allocated sets at its destruction (\p true) or not (\p false).
		*\return
		*	The created pool.
		*/
		DescriptorSetPoolPtr createPool( uint32_t maxSets
			, bool automaticFree = true )const;
		/**
		*\brief
		*	Creates a pool for the descriptor sets using this layout.
		*\param[in] maxSets
		*	The maximum descriptor sets taht can be created from the pool.
		*\param[in] automaticFree
		*	Tells if the pool will destroy all allocated sets at its destruction (\p true) or not (\p false).
		*\return
		*	The created pool.
		*/
		DescriptorSetPoolPtr createPool( std::string const & debugName
			, uint32_t maxSets
			, bool automaticFree = true )const;
		/**
		*\brief
		*	VkDescriptorSetLayout implicit cast operator.
		*/
		inline operator VkDescriptorSetLayout const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkDescriptorSetLayoutBindingArray m_bindings;
		VkDescriptorSetLayout m_internal{};
	};
}

#endif
