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
	struct Texture
	{
		Image data;
		renderer::TexturePtr texture;
		renderer::TextureViewPtr view;
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
		std::vector< Texture > textures;
		renderer::DescriptorSetLayoutPtr layout;
		renderer::DescriptorSetPoolPtr pool;
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
		renderer::VertexBufferPtr< Vertex > vbo;
		renderer::VertexLayoutPtr layout;
	};
	struct IndexBuffer
	{
		std::vector< uint32_t > data;
		renderer::BufferPtr< uint32_t > ibo;
	};
	struct Submesh
	{
		VertexBuffer vbo;
		IndexBuffer ibo;
		Material material;
		renderer::DescriptorSetPtr descriptorSetTextures;
		renderer::DescriptorSetPtr descriptorSetUbos;
		renderer::GeometryBuffersPtr geometryBuffers;
		renderer::PipelineLayoutPtr pipelineLayout;
		renderer::PipelinePtr pipeline;
	};
	using Object = std::vector< Submesh >;

	renderer::ConnectionPtr makeConnection( wxWindow * window
		, renderer::Renderer const & renderer );

	class Application;
	class MainFrame;
}
