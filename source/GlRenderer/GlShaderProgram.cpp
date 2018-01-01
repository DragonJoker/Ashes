/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlShaderProgram.hpp"

#include "GlDevice.hpp"

namespace gl_renderer
{
	ShaderProgram::ShaderProgram( renderer::Device const & device )
		: renderer::ShaderProgram{ device }
		, m_program{ static_cast< Device const & >( device ).getDevice().createShaderProgram() }
	{
	}

	void ShaderProgram::createModule( std::string const & shader
		, renderer::ShaderStageFlag stage )
	{
#if VKRENDERER_GLSL_TO_SPV

		TBuiltInResource resources;
		doInitResources( resources );

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		auto messages = ( EShMessages )( EShMsgSpvRules | EShMsgVulkanRules );
		auto glstage = doGetLanguage( stage );

		glslang::TShader glshader{ glstage };
		char const * const str = shader.c_str();
		glshader.setStrings( &str, 1 );

		if ( !glshader.parse( &resources, 100, false, messages ) )
		{
			std::cerr << glshader.getInfoLog() << std::endl;
			std::cerr << glshader.getInfoDebugLog() << std::endl;
			throw std::runtime_error{ "Shader compilation failed." };
		}

		glslang::TProgram glprogram;
		glprogram.addShader( &glshader );

		if ( !glprogram.link( messages ) )
		{
			std::cerr << glprogram.getInfoLog() << std::endl;
			std::cerr << glprogram.getInfoDebugLog() << std::endl;
			throw std::runtime_error{ "Shader linkage failed." };
		}

		renderer::UInt32Array spirv;
		glslang::GlslangToSpv( *glprogram.getIntermediate( glstage ), spirv );
		m_program->createModule( spirv, convert( stage ) );

#else

		throw std::runtime_error{ "SPIR-V compilation from GLSL is not supported." };

#endif
	}

	void ShaderProgram::createModule( renderer::ByteArray const & fileData
		, renderer::ShaderStageFlag stage )
	{
		m_program->createModule( fileData, convert( stage ) );
	}
}
