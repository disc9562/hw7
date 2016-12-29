#ifndef SHAPEMEDIA_H_INCLUDED
#define SHAPEMEDIA_H_INCLUDED

#include "Shapes.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <map>
using namespace std;

class ShapeMedia;
class ComboMedia;
class MediaVisitor{
public:
    virtual void visitComboMedia(ComboMedia *cm) = 0;
    virtual void visitShapeMedia(ShapeMedia *sm) = 0;
};

class AreaVisitor : public MediaVisitor{
public:
    void visitComboMedia(ComboMedia *cm);
    void visitShapeMedia(ShapeMedia *sm);
    double getArea() const {return total;}
private:
    double total = 0;
};

class PerimeterVisitor : public MediaVisitor{
public:
    void visitComboMedia(ComboMedia *cm);
    void visitShapeMedia(ShapeMedia *sm);
    double getPerimeter() const {return total;}
private:
    double total = 0;
};
class DescriptionVisitor {
public:
    DescriptionVisitor (): desc(std::string("")) {}
    void visitShapeMedia(ShapeMedia *sm);
    void visitComboMedia(ComboMedia *cm);
    std::string getDescription() const {return desc;}
private:
    std::string desc;
};
class NameVisitor{
public:
    NameVisitor(): name(std::string("")) {}
    void visitShapeMedia(ShapeMedia *sm);
    void visitComboMediaPre(ComboMedia *cm);
    void visitComboMediaPost(ComboMedia *cm);
    std::string getName() const {return name;};
private:
    std::string name;
};
class Media {
public :
    virtual double area() const {} ;
    virtual double perimeter() const {} ;
    virtual void accept(MediaVisitor * mv) {};
    virtual void accept(DescriptionVisitor * dv) {};
    virtual void accept(NameVisitor * nv) {};
    virtual void removeMedia(vector<Media *> *removeVector, Media *m) {};
    virtual void setMedia(map<std::string, Media *> *mapMedia,vector<string> *s) {};
    virtual void add(Media * m) {
        throw std::string("Illegal: add on media");
    }
    virtual std::string text(Rectangle boundingbox, std::string text) const{};
};

class ShapeMedia :public Media {
private:
    Shape* shape ;
public :
    ShapeMedia( Shape* s ): shape(s) {}
    virtual void accept(DescriptionVisitor * dv) {
        dv->visitShapeMedia(this);
    }
    void accept(MediaVisitor * mv){
        mv -> visitShapeMedia(this);
    }
    void accept(NameVisitor * nv){
        nv -> visitShapeMedia(this);
    }
    double area() const { return shape->area() ; }
    void removeMedia(vector<Media *> *removeVector, Media *m){}
    double perimeter() const { return shape->perimeter() ; }
    Shape * getShape()const {return shape;}
    void setMedia(map<std::string, Media *> *mapMedia, vector<string> *s) {
        mapMedia -> insert(pair<std::string, Media*>(s -> at(0),this));
        shape-> setName(s->at(0));
        s->erase(s->begin());
    }
};

class ComboMedia : public Media {
private:
    std::vector<Media*> media;
    std::string name;
public:
    ComboMedia( std::vector<Media*>m ) : media(m) {}
    ComboMedia () {}
    double area( ) const {
        double total =0;
        for (Media *m: media)
            total += m->area();
        return total;
    }
    double perimeter() const{
        double total = 0;
        for (Media *m: media)
            total += m->perimeter();
        return total;
    }
    void accept(MediaVisitor * mv){
        for (Media *m:media)
            m->accept(mv);
        mv->visitComboMedia(this);
        }
    void accept(DescriptionVisitor * dv) {
        for (Media *m: media)
            m->accept(dv);
        dv->visitComboMedia(this);
    }
    void accept(NameVisitor * nv){
        nv -> visitComboMediaPre(this);
        for (Media *m :media)
            m -> accept(nv);
        nv -> visitComboMediaPost(this);
    }
    void add (Media *m) {
        media.push_back(m);
    }
    void removeMedia(vector<Media *> *removeVector, Media *m){
        std::vector <Media *>::const_iterator it;
        for(it = media.begin();it != media.end();++it){
            if((*it) == m){
                removeVector->push_back(this);
                it = media.erase(it);
                break;
            }
            else
                (*it)->removeMedia(removeVector, m);
        }
    }
    void setMedia(map<string, Media *> *mapMedia, vector<string> *s) {
        if(s->size()>0){
            mapMedia -> insert(pair<string, Media*>(s -> at(0),this));
            name = s->at(0);
            s->erase(s->begin());
            for(Media *m : media){
                m->setMedia(mapMedia,s);
            }
        }
    }
    void setName(std::string _name){
        name = _name;
    }
    std::string getName(){
        return name;
    }
};

class TextMedia : public Media{
public:
    TextMedia(Rectangle boundingbox, std::string text):r(boundingbox), txt(text){};
    std::string getText(){return txt;}
    Rectangle getRectangle(){return r;}
private:
    std::string txt;
    Rectangle r;
};

void AreaVisitor::visitShapeMedia(ShapeMedia *sm){
    total = sm->area();
}

void PerimeterVisitor::visitShapeMedia(ShapeMedia *sm){
    total = sm->perimeter();
}

void DescriptionVisitor::visitShapeMedia(ShapeMedia *sm) {
    desc += sm->getShape()->description();
}

void AreaVisitor::visitComboMedia(ComboMedia *cm){
    total = cm->area();
}

void PerimeterVisitor::visitComboMedia(ComboMedia *cm){
    total = cm->perimeter();
}

void DescriptionVisitor::visitComboMedia(ComboMedia *cm) {
    desc = std::string("combo(")+desc+std::string(")");
}

void NameVisitor::visitShapeMedia(ShapeMedia *sm){
    name = name + sm ->getShape()->getName() + " ";
}

void NameVisitor::visitComboMediaPre(ComboMedia *cm){
    name = name + cm ->getName() + "{";
}

void NameVisitor::visitComboMediaPost(ComboMedia *cm){
    name = name + "}";
}

/*class MediaBuilder{
public:
    virtual void buildComboMedia() = 0;
    virtual void buildShapeMedia(Shape *s) = 0;
    virtual Media *getMedia() = 0;
};

class ShapeMediaBuilder : public MediaBuilder{
public:
    ShapeMediaBuilder () : shape(0){}
    void buildComboMedia(){}
    void buildShapeMedia(Shape *s){
        shape = new ShapeMedia(s);
    }
    Media * getMedia() {return shape;}
private:
    ShapeMedia * shape;
};

class ComboMediaBuilder : public MediaBuilder
{
public:
    ComboMediaBuilder () :combo(0) {}
    void buildComboMedia() {combo = new ComboMedia;}
    void buildShapeMedia(Shape *s){
        shape = new ShapeMedia(s);
        combo->add(shape);
    }
    void buildAddComboMedia(Media * cm){
        combo->add(cm);
    }
    Media *getMedia() {return combo;}
    Media *getShapeMedia() {return shape;}
private:
    ComboMedia *combo;
    ShapeMedia * shape;
};*/

/*class TextMediaBuilder : public MediaBuilder{
public:
    TextMediaBuilder ():text(0){}
    void buildComboMedia(){}
    void buildShapeMedia(Shape *s){}
    void buildTextMedia(Rectangle r,std::string s){
        text = new TextMedia(r,s);
    }
    Media *getMedia(){return text;}
private:
    TextMedia *text;
};*/

class MediaBuilder {
public:
    MediaBuilder (): combo(0) {}
    void buildComboMedia() {combo = new ComboMedia;}
    void buildShapeMedia(Shape * s) {
        if(!combo)
            throw std::string("null point ex") ;
        combo->add(new ShapeMedia(s));

    }
    void buildAddComboMedia(ComboMedia * cm){
        combo->add(cm);
    }
    ComboMedia * getComboMedia() const {return combo;}
private:
    ComboMedia * combo;
};

class MediaDirctror{
    public :
		void setMediaBuilder(std::stack<MediaBuilder *> *mbs){
            mbs = &mb;
		}
		void concrete(string content){
		    string temp;
		    double rec[4];
		    double cir[3];
		    //double tri[6];
		    for(int i = 0; i <content.size(); i++){
                temp += content.at(i);
                //cout << temp << endl;
                if(temp == "combo(" || temp == " combo("){
                    mb.push(new MediaBuilder());
                    mb.top()->buildComboMedia();
                    temp = "";
                }
                else if(temp == "r(" || temp == " r("){
                    string num;
                    char numTemp[5];
                    int index = 0;
                    i++;
                    string rTemp;
                    rTemp += content.at(i);
                    while(rTemp != ")"){
                        if(rTemp != " "){
                            num += content.at(i);
                        }
                        else{
                            strcpy(numTemp, num.c_str());
                            rec[index] = atof(numTemp);
                            index++;
                            num = "";
                            memset(numTemp, 0,sizeof numTemp);
                        }
                        i++;
                        rTemp = content.at(i);
                    }
                    strcpy(numTemp, num.c_str());
                    rec[index] = atof(numTemp);
                    mb.top()->buildShapeMedia(new Rectangle(rec[0],rec[1],rec[2],rec[3]));
                    temp = "";
                }
                else if(temp == "c(" || temp == " c("){
                    string num;
                    char numTemp[5];
                    int index = 0;
                    i++;
                    string cTemp;
                    cTemp += content.at(i);
                    while(cTemp != ")"){
                        if(cTemp != " "){
                            num += content.at(i);
                        }
                        else{
                            strcpy(numTemp, num.c_str());
                            cir[index] = atof(numTemp);
                            index++;
                            num = "";
                            memset(numTemp, 0,sizeof numTemp);
                        }
                        i++;
                        cTemp = content.at(i);
                    }
                    strcpy(numTemp, num.c_str());
                    cir[index] = atof(numTemp);
                    mb.top()->buildShapeMedia(new Circle(cir[0],cir[1],cir[2]));
                    temp = "";
                }
                else if(temp == ")" || temp == " )"){
                    if(mb.size() > 1){
                        ComboMedia *cm = mb.top()->getComboMedia();
                        mb.pop();
                        mb.top()->buildAddComboMedia(cm);
                        temp = "";
                    }
                }
		    }
		}
		std::stack<MediaBuilder *> getStack(){
            return mb;
		}
		vector<std::string> getNameVector(string s){
            vector<std::string> nameVector;
            string temp;
            for(int i = 0; i <s.size(); i++){
                temp += s.at(i);
                if(s.at(i) == '{'){
                    nameVector.push_back(temp.substr(0,temp.size()-1));
                    temp = "";
                }
                else if(s.at(i) == ' '){
                    nameVector.push_back(temp.substr(0,temp.size()-1));
                    temp = "";
                }
            }
            return nameVector;
		}
	private :
        std::stack<MediaBuilder *> mb;

};
#endif // SHAPEMEDIA_H_INCLUDED
