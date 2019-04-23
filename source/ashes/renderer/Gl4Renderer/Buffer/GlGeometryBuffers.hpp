/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_GeometryBuffers_HPP___
#define ___GlRenderer_GeometryBuffers_HPP___
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class GeometryBuffers
	{
	public:
		struct VBO
		{
			VBO( VkBuffer vbo
				, VkDeviceSize offset
				, VkVertexInputBindingDescription binding
				, VkVertexInputAttributeDescriptionArray attributes )
				: vbo{ vbo }
				, offset{ offset }
				, binding{ binding }
				, attributes{ attributes }
			{
			}

			VkBuffer vbo;
			VkDeviceSize offset;
			VkVertexInputBindingDescription binding;
			VkVertexInputAttributeDescriptionArray attributes;
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
			, VkIndexType type );
		~GeometryBuffers()noexcept;

		void initialise();

		static std::vector< VBO > createVBOs( VboBindings const & vbos
			, VkPipelineVertexInputStateCreateInfo const & vertexInputState );

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
		VkDevice m_device;
		std::vector< VBO > m_vbos;
		std::unique_ptr< IBO > m_ibo;
		GLuint m_vao{ GL_INVALID_INDEX };
	};
}

#endif
