#pragma once

#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Instance.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>

#include <util/Mat4.hpp>
#include <util/UtilsDebug.hpp>
#include <util/UtilsDevice.hpp>
#include <util/UtilsInstance.hpp>
#include <util/UtilsSwapChain.hpp>

#pragma warning( disable: 4996 )
#include <wx/wx.h>
#include <wx/windowptr.h>

#include <array>
#include <chrono>

namespace common
{
	static wxSize const WindowSize{ 800, 600 };
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
		VkExtent2D size;
		ashes::ByteArray data;
		VkFormat format;
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
		utils::Vec4 diffuse;
		utils::Vec4 specular;
		utils::Vec4 emissive;
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
		ashes::ImagePtr texture;
		ashes::ImageView view;
	};

	using TextureNodePtr = std::shared_ptr< TextureNode >;
	using TextureNodePtrArray = std::vector< TextureNodePtr >;

	template< typename NodeType >
	struct MaterialNode
	{
		std::shared_ptr< NodeType > instance;
		TextureNodePtrArray textures;
		ashes::DescriptorSetLayoutPtr layout;
		ashes::DescriptorSetPoolPtr pool;
		ashes::DescriptorSetPtr descriptorSetTextures;
		ashes::DescriptorSetPtr descriptorSetUbos;
		ashes::PipelineLayoutPtr pipelineLayout;
		ashes::GraphicsPipelinePtr pipeline;
	};

	struct SubmeshNode
	{
		ashes::VertexBufferPtr< Vertex > vbo;
		ashes::BufferPtr< Face > ibo;
	};

	struct BillboardNode
	{
		ashes::VertexBufferPtr< Vertex > vbo;
		ashes::VertexBufferPtr< BillboardInstanceData > instance;
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

	ashes::WindowHandle makeWindowHandle( wxWindow const & window );

	std::vector< VkFormat > getFormats( ashes::ImageViewArray const & views );

	struct Scene;

	class Application;
	class MainFrame;
	class NodesRenderer;
	class OpaqueRendering;
	class RenderPanel;
	class RenderTarget;
	class TransparentRendering;

	using NodesInstancePtr = std::unique_ptr< NodesRenderer >;
	using OpaqueRenderingPtr = std::unique_ptr< OpaqueRendering >;
	using TransparentRenderingPtr = std::unique_ptr< TransparentRendering >;
}
