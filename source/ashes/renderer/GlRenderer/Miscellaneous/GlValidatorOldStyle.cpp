#include "Miscellaneous/GlValidatorOldStyle.hpp"

#include "Core/GlDevice.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl_api.hpp"

#include <algorithm>
#include <sstream>

#if defined( interface )
#	undef interface
#endif

namespace ashes::gl::gl3
{
	namespace
	{
		enum GlslInterface
			: GLenum
		{
			GL_ACTIVE_UNIFORM_BLOCKS = 0x8A36,
			GL_ACTIVE_UNIFORM_BLOCK_NAME_LENGTH = 0x8A41,
			GL_ACTIVE_UNIFORMS = 0x8B86,
			GL_ACTIVE_UNIFORM_MAX_LENGTH = 0x8B87,
			GL_ACTIVE_ATTRIBUTES = 0x8B89,
			GL_ACTIVE_ATTRIBUTE_MAX_LENGTH = 0x8B8A,
		};

		enum GlslUniformInterface
			: GLenum
		{
			GL_UNIFORM_TYPE = 0x8A37,
			GL_UNIFORM_SIZE = 0x8A38,
			GL_UNIFORM_BLOCK_INDEX = 0x8A3A,
			GL_UNIFORM_OFFSET = 0x8A3B,
		};

		enum GlslUniformBlockInterface
		{
			GL_UNIFORM_BLOCK_BINDING = 0x8A3F,
			GL_UNIFORM_BLOCK_DATA_SIZE = 0x8A40,
		};
	}

	template< typename FuncT >
	void getInterfaceInfos( ContextLock const & context
		, GLuint program
		, GlslInterface interfaceName
		, GlslInterface interfaceMaxLengthName
		, FuncT func )
	{
		GLint count = 0u;
		glLogCall( context
			, glGetProgramiv
			, program
			, interfaceName
			, &count );

		if ( count )
		{
			GLint maxLength = 0;
			glLogCall( context
				, glGetProgramiv
				, program
				, interfaceMaxLengthName
				, &maxLength );
			maxLength = maxLength
				? std::min( 256, maxLength )
				: 256;

			for ( GLuint i = 0u; i < GLuint( count ); ++i )
			{
				func( i, maxLength );
			}
		}
	}

	InputsLayout getInputs( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		InputsLayout result;
		getInterfaceInfos( context
			, program
			, GL_ACTIVE_ATTRIBUTES
			, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
			, [&result, &context, &program]( GLint index, GLint maxLength )
			{
				std::vector< char > nameBuf;
				nameBuf.resize( maxLength );
				GLsizei nameLength = 0;
				GLint arraySize = 0;
				auto offset = 0u;
				GlslAttributeType attributeType;
				glLogCall( context
					, glGetActiveAttrib
					, program
					, index
					, maxLength - 1
					, &nameLength
					, &arraySize
					, (GLenum * )&attributeType
					, nameBuf.data() );
				auto location = glLogNonVoidCall( context
					, glGetAttribLocation
					, program
					, nameBuf.data() );
				uint32_t locOffset = 0u;

				switch ( attributeType )
				{
				case GLSL_ATTRIBUTE_FLOAT_MAT4x2:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32_SFLOAT, offset } );
					offset += 2 * sizeof( float );
					[[fallthrough]];
				case GLSL_ATTRIBUTE_FLOAT_MAT3x2:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32_SFLOAT, offset } );
					offset += 2 * sizeof( float );
					[[fallthrough]];
				case GLSL_ATTRIBUTE_FLOAT_MAT2:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32_SFLOAT, offset } );
					offset += 2 * sizeof( float );
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32_SFLOAT, offset } );
					break;
				case GLSL_ATTRIBUTE_FLOAT_MAT4x3:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32_SFLOAT, offset } );
					offset += 3 * sizeof( float );
					[[fallthrough]];
				case GLSL_ATTRIBUTE_FLOAT_MAT3:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32_SFLOAT, offset } );
					offset += 3 * sizeof( float );
					[[fallthrough]];
				case GLSL_ATTRIBUTE_FLOAT_MAT2x3:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32_SFLOAT, offset } );
					offset += 3 * sizeof( float );
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32_SFLOAT, offset } );
					break;
				case GLSL_ATTRIBUTE_FLOAT_MAT4:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offset } );
					offset += 4 * sizeof( float );
					[[fallthrough]];
				case GLSL_ATTRIBUTE_FLOAT_MAT3x4:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offset } );
					offset += 4 * sizeof( float );
					[[fallthrough]];
				case GLSL_ATTRIBUTE_FLOAT_MAT2x4:
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offset } );
					offset += 4 * sizeof( float );
					result.vertexAttributeDescriptions.push_back( { location + locOffset++, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offset } );
					break;
				default:
					result.vertexAttributeDescriptions.push_back( { location + 0u, 0u, getAttributeFormat( attributeType ), offset } );
					break;
				}
			} );

		return result;
	}

	template< typename FilterT, typename FuncT >
	void getUniformInfos( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program
		, FilterT filter
		, FuncT func )
	{
		GLuint uniformCount = 0u;
		glLogCall( context
			, glGetProgramiv
			, program
			, GL_ACTIVE_UNIFORMS
			, reinterpret_cast< GLint * >( &uniformCount ) );
		std::vector< std::array< char, 256 > > uniformNamesData;
		std::vector< const char * > uniformNames;
		uniformNamesData.resize( uniformCount );

		for ( auto index = 0u; index < uniformCount; ++index )
		{
			GLint actualLength = 0;
			GLint arraySize = 0;
			glLogCall( context
				, glGetActiveUniformName
				, program
				, index
				, GLsizei( uniformNamesData[index].size() )
				, &actualLength
				, uniformNamesData[index].data() );
			uniformNames.push_back( uniformNamesData[index].data() );
		}

		std::vector< GLuint > uniformIndices;
		uniformIndices.resize( uniformCount );
		glLogCall( context
			, glGetUniformIndices
			, program
			, uniformCount
			, uniformNames.data()
			, uniformIndices.data() );
		std::vector< GlslAttributeType > uniformBlockIndices;
		uniformBlockIndices.resize( uniformCount );
		glLogCall( context
			, glGetActiveUniformsiv
			, program
			, uniformCount
			, uniformIndices.data()
			, GL_UNIFORM_BLOCK_INDEX
			, reinterpret_cast< GLint * >( uniformBlockIndices.data() ) );
		std::vector< GlslAttributeType > uniformTypes;
		uniformTypes.resize( uniformCount );
		glLogCall( context
			, glGetActiveUniformsiv
			, program
			, uniformCount
			, uniformIndices.data()
			, GL_UNIFORM_TYPE
			, reinterpret_cast< GLint * >( uniformTypes.data() ) );
		std::vector< GLuint > uniformArraySizes;
		uniformArraySizes.resize( uniformCount );
		glLogCall( context
			, glGetActiveUniformsiv
			, program
			, uniformCount
			, uniformIndices.data()
			, GL_UNIFORM_SIZE
			, reinterpret_cast< GLint * >( uniformArraySizes.data() ) );
		std::vector< GLuint > uniformOffsets;
		uniformOffsets.resize( uniformCount );
		glLogCall( context
			, glGetActiveUniformsiv
			, program
			, uniformCount
			, uniformIndices.data()
			, GL_UNIFORM_OFFSET
			, reinterpret_cast< GLint * >( uniformOffsets.data() ) );

		for ( uint32_t index = 0; index < uniformCount; ++index )
		{
			if ( filter( uniformBlockIndices[index] ) )
			{
				func( uniformNames[index]
					, uniformTypes[index]
					, uniformIndices[index]
					, uniformOffsets[index]
					, uniformArraySizes[index] );
			}
		}
	}

	ConstantsLayout & getPushConstants( ContextLock const & context
		, ConstantsLayout & constants
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		getUniformInfos( context
			, stage
			, program
			, []( GLuint blockIndex ){ return blockIndex == -1; }
			, [&constants, &stage, &program]( const char * name
				, GlslAttributeType type
				, GLuint location
				, GLuint offset
				, GLuint arraySize )
			{
				if ( !isSampler( type )
					&& !isImage( type )
					&& !isSamplerBuffer( type )
					&& !isImageBuffer( type )
					&& location != -1 )
				{
					auto it = std::find_if( constants.begin()
						, constants.end()
						, [&name]( FormatDescT< ConstantFormat > const & desc )
						{
							return name == desc.name;
						} );
					if ( it != constants.end() )
					{
						it->location = uint32_t( location );
					}
				}
			} );
		return constants;
	}

	InterfaceBlocksLayout getUniformBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		InterfaceBlocksLayout result;
		getInterfaceInfos( context
			, program
			, GL_ACTIVE_UNIFORM_BLOCKS
			, GL_ACTIVE_UNIFORM_BLOCK_NAME_LENGTH
			, [&result, &context, &program, &stage]( GLint index, GLint maxLength )
			{
				ConstantBufferDesc desc;
				glLogCall( context
					, glGetActiveUniformBlockiv
					, program
					, index
					, GL_UNIFORM_BLOCK_BINDING
					, reinterpret_cast< GLint * >( &desc.binding ) );
				glLogCall( context
					, glGetActiveUniformBlockiv
					, program
					, index
					, GL_UNIFORM_BLOCK_DATA_SIZE
					, reinterpret_cast< GLint * >( &desc.size ) );
				std::vector< char > nameBuf;
				nameBuf.resize( maxLength );
				glLogCall( context
					, glGetActiveUniformBlockName
					, program
					, index
					, maxLength
					, nullptr
					, nameBuf.data() );
				desc.name = nameBuf.data();
				getUniformInfos( context
					, stage
					, program
					, [&index]( GLuint blockIndex ){ return blockIndex == index; }
					, [&desc, &stage, &program]( const char * name
						, GlslAttributeType type
						, GLuint location
						, GLuint offset
						, GLuint arraySize )
					{
						if ( !isSampler( type )
							&& !isImage( type )
							&& !isSamplerBuffer( type ) )
						{
							desc.constants.push_back( { program
								, stage
								, name
								, uint32_t( location )
								, getConstantFormat( type )
								, getSize( type )
								, uint32_t( arraySize )
								, ( offset == -1
									? 0u
									: uint32_t( offset ) ) } );
						}
					} );
				result.push_back( desc );
			} );
		return result;
	}

	InterfaceBlocksLayout getStorageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		InterfaceBlocksLayout result;
		return result;
	}

	SamplersLayout getSamplerBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		SamplersLayout result;
		getUniformInfos( context
			, stage
			, program
			, []( GLuint blockIndex ){ return blockIndex == -1; }
			, [&result, &stage, &program]( const char * name
				, GlslAttributeType type
				, GLuint location
				, GLuint offset
				, GLuint arraySize )
			{
				if ( isSamplerBuffer( type ) )
				{
					result.push_back( { program
						, stage
						, name
						, uint32_t( location )
						, getSamplerFormat( type )
						, 1u
						, uint32_t( arraySize )
						, 0u } );
				}
			} );
		return result;
	}

	SamplersLayout getSamplers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		SamplersLayout result;
		getUniformInfos( context
			, stage
			, program
			, []( GLuint blockIndex ){ return blockIndex == -1; }
			, [&result, &stage, &program]( const char * name
				, GlslAttributeType type
				, GLuint location
				, GLuint offset
				, GLuint arraySize )
			{
				if ( isSampler( type ) )
				{
					result.push_back( { program
						, stage
						, name
						, uint32_t( location )
						, getSamplerFormat( type )
						, 1u
						, uint32_t( arraySize )
						, 0u } );
				}
			} );
		return result;
	}

	ImagesLayout getImageBuffers( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		ImagesLayout result;
		getUniformInfos( context
			, stage
			, program
			, []( GLuint blockIndex )
			{
				return blockIndex == -1;
			}
			, [&result, &stage, &program]( const char * name
				, GlslAttributeType type
				, GLuint location
				, GLuint offset
				, GLuint arraySize )
			{
				if ( isImageBuffer( type ) )
				{
					result.push_back( { program
						, stage
						, name
						, uint32_t( location )
						, getImageFormat( type )
						, 1u
						, uint32_t( arraySize )
						, 0u } );
				}
			} );
		return result;
	}

	ImagesLayout getImages( ContextLock const & context
		, VkShaderStageFlagBits stage
		, GLuint program )
	{
		ImagesLayout result;
		getUniformInfos( context
			, stage
			, program
			, []( GLuint blockIndex ){ return blockIndex == -1; }
			, [&result, &stage, &program]( const char * name
				, GlslAttributeType type
				, GLuint location
				, GLuint offset
				, GLuint arraySize )
			{
				if ( isImage( type ) )
				{
					result.push_back( { program
						, stage
						, name
						, uint32_t( location )
						, getImageFormat( type )
						, 1u
						, uint32_t( arraySize )
						, 0u } );
				}
			} );
		return result;
	}
}
