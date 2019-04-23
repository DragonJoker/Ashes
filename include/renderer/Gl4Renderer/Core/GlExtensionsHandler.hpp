/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl4Renderer/Enum/GlShaderBinaryFormat.hpp"

namespace ashes::gl4
{
	class ExtensionsHandler
	{
	public:
		void initialise();

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

	private:
		StringArray m_deviceExtensionNames;
		StringArray m_deviceSPIRVExtensionNames;
		std::vector< GlShaderBinaryFormat > m_shaderBinaryFormats;
		int m_major{ 0 };
		int m_minor{ 0 };
		int m_shaderVersion{ 0 };
		bool m_spirvSupported;
	};
}
