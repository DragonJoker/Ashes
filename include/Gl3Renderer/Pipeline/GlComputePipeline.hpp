/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_ComputePipeline_HPP___
#define ___GlRenderer_ComputePipeline_HPP___
#pragma once

#include "Gl3Renderer/Shader/GlShaderProgram.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Pipeline/ComputePipeline.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class ComputePipeline
		: public ashes::ComputePipeline
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] program
		*	Le programme shader.
		*/
		ComputePipeline( Device const & device
			, ashes::PipelineLayout const & layout
			, ashes::ComputePipelineCreateInfo createInfo );
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		/**
		*\return
		*	Le PipelineLayout.
		*/
		inline ashes::PipelineLayout const & getLayout()const
		{
			return m_layout;
		}
		/**
		*\return
		*	Le ShaderProgram.
		*/
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
