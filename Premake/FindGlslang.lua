if ( os.istarget( "Windows" ) )
then
	GlslangRootDir = path.join( "..", "..", "dependencies", "glslang" )
else
	GlslangRootDir = VulkanSDKDir
end

GlslangIncludeDir = path.join( GlslangRootDir, "include" )
GlslangEnabled = false

if ( os.isdir( GlslangIncludeDir ) )
then
	GlslangLibDir = path.join( GlslangRootDir, "lib" )
	includedirs{
		GlslangIncludeDir,
	}
	libdirs { GlslangLibDir }
	if ( os.istarget( "Windows" ) )
	then
		filter( "configurations:Debug" )
			links{ "glslangd" }
		filter( "configurations:Release" )
			links{ "glslang" }
	else
		links{ "glslang" }
	end
	GlslangEnabled = true
end
