# learn-modern-opengl

Projeto criado para aprender opengl - exemplos com refatorações do site [learn opengl](https://learnopengl.com/)

## 📋 Instalações das dependências (Linux)

### Instalação da lib X11

```
sudo apt-get install doxygen dnf xorg-dev
sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```

### Instalação da lib GLFW (GLFW-3.3.8)

Baixar o pacote [GLFW-3.3.8](https://www.glfw.org/download)

Após baixar, extrair e faça a compilação usando os comandos dentro da pasta ```glfw-3.3.8```

```
cmake -S . -B build
cmake build
cd build
make build
sudo make install
```

Ao finalizar, será exibido no terminal, os locais de instalações das libs e os includes como pode ser visto na imagem abaixo:

![glfw-install](./images/img_1.png)

### Instalação do Assimp (Assimp-5.3.1)

Baixe o Assimp [assimp-5.3.1](https://github.com/assimp/assimp)
após baixar, extrair e faça a sua build

```
cd assimp
cmake CMakeLists.txt 
cmake --build .
```

após fazer a build, copie os arquivos ```.so``` para a pasta ```/usr/local/lib``` e crie um atalho do ```libassimp.so.5``` para a 
pasta principal do projeto clonado

### Instalação do FreeType 2 (freetype-2.13.2)

Baixe o freetype [freetype-2.13.2](https://freetype.org/)

Após baixar, extrai e faça a build

```
cd freetype-2.13.2
./configure --enable-static
make
```

O arquivo ```.a```  deverá estar em uma pasta oculta chamada ```.lib``` dentro de objs

PS: O projeto já consta com a lib compilada e configurada para linux (Ubuntu 24.04 - 6.2.0-37-generic)

## 🔧 Configuração do CodeBlock

Verifique se a configuração do seu codeblock como segue as imagens a seguir:

![img_2](./images/img_2.png)
![img_3](./images/img_3.png)
![img_4](./images/img_4.png)
![img_5](./images/img_5.png)

Pronto, o Codeblock está configurado

### 🚀 Executando um exemplo

Após finalizar a instalação das libs e a configuração do codeblock, no arquivo ```tests.hpp``` mude os exemplos pela definition ```#define run(width, height) run_XXX(width, height)``` , onde o ```XXX``` é o número do exemplo que você deseja.

## Exemplo-1 (Draw Quad)
![ex_1](./images/ex_1.png)

## Exemplo-2 (Draw Quad)
![ex_2](./images/ex_2.png)

## Exemplo-3 (Draw Quad)
![ex_3](./images/ex_3.png)

## Exemplo-4 (Draw Cube)
![ex_4](./images/ex_4.gif)

## Exemplo-5 (Light Shader)
![ex_5](./images/ex_5.png)

## Exemplo-6 (Material Shader)
![ex_6](./images/ex_6.gif)

## Exemplo-7 (Material Shader)
![ex_7](./images/ex_7.gif)

## Exemplo-8 (Emit Shader)
![ex_8](./images/ex_8.gif)

## Exemplo-9 (Light Shader)
![ex_9](./images/ex_9.gif)

## Exemplo-10 (Light Shader)
![ex_10](./images/ex_10.gif)

## Exemplo-12 (Loading Model)
![ex_12](./images/ex_12.png)

## Exemplo-14 (Outline Shader)
![ex_14](./images/ex_14.png)

## Exemplo-15 (Transparent Shader)
![ex_15](./images/ex_15.png)

## Exemplo-16 (Kernel Shader)
![ex_16](./images/ex_16.png)

## Exemplo-17 (Kernel Shader)
![ex_17](./images/ex_17.png)

## Exemplo-18 (Skybox Mapping)
![ex_18](./images/ex_18.png)

## Exemplo-22 (Geometry Shader)
![ex_22](./images/ex_22.png)

## Exemplo-23 (Geometry Shader)
![ex_23](./images/ex_23.png)

## Exemplo-24 (Exploded Shader)
![ex_24](./images/ex_24.gif)

## Exemplo-25 (Normal Vertex Shader)
![ex_25](./images/ex_25.png)

## Exemplo-26 (Instances)
![ex_26](./images/ex_26.png)

## Exemplo-27 (Instances)
![ex_27](./images/ex_27.png)

## Exemplo-28 (Instances)
![ex_28](./images/ex_28.png)

## Exemplo-29 (MSAA)
![ex_29](./images/ex_29.png)

## Exemplo-30 (Blinn-Phong)
![ex_30](./images/ex_30.png)

## Exemplo-31 (Gamma Correction)
![ex_31](./images/ex_31.png)

## Exemplo-32 (Camera Depth Mapping)
![ex_32](./images/ex_32.gif)

## Exemplo-33 (Directional Shadow Mapping)
![ex_33](./images/ex_33.gif)

## Exemplo-34 (Point Shadow Mapping)
![ex_34](./images/ex_34.gif)

## Exemplo-35 (Normal Mapping)
![ex_35](./images/ex_35.png)

## Exemplo-37 (Model Normal Mapping)
![ex_37](./images/ex_37.png)

## Exemplo-38 (Render Text)
![ex_38](./images/ex_38.png)

## Exemplo-41 (Parallax Mapping)
![ex_40](./images/ex_40.png)

## Exemplo-41 (Parallax Mapping Occlusion)
![ex_41_1](./images/ex_41_1.png)
![ex_41_2](./images/ex_41_2.png)

