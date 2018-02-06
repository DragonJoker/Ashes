OGLCompilerIncludeDir = path.join( VulkanSDKDir, "include" )
OGLCompilerEnabled = false

if ( os.isdir( OGLCompilerIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		OGLCompilerLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		OGLCompilerLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		if ( os.isdir( OGLCompilerLibDirDebug )
			and os.isdir( OGLCompilerLibDirRelease )
			and os.isfile( path.join( OGLCompilerLibDirDebug, "OGLCompiler.lib" ) )
			and os.isfile( path.join( OGLCompilerLibDirRelease, "OGLCompiler.lib" ) ) )
		then
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
		end
	else
		OGLCompilerLibDir = path.join( VulkanSDKDir, "lib" )

		if ( os.isdir( OGLCompilerLibDir )
			and os.isfile( path.join( OGLCompilerLibDir, "libOGLCompiler.so" ) ) )
		then
			includedirs{
				OGLCompilerIncludeDir,
			}
			libdirs { OGLCompilerLibDir }
			links{ "OGLCompiler" }
			OGLCompilerEnabled = true
		end
	end
end
