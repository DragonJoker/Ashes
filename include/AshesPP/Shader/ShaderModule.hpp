/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ShaderModule_HPP___
#define ___AshesPP_ShaderModule_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

namespace ashespp
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
		*\param[in] stage
		*	The module's shader stage.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] stage
		*	Le niveau de shader utilisé pour le module.
		*/
		ShaderModule( Device const & device
			, VkShaderStageFlagBits stage );
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
		*\~english
		*\brief
		*	Loads a SPIR-V shader.
		*\param[in] shader
		*	The SPIR-V shader code.
		*\~french
		*\brief
		*	Charge un programme SPIR-V.
		*\param[in] shader
		*	Le code SPIR-V du shader.
		*/
		void loadShader( UInt32Array const & shader );
		
		inline VkShaderStageFlagBits getStage()const
		{
			return m_stage;
		}
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
		VkShaderStageFlagBits m_stage;
		VkShaderModule m_internal{ VK_NULL_HANDLE };
	};
}

#endif
