/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl
{
	struct InputsLayout
	{
		VkVertexInputBindingDescriptionArray vertexBindingDescriptions;
		VkVertexInputAttributeDescriptionArray vertexAttributeDescriptions;
	};

	inline bool operator==( InputsLayout const & lhs
		, InputsLayout const & rhs )
	{
		return lhs.vertexBindingDescriptions == rhs.vertexBindingDescriptions
			&& lhs.vertexAttributeDescriptions == rhs.vertexAttributeDescriptions;
	}

	inline bool operator!=( InputsLayout const & lhs
		, InputsLayout const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	enum class ImageFormat
	{
		e1D,
		e2D,
		e3D,
		eCube,
		eBuffer,
		e2DRect,
		e1DArray,
		e2DArray,
		eCubeArray,
		e2DMultisample,
		e2DMultisampleArray,
		eInt1D,
		eInt2D,
		eInt3D,
		eIntCube,
		eIntBuffer,
		eInt2DRect,
		eInt1DArray,
		eInt2DArray,
		eIntCubeArray,
		eInt2DMultisample,
		eInt2DMultisampleArray,
		eUInt1D,
		eUInt2D,
		eUInt3D,
		eUIntCube,
		eUIntBuffer,
		eUInt2DRect,
		eUInt1DArray,
		eUInt2DArray,
		eUIntCubeArray,
		eUInt2DMultisample,
		eUInt2DMultisampleArray,
	};
	
	enum class SamplerFormat
	{
		e1D,
		e2D,
		e3D,
		eCube,
		e1DShadow,
		e2DShadow,
		e2DRect,
		e2DRectShadow,
		e1DArray,
		e2DArray,
		eCubeArray,
		eBuffer,
		e1DArrayShadow,
		e2DArrayShadow,
		eCubeShadow,
		eCubeArrayShadow,
		eInt1D,
		eInt2D,
		eInt3D,
		eIntCube,
		eInt2DRect,
		eInt1DArray,
		eInt2DArray,
		eIntCubeArray,
		eIntBuffer,
		eUInt1D,
		eUInt2D,
		eUInt3D,
		eUIntCube,
		eUInt2DRect,
		eUInt1DArray,
		eUInt2DArray,
		eUIntCubeArray,
		eUIntBuffer,
		e2DMultisample,
		eInt2DMultisample,
		eUInt2DMultisample,
		e2DMultisampleArray,
		eInt2DMultisampleArray,
		eUInt2DMultisampleArray,
	};

	template< typename FormatT >
	struct FormatDescT
	{
		uint32_t program;
		VkShaderStageFlagBits stageFlag;
		std::string name;
		uint32_t location{ 0u };
		FormatT format{};
		uint32_t size{ 0u };
		uint32_t arraySize{ 1u };
		uint32_t offset{ 0u };
	};

	template< typename FormatT >
	inline bool operator==( FormatDescT< FormatT > const & lhs
		, FormatDescT< FormatT > const & rhs )
	{
		return lhs.name == rhs.name
			&& lhs.stageFlag == rhs.stageFlag
			&& lhs.location == rhs.location
			&& lhs.format == rhs.format
			&& lhs.size == rhs.size
			&& lhs.arraySize == rhs.arraySize
			&& lhs.offset == rhs.offset;
	}

	template< typename FormatT >
	inline bool operator!=( FormatDescT< FormatT > const & lhs
		, FormatDescT< FormatT > const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	template< typename FormatT >
	using DescLayoutT = std::vector< FormatDescT< FormatT > >;

	using ConstantDesc = FormatDescT< ConstantFormat >;
	using ImageDesc = FormatDescT< ImageFormat >;
	using SamplerDesc = FormatDescT< SamplerFormat >;

	using ConstantsLayout = DescLayoutT< ConstantFormat >;
	using ImagesLayout = DescLayoutT< ImageFormat >;
	using SamplersLayout = DescLayoutT< SamplerFormat >;

	struct ConstantBufferDesc
	{
		std::string name;
		uint32_t binding{ 0u };
		uint32_t size{ 0u };
		ConstantsLayout constants;
	};

	inline bool operator==( ConstantBufferDesc const & lhs
		, ConstantBufferDesc const & rhs )
	{
		return lhs.name == rhs.name
			&& lhs.binding == rhs.binding
			&& lhs.size == rhs.size
			&& lhs.constants == rhs.constants;
	}

	inline bool operator!=( ConstantBufferDesc const & lhs
		, ConstantBufferDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	using InterfaceBlocksLayout = std::vector< ConstantBufferDesc >;

	struct ShaderDesc
	{
		explicit ShaderDesc( bool isGlsl = {}
			, GLuint program = {}
			, VkShaderStageFlags stageFlags = {}
			, InputsLayout inputs = {}
			, ConstantsLayout pcb = {}
			, InterfaceBlocksLayout ubo = {}
			, InterfaceBlocksLayout sbo = {}
			, SamplersLayout tbo = {}
			, SamplersLayout tex = {}
			, ImagesLayout ibo = {}
			, ImagesLayout img = {} )
			: isGlsl{ isGlsl }
			, program{ program }
			, stageFlags{ stageFlags }
			, inputs{ std::move( inputs ) }
			, pcb{ std::move( pcb ) }
			, ubo{ std::move( ubo ) }
			, sbo{ std::move( sbo ) }
			, tbo{ std::move( tbo ) }
			, tex{ std::move( tex ) }
			, ibo{ std::move( ibo ) }
			, img{ std::move( img ) }
		{
		}

		bool isGlsl{};
		GLuint program{};
		VkShaderStageFlags stageFlags{};
		InputsLayout inputs{};
		ConstantsLayout pcb{};
		InterfaceBlocksLayout ubo{};
		InterfaceBlocksLayout sbo{};
		SamplersLayout tbo{};
		SamplersLayout tex{};
		ImagesLayout ibo{};
		ImagesLayout img{};
	};

	inline bool operator==( ShaderDesc const & lhs, ShaderDesc const & rhs )
	{
		return lhs.stageFlags == rhs.stageFlags
			&& lhs.inputs == rhs.inputs
			&& lhs.pcb == rhs.pcb
			&& lhs.ubo == rhs.ubo
			&& lhs.sbo == rhs.sbo
			&& lhs.tbo == rhs.tbo
			&& lhs.tex == rhs.tex
			&& lhs.ibo == rhs.ibo
			&& lhs.img == rhs.img;
	}

	inline bool operator!=( ShaderDesc const & lhs, ShaderDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}

	using ProgramLayout = std::vector< ShaderDesc >;

	struct PushConstantsDesc
	{
		VkShaderStageFlags stageFlags{};
		uint32_t offset{ 0u };
		uint32_t size{ 0u };
		ConstantsLayout constants;
		std::vector< uint8_t > data;
	};

	inline bool operator==( PushConstantsDesc const & lhs, PushConstantsDesc const & rhs )
	{
		return lhs.stageFlags == rhs.stageFlags
			&& lhs.offset == rhs.offset
			&& lhs.size == rhs.size
			&& lhs.constants == rhs.constants
			&& lhs.data == rhs.data;
	}

	inline bool operator!=( PushConstantsDesc const & lhs, PushConstantsDesc const & rhs )
	{
		return !operator==( lhs, rhs );
	}
}
