/***********************************************************************************************
This header file contains the Symmetric matrix class - SymMat

It stores only the upper triangle of the matrix in classical packed format.
The elements are stored such as first row is stored first , then second , then third row and so on.

Aim of this header file is to provide a symmetric matrix class which stores only the needed elements
and hence reduce the storage space and optimise the operations since the number of elements on which 
the operations are performed are less. 

************************************************************************************************/
//-----------------------------------------------------------------------------------------------


/************************************************************************************************
						INCLUDE GAURDS
*************************************************************************************************/
#ifndef SYMMAT_H
#define SYMMAT_H


/*************************************************************************************************
						INCLUDING THE DEPENDENCIES
**************************************************************************************************/
#include <vector>				//to use vector and its inbuilt functions
#include <initializer_list>		//to initilise the matrix
#include <iostream> 			//for cout in print function
#include <iomanip>  			//to use setw()
#include <cassert>  			//to use assert function
#include <cmath>				//to calculate the squareroot of a number
#include <cstdlib>  			//to use std::exit function 
#include <Eigen/Eigen> 			//to pass eigen matrix as arguments to functions


/*************************************************************************************************
						CLASS DEFINITION
						----------------
Template parameters used are same as in eigen so that it will be easy to merge with the main branch
**************************************************************************************************/
template <typename _Scalar>
class SymMat
{
public:

	//Order of the matrix
	int order;

	//Vector which stores the elements of the matrix
	std::vector<_Scalar> mat;
	
	//Initializer list
	SymMat(std::initializer_list<_Scalar>);

	//Default constructor
	SymMat();

	//Parametrized constructor(parameter is order of matrice)
	SymMat(int);
	
	//Returns the index of the element (i,j) stored in the vector
	int index(int,int);
	
	//Prints the matrix 
	void print();

	//Trace of matrix
	_Scalar trace();

	//Overloading the funtion call operator
	_Scalar& operator()(int,int);

};

/***************************************************************************************************
						FUNCTION PROTOTYPES
						-------------------
This section contains the prototypes of the functions used:-
****************************************************************************************************/

//Addition------------------------------------------------------------------------------------------
template<typename _Scalar>
SymMat<_Scalar> add(SymMat<_Scalar>&,SymMat<_Scalar>&);

template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> add(SymMat<_Scalar>&,Eigen::Matrix<_Scalar,_Rows,_Cols>&);

template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> add(Eigen::Matrix<_Scalar,_Rows,_Cols>&,SymMat<_Scalar>&);


//Subtraction----------------------------------------------------------------------------------------
template<typename _Scalar>
SymMat<_Scalar> sub(SymMat<_Scalar>&,SymMat<_Scalar>&);

template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> sub(SymMat<_Scalar>&,Eigen::Matrix<_Scalar,_Rows,_Cols>&);

template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> sub(Eigen::Matrix<_Scalar,_Rows,_Cols>&,SymMat<_Scalar>&);


//Multiplication--------------------------------------------------------------------------------------
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> mult(SymMat<_Scalar>&,SymMat<_Scalar>&);

template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> mult(SymMat<_Scalar>&,Eigen::Matrix<_Scalar,_Rows,_Cols>&);

template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> mult(Eigen::Matrix<_Scalar,_Rows,_Cols>&,SymMat<_Scalar>&);



/*****************************************************************************************************
						CONSTRUCTOR
******************************************************************************************************/

//Initialize with a default 3x3 matrix initialised with '0'
template<typename _Scalar>
SymMat<_Scalar>::SymMat()
{
	int elements,i;
	order=3;
	elements=6;
	mat.resize(elements);
	std::fill(mat.begin(),mat.end(),0); //filling all elements of the vector with '0'
}

//Custom matrice of order as inputted by user intialised with '0'
template<typename _Scalar>
SymMat<_Scalar>::SymMat(int o)
{
	int elements;
	order=o;
	elements=(order*(order+1))/2;
	mat.resize(elements);
	std::fill(mat.begin(),mat.end(),0); //filling all elements of the vector with '0'
}

//Using initializer_list to initialize the matrice
template<typename _Scalar>
SymMat<_Scalar>::SymMat(std::initializer_list<_Scalar> list) :mat(list) 
{
	/*First it is checked whether the number of elements entered are equal to the elements in
	  the upper triangle of a square matrix.	
	  Since the the no. of elements follow the following series:- 3(for 2x2),6(for 3x3),10(for 4x4),... 
	  the following formula is used to calculate the order of the matrix and whether such an order exist:-
	  -----------------------------------
	  no.of elements=(order*(order+1))/2 
	  (Sum of arithmetic series:- 1,2,3,4...)
	  -----------------------------------
	  */
	order=int(sqrt(2*(mat.size())));
	try
	{
		if((order*(order+1))/2!=mat.size())
		{
			throw 'f';	
		}
		
	}
	catch(char& check)
	{
		std::cout<<"Wrong number of elements for a upper triangle of square matrix!\nTerminating the program..."<<std::endl;
		std::exit(0);
	}
	
}

/*******************************************************************************************************
						OVERLOADING FUNCTION CALL OPERATOR
********************************************************************************************************/
template<typename _Scalar>
_Scalar& SymMat<_Scalar>::operator()(int i, int j)
{
	if(i>=j)
	{
		return mat[index(i,j)];
	}
	else
	{
		//Since the matrix is symmetric, it means that A(i,j)==A(j,i)
		return mat[index(j,i)];
	}

}

/********************************************************************************************************
				CALCULATES THE INDEX OF ELEMENT AT (i,j) IN VECTOR
*********************************************************************************************************/
template<typename _Scalar>
int SymMat<_Scalar>::index(int i, int j)
{
	//to make calculations easy to understand (since the index starts from 0)
	i=i+1;
	j=j+1;

	int Index,total_elements,below_elements,temp;
	if(i>j)  //swapping i and j since the 
	{
		temp=i;
		i=j;
		j=temp;
	}

	//This variable stores the total no. of elements in the triangle
	total_elements=((order)*(order+1))/2;

	//This variable stores the elements which are below/next (from top to bottom) the element to be accessed
	below_elements=((order-i)*(order-i+1))/2 +(order-j);

	//This gives us the index of that element
	Index=total_elements-below_elements-1;	

	return Index;

}


/**********************************************************************************************************
						PRINT MATRIX
***********************************************************************************************************/

//Print function for printing the matrix belonging to SymMat class on screen
template<typename _Scalar>
void SymMat<_Scalar>::print()
{
  int i,j;
  for(i=0;i<order;i++)
  {
  	for (j=0;j<order;j++)
  	{  
  		std::cout<<std::setw(4)<<mat[index(i,j)]<<" ";  //setw() here sets 4 digits space for each element
	} 
  	std::cout<<"\n";
  }
}

/**********************************************************************************************************
						TRACE OF MATRIX
***********************************************************************************************************/

//Trace function to return trace of matrix
template<typename _Scalar>
_Scalar SymMat<_Scalar>::trace()
{

  //This variable stores the trace of the matrix
  _Scalar store_trace=0;  

  for(int i=0;i<order;i++)
  {
  	store_trace += mat[index(i,i)];
  }
  return store_trace;
}


/***********************************************************************************************************
						ADDITION
					---------------	
There is function overloading for the following:
1)Both belongs to SymMat
2)First belongs to SymMat and other to Eigen::Matrix
3)First belongs to Eigen::Matrix and other to SymMat
************************************************************************************************************/


//Addition function for two matrices belonging to SymMat class
template<typename _Scalar>
SymMat<_Scalar> add(SymMat<_Scalar>& m1,SymMat<_Scalar>& m2)
{
	assert((m1.mat).size()==(m2.mat).size());       //Condition for matrices to be conformable for addition
	SymMat<_Scalar> m3(m1.order);					//Creating a matrix which stores the addition of these two matrices

	for(int i=0;i<(m1.mat).size();i++)
	{
		m3.mat[i]=m1.mat[i]+m2.mat[i];
	}
	return m3;

}


//Addition function between one matrix belonging to SymMat class(First argument) and another(Second argument) to Eigen::Matrix
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> add(SymMat<_Scalar>& m1,Eigen::Matrix<_Scalar,_Rows,_Cols>& m2)
{
	assert(m1.order == m2.rows() && m1.order == m2.cols());       //Condition for matrices to be conformable for addition
	Eigen::Matrix<_Scalar,_Rows,_Cols> m3;
	for(int i=0;i<m1.order;i++)
	{
		for(int j=0;j<m1.order;j++)
		{
			m3(i,j)=m1.mat[m1.index(i,j)]+m2(i,j);
		}
	}
	return m3;
}


//Addition function between one matrix belonging to Eigen::matrix(First argument) and SymMat class(Second argument) to Eigen::Matrix
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> add(Eigen::Matrix<_Scalar,_Rows,_Cols>& m2,SymMat<_Scalar>& m1)
{
	assert(m1.order==m2.rows() && m1.order == m2.cols());       //Condition for matrices to be conformable for addition
	Eigen::Matrix<_Scalar,_Rows,_Cols> m3;
	for(int i=0;i<m1.order;i++)
	{
		for(int j=0;j<m1.order;j++)
		{
			m3(i,j)=m1.mat[m1.index(i,j)]+m2(i,j);
		}
	}
	return m3;
}



/********************************************************************************************************
						SUBTRACTION
					------------------	
There is function overloading for the following:
1)Both belongs to SymMat
2)First belongs to SymMat and other to Eigen::Matrix
3)First belongs to Eigen::Matrix and other to SymMat
**********************************************************************************************************/

//Addition function for two matrices belonging to SymMat class
template<typename _Scalar>
SymMat<_Scalar> sub(SymMat<_Scalar>& m1,SymMat<_Scalar>& m2)
{
	assert((m1.mat).size()==(m2.mat).size());       //Condition for matrices to be conformable for addition
	SymMat<_Scalar> m3(m1.order);

	for(int i=0;i<(m1.mat).size();i++)
	{
		m3.mat[i]=m1.mat[i]-m2.mat[i];
	}
	return m3;

}


//Addition function between one matrix belonging to SymMat class(First argument) and another(Second argument) to Eigen::Matrix
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> sub(SymMat<_Scalar>& m1,Eigen::Matrix<_Scalar,_Rows,_Cols>& m2)
{
	assert(m1.order == m2.rows() && m1.order == m2.cols());       //Condition for matrices to be conformable for addition
	Eigen::Matrix<_Scalar,_Rows,_Cols> m3;
	for(int i=0;i<m1.order;i++)
	{
		for(int j=0;j<m1.order;j++)
		{
			m3(i,j)=m1.mat[m1.index(i,j)]-m2(i,j);
		}
	}
	return m3;
}


//Addition function between one matrix belonging to Eigen::matrix(First argument) and SymMat class(Second argument) to Eigen::Matrix
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> sub(Eigen::Matrix<_Scalar,_Rows,_Cols>& m2,SymMat<_Scalar>& m1)
{
	assert(m1.order==m2.rows() && m1.order == m2.cols());       //Condition for matrices to be conformable for addition
	Eigen::Matrix<_Scalar,_Rows,_Cols> m3;
	for(int i=0;i<m1.order;i++)
	{
		for(int j=0;j<m1.order;j++)
		{
			m3(i,j)=m2(i,j)-m1.mat[m1.index(i,j)];
		}
	}
	return m3;
}

/**********************************************************************************************************
						MULTIPLICATION
					--------------------	
There is function overloading for the following:
1)Both belongs to SymMat
2)First belongs to SymMat and other to Eigen::Matrix
3)First belongs to Eigen::Matrix and other to SymMat
************************************************************************************************************/

//Multiplication function for two matrices belonging to SymMat class
/*
Only for this function the template parameters are passed from the main funciton since both the function parameters are 
matrices belonging to SymMat class, so they don't contain rows and columns.
This will be corrected once this class inherits from Eigen::Matrix or is changed fully to match the Eigen library
*/
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> mult(SymMat<_Scalar>& m1,SymMat<_Scalar>& m2)
{	
	try
	{
		if(m1.order != m2.order)
		{
		  throw 'f';
		}

	}
	catch(char& check)
	{
		std::cout<< "Matrices are not compatible for multiplication!\nTerminating the program..."<< std::endl;
		exit(0);
	}

	//Multiplication
	Eigen::Matrix<_Scalar,_Rows,_Cols> m3;
	int i,j,k;
	for(i=0;i<m2.order;i++)
	{
		for(j=0;j<m2.order;j++)
		{
			m3(i,j)=0;
			for(k=0;k<m2.order;k++)
			{
				m3(i,j)+=m1.mat[m1.index(i,k)]*m2.mat[m2.index(k,j)];
			}
		}
	}
	return m3;
}


//Multiplication function between one matrix of SymMat class(first parameter) and another from Eigen::Matrix class(second parameter)
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> mult(SymMat<_Scalar>& m1,Eigen::Matrix<_Scalar,_Rows,_Cols>& m2)
{

	try
	{
		if(m1.order != m2.rows())
		{
		  throw 'f';
		}

	}
	catch(char& check)
	{
		std::cout<< "Matrices are not compatible for multiplication!\nTerminating the program..."<< std::endl;
		exit(0);
	}

	//Multiplication
	Eigen::Matrix<_Scalar,_Rows,_Cols> m3;
	int i,j,k;
	for(i=0;i<m1.order;i++)
	{
		for(j=0;j<m2.cols();j++)
		{
			m3(i,j)=0;
			for(k=0;k<m1.order;k++)
			{
				m3(i,j)+=m1.mat[m1.index(i,k)]*m2(k,j);
			}
		}
	}
	return m3;
}


//Multiplication function between one matrix of Eigen::Matrix class(first parameter) and another from SymMat class(second parameter)
template<typename _Scalar,int _Rows, int _Cols>
Eigen::Matrix<_Scalar,_Rows,_Cols> mult(Eigen::Matrix<_Scalar,_Rows,_Cols>& m2,SymMat<_Scalar>& m1)
{
	try
	{
		if(m1.order != m2.cols())
		{
		  throw 'f';
		}

	}
	catch(char& check)
	{
		std::cout<<"Matrices are not compatible for multiplication!\nTerminating the program..."<< std::endl;
		exit(0);
	}

	//Multiplication
	Eigen::Matrix<_Scalar,_Cols,_Rows> m3;
	int i,j,k;
	for(i=0;i<m2.rows();i++)
	{
		for(j=0;j<m1.order;j++)
		{
			m3(i,j)=0;
			for(k=0;k<m1.order;k++)
			{
				m3(i,j)+=m2(i,k)*m1.mat[m1.index(k,j)];
			}
		}
	}
	return m3;
}


//------------------------------------------------------------------------------------------------
#endif //SYMMAT_H
/*************************************************************************************************
								SYMMAT HEADER FILE ENDED
**************************************************************************************************/		