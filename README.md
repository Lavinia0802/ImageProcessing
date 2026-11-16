# ImageProcessing

ImageProcessing is a native Windows desktop image editor written in C++20. It
uses Dear ImGui for the user interface, GLFW and OpenGL for the application
window and image preview, and GDI+ for loading and saving image files.

## What it does

The application provides a simple image-editing workflow:

- Open PNG, JPEG, BMP, or GIF images.
- Preview the current image in the editor window.
- Apply grayscale, invert, sepia, or brightness filters.
- Process filters asynchronously so the UI remains responsive.
- Undo and redo filter operations.
- Reset the image to its original loaded state.
- Save the edited image as PNG, JPEG, or BMP.
- Remove the current image and start over.

The default brightness adjustment is `+30`. Image history is kept in memory
for the current editing session.

## Requirements

- Windows 10 or later
- Visual Studio with:
  - Desktop development with C++
  - The `v145` MSVC platform toolset
  - A Windows 10 SDK
- vcpkg

The project uses vcpkg manifest mode. The manifest is located at
[`ImageProcessing/vcpkg.json`](ImageProcessing/vcpkg.json) and declares:

- GLFW
- Dear ImGui with GLFW and OpenGL 3 bindings

GDI+, OpenGL, and the common dialog library are provided by Windows and are
linked by the Visual C++ project.

## Build

1. Install Visual Studio with the requirements above.
2. Install vcpkg and set the `VCPKG_ROOT` environment variable to its
   installation directory.
3. Open [`ImageProcessing.slnx`](ImageProcessing.slnx) in Visual Studio.
4. Select the `x64` platform and either the `Debug` or `Release` configuration.
5. Build the solution. Visual Studio restores the manifest dependencies and
   builds the application.
6. Run the `ImageProcessing` project.

The executable is produced under the project's configuration output directory,
for example:

```text
ImageProcessing\x64\Release\ImageProcessing.exe
```

### Command-line build

From a Visual Studio Developer PowerShell, run:

```powershell
msbuild ImageProcessing.slnx /p:Configuration=Release /p:Platform=x64
```

For a debug build:

```powershell
msbuild ImageProcessing.slnx /p:Configuration=Debug /p:Platform=x64
```

## Usage

Launch the executable, choose **Open Image**, and select an image. Once an
image is loaded, use the filter buttons in the right-hand panel. Use **Undo**,
**Redo**, or **Reset** to manage the edit history, then choose **Save Image**
to export the result.