# learn-modern-opengl

Projeto criado para aprender opengl - exemplos com refatorações do site [learn opengl](https://learnopengl.com/)

## 📋 Instalações das dependências (Linux)

### Instalação da lib X11

```
sudo apt-get install doxygen dnf xorg-dev
sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```

### Instalação da lib GLFW (GLFW-3.3.8):

Baixar o pacote [GLFW-3.3.8](https://www.glfw.org/download)

Após baixar, extrair e faça a compilação usando os comandos dentro da pasta ```glfw-3.3.8```

```
cmake -S . -B build
cmake build
cd build
make build
sudo make install
```

Ao finalizar, será exibido no terminal, os locais de instalações das libs e os includes como pode ser visto na imagem abaixo
ser visto na imagem:

![glfw-install](./images/img_1.png)

### Instalação do Assimp (Assimp-5.3.1):

Baixe o Assimp [assimp-5.3.1](https://github.com/assimp/assimp)
após baixar, extrair e faça a sua build

```
cd assimp
cmake CMakeLists.txt 
cmake --build .
```

após fazer a build, copie os arquivos ```.so``` para a pasta ```/usr/local/lib``` e crie um atalho do ```libassimp.so.5``` para a 
pasta principal do projeto clonado

## 🔧 Configuração do CodeBlock:

Verifique se a configuração do seu codeblock como segue as imagens a seguir:

![img_2](./images/img_2.png)
![img_3](./images/img_3.png)
![img_4](./images/img_4.png)
![img_5](./images/img_5.png)

Pronto, o Codeblock está configurado

### 🚀 Executando um exemplo

Após finalizar a instalação das libs e a configuração do codeblock, no arquivo ```main.cpp``` mude os exemplos pela definition ```EXEC_TEST X``` , onde o ```X``` é o número do exemplo que você desejar.

## Exemplo-1
![ex_1](./images/ex_1.png)

## Exemplo-2
![ex_2](./images/ex_2.png)

## Exemplo-3
![ex_3](./images/ex_3.png)

## Exemplo-4
![ex_4](./images/ex_4.gif)

## Exemplo-5
![ex_5](./images/ex_5.png)

## Exemplo-6
![ex_6](./images/ex_6.gif)

## Exemplo-7
![ex_7](./images/ex_7.gif)

## Exemplo-8
![ex_8](./images/ex_8.gif)

## Exemplo-9
![ex_9](./images/ex_9.gif)

## Exemplo-10
![ex_10](./images/ex_10.gif)

## Exemplo-12
![ex_12](./images/ex_12.png)

## Exemplo-14
![ex_14](./images/ex_14.png)

## Exemplo-15
![ex_15](./images/ex_15.png)

## Exemplo-16
![ex_16](./images/ex_16.png)

## Exemplo-17
![ex_17](./images/ex_17.png)
