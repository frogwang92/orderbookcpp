#include <iostream>
#include <memory>
#include "OrderBookMgmt.hpp"
#include <random>
using namespace std;

int main() {
    auto p_mgmt = std::make_unique<OrderBookMgmt>();

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution1(1,20000);
    std::uniform_int_distribution<int> distribution2(1,2);
    std::uniform_int_distribution<int> distribution3(1,100);
    auto n = 0;
    while( n < 100000) {
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


    return 0;
}