#include <iostream>
#include <memory>
#include "OrderBookMgmt.hpp"
#include <random>
#include <chrono>
using namespace std;

int main() {
    auto p_mgmt = std::make_unique<OrderBookMgmt>();

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution1(1,20000);
    std::uniform_int_distribution<int> distribution2(1,2);
    std::uniform_int_distribution<int> distribution3(1,100);
    cout << "start" << endl;
    auto start = std::chrono::system_clock::now();
    auto n = 0, times = 10000000;
    while( n < times) {
        int side_roll = distribution2(generator);
        int price_roll = distribution1(generator);
        int vol_roll = distribution3(generator);
        // cout << price_roll << "-" << vol_roll << "-"<< side_roll<< endl;
        if(side_roll == 1) {
            p_mgmt->bid(price_roll, vol_roll);
        } else {
            p_mgmt->ask(price_roll, vol_roll);
        }
        ++n;
    }
    auto finish = std::chrono::system_clock::now();
    cout << "finish, " << times << " order match, cost " << (finish - start).count() << "ns" << endl;
    return 0;
}