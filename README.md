# Simple Model Viewer
A simple OpenGL-based application for displaying static 3D models with a single diffuse texture

![Model Viewer 1](screenshots/ModelViewer.png)

## Features

- Orbital camera
- Phong lighting model
- Loading model and texture via drag-and-drop
- Supported model formats: Common formats supported by [Assimp](https://github.com/assimp/assimp/blob/master/doc/Fileformats.md)
- Supported texture formats: `png`, `jpg`

## Limitations
- Static models only
- Single diffuse texture per model

## Prerequisites

- Windows 11
- Visual Studio 2022

## Build Instructions

1. Run `generate_vs2022.bat` script
2. Open `SimpleModelViewer.sln` solution
3. Select desired configuration (`Debug` or `Release`)
4. Click `F5` to build and run

## Controls

- **Rotate camera:** LMB + Drag  
- **Zoom camera:** Mouse Wheel 

## Credits

### Default Texture
- "wooden crate texture" by **dementiaRunner**  
  Source: [https://www.deviantart.com/dementiarunner/art/wooden-crate-texture-439676818](https://www.deviantart.com/dementiarunner/art/wooden-crate-texture-439676818)
