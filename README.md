# Neuro-Skyrim-Integration

### Requirements
* `Skyrim Special Edition 1.6.1170` or `Skyrim Anniversary Edition 1.6.1170` (anniversary edition is untested) (latest steam release)
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
Clone the repository:
```bat
git clone --recurse-submodules https://github.com/vituha230/Neuro-Skyrim-Integration
cd Neuro-Skyrim-Integration
```

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Build Output
If you want to redirect the build output, set the following environment variable:

- Path to Skyrim install folder: `SKYRIM_PATH`
- (example: `D:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition`)
- ***Resulting .dll file will be in `...\Skyrim Special Edition\Data\SKSE\Plugins\`***

Alternatively, from `\Neuro-Skyrim-Integration\build\windows\x64\debug` manually copy `Neuro-Skyrim-Integration.dll` into `...\Skyrim Special Edition\Data\SKSE\Plugins\` folder. If the folder does not exist, create it.


### Project Generation (Optional)
If you use Visual Studio, run the following command:
```bat
xmake project -k vsxmake
```
> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### SKSE Installation
The mod requires the runtime component of SKSE to run.
- Follow the link: https://skse.silverlock.org/
- Download archive: `Current Anniversary Edition build 2.2.6 (game version 1.6.1170)` (first link)
- ***Both Anniversary and Special edition 1.6.1170 use the same game version***
- Extract archive contents into Skyrim install folder
- Copy `mysc.esp` file from repository into `...\Skyrim Special Edition\Data\` directory. This file is required for some core mechanics of the mod to work.
- At `%appdata%\Local\Skyrim Special Edition\` edit the `Plugins.txt` file: add `*mysc.esp` line in the end (asterisk required). If the file does not exist, create it. Example file contents (for me it has been created automatically):
```
# This file is used by Skyrim to keep track of your downloaded content.
# Please do not modify this file.
*mysc.esp
```
**THE GAME HAS TO BE RUN USING `skse64_loader.exe` FOR THE MOD TO WORK. However, you may run the game launcher (from steam or manually from game folder) to change graphics settings at any time.**
