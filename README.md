# RocketManPSP

# Compilation instructions
1. Download the PSPSDK for your system.
   * For Mac OS / Linux:
      * You can install the SDK [here](https://github.com/pspdev/pspdev/releases)
      * You may need to run some of the preparation scripts [here](https://github.com/pspdev/pspdev)
      * After, you can the sdk's commands to your PATH. Or not, you will need to update the makefile to point to your pspsdk/bin/psp-g++
    * For Windows:
      * 🫡 https://www.youtube.com/watch?v=35q-7ITBzSk
2. Run ```make```

# Run Instructions
To build an run ```make``` to build the game, then execute EBOOT.PBP on your emulator
Alternatively you can just navigate to the releases page and download the RocketMan zip. Once downloaded, simply navigate to the Game folder of your psp and unzip the files there. Alternatively these same steps could be followed on the PPSSPP emulator.

# Description:
Rocketman PSP is a 2D platformer PSP game written in c++.
Play as renowned singer Elton John and fight against the a mercenary group while trying not to blow yourself up in the process!

# Technical Info:
The program uses a built from scratch BMP loader and renderer as well as a logger and utility library for debugging, The GUI interface, image library, memory allocation wrapper, collisions manager and physics handler where all written from scratch

# Screenshots:
![image](https://github.com/JamaicaNater/RocketManPSP/assets/52978102/f221ebcf-c951-4f95-9e5b-9689c49506f3)
![image](https://github.com/JamaicaNater/RocketManPSP/assets/52978102/317cfefd-19e9-407a-8689-e5350cc6053d)
![image](https://user-images.githubusercontent.com/52978102/174581212-2125485f-cf5a-4ab9-92ce-b6f7161b0169.png)
![image](https://github.com/JamaicaNater/RocketManPSP/assets/52978102/c143e422-c4e6-4ab4-be9e-e0af74d17495)
