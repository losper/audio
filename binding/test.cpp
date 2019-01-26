#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include "plugin.h"
uint32_t toHash(std::string& str)
{
    //BKDRHash
    uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
    uint32_t hash = 0;

    for(auto it:str)
    {
        hash = hash * seed + it;
    }
    return (hash & 0x7FFFFFFF);
}
uint32_t simHash(std::vector<std::pair<uint32_t,int>> feature) {
    int v[32];
    memset(v, 0, sizeof(int)*32);
    for (auto it:feature) {
        for (size_t j = 0; j < 32; j++) {
            v[j] += (it.first & (1 << j)) ? it.second : -it.second;
        }
    }
    uint32_t hash = 0;
    for (size_t i = 0; i < 32; i++) {
        if (v[i] > 0) {
            hash |= (1 << i);
        }
    }
    return hash;
}
uint32_t hanming_distance(uint32_t c1,uint32_t c2) {
    uint32_t v=c1^c2,dist=0;
    while (v){
        v=v & (v-1);
        dist++;
    }
    return dist;
}
int testgg(int x,std::shared_ptr<std::vector<int>> tmp) {
    std::cout << tmp << std::endl;
    return x;
}

int main() {
    //bind(std::bind(testgg, 122, 23));
    pa_plugin g;
    bind_io(g,std::bind(
        testgg,
        23,
        std::make_shared<std::vector<int>>(20)));
    
    /*std::string x="ABCDEFGHIJKLVVABCDEFGHIJKLVVABCDEFGHIJKLVVABCDEFGHIJKLVVVVVVVVVVVVVV";
    std::string tmp="ABCDEFGHIJKLABCDEFGHIJKLABCDEFGHIJKLABCDEFGHIJKLABCDEFGHIJKL";
    std::string v;
    std::vector<std::pair<uint32_t, int>> list;
    uint32_t c1, c2;
    v="123";
    for (size_t i = 0; i < tmp.size(); )
    {
        list.push_back(std::make_pair(toHash(tmp.substr(i,2)), 1));
        i += 2;
    }
    c1=simHash(list);
    list.clear();
    for (size_t i = 0; i < x.size(); )
    {
        list.push_back(std::make_pair(toHash(x.substr(i, 2)), 1));
        i += 2;
    }
    c2 = simHash(list);
    std::cout << hanming_distance(c1,c2) << std::endl;*/
    
    
	return 0;
}