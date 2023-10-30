# learn-modern-opengl

Instalação das dependências:

Faça a instalação das seguintes dependências

```
sudo apt-get install doxygen dnf xorg-dev
sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```


Instalação do GLFW:

Instalar GLFW-3.3.8 pelo site: https://www.glfw.org/download
Após isso, extrair e usar os seguintes comandos dentro da pasta ```glfw-3.3.8```

```
cmake -S . -B build
```

O comando acima irá criar uma pasta chamada build, após isso de cmake build

```
cmake build
```

entre na pasta build com o comando 

```
cd build
``` 

após isso, fala a compilação do glfw

```
make build
```

e a instalação com permissão de root

```
sudo make install
```

Ao finalizar, será exibido no terminal o local de instalação das libs e os includes como pode
ser visto na imagem:

![glfw-install](./images/img_1.png)

Configuração do CodeBlock:

Em ```Build Options -> Compiler Settings``` habilite o ```-std=c11``` o ```-static-libstdc++```

![glfw-codeblock-config](./images/img_2.png)

Em ```Build Options -> Linker Settings``` adicione o local onde a lib ```libglfw3.a``` está instalado

Ainda em ```Build Options -> Linker Settings```, adicione em ```Other linker options``` os seguintes links ```-lglfw3 -lX11```

Em ```Build Options -> Search directories``` adicione o local onde o include ```GLFW``` está instalado

Pronto, o Codeblock está configurado

