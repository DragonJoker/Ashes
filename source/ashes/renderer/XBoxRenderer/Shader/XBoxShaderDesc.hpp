/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	struct InputElementDesc
	{
		std::string SemanticName;
		UINT SemanticIndex{};
		UINT BaseSemanticIndex{};
		DXGI_FORMAT Format{};
		UINT InputSlot{};
		UINT AlignedByteOffset{};
		D3D11_INPUT_CLASSIFICATION InputSlotClass{};
		UINT InstanceDataStepRate{};
	};

	struct ConstantDesc
	{
		std::string name;
		UINT offset{};
		UINT size{};
	};

	struct ConstantBufferDesc
	{
		std::string name;
		UINT binding{};
		UINT size{};
		std::vector< ConstantDesc > constants;
	};

	using InputLayout = std::vector< InputElementDesc >;
	using InterfaceBlockLayout = std::vector< ConstantBufferDesc >;

	class CompiledShaderModule;

	struct ShaderDesc
	{
		CompiledShaderModule * shaderModule{ nullptr };
		InputLayout inputLayout;
		InterfaceBlockLayout interfaceBlockLayout;
	};

	using ProgramLayout = std::map< VkShaderStageFlagBits, ShaderDesc >;
}
