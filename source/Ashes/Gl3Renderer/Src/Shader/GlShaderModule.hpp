/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Shader/ShaderModule.hpp>

namespace gl_renderer
{
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

		inline GLuint getShader()const
		{
			return m_shader;
		}

		inline bool isSpirV()const
		{
			return m_isSpirV;
		}

	private:
		Device const & m_device;
		GLuint m_shader;
		bool m_isSpirV;
	};
}
