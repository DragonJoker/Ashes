/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_GeometryBuffers_HPP___
#define ___GlRenderer_GeometryBuffers_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Pipeline/VertexInputAttributeDescription.hpp>
#include <Ashes/Pipeline/VertexInputBindingDescription.hpp>
#include <Ashes/Pipeline/VertexInputState.hpp>

namespace gl_renderer
{
	class GeometryBuffers
	{
	public:
		struct VBO
		{
			VBO( GLuint vbo
				, uint64_t offset
				, ashes::VertexInputBindingDescription binding
				, ashes::VertexInputAttributeDescriptionArray attributes )
				: vbo{ vbo }
				, offset{ offset }
				, binding{ binding }
				, attributes{ attributes }
			{
			}

			GLuint vbo;
			uint64_t offset;
			ashes::VertexInputBindingDescription binding;
			ashes::VertexInputAttributeDescriptionArray attributes;
		};

		struct IBO
		{
			IBO( GLuint ibo
				, uint64_t offset
				, ashes::IndexType type )
				: ibo{ ibo }
				, offset{ offset }
				, type{ type }
			{
			}

			GLuint ibo;
			uint64_t offset;
			ashes::IndexType type;
		};

	public:
		GeometryBuffers( Device const & device
			, VboBindings const & vbos
			, IboBinding const & ibo
			, ashes::VertexInputState const & vertexInputState
			, ashes::IndexType type );
		~GeometryBuffers()noexcept;

		void initialise();

		static std::vector< VBO > createVBOs( VboBindings const & vbos
			, ashes::VertexInputState const & vertexInputState );

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
		Device const & m_device;
		std::vector< VBO > m_vbos;
		std::unique_ptr< IBO > m_ibo;
		GLuint m_vao{ GL_INVALID_INDEX };
	};
}

#endif
