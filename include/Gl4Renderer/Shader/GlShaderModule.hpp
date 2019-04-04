/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class ShaderModule
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );
		~ShaderModule();
		void compile( VkPipelineShaderStageCreateInfo const & state )const;

		inline GLuint getInternal()const
		{
			return m_internal;
		}

	private:
		VkDevice m_device;
		mutable GLuint m_internal;
		VkShaderModuleCreateFlags m_flags;
		UInt32Array m_code;
		mutable std::string m_source;
	};
}
