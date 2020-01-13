#include<iostream>
#include<vector>
// #include<utility>/
#include<algorithm>


int main()
{

    std::vector<int> vec = {1,4,7,2};
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> parka = std::make_pair(vec.begin(), vec.end());

    std::sort(*parka);


    return 0;
}