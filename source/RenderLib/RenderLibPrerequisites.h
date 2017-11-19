/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderLibPrerequisites_HPP___
#define ___RenderLib_RenderLibPrerequisites_HPP___
#pragma once

#include <GlLib/GlArrayUniform.h>
#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlDebug.h>
#include <GlLib/GlFrameBuffer.h>
#include <GlLib/GlRenderBuffer.h>
#include <GlLib/GlSampler.h>
#include <GlLib/GlShaderProgram.h>
#include <GlLib/GlTexture.h>
#include <GlLib/GlUniform.h>
#include <GlLib/OpenGL.h>
#include <GlLib/Quaternion.h>

#include <time.h>
#include <functional>
#include <iomanip>
#include <sstream>

namespace render
{
	/**
	*\name Typedefs généralistes.
	*/
	/**@{*/
	using ByteArray = gl::ByteArray;
	using UInt16Array = gl::UInt16Array;
	using Vec3Array = gl::Vec3Array;
	using Vec2Array = gl::Vec2Array;
	/**@}*/
	/**
	*\name Prédéclarations.
	*/
	/**@{*/
	class BillboardAttributes;
	class BillboardBuffer;
	class Billboard;
	class BorderPanelOverlay;
	class Camera;
	class CameraState;
	class Font;
	class FontLoader;
	class FontTexture;
	class Glyph;
	class Material;
	class Mesh;
	class Movable;
	class Object;
	class Overlay;
	class OverlayRenderer;
	class PanelOverlay;
	class PolyLine;
	class RenderTarget;
	class RenderWindow;
	class Scene;
	class Submesh;
	class TextOverlay;
	class Texture;
	class UniformBuffer;
	class UniformBufferBinding;
	class Viewport;
	struct RenderSubmesh;

	template< typename T >
	class ElementsList;
	template< typename Signal >
	class Connection;
	template< typename Function >
	class Signal;
	/**@}*/
	/**
	*\name Définitions diverses depuis les prédéclarations.
	*/
	/**@{*/
	using BillboardPtr = std::shared_ptr< Billboard >;
	using BillboardAttributesPtr = std::unique_ptr< BillboardAttributes >;
	using BillboardBufferPtr = std::shared_ptr< BillboardBuffer >;
	using FontPtr = std::unique_ptr< Font >;
	using FontLoaderPtr = std::unique_ptr< FontLoader >;
	using FontTexturePtr = std::unique_ptr< FontTexture >;
	using MaterialPtr = std::shared_ptr< Material >;
	using MeshPtr = std::shared_ptr< Mesh >;
	using ObjectPtr = std::shared_ptr< Object >;
	using OverlayRendererPtr = std::unique_ptr< OverlayRenderer >;
	using PolyLinePtr = std::shared_ptr< PolyLine >;
	using RenderWindowPtr = std::unique_ptr< RenderWindow >;
	using SubmeshPtr = std::shared_ptr< Submesh >;
	using BorderPanelOverlayPtr = std::shared_ptr< BorderPanelOverlay >;
	using TextOverlayPtr = std::shared_ptr< TextOverlay >;
	using OverlayPtr = std::shared_ptr< Overlay >;
	using TexturePtr = std::shared_ptr< Texture >;

	using MaterialArray = std::vector< MaterialPtr >;
	using MeshArray = std::vector< MeshPtr >;
	using ObjectArray = std::vector< ObjectPtr >;
	using PolyLineArray = std::vector< PolyLinePtr >;
	using SubmeshArray = std::vector< SubmeshPtr >;
	using BillboardArray = std::vector< BillboardPtr >;

	using MaterialList = ElementsList< Material >;
	using TextureList = ElementsList< Texture >;
	using BillboardList = ElementsList< BillboardBuffer >;
	using MeshList = ElementsList< Mesh >;
	using OverlayList = ElementsList< Overlay >;
	/**@}*/
	/**
	*\name Définitions des signaux.
	*/
	/**@{*/
	using OnMovableChanged = Signal< std::function< void( Movable & ) > >;
	using OnBillboardChanged = Signal< std::function< void( Billboard & ) > >;
	using OnBillboardBufferChanged = Signal< std::function< void( BillboardBuffer & ) > >;
	using OnPolyLineChanged = Signal< std::function< void( PolyLine & ) > >;
	using OnObjectPicked = Signal< std::function< void( Object & ) > >;
	using OnBillboardPicked = Signal< std::function< void( Billboard &, uint32_t ) > >;
	using OnUnpick = Signal< std::function< void() > >;
	/**@}*/
	/**
	*\brief
	*	Les types de noeuds transparents.
	*/
	enum class TransparentNodeType
	{
		//! Noeud sans texture.
		eNoTex,
		//! Noeud avec texture de diffuse.
		eDiff,
		//! Noeud avec texture d'opacité.
		eOpa,
		//! Noeud avec textures d'opacité et de diffuse.
		eOpaDiff,
		GlLib_EnumBounds( eNoTex )
	};
	/**
	*\brief
	*	Le maillage et le matériau d'un objet, dans la scène.
	*/
	struct RenderSubmesh
	{
		//! Le maillage.
		MeshPtr m_mesh;
		//! Le sous-maillage.
		SubmeshPtr m_submesh;
		//! Le matériau.
		MaterialPtr m_material;
		//! L'objet parent.
		ObjectPtr m_object;
	};
	/**
	*\brief
	*	Les types de noeuds.
	*/
	enum class NodeType
	{
		//! Noeud opaque.
		eOpaque,
		//! Noeud opaque sans texture.
		eOpaqueNoTex = eOpaque,
		//! Noeud opaque avec une texture de diffuse.
		eOpaqueDiff,
		//! Noeud avec alpha blending.
		eAlphaBlend,
		//! Noeud avec alpha blending, sans texture.
		eAlphaBlendNoTex = eAlphaBlend,
		//! Noeud avec alpha blending, avec texture de diffuse.
		eAlphaBlendDiff,
		//! Noeud avec alpha blending, avec texture d'opacité.
		eAlphaBlendOpa,
		//! Noeud avec alpha blending, avec textures d'opacité et de diffuse.
		eAlphaBlendOpaDiff,
		//! Noeud avec alpha testing.
		eAlphaTest,
		//! Noeud avec alpha testing, sans texture.
		eAlphaTestNoTex = eAlphaTest,
		//! Noeud avec alpha testing, avec texture de diffuse.
		eAlphaTestDiff,
		//! Noeud avec alpha testing, avec texture d'opacité.
		eAlphaTestOpa,
		//! Noeud avec alpha testing, avec textures d'opacité et de diffuse.
		eAlphaTestOpaDiff,
		GlLib_EnumBounds( eOpaque )
	};
	using RenderSubmeshVector = std::vector< RenderSubmesh >;
	using RenderSubmeshArray = std::array< RenderSubmeshVector, size_t( NodeType::eCount ) >;
	using RenderBillboardArray = std::array< BillboardArray, size_t( NodeType::eCount ) >;
	using RenderPolyLineArray = std::array< PolyLineArray, size_t( NodeType::eCount ) >;
	/**
	*\brief
	*	Charge une texture depuis le contenu donné.
	*\param[in] fileContent
	*	Le contenu du fichier de l'image.
	*\param[out] texture
	*	Reçoit la texture.
	*/
	void loadTexture( ByteArray const & fileContent
		, Texture & texture );
	/**
	*\brief
	*	Charge une texture depuis le contenu donné.
	*\param[in] fileContent
	*	Le contenu du fichier de l'image.
	*\param[in] format
	*	Le format des pixels voulu pour la texture.
	*\param[out] texture
	*	Reçoit la texture.
	*/
	void loadTexture( ByteArray const & fileContent
		, gl::PixelFormat format
		, Texture & texture );
	/**
	*\brief
	*	Charge une police depuis le contenu donné.
	*\param[in] content
	*	Le contenu du fichier de description la police.
	*\param[out] font
	*	Reçoit la police.
	*/
	void loadFont( std::string const & content
		, Font & font );
	/**
	*\brief
	*	Charge une police depuis le loader donné.
	*\param[in] loader
	*	Le loader.
	*\param[out] font
	*	Reçoit la police.
	*/
	void loadFont( FontLoader & loader
		, Font & font );
	/**
	*\name Opérateurs de flux.
	*/
	/**@{*/
	inline std::ostream & operator<<( std::ostream & stream, gl::Vec2 const & value )
	{
		stream << value.x << ", " << value.y;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, gl::Vec3 const & value )
	{
		stream << value.x << ", " << value.y << ", " << value.z;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, gl::Vec4 const & value )
	{
		stream << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, gl::Quaternion const & value )
	{
		stream << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, gl::Mat4 const & value )
	{
		stream << value[0].x << ", " << value[0].y << ", " << value[0].z << ", " << value[0].w << "\n";
		stream << value[1].x << ", " << value[1].y << ", " << value[1].z << ", " << value[1].w << "\n";
		stream << value[2].x << ", " << value[2].y << ", " << value[2].z << ", " << value[2].w << "\n";
		stream << value[3].x << ", " << value[3].y << ", " << value[3].z << ", " << value[3].w << "\n";
		return stream;
	}
	/**@}*/
}

#endif
