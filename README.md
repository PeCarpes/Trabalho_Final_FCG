# Trabalho_Final_FCG
Trabalho final da cadeira de fundamentos da computação gráfica UFRGS 2025/1

## How to Build 

1. Download Dev Containers VSCode extension;
2. Press `Ctrl + Shift + P` and select : `Dev Containers: Open Folder in Container`;
3. Wait for the container to be built (can take a few minutes in the first time);

If you want to compile for Linux:
4. Open a new terminal inside the container and type `cmake .` to generate the Makefile;
5. Type `make` to compile the project.

Else if you want to compile for Windows:
4. Open a new terminal inside the container and run `sudo chmod +x ./build_windows.sh && ./build_windows.sh`
5. Run the Makefile with `make`

**IMPORTANT:** It it not possible to run the application inside the container.
 