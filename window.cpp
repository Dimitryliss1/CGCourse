//
// Created by Павел Жуков on 28/11/2021.
//

#include "window.h"
#include <iostream>

Window::Window(int x1, int y1, int x2, int y2): x1(x1), x2(x2), y1(y1), y2(y2) {
    size = abs(x1 - x2) * abs(y2 - y1);
}
