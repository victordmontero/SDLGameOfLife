-- require("cmake")

workspace("SDLGameOfLife")
	configurations({"Debug","Release"})
	platforms ({"Windows","Linux"})
		
	project("SDLGameOfLife")
		kind("ConsoleApp")
		language("C++")
		targetdir("bin/%{cfg.buildcfg}")

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
