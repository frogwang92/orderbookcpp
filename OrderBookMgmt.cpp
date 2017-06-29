//
// Created by yi wang on 6/27/2017.
//

#include "OrderBookMgmt.hpp"

using namespace OrderBook;

OrderBookMgmt::OrderBookMgmt() noexcept {
    init();
}

void OrderBookMgmt::ask(PriceType price, VolType vol, bool _cancel) {
    if(_cancel) {
        if(!cancel(price, vol, order_book_ask)) {
            cancel_err_handler();
        }
    } else {
        match(price, vol, order_book_bid, order_book_ask);
    }
}

void OrderBookMgmt::bid(PriceType price, VolType vol, bool _cancel) {
    if(_cancel) {
        if(!cancel(price, vol, order_book_bid)) {
            cancel_err_handler();
        }
    } else {
        match(price, vol, order_book_ask, order_book_bid);
    }
}

std::tuple<OrderBook::VolType,OrderBookMgmt::Side> OrderBookMgmt::query(PriceType price) const {
    auto iter = order_book_ask.find(price);
    if (iter != order_book_ask.end()) {
        return std::make_tuple(iter->second, Side::ASK);
    } else {
        iter = order_book_bid.find(price);
        if (iter != order_book_bid.end()) {
            return std::make_tuple(iter->second, Side::BID);
        }
    }
    return std::make_tuple(OrderBook::NaN, Side::ASK);
}

std::tuple<ReverseOrderBookType, OrderBookType> OrderBookMgmt::take_snap_shot() const {
    return std::make_tuple(order_book_bid, order_book_ask);
}

template<typename TKey, typename TVal, typename TCompareFrom, typename TCompareTo>
void OrderBookMgmt::match(TKey price, TVal vol, std::map<TKey, TVal, TCompareFrom> &from_map, std::map<TKey, TVal, TCompareTo> &to_map) {
    TKey gap = get_gap(from_map);
    if (gap!=OrderBook::NaN && !(from_map.key_comp()(price, gap))) {
        auto iter = from_map.begin();
        auto v = vol;
        while (v > 0 && iter != from_map.end()) {
            if (v >= iter->second) {
                v -= iter->second;
                //iter->second = 0;
                notify_order_match(iter->first, iter->second);
                from_map.erase(iter++);
            } else {
                iter->second -= v;
                notify_order_match(iter->first, v);
                v = 0;
            }
        }
        if (v > 0) {   // a bid offer clear up all asks...
            to_map.insert(OrderPairType{price, v});
        }
    } else {       // price < gap_ask
        auto iter = to_map.find(price);
        if (iter != to_map.end()) {
            iter->second += vol;
        } else {
            to_map.insert(OrderPairType{price, vol});
        }
    }
}

template<typename TKey, typename TVal, typename TCompare>
bool OrderBookMgmt::cancel(TKey price, TVal vol, std::map<TKey, TVal, TCompare>& map) {
    auto iter = map.find(price);
    auto result = false;
    if(iter != map.end()) {
        if(vol <= iter->second) {
            iter->second -= vol;
            if(iter->second == 0) {
                map.erase(iter);
            }
            result = true;
            notify_order_cancel(price, vol);
        }
    }
    return result;
}


