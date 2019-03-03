/** \file perspective.cpp
 *  \brief This file contains the perspective transformations used in DSP.
 *
 *  This file includes the perspective header only.
 */
#include "perspective.h"

template <typename T>
Matrix<T>::Matrix(){
    ///Defaul Matrix constructor.

    /**
    *   Constructor which creates a 2x2 size Matrix and initializes all values with 0.
    */

    unsigned char defaultRows = 2;
    unsigned char defaultCols = 2;

    setRows(defaultRows);
    setCols(defaultCols);

    matrixValues = new T[defaultRows*defaultCols];

    setValue(0,0,0);// M(0,0) = 0
    setValue(0,1,0);// M(0,1) = 0
    setValue(1,0,0);// M(1,0) = 0
    setValue(1,1,0);// M(1,1) = 0

}

template <typename T>
Matrix<T>::Matrix(unsigned char N, unsigned char M){
    ///Matrix constructor with size NxM.

    /**
    *   Constructor which creates an NxM size Matrix and initializes all values with 0.
    */


    setRows(N);
    setCols(M);

    matrixValues = new T[N*M];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            setValue(i,j,0); //set all values to 0
        }
    }

}

template <typename T>
Matrix<T>::~Matrix(){
    ///Default Matrix destructor.

    /**
    *   Destructor deletes the matrixValues data and resets the pointer to NULL.
    */

    delete[] matrixValues;
    matrixValues = NULL;

}

template <typename T>
unsigned char Matrix<T>::rows(){
    ///Get rows function for Matrix class.

    /**
    *   The rows function returns the number of rows of the matrix.
    */

    return r;
}

template <typename T>
unsigned char Matrix<T>::cols(){
    ///Get cols function for Matrix class.

    /**
    *   The cols function returns the number of columns of the matrix.
    */

    return c;
}

template <typename T>
void Matrix<T>::setRows(unsigned char row_value){
    ///Set rows function for Matrix class.

    /**
    *   The setRows function sets r with the given parameter value.
    *   @param row_value is the number of rows for this matrix object.
    */

    r = row_value;
}

template <typename T>
void Matrix<T>::setCols(unsigned char col_value){
    ///Set columns function for Matrix class.

    /**
    *   The setCols function sets c with the given parameter value.
    *   @param col_value is the number of columns for this matrix object.
    */

    c = col_value;
}

template <typename T>
void Matrix<T>::setValue(unsigned char N, unsigned char M, T value_par){
    ///Set the value of a single member in the Matrix.

    /**
    *   The setValue function simply overwrites the value of the Matrix at position [N,M] with "value_par".
    *   @param N is the row position of the member.
    *   @param M is the column position of the member.
    *   @param value_par is the actual value.
    */

    matrixValues[N*c + M] = value_par;

}

template <typename T>
T Matrix<T>::getValue(unsigned char N, unsigned char M){
    ///Get the value of a single member in the Matrix.

    /**
    *   The getValue function returns the value of the matrixValues[N*c + M] array.
    *   @param N is the row position of the member.
    *   @param M is the column position of the member.
    */

    if((N < r) && (M < c)){
        return matrixValues[N*c + M];
    }else{
        std::cout << "Position accessed is out of bounds. Most likely N or M is bigger than the size of the Matrix." << std::endl;
        exit(0);
    }

}

template Matrix<int>::Matrix();
template Matrix<int>::Matrix(unsigned char N, unsigned char M);
template Matrix<int>::~Matrix();
template unsigned char Matrix<int>::rows();
template unsigned char Matrix<int>::cols();
template void Matrix<int>::setRows(unsigned char row_value);
template void Matrix<int>::setCols(unsigned char row_value);
template void Matrix<int>::setValue(unsigned char N, unsigned char M, int val);
template int Matrix<int>::getValue(unsigned char N, unsigned char M);

