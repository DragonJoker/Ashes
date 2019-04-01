/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Shader/ShaderModule.hpp>

namespace gl_renderer
{
	class ShaderModule
	{
	public:
		ShaderModule( Device const & device
			, VkShaderModuleCreateInfo createInfo );
		~ShaderModule();
		void compile( VkShaderStageFlagBits stage
			, ashes::ShaderStageState const & state )const;

		inline GLuint getInternal()const
		{
			return m_internal;
		}

		VkShaderModuleCreateInfo createInfo;

	private:
		Device const & m_device;
		mutable GLuint m_internal;
		ashes::UInt32Array m_spv;
		mutable std::string m_source;
	};
}
