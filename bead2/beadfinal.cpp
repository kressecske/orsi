#include <fstream>
#include <future> // ASYNC, FUTURE
#include <string>
#include <vector>
#include <numeric>
#include <iostream> //time,debug
#include <chrono>
#include <ctime>
#include <algorithm>

typedef std::vector<std::string>::iterator vit;

std::vector<std::string> merge(const vit begin, const vit mid, const vit end)
{
	//TODO: WITH ITER_SWAP?
    std::vector<std::string> v;
    vit it_l{ begin }, it_r{ mid };
    const vit it_mid{ mid }, it_end{ end };

    while (it_l != it_mid && it_r != it_end)
    {
    	v.push_back((*it_l <= *it_r) ? *it_l++ : *it_r++);
    }

    v.insert(v.end(), it_l, it_mid);
    v.insert(v.end(), it_r, it_end);

    return v;
}

void bubble_sort(const vit begin, const vit end)
{
	for(vit i = begin; i!=end;++i)
	{
		for(vit j = begin; j!=end-1;++j)
		{
			vit next = std::next(j);
			if((*j) > (*next))
			{
				iter_swap(j,next);
			}
		}
	}
}


void merge_sort(vit begin, vit end)
{
    if (end - begin > 300) {
        vit middle = begin + (end - begin) / 2;
 		auto first_half = std::async(std::launch::async, merge_sort, begin, middle);
	 	//std::async(std::launch::async, merge_sort, middle, end);
    	merge_sort(middle,end);
    	first_half.wait();
    	std::vector<std::string> v = merge(begin, middle, end);
    	//merge(begin,middle,end);
	    std::move(v.cbegin(), v.cend(), begin);
    }else{
			bubble_sort(begin,end);
	}
}

int main()
{
	std::ifstream input("input.txt");

	unsigned int N;

	input >> N;

	std::vector<std::string> data;

	//Read
	for (int i = 0; i<N; ++i)
	{
		std::string _neptun;
		input >> _neptun;
		data.push_back(_neptun);
	}
	//Read end

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

 	merge_sort(data.begin(),data.end());

 	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout <<"Time taken: " << elapsed_seconds.count()<< "s\n";


	std::ofstream output("output.txt");
	for (auto& neptun : data)
	{
		output << neptun << std::endl;
	}
	output.close();
	return 0;
}
