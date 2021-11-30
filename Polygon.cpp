//
// Created by Павел Жуков on 27/11/2021.
//

#include "Polygon.h"

std::vector<Matrix> *Polygon::getPoints() {
    return &points;
}

Matrix Polygon::getEqn() {
    std::vector<Matrix> ptr = Polygon::getPoints()[0];
    float y1 = ptr[0].getByRowCol(0,1);
    float y2 = ptr[1].getByRowCol(0,1);
    float y3 = ptr[2].getByRowCol(0,1);
    float x1 = ptr[0].getByRowCol(0,0);
    float x2 = ptr[1].getByRowCol(0,0);
    float x3 = ptr[2].getByRowCol(0,0);
    float z1 = ptr[0].getByRowCol(0,2);
    float z2 = ptr[1].getByRowCol(0,2);
    float z3 = ptr[2].getByRowCol(0,2);
//    x1 += z1/2;
//    y1 -= z1/2;
//    x2 += z2/2;
//    y2 -= z2/2;
//    x3 += z3/2;
//    y3 -= z3/2;
    float eq[4] = {(float) (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1),
                   (float) (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1),
                   (float) (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1),
                   0};
    float tmp = 0;
    tmp += (-x1) * ((y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1));
    tmp += (-y1) * ((z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1));
    tmp += (-z1) * ((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
    eq[3] = tmp;
    equation.fill(eq, 4);
    return equation;
}

float Polygon::getZat(float x, float y) {
    float z = 0;
    this->getEqn();
    z = (-x*equation.getByRowCol(0,0) - y*equation.getByRowCol(0,1) - equation.getByRowCol(0,3))/(equation.getByRowCol(0,2));
    return z;
}

float Polygon::getMidZ() {
    if (isShadow){
        return 1000000000.0;
    }
    float res = 0;
    for (auto & point : points) {
        res += point.getByRowCol(0, 2);
    }
    return res/3;
}

Matrix Polygon::getCover() {
    float xmin = 1000000000.0, xmax = -1000000000.0;
    float ymin = 1000000000.0, ymax = -1000000000.0;
    for (auto & point : points) {
        if (point.getByRowCol(0,0) < xmin) {
            xmin = point.getByRowCol(0,0);
        }
        if (point.getByRowCol(0,0) > xmax){
            xmax = point.getByRowCol(0,0);
        }
        if (point.getByRowCol(0,1) < ymin) {
            ymin = point.getByRowCol(0,0);
        }
        if (point.getByRowCol(0,1) > ymax){
            ymax = point.getByRowCol(0,0);
        }
    }
    Matrix res (1, 4);
    float data[4] = {xmin, ymin, xmax, ymax};
    res.fill(data, 4);
    return res;
}

Polygon Polygon::getShadow(Matrix &LightSource, float maxY) {
    Polygon res;
    res.assign(*this);
    Matrix tmp = generateShadowMatrix(LightSource, maxY);
    for (auto& i: res.points){
        i = i * tmp;
    }
    res.isShadow = true;
    res.color = 15;
    return res;
}

void Polygon::rotate(int angle, Matrix &rotPoint, char axis) {
    float rotX = rotPoint.getByRowCol(0, 0);
    float rotY = rotPoint.getByRowCol(0, 1);
    float rotZ = rotPoint.getByRowCol(0, 2);
    Matrix move = generate3DMoveMatrix(-rotX, -rotY, -rotZ);
    Matrix negMove = generate3DMoveMatrix(rotX, rotY, rotZ);
    Matrix rot = generate3DRotationMatrix(angle, axis);
    for(auto & point : points){
        point = point*move*rot*negMove;
    }
}

void Polygon::scale(float size, Matrix scalePoint) {
    float rotX = scalePoint.getByRowCol(0, 0);
    float rotY = scalePoint.getByRowCol(0, 1);
    float rotZ = scalePoint.getByRowCol(0, 2);
    Matrix move = generate3DMoveMatrix(-rotX, -rotY, -rotZ);
    Matrix negMove = generate3DMoveMatrix(rotX, rotY, rotZ);
    Matrix scale = generate3DScaleMatrix(size);
    for(auto & point : points){
        point = point*move*scale*negMove;
    }
}

void Polygon::move(int x, int y, int z) {
    Matrix move = generate3DMoveMatrix(x, y, z);
    for(auto & point : points){
        point = point*move;
    }
}

void Polygon::convertToScreenCoords() {
    Matrix projection(4,4);
    float data[16] = {1, 0, 0, 0, 0, 1, 0, 0, 1.0 / 2,  -1.0 / 2, 1, 0, 0, 0, 0, 1};
    projection.fill(data, 16);
    for(auto & point : points){
        point = point*projection;
    }
    getEqn();
}

void Polygon::assign(Polygon &from) {
    points.clear();
    for(auto & point : *from.getPoints()){
        points.emplace_back(point);
    }
}

Polygon::Polygon(Matrix &pt1, Matrix &pt2, Matrix &pt3, bool isSh, int c) {
    color = c;
    isShadow = isSh;
    points.emplace_back(Matrix(pt1));
    points.emplace_back(Matrix(pt2));
    points.emplace_back(Matrix(pt3));
}

Polygon::Polygon(Matrix &pt1, Matrix &pt2, Matrix &pt3, Matrix &pt4, bool isSh, int c) {
    color = c;
    isShadow = isSh;
    points.emplace_back(Matrix(pt1));
    points.emplace_back(Matrix(pt2));
    points.emplace_back(Matrix(pt3));
    points.emplace_back(Matrix(pt4));
}

Polygon::Polygon(Polygon const &from) {
    isShadow = from.isShadow;
    color = from.color;
    for(const auto & point : from.points){
        points.emplace_back(Matrix(point));
    }
    equation = this->getEqn();
}

int Polygon::getAmtOfPointsInsidePoly(std::vector<int>& xp, std::vector<int>& yp) {
    getEqn();
    int res = 0;
    for (auto& x: xp) {
        for (auto &y: yp) {
            bool c = false;
            for (int i = 0, j = points.size() - 1; i < points.size(); j = i++) {
                int yi = points[i].getByRowCol(0, 1);
                int yj = points[j].getByRowCol(0, 1);
                int xi = points[i].getByRowCol(0, 0);
                int xj = points[j].getByRowCol(0, 0);
                if ( ((yi > y) != (yj > y)) && (x < (xj - xi) *(y - yi) / (yj -yi) + xi)){
                    c = !c;
                }
            }
            res += c;
        }
    }
    return res;
}

int Polygon::getColor() {
    return color;
}

void Polygon::setIsShadow(bool isShadow) {
    Polygon::isShadow = isShadow;
}

void Polygon::setColor(int color) {
    Polygon::color = color;
}

bool Polygon::operator==(const Polygon &rhs) const {
    return points == rhs.points &&
           equation == rhs.equation &&
           isShadow == rhs.isShadow &&
           color == rhs.color;
}

bool Polygon::operator!=(const Polygon &rhs) const {
    return !(rhs == *this);
}
