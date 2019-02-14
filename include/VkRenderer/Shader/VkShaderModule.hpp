/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Shader/ShaderModule.hpp>

namespace vk_renderer
{
	/**
	*\~french
	*\brief
	*	Classe encapsulant un VkShaderModule.
	*\~english
	*\brief
	*	VkShaderModule wrapper.
	*/
	class ShaderModule
		: public ashes::ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, ashes::ShaderStageFlag stage );
		~ShaderModule();
		/**
		*\~copydoc	ashes::ShaderModule::loadShader
		*/
		void loadShader( ashes::UInt32Array const & shader )override;
		/**
		*\~french
		*\brief
		*	Le niveau de shader utilisé pour le module.
		*\~english
		*\brief
		*	The stage used for the module.
		*/
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
		inline operator VkShaderModule const &()const
		{
			return m_shader;
		}

	private:
		void doLoadShader( uint32_t const * const begin
			, uint32_t size );

	private:
		Device const & m_device;
		VkShaderStageFlagBits m_stage;
		VkShaderModule m_shader{ VK_NULL_HANDLE };
	};
}
