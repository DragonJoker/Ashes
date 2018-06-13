/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Shader/ShaderModule.hpp>

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
		: public renderer::ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, renderer::ShaderStageFlag stage );
		~ShaderModule();
		/**
		*\~copydoc	renderer::ShaderModule::loadShader
		*/
		void loadShader( std::string const & shader )override;
		/**
		*\~copydoc	renderer::ShaderModule::loadShader
		*/
		void loadShader( renderer::ByteArray const & shader )override;
		/**
		*\~copydoc	renderer::ShaderModule::loadShader
		*/
		void loadShader( renderer::UInt32Array const & shader );

	private:
		Device const & m_device;
	};
}
