#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <iostream> //time,debug
#include <chrono>
#include <algorithm>
#include <utility>
#include <cassert>

#include "pipeline.hpp"

typedef std::vector<int> Vector;
typedef std::vector<Vector> Matrix;

Vector multiplication(const Matrix& m, Vector& v)
{
	Vector result;
	for (int i=0;i<4;i++){
		result.push_back(0);
    	for (int j=0;j<4;j++){
    		result[i] += m[i][j]*v[j];
        }
	}
	return result;
}

void Operation (Matrix m, Pipeline<Vector>& from, Pipeline<Vector>& to,int size) {
	for(int i=0;i<size;++i){
		Vector v = from.pop();
		to.push(multiplication(m,v));
	}
}

int main()
{
	std::vector<Matrix> operations;
	Pipeline<Vector> points;

	std::ifstream input_matrices("0/input_matrices.txt");
	std::ifstream input_points("0/input_points.txt");

	unsigned int M;
	unsigned int N;

	input_matrices >> M;
	//READ OPERATIONS
	for (int i=0;i<M;++i) {
		Matrix op;
		for (int row=0;row<4;++row) {
			Vector r;
			for(int col=0;col<4;++col){
				int v;
				input_matrices >> v;
				r.push_back(v);
			}
			op.push_back(r);
		}
		operations.push_back(op);
	}

	input_points >> N;

	std::vector<std::thread> threads;
	std::vector<Pipeline<Vector>> pipelines(M+1);
	//start threads
	for (int i=0;i<M;++i)
	{
		threads.push_back(std::thread(Operation, operations[i], std::ref(pipelines[i]), std::ref(pipelines[i+1]),N));
	}


	//Read
	
	//READ POINTS
	for (int i = 0; i<N; ++i)
	{
		Vector point;
		for (int j=0;j<3;++j) 
		{
			int value;
			input_points >> value;
			point.push_back(value);
		}
		point.push_back(1);
		pipelines[0].push(point);
	}
	//Read end

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for(auto& t : threads)
    {
   		t.join();
    }
    

    std::vector<Vector> results;

	for(int i=0;i<N;++i){
		Vector v = pipelines[pipelines.size()-1].pop();
		results.push_back(v);
	}


 	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout <<"Time taken: " << elapsed_seconds.count()<< "s\n";


	std::ofstream output("output.txt");
	for(auto& v : results)
	{
		for(int i=0;i<3;++i)
		{
			output << v[i] << " ";
		}
		output<<std::endl;
	}
	output.close();
	return 0;
}