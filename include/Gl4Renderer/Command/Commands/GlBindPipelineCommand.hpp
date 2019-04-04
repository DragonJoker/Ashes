/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineColorBlendStateCreateInfo const & state );
	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineRasterizationStateCreateInfo const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias );
	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineMultisampleStateCreateInfo const & state );
	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineDepthStencilStateCreateInfo const & state );
	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineTessellationStateCreateInfo const & state );
	/**
	*\brief
	*	Commande d'activation d'un pipeline: shaders, tests, �tats, ...
	*/
	class BindPipelineCommand
		: public CommandBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] pipeline
		*	Le pipeline � activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		BindPipelineCommand( VkDevice device
			, VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint );

		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;
		GLuint m_program;
		VkPipelineBindPoint m_bindingPoint;
		bool m_dynamicLineWidth;
		bool m_dynamicDepthBias;
		bool m_dynamicScissor;
		bool m_dynamicViewport;
	};
}
