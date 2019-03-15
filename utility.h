/*************************************/
/*     File:utility.h                */
/*	                                 */
/*    Definition of utility class    */
/*                                   */
/*************************************/

#ifndef USAGE_H
#define USAGE_H

#include <string>
#include <vector>
#include <iostream>


/***********************
Structure to represent
input data points
************************/
struct data_point{
	std::string id;
	std::vector<double> p;
};


/********************************************
Utility class. 
Consists of static member functions for various 
utility tasks like i/o.
*********************************************/
class Utility{
public:
	static void parse_lsh_options(int argc , char* argv[],std::string& in,std::string& qu,std::string& out,int& k ,int& L);

	static void parse_cube_options(int argc , char* argv[],std::string& in,std::string& qu,std::string& out,int& k ,int& M ,int& probes);

	static void read_input_file(std::string& in,char type,std::string* metric,int* dim,double* radius,std::vector<data_point>& l);

};


/****************************************
Calculate the dot product of two vectors
*****************************************/
template <typename T,typename V>
T inner_product(std::vector<T>& a, std::vector<V>& b){ 
	if (a.size() != b.size()){
		std::cerr << "Inner product:vectors dont have the same dimension\n";
		exit(EXIT_FAILURE);
	}

    T product = 0;  
    for (int i = 0; i < a.size(); i++){
		product += (a[i] * b[i]); 
    }

    return product; 
}

#endif