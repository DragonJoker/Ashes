/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <string>

namespace ashes::gl
{
#define makeGlExtension( x )\
	static const std::string x = "GL_"#x

	makeGlExtension( AMDX_debug_output );
	makeGlExtension( ARB_base_instance );
	makeGlExtension( ARB_buffer_storage );
	makeGlExtension( ARB_clear_texture );
	makeGlExtension( ARB_clip_control );
	makeGlExtension( ARB_compute_shader );
	makeGlExtension( ARB_copy_image );
	makeGlExtension( ARB_debug_output );
	makeGlExtension( ARB_draw_buffers_blend );
	makeGlExtension( ARB_get_program_binary );
	makeGlExtension( ARB_gl_spirv );
	makeGlExtension( ARB_internalformat_query );
	makeGlExtension( ARB_internalformat_query2 );
	makeGlExtension( ARB_invalidate_subdata );
	makeGlExtension( ARB_multi_draw_indirect );
	makeGlExtension( ARB_query_buffer_object );
	makeGlExtension( ARB_sample_shading );
	makeGlExtension( ARB_separate_shader_objects );
	makeGlExtension( ARB_shader_image_load_store );
	makeGlExtension( ARB_shader_storage_buffer_object );
	makeGlExtension( ARB_tessellation_shader );
	makeGlExtension( ARB_texture_buffer_range );
	makeGlExtension( ARB_texture_storage );
	makeGlExtension( ARB_texture_storage_multisample );
	makeGlExtension( ARB_texture_view );
	makeGlExtension( ARB_viewport_array );
	makeGlExtension( ARB_ES2_compatibility );
	makeGlExtension( EXT_polygon_offset_clamp );
	makeGlExtension( KHR_debug );
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
