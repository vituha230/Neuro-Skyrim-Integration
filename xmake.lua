


-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")
includes("lib/xmake.lua")

-- set project
set_project("Neuro-Skyrim-Integration")
set_version("0.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
--add_rules("mode.debug", "mode.releasedbg")
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)


local skyrim_path = os.getenv("SKYRIM_PATH")

set_defaultmode("debug")

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

task("tasks")
    set_menu {
        usage = "xmake tasks",
        description = "Lists only custom tasks.",
        options = {},
    }
    on_run(function ()
        local help_output = os.iorun("xmake --help")
        if not help_output then
            raise("Failed to capture help output.")
        end
        local tasks_section = help_output:match("Tasks:(.-)Common options:")
        if not tasks_section then
            raise ("Failed to parse tasks.")
        end
        cprint("${bright blue}Tasks:${clear}")
        cprint(tasks_section)
    end)

task("setup-skyrim-path")
    set_menu {
        usage = "xmake setup-skyrim-path [path]",
        description = "Sets SKYRIM_PATH to provided/discovered/interactive value, if verified.",
        options = {
            { 'p', "path", "v", nil, "The path to the game directory." },
        },
    }
    on_run(function ()
        import("core.base.option")

        -- Prefer provided
        local skypath = option.get("path") or (args and args[1]) -- trick to accept path as 1st arg, without requiring flag name
        -- 1st fallback: discover (via registry)
        if not skypath then -- should work if Skyrim was installed via steam and launched as least once
            skypath = vformat([[$(reg HKLM\SOFTWARE\WOW6432Node\Bethesda Softworks\Skyrim Special Edition;installed path)]]):gsub("\\$", "") -- AE/SE share the same key
        end
        -- 2nd fallback: prompt user
        if not skypath then
            cprint("Enter the full path to your Skyrim Special Edition/Anniversary folder:")
            io.flush() -- Ensure print precedes read
            skypath = io.read()
            if not skypath or skypath == "" then
                raise("No path provided.")
            end
            skypath = path.translate(
                skypath:trim()
                :gsub("%%(.-)%%", function(winvar) return os.getenv(winvar) end) -- resolve windows vars like %ProgramFiles(x86)%
                :gsub("\"", "") -- strip quotes
            ) 
        end

        -- verify
        if not os.isdir(skypath) then
            raise("Error: The directory '%s' does not exist.", skypath)
        elseif not os.isdir(path.join(skypath, "Data")) then
            raise("Error: '%s' does not appear to be a valid Skyrim directory (missing Data folder).", skypath)
        elseif not os.isfile(path.join(skypath, "SkyrimSE.exe")) then
            raise("Error: '%s' does not contain a supported version of Skyrim (both Special & Anniversay editions use 'SkyrimSE.exe').", skypath)
        end
        cprint("Path verified!")

        -- SET
        local ps_command = string.format(
            "[System.Environment]::SetEnvironmentVariable('SKYRIM_PATH', '%s', 'User')", 
            skypath
        )
        os.execv("powershell", {"-Command", ps_command}) -- Just let this exception throw if it fails
        cprint("SUCCESS!")
        cprint("Before: %s", skyrim_path)
        cprint("After:  %s", skypath)
        cprint("NOTE: You must RESTART your terminal/ide for this to take effect.")
    end)

task("launch")
    set_menu {
        usage = "xmake launch",
        description = "Launches Skyrim via SKSE.",
        options = {},
    }
    on_run(function ()
        local skse_loader = path.join(skyrim_path, "skse64_loader.exe")
        if not os.isfile(skse_loader) then
            raise("SKSE loader not found at: ", skse_loader)
        end
        os.execv(skse_loader, {}, {curdir = skyrim_path, detach = true})
    end)

task("close")
    set_menu {
        usage = "xmake launch",
        description = "Close Skyrim.",
        options = {},
    }
    on_run(function ()
        os.execv("taskkill", {"/F", "/T", "/IM", "SkyrimSE.exe"})
    end)

task("setup-skse")
    set_menu {
        usage = "xmake setup-skse [--reinstall]",
        description = "Verifies SKSE is in place; otherwise, sets it up.",
        options = {
            { 'r', "reinstall", "k", nil, "Force re-install."},
        },
    }
    on_run(function ()
        import("core.base.option")
        import("utils.archive")

        if not option.get("reinstall") and os.isfile(path.join(skyrim_path, "skse64_loader.exe")) then
            cprint("SKSE already setup. If you wish to update, overwrite with latest via 'xmake setup-skse --reinstall'")
            return
        end

        local user_downloads_folder = vformat([[$(reg HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders;{374DE290-123F-4565-9164-39C4925E467B})]])
        local skse_download_folder = path.join(user_downloads_folder, "SKSE")
        if os.isdir(skse_download_folder) then
            os.rm(skse_download_folder)
        end
        os.mkdir(skse_download_folder)

        local skse_site = "https://skse.silverlock.org/"
        local tmp_html = path.join(skse_download_folder, "skse_site.html")
        os.execv("powershell", {
            "-NoProfile", 
            "-Command", 
            "Invoke-WebRequest", 
            "-Uri", skse_site, 
            "-OutFile", tmp_html
        })
        local html_content = io.readfile(tmp_html)
        os.rm(tmp_html)
        -- Find the .7z link for the AE build, pattern: beta/skse64_X_XX_XX.7z
        local latest_link = html_content:match("beta/skse64_[%d_]+%.7z")
        if not latest_link then
            raise("Could not parse the latest SKSE download link from the website.")
        end
        local full_link = skse_site .. latest_link
        local skse_version = latest_link:match("skse64_[%d_]+")
        local skse_zip = path.join(skse_download_folder, string.format("%s.7z", skse_version))
        cprint("DOWNLOADING skse from: %s", full_link)
        cprint("to: %s", skse_zip)
        os.execv("powershell", {
            "-NoProfile", 
            "-Command", 
            "Invoke-WebRequest", 
            "-Uri", full_link, 
            "-OutFile", skse_zip
        })
        if not os.isfile(skse_zip) then
            raise("Download failed!")
        end
        archive.extract(skse_zip, skse_download_folder)
        local skse_extract = path.join(skse_download_folder, skse_version)
        cprint("EXTRACTING contents of %s: %s", skse_version, skse_extract)
        cprint("to: %s", skyrim_path)
        os.cp(path.join(skse_extract, "*"), skyrim_path)

        local skse_addresses_extract = path.join(skyrim_path, "Data", "SKSE", "Plugins") -- requires xmake build first?
        local addresses_zip = path.join(os.projectdir(), "SKSE_addresses.zip")
        cprint("EXTRACTING: %s", addresses_zip)
        cprint("to: %s", skse_addresses_extract)
        archive.extract(addresses_zip, skse_addresses_extract)
    end)

task("deploy")
    set_menu {
        usage = "xmake deploy",
        description = "Backs up scripts, deploys mod files, and registers the ESP.",
        options = {},
    }
    on_run(function ()
        local scripts_dir = path.join(skyrim_path, "Data", "Scripts")
        if not os.isdir(scripts_dir) then
            raise("Script dir not found! Checked: %s", scripts_dir)
        end
        local backup_name = scripts_dir .. "_backup"
        if os.isdir(backup_name) then
            os.rm(backup_name)
        end
        cprint("Backing up Scripts to: ", backup_name)
        os.cp(scripts_dir, backup_name)

        local data_dir = path.join(skyrim_path, "Data")
        if not os.isdir(data_dir) then
            raise("Data dir not found! Checked: %s", data_dir)
        end
        local compiled_scripts = path.join(os.projectdir(), "scripts_compiled", "Scripts")
        if not os.isdir(compiled_scripts) then
            raise("scripts_compiled/Scripts dir not found! Checked: %s", compiled_scripts)
        end
		
		local scripts_dir = path.join(data_dir, "Scripts")
		
        cprint("Deploying compiled scripts to: %s", scripts_dir)
        os.cp(path.join(compiled_scripts, "*"), scripts_dir)

        local esp = path.join(os.projectdir(), "mysc.esp")
        if not os.isfile(esp) then
            raise("esp file not found! Checked: %s", esp)
        end
        cprint("Transferring mysc.esp to: %s", data_dir)
        os.cp(esp, data_dir)

        local plugins_dir = path.join(path.translate(os.getenv("localappdata")), "Skyrim Special Edition")
        local plugins_txt = path.join(plugins_dir, "Plugins.txt")
        local plugin_line = "*mysc.esp"
        cprint("Updating Plugins at: %s", plugins_txt)
        if not os.isfile(plugins_txt) then
            if not os.isdir(plugins_dir) then
                os.mkdir(plugins_dir)
            end
            local contents = "# This file is used by Skyrim to keep track of your downloaded content."
            contents = contents .. "\n" .. "# Please do not modify this file."
            contents = contents .. "\n" .. plugin_line
            io.writefile(plugins_txt, contents)
        else
            local content = io.readfile(plugins_txt)
            if not content:find(plugin_line, 1, true) then
                local file = io.open(plugins_txt, "a")
                file:write(plugin_line)
                file:close()
            else
                cprint("Plugins.txt already up-to-date.")
            end
        end

        cprint("Success!")
    end)

task("debug")
    set_menu {
        usage = "xmake debug",
        description = "Build debug version (requires git repos for skse64/common, see readme).",
        options = {},
    }
    on_run(function ()
        cprint("${bright}Building Common Library...")
        os.exec([[cmake -B common/build -S common -DCMAKE_INSTALL_PREFIX=extern common]])
        os.exec([[cmake --build common/build --config Debug --target install]])

        cprint("${bright}Building SKSE64...")
        os.exec([[cmake -B skse64/build -S skse64 -DCMAKE_INSTALL_PREFIX=extern skse64]])
        os.exec([[cmake --build skse64/build --config Debug]])

        cprint("${bright blue}Deploying SKSE components to Skyrim...")
        os.cp("skse64/build/skse64_loader/Debug/*", skyrim_path)
        os.cp("skse64/build/skse64/Debug/*", skyrim_path)
    end)

task("first-time-setup")
    set_menu {
        usage = "xmake first-time-setup",
        description = "Attempts to run through all setups steps in sequence",
        options = {},
    }
    on_run(function ()
        import("core.base.task")

        task.run("setup-skyrim-path")
        task.run("build")
        task.run("setup-skse")
        task.run("deploy")
        task.run("launch")
        cprint("Success!")
    end)
