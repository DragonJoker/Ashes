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
	using RendererFactory = utils::Factory< renderer::Renderer, std::string, renderer::RendererPtr >;

	static uint32_t constexpr MAX_TEXTURES = 6u;
	static uint32_t constexpr MAX_LIGHTS = 10u;

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

	struct TextureOperators
	{
		int diffuse{ 0 }; // 0 or 1
		int specular{ 0 }; // 0 or 1
		int emissive{ 0 }; // 0 or 1
		uint32_t shininess{ 0 }; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
		uint32_t opacity{ 0 }; // 0 for none, 1 for R, 2 for G, 4 for B, 8 for A
		float normalModifier{ 0.0f };
		renderer::Vec2 fill; // align to 16 bytes.
	};

	struct MaterialData
	{
		renderer::RgbaColour diffuse;
		renderer::RgbaColour specular;
		renderer::RgbaColour emissive;
		float shininess{ 0.0f };
		float opacity{ 1.0f };
		uint32_t texturesCount{ 0u };
		float fill;
		std::array< TextureOperators, MAX_TEXTURES > textureOperators;
	};

	struct Material
	{
		MaterialData data;
		std::vector< Image > textures;
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

	struct IndexBuffer
	{
		std::vector< uint32_t > data;
	};

	struct Submesh
	{
		VertexBuffer vbo;
		IndexBuffer ibo;
		Material material;
	};

	using Object = std::vector< Submesh >;
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
		renderer::TexturePtr texture;
		renderer::TextureViewPtr view;
	};

	struct MaterialNode
	{
		std::vector< TextureNode > textures;
		renderer::DescriptorSetLayoutPtr layout;
		renderer::DescriptorSetPoolPtr pool;
	};

	struct SubmeshNode
	{
		MaterialNode material;
		renderer::VertexBufferPtr< Vertex > vbo;
		renderer::VertexLayoutPtr vertexLayout;
		renderer::BufferPtr< uint32_t > ibo;
		renderer::DescriptorSetPtr descriptorSetTextures;
		renderer::DescriptorSetPtr descriptorSetUbos;
		renderer::GeometryBuffersPtr geometryBuffers;
		renderer::PipelineLayoutPtr pipelineLayout;
		renderer::PipelinePtr pipeline;
	};

	using ObjectNode = std::vector< SubmeshNode >;
	/**\}*/

	renderer::ConnectionPtr makeConnection( wxWindow * window
		, renderer::Renderer const & renderer );

	class Application;
	class MainFrame;
}
