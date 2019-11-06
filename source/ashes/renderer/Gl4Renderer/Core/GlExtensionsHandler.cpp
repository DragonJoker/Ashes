/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/GlExtensionsHandler.hpp"

#if ASHES_XLIB
#	include <X11/Xlib.h>
#	include <GL/glx.h>
#endif

#include <GL/gl.h>

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sstream>

#ifdef max
#	undef max
#endif
#ifdef min
#	undef min
#endif

namespace ashes::gl4
{
	namespace
	{
		enum GlSpirvExtensions
		{
			GL_SPIRV_SHADER_BINARY_FORMATS = 0x8DF8,
			GL_SPIRV_NUM_SHADER_BINARY_FORMATS = 0x8DF9,
			GL_SPIRV_SPIR_V_BINARY = 0x9552,
			GL_SPIRV_SPIR_V_EXTENSIONS = 0x9553,
			GL_SPIRV_NUM_SPIR_V_EXTENSIONS = 0x9554,
		};

		using PFN_glGetStringi = const GLubyte *( GLAPIENTRY * )( GLenum name, GLuint index );
		PFN_glGetStringi glGetStringi;
	}

	void ExtensionsHandler::initialise()
	{
		getFunction( "glGetStringi", glGetStringi );
		char const * const cversion = ( char const * )glGetString( GL_VERSION );

		if ( cversion )
		{
			std::string sversion = cversion;
			std::stringstream stream( sversion );
			float fversion;
			stream >> fversion;
			auto version = std::min( int( fversion * 10 ), 42 );

			if ( version < 42 )
			{
				throw std::runtime_error{ "OpenGL >= 4.2 is needed for this renderer." };
			}

			m_major = version / 10;
			m_minor = version % 10;
			m_shaderVersion = version * 10;
		}

		auto const * cextensions = ( char const * )glGetString( GL_EXTENSIONS );

		if ( cextensions )
		{
			std::string extensions = cextensions;
			std::istringstream stream{ extensions };
			std::copy( std::istream_iterator< std::string >( stream ),
				std::istream_iterator< std::string >(),
				std::back_inserter( m_deviceExtensionNames ) );
		}

		int numSpirvExtensions = 0;
		glGetIntegerv( GL_SPIRV_NUM_SPIR_V_EXTENSIONS, &numSpirvExtensions );

		for ( auto index = 0; index < numSpirvExtensions; ++index )
		{
			auto const * cspirvext = ( char const * )glGetStringi( GL_SPIRV_SPIR_V_EXTENSIONS, index );

			if ( cspirvext )
			{
				m_deviceSPIRVExtensionNames.emplace_back( cspirvext );
			}
		}

		int numBinaryFormats = 0;
		glGetIntegerv( GL_SPIRV_NUM_SHADER_BINARY_FORMATS, &numSpirvExtensions );

		if ( numBinaryFormats > 0 )
		{
			m_shaderBinaryFormats.resize( numBinaryFormats );
			glGetIntegerv( GL_SPIRV_SHADER_BINARY_FORMATS, reinterpret_cast< int * >( m_shaderBinaryFormats.data() ) );
		}

		// Currently disabled, because I need to parse SPIR-V to retrieve push constant blocks...
		m_spirvSupported = false
			&& ( find( ARB_gl_spirv )
				|| hasSPIRVShaderBinaryFormat() );

		m_features.hasTexBufferRange = find( ARB_texture_buffer_range );
		m_features.hasImageTexture = findAll( { ARB_texture_storage, ARB_shader_image_load_store } );
		m_features.hasBaseInstance = find( ARB_base_instance );
		m_features.hasClearTexImage = find( ARB_clear_texture );
		m_features.hasComputeShaders = find( ARB_compute_shader );
		m_features.hasStorageBuffers = findAll( { ARB_compute_shader, ARB_buffer_storage, ARB_shader_image_load_store, ARB_shader_storage_buffer_object } );
		m_features.supportsPersistentMapping = true;
	}

	bool ExtensionsHandler::find( std::string const & name )const
	{
		return m_deviceExtensionNames.end() != std::find( m_deviceExtensionNames.begin()
			, m_deviceExtensionNames.end()
			, name );
	}

	bool ExtensionsHandler::findAny( StringArray const & names )const
	{
		return names.end() != std::find_if( names.begin()
			, names.end()
			, [this]( std::string const & name )
			{
				return find( name );
			} );
	}

	bool ExtensionsHandler::findAll( StringArray const & names )const
	{
		return names.end() == std::find_if( names.begin()
			, names.end()
			, [this]( std::string const & name )
			{
				return !find( name );
			} );
	}

	bool ExtensionsHandler::hasSPIRVShaderBinaryFormat()const
	{
		return m_shaderBinaryFormats.end() != std::find( m_shaderBinaryFormats.begin()
			, m_shaderBinaryFormats.end()
			, GL_SHADER_BINARY_FORMAT_SPIR_V );
	}
}
