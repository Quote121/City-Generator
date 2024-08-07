# City-Generator
Procedural city generator written from scratch in OpenGL and C++.

## Running
In the project directory you need to create a build directory to build the solution in.
```bash
mkdir build
cd build
```

Once we are in the directory we then generate the build files
```bash
cmake ..
```

After the project makefiles have been built we can then build the solution.
```bash
make
```

To run the project you can directly run the executable or run
```
make run
```
## Controls
- Translating the camera can be done with ```w```, ```a```, ```s```, ```d```. The ```mouse``` is used to control pitch and yaw and the ```scroll wheel``` is used for zoom.

- To toggle the mouse window capture ```RMB (right mouse button)``` can be used or with ```o``` and ```p```.

## Usage
The menu will have several drop down elements to allow you to control various elements of the world and the objects within it. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/434b9387-4bd7-44ab-8ba9-4df59d6525ae" />
</p>

- Generator settings - (Controls the generation of cities. Previous seeds can be used for generating the same cities.)
- World controls - (Controls the elements of the world such as skybox visibility and axis line visibility.)
- Spawning - (Gives a list of loaded assets to spawn into the world*. Including models, sprites and lights.)
- Objects - (Gives a list of all objects in the scene, they can be selected and modified from the menu.)

<br>

Models can be edited via the `Objects` tab in the control menu or by clicking on them with ```LMB```. A box will highlight around them to indicate they are selected and a dialogue box will appear with its properties as shown below. 

<p align="center">
  <img src="https://github.com/user-attachments/assets/1d7feeb0-32ab-4f06-a8cb-0287e7ed4ad8"/width=570>
</p>

*Note: custom objects can be spawned into the world by adding them to the ```City-Generator/assets/PreLoadList.txt``` file.

# Images

<p align="center">
  <img src="https://github.com/user-attachments/assets/a17f9b8c-8c84-4299-93a5-c8e0d2a4766a"/width=700>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/6c4101f5-d07b-4853-9b93-267f32fdf752"/width=700>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/377163a9-1bf5-4f46-a822-b274198376f6"/width=700>
</p>


<br><br>

Libraries and headers used:
- assimp asset loader - https://github.com/assimp/assimp
- glfw - https://www.glfw.org/
- stb_image.h - https://github.com/nothings/stb/blob/master/stb_image.h
- glad.h - https://github.com/go-gl/glfw/blob/master/v3.1/glfw/glfw/deps/glad/glad.h
- ImGui - https://github.com/ocornut/imgui
- Building assets - www.kenney.nl
