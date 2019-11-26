/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class BindPipelineCommand
		: public CommandBase
	{
	public:
		BindPipelineCommand( VkDevice device
			, VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;
		VkPipelineBindPoint m_bindingPoint;
		bool m_dynamicBlendFactor;
		bool m_dynamicLineWidth;
		bool m_dynamicDepthBias;
		bool m_dynamicStencil;
		bool m_dynamicScissor;
		bool m_dynamicViewport;
	};

	class BindDepthStencilStateCommand
		: public CommandBase
	{
	public:
		BindDepthStencilStateCommand( VkDevice device
			, VkPipeline pipeline );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkPipeline m_pipeline;
	};

	class BindRasterizerStateCommand
		: public CommandBase
	{
	public:
		BindRasterizerStateCommand( VkDevice device
			, VkPipeline pipeline );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkPipeline m_pipeline;
	};

	class BindBlendStateCommand
		: public CommandBase
	{
	public:
		BindBlendStateCommand( VkDevice device
			, VkPipeline pipeline
			, std::array< float, 4u > const & blendConstants );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkPipeline m_pipeline;
		std::array< float, 4u > m_blendConstants;
	};

	class SetDepthBoundsCommand
		: public CommandBase
	{
	public:
		SetDepthBoundsCommand( VkDevice device
			, DepthBounds const & depthBounds );

		void apply( Context const & context )const;
		void remove( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkPhysicalDevice m_physicalDevice;
		DepthBounds m_depthBounds;
	};
}
