/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "util/GlslToSpv.hpp"

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 4464 )
#pragma warning( disable: 5267 )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wdeprecated-copy-dtor"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wsigned-enum-bitfield"
#pragma clang diagnostic ignored "-Wsuggest-destructor-override"
#pragma clang diagnostic ignored "-Wsuggest-override"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop
#pragma warning( pop )

#include <iostream>
#include <locale>
#include <regex>

namespace utils
{
	namespace
	{
		struct BlockLocale
		{
			BlockLocale( BlockLocale const & ) = delete;
			BlockLocale & operator=( BlockLocale const & ) = delete;
			BlockLocale( BlockLocale && )noexcept = delete;
			BlockLocale & operator=( BlockLocale && )noexcept = delete;

			BlockLocale()
			{
				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( std::locale{ "C" } );
				}
			}

			~BlockLocale()noexcept
			{
				if ( m_prvLoc.name() != "C" )
				{
					std::locale::global( m_prvLoc );
				}
			}

		private:
			std::locale m_prvLoc{ std::locale( "" ) };
		};

		void doInitResources( VkPhysicalDeviceProperties const & props
			, TBuiltInResource & resources )
		{
			auto & limits = props.limits;

			resources.limits.doWhileLoops = true;
			resources.limits.generalAttributeMatrixVectorIndexing = true;
			resources.limits.generalConstantMatrixVectorIndexing = true;
			resources.limits.generalSamplerIndexing = true;
			resources.limits.generalUniformIndexing = true;
			resources.limits.generalVariableIndexing = true;
			resources.limits.generalVaryingIndexing = true;
			resources.limits.nonInductiveForLoops = true;
			resources.limits.whileLoops = true;
			resources.maxAtomicCounterBindings = 1;
			resources.maxAtomicCounterBufferSize = 16384;
			resources.maxClipDistances = int( limits.maxClipDistances );
			resources.maxClipPlanes = 6;
			resources.maxCombinedAtomicCounterBuffers = 1;
			resources.maxCombinedAtomicCounters = 8;
			resources.maxCombinedClipAndCullDistances = int( limits.maxCombinedClipAndCullDistances );
			resources.maxCombinedImageUniforms = 8;
			resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
			resources.maxCombinedShaderOutputResources = 8;
			resources.maxCombinedTextureImageUnits = 80;
			resources.maxComputeAtomicCounterBuffers = 1;
			resources.maxComputeAtomicCounters = 8;
			resources.maxComputeImageUniforms = 8;
			resources.maxComputeTextureImageUnits = 16;
			resources.maxComputeUniformComponents = 1024;
			resources.maxComputeWorkGroupCountX = int( limits.maxComputeWorkGroupCount[0] );
			resources.maxComputeWorkGroupCountY = int( limits.maxComputeWorkGroupCount[1] );
			resources.maxComputeWorkGroupCountZ = int( limits.maxComputeWorkGroupCount[2] );
			resources.maxComputeWorkGroupSizeX = int( limits.maxComputeWorkGroupSize[0] );
			resources.maxComputeWorkGroupSizeY = int( limits.maxComputeWorkGroupSize[1] );
			resources.maxComputeWorkGroupSizeZ = int( limits.maxComputeWorkGroupSize[2] );
			resources.maxCullDistances = int( limits.maxCullDistances );
			resources.maxDrawBuffers = int( limits.maxColorAttachments );
			resources.maxFragmentAtomicCounterBuffers = 1;
			resources.maxFragmentAtomicCounters = 8;
			resources.maxFragmentImageUniforms = 8;
			resources.maxFragmentInputComponents = int( limits.maxFragmentInputComponents );
			resources.maxFragmentInputVectors = 15;
			resources.maxFragmentUniformComponents = 4096;
			resources.maxFragmentUniformVectors = 16;
			resources.maxGeometryAtomicCounterBuffers = 0;
			resources.maxGeometryAtomicCounters = 0;
			resources.maxGeometryImageUniforms = 0;
			resources.maxGeometryInputComponents = int( limits.maxGeometryInputComponents );
			resources.maxGeometryOutputComponents = int( limits.maxGeometryOutputComponents );
			resources.maxGeometryOutputVertices = 256;
			resources.maxGeometryTextureImageUnits = 16;
			resources.maxGeometryTotalOutputComponents = int( limits.maxGeometryTotalOutputComponents );
			resources.maxGeometryUniformComponents = 1024;
			resources.maxGeometryVaryingComponents = int( limits.maxGeometryOutputVertices );
			resources.maxImageSamples = 0;
			resources.maxImageUnits = 8;
			resources.maxLights = 32;
			resources.maxPatchVertices = int( limits.maxTessellationPatchSize );
			resources.maxProgramTexelOffset = int( limits.maxTexelOffset );
			resources.maxSamples = 4;
			resources.maxTessControlAtomicCounterBuffers = 0;
			resources.maxTessControlAtomicCounters = 0;
			resources.maxTessControlImageUniforms = 0;
			resources.maxTessControlInputComponents = int( limits.maxTessellationControlPerVertexInputComponents );
			resources.maxTessControlOutputComponents = 128;
			resources.maxTessControlTextureImageUnits = 16;
			resources.maxTessControlTotalOutputComponents = int( limits.maxTessellationControlTotalOutputComponents );
			resources.maxTessControlUniformComponents = 1024;
			resources.maxTessEvaluationAtomicCounterBuffers = 0;
			resources.maxTessEvaluationAtomicCounters = 0;
			resources.maxTessEvaluationImageUniforms = 0;
			resources.maxTessEvaluationInputComponents = int( limits.maxTessellationEvaluationInputComponents );
			resources.maxTessEvaluationOutputComponents = int( limits.maxTessellationEvaluationOutputComponents );
			resources.maxTessEvaluationTextureImageUnits = 16;
			resources.maxTessEvaluationUniformComponents = 1024;
			resources.maxTessGenLevel = int( limits.maxTessellationGenerationLevel );
			resources.maxTessPatchComponents = int( limits.maxTessellationControlPerPatchOutputComponents );
			resources.maxTextureCoords = 32;
			resources.maxTextureImageUnits = 32;
			resources.maxTextureUnits = 32;
			resources.maxTransformFeedbackBuffers = 4;
			resources.maxTransformFeedbackInterleavedComponents = 64;
			resources.maxVaryingComponents = 60;
			resources.maxVaryingFloats = 64;
			resources.maxVaryingVectors = 8;
			resources.maxVertexAtomicCounterBuffers = 0;
			resources.maxVertexAtomicCounters = 0;
			resources.maxVertexAttribs = 64;
			resources.maxVertexImageUniforms = 0;
			resources.maxVertexOutputComponents = 64;
			resources.maxVertexOutputVectors = 16;
			resources.maxVertexTextureImageUnits = 32;
			resources.maxVertexUniformComponents = 4096;
			resources.maxVertexUniformVectors = 128;
			resources.maxViewports = int( limits.maxViewports );
			resources.minProgramTexelOffset = -8;
		}

		EShLanguage doGetLanguage( VkShaderStageFlagBits stage )
		{
			switch ( stage )
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				return EShLangVertex;

			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				return EShLangTessControl;

			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				return EShLangTessEvaluation;

			case VK_SHADER_STAGE_GEOMETRY_BIT:
				return EShLangGeometry;

			case VK_SHADER_STAGE_FRAGMENT_BIT:
				return EShLangFragment;

			case VK_SHADER_STAGE_COMPUTE_BIT:
				return EShLangCompute;

			default:
				assert( false && "Unsupported shader stage." );
				return EShLangVertex;
			}
		}
	}

	void initialiseGlslang()
	{
		glslang::InitializeProcess();
	}

	void cleanupGlslang()
	{
		glslang::FinalizeProcess();
	}

	ashes::UInt32Array compileGlslToSpv( VkPhysicalDeviceProperties const & props
		, VkShaderStageFlagBits stage
		, std::string const & shader )
	{
		BlockLocale guard;
		TBuiltInResource resources{};
		doInitResources( props, resources );

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		auto messages = EShMessages( EShMsgSpvRules | EShMsgVulkanRules );
		auto glstage = doGetLanguage( stage );

		std::string source = shader;

		glslang::TShader glshader{ glstage };
		char const * const str = source.c_str();
		glshader.setStrings( &str, 1 );

		if ( !glshader.parse( &resources, 460, ECoreProfile, false, true, messages ) )
		{
			std::cerr << glshader.getInfoLog() << std::endl;
			std::cerr << glshader.getInfoDebugLog() << std::endl;
			std::cerr << source << std::endl;
			throw ashes::BaseException{ "Shader compilation failed." };
		}

		glslang::TProgram glprogram;
		glprogram.addShader( &glshader );

		if ( !glprogram.link( messages ) )
		{
			std::cerr << glprogram.getInfoLog() << std::endl;
			std::cerr << glprogram.getInfoDebugLog() << std::endl;
			std::cerr << source << std::endl;
			throw ashes::BaseException{ "Shader linkage failed." };
		}

		ashes::UInt32Array spirv;
		glslang::GlslangToSpv( *glprogram.getIntermediate( glstage ), spirv );

		return spirv;
	}
}
