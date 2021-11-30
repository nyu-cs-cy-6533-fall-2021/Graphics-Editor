#include <iostream>
#include <exception>

#include "MyImageClass.cpp"

int main(int argc, char* argv[]) {

    try {

        cout << "reading picture.."<<endl;

        MyImageClass image1;
        MyImageClass image2;
        ifstream imageFile1;

        imageFile1.open("C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_1\\images\\Mandrill.ppm", ios::in | ios::binary);
        if (imageFile1.is_open()) {
            imageFile1 >> image1;
            imageFile1.close();
        }

        imageFile1.open("C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_1\\images\\tandon_stacked_color.ppm", ios::in | ios::binary);
        if (imageFile1.is_open()) {
            imageFile1 >> image2;
            imageFile1.close();
        }

        //image1=image1+image2;
        //image1=image1-image2;
        //image1 = image1 * 0.8;
        //image1 = image1 * 1.5;
        image1+=image2;
        //image1.mirrorX();
        //image1.gammaCorrection(0.2);
        //image1.alphaComposite(image2, 0.85);
        //image1.alphaComposite(image2, 0.5);

        //You also can do it in the "C++ way"
        ofstream imageFile;
        imageFile.open("C:\\Users\\roder\\Desktop\\Computer graphics\\base\\Assignment_1\\changed_images\\average_pic1.ppm");
        if (imageFile.is_open()) {
            imageFile << image1;
            imageFile.close();
            
        }
    }
    catch (exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
