/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ShaderModule_HPP___
#define ___AshesPP_ShaderModule_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Gère un module shader qu'un pipeline peut utiliser.
	*/
	class ShaderModule
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructeur.
		*\param[in] device
		*	The logical device.
		*\param[in] shader
		*	The module's SPIR-V code.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] shader
		*	Le code SPIR-V du module.
		*/
		ShaderModule( Device const & device
			, UInt32Array const & shader );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~ShaderModule();
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkShaderModule.
		*\~english
		*\brief
		*	VkShaderModule implicit cast operator.
		*/
		inline operator VkShaderModule const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkShaderModule m_internal{ VK_NULL_HANDLE };
	};
}

#endif
