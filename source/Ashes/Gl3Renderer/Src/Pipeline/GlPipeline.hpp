/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Pipeline_HPP___
#define ___GlRenderer_Pipeline_HPP___
#pragma once

#include "Buffer/GlGeometryBuffers.hpp"
#include "Shader/GlShaderProgram.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/ColourBlendState.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/RasterisationState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/ShaderStageState.hpp>
#include <Pipeline/TessellationState.hpp>
#include <Pipeline/Viewport.hpp>

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
			, ashes::GraphicsPipelineCreateInfo && createInfo );
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
			return (bool)m_viewport;
		}
		/**
		*\return
		*	\p true si le Scissor est défini.
		*/
		inline bool hasScissor()const
		{
			return (bool)m_scissor;
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
		inline ashes::Viewport const & getViewport()const
		{
			assert( m_viewport );
			return *m_viewport;
		}
		/**
		*\return
		*	Le Scissor.
		*/
		inline ashes::Scissor const & getScissor()const
		{
			assert( m_scissor );
			return *m_scissor;
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
		inline bool hasDynamicState( ashes::DynamicState state )const
		{
			return m_createInfo.dynamicStates.end() != std::find( m_createInfo.dynamicStates.begin()
				, m_createInfo.dynamicStates.end()
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
		ashes::Optional< ashes::Viewport > m_viewport;
		ashes::Optional< ashes::Scissor > m_scissor;
		PushConstantsDesc m_constantsPcb;
		ShaderProgram m_program;
		mutable std::vector< std::pair< size_t, GeometryBuffersPtr > > m_geometryBuffers;
		mutable std::unordered_map< GLuint, BufferDestroyConnection > m_connections;
		size_t m_vertexInputStateHash;
	};
}

#endif
