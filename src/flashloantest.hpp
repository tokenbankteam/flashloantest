#ifndef WORK_FLASHLOANTEST_H
#define WORK_FLASHLOANTEST_H

#include <eosiolib/eosio.hpp>
#include <eosiolib/name.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/crypto.hpp>
#include <eosiolib/types.h>
#include <eosio.system/eosio.system.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/permission.hpp>
#include <iterator>
#include <cstring>
#include <eosio.token/eosio.token.hpp>
#include <vector>

#define TEST 1
#if TEST
#define EOS_CONTRACT_CODE "eosio.token"_n
#define TPT_CONTRACT_CODE "eosiotptoke1"_n
#define FLASH_LOAN_CODE "flashloan211"_n
#else
#define EOS_CONTRACT_CODE "eosio.token"_n
#define FLASH_LOAN_CODE "flashloan.tp"_n
#endif

#define INT_BUY_LIMIT 1
#define INT_BUY_MARKET 3
#define INT_SELL_LIMIT 2
#define INT_SELL_MARKET 4
#define NEWDEX_PUBLIC_CODE "newdexpublic"_n

using namespace eosio;
using namespace std;

namespace tpflashloantest {

    using namespace eosio;
    using eosio::name;
    using eosio::contract;
    using eosio::asset;
    using eosio::symbol_code;
    using std::string;
    using std::vector;
    using eosio::permission_level;
    using eosio::public_key;
    using eosiosystem::wait_weight;
    using eosiosystem::key_weight;
    using eosiosystem::permission_level_weight;
    using eosiosystem::authority;

    class [[eosio::contract]] flashloantest : public eosio::contract {
    public:

        using eosio::contract::contract;

        flashloantest(eosio::name s, eosio::name code, eosio::datastream<const char *> ds) : contract(s, code, ds) {
        }

        ~flashloantest();

        [[eosio::action]]
        void transfer(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo);

        [[eosio::action]]
        void testfloan(eosio::name code, eosio::asset quantity);

        [[eosio::action]]
        void testffloan(eosio::asset quantity);

        [[eosio::action]]
        void flashloan();

    private:
        // code is token contract account, then scope is account
        struct [[eosio::table, eosio::contract("eosio.token")]] account {
            asset balance;

            uint64_t primary_key() const { return balance.symbol.code().raw(); }
        };

        typedef eosio::multi_index<"accounts"_n, account> accounts_table;

        struct [[eosio::table]]  order {
            uint64_t order_id;                  //order ID
            uint64_t pair_id;                    //pair ID
            uint8_t type;                          //order type, 1:limit-buy  2:limit-sell  3:market-buy  4:market-sell
            name owner;
            time_point_sec placed_time;
            asset remain_quantity;
            asset remain_convert;
            double price;
            name contract;                        // token contract
            uint8_t count;                         //transfer count
            uint8_t crosschain;                  // is crosschain
            uint64_t ext1;
            string extstr;

            uint64_t primary_key() const { return order_id; }

            uint128_t get_price() const {
                uint64_t max = 1000000000000;
                if (type == INT_BUY_LIMIT || type == INT_BUY_MARKET) {
                    uint128_t high = (uint128_t)(max * price);
                    uint64_t low = max - placed_time.utc_seconds;
                    uint128_t price128 = (high << 64) + low;
                    return price128;
                } else {
                    return (uint128_t)(max * price);
                }
            }

            uint64_t get_name() const { return owner.value; }

            EOSLIB_SERIALIZE( order, (order_id)(pair_id)(type)(owner)(placed_time)(remain_quantity)(remain_convert)(
                    price)(contract)(count)(crosschain)(ext1)(extstr)
            )
        };

        typedef eosio::multi_index<"buyorder"_n, order,
                indexed_by<"byprice"_n, const_mem_fun < order, uint128_t, &order::get_price> >,
        indexed_by<"byname"_n, const_mem_fun<order, uint64_t, &order::get_name> > >
        buy_order_t;

        typedef eosio::multi_index<"sellorder"_n, order,
                indexed_by<"byprice"_n, const_mem_fun < order, uint128_t, &order::get_price> >,
        indexed_by<"byname"_n, const_mem_fun<order, uint64_t, &order::get_name> > >
        sell_order_t;
    private:
        static std::vector <std::string> &
        split(const std::string &s, const std::string delim, std::vector <std::string> &result);

        static uint128_t _ttoi(eosio::name code, eosio::symbol symbol) {
            return (uint128_t(code.value) << 64) + symbol.raw();
        }

        static constexpr eosio::name active_permission{"active"_n};

    private:
        uint32_t delay_sec = 1;
    };
}

#define EOSIO_DISPATCH_EX(TYPE, MEMBERS) \
            extern "C" { \
                void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
                    auto self = receiver; \
                    if( action == eosio::name("onerror").value) { \
                        /* onerror is only valid if it is for the "enumivo" code account and authorized by "eosio"'s "active permission */ \
                        eosio_assert(code == eosio::name("eosio").value, "onerror action's are only valid from the \"eosio\" system account"); \
                    } \
                    if((code == self && action != eosio::name("transfer").value) || action == eosio::name("onerror").value || (action == eosio::name("transfer").value && code != self)) { \
                            switch( action ) { \
                            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
                        } \
                    } \
                } \
            } \

EOSIO_DISPATCH_EX(tpflashloantest::flashloantest, (transfer)(testfloan)(testffloan))

#endif //WORK_FLASHLOANTEST_H
