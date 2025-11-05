# A1 Report

Author: sebastian gubacsi 

Date: 2005-06-17 

Check [readme.txt](readme.txt) for course work statement and self-evaluation. 
  
## Q1 Concepts of raster display systems (short answer)


### Q1.1 Framebuffers, pixels, color depth, and resolution
Framebuffers are memory areas that store pixel data for display. Each pixel represents a tiny dot on the screen, and its color is determined by the color depth, which specifies how many bits are used for each pixel. Higher color depth allows more colors to be displayed. Resolution refers to the number of pixels arranged horizontally and vertically, affecting image clarity and detail.


### Q1.2 Frame, refreshment, and refresh rate
A frame is a single image displayed on the screen. Refreshment is the process of updating the display with new frames. Refresh rate is the number of times per second the screen is updated, measured in hertz (Hz). Higher refresh rates result in smoother motion and less flicker.



## Q2 Concepts of image rendering (short answer)


### Q2.1 What are the two basic image rendering approaches? and what are their differences?
The two basic image rendering approaches are rasterization and ray tracing. Rasterization converts 3D objects into pixels for display, focusing on speed and efficiency. Ray tracing simulates the path of light rays for realistic lighting and shadows, but is more computationally intensive.


### Q2.2 Why rasterization is commonly used in graphics applications?
Rasterization is commonly used because it is fast and efficient, making it suitable for real-time applications like video games and interactive graphics. It can quickly convert 3D models into 2D images with acceptable visual quality.



## Q3 Roles of CPU and GPU in Graphics Computing (short answer)


### Q3.1 CPU roles
The CPU manages general system operations, runs the main program logic, and coordinates tasks such as input/output, memory management, and sending instructions to the GPU for graphics processing.


### Q3.2 GPU roles
The GPU is specialized for parallel processing of graphics data. It handles rendering, shading, and complex calculations needed for images and animations, enabling fast and efficient graphics performance.



## Q4 C/C++ OpenGL programming environment (lab practice)


### Q4.1 C/C++ OpenGL installation 

Complete? Yes
<!--If you answer Yes, insert one or more screenshot images to show the completion.-->

![](images/cube.png){width=90%}
![](images/glsl_demo.png){width=90%}

<!-- If No, add a short description to describe the issues encountered.-->

### Q4.2 OpenGL C project 

Complete? Yes

<!--If you answer Yes, insert one or more screenshot images to show the completion.-->

![](images/myopengl.png){width=90%}

<!-- If No, add a short description to describe the issues encountered.-->

### Q4.3 OpenGL C++ project 

Complete? Yes

<!--If you answer Yes, insert one or more screenshot images to show the completion.-->

![](images/glut_test.png){width=90%}

<!-- If No, add a short description to describe the issues encountered.-->



**References**

1. CP411 a1
2. gpt for first 3
3. provided copyable code
