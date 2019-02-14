/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Shader/ShaderModule.hpp>

namespace gl_renderer
{
	class ShaderModule
		: public ashes::ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, ashes::ShaderStageFlag stage );
		~ShaderModule();
		void compile( ashes::ShaderStageState const & state )const;
		/**
		*\~copydoc	ashes::ShaderModule::loadShader
		*/
		void loadShader( ashes::UInt32Array const & shader )override;

		inline GLuint getShader()const
		{
			return m_shader;
		}

	private:
		Device const & m_device;
		GLuint m_shader;
		ashes::UInt32Array m_spv;
		mutable std::string m_source;
	};
}
