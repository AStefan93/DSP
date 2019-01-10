/** \file perspective.h
 *  \brief This file includes all the resources necessary to implement the geometrical transformations used in DSP.
 */

#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include <stdlib.h> //for exit function
#include <iostream> //for cout

template <typename T>
class Matrix{

    public:
        Matrix();//default constructor with size 2x2 and 0 values
        ~Matrix();//default destructor
        Matrix(unsigned char N, unsigned char M);//constructor for matrices of size NxM
        unsigned char rows();//return value of r
        unsigned char cols();//return value of c
        void setRows(unsigned char);//set the value of r
        void setCols(unsigned char);//set the value of c
        void setValue(unsigned char N, unsigned char M, T value);//sets the value
        void setValue(unsigned char N, unsigned char M, T* v);//sets all values from size NxM array
        T getValue(unsigned char, unsigned char); //returns the value of selected position

        Matrix operator=(const Matrix& );
        Matrix operator*(const Matrix& );
        Matrix operator+(const Matrix& );

    private:

        T* matrixValues;

	    unsigned char r;//rows
    	unsigned char c;//columns

};


#endif
