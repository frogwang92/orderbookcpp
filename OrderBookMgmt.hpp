//
// Created by yi wang on 6/27/2017.
//

#ifndef ORDERBOOKCPP_ORDERBOOKMGMT_HPP
#define ORDERBOOKCPP_ORDERBOOKMGMT_HPP

#include "Global.hpp"
#include <iostream>
#include <functional>
/* limit orders saved in a in-memory map
 */

class OrderBookMgmt {

public:
    enum Side {
        BID = 1,
        ASK = 2
    };

public:
    OrderBookMgmt() noexcept;

    OrderBookMgmt(const OrderBookMgmt &rhs) = delete;

    OrderBookMgmt(const OrderBookMgmt &&rhs) = delete;

    OrderBookMgmt &operator=(const OrderBookMgmt &rhs) = delete;

    virtual ~OrderBookMgmt() noexcept {}

public:
    void ask(OrderBook::PriceType price, OrderBook::VolType vol, bool _cancel = false);

    void bid(OrderBook::PriceType price, OrderBook::VolType vol, bool _cancel = false);

    std::tuple<OrderBook::VolType,Side> query(OrderBook::PriceType price) const;

    // expensive func
    // return a copy of internal memory
    std::tuple<OrderBook::ReverseOrderBookType, OrderBook::OrderBookType> take_snap_shot() const;

private:
    void init(void) noexcept { cancel_err_handler = [&](){std::cout << "cancel error" << std::endl; }; }

    template<typename TKey, typename TVal, typename TCompare>
    OrderBook::PriceType get_gap(const std::map<TKey, TVal, TCompare>& book) const {
        if (book.empty()) { return OrderBook::NaN; }
        else { return book.begin()->first; }
    };

    // when ask:
    // from_map => order_book_bid
    // to_map   => order_book_ask
    // vise versa
    template<typename TKey, typename TVal, typename TCompareFrom, typename TCompareTo>
    void match(TKey price,
               TVal vol,
               std::map<TKey, TVal, TCompareFrom>& from_map,
               std::map<TKey, TVal, TCompareTo>& to_map);

    template<typename TKey, typename TVal, typename TCompare>
    bool cancel(TKey price, TVal vol, std::map<TKey, TVal, TCompare>& map);

    // preserved, do some notif job when order matched
    void notify_order_match(OrderBook::PriceType _price, OrderBook::VolType _vol) const {
        std::cout << "order matched: "
                << " price: "
                << _price
                << " vol: "
                << _vol
                << std::endl;
    }
    void notify_order_cancel(OrderBook::PriceType _price, OrderBook::VolType _vol) const {
        std::cout << "order cancelled: "
                  << " price: "
                  << _price
                  << " vol: "
                  << _vol
                  << std::endl;
    }

    std::function<void(void)> cancel_err_handler;

private:
    OrderBook::OrderBookType order_book_ask;
    OrderBook::ReverseOrderBookType order_book_bid;

};


#endif //ORDERBOOKCPP_ORDERBOOKMGMT_HPP
