/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_ShaderStageState_HPP___
#define ___Renderer_ShaderStageState_HPP___
#pragma once

#include "Pipeline/SpecialisationInfo.hpp"
#include "Shader/ShaderModule.hpp"

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Holds the data describing a shader stage within a pipeline.
	*\~french
	*\brief
	*	Contient les données décrivant un niveau de shader dans un pipeline.
	*/
	class ShaderStageState
	{
	public:
		ShaderStageState( ShaderStageState const & rhs ) = delete;
		ShaderStageState( ShaderStageState && rhs ) = default;
		ShaderStageState & operator=( ShaderStageState const & rhs ) = delete;
		ShaderStageState & operator=( ShaderStageState && rhs ) = default;
		~ShaderStageState() = default;
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] module
		*	The shader module.
		*\param[in] specialisationInfo
		*	The shader module specialisation constants.
		*\param[in] entryPoint.
		*	The program entry point name (unsupported with GLSL shaders).
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] module
		*	Le module shader.
		*\param[in] specialisationInfo
		*	Les constantes de spécialisation du module shader.
		*\param[in] entryPoint.
		*	Le nom du point d'entrée du programme (non supporté en GLSL).
		*/
		ShaderStageState( ShaderModulePtr && module
			, SpecialisationInfoBasePtr && specialisationInfo = nullptr
			, std::string entryPoint = "main" );
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline ShaderModule const & getModule()const
		{
			return *m_module;
		}

		inline ShaderModule & getModule()
		{
			return *m_module;
		}

		inline bool hasSpecialisationInfo()const
		{
			return m_specialisationInfo != nullptr;
		}

		inline SpecialisationInfoBase const & getSpecialisationInfo()const
		{
			assert( m_specialisationInfo );
			return *m_specialisationInfo;
		}

		inline std::string const & getEntryPoint()const
		{
			return m_entryPoint;
		}
		/**@}*/

	private:
		ShaderModulePtr m_module;
		SpecialisationInfoBasePtr m_specialisationInfo;
		std::string m_entryPoint;
		friend bool operator==( ShaderStageState const & lhs, ShaderStageState const & rhs );
	};

	inline bool operator==( ShaderStageState const & lhs, ShaderStageState const & rhs )
	{
		return lhs.m_module == rhs.m_module
			&& lhs.m_specialisationInfo == rhs.m_specialisationInfo
			&& lhs.m_entryPoint == rhs.m_entryPoint;
	}

	inline bool operator!=( ShaderStageState const & lhs, ShaderStageState const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
