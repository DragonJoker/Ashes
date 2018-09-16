/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ShaderModule_HPP___
#define ___Ashes_ShaderModule_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Gère un module shader qu'un pipeline peut utiliser.
	*/
	class ShaderModule
	{
	protected:
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
			, ShaderStageFlag stage );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~ShaderModule();
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
		virtual void loadShader( UInt32Array const & shader ) = 0;
		
		inline ShaderStageFlag getStage()const
		{
			return m_stage;
		}

	protected:
		Device const & m_device;
		ShaderStageFlag m_stage;
	};
}

#endif
