# Neuro-Skyrim-Integration

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
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

### Build Output (Optional)
If you want to redirect the build output, set one of the following environment variable:

- Path to a Skyrim install folder: `SKYRIM_PATH`
- (example: `D:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition`)
- ***Resulting .dll file will be in `...\Skyrim Special Edition\Data\SKSE\Plugins\`***

### Project Generation (Optional)
If you use Visual Studio, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

