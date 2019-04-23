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
	*\~english
	*\brief
	*	A descriptor set layout.
	*\~french
	*\brief
	*	Un layout de set de descripteurs.
	*/
	class DescriptorSetLayout
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] bindings
		*	The layout bindings.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] bindings
		*	Les attaches du layout.
		*/
		DescriptorSetLayout( Device const & device
			, VkDescriptorSetLayoutBindingArray bindings );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~DescriptorSetLayout();
		/**
		*\~english
		*\return
		*	The descriptor set attach at given binding point and index.
		*\param[in] point
		*	The binding point.
		*\param[in] index
		*	The binding index, for array descriptors.
		*\~french
		*\return
		*	L'attache de descripteur au point d'attache et à l'index donnés.
		*\param[in] point
		*	Le point d'attache.
		*\param[in] index
		*	L'index d'attache, pour les descipteurs tableau.
		*/
		VkDescriptorSetLayoutBinding const & getBinding( uint32_t point
			, uint32_t index = 0u )const;
		/**
		*\~english
		*\return
		*	The descriptor set attaches.
		*\~french
		*\return
		*	Les attaches de descripteurs.
		*/
		inline VkDescriptorSetLayoutBindingArray const & getBindings()const
		{
			return m_bindings;
		}
		/**
		*\~english
		*\brief
		*	Creates a pool for the descriptor sets using this layout.
		*\param[in] maxSets
		*	The maximum descriptor sets taht can be created from the pool.
		*\param[in] automaticFree
		*	Tells if the pool will destroy all allocated sets at its destruction (\p true) or not (\p false).
		*\return
		*	The created pool.
		*\~french
		*\brief
		*	Crée un pool pour les descripteurs qui utiliseront ce layout.
		*\param[in] maxSets
		*	Le nombre maximum de sets que le pool peut créer.
		*\param[in] automaticFree
		*	Dit si le pool détruira automatiquement les sets qu'il a alloués à sa destruction (\p true) ou pas (\p false).
		*\return
		*	Le pool créé.
		*/
		DescriptorSetPoolPtr createPool( uint32_t maxSets
			, bool automaticFree = true )const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkDescriptorSetLayout.
		*\~english
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
		VkDescriptorSetLayout m_internal{ VK_NULL_HANDLE };
	};
}

#endif
