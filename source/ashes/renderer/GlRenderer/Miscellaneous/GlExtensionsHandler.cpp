/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "GlExtensionsHandler.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>

#if _WIN32
#	define GLAPIENTRY __stdcall
#	include <gl/GL.h>
#else
#	define GLAPIENTRY
#endif

#include "ashesgl_api.hpp"

#define AshesGL_ForceMinVersion 0

namespace ashes::gl
{
	namespace
	{
		enum GlThings
		{
			GL_SPIRV_SHADER_BINARY_FORMATS = 0x8DF8,
			GL_SPIRV_NUM_SHADER_BINARY_FORMATS = 0x8DF9,
			GL_SPIRV_SPIR_V_BINARY = 0x9552,
			GL_SPIRV_SPIR_V_EXTENSIONS = 0x9553,
			GL_SPIRV_NUM_SPIR_V_EXTENSIONS = 0x9554,
		};

		using GLenum = unsigned int;
		using GLint = int;
		using GLubyte = unsigned char;
		using GLuint = unsigned int;

		using PFN_glGetError = GLenum( GLAPIENTRY * )( void );
		using PFN_glGetStringi = const GLubyte *( GLAPIENTRY * )( GLenum name, GLuint index );
		using PFN_glGetString = const GLubyte *( GLAPIENTRY * )( GLenum name );
		using PFN_glGetIntegerv = void ( GLAPIENTRY * )( GLenum pname, GLint * data );

		PFN_glGetStringi getStringi;
		PFN_glGetString getString;
		PFN_glGetIntegerv getIntegerv;

		bool isInCore( uint32_t value )
		{
			static uint32_t constexpr notInCore = makeVersion( NotInCore, NotInCore, 0 );
			return value == notInCore;
		}
	}

	void ExtensionsHandler::initialise()
	{
#if AshesGL_ForceMinVersion
		static int constexpr maxMajor = MinMajor;
		static int constexpr maxMinor = MinMinor;
#else
		static int constexpr maxMajor = MaxMajor;
		static int constexpr maxMinor = MaxMinor;
#endif

		std::stringstream errStream;
		getFunction( "glGetStringi", getStringi, errStream );

#ifndef _WIN32
		getFunction( "glGetString", getString, errStream );
		getFunction( "glGetIntegerv", getIntegerv, errStream );
#else
		getString = glGetString;
		getIntegerv = glGetIntegerv;
#endif

		char const * const cversion = ( char const * )getString( GL_VERSION );

		if ( cversion )
		{
			std::string sversion = cversion;
			std::stringstream stream( sversion );
			float fversion;
			stream >> fversion;
			int maxVersion = ( maxMajor * 10 ) + maxMinor;
			auto version = uint32_t( std::min( maxVersion, int( fversion * 10 ) ) );
			m_major = version / 10;
			m_minor = version % 10;
			m_version = makeVersion( m_major, m_minor, 0u );

			if ( version >= 33 )
			{
				m_shaderVersion = version * 10;
			}
			else if ( version >= 32 )
			{
				m_shaderVersion = 150;
			}
			else if ( version >= 31 )
			{
				m_shaderVersion = 140;
			}
			else
			{
				m_shaderVersion = 130;
			}
		}

		auto const * cextensions = ( char const * )getString( GL_EXTENSIONS );

		if ( cextensions )
		{
			std::string extensions = cextensions;
			std::istringstream stream{ extensions };
			std::copy( std::istream_iterator< std::string >( stream ),
				std::istream_iterator< std::string >(),
				std::back_inserter( m_deviceExtensionNames ) );
		}
		else
		{
			int max = 0;
			getIntegerv( GL_NUM_EXTENSIONS, &max );

			for ( int i = 0; i < max; ++i )
			{
				m_deviceExtensionNames.push_back( ( char const * )getStringi( GL_EXTENSIONS, i ) );
			}
		}
		

		int numSpirvExtensions = 0;
		getIntegerv( GL_SPIRV_NUM_SPIR_V_EXTENSIONS, &numSpirvExtensions );

		for ( auto index = 0; index < numSpirvExtensions; ++index )
		{
			auto const * cspirvext = ( char const * )getStringi( GL_SPIRV_SPIR_V_EXTENSIONS, index );

			if ( cspirvext )
			{
				m_deviceExtensionNames.emplace_back( cspirvext );
			}
		}

		int numBinaryFormats = 0;
		getIntegerv( GL_SPIRV_NUM_SHADER_BINARY_FORMATS, &numBinaryFormats );

		if ( numBinaryFormats > 0 )
		{
			m_shaderBinaryFormats.resize( numBinaryFormats );
			getIntegerv( GL_SPIRV_SHADER_BINARY_FORMATS, reinterpret_cast< int * >( m_shaderBinaryFormats.data() ) );
		}

		// Currently disabled, because I need to parse SPIR-V to retrieve push constant blocks...
		m_spirvSupported = false
			&& ( find( ARB_gl_spirv )
				|| hasSPIRVShaderBinaryFormat() );

		m_features.hasTexBufferRange = find( ARB_texture_buffer_range );
		m_features.hasImageTexture = findAll( { ARB_texture_storage, ARB_shader_image_load_store } );
		m_features.hasBaseInstance = find( ARB_base_instance );
		m_features.hasClearTexImage = find( ARB_clear_texture );
		m_features.hasComputeShaders = findAny( { ARB_compute_shader, ARB_gpu_shader5 } );
		m_features.hasStorageBuffers = findAll( { ARB_compute_shader, ARB_gpu_shader5, ARB_buffer_storage, ARB_shader_image_load_store, ARB_shader_storage_buffer_object } );
		m_features.supportsPersistentMapping = true;
		m_features.maxShaderLanguageVersion = m_shaderVersion;
	}

	bool ExtensionsHandler::find( VkExtensionProperties const & extension )const
	{
		return ( isInCore( extension.specVersion ) && ( m_version >= extension.specVersion ) )
			|| ( m_deviceExtensionNames.end() != std::find( m_deviceExtensionNames.begin()
				, m_deviceExtensionNames.end()
				, extension.extensionName ) );
	}

	bool ExtensionsHandler::findAny( VkExtensionPropertiesArray const & extensions )const
	{
		return extensions.end() != std::find_if( extensions.begin()
			, extensions.end()
			, [this]( VkExtensionProperties const & lookup )
			{
				return find( lookup );
			} );
	}

	bool ExtensionsHandler::findAll( VkExtensionPropertiesArray const & extensions )const
	{
		return extensions.end() == std::find_if( extensions.begin()
			, extensions.end()
			, [this]( VkExtensionProperties const & lookup )
			{
				return !find( lookup );
			} );
	}

	bool ExtensionsHandler::hasSPIRVShaderBinaryFormat()const
	{
		return m_shaderBinaryFormats.end() != std::find( m_shaderBinaryFormats.begin()
			, m_shaderBinaryFormats.end()
			, GL_SHADER_BINARY_FORMAT_SPIR_V );
	}
}
