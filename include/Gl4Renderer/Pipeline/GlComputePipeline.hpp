/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_ComputePipeline_HPP___
#define ___GlRenderer_ComputePipeline_HPP___
#pragma once

#include "Gl4Renderer/Shader/GlShaderProgram.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Pipeline/ComputePipeline.hpp>

namespace gl_renderer
{
	class ComputePipeline
		: public ashes::ComputePipeline
	{
	public:
		ComputePipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::ComputePipelineCreateInfo && createInfo );
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;

		inline ashes::PipelineLayout const & getLayout()const
		{
			return m_layout;
		}

		inline GLuint getProgram()const
		{
			return m_program.getProgram();
		}

	private:
		Device const & m_device;
		ashes::PipelineLayout const & m_layout;
		ShaderProgram m_program;
		PushConstantsDesc m_constantsPcb;
	};
}

#endif
