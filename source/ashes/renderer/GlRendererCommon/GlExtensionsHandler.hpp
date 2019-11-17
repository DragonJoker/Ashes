/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommonPrerequisites.hpp"

#include <string>

namespace ashes::gl
{
#define makeGlExtension( x )\
	static const std::string x = "GL_"#x

	makeGlExtension( KHR_debug );
	makeGlExtension( ARB_debug_output );
	makeGlExtension( AMDX_debug_output );
	makeGlExtension( ARB_texture_buffer_range );
	makeGlExtension( ARB_texture_storage );
	makeGlExtension( ARB_shader_image_load_store );
	makeGlExtension( ARB_shader_storage_buffer_object );
	makeGlExtension( ARB_base_instance );
	makeGlExtension( ARB_clear_texture );
	makeGlExtension( ARB_compute_shader );
	makeGlExtension( ARB_buffer_storage );
	makeGlExtension( ARB_gl_spirv );
#undef makeGlExtension

	class ExtensionsHandler
	{
	public:
		void initialise( int reqMajor, int reqMinor
			, int maxMajor, int maxMinor );

		bool find( std::string const & name )const;
		bool findAny( StringArray const & names )const;
		bool findAll( StringArray const & names )const;
		bool hasSPIRVShaderBinaryFormat()const;

		inline int isSPIRVSupported()const
		{
			return m_spirvSupported;
		}

		inline int getMajor()const
		{
			return m_major;
		}

		inline int getMinor()const
		{
			return m_minor;
		}

		inline int getShaderVersion()const
		{
			return m_shaderVersion;
		}

		inline AshPluginFeatures const & getFeatures()const
		{
			return m_features;
		}

	private:
		StringArray m_deviceExtensionNames;
		StringArray m_deviceSPIRVExtensionNames;
		std::vector< uint32_t > m_shaderBinaryFormats;
		AshPluginFeatures m_features;
		int m_major{ 0 };
		int m_minor{ 0 };
		int m_shaderVersion{ 0 };
		bool m_spirvSupported;
	};
}
