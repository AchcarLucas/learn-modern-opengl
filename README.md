# learn-modern-opengl

Project created to learn OpenGL - refactored examples from the [learn opengl](https://learnopengl.com/) website

**[📖 README - Português do Brasil](./README-PT-BR.md)**

## 📋 Dependencies Installation (Linux)

### X11 Library Installation

```
sudo apt-get install doxygen xorg-dev
sudo apt install libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libglx-dev libgl1-mesa-dev libxxf86vm-dev
```

### GLFW Library Installation (GLFW-3.3.8)

Download the [GLFW-3.3.8](https://www.glfw.org/download) package

After downloading, extract and compile using the commands inside the ```glfw-3.3.8``` folder

```
cmake -S . -B build
cmake build
cd build
sudo make install
```

After completion, the terminal will display the installation locations for the libraries and includes as shown in the image below:

![glfw-install](./images/img_1.png)

### Assimp Installation (Assimp-5.3.1)

Download Assimp [assimp-5.3.1](https://github.com/assimp/assimp)
After downloading, extract and build it

```
cd assimp
cmake CMakeLists.txt 
cmake --build .
```

After building, copy the ```.so``` files to the ```/usr/local/lib``` folder and create a symlink of ```libassimp.so.5``` to the 
main folder of the cloned project

### FreeType 2 Installation (freetype-2.13.2)

Download freetype [freetype-2.13.2](https://freetype.org/)

After downloading, extract and build it

```
cd freetype-2.13.2
./configure --enable-static
make
```

The ```.a``` file should be in a hidden folder called ```.lib``` inside objs
Copy the ```libfreetype.a``` lib into the ```./lib/freetype-2.13.2/lib``` folder of the project

PS: The project already comes with the library compiled and configured for Linux (Ubuntu 24.04 - 6.2.0-37-generic)

## 🔧 CodeBlock Configuration

Verify if your CodeBlock configuration matches the following images:

![img_2](./images/img_2.png)
![img_3](./images/img_3.png)
![img_4](./images/img_4.png)
![img_5](./images/img_5.png)

Done, CodeBlock is configured

### 🚀 Running an Example

After finishing the installation of the libraries and the CodeBlock configuration, in the ```tests.hpp``` file change the examples by the definition ```#define run(width, height) run_XXX(width, height)```, where ```XXX``` is the number of the example you want.

## Example-1 (Draw Quad)
![ex_1](./images/ex_1.png)

## Example-2 (Draw Quad)
![ex_2](./images/ex_2.png)

## Example-3 (Draw Quad)
![ex_3](./images/ex_3.png)

## Example-4 (Draw Cube)
![ex_4](./images/ex_4.gif)

## Example-5 (Light Shader)
![ex_5](./images/ex_5.png)

## Example-6 (Material Shader)
![ex_6](./images/ex_6.gif)

## Example-7 (Material Shader)
![ex_7](./images/ex_7.gif)

## Example-8 (Emit Shader)
![ex_8](./images/ex_8.gif)

## Example-9 (Light Shader)
![ex_9](./images/ex_9.gif)

## Example-10 (Light Shader)
![ex_10](./images/ex_10.gif)

## Example-12 (Loading Model)
![ex_12](./images/ex_12.png)

## Example-14 (Outline Shader)
![ex_14](./images/ex_14.png)

## Example-15 (Transparent Shader)
![ex_15](./images/ex_15.png)

## Example-16 (Kernel Shader)
![ex_16](./images/ex_16.png)

## Example-17 (Kernel Shader)
![ex_17](./images/ex_17.png)

## Example-18 (Skybox Mapping)
![ex_18](./images/ex_18.png)

## Example-22 (Geometry Shader)
![ex_22](./images/ex_22.png)

## Example-23 (Geometry Shader)
![ex_23](./images/ex_23.png)

## Example-24 (Exploded Shader)
![ex_24](./images/ex_24.gif)

## Example-25 (Normal Vertex Shader)
![ex_25](./images/ex_25.png)

## Example-26 (Instances)
![ex_26](./images/ex_26.png)

## Example-27 (Instances)
![ex_27](./images/ex_27.png)

## Example-28 (Instances)
![ex_28](./images/ex_28.png)

## Example-29 (MSAA)
![ex_29](./images/ex_29.png)

## Example-30 (Blinn-Phong)
![ex_30](./images/ex_30.png)

## Example-31 (Gamma Correction)
![ex_31](./images/ex_31.png)

## Example-32 (Camera Depth Mapping)
![ex_32](./images/ex_32.gif)

## Example-33 (Directional Shadow Mapping)
![ex_33](./images/ex_33.gif)

## Example-34 (Point Shadow Mapping)
![ex_34](./images/ex_34.gif)

## Example-35 (Normal Mapping)
![ex_35](./images/ex_35.png)

## Example-37 (Model Normal Mapping)
![ex_37](./images/ex_37.png)

## Example-38 (Render Text)
![ex_38](./images/ex_38.png)

## Example-41 (Parallax Mapping)
![ex_40](./images/ex_40.png)

## Example-41 (Parallax Mapping Occlusion)
![ex_41_1](./images/ex_41_1.png)
![ex_41_2](./images/ex_41_2.png)

## Example-42 (Multi Shadow and Light)
![ex_42](./images/ex_42.png)

## Example-43 (HDR with Multi Light)
![ex_43](./images/ex_43.png)

## Example-44 (Bloom Without HDR)
![ex_44_1](./images/ex_44_1.png)

## Example-44 (Bloom With HDR)
![ex_44_1](./images/ex_44_2.png)

## Example-45 (Deferred Shader no Optimization)
![ex_45](./images/ex_45.png)

## Example-46 (Deferred Shader Simple Optimization)
![ex_46](./images/ex_46.png)

## Example-47 (SSAO - Ambient Occlusion)
![ex_47](./images/ex_47.gif)
