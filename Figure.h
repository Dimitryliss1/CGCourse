//
// Created by Павел Жуков on 27/11/2021.
//

#ifndef CGCOURSE_FIGURE_H
#define CGCOURSE_FIGURE_H

#ifndef CGCOURSE_POLYGON_H
#include "Polygon.h"
#endif

#include <vector>

class Figure {
    std::vector<Polygon> polys;
public:
    Figure() = default;
    Figure(std::vector<Matrix>& points, int type); // type = 1 -- пирамида, 2 -- параллелепипед
    std::vector<Polygon>* getPolys();
    Matrix getMidPoint();
    ~Figure() = default;
    void rotate(int angle, Matrix& rotPoint, char axis);
    void scale(float size, Matrix scalePoint);
    void move(int x, int y, int z);
};


#endif //CGCOURSE_FIGURE_H
