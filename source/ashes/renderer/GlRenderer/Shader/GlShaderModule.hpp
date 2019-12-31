/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl
{
	class ShaderModule
		: public AutoIdIcdObject< ShaderModule >
	{
	public:
		ShaderModule( VkDevice device
			, VkShaderModuleCreateInfo createInfo );

		ShaderDesc compile( VkPipelineShaderStageCreateInfo const & state
			, VkPipelineLayout pipelineLayout
			, VkPipelineCreateFlags createFlags
			, bool invertY );

		inline ConstantsLayout const & getConstants()const
		{
			return m_constants;
		}

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkShaderModuleCreateFlags m_flags;
		UInt32Array m_code;
		mutable std::string m_source;
		mutable ConstantsLayout m_constants;
	};
}
