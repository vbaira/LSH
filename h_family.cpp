/*************************************/
/*		File:h_family.cpp            */
/*                                   */
/* Definition of  h_family classes'  */
/*     member functions              */
/*                                   */
/*************************************/

#include "h_family.h"
#include <random>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>


#define W 350

using namespace std;


/*******************************************************
Constructor for eucledian h_family.
Initialize k,L,d and create random vector v and t for each h struct.
This is done L times,once for each hash table.
Vectors are created using standard normal distribution and t from uniform distribution.
********************************************************/
H_family_eucledian::H_family_eucledian(int k,int L,int d):H_family(k,L,d){
	/*create the statistical dist. needed*/
	random_device rd; 			//random device to generate random seed
	mt19937 generator( rd() );	//random generator
	normal_distribution<float> std_normal_distr(0,1);		// strandard distr for vectors v
	uniform_real_distribution<float> uniform_distr(0,W); 	//uniform distr for in range[0,W) 
	for (int i=0; i<L; i++){
		bool is_equal=false;
		vector<h> temp(k);		//keep k structs h(vector v and t)
		do{
			for (int j=0; j<k; j++){
				struct h h_i;
				h_i.t=uniform_distr(generator);
				/*create random vector v of d dimension*/
				for (int dim=0; dim<d; dim++){
					h_i.v.push_back( std_normal_distr(generator) ); 
				}
				temp[j]=h_i;
			}

			/*ensure h1,h2..hk is different from h1..hk of other hash tables*/
			is_equal=false;
			for (int l=0; l<i; l++){
				if (temp == hashes[l]) is_equal=true;
			}
		}while(is_equal==true);

		/*its different so push it to hashes vector*/
		hashes.push_back(temp);	
	}
}



/**********************************************
Calculate hash function h_i of hash table l for input vector p.
***********************************************/
int H_family_eucledian:: h_i(vector<double>& p ,int l, int i){
	vector<h>& h = hashes[l];
	return floor( ( inner_product(p,h[i].v) + h[i].t ) / W );
}



/********************************************************
Calculate amplified hash function g(p) for hash table l
**********************************************************/
vector<int> H_family_eucledian:: g(vector<double>& p ,int l){
	vector<int> result(k);
	for (int i=0; i<k; i++){
		result[i]=( this->h_i(p,l,i) );
	}
	return result;
}



/*************************************************
Print function
***************************************************/
void H_family_eucledian:: print(){
	cout<<"k:"<<k<<" L:"<<L<<" d:"<<d<<"\n";
	for (int i=0; i<L; i++){
		cout<< "---HASH TABLE "<<i<<"---\n";
		for (int j=0; j<k; j++){
			hashes[i][j].print();
		}
		cout<<"\n";
	}
}



/*******************************************************
Constructor for eucledian h_family.
Initialize k,L,d and create random vector r for each h struct.
This is done L times,once for each hash table.
Vectors are created using standard normal distribution.
********************************************************/
H_family_cosine::H_family_cosine(int k,int L,int d):H_family(k,L,d){
	/*create the statistical dist. needed*/
	random_device rd; 			//random device to generate random seed
	mt19937 generator( rd() );	//random generator 
	normal_distribution<float> std_normal_distr(0,1);	// strandard distr. for vectors r
	for (int i=0; i<L; i++){
		bool is_equal=false;
		vector<h> temp(k);		//keep k structs h(vector r)
		do{
			for (int j=0; j<k; j++){
				struct h h_i;
				/*create random vector r of d dimension*/
				for (int dim=0; dim<d; dim++){
					h_i.r.push_back( std_normal_distr(generator) ); 
				}
				temp[j]=h_i;
			}

			/*ensure h1,h2..hk is different from h1..hk of other hash tables*/
			is_equal=false;
			for (int l=0; l<i; l++){
				if (temp == hashes[l]) is_equal=true;
			}
		}while(is_equal==true);

		/*its different so push it to hashes vector*/
		hashes.push_back(temp);	
	}	
}



/**********************************************
Calculate hash function h_i of hash table l for input vector p.
***********************************************/
int H_family_cosine:: h_i(vector<double>& p ,int l, int i){
	vector<h>& h = hashes[l];
	if ( inner_product(p,h[i].r) >= 0 ){
		return 1;
	}
	else{
		return 0;
	}
}



/********************************************************
Calculate amplified hash function g(p) for hash table l
**********************************************************/
vector<int> H_family_cosine:: g(vector<double>& p ,int l){
	vector<int> result(k);
	for (int i=0; i<k; i++){
		result[i] = ( this->h_i(p,l,i) );
	}
	return result;
}



/*************************************************
Print function
***************************************************/
void H_family_cosine:: print(){
	cout<<"k:"<<k<<" L:"<<L<<" d:"<<d<<"\n";
	for (int i=0; i<L; i++){
		cout<< "---HASH TABLE "<<i<<"---\n";
		for (int j=0; j<k; j++){
			hashes[i][j].print();
		}
		cout<<"\n";
	}
}