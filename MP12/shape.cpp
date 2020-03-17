#include "shape.hpp"
#define PI 3.14159265358979

// Murugan Narayanan
// MP12

//Base class
//Please implement Shape's member functions
//constructor, getName()
//
//Base class' constructor should be called in derived classes'
//constructor to initialize Shape's private variable
Shape::Shape(string name)
{
  name_ = name;
}

string Shape::getName()
{
  return name_;
}


//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
Rectangle::Rectangle(double width, double length):Shape("Rectangle")
{
  width_ = width;
  length_ = length;
}

double Rectangle::getArea()
{
  return width_*length_;
}

double Rectangle::getVolume()
{
  return 0;
}

Rectangle Rectangle::operator + (const Rectangle& rec)
{
  return Rectangle(width_ + rec.width_,length_ + rec.length_);
}

Rectangle Rectangle::operator - (const Rectangle& rec)
{
  double len = (0 > length_ - rec.length_) ? 0 : length_ - rec.length_;
  double wid = (0 > width_ - rec.width_) ? 0 : width_ - rec.width_;
  return Rectangle(wid, len);
}

double Rectangle::getWidth(){return width_;}
double Rectangle::getLength(){return length_;}


//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

Circle::Circle(double radius):Shape("Circle")
{
  radius_ = radius;
}

double Circle::getArea()
{
  return pow(radius_,2)*PI;
}

double Circle::getVolume()
{
  return 0;
}

Circle Circle::operator + (const Circle& cir)
{
  return Circle(radius_ + cir.radius_);
}

Circle Circle::operator - (const Circle& cir)
{
  double rad = (0 > radius_ - cir.radius_) ? 0 : radius_ - cir.radius_;
  return Circle(rad);
}

double Circle::getRadius(){return radius_;}

//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

Sphere::Sphere(double radius):Shape("Sphere")
{
  radius_ = radius;
}

double Sphere::getArea()
{
  return 4*PI*pow(radius_,2);
}

double Sphere::getVolume()
{
  return PI*pow(radius_,3)*4/3;
}

Sphere Sphere::operator + (const Sphere& sph)
{
  return Sphere(radius_ + sph.radius_);
}

Sphere Sphere::operator - (const Sphere& sph)
{
  double rad = (0 > radius_ - sph.radius_) ? 0 : radius_ - sph.radius_;
  return Sphere(rad);
}

double Sphere::getRadius(){return radius_;}

//RectPrism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

RectPrism::RectPrism(double width, double length, double height):Shape("RectPrism")
{
  width_ = width;
  length_ = length;
  height_ = height;
}

double RectPrism::getArea()
{
  double face1 = width_*length_;
  double face2 = width_*height_;
  double face3 = length_*height_;
  return 2*(face1 + face2 + face3);
}

double RectPrism::getVolume()
{
  return width_*length_*height_;
}

RectPrism RectPrism::operator + (const RectPrism& rectp)
{
  return RectPrism(width_ + rectp.width_, length_ + rectp.length_, height_ + rectp.height_);
}

RectPrism RectPrism::operator - (const RectPrism& rectp)
{
  double wid = (0 > width_ - rectp.width_) ? 0 : width_ - rectp.width_;
  double len = (0 > length_ - rectp.length_) ? 0 : length_ - rectp.length_;
  double hgt = (0 > height_ - rectp.height_) ? 0 : height_ - rectp.height_;
  return RectPrism(wid, len, hgt);
}

double RectPrism::getWidth(){return width_;}
double RectPrism::getHeight(){return height_;}
double RectPrism::getLength(){return length_;}



// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects
vector<Shape*> CreateShapes(char* file_name){
	//@@Insert your code here
  ifstream ifs (file_name, std::ifstream::in);
  int count;
  ifs>>count;
  string name;
  double len, wid, hgt, rad;
  vector<Shape*> shapes;
  for(int i = 0; i < count; i++)
  {
    ifs>>name;
    if (name == "Rectangle")
    {
      ifs>>wid>>len;
      Shape* rect = new Rectangle(wid, len);
      shapes.push_back(rect);
    }
    else if (name == "Circle")
    {
      ifs>>rad;
      Shape* cir = new Circle(rad);
      shapes.push_back(cir);
    }
    else if (name == "Sphere")
    {
      ifs>>rad;
      Shape* sph = new Sphere(rad);
      shapes.push_back(sph);
    }
    else // must be RectPrism
    {
      ifs>>wid>>len>>hgt;
      Shape* rp = new RectPrism(wid,len,hgt);
      shapes.push_back(rp);
    }
  }
  ifs.close();
	return shapes;
}

// call getArea() of each object
// return the max area
double MaxArea(vector<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
  double ar;
  for (int i = 0; i < shapes.size(); i++)
  {
    ar = shapes[i]->getArea();
    max_area = (ar > max_area) ? ar : max_area;
  }
	return max_area;
}


// call getVolume() of each object
// return the max volume
double MaxVolume(vector<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
  double vol;
  for (int i = 0; i < shapes.size(); i++)
  {
    vol = shapes[i]->getVolume();
    max_volume = (vol > max_volume) ? vol : max_volume;
  }
	return max_volume;
}
