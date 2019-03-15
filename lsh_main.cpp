#include "utility.h"
#include "lsh.h"
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <chrono>
#include <fstream>

using namespace std;


int main(int argc,char* argv[]){
	/**parse command line options**/
	string input_file,query_file,output_file;
	int k=4,L=5;
	Utility::parse_lsh_options(argc , argv ,input_file , query_file , output_file , k ,	L);
	//cout << input_file << " "+query_file << " "+output_file << " " << k << " " << L << "\n";

	/**determine type of train data set and read it**/
	string metric = "eucledian";
	int dimension,train_set_sz;
	vector<data_point> train_set;
	Utility::read_input_file(input_file,'d',&metric,&dimension,NULL,train_set);
	train_set_sz=train_set.size();
	//cout << metric+" " << dimension <<" "<< k <<" "<< L << "\n";

	/**create the corresponding lsh object**/
	LSH* lsh;
	if (metric=="eucledian"){
		lsh=new LSH_eucledian(k,L,dimension,train_set_sz,train_set);	
	}
	else{
		lsh=new LSH_cosine(k,L,dimension,train_set_sz,train_set) ;
	}
	
	/**open output filestream**/
	ofstream output(output_file);
	if ( ! output.is_open() ){
		cerr << "Unable to open output file\n";
		exit(EXIT_FAILURE);
	}
	/**read query data sets and print results to output**/
	char answer;
	do{
		double r=0;
		double max_af=0; 	//max approximation factor
		double ann_mtime=0; //ann search mean time
		vector<data_point> test_set;
		Utility::read_input_file(query_file,'q',NULL,NULL,&r,test_set);
		/**for each query point find true and approximate nn and measure time**/
		for (int i=0; i<test_set.size(); i++){
			/**true nearest neighbor**/
			auto start = chrono::high_resolution_clock::now();
			pair<data_point&,double> t_nn = lsh->trueNN(test_set[i]);
			auto stop	= chrono::high_resolution_clock::now();
			chrono::duration<double,milli> t_true = stop - start;

			/**approximate nearest neighbor**/
			vector<data_point*> within_range;
			start = chrono::high_resolution_clock::now();
			pair<data_point&,double> a_nn = lsh->approximateNN(test_set[i],r,within_range);
			stop	= chrono::high_resolution_clock::now();
			chrono::duration<double,milli> t_lsh = stop - start;
			ann_mtime += t_lsh.count();

			/**calculate aproximation factor**/
			double af=(a_nn.second/t_nn.second);
			if ( af > max_af ){
				max_af = af;
			}

			/**write results to output**/
			output << "Query: "+test_set[i].id+"\n";
			output << "R-near neighbors: \n";
			for(int j=0; j<within_range.size(); j++){
				output << within_range[j]->id;
			}
			output << "Nearest neighbor: " << a_nn.first.id <<"\n";
			output << "distanceLSH: " << a_nn.second << "\n";
			output << "distanceTrue: "<< t_nn.second << "\n";
			output << "tLSH: " << t_lsh.count() << " ms\n";
			output << "tTrue: "<< t_true.count() << " ms\n\n";
			
		}
		
		ann_mtime = ann_mtime/test_set.size();
		output << "\nMax approximation factor : " << max_af << "\n";
		output << "Mean approximate nearest neighbor search time : " << ann_mtime << " ms\n\n";
		
		cout << "\nMax approximation factor : " << max_af << "\n";
		cout << "Mean approximate nearest neighbor search time : " << ann_mtime << " ms\n\n";

		cout << "Do you want to insert a new test_set?(y/n)\n";
		cin  >> answer;
		if (answer == 'y'){
			cout << "Please enter path for query file\n";
			cin  >> query_file;
		}
	}while(answer=='y');

	/**close output file stream**/
	delete lsh;
	output.close();
	
}

