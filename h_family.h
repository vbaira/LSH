/**********************************************************************************/
/*     File:h_family.h                                                            */
/*	                                                                              */
/* Definition of H_family abstract class and of it's derived classes.             */
/* Each type of H_family (eucledian,cosine etc) inherits H_family and has         */
/* to implement pure virtual functions h_i() and g()                              */
/**********************************************************************************/

#ifndef HFAM_H
#define HFAM_H

#include "utility.h"
#include <string>
#include <vector>
#include <iostream>



/***************************************
Abstract class of H_family.
Each type of H_family(eucledian,cosine etc) inherits this class
****************************************/ 
class H_family{
protected:
	int k;	//number of hash functions h
	int L;	//number of hash tables
	int d;	//dimension of input and query vectors

public:
	H_family(int k,int L , int d):k(k),L(L),d(d){}
	virtual ~H_family(){}
	virtual std::vector<int> g(std::vector<double>& p ,int l)=0;
	virtual int h_i(std::vector<double>& p ,int l, int i)=0;
	virtual void print(){};
};



/*******************************
Eucledian H family of functions
********************************/
class H_family_eucledian : public H_family{
private:
	/*struct containing random vector v and random t 
	for each of the hash functions h*/
	struct h{
		std::vector<float> v;
		float t;
		
		void print(){
			std::cout<<"*********************\n";
			std::cout << t <<"\n";
			for (auto i=v.begin(); i!=v.end(); ++i ){
				std::cout<<*i<<" ";
			}
			std::cout<<"\n";
			std::cout<<"*********************\n";

		}

		/*overload == operator to check equality between h's*/
		bool operator==(const h& rv) const{
			if (t != rv.t){
				return false;
			}
			if(v != rv.v){
				return false;
			}
			return true;
		}
	};

	std::vector< std::vector<h> > hashes ;	//vector of size L.At each position contains k structs h.
public:
	H_family_eucledian(int k,int L,int d);
	std::vector<int> g(std::vector<double>& p ,int l);
	int h_i(std::vector<double>& p ,int l, int i);
	void print();
};



/****************************
Cosine H family of functions
*****************************/
class H_family_cosine : public H_family{
private:
	/*struct containing random vector r for each of the 
	hash functions h*/
	struct h{
		std::vector<float> r;

		void print(){
			std::cout<<"*********************\n";
			for (auto i=r.begin(); i!=r.end(); ++i ){
				std::cout<<*i<<" ";
			}
			std::cout<<"\n";
			std::cout<<"*********************\n";

		}

		/*overload == operator to check equality between h's*/
		bool operator==(const h& rv) const{
			if(r != rv.r){
				return false;
			}
			return true;;
		}
	};

	std::vector< std::vector<h> > hashes ; //vector of size L.At each position contains k structs h.
public:
	H_family_cosine(int k,int L,int d);
	std::vector<int> g(std::vector<double>& p ,int l);
	int h_i(std::vector<double>& p ,int l, int i);
	void print();
};




#endif