/*************************************/
/*		File:lsh.cpp                 */
/*                                   */
/* Definition of  lsh classes'       */
/*     member functions              */
/*                                   */
/*************************************/

#include "lsh.h"
#include <cmath>
#include <vector>
#include <utility>
#include <limits>
#include <iterator>
#include <set>
#include <string>


using namespace std;



/*********************************************************
Find true nearest neighbor of query using exhaustive search.
Return a pair of the nn and the distance from it.
*********************************************************/
pair<data_point&,double> LSH::trueNN(data_point& query){
	int nn_index=0;
	double nn_dist=distance(query.p, train_data[0].p);
	for(int i=1; i<train_data_sz; i++){
		double dist = distance(query.p, train_data[i].p );
		if (dist < nn_dist){
			nn_dist = dist ;
			nn_index = i ;
		}
	}
	pair<data_point&,double> result(train_data[nn_index],nn_dist);
	return result;
}


/*************************************************************************************************************************************/

/*******************************************************
Constructor for eucledian LSH.
Initilialize hashtables vector with size L and each of
the unordered multimaps with size n.
********************************************************/
LSH_eucledian::LSH_eucledian(int k,int L,int d,int n,vector<data_point>& td) : LSH(n,td),H(k,L,d),hashtables(L,unordered_multimap<string,data_point&>(n)){
	/*set max load factor to prevent rehashing*/
	for (int l=0; l<L; l++){
		hashtables[l].max_load_factor(1.1);
	}
	/*insert each train data point to all L hashtables*/
	for (int l=0; l<L; l++){
		for (int i=0; i<train_data_sz; i++){
			/*get g vector and convert it to string to use it as hash key*/
			vector<int> g=H.g( train_data[i].p , l );
			string key = convert_key(g);
			/*insert the key-value pair to the l hashtable*/
			pair<string,data_point&> elem(key,train_data[i]);
			hashtables[l].insert( elem );
		}
	} 
}



/**********************************************************************
Find approximate nearest neighbor of query using the eucledian lsh.
For every hashtable find  the corresponding g() of query,
convert it to string and find datapoints with the same key(same g).
Return a pair of the ann and the distance from it.
Also find all neighbors with range r and put them in vector within_range.
Its obvious that if range==0 this vector is empty and the function only
returns the ann.
***********************************************************************/
pair<data_point&,double> LSH_eucledian::approximateNN(data_point& query,double r,vector<data_point*>& within_range){
	data_point* ann=NULL;
	double nn_dist=numeric_limits<double>::max();
	/*set of already cheched neighbors so not to recalculate distance if found in other hash tables*/
	set<string> already_checked;
	for (int l=0; l<hashtables.size(); l++){
		vector<int> g=H.g( query.p , l );
		string query_key=convert_key(g);
		auto iter = hashtables[l].equal_range(query_key);
		//cout << std::distance(iter.first,iter.second) << "\n";
		for (auto i=iter.first; i!=iter.second; ++i){
			string neighbor_id = (i->second).id;
			/*ignore this neighbor if his dist is already calculated from another hashtable*/
			if ( already_checked.find( neighbor_id ) != already_checked.end() ) {
				continue;
			}
			already_checked.insert(neighbor_id);
			double dist = distance(query.p, (i->second).p);
			if (dist < r){
				within_range.push_back( &(i->second) );
			} 
			if ( dist < nn_dist){
				nn_dist = dist;
				ann = &(i->second);
			}
		}
	}
	//cout << already_checked.size() << "\n\n";
	pair<data_point&,double> result(*ann,nn_dist);
	return result;
}



/******************************************************
Convert the vector of ints that g() returns to a string
in order to use it as key.
*******************************************************/
string LSH_eucledian::convert_key(vector<int>& g){
	string key;
	for (int i=0; i<g.size(); i++){
		key += to_string(g[i]);
	}
	return key;
}



/****************************************
Eucledian distance between two vectors
****************************************/
double LSH_eucledian:: distance( vector<double>& a , vector<double>& b){
	double dist=0;
	int dim=a.size();
	for (int i=0; i<dim; i++){
		dist+= pow(b[i]-a[i],2);
	}
	if(dist==0){
		return 0;
	}
	else{
		return sqrt(dist);
	}
} 



/*************************************************************************************/




/*******************************************************
Constructor for cosine LSH.
Initilialize hashtables vector with size L and each of
the hashtables with size 2^k.
Hash function used is identity.
********************************************************/
LSH_cosine::LSH_cosine(int k , int L, int d, int n ,std::vector<data_point>& td):LSH(n,td),H(k,L,d),hashtables(L,vector< vector<data_point*> >(pow(2,k))){
	/*insert each train data point to all L hashtables*/
	for (int l=0; l<L; l++){
		for (int i=0; i<train_data_sz; i++){
			/*get g vector and convert it to int to use it as hash key*/
			vector<int> g=H.g( train_data[i].p , l );
			int key = convert_key(g);
			/*insert the key-value pair to the l hashtable at bucket with number key*/
			hashtables[l][key].push_back(&train_data[i]);
		}	
	}	
}



/**********************************************************************
Find approximate nearest neighbor of query using the cosine lsh.
For every hashtable find  the corresponding g() of query which is a bitstring,
convert it to int which represents the bucket number that similar vectors are in,  
and calculate distance from those vectors.
Return a pair of the ann and the distance from it.
Also find all neighbors with range r and put them in vector within_range.
Its obvious that if range==0 this vector is empty and the function only
returns the ann.
***********************************************************************/
pair<data_point&,double> LSH_cosine::approximateNN(data_point& query,double r,std::vector<data_point*>& within_range){
	data_point* ann=NULL;
	double nn_dist=numeric_limits<double>::max();
	/*set of already cheched neighbors so not to recalculate distance if found in other hash tables*/
	set<string> already_checked;
	for (int l=0; l<hashtables.size(); l++){
		vector<int> g=H.g( query.p , l );
		int query_key=convert_key(g);
		for (auto i=hashtables[l][query_key].begin(); i!=hashtables[l][query_key].end(); ++i){
			string neighbor_id = (*i)->id;
			/*ignore this neighbor if his dist is already calculated from another hashtable*/
			if ( already_checked.find( neighbor_id ) != already_checked.end() ) {
				continue;
			}
			already_checked.insert(neighbor_id);
			double dist = distance(query.p, (*i)->p);
			if (dist < r){
				within_range.push_back( *i );
			} 
			if ( dist < nn_dist){
				nn_dist = dist;
				ann = *i;
			}
		}
	}
	pair<data_point&,double> result(*ann,nn_dist);
	return result;
}



/******************************************************
Convert the vector of ints that g() returns to a 
bitstring and then translate this bitstring to
the corresponding integer value
*******************************************************/
int LSH_cosine::convert_key(vector<int>& g){
	string key;
	for (int i=0; i<g.size(); i++){
		key += to_string(g[i]);
	}
	//cout << key +":"<< stoi(key,NULL,2) <<"\n";
	return stoi(key,NULL,2);
}



/****************************************
Cosine distance between two vectors
****************************************/
double LSH_cosine:: distance(vector<double>& a , vector<double>& b){
	double inner_prod = inner_product(a,b);
	int dim=a.size();
	double tmp1=0,tmp2=0;
	for (int i=0; i<dim; i++){
		tmp1 += pow(a[i],2);
		tmp2 += pow(b[i],2);
	}
	return 1 - ( inner_prod/( (sqrt(tmp1)) * (sqrt(tmp2)) ) );
}