dofile "../Premake/FindwxWidgets.lua"

folders = os.matchdirs( "*" )

for i, folder in ipairs( folders ) do
	if ( folder ~= "Assets" ) then
		currentSourceDir = path.join( sourceDir, "Test", folder )
		currentBinaryDir = path.join( binaryDir, "Test", folder )

		project( "Test-" .. folder )

		location( currentBinaryDir )

		if ( folder == "00-Common" ) then
			kind( "StaticLib" )
			targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
			includedirs{
				path.join( sourceDir, "Utils", "Src" ),
				path.join( sourceDir, "Ashes", "Ashes", "Src" ),
				path.join( binaryDir, "Ashes", "Ashes", "Src" ),
				path.join( currentBinaryDir, "Src" ),
				path.join( currentSourceDir, "Src" )
			}
			links{
				"Utils",
				"Ashes"
			}
			postbuildcommands {
				"{COPY} " .. path.join( sourceDir, "Test", "Assets" ) .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, "Assets" )
			}
		else
			kind( "WindowedApp" )
			targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", executableDir ) )
			includedirs{
				path.join( sourceDir, "Utils", "Src" ),
				path.join( sourceDir, "Ashes", "Ashes", "Src" ),
				path.join( binaryDir, "Ashes", "Ashes", "Src" ),
				path.join( sourceDir, "Test", "00-Common", "Src" ),
				path.join( currentBinaryDir, "Src" ),
				path.join( currentSourceDir, "Src" )
			}
			links{
				"Utils",
				"Ashes",
				"Test-00-Common",
				binaryLinks
			}
			shadersFolder = path.join( sourceDir, "Test", folder, "Shaders" )
			if ( os.isdir( shadersFolder ) ) then
				postbuildcommands {
					"{COPY} " .. shadersFolder .. " " .. path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", assetsDir, folder, "Shaders" )
				}
			end
		end

		files{ currentSourceDir .. "/Src/**.hpp",
			currentSourceDir .. "/Src/**.inl",
			currentSourceDir .. "/Src/**.cpp"
		}

		vpaths{ ["Header Files"] = "**.hpp" }
		vpaths{ ["Header Files"] = "**.inl" }
		vpaths{ ["Source Files"] = "**.cpp" }

		filter( "configurations:Debug" )
			wx_config {Unicode="yes", Version="3.1", Libs="core,aui", Static="yes", Debug="yes"}
		filter( "configurations:Release" )
			wx_config {Unicode="yes", Version="3.1", Libs="core,aui", Static="yes"}
	end
end
