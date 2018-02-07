GlslangIncludeDir = path.join( VulkanSDKDir, "include" )
GlslangEnabled = false

if ( os.isdir( GlslangIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		GlslangLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		GlslangLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		includedirs{
			GlslangIncludeDir,
		}
		filter( "configurations:Debug" )
			libdirs { GlslangLibDirDebug }
			links{ "glslang" }
		filter( "configurations:Release" )
			libdirs { GlslangLibDirRelease }
			links{ "glslang" }
		GlslangEnabled = true
	else
		GlslangLibDir = path.join( VulkanSDKDir, "lib" )
		includedirs{
			GlslangIncludeDir,
		}
		libdirs { GlslangLibDir }
		links{ "glslang" }
		GlslangEnabled = true
	end
end
