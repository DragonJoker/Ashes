OSDependentIncludeDir = path.join( VulkanSDKDir, "include" )
OSDependentEnabled = false

if ( os.isdir( OSDependentIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		OSDependentLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		OSDependentLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		if ( os.isdir( OSDependentLibDirDebug )
			and os.isdir( OSDependentLibDirRelease )
			and os.isfile( path.join( OSDependentLibDirDebug, "OSDependent.lib" ) )
			and os.isfile( path.join( OSDependentLibDirRelease, "OSDependent.lib" ) ) )
		then
			includedirs{
				OSDependentIncludeDir,
			}
			filter( "configurations:Debug" )
				libdirs { OSDependentLibDirDebug }
				links{ "OSDependent" }
			filter( "configurations:Release" )
				libdirs { OSDependentLibDirRelease }
				links{ "OSDependent" }
			OSDependentEnabled = true
		end
	else
		OSDependentLibDir = path.join( VulkanSDKDir, "lib" )

		if ( os.isdir( OSDependentLibDir )
			and os.isfile( path.join( OSDependentLibDir, "libOSDependent.so" ) ) )
		then
			includedirs{
				OSDependentIncludeDir,
			}
			libdirs { OSDependentLibDir }
			links{ "OSDependent" }
			OSDependentEnabled = true
		end
	end
end
