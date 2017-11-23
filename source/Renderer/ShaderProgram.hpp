/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/ShaderProgram.hpp>

namespace renderer
{
	/**
	*\brief
	*	Gère les différents ShaderModules qu'un programme shader peut utiliser.
	*/
	class ShaderProgram
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		ShaderProgram( Device const & device );
		/**
		*\brief
		*	Crée un module de shader et l'ajoute à la liste.
		*\param[in] shader
		*	Le code GLSL du shader.
		*\param[in] stage
		*	Le niveau de shader utilisé pour le module.
		*/
		void createModule( std::string const & shader
			, ShaderStageFlag stage );
		/**
		*\brief
		*	Crée un module de shader et l'ajoute à la liste.
		*\param[in] shader
		*	Le code SPIR-V du shader.
		*\param[in] stage
		*	Le niveau de shader utilisé pour le module.
		*/
		void createModule( ByteArray const & shader
			, ShaderStageFlag stage );
		/**
		*\brief
		*	Le début du tableau de modules.
		*/
		inline vk::ShaderProgram const & getProgram()const
		{
			return *m_program;
		}

	private:
		vk::ShaderProgramPtr m_program;
	};
}
