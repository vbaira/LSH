/*************************************/
/*		File:utility.cpp             */
/*                                   */
/* Definition of  utility class      */
/*     member functions              */
/*                                   */
/*************************************/

#include "utility.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <vector> 
#include <stdio.h> 
#include <iterator> 
#include <fstream>
#include <sstream>
#include <regex>


using namespace std;


/*Parse command line options of lsh program*/
void Utility::parse_lsh_options(int argc , char* argv[],string& in,string& qu,string& out,int& k ,int& L){
	int opt;
	string s="USAGE:$./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>\n";
	if (argc>11 || (argc%2 != 1) ){
		cerr << "Wrong number of arguments : "+s ;
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "d:q:k:L:o:")) != -1) { 
		switch(opt){
			case 'd':
				in = optarg;
				break;
			case 'q':
				qu=optarg;
				break;
			case 'k':
				k = atoi(optarg);
				break;			
			case 'L':
				L = atoi(optarg);
				break;
			case 'o':
				out= optarg; 
				break;
			default:
				cerr << s ;
				exit(EXIT_FAILURE);
		}
	}
	if( k <= 0 || L <= 0){
		cerr << "Arguments k and L should be >0 : "+s ;
		exit(EXIT_FAILURE);
	}
	if ( in.empty() ){
		cout << "Please enter path for input file\n";
		cin >> in; 
	}
	if ( qu.empty() ){
		cout << "Please enter path for query file\n";
		cin >> qu;
	}
	if ( out.empty() ){
		cout << "Please enter path for output file\n";
		cin >> out;
	}
}



/*Parse command line options of binary cube program*/
void Utility::parse_cube_options(int argc , char* argv[],string& in,string& qu,string& out,int& k ,int& M ,int& probes){
	int opt;
	string s="USAGE:$./cube –d <input file> –q <query file> –k <int> -M <int> -p <int> -ο <output file>\n";
	if (argc>13 || (argc%2 != 1) ){
		cerr << "Wrong number of arguments : "+s ;
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "d:q:k:M:o:p:")) != -1) { 
		switch(opt){
			case 'd':
				in = optarg;
				break;
			case 'q':
				qu=optarg;
				break;
			case 'k':
				k = atoi(optarg);
				break;			
			case 'M':
				M = atoi(optarg);
				break;
			case 'o':
				out= optarg; 
				break;
			case 'p':
				probes = atoi(optarg);
				break;
			default:
				cerr << s ;
				exit(EXIT_FAILURE);
		}
	}
	if( k <= 0 || M <= 0 || probes < 0){
		cerr << "Arguments k and M and p should be >0 : "+s ;
		exit(EXIT_FAILURE);
	}
	if ( in.empty() ){
		cout << "Please enter path for input file\n";
		cin >> in; 
	}
	if ( qu.empty() ){
		cout << "Please enter path for query file\n";
		cin >> qu;
	}
	if ( out.empty() ){
		cout << "Please enter path for output file\n";
		cin >> out;
	}
}



/*Function to read vectors from file stream and place them in  list */
void fill_vector_list(ifstream& fs,vector<data_point>& l,string& buf){
	/*tokenize input vectors by using a regular expreassion iterator*/
	regex delim("[^\\s]+");
	sregex_iterator start(buf.begin(),buf.end(),delim);
	sregex_iterator end ;
	/*insert the first vector*/
	data_point dp;
	for (sregex_iterator i=start; i!=end; ++i){
		if (i==start){
			dp.id=(*i).str();
		}
		else{
			dp.p.push_back(stod((*i).str()));		
		}
	}
	l.push_back(dp);
	/*insert the rest of vectors*/
	while( getline(fs,buf) ){
		if (buf.empty()){
			continue;
		}
		data_point dp;
		start=sregex_iterator(buf.begin(),buf.end(),delim);
		end = sregex_iterator();
		for (sregex_iterator i=start; i!=end; ++i){
			if (i==start){
				dp.id=(*i).str();
			}
			else{
				dp.p.push_back(stod((*i).str()));		
			}
		}
		l.push_back(dp);
	}

}

/*determine input  metric/dimension and fill vector lists*/
void Utility::read_input_file(string& in,char type,string* metric,int* dim,double* radius,vector<data_point>& l){
	ifstream fs;
	fs.open(in.c_str());
	if (fs.is_open()){
		string buf;
		/*go to first non empty line*/
		getline(fs,buf);
		while (buf.empty()){
			getline(fs,buf);
		}
		/*check if data file or query file*/
		if (type=='d'){
			if ( buf=="cosine" || buf =="eucledian"){
				*metric=buf;
				/*go to the next non empty line and fill vector list*/
				getline(fs,buf);
				while (buf.empty()){
					getline(fs,buf);
				}
				fill_vector_list(fs,l,buf);
			}
			else{
				fill_vector_list(fs,l,buf);
			}
			*dim=l.front().p.size();
		}
		else{
			if  ((buf.find("Radius") != string::npos) || (buf.find("radius") != string::npos) ){
				/*get radius value*/
				stringstream ss(buf);
				string temp;
				getline(ss,temp,':');
				getline(ss,temp,':');
				*radius = stod(temp);
				/*go to next non empty line and fill vector list*/
				getline(fs,buf);
				while (buf.empty()){
					getline(fs,buf);
				}
				fill_vector_list(fs,l,buf);
			}
			else{
				fill_vector_list(fs,l,buf);
			}
		}
		fs.close();
	}
	else{
		cerr << "Unable to open input file\n";
		exit(EXIT_FAILURE);
	}
}














