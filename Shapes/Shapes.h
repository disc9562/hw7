#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>


class Shape
{
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void setName(std::string _name) = 0;
    virtual std::string getName() = 0;
    virtual std::string description() const {return std::string("");}
};

double sumOfArea(const std::vector<Shape *> & shapes);

class Rectangle : public Shape
{
public:
    Rectangle(double ulcx, double ulcy, double length, double width):
        x(ulcx),y(ulcy),l(length),w(width) {}
    double area() const
    {
        return l*w;
    }
    double perimeter() const {return 2*(l + w);}
    std::string description() const {
        std::stringstream ss;
        ss << "r(" << x << " " << y << " " << l << " " << w << ") ";
        return ss.str();
    }
    void setName(std::string _name){
        name = _name;
    }
    std::string getName(){
        return name;
    }
private:
    double x,y,l,w;
    std::string name;
};

class Circle : public Shape
{
public:
    Circle(double centerX,double centerY,double radius):
        cx(centerX),cy(centerY),r(radius) {}
    double area()const
    {
        return 3*r*r;
    }
    double perimeter() const {return 3*2*r;}
    std::string description() const {
        std::stringstream ss;
        ss << "c(" << cx << " " << cy << " " << r << ") ";
        return ss.str();
    }
    void setName(std::string _name){
        name = _name;
    }
    std::string getName(){
        return name;
    }
private:
    double cx,cy,r;
    std::string name;
};

class Triangle : public Shape{
public:
    Triangle(double triangleX1, double triangleY1, double triangleX2, double triangleY2, double triangleX3, double triangleY3):
        x1(triangleX1),y1(triangleY1),x2(triangleX2),y2(triangleY2),x3(triangleX3),y3(triangleY3){
        side_1 = sqrt(pow((x1-x2),2) + pow((y1-y2),2));
        side_2 = sqrt(pow((x2-x3),2) + pow((y2-y3),2));
        side_3 = sqrt(pow((x3-x1),2) + pow((y3-y1),2));
        }
    /*Triangle(double triangleX1, double triangleY1, double triangleX2, double triangleY2, double triangleX3, double triangleY3, string objName):
        x1(triangleX1),y1(triangleY1),x2(triangleX2),y2(triangleY2),x3(triangleX3),y3(triangleY3){
        side_1 = sqrt(pow((x1-x2),2) + pow((y1-y2),2));
        side_2 = sqrt(pow((x2-x3),2) + pow((y2-y3),2));
        side_3 = sqrt(pow((x3-x1),2) + pow((y3-y1),2));
        setName(objName);
        }*/
    double perimeter() const {
        double total = 0;

        total = side_1 + side_2 + side_3;
        return total;
    }
    double area() const{
        double area = 0;

        area = ((x1*y2+x2*y3+x3*y1) - (x2*y1+x3*y2+x1*y3))/2;
        if(area < 0)
            area = -1 * area;
        return area;
    }
    bool check(){

        if(side_1 + side_2 >side_3 && side_2 + side_3 > side_1 && side_1 + side_3 > side_2){
            return true;
        }
        return false;
    }
    std::string description() const {
        std::stringstream ss;
        ss << "t(" << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << ") ";
        return ss.str();
    }
    void setName(std::string _name){
        name = _name;
    }
    std::string getName(){
        return name;
    }
private:
    double x1,y1,x2,y2,x3,y3,side_1,side_2,side_3;
    std::string name;
};

double sumOfArea(const std::vector<Shape *> & shapes) {
    double total =0;
    for (Shape *s: shapes)
        total += s->area();
    return total;
}

/*double sumOfPerimeter(const std::vector<Shape *> & shapes){
    double total = 0;
    for(Shape *s: shapes)
        total += s->perimeter();
    return total;
}*/
#endif // SHAPES_H_INCLUDED
