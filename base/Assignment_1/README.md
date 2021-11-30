# Assignment 1
<!-- PROJECT LOGO -->
<br />
<p align="center">
  
  </a>

  <h3 align="center">  <h3 align="center">All pictures in this document are SCREENSHOTS, please check change_images for original .ppm files!</h3></h3>


<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#task-1">Task 1 Image Class</a>
      <ul>
        <li><a href="operator-overloading">Operator Overloading</a></li>
      </ul>
    </li>
    <li>
      <a href="#task-2">Task 2 Operations</a>
      <ul>
        <li><a href="#addition">Addtion</a></li>
        <li><a href="#subtraction">Subtraction</a></li>
        <li><a href="#multiplication">Multiplication</a></li>
        <li><a href="#average">Average</a></li>
        <li><a href="#mirror">Mirror</a></li>
        <li><a href="#gamma-correction">Gamma Correction</a></li>
        <li><a href="#alpha-composition">Alpha Composition</a></li>
      </ul>
    </li>
  </ol>
</details>

The `ImageClass.cpp` is the main function that I used to generate modified ppm pictures, and `MyImageClass.cpp` is the where I implemented the class.

<!-- Task 1 -->
## Task 1
For task 1, I implemented the class with image header, height, width, maximum pixel value, and the data map of each pixel.
![alt text](/blob/class.jpg?raw=true)

### Operator Overloading
To Overload `>>`, I code to ignore the comment, and then read the first string as image header, the second string as image width, the third string as image height, thr fourth string as image maximum pixel value. The rest of the data are all pixel values, so we read them to our data map.
![alt text](/blob/overload_1.jpg?raw=true)
![alt text](/blob/overload_2.jpg?raw=true)


Overloading `<<` doesn't need to deal with comment, so we output header, width etc. Just like above.


![alt text](/blob/overload_3.jpg?raw=true)

<!-- Task 2 -->
## Task 2

To get a local copy up and running follow these simple steps.

### Addition
I simply added pixel value one by one, and restrict the maximum value to be below 255.
![alt text](/blob/add_1.jpg?raw=true)

The output is
![alt text](/blob/add_pic.jpg?raw=true)
### Subtraction
I subtract pixel value one by one, and restrict the maximum value to be above 0.
![alt text](/blob/sub_1.jpg?raw=true)

The output is
![alt text](/blob/sub_pic.jpg?raw=true)
### Average
I add pixel value and then devide it by two, and restrict the maximum value to be below 255.
![alt text](/blob/avg_1.jpg?raw=true)

The output is
![alt text](/blob/avg_pic.jpg?raw=true)
### Mirror
Since I didn't implement an RGB classs, so I manually calculated how the pixel should be reversed.
![alt text](/blob/mirror_1.jpg?raw=true)

The output is
![alt text](/blob/mirror_pic.jpg?raw=true)
### Multiplication
I multiply each pixel value by the multiplier, and restrict the maximum value to be below 255 and above 0.
![alt text](/blob/multi_1.jpg?raw=true)

The output with multiplier 0.8 is
![alt text](/blob/multi_0.8.jpg?raw=true "Multiplier=0.8" )

The output with multiplier 1.5 is
![alt text](/blob/multi_1.5.jpg?raw=true "Multiplier=1.5")
### Gamma Correction
By the Gamma Correction Formula, `new pixel value=(pixel value/255)^(1/gamma)*255`. Then update all the pixel values.
![alt text](/blob/gamma_1.jpg?raw=true)

The output with gamma 0.2 is
![alt text](/blob/gamma_0.2.jpg?raw=true "Gamma=0.2" )

The output with gamma 2 is
![alt text](/blob/gamma_2.jpg?raw=true "Gamma=2" )
### Alpha Composition
By the Alpha Composition Formula, `new_img=alpha*img1+(1-alpha)*img2`. Then update all the pixel values.
![alt text](/blob/alpha_1.jpg?raw=true)


The output is
The output with gamma 0.85 is
![alt text](/blob/alpha_0.85.jpg?raw=true "Aplha=0.85")


The output with gamma 0.5 is
![alt text](/blob/alpha_0.5.jpg?raw=true "Aplha=0.5")

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

