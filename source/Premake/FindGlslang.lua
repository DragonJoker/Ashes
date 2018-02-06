GlslangIncludeDir = path.join( VulkanSDKDir, "include" )
GlslangEnabled = false

if ( os.isdir( GlslangIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		GlslangLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		GlslangLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		if ( os.isdir( GlslangLibDirDebug )
			and os.isdir( GlslangLibDirRelease )
			and os.isfile( path.join( GlslangLibDirDebug, "glslang.lib" ) )
			and os.isfile( path.join( GlslangLibDirRelease, "glslang.lib" ) ) )
		then
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
		end
	else
		GlslangLibDir = path.join( VulkanSDKDir, "lib" )

		if ( os.isdir( GlslangLibDir )
			and os.isfile( path.join( GlslangLibDir, "libglslang.so" ) ) )
		then
			includedirs{
				GlslangIncludeDir,
			}
			libdirs { GlslangLibDir }
			links{ "glslang" }
			GlslangEnabled = true
		end
	end
end
