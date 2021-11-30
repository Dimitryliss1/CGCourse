#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
private:
    int numRows, numColumns;
    float** contents;
public:
    Matrix(int rows, int columns);
    Matrix(float x, float y, float z);
    Matrix(const Matrix &matrix);
    Matrix();
    ~Matrix();
    void fill(const float *data, int dataSize);
    Matrix& operator = (const Matrix& right);
    float getByRowCol(int row, int col);
    friend Matrix operator * (const Matrix& Matrix1, const Matrix& Matrix2);
    friend Matrix operator + (const Matrix& Matrix1, const Matrix& Matrix2);
    friend Matrix operator / (const Matrix& Matrix1, float num);

    bool operator==(const Matrix &rhs) const;

    bool operator!=(const Matrix &rhs) const;
    friend Matrix generateShadowMatrix(Matrix& lSource, float maxY);
};

Matrix generate3DRotationMatrix(int angle, char axis);
Matrix generate3DScaleMatrix(float scale);
Matrix generate3DMoveMatrix(int x, int y, int z);

#endif
