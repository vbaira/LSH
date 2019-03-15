# LSH
Implementation of the LSH Nearest Neighbor and Range Search algorithm using Eucledian metric and Cosine similarity.  
Program can easily be extended to support more vector metrics (e.g p-norm) simply by extending LSH and H_family abstract classes.  


## Compilation
~~~
make -f lsh_makefile
~~~

## Execution
To execute the program :
~~~
./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>
~~~
k : number of lsh functions  (default k=4)  
L : number of hash tables    (default L=5)  
- Execution can also be performed without all or some of the arguments.In that case  the missing argument either takes a default value (k,L) or the user is prompted to enter the value at runtime (input,output and query files).  
- In order to perform Range Search , radius must be specified in the first line of the query file ( *Radius : r* ) otherwise only Nearest Neighbor Search is performed.
