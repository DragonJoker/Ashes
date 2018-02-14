#pragma once

#include <Core/Connection.hpp>

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
		, std::function< renderer::RendererPtr( bool ) > >;

	static uint32_t constexpr MAX_TEXTURES = 6u;
	static uint32_t constexpr MAX_LIGHTS = 10u;

	struct SceneData
	{
		renderer::Mat4 mtxProjection;
		renderer::Mat4 mtxView;
		renderer::Vec4 cameraPosition;
	};

	struct ObjectData
	{
		renderer::Mat4 mtxModel;
	};

	struct BillboardInstanceData
	{
		renderer::Vec3 offset;
		renderer::Vec2 dimensions;
	};

	struct TexturedVertexData
	{
		renderer::Vec4 position;
		renderer::Vec2 uv;
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
		renderer::UIVec2 size;
		renderer::ByteArray data;
		renderer::PixelFormat format;
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
		renderer::Vec3 position;
		renderer::Vec3 normal;
		renderer::Vec3 tangent;
		renderer::Vec3 bitangent;
		renderer::Vec2 texture;
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
		renderer::Vec4 colour;
		renderer::Vec4 intensities;
	};

	struct DirectionalLight
	{
		Light base;
		renderer::Vec4 direction;
	};

	struct PointLight
	{
		Light base;
		renderer::Vec4 position;
		renderer::Vec4 attenation;
	};

	struct SpotLight
	{
		PointLight base;
		renderer::Vec4 direction;
		renderer::Vec4 coeffs;// .x = cutoff, .y = exponent
	};

	struct LightsData
	{
		renderer::IVec4 lightsCount;
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
		renderer::GeometryBuffersPtr geometryBuffers;
	};

	struct BillboardNode
	{
		renderer::VertexBufferPtr< Vertex > vbo;
		renderer::VertexBufferPtr< BillboardInstanceData > instance;
		renderer::GeometryBuffersPtr geometryBuffers;
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

	std::vector< renderer::PixelFormat > getFormats( renderer::TextureViewCRefArray const & views );

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
