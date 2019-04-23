/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Shader/ShaderModule.hpp>

namespace test_renderer
{
	/**
	*\~french
	*\brief
	*	Classe encapsulant un TestShaderModule.
	*\~english
	*\brief
	*	TestShaderModule wrapper.
	*/
	class ShaderModule
		: public ashes::ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, VkShaderStageFlagBits stage );
		~ShaderModule();
		/**
		*\~copydoc	ashes::ShaderModule::loadShader
		*/
		void loadShader( ashes::UInt32Array const & shader )override;

	private:
		Device const & m_device;
	};
}
