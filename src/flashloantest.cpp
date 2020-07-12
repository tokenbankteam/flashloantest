#include "flashloantest.hpp"

using namespace tpflashloantest;
using namespace eosio;

std::vector <std::string> &
flashloantest::split(const std::string &s, const std::string delim, std::vector <std::string> &result) {
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    while (index != std::string::npos) {
        result.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (index - last > 0) {
        result.push_back(s.substr(last, index - last));
    }
    return result;
}

//memo的格式 method:param1,param2,param3
//method 方法 deposit
void flashloantest::transfer(name from, name to, asset quantity, string memo) {
    require_auth(from);
    if (!(from != _self && to == _self)) {
        return;
    }
    if (from == FLASH_LOAN_CODE && quantity.symbol.code() == eosio::symbol_code{std::string("EOS")}) {
        //TODO BUSINESS
        //TODO BUSINESS
//        check(false, get_first_receiver().to_string() + " go here");

        //充值
        auto deposit_quantity = asset{int64_t((1000)), quantity.symbol};
        action{
                permission_level{_self, "active"_n},
                get_first_receiver(),
                "transfer"_n,
                std::make_tuple(_self, FLASH_LOAN_CODE, deposit_quantity, std::string("deposit:1"))
        }.send();

        auto pay_quantity = asset{int64_t((1 + 0.01) * quantity.amount), quantity.symbol};
        action{
                permission_level{_self, "active"_n},
                get_first_receiver(),
                "transfer"_n,
                std::make_tuple(_self, FLASH_LOAN_CODE, pay_quantity, std::string("repay:"))
        }.send();
        return;
    }
}

void flashloantest::testfloan(eosio::name code, asset quantity) {
    action{
            permission_level{_self, "active"_n},
            FLASH_LOAN_CODE,
            "floan"_n,
            std::make_tuple(code, quantity, _self)
    }.send();
}

void flashloantest::testffloan(asset quantity) {
    action{
            permission_level{_self, "active"_n},
            FLASH_LOAN_CODE,
            "ffloan"_n,
            std::make_tuple(uint64_t(0), quantity, _self)
    }.send();
}

void flashloantest::flashloan() {
    buy_order_t buy_order(NEWDEX_PUBLIC_CODE, 235);
    auto idx = buy_order.get_index<"byprice"_n>();
    auto begin_it = idx.begin();
    if (begin_it != idx.end()) {
        check(false, "go here");
    }
}

flashloantest::~flashloantest() {
}