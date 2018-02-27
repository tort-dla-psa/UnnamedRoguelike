#include"camera.h"
#include"../gameobject.h"

camera::camera(gameobjectmovable* target){
        targetmov = target;
        targetstat = nullptr;
}

camera::camera(gameobjectstatic* target){
        targetstat = target;
        targetmov = nullptr;
}

camera::~camera(){}

void camera::SetParams(ushort width, ushort height){
        this->width = width;
        this->height=  height;
}


ushort camera::GetOffsetX(){
        return GetX()-width/2;
}

ushort camera::GetOffsetY(){
        return GetY()-height/2;
}

void camera::Follow(gameobjectmovable* newtarget){
        targetmov = newtarget;
        targetstat = nullptr;
}

void camera::Follow(gameobjectstatic* newtarget){
        targetstat = newtarget;
        targetmov = nullptr;
}

ushort camera::GetX(){
        if(targetmov)
                return targetmov->GetX();
        return targetstat->GetX();
}
ushort camera::GetY(){
        if(targetmov)
                return targetmov->GetY();
        return targetstat->GetY();
}
ushort camera::GetZ(){
        if(targetmov)
                return targetmov->GetZ();
        return targetstat->GetZ();
}
bool camera::Flying(){  return (targetstat);}
