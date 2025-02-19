/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_GeometryBuffers_HPP___
#define ___GlRenderer_GeometryBuffers_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl
{
	class GeometryBuffers
	{
	public:
		struct VBO
		{
			VBO( VkBuffer vbo
				, VkDeviceSize offset
				, VkVertexInputBindingDescription binding
				, VkVertexInputAttributeDescriptionArray attributes
				, VkVertexInputAttributeDescriptionArray programAttributes )
				: vbo{ vbo }
				, offset{ offset }
				, binding{ binding }
				, attributes{ std::move( attributes ) }
				, programAttributes{ std::move( programAttributes ) }
			{
			}

			VkBuffer vbo;
			VkDeviceSize offset;
			VkVertexInputBindingDescription binding;
			VkVertexInputAttributeDescriptionArray attributes;
			VkVertexInputAttributeDescriptionArray programAttributes;
		};

		struct IBO
		{
			IBO( VkBuffer ibo
				, VkDeviceSize offset
				, VkIndexType type )
				: ibo{ ibo }
				, offset{ offset }
				, type{ type }
			{
			}

			VkBuffer ibo;
			uint64_t offset;
			VkIndexType type;
		};

	public:
		GeometryBuffers( VkDevice device
			, VboBindings const & vbos
			, IboBinding const & ibo
			, VkPipelineVertexInputStateCreateInfo const & vertexInputState
			, InputsLayout const & inputLayout
			, VkIndexType type );
		~GeometryBuffers()noexcept;

		void initialise( ContextLock const & context );

		static std::vector< VBO > createVBOs( VboBindings const & vbos
			, VkPipelineVertexInputStateCreateInfo const & vertexInputState
			, InputsLayout const & inputLayout );

		inline GLuint getVao()const
		{
			return m_vao;
		}

		inline std::vector< VBO > const & getVbos()const
		{
			return m_vbos;
		}

		inline bool hasIbo()const
		{
			return m_ibo != nullptr;
		}

		inline IBO const & getIbo()const
		{
			assert( m_ibo != nullptr );
			return *m_ibo;
		}

	private:
		void enableAttribute( ContextLock const & context
			, VkVertexInputBindingDescription const & binding
			, VkVertexInputAttributeDescription const & attribute
			, VkDeviceSize offset );

	private:
		VkDevice m_device;
		std::vector< VBO > m_vbos;
		std::unique_ptr< IBO > m_ibo;
		GLuint m_vao{ GL_INVALID_INDEX };
	};
}

#endif
