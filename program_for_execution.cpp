#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    auto begin = std::chrono::high_resolution_clock::now();
    std::fstream fRes;
    fRes.open(argv[argc-1], std::ios::out | std::ios::app);
    if(fRes.is_open())
    {
        double A = strtod(argv[1], NULL), B = strtod(argv[2], NULL), steps_quantity = strtod(argv[3], NULL);
        double eps = 0.001, total{0}, holder{1};
        for(double x = A, step = (B-A)/steps_quantity; x < B; x+= step)
        {
            int sign_ind = 0;
            holder = 1;
            total = 0;
            while(fabs(holder) >= eps)
            {
                total += pow(-1, sign_ind)*holder;
                sign_ind++;
                holder = pow(x, sign_ind)*(sign_ind+1);
            }
	    fRes << x << ' ' << total << ' ' << pow(1+x, -2) << std::endl;
        }
        fRes.close();
    }else{
        std::cerr << "Error: file could not be opened" << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "Time measured for " << argv[argc-1] << ": " << elapsed.count()* 1e-9 << std::endl;    
}

