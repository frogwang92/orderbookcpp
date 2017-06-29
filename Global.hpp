//
// Created by yi wang on 6/27/2017.
//

#ifndef ORDERBOOKCPP_GLOBAL_HPP
#define ORDERBOOKCPP_GLOBAL_HPP

#include <map>

namespace OrderBook {

    template<typename TKey, typename TVal, typename TCompare=std::less<TKey>>
    using Map = std::map<TKey, TVal, TCompare>;

    template <typename TKey, typename TVal, typename TCompare=std::greater<TKey>>
    using ReverseMap = std::map<TKey,TVal, TCompare>;

    template<typename TKey, typename TVal>
    using Pair = std::pair<TKey, TVal>;

    using PriceType = unsigned;
    using VolType   = int;

    using OrderBookType = Map<PriceType, VolType>;
    using ReverseOrderBookType = ReverseMap<PriceType, VolType>;

    using OrderPairType = Pair<PriceType, VolType>;

    static const PriceType NaN = 0;
}
#endif //ORDERBOOKCPP_GLOBAL_HPP
