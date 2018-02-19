/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_GeometryBuffers_HPP___
#define ___Renderer_GeometryBuffers_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <vector>

namespace renderer
{
	/**
	*\~english
	*\brief
	*	Class wrapping the VBOs and the IBO of a mesh.
	*\~french
	*\brief
	*	Classe encapsulant les VBOs et l'IBO d'un mesh.
	*/
	class GeometryBuffers
	{
	public:
		struct VBO
		{
			VertexBufferBase const & vbo;
			uint64_t offset;
			VertexLayout const & layout;
		};

		struct IBO
		{
			IBO( BufferBase const & buffer
				, uint64_t offset
				, IndexType type )
				: buffer{ buffer }
				, offset{ offset }
				, type{ type }
			{
			}

			BufferBase const & buffer;
			uint64_t offset;
			IndexType type;
		};

	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] vbos
		*	The VBOs.
		*\param[in] vboOffsets
		*	The offset for the first vertex of each VBO.
		*\param[in] layouts
		*	The layouts, one per VBO in \p vbos.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] vbos
		*	Les VBOs.
		*\param[in] vboOffsets
		*	L'offset du premier sommet pour chaque VBO.
		*\param[in] layouts
		*	Les layouts, un par vbo de \p vbos.
		*/
		GeometryBuffers( VertexBufferCRefArray const & vbos
			, std::vector< uint64_t > vboOffsets
			, VertexLayoutCRefArray const & layouts );
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] vbos
		*	The VBOs.
		*\param[in] vboOffsets
		*	The offset for the first vertex of each VBO.
		*\param[in] layouts
		*	The layouts, one per VBO in \p vbos.
		*\param[in] ibo
		*	The IBO.
		*\param[in] iboOffset
		*	The offset of the first index in the IBO.
		*\param[in] type
		*	The indices type.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] vbos
		*	Les VBOs.
		*\param[in] vboOffsets
		*	L'offset du premier sommet pour chaque VBO.
		*\param[in] layouts
		*	Les layouts, un par vbo de \p vbos.
		*\param[in] ibo
		*	L'IBO.
		*\param[in] iboOffset
		*	L'offset du premier sommet dans l'IBO.
		*\param[in] type
		*	Le type des indices.
		*/
		GeometryBuffers( VertexBufferCRefArray const & vbos
			, std::vector< uint64_t > vboOffsets
			, VertexLayoutCRefArray const & layouts
			, BufferBase const & ibo
			, uint64_t iboOffset
			, IndexType type );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~GeometryBuffers() = default;
		/**
		*\~english
		*\return
		*	The VBOs.
		*\~french
		*\return
		*	Les VBOs.
		*/
		inline std::vector< VBO > const & getVbos()const
		{
			return m_vbos;
		}
		/**
		*\~english
		*\return
		*	\p true if the IBO is defined.
		*\~french
		*\return
		*	\p true si l'IBO est défini.
		*/
		inline bool hasIbo()const
		{
			return m_ibo != nullptr;
		}
		/**
		*\~english
		*\return
		*	The IBO.
		*\~french
		*\return
		*	L'IBO.
		*/
		inline IBO const & getIbo()const
		{
		    assert( m_ibo != nullptr );
			return *m_ibo;
		}

	protected:
		std::vector< VBO > m_vbos;
		std::unique_ptr< IBO > m_ibo;
	};
}

#endif
