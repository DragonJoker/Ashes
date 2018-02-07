AssimpIncludeDir = path.join( AssimpRootDir, "include" )
AssimpEnabled = false

if ( os.isdir( AssimpIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		AssimpLibDir = path.join( AssimpRootDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}" )
		includedirs{
			AssimpIncludeDir,
		}
		libdirs { AssimpLibDir }
		filter( "configurations:Debug" )
			links{ "assimp-vc140-mtd.lib" }
		filter( "configurations:Release" )
			links{ "assimp-vc140-mt.lib" }
		filter {}
	else
		AssimpLibDir = path.join( AssimpRootDir, "lib" )
		includedirs{
			AssimpIncludeDir,
		}
		libdirs { AssimpLibDir }
		links{ "assimp" }
	end
end
