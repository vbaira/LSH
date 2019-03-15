/****************************************************************/
/*       File:lsh.h                                             */
/*                                                              */
/*  LSH classes act as wrappers for the                         */
/*    corresponding H_family of functions.                      */
/*    They also contain the needed data structure(hashtable)    */
/*    and implement the methods for searching for               */
/*    the nearest neighbor(true,approximate or range)           */     
/*                                                              */
/****************************************************************/

#ifndef LSH_H
#define LSH_H

#include "utility.h"
#include "h_family.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <utility>



/********************************************
Abstract class of LSH.Each type of LSH inherits 
this class and must implement its pure virtual 
functions
*********************************************/
class LSH{
protected:
	int train_data_sz;					//size of train data
	std::vector<data_point>& train_data;//reference to train data

public:
	LSH(int n,std::vector<data_point>& td) : train_data_sz(n),train_data(td){}
	virtual ~LSH(){}
	virtual double distance(std::vector<double>& a,std::vector<double>& b)=0;
	virtual std::pair<data_point&,double> approximateNN(data_point& query,double r,std::vector<data_point*>& within_range)=0;
	std::pair<data_point&,double> trueNN(data_point& query);
};



/*********************************************
Eucledian LSH containing a eucledian H family 
of functions and a vector of L hashtables
(unordered multimaps)
*********************************************/
class LSH_eucledian : public LSH{
private:
	std::vector< std::unordered_multimap< std::string,data_point& > > hashtables;
	H_family_eucledian H;
	std::string convert_key(std::vector<int>& g); 

public:
	LSH_eucledian(int k,int L,int d,int n,std::vector<data_point>& td);
	double distance(std::vector<double>& a,std::vector<double>& b );
	std::pair<data_point&,double> approximateNN(data_point& query,double r,std::vector<data_point*>& within_range);
};



/*********************************************
Cosine LSH containing a cosine H family 
of functions and a vector of L hashtables
(vectors of datapoints)
*********************************************/
class LSH_cosine : public LSH{
private:
	std::vector< std::vector<std::vector<data_point*> > > hashtables;
	H_family_cosine H;
	int convert_key(std::vector<int>& g);

public:
	LSH_cosine(int k , int L, int d, int n ,std::vector<data_point>& td);
	double distance(std::vector<double>& a,std::vector<double>& b );
	std::pair<data_point&,double> approximateNN(data_point& query,double r,std::vector<data_point*>& within_range);
};

#endif