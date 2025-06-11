#include <iostream>

void calcFibonacci(int n)
{
    //1+1 = 2
    //2+1 = 3
    //3+2 = 5
    //5+3 = 8
    //8+5 = 13

    int x = 1, y = 1, z;
    
    for (size_t i = 0; i < n; i++)
    {
        z = x + y;
        std::cout << z << "\n";
        y = x;
        x = z;
    }    
}


int main()
{
    std::cout << "--------------\n";

    calcFibonacci(10);

}
