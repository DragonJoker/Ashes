if ( os.istarget( "Windows" ) )
then
	OGLCompilerRootDir = path.join( "..", "..", "dependencies", "glslang" )
else
	OGLCompilerRootDir = VulkanSDKDir
end

OGLCompilerIncludeDir = path.join( OGLCompilerRootDir, "include" )
OGLCompilerEnabled = false

if ( os.isdir( OGLCompilerIncludeDir ) )
then
	OGLCompilerLibDir = path.join( OGLCompilerRootDir, "lib" )
	includedirs{
		OGLCompilerIncludeDir,
	}
	libdirs { OGLCompilerLibDir }
	if ( os.istarget( "Windows" ) )
	then
		filter( "configurations:Debug" )
			links{ "OGLCompilerd" }
		filter( "configurations:Release" )
			links{ "OGLCompiler" }
	else
		links{ "OGLCompiler" }
	end
	OGLCompilerEnabled = true
end
