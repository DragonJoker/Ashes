/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DescriptorPool_HPP___
#define ___AshesPP_DescriptorPool_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include <vector>

namespace ashes
{
	/**
	*\brief
	*	Descriptor pool wrapper.
	*/
	class DescriptorPool
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] flags
		*	The creation flags.
		*/
		DescriptorPool( Device const & device
			, VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, VkDescriptorPoolSizeArray poolSizes );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] flags
		*	The creation flags.
		*/
		DescriptorPool( Device const & device
			, std::string const & debugName
			, VkDescriptorPoolCreateFlags flags
			, uint32_t maxSets
			, VkDescriptorPoolSizeArray poolSizes );
		/**
		*\brief
		*	Destructor.
		*/
		~DescriptorPool();
		/**
		*\brief
		*	Creates a descriptor set matching the given layout.
		*\param[in] layout
		*	The layout describing the set.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*\return
		*	The created descriptor set.
		*/
		DescriptorSetPtr createDescriptorSet( DescriptorSetLayout const & layout
			, uint32_t bindingPoint = 0u )const;
		/**
		*\brief
		*	Creates a descriptor set matching the given layout.
		*\param[in] layout
		*	The layout describing the set.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*\return
		*	The created descriptor set.
		*/
		DescriptorSetPtr createDescriptorSet( std::string const & debugName
			, DescriptorSetLayout const & layout
			, uint32_t bindingPoint = 0u )const;
		void freeDescriptorSet( DescriptorSetPtr set )const;
		/**
		*\name
		*	Getters.
		*/
		/**@{*/
		/**
		*\return
		*	Tells if the pool automatically deallocates the descriptor sets during its on destruction.
		*/
		inline bool hasAutomaticFree()const
		{
			return m_automaticFree;
		}

		inline Device const & getDevice()const
		{
			return m_device;
		}
		/**@}*/
		/**
		*\brief
		*	VkDescriptorPool implicit cast operator.
		*/
		inline operator VkDescriptorPool const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		bool m_automaticFree;
		VkDescriptorPool m_internal{};
	};
}

#endif
