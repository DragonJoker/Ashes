/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "Gl4Renderer/Buffer/GlGeometryBuffers.hpp"
#include "Gl4Renderer/Shader/GlShaderDesc.hpp"
#include "Gl4Renderer/Shader/GlShaderProgram.hpp"

#include <Ashes/Buffer/PushConstantsBuffer.hpp>
#include <Ashes/Pipeline/Pipeline.hpp>
#include <Ashes/Pipeline/ColourBlendState.hpp>
#include <Ashes/Pipeline/DepthStencilState.hpp>
#include <Ashes/Pipeline/InputAssemblyState.hpp>
#include <Ashes/Pipeline/MultisampleState.hpp>
#include <Ashes/Pipeline/RasterisationState.hpp>
#include <Ashes/Pipeline/Scissor.hpp>
#include <Ashes/Pipeline/ShaderStageState.hpp>
#include <Ashes/Pipeline/TessellationState.hpp>
#include <Ashes/Pipeline/Viewport.hpp>

#include <algorithm>
#include <unordered_map>

namespace gl_renderer
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class Pipeline
		: public ashes::Pipeline
	{
	public:
		/**
		*name
		*	Construction / Destruction.
		*/
		/**@{*/
		Pipeline( Device const & device
			, PipelineLayout const & layout
			, ashes::GraphicsPipelineCreateInfo createInfo );
		GeometryBuffers * findGeometryBuffers( VboBindings const & vbos
			, IboBinding const & ibo )const;
		GeometryBuffersRef createGeometryBuffers( VboBindings vbos
			, IboBinding const & ibo
			, ashes::IndexType type )const;
		~Pipeline();
		PushConstantsDesc findPushConstantBuffer( PushConstantsDesc const & pushConstants )const;
		/**@}*/
		/**
		*\return
		*	\p true si le Viewport est défini.
		*/
		inline bool hasViewport()const
		{
			return !m_viewports.empty();
		}
		/**
		*\return
		*	\p true si le Scissor est défini.
		*/
		inline bool hasScissor()const
		{
			return !m_scissors.empty();
		}
		/**
		*\return
		*	Le ShaderStageState.
		*/
		inline std::vector< ashes::ShaderStageState > const & getShaderStageState()const
		{
			return m_ssState;
		}
		/**
		*\return
		*	Le InputAssemblyState.
		*/
		inline ashes::InputAssemblyState const & getInputAssemblyState()const
		{
			return m_iaState;
		}
		/**
		*\return
		*	Le ColourBlendState.
		*/
		inline ashes::ColourBlendState const & getColourBlendState()const
		{
			return m_cbState;
		}
		/**
		*\return
		*	Le RasterisationState.
		*/
		inline ashes::RasterisationState const & getRasterisationState()const
		{
			return m_rsState;
		}
		/**
		*\return
		*	Le DepthStencilState.
		*/
		inline ashes::DepthStencilState const & getDepthStencilState()const
		{
			return m_dsState;
		}
		/**
		*\return
		*	Le MultisampleState.
		*/
		inline ashes::MultisampleState const & getMultisampleState()const
		{
			return m_msState;
		}
		/**
		*\return
		*	Le TessellationState.
		*/
		inline ashes::TessellationState const & getTessellationState()const
		{
			return m_tsState;
		}
		/**
		*\return
		*	Le VertexInputState.
		*/
		inline ashes::VertexInputState const & getVertexInputState()const
		{
			return m_vertexInputState;
		}
		/**
		*\return
		*	Le Viewport.
		*/
		inline ashes::ViewportArray const & getViewports()const
		{
			assert( !m_viewports.empty() );
			return m_viewports;
		}
		/**
		*\return
		*	Le Scissor.
		*/
		inline ashes::ScissorArray const & getScissors()const
		{
			assert( !m_scissors.empty() );
			return m_scissors;
		}
		/**
		*\return
		*	Le PipelineLayout.
		*/
		inline PipelineLayout const & getLayout()const
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
		/**
		*\return
		*	Dit si le pipeline a des sommets.
		*/
		inline bool hasVertexLayout()const
		{
			return !m_vertexInputState.vertexBindingDescriptions.empty()
				&& !m_vertexInputState.vertexAttributeDescriptions.empty();
		}
		/**
		*\return
		*	Le hash du VertexInputState.
		*/
		inline size_t getVertexInputStateHash()const
		{
			return m_vertexInputStateHash;
		}
		/**
		*\return
		*	\p true si l'état dynamique est dans la liste d'états dynamiques.
		*/
		inline bool hasDynamicStateEnable( ashes::DynamicStateEnable state )const
		{
			return bool( m_createInfo.dynamicState )
				&& m_createInfo.dynamicState.value().dynamicStates.end() != std::find( m_createInfo.dynamicState.value().dynamicStates.begin()
					, m_createInfo.dynamicState.value().dynamicStates.end()
					, state );
		}

	private:
		Device const & m_device;
		PipelineLayout const & m_layout;
		std::vector< ashes::ShaderStageState > m_ssState;
		ashes::VertexInputState m_vertexInputState;
		ashes::RenderPass const & m_renderPass;
		ashes::InputAssemblyState m_iaState;
		ashes::ColourBlendState m_cbState;
		ashes::RasterisationState m_rsState;
		ashes::DepthStencilState m_dsState;
		ashes::MultisampleState m_msState;
		ashes::TessellationState m_tsState;
		ashes::ViewportArray m_viewports;
		ashes::ScissorArray m_scissors;
		PushConstantsDesc m_constantsPcb;
		ShaderProgram m_program;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, BufferDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif
