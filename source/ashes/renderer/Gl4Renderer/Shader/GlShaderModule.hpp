/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	class ShaderModule
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );

		GLuint compile( VkPipelineShaderStageCreateInfo const & state
			, bool isRtot )const;

		inline ConstantsLayout const & getConstants()const
		{
			return m_constants;
		}

	private:
		VkDevice m_device;
		VkShaderModuleCreateFlags m_flags;
		UInt32Array m_code;
		mutable std::string m_source;
		mutable ConstantsLayout m_constants;
	};
}
