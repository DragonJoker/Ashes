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
	*	Wrapper to a pipeline shader module.
	*/
	class ShaderModule
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	The logical device.
		*\param[in] shader
		*	The module's SPIR-V code.
		*/
		ShaderModule( Device const & device
			, UInt32Array const & shader );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	The logical device.
		*\param[in] shader
		*	The module's SPIR-V code.
		*/
		ShaderModule( Device const & device
			, std::string const & debugName
			, UInt32Array const & shader );
		/**
		*\brief
		*	Destructor.
		*/
		~ShaderModule()noexcept;
		/**
		*\brief
		*	VkShaderModule implicit cast operator.
		*/
		operator VkShaderModule const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkShaderModule m_internal{};
	};
}

#endif
