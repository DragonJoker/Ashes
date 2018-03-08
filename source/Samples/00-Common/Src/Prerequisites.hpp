#pragma once

#include <Core/Connection.hpp>
#include <Core/Renderer.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Miscellaneous/Extent2D.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <RenderPass/RenderSubpass.hpp>

#include <Utils/Factory.hpp>
#include <Utils/Mat4.hpp>

#pragma warning( disable: 4996 )
#include <wx/wx.h>
#include <wx/windowptr.h>

#include <array>
#include <chrono>

namespace common
{
	static wxSize const WindowSize{ 800, 600 };
	using RendererFactory = utils::Factory< renderer::Renderer
		, std::string
		, renderer::RendererPtr
		, std::function< renderer::RendererPtr( renderer::Renderer::Configuration const & ) > >;

	static uint32_t constexpr MAX_TEXTURES = 6u;
	static uint32_t constexpr MAX_LIGHTS = 10u;

	struct NonTexturedVertex2DData
	{
		utils::Vec2 position;
	};

	struct SceneData
	{
		utils::Mat4 mtxProjection;
		utils::Mat4 mtxView;
		utils::Vec4 cameraPosition;
	};

	struct ObjectData
	{
		utils::Mat4 mtxModel;
	};

	struct BillboardInstanceData
	{
		utils::Vec3 offset;
		utils::Vec2 dimensions;
	};

	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	/**
	*\~english
	*\name Loaded data.
	*\~french
	*\name Données chargées.
	*/
	/**\{*/
	struct Image
	{
		renderer::Extent2D size;
		renderer::ByteArray data;
		renderer::Format format;
		bool opacity{ false };
	};

	using ImagePtr = std::shared_ptr< Image >;
	using ImagePtrArray = std::vector< ImagePtr >;

	struct TextureOperators
	{
		int diffuse{ 0 }; // 0 or 1
		int specular{ 0 }; // 0 or 1
		int emissive{ 0 }; // 0 or 1
		int normal{ 0 }; // 0 for none, 1 for normals, 2 for inverted normals
		uint32_t shininess{ 0 }; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
		uint32_t opacity{ 0 }; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
		uint32_t height{ 0 }; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
		float fill{ 0.0f }; // align to 16 bytes.
	};

	struct MaterialData
	{
		renderer::RgbaColour diffuse;
		renderer::RgbaColour specular;
		renderer::RgbaColour emissive;
		float shininess{ 0.0f };
		float opacity{ 1.0f };
		uint32_t texturesCount{ 0u };
		int backFace{ 0 }; // 0 or 1
		std::array< TextureOperators, MAX_TEXTURES > textureOperators;
	};

	struct Material
	{
		MaterialData data;
		bool hasOpacity{ false };
		ImagePtrArray textures;
	};

	struct Vertex
	{
		utils::Vec3 position;
		utils::Vec3 normal;
		utils::Vec3 tangent;
		utils::Vec3 bitangent;
		utils::Vec2 texture;
	};

	struct VertexBuffer
	{
		std::vector< Vertex > data;
		bool hasNormals{ false }; // true implies that it will also have tangents and bitangents
	};

	struct Face
	{
		uint32_t a;
		uint32_t b;
		uint32_t c;
	};

	struct IndexBuffer
	{
		std::vector< Face > data;
	};

	struct Submesh
	{
		VertexBuffer vbo;
		IndexBuffer ibo;
		std::vector< Material > materials;
	};

	using Object = std::vector< Submesh >;

	struct Billboard
	{
		Material material;
		std::vector< BillboardInstanceData > list;
	};
	/**\}*/
	/**
	*\~english
	*\name Lighting.
	*\~french
	*\name Eclairage.
	*/
	/**\{*/
	struct Light
	{
		utils::Vec4 colour;
		utils::Vec4 intensities;
	};

	struct DirectionalLight
	{
		Light base;
		utils::Vec4 direction;
	};

	struct PointLight
	{
		Light base;
		utils::Vec4 position;
		utils::Vec4 attenation;
	};

	struct SpotLight
	{
		PointLight base;
		utils::Vec4 direction;
		utils::Vec4 coeffs;// .x = cutoff, .y = exponent
	};

	struct LightsData
	{
		utils::IVec4 lightsCount;
		DirectionalLight directionalLights[MAX_LIGHTS];
		PointLight pointLights[MAX_LIGHTS];
		SpotLight spotLights[MAX_LIGHTS];
	};
	/**\}*/
	/**
	*\~english
	*\name Rendered data.
	*\~french
	*\name Données rendues.
	*/
	/**\{*/
	struct TextureNode
	{
		ImagePtr image;
		renderer::TexturePtr texture;
		renderer::TextureViewPtr view;
	};

	using TextureNodePtr = std::shared_ptr< TextureNode >;
	using TextureNodePtrArray = std::vector< TextureNodePtr >;

	template< typename NodeType >
	struct MaterialNode
	{
		std::shared_ptr< NodeType > instance;
		TextureNodePtrArray textures;
		renderer::DescriptorSetLayoutPtr layout;
		renderer::DescriptorSetPoolPtr pool;
		renderer::DescriptorSetPtr descriptorSetTextures;
		renderer::DescriptorSetPtr descriptorSetUbos;
		renderer::PipelineLayoutPtr pipelineLayout;
		renderer::PipelinePtr pipeline;
	};

	struct SubmeshNode
	{
		renderer::VertexBufferPtr< Vertex > vbo;
		renderer::BufferPtr< Face > ibo;
	};

	struct BillboardNode
	{
		renderer::VertexBufferPtr< Vertex > vbo;
		renderer::VertexBufferPtr< BillboardInstanceData > instance;
	};

	using SubmeshNodePtr = std::shared_ptr< SubmeshNode >;
	using BillboardNodePtr = std::shared_ptr< BillboardNode >;
	using SubmeshMaterialNode = MaterialNode< SubmeshNode >;
	using BillboardMaterialNode = MaterialNode< BillboardNode >;

	using ObjectNodes = std::vector< SubmeshMaterialNode >;
	using BillboardListNodes = std::vector< BillboardMaterialNode >;

	using SubmeshNodes = std::vector< SubmeshNodePtr >;
	using BillboardNodes = std::vector< BillboardNodePtr >;
	/**\}*/

	renderer::ConnectionPtr makeConnection( wxWindow * window
		, renderer::Renderer const & renderer );

	std::vector< renderer::Format > getFormats( renderer::TextureViewCRefArray const & views );

	struct Scene;

	class Application;
	class MainFrame;
	class NodesRenderer;
	class OpaqueRendering;
	class RenderPanel;
	class RenderTarget;
	class TransparentRendering;

	using NodesRendererPtr = std::unique_ptr< NodesRenderer >;
	using OpaqueRenderingPtr = std::unique_ptr< OpaqueRendering >;
	using TransparentRenderingPtr = std::unique_ptr< TransparentRendering >;
}
