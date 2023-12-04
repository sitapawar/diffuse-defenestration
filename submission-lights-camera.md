## Project 5: Lights, Camera

Please fill this out for Lights, Camera only. The project handout can be found [here](https://cs1230.graphics/projects/realtime/1).

### Output Comparison

Run the program, open the specified `.json` file, follow the instructions to set the parameters, and save the image with the specified file name using the "Save image" button in the UI. It should automatically suggest the correct directory - again, be sure to follow the instructions in the left column to set the file name. Once you save the images, they will appear in the table below.

> If your program can't find certain files or you aren't seeing your output images appear, make sure to:<br/>
>
> 1. Set your working directory to the project directory
> 2. Clone the `scenefiles` submodule. If you forgot to do this when initially cloning this repository, run `git submodule update --init --recursive` in the project directory

> Note: once all images are filled in, the images will be the same size in the expected and student outputs.

|                                           File/Method To Produce Output                                            |                                                     Expected Output                                                     |                                                                     Your Output                                                                     |
| :----------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------: |
|                Input: `unit_cone.json`<br/>Output: `unit_cone.png`<br/>Parameters: (5, 5, 0.1, 100)                |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone.png)      |            ![Place unit_cone.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone.png)            |
|            Input: `unit_cone_cap.json`<br/>Output: `unit_cone_cap.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_cap.png)    |        ![Place unit_cone_cap.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_cap.png)        |
|               Input: `unit_cube.json`<br/>Output: `unit_cube.png`<br/> Parameters: (5, 5, 0.1, 100)                |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cube.png)      |            ![Place unit_cube.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cube.png)            |
|            Input: `unit_cylinder.json`<br/>Output: `unit_cylinder.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cylinder.png)    |        ![Place unit_cylinder.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cylinder.png)        |
|              Input: `unit_sphere.json`<br/>Output: `unit_sphere.png`<br/>Parameters: (5, 5, 0.1, 100)              |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_sphere.png)     |          ![Place unit_sphere.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_sphere.png)          |
|          Input: `unit_cone.json`<br/>Output: `unit_cone_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)          |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_min.png)      |        ![Place unit_cone_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_min.png)        |
|      Input: `unit_cone_cap.json`<br/>Output: `unit_cone_cap_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)      |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cone_cap_min.png)    |    ![Place unit_cone_cap_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cone_cap_min.png)    |
|          Input: `unit_cube.json`<br/>Output: `unit_cube_min.png`<br/>Parameters: (**1**, **1**, 0.1, 100)          |      ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cube_min.png)      |        ![Place unit_cube_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cube_min.png)        |
|      Input: `unit_cylinder.json`<br/>Output: `unit_cylinder_min.png`<br/>Parameters: (**1**, **3**, 0.1, 100)      |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_cylinder_min.png)    |    ![Place unit_cylinder_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_cylinder_min.png)    |
|        Input: `unit_sphere.json`<br/>Output: `unit_sphere_min.png`<br/>Parameters: (**2**, **3**, 0.1, 100)        |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/unit_sphere_min.png)     |      ![Place unit_sphere_min.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/unit_sphere_min.png)      |
|           Input: `parse_matrix.json`<br/>Output: `parse_matrix.png`<br/>Parameters: (**3**, 5, 0.1, 100)           |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/parse_matrix.png)     |         ![Place parse_matrix.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/parse_matrix.png)         |
|            Input: `ambient_total.json`<br/>Output: `ambient_total.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/ambient_total.png)    |        ![Place ambient_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/ambient_total.png)        |
|            Input: `diffuse_total.json`<br/>Output: `diffuse_total.png`<br/>Parameters: (5, 5, 0.1, 100)            |    ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/diffuse_total.png)    |        ![Place diffuse_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/diffuse_total.png)        |
|           Input: `specular_total.json`<br/>Output: `specular_total.png`<br/>Parameters: (5, 5, 0.1, 100)           |   ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/specular_total.png)    |       ![Place specular_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/specular_total.png)       |
|              Input: `phong_total.json`<br/>Output: `phong_total.png`<br/>Parameters: (5, 5, 0.1, 100)              |     ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/phong_total.png)     |          ![Place phong_total.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/phong_total.png)          |
|      Input: `directional_light_1.json`<br/>Output: `directional_light_1.png`<br/>Parameters: (5, 5, 0.1, 100)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_1.png) |  ![Place directional_light_1.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_1.png)  |
| Input: `directional_light_2.json`<br/>Output: `directional_light_2.png`<br/>Parameters: (**10**, **10**, 0.1, 100) | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_2.png) |  ![Place directional_light_2.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_2.png)  |
|      Input: `phong_total.json`<br/>Output: `phong_total_near_far.png`<br/>Parameters: (5, 5, **9.5**, **12**)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/phong_total_near_far.png) | ![Place phong_total_near_far.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/phong_total_near_far.png) |
|      Input: `directional_light_1.json`<br/>Output: `directional_light_1_near_far.png`<br/>Parameters: (**25**, **25**, **8**, **10**)      | ![](https://raw.githubusercontent.com/BrownCSCI1230/scenefiles/main/lights-camera/required_outputs/directional_light_1_near_far.png) | ![Place directional_light_1_near_far.png in student_outputs/lights-camera/required folder](student_outputs/lights-camera/required/directional_light_1_near_far.png) |

### Design Choices
- I use my camera but don't make a scene object for it so I instantiate it directly in my realtime cpp file.
- I have seperate shapes classes like in lab 8 where I get the vertex data depending on the type
- I thought we had to implement different light types so I started doing that in my light uniforms and left some of that so I still pass in that info
- i have a genBufferData function that is called in init and also when parameters or scenefiles change to recalculate data, but if only the camera parameters (near and far) change it only recalcs camera params since the vertices aren't change

### Collaboration/References
- jho4
- aburris3
- Lots of people in hours

### Known Bugs
Literally so many: 
- shape rotations are off (but I don't know what to change and I've checked my matrices and ctms and tried so many different variations)
      - I think this is why my specular is showing up differently/missing for some shapes, but it also could be another issue
- my camera is a little zoomed in on phong (and some of the shapes I thing) but I don't know why and this has been an eternal issue for me
- sphere isn't impacted by near/far plane changes in the way that everything else is
  
### Extra Credit
