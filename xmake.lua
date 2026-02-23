


-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("lib/libneurosdk")
includes("lib/glaze")
includes("lib/xmake.lua")

-- set project
set_project("Neuro-Skyrim-Integration")
set_version("0.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)


local skyrim_path = os.getenv("SKYRIM_PATH")


-- targets
target("Neuro-Skyrim-Integration")
    -- add dependencies to target
    add_deps("commonlibsse")
	add_deps("libneurosdk")
	add_deps("glaze")
	
    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "Neuro-Skyrim-Integration",
        author = "vituha",
        description = "Skyrim integration for Neuro-Sama"
    })

    -- add src files
    add_files("src/**.cpp") 
    add_headerfiles("src/**.h", "src/**.hpp")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

	on_install(function (target)
        local target_file = target:targetfile()
        local plugin_folder = path.join(skyrim_path, "Data/SKSE/Plugins")

        os.mkdir(plugin_folder)
        os.cp(target_file, plugin_folder)
        os.cp(
            path.join(
                path.directory(target_file),
                path.basename(target_file) .. ".pdb" -- Evil hack #2
            ),
            plugin_folder
        )

        cprint("${bright green}Installed plugin to " .. plugin_folder)
    end)
	
	
	after_build(function (target)
	
		local target_file = target:targetfile()
        local plugin_folder = path.join(skyrim_path, "Data/SKSE/Plugins")

        os.mkdir(plugin_folder)
        os.cp(target_file, plugin_folder)
        os.cp(
            path.join(
                path.directory(target_file),
                path.basename(target_file) .. ".pdb" -- Evil hack #2
            ),
            plugin_folder
        )

        cprint("${bright green}Installed plugin to " .. plugin_folder)
		
    end)
	
	
	
	
	