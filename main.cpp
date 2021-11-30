#include <iostream>
#include <vector>
#include "Polygon.h"
#include "Figure.h"
#include "window.h"
#include <graphics.h>
#include <algorithm>

using namespace std;

std::vector<Figure> init();
void fill(int x, int y, int color, int xScr1 = 0, int xScr2 = getmaxx(), int yScr1 = 0, int yScr2 = getmaxy());
void draw(std::vector<Figure>& figures, int x1, int y1, int x2, int y2);

int main() {
    initwindow(600, 600);
    sdlbgiauto();
    std::vector<Figure> figs = init();
//    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
    int curFig = 0;
    Matrix rotPoint = figs[curFig].getMidPoint();
    while(true) {
        char c = getch();
        if (c == ' ') {
            closegraph();
            return 0;
        } else {
            rotPoint = figs[curFig].getMidPoint();
            switch (c) {
                case 'q':
                    figs[curFig].move(0, 0, -50);
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                case 'e':
                    figs[curFig].move(0, 0, 50);
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                case 'w':
                    figs[curFig].move(0, -50, 0);
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                case 'a':
                    figs[curFig].move(-50, 0, 0);
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                case 's':
                    figs[curFig].move(0, 50, 0);
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                case 'd':
                    figs[curFig].move(50, 0, 0);
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                case 'r': {
                    figs[curFig].rotate(10, rotPoint, 'x');
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                }
                case 't': {
                    figs[curFig].rotate(-10, rotPoint, 'x');
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                }
                case 'f': {
                    figs[curFig].rotate(10, rotPoint, 'y');
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                }
                case 'g': {
                    figs[curFig].rotate(-10, rotPoint, 'y');
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                }
                case 'c': {
                    figs[curFig].rotate(10, rotPoint, 'z');
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                }
                case 'v': {

                    figs[curFig].rotate(-10, rotPoint, 'z');
                    setbkcolor(0);
                    draw(figs, 0, 0, getmaxx() + 1, getmaxy() + 1);
                    break;
                }
                case 'l':
                    curFig += 1;
                    curFig %= figs.size();
                    break;
            }
        }
    }
}

void fill(int x, int y, int color, int xScr1, int xScr2, int yScr1, int yScr2){
    struct toW{
        toW(int i, int i1): x(i), y(i1) {};
        toW() = default;
        int x, y;
    }init;
    init.x = x;
    init.y = y;
    vector <toW> stack;
    stack.emplace_back(init);
    while(!stack.empty()) {
        int curx = stack[stack.size() - 1].x;
        int curY = stack[stack.size() - 1].y;
        stack.pop_back();
        if (getpixel(curx, curY) != 0) {
            continue;
        } else {
            putpixel(curx, curY, color);
            if (curx - 1 >= xScr1) stack.emplace_back(curx - 1, curY);
            if (curx + 1 <= xScr2) stack.emplace_back(curx + 1, curY);
            if (curY - 1 >= yScr1) stack.emplace_back(curx, curY - 1);
            if (curY + 1 <= yScr2) stack.emplace_back(curx, curY + 1);
        }
    }
}

bool comp(Polygon& a, Polygon& b){
    return a.getMidZ() < b.getMidZ();
}

void draw(std::vector<Figure>& figures, int x1, int y1, int x2, int y2){
    vector<Window>stack;
    Window fst(x1, y1, x2, y2);
    stack.push_back(fst);
    vector<Polygon> unsorted;
    for (auto &figure : figures) {
        for (int j = 0; j < figure.getPolys()->size(); j++) {
            unsorted.emplace_back(Polygon(figure.getPolys()[0][j]));
            unsorted[unsorted.size() - 1].convertToScreenCoords();
            unsorted[unsorted.size() - 1].getEqn();
        }
    }
    sort(unsorted.begin(), unsorted.end(), comp);
//    Matrix LightSource(0, 0, 0);
//    for (auto& figure: figures){
//        for (int j = 0; j < figure.getPolys()->size(); j++) {
//            unsorted.emplace_back(unsorted[j].getShadow(LightSource, getmaxy()));
//            unsorted[unsorted.size() - 1].convertToScreenCoords();
//            unsorted[unsorted.size() - 1].getEqn();
//            for (auto& k: *unsorted[unsorted.size() - 1].getPoints()){
//                std::cout << k.getByRowCol(0, 0) << ' ' << k.getByRowCol(0, 1) << ' ' << k.getByRowCol(0, 2) << '\n';
//            }
//        }
//    }
    int cnt = 0;
    while (!stack.empty()) {
        Window win = stack[stack.size() - 1];
        stack.pop_back();
        if (win.size < 1) continue;
        int xStart = win.x1;
        int yStart = win.y1;
        int xEnd = win.x2;
        int yEnd = win.y2;
//        line(xStart, yStart, xEnd - 1, yStart);
//        line(xEnd - 1, yStart, xEnd - 1, yEnd - 1);
//        line(xEnd - 1, yEnd - 1, xStart, yEnd - 1);
//        line(xStart, yEnd - 1, xStart, yStart);
        vector<Polygon> in;
        vector<Polygon> out;
        vector<Polygon> intersect;
        vector<Polygon> overlap;
        vector<int> xs = {xStart, xEnd - 1};
        vector<int> ys = {yStart, yEnd - 1};

        for (auto &poly: unsorted) {
            int tmp = poly.getAmtOfPointsInsidePoly(xs, ys);
            if (tmp == 4) {
                overlap.emplace_back(Polygon(poly));
            } else if (tmp < 4 && tmp > 0) {
                intersect.emplace_back(Polygon(poly));
            } else {
                int ptsInWin = 0;
                vector<Matrix> pts = poly.getPoints()[0];
                for (auto &pt : pts) {
                    float x = pt.getByRowCol(0, 0);
                    float y = pt.getByRowCol(0, 1);
                    if (x >= xStart && x < xEnd && y >= yStart && y < yEnd) {
                        ptsInWin++;
                    }
                }
                if (ptsInWin == 0) {
                    out.emplace_back(Polygon(poly));
                } else if (ptsInWin == pts.size()) {
                    in.emplace_back(Polygon(poly));
                } else {
                    intersect.emplace_back(Polygon(poly));
                }
            }
        }

        if (out.size() == unsorted.size()) {
            continue;
        }
        if (win.size == 1){
            vector<Polygon>tmp;
            tmp.reserve(overlap.size() + in.size() + intersect.size());
            for(auto& i: overlap){
                tmp.push_back(i);
            }
            for(auto& i: in){
                tmp.push_back(i);
            }
            for(auto& i: intersect){
                tmp.push_back(i);
            }
            sort(tmp.begin(), tmp.end(), comp);
            putpixel(xStart, yStart, tmp[0].getColor());
            continue;
        }

        if (!intersect.empty() || !in.empty()) {
            if (win.size > 1) {
                stack.emplace_back(xStart, yStart, (xStart + xEnd) / 2, (yStart + yEnd) / 2);
                stack.emplace_back((xStart + xEnd) / 2, (yStart + yEnd) / 2, xEnd, yEnd);
                stack.emplace_back(xStart, (yStart + yEnd) / 2, (xStart + xEnd) / 2, yEnd);
                stack.emplace_back((xStart + xEnd) / 2, yStart, xEnd, (yStart + yEnd) / 2);
            }
            continue;
        }

        if (!overlap.empty()) {
            bool flag = false;
            float zAt1 = overlap[0].getZat(xStart, yStart);
            float zAt2 = overlap[0].getZat(xEnd - 1, yStart);
            float zAt3 = overlap[0].getZat(xEnd - 1, yEnd - 1);
            float zAt4 = overlap[0].getZat(xStart, yEnd - 1);
            int color = overlap[0].getColor();
            for (auto &poly: overlap) {

                if (zAt1 > poly.getZat(xStart, yStart)) {
                    flag = true;
                    break;
                }
                if (zAt2 > poly.getZat(xEnd - 1, yStart)) {
                    flag = true;
                    break;
                }
                if (zAt3 > poly.getZat(xEnd - 1, yEnd - 1)) {
                    flag = true;
                    break;
                }
                if (zAt4 > poly.getZat(xStart, yEnd - 1)) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                if (xEnd - xStart <= 1 && yEnd - yStart <= 1) {
                    continue;
                }
                if (win.size > 1) {
                    stack.emplace_back(xStart, yStart, (xStart + xEnd) / 2, (yStart + yEnd) / 2);
                    stack.emplace_back((xStart + xEnd) / 2, (yStart + yEnd) / 2, xEnd, yEnd);
                    stack.emplace_back(xStart, (yStart + yEnd) / 2, (xStart + xEnd) / 2, yEnd);
                    stack.emplace_back((xStart + xEnd) / 2, yStart, xEnd, (yStart + yEnd) / 2);
                }
                continue;
            }
            fill((xStart + xEnd) / 2, (yStart + yEnd) / 2, color, xStart, xEnd - 1, yStart, yEnd - 1);
            cnt++;
        }
    }
}

std::vector<Figure> init(){
    std::vector<Figure>res;
    std::vector<Matrix>pts;
    pts.emplace_back(Matrix(300, 400, 100));
    pts.emplace_back(Matrix(500, 400, 100));
    pts.emplace_back(Matrix(400, 400, 300));
    pts.emplace_back(Matrix(400, 200, 166));
    res.emplace_back(Figure(pts, 1));
    pts.clear();

    pts.emplace_back(Matrix(100, 400, 100));
    pts.emplace_back(Matrix(200, 400, 100));
    pts.emplace_back(Matrix(100, 400, 300));
    pts.emplace_back(Matrix(200, 400, 300));
    pts.emplace_back(Matrix(100, 200, 100));
    pts.emplace_back(Matrix(200, 200, 100));
    pts.emplace_back(Matrix(100, 200, 300));
    pts.emplace_back(Matrix(200, 200, 300));

    res.emplace_back(Figure(pts, 2));
    return res;
}
