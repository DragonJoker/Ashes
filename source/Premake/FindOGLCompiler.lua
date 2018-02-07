OGLCompilerIncludeDir = path.join( VulkanSDKDir, "include" )
OGLCompilerEnabled = false

if ( os.isdir( OGLCompilerIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		OGLCompilerLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		OGLCompilerLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		includedirs{
			OGLCompilerIncludeDir,
		}
		filter( "configurations:Debug" )
			libdirs { OGLCompilerLibDirDebug }
			links{ "OGLCompiler" }
		filter( "configurations:Release" )
			libdirs { OGLCompilerLibDirRelease }
			links{ "OGLCompiler" }
		OGLCompilerEnabled = true
	else
		OGLCompilerLibDir = path.join( VulkanSDKDir, "lib" )
		includedirs{
			OGLCompilerIncludeDir,
		}
		libdirs { OGLCompilerLibDir }
		links{ "OGLCompiler" }
		OGLCompilerEnabled = true
	end
end
