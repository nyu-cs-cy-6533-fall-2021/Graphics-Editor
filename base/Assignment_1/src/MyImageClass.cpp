#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include<sstream>
#include <algorithm>
using namespace std;

class MyImageClass {
	unsigned int height;
	unsigned int width;
	unsigned int maxVal;
	string header;
	unsigned int* intMap;
	char* biMap;

	friend istream& operator>>(istream& ins, MyImageClass& rhs);
	friend ostream& operator<<(ostream& ous, MyImageClass& rhs); 

public:
	MyImageClass() :height(0), width(0), maxVal(0),intMap(nullptr),biMap(nullptr) {};
	/*
	~MyImageClass() {
		if (this->header == "P2" || this->header == "P3") {
			delete[] intMap;
		}
		if (this->header == "P5" || this->header == "P6") {
			delete[] biMap;
		}
	};*/
	MyImageClass(const MyImageClass& rhs) {
		header = rhs.header;
		width = rhs.width;
		height = rhs.height;
		maxVal = rhs.maxVal;
		intMap = rhs.intMap;
		biMap = rhs.biMap;
	}

	string getHeader() const { return header; }
	int getHeight() const { return height; }
	int getWidth() const { return width; }
	int getMaxVal() const { return maxVal; }

	void setHeight(unsigned int newHeight) { height = newHeight; }
	void setWidth(unsigned int newWidth) { width = newWidth; }
	void setMaxVal(unsigned int newMaxVal) { maxVal = newMaxVal; }

	bool compareDim(const MyImageClass rhs) {
		if ((this->height == rhs.height) && (this->width == rhs.width)) {
			return true;
		}
		else {
			return false;
		}
	}

	bool compareType(const MyImageClass rhs) {
		if ((this->header == rhs.header)){
			return true;
		}
		else {
			return false;
		}
	}

	void convertToP3(MyImageClass& img) {
		if (img.header == "P2") {//channel=1 int
			img.header = "P3";

			int size = img.width * img.height;
			unsigned int* newintMap = new unsigned int[3*size];//new image type has three channels
			for (int i = 0; i <= size; i++) {
				newintMap[3*i] = img.intMap[i];
				newintMap[3*i+1] = img.intMap[i];
				newintMap[3*i+2] = img.intMap[i];
			}
			//unsigned int* temp;
			//temp = img.intMap;
			delete[] img.intMap;
			img.intMap = newintMap;
			//newintMap = temp;
			
		}
		else if (img.header == "P5") {//channel=1 binary
			img.header = "P3";

			int size = img.width * img.height;
			unsigned int* newintMap = new unsigned int[3 * size];//new image type has three channels
			for (int i = 0; i <= size; i++) {
				newintMap[3 * i] = (int)img.biMap[i];
				newintMap[3 * i + 1] = (int)img.biMap[i];
				newintMap[3 * i + 2] = (int)img.biMap[i];
			}
			
			img.intMap = newintMap;
			delete[] img.biMap;
			img.biMap = nullptr;
			//delete[] newintMap;
		}
		else if (img.header == "P6") {//channel=3
			img.header = "P3";

			int size = img.width * img.height;
			unsigned int* newintMap = new unsigned int[size];
			for (int i = 0; i <= size; i++) {
				newintMap[i] = (unsigned int)img.biMap[i];
			}

			img.intMap = newintMap;
			delete[] img.biMap;
			img.biMap = nullptr;
		}

	}

	MyImageClass& operator+ (MyImageClass& rhs);
	MyImageClass& operator- (MyImageClass& rhs);
	MyImageClass& operator* (double multiplier);
	MyImageClass& operator+= (MyImageClass& rhs);

	MyImageClass& mirrorX() {
		if (this->header != "P3") {
			convertToP3(*this);
		}
		int channel=3;//RGB for P3
		
		for (unsigned int j = 0; j < this->height ; j++) {
			for (unsigned int i = 0; i < (this->width)/2; i++) {
				//cout << "left " << j * this->width * channel + i * channel << "right " << j * this->width * channel + channel * (this->width - 1 - i) << endl;
				swap(this->intMap[j * this->width * channel + i * channel], this->intMap[j * this->width * channel +channel*(this->width-1-i)]);
				//cout << "newleft " << j * this->width * channel + i * channel << "newright " << j * this->width * channel + channel * (this->width - 1 - i) << endl;
				swap(this->intMap[j * this->width * channel + i * channel+1], this->intMap[j * this->width * channel+channel * (this->width - 1 - i)+1]);
				swap(this->intMap[j * this->width * channel + i * channel+2], this->intMap[j * this->width * channel+channel * (this->width - 1 - i)+2]);
			}
		}
		return *this;
	}

	void gammaCorrection(const double gamma) {
		if (this->header != "P3") {
			convertToP3(*this);
		}
		int channel = 3;//RGB for P3
		for (unsigned int i = 0; i < channel * this->width * this->height; i++) {
			double pixvalue;
			pixvalue = pow((this->intMap[i] / 255.0),1.0/gamma)*255;//gamma correction
			this->intMap[i] = (int)pixvalue;
		}
	}

	MyImageClass& alphaComposite(MyImageClass& rhs,double alpha){
		if (rhs.header != "P3") {
			convertToP3(rhs);
		}
		if (this->header != "P3") {
			convertToP3(*this);
		}
		try {
			if (!(this->compareDim(rhs))) {
				throw("Image size not match!");
			}
			for (unsigned int i = 0; i < rhs.width * rhs.height * 3; i++) {
				this->intMap[i] = (int)(alpha*(this->intMap[i])+(rhs.intMap[i])*(1-alpha));
			}
			
		}
		catch (const char* err) {
			fprintf(stderr, "%s\n", err);
		}
	return *this;
	}


};

inline MyImageClass& MyImageClass::operator+ (MyImageClass& rhs) {
	if (rhs.header != "P3") {
		convertToP3(rhs);
	}
	if (this->header != "P3") {
		convertToP3(*this);
	}
	try {
		if (!(this->compareDim(rhs))) {
			throw("Image size not match!");
		}
		for (unsigned int i = 0; i < rhs.width * rhs.height * 3; i++) {
			//cout << "old" << this->intMap[i] << endl;
			//cout << "add" << this->intMap[i] + rhs.intMap[i] << endl;
			this->intMap[i] = min(rhs.maxVal, this->intMap[i] + rhs.intMap[i]);
			//cout << "new" << this->intMap[i] << endl;

		}
		
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
	}
	return *this;
}
inline MyImageClass& MyImageClass::operator- (MyImageClass& rhs) {
	if (rhs.header != "P3") {
		convertToP3(rhs);
	}
	if (this->header != "P3") {
		convertToP3(*this);
	}
	try {
		if (!(this->compareDim(rhs))) {
			throw("Image size not match!");
		}
		for (unsigned int i = 0; i < rhs.width * rhs.height * 3; i++) {
			//cout<< "old"<<this->intMap[i]<<endl;
			//cout << "rhs" << (int)(this->intMap[i] - rhs.intMap[i]) << endl;
			this->intMap[i] = max((int)(this->intMap[i] - rhs.intMap[i]),0);//unsigned int 0
			//cout << "new"<<this->intMap[i]<<endl;
		}
		
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
	}
	return *this;
}
inline MyImageClass& MyImageClass::operator* (double multiplier) {

	try {
		if (multiplier < 0) {
			throw("Invalid multiplier!");
		}
		for (unsigned int i = 0; i < this->width * this->height * 3; i++) {
			//cout << "old" << this->intMap[i] << endl;
			//cout << "add" << this->intMap[i] * multiplier << endl;
			this->intMap[i] = min(255,max(0, (int)(this->intMap[i] * multiplier)));//min(upper, max(x, lower));
			//cout << "new"<<this->intMap[i]<<endl;
		}
		
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
	}
	return *this;
}
inline MyImageClass& MyImageClass::operator+= (MyImageClass& rhs) {
	if (rhs.header != "P3") {
		convertToP3(rhs);
	}
	if (this->header != "P3") {
		convertToP3(*this);
	}
	try {
		if (!(this->compareDim(rhs))) {
			throw("Image size not match!");
		}
		for (unsigned int i = 0; i < rhs.width * rhs.height * 3; i++) {
			this->intMap[i] = (this->intMap[i] + rhs.intMap[i])/2;
		}
		
	}
	catch (const char* err) {
		fprintf(stderr, "%s\n", err);
	}
	return *this;
}

/*
istream& operator>>(istream& in, MyImageClass::RGB& rgb) {
		in >> rgb.R;
		in >> rgb.G;
		in >> rgb.B;
		return in;
}
ostream& operator<<(ostream& out, MyImageClass::RGB& rgb) {
		out << rgb.R << " ";
		out << rgb.G << " ";
		out << rgb.B << std::endl;
		return out;
}

i*/
inline istream& operator>>(istream& ins, MyImageClass& rhs) {
	ins >> rhs.header;
	//ins.ignore(256, '\n');
	int counter=0;
	string line;

	while (counter<3) {
		getline(ins, line);
		if (line[0] != '#')
		{
			std::istringstream iss(line);
			unsigned int num;
			// The number in the line
			//while the iss is a number 
			while ((iss >> num)&&counter<3)
			{
				counter++;
				if (counter == 1) {
					rhs.width = num;
				}
				else if (counter == 2) {
					rhs.height = num;
				}
				else if (counter == 3) {
					rhs.maxVal = num;
				}
				
			}
		}

	}
	//cout << rhs.header << rhs.width << rhs.height << rhs.maxVal<<endl;
	//input image
	int channel;
	if (rhs.header == "P3" || rhs.header == "P6") {
		channel = 3;
	}
	else {
		channel = 1;
	}
	int size = rhs.width * rhs.height * channel;
	if (rhs.header == "P2" || rhs.header == "P3") {
		rhs.intMap = new unsigned int[size];
		int temp,enumerator=0;
		while ((ins >> temp)&& enumerator<size) {
			rhs.intMap[enumerator++] = temp;
		}
		/*
		for (int i = 0; i < 10; i++) {
			cout << rhs.intMap[i] << endl;
		}*/
	}
	else if (rhs.header == "P5" || rhs.header == "P6") {
		rhs.biMap = new char[size];
		ins.read(rhs.biMap, size);
		/*
		for (int i = 0; i < 20; i++) {
			cout << rhs.biMap[i] << endl;
		}*/
	}
	
	return ins;
}


inline ostream& operator<<(ostream& ous, MyImageClass& rhs) {
	if (rhs.getHeader() == "P2") {
		ous << "P2" << "\n";
	}
	else if (rhs.getHeader() == "P3") {
		ous << "P3" << "\n";
	}
	else if (rhs.getHeader() == "P5") {
		ous << "P5" << "\n";
	}
	else if (rhs.getHeader() == "P6") {
		ous << "P6" << "\n";
	}
	
	ous<< rhs.width << " "<< rhs.height << "\n"<< rhs.maxVal << "\n";
	int channel;
	if (rhs.header == "P3" || rhs.header == "P6") {
		channel = 3;
	}
	else {
		channel = 1;
	}
	int size = rhs.width * rhs.height * channel;

	if (rhs.header == "P2" || rhs.header == "P3") {
		int temp = 0;
		while (temp < size) {
			ous << rhs.intMap[temp];
			ous << "\n";
			temp++;
		}
		//delete[] rhs.intMap;
	}
	else if (rhs.header == "P5" || rhs.header == "P6") {
		ous.write(rhs.biMap, size);
		//delete[] rhs.biMap;
	}

	return ous;
}



