require("cmake")

workspace("SDLGameOfLife")
	configurations({"Debug","Release"})
	platforms ({"Windows","WindowsMingw32","Linux"})
		
	project("SDLGameOfLife")
		kind("ConsoleApp")
		language("C++")
		targetdir("bin/%{cfg.buildcfg}")

		-- includedirs({
		-- 	"../SDL2-2.0.9/include"
		-- })

		-- libdirs({
		-- 	"../SDL2-2.0.9/lib/x86"
		-- })

		files({"**.h","**.cpp","**.lua"})

        filter("configurations:Debug")
            defines({"DEBUG"})
            symbols("On")

        filter("configurations:Release")
            defines({"NDEBUG"})
			optimize("On")
			
		filter("platforms:Windows")
            defines({"WINDOWS"})
			system("windows")
			includedirs({
				"../SDL2-2.0.9/include"
			})
	
			libdirs({
				"../SDL2-2.0.9/lib/x86"
			})
			links({
				"SDL2main",
				"SDL2"
			})
		
		filter("platforms:WindowsMingw32")
            defines({"WINDOWS"})
			system("windows")
			includedirs({
				"../SDL2-2.0.9/include"
			})
	
			libdirs({
				"../SDL2-2.0.9/lib/x86"
			})
			links({
				"Mingw32",
				"SDL2main",
				"SDL2"
			})

		filter("platforms:Linux")
            defines({"LINUX"})
			system("linux")
			links({
				"SDL2main",
				"SDL2"
			})
