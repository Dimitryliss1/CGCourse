#include "matrix.h"

#include <iostream>
#include <cmath>

using namespace std;

Matrix::Matrix(int rows, int columns) {
    numRows = rows;
    numColumns = columns;
    contents = new float*[numRows];
    for (int i = 0; i < numRows; i++){
        contents[i] = new float[numColumns];
        for (int j = 0; j < numColumns; j++) {
            contents[i][j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix &matrix){
    numRows = matrix.numRows;
    numColumns = matrix.numColumns;
    contents = new float * [numRows];
    for (int i = 0; i < numRows; i++){
        contents[i] = new float[numColumns];
        for (int j = 0; j < numColumns; j++) {
            contents[i][j] = matrix.contents[i][j];
        }
    }
}

Matrix::Matrix(){
    numRows = 1;
    numColumns = 1;
    contents = new float*[numRows];
    for (int i = 0; i < numRows; i++){
        contents[i] = new float[numColumns];
        for (int j = 0; j < numColumns; j++) {
            contents[i][j] = 0;
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < numRows; i++){
        delete [] contents[i];
    }
    delete [] contents;
}


void Matrix::fill(const float *data, int dataSize){
    int i = 0;
    int j = 0;
    for (i = 0; i < this->numRows; i++) {

        for (j = 0; j < this->numColumns; j++) {
            if (i * this -> numRows + j >= dataSize) break;
            contents[i][j] = data[i*this->numRows + j];
        }
        if (i * this -> numRows + j >= dataSize) break;
    }
}

Matrix &Matrix::operator=(const Matrix &right) {
    if (this == &right){
        return *this;
    }
    for(int i = 0; i < this->numRows; i++){
        delete [] contents[i];
    }
    delete [] contents;
    this->numRows = right.numRows;
    this->numColumns = right.numColumns;
    contents = new float*[this->numRows];
    for (int i = 0; i < this->numRows; i++){
        contents[i] = new float[this->numColumns];
        for (int j = 0; j < this->numColumns; j++){
            contents[i][j] = right.contents[i][j];
        }
    }
    return *this;
}

Matrix operator*(const Matrix &Matrix1, const Matrix &Matrix2) {
    Matrix res(Matrix1.numRows, Matrix2.numColumns);
    for (int row = 0; row < Matrix1.numRows; row++){
        for (int col = 0; col < Matrix2.numColumns; col++){
            for (int item = 0; item < Matrix1.numColumns; item++){
                res.contents[row][col] += Matrix1.contents[row][item] * Matrix2.contents[item][col];
            }
        }
    }
    return res;
}

Matrix operator+(const Matrix &Matrix1, const Matrix &Matrix2) {
    Matrix res(Matrix1.numRows, Matrix1.numColumns);
    for (int row = 0; row < Matrix1.numRows; row++){
        for (int col = 0; col < Matrix1.numColumns; col++){
            res.contents[row][col] += Matrix1.contents[row][col] + Matrix2.contents[row][col];
        }
    }
    return res;
}

Matrix operator/(const Matrix &Matrix1, float num) {
    Matrix res(Matrix1.numRows, Matrix1.numColumns);
    for (int row = 0; row < Matrix1.numRows; row++){
        for (int col = 0; col < Matrix1.numColumns; col++){
            res.contents[row][col] += Matrix1.contents[row][col] / num;
        }
    }
    return res;
}


float Matrix::getByRowCol(int row, int col){
  return contents[row][col];
}

Matrix::Matrix(float x, float y, float z) {
    numRows = 1;
    numColumns = 4;
    contents = new float*[numRows];
    for (int i = 0; i < numRows; i++){
        contents[i] = new float[numColumns];
        for (int j = 0; j < numColumns; j++) {
            contents[i][j] = 1;
        }
    }
    contents[0][0] = x;
    contents[0][1] = y;
    contents[0][2] = z;

}

bool Matrix::operator==(const Matrix &rhs) const {
    return numRows == rhs.numRows &&
           numColumns == rhs.numColumns &&
           contents == rhs.contents;
}

bool Matrix::operator!=(const Matrix &rhs) const {
    return !(rhs == *this);
}

Matrix generate3DRotationMatrix(int angle, char axis) {
    Matrix res(4, 4);
    float *data = new float[16];
    for (int i = 0; i < 16; i++) data[i] = 0;
    if (axis == 'z') {
        data[0] = cos(M_PI * angle / 180);
        data[5] = cos(M_PI * angle / 180);
        data[1] = sin(M_PI * angle / 180);
        data[4] = -sin(M_PI * angle / 180);
        data[10] = 1;
        data[15] = 1;
    } else if (axis == 'x') {
        data[0] = 1;
        data[15] = 1;
        data[5] = cos(M_PI * angle / 180);
        data[6] = sin(M_PI * angle / 180);
        data[9] = -sin(M_PI * angle / 180);
        data[10] = cos(M_PI * angle / 180);
    } else if (axis == 'y') {
        data[5] = 1;
        data[15] = 1;
        data[0] = cos(M_PI * angle / 180);
        data[8] = sin(M_PI * angle / 180);
        data[2] = -sin(M_PI * angle / 180);
        data[10] = cos(M_PI * angle / 180);
    }
    res.fill(data, 16);
    delete[] data;
    return res;
}

Matrix generate3DScaleMatrix(float scale) {
    Matrix res(4, 4);
    float *data = new float[16];
    for (int i = 0; i < 16; i++) data[i] = 0;
    data[15] = 1;
    data[0] = scale;
    data[5] = scale;
    data[10] = scale;
    res.fill(data, 16);
    delete[] data;
    return res;
}

Matrix generate3DMoveMatrix(int x, int y, int z) {
    Matrix res(4, 4);
    float *data = new float[16];
    for (int i = 0; i < 16; i++) data[i] = 0;
    data[0] = 1;
    data[5] = 1;
    data[10] = 1;
    data[15] = 1;
    data[12] = x;
    data[13] = y;
    data[14] = z;
    res.fill(data, 16);
    delete[] data;
    return res;
}

Matrix generateShadowMatrix(Matrix& lSource, float maxY){
    Matrix res(4, 4);
    for (int i = 0; i < 16; i++) res.contents[i / 4][i % 4] = 0;

    res.contents[0][0] = maxY - lSource.getByRowCol(0, 1);
    res.contents[1][0] = lSource.getByRowCol(0, 0);
    res.contents[1][1] = maxY;
    res.contents[1][2] = lSource.getByRowCol(0, 2);
    res.contents[1][3] = 1;
    res.contents[2][2] = maxY - lSource.getByRowCol(0, 1);
    res.contents[3][0] = -maxY * lSource.getByRowCol(0, 0);
    res.contents[3][1] = -maxY * lSource.getByRowCol(0, 1);
    res.contents[3][2] = -maxY * lSource.getByRowCol(0, 2);
    res.contents[3][3] = -lSource.getByRowCol(0, 1);
    return res;
}