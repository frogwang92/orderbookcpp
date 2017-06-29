//
// Created by yi wang on 6/28/2017.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "OrderBookMgmt.hpp"

using namespace std;
using namespace OrderBook;

static OrderBookMgmt mgmt;

TEST_CASE("1_bid1", "[bid][1st bid]") {
    mgmt.bid(100, 1);
    auto book = mgmt.take_snap_shot();
    auto iter = std::get<0>(book).begin();
    REQUIRE(iter->first == 100);
    REQUIRE(iter->second == 1);
}

TEST_CASE("1_bid2", "[bid][2nd bid]") {
    mgmt.bid(100, 1);
    mgmt.bid(200, 2);
    auto book = mgmt.take_snap_shot();
    auto iter = std::get<0>(book).begin();
    REQUIRE(iter->first == 200);
    REQUIRE(iter->second == 2);
    ++iter;
    REQUIRE(iter->first == 100);
    REQUIRE(iter->second == 2);
}

TEST_CASE("2_ask1", "[ask][1st ask]") {
    mgmt.ask(300, 1);
    auto book = mgmt.take_snap_shot();
    auto iter = std::get<1>(book).begin();
    REQUIRE(iter->first == 300);
    REQUIRE(iter->second == 1);
}

TEST_CASE("2_ask2", "[ask][2nd ask]") {
    mgmt.ask(300, 1);
    mgmt.ask(400, 2);
    auto book = mgmt.take_snap_shot();
    auto iter = std::get<1>(book).begin();
    REQUIRE(iter->first == 300);
    REQUIRE(iter->second == 2);
    ++iter;
    REQUIRE(iter->first == 400);
    REQUIRE(iter->second == 2);
}

TEST_CASE("2_ask3", "[ask][ask and a order match]") {
    mgmt.ask(200, 1);
    auto book = mgmt.take_snap_shot();
    auto iter = std::get<0>(book).begin();
    REQUIRE(iter->first == 200);
    REQUIRE(iter->second == 1);
}

TEST_CASE("2_ask4", "[ask][ask and a order match, gap changed]") {
    mgmt.ask(100, 1);
    auto book = mgmt.take_snap_shot();
    auto iter = std::get<0>(book).begin();
    REQUIRE(iter->first == 100);
    REQUIRE(iter->second == 2);
}

TEST_CASE("2_ask5", "[ask][ask and a order match, clears bids]") {
    mgmt.bid(200, 2);
    mgmt.ask(100, 10);
    auto book = mgmt.take_snap_shot();
    REQUIRE(std::get<0>(book).empty());
    auto iter = std::get<1>(book).begin();
    REQUIRE(iter->first == 100);
    REQUIRE(iter->second == 6);
}

TEST_CASE("3_cancel1", "[cancel][cancel ask]") {
    mgmt.ask(100, 1, true);
    REQUIRE(std::get<0>(mgmt.query(100)) == 5);
    mgmt.ask(100, 5, true);
    REQUIRE(std::get<0>(mgmt.query(100)) == OrderBook::NaN);
}

TEST_CASE("4_bid3", "[bid][bid and a order match]") {
    mgmt.bid(400, 1);
    REQUIRE(std::get<0>(mgmt.query(300)) == 1);
    mgmt.bid(300, 1);
    REQUIRE(std::get<0>(mgmt.query(300)) == 0);
}

TEST_CASE("4_bid4", "[bid][bid and a order match, clears asks]") {
    mgmt.bid(500, 10);
    REQUIRE(std::get<0>(mgmt.query(500)) == 8);
}