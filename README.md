# 4015_VR
This repository contains the code for the 4015 VR App. 

## Libraries used:
1. OpenCV to take camera input
2. Steam's openVR to compose textures for the VR headset
3. GLFW to create openGL context
4. GLEW to load drivers.

## Theory of operation 
We are able to take in camera input from OpenCV into a Mat object. 
The Mat can easily be converted to an openGL 2D Texture.
The Textures are then sent to the SteamVR Compositor that is able to 
render the textures to the HMD. 
