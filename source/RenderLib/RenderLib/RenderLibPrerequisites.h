/**
*\file
*	RenderLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderLibPrerequisites_HPP___
#define ___RenderLib_RenderLibPrerequisites_HPP___
#pragma once

#include <Renderer/RendererPrerequisites.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/GeometryBuffers.hpp>
#include <Renderer/VertexLayout.hpp>

#include <Utils/Factory.hpp>
#include <Utils/Quaternion.hpp>
#include <Utils/UtilsSignal.hpp>

#include <time.h>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>

namespace render
{
	static uint32_t constexpr MaxObjectsCount = 100u;
	using RendererFactory = utils::Factory< renderer::Renderer, std::string, renderer::RendererPtr >;

	//! Les données de l'UBO contenant les matrices.
	struct MatrixUbo
	{
		//! La variable uniforme contenant la matrice de projection.
		renderer::Mat4 projection;
		//! La variable uniforme contenant la matrice de vue.
		renderer::Mat4 view;
		//! La variable uniforme contenant la matrice du modèle.
		renderer::Mat4 model;
	};
	//! Les données de l'UBO contenant les informations du matériau.
	struct MaterialUbo
	{
		//! La variable uniforme contenant la couleur ambiante.
		renderer::Vec3 ambient;
		//! La variable uniforme contenant la couleur diffuse.
		renderer::Vec3 diffuse;
		//! La variable uniforme contenant la couleur spéculaire.
		renderer::Vec3 specular;
		//! La variable uniforme contenant la couleur émissive.
		renderer::Vec3 emissive;
		//! La variable uniforme contenant l'exposant spéculaire.
		float exponent;
		//! La variable uniforme contenant l'opacité
		float opacity;
	};
	//! Les données de l'UBO contenant les variables liées au billboard.
	struct BillboardUbo
	{
		//! La variable uniforme contenant les dimensions du billboard.
		renderer::Vec2 dimensions;
		//! La variable uniforme contenant la position de la caméra.
		renderer::Vec3 camera;
	};
	//! Les données de l'UBO contenant les variables liées à la ligne.
	struct LineUbo
	{
		//! La variable uniforme contenant la largeur de la ligne.
		float lineWidth;
		//! La variable uniforme contenant la plume.
		float lineFeather;
		//! La variable uniforme contenant l'échelle.
		float lineScale;
		//! La variable uniforme contenant la position de la caméra.
		renderer::Vec3 camera;
	};
	/**
	*\name Typedefs généralistes.
	*/
	/**\{*/
	using ByteArray = renderer::ByteArray;
	using UInt16Array = renderer::UInt16Array;
	using Vec3Array = renderer::Vec3Array;
	using Vec2Array = renderer::Vec2Array;
	/**\}*/
	/**
	*\name Prédéclarations.
	*/
	/**\{*/
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
	class Viewport;
	struct RenderSubmesh;

	template< typename T >
	class ElementsList;
	/**\}*/
	/**
	*\name Définitions diverses depuis les prédéclarations.
	*/
	/**\{*/
	using BillboardAttributesPtr = std::unique_ptr< BillboardAttributes >;
	using FontLoaderPtr = std::unique_ptr< FontLoader >;
	using FontPtr = std::unique_ptr< Font >;
	using FontTexturePtr = std::unique_ptr< FontTexture >;
	using OverlayRendererPtr = std::unique_ptr< OverlayRenderer >;
	using RenderTargetPtr = std::unique_ptr< RenderTarget >;
	using RenderWindowPtr = std::unique_ptr< RenderWindow >;

	using BillboardPtr = std::shared_ptr< Billboard >;
	using BillboardBufferPtr = std::shared_ptr< BillboardBuffer >;
	using BorderPanelOverlayPtr = std::shared_ptr< BorderPanelOverlay >;
	using MaterialPtr = std::shared_ptr< Material >;
	using MeshPtr = std::shared_ptr< Mesh >;
	using ObjectPtr = std::shared_ptr< Object >;
	using OverlayPtr = std::shared_ptr< Overlay >;
	using PanelOverlayPtr = std::shared_ptr< PanelOverlay >;
	using PolyLinePtr = std::shared_ptr< PolyLine >;
	using SubmeshPtr = std::shared_ptr< Submesh >;
	using TextOverlayPtr = std::shared_ptr< TextOverlay >;
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
	/**\}*/
	/**
	*\name Définitions des signaux.
	*/
	/**\{*/
	using OnMovableChanged = utils::Signal< std::function< void( Movable & ) > >;
	using OnBillboardChanged = utils::Signal< std::function< void( Billboard & ) > >;
	using OnBillboardBufferChanged = utils::Signal< std::function< void( BillboardBuffer & ) > >;
	using OnPolyLineChanged = utils::Signal< std::function< void( PolyLine & ) > >;
	using OnOverlayChanged = utils::Signal< std::function< void( Overlay & ) > >;
	using OnObjectPicked = utils::Signal< std::function< void( Object & ) > >;
	using OnBillboardPicked = utils::Signal< std::function< void( Billboard &, uint32_t ) > >;
	using OnUnpick = utils::Signal< std::function< void() > >;
	/**\}*/
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
		Utils_EnumBounds( eNoTex )
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
		Utils_EnumBounds( eOpaque )
	};
	/**
	*\brief
	*	Le maillage et le matériau d'un objet, dans la scène.
	*/
	struct RenderSubmesh
	{
		RenderSubmesh( renderer::Device const & device
			, renderer::DescriptorSetPool const & pool
			, MeshPtr mesh
			, SubmeshPtr submesh
			, MaterialPtr material
			, ObjectPtr object
			, uint32_t index
			, renderer::UniformBuffer< MatrixUbo > const & mtxUbo
			, renderer::UniformBuffer< MaterialUbo > const & matUbo
			, renderer::VertexLayout const & posLayout
			, renderer::VertexLayoutPtr const & nmlLayout
			, renderer::VertexLayoutPtr const & texLayout );
		//! Le maillage.
		MeshPtr m_mesh;
		//! Le sous-maillage.
		SubmeshPtr m_submesh;
		//! Le matériau.
		MaterialPtr m_material;
		//! L'objet parent.
		ObjectPtr m_object;
		//! Le descriptor set.
		renderer::DescriptorSetPtr m_descriptor;
		//! Le tampon de géométries.
		renderer::GeometryBuffersPtr m_vao;
	};
	//! Un vecteur de RenderSubmesh.
	using RenderSubmeshVector = std::vector< RenderSubmesh >;
	/**
	*\brief
	*	Un billboard et son descriptor set, dans la scène.
	*/
	struct RenderBillboard
	{
		RenderBillboard( renderer::Device const & device
			, renderer::DescriptorSetPool const & pool
			, BillboardPtr billboard
			, uint32_t index
			, renderer::UniformBuffer< MatrixUbo > const & mtxUbo
			, renderer::UniformBuffer< MaterialUbo > const & matUbo
			, renderer::UniformBuffer< BillboardUbo > const & billboardUbo
			, renderer::VertexLayout const & layout );
		//! Le billboard.
		BillboardPtr m_billboard;
		//! Le descriptor set.
		renderer::DescriptorSetPtr m_descriptor;
		//! Le tampon de géométries.
		renderer::GeometryBuffersPtr m_vao;
	};
	//! Un vecteur de RenderBillboard.
	using RenderBillboardVector = std::vector< RenderBillboard >;
	/**
	*\brief
	*	Une polyligne et son descriptor set, dans la scène.
	*/
	struct RenderPolyLine
	{
		RenderPolyLine( renderer::Device const & device
			, renderer::DescriptorSetPool const & pool
			, PolyLinePtr line
			, uint32_t index
			, renderer::UniformBuffer< MatrixUbo > const & mtxUbo
			, renderer::UniformBuffer< MaterialUbo > const & matUbo
			, renderer::UniformBuffer< LineUbo > const & lineUbo
			, renderer::VertexLayout const & layout );
		//! La polyligne.
		PolyLinePtr m_line;
		//! Le descriptor set.
		renderer::DescriptorSetPtr m_descriptor;
		//! Le tampon de géométries.
		renderer::GeometryBuffersPtr m_vao;
	};
	//! Un vecteur de RenderPolyLine.
	using RenderPolyLineVector = std::vector< RenderPolyLine >;
	using RenderSubmeshArray = std::array< RenderSubmeshVector, size_t( NodeType::eCount ) >;
	using RenderBillboardArray = std::array< RenderBillboardVector, size_t( NodeType::eCount ) >;
	using RenderPolyLineArray = std::array< RenderPolyLineVector, size_t( NodeType::eCount ) >;
	/**
	*\brief
	*	Charge une texture depuis le contenu donné.
	*\param[in] fileContent
	*	Le contenu du fichier de l'image.
	*\param[out] texture
	*	Reçoit la texture.
	*/
	void loadTexture( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, ByteArray const & fileContent
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
	void loadTexture( renderer::StagingBuffer const & stagingBuffer
		, renderer::CommandBuffer const & commandBuffer
		, ByteArray const & fileContent
		, renderer::PixelFormat format
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
	/**\{*/
	inline std::ostream & operator<<( std::ostream & stream, renderer::Vec2 const & value )
	{
		stream << value.x << ", " << value.y;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, renderer::Vec3 const & value )
	{
		stream << value.x << ", " << value.y << ", " << value.z;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, renderer::Vec4 const & value )
	{
		stream << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, renderer::Quaternion const & value )
	{
		stream << value.x << ", " << value.y << ", " << value.z << ", " << value.w;
		return stream;
	}

	inline std::ostream & operator<<( std::ostream & stream, renderer::Mat4 const & value )
	{
		stream << value[0].x << ", " << value[0].y << ", " << value[0].z << ", " << value[0].w << "\n";
		stream << value[1].x << ", " << value[1].y << ", " << value[1].z << ", " << value[1].w << "\n";
		stream << value[2].x << ", " << value[2].y << ", " << value[2].z << ", " << value[2].w << "\n";
		stream << value[3].x << ", " << value[3].y << ", " << value[3].z << ", " << value[3].w << "\n";
		return stream;
	}
	/**\}*/
}

#endif
