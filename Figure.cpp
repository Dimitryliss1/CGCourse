//
// Created by Павел Жуков on 27/11/2021.
//

#include "Figure.h"
#include <graphics.h>

Figure::Figure(std::vector<Matrix> &points, int type)  {
    if (type == 1){
        polys.emplace_back(Polygon(points[0], points[1], points[2], false, 1));
        polys.emplace_back(Polygon(points[0], points[1], points[3], false, 2));
        polys.emplace_back(Polygon(points[3], points[1], points[2], false, 3));
        polys.emplace_back(Polygon(points[0], points[3], points[2], false, 4));
    }
    if (type == 2){
        polys.emplace_back(Polygon(points[0], points[1], points[3], points[2], false, 1));
        polys.emplace_back(Polygon(points[4], points[5], points[7], points[6], false, 2));
        polys.emplace_back(Polygon(points[2], points[3], points[7], points[6], false, 3));
        polys.emplace_back(Polygon(points[0], points[1], points[5], points[4], false, 4));
        polys.emplace_back(Polygon(points[1], points[3], points[7], points[5], false, 5));
        polys.emplace_back(Polygon(points[0], points[2], points[6], points[4], false, 6));
    }
}

std::vector<Polygon> *Figure::getPolys() {
    return &polys;
}

Matrix Figure::getMidPoint() {
    Matrix res(1, 4);
    int sz = 0;
    for (int i = 0; i < polys.size(); i++){
        std::vector<Matrix> tmp = polys[i].getPoints()[0];
        for (int j = 0; j < tmp.size(); j++){
            sz += 1;
            res = tmp[j] + res;
        }
    }
    return res / sz;
}

void Figure::rotate(int angle, Matrix &rotPoint, char axis) {
    for(auto& p: polys){
        p.rotate(angle, rotPoint, axis);
        p.getEqn();
    }
}

void Figure::scale(float size, Matrix scalePoint) {
    for(auto& p: polys){
        p.scale(size, scalePoint);
        p.getEqn();
    }
}

void Figure::move(int x, int y, int z) {
    for(auto& p: polys){
        p.move(x, y, z);
        p.getEqn();
    }
}

void Figure::convertToScreenCoords() {
    for (auto& p: polys){
        p.convertToScreenCoords();
        p.getEqn();
    }
}

Figure Figure::getShadow(Matrix &LightSource, int maxY) {
    Figure res;
    res.assign(*this);
    for(auto & poly : res.polys){
        poly = poly.getShadow(LightSource, maxY);
        poly.setColor(15);
        poly.setIsShadow(true);
    }
    return res;
}

void Figure::assign(Figure &from) {
    polys.clear();
    for(auto & poly : from.polys){
        polys.emplace_back(Polygon(poly));
    }
}
