/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	class ExtensionsHandler
	{
	public:
		void initialise();

		bool find( std::string const & name )const;
		bool findAny( ashes::StringArray const & names )const;
		bool findAll( ashes::StringArray const & names )const;
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

	private:
		ashes::StringArray m_deviceExtensionNames;
		ashes::StringArray m_deviceSPIRVExtensionNames;
		std::vector< GlShaderBinaryFormat > m_shaderBinaryFormats;
		int m_major{ 0 };
		int m_minor{ 0 };
		int m_shaderVersion{ 0 };
		bool m_spirvSupported;
	};
}
