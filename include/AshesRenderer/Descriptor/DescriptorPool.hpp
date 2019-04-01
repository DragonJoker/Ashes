/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DescriptorPool_HPP___
#define ___Ashes_DescriptorPool_HPP___
#pragma once

#include "AshesRenderer/Descriptor/DescriptorPoolSize.hpp"

#include <vector>

namespace ashes
{
	/**
	*\brief
	*	Pool de descripteurs.
	*/
	class DescriptorPool
	{
	protected:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] flags
		*	The creation flags.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] flags
		*	Les indicateurs de création.
		*/
		DescriptorPool( Device const & device
			, DescriptorPoolCreateFlags flags );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~DescriptorPool();
		/**
		*\~english
		*\brief
		*	Creates a descriptor set matching the given layout.
		*\param[in] layout
		*	The layout describing the set.
		*\param[in] bindingPoint
		*	The binding point for the set.
		*\return
		*	The created descriptor set.
		*\~french
		*\brief
		*	Crée un descriptor set correspondant au layout donné.
		*\param[in] layout
		*	Le layout décrivant l'ensemble.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*\return
		*	Le descriptor set créé.
		*/
		virtual DescriptorSetPtr createDescriptorSet( ashes::DescriptorSetLayout const & layout
			, uint32_t bindingPoint = 0u )const = 0;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		/**
		*\~french
		*\brief
		*	Dit si le pool désalloue automatiquement les descripteurs à sa propre destruction.
		*\~english
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

	private:
		Device const & m_device;
		bool m_automaticFree;
	};
}

#endif
