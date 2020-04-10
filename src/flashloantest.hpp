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
        void testfloan(eosio::asset quantity);

        [[eosio::action]]
        void testffloan(eosio::asset quantity);

        [[eosio::action]]
        void testfloant(eosio::asset quantity);

        [[eosio::action]]
        void testffloant(eosio::asset quantity);

    private:
        // code is token contract account, then scope is account
        struct [[eosio::table, eosio::contract("eosio.token")]] account {
            asset balance;

            uint64_t primary_key() const { return balance.symbol.code().raw(); }
        };

        typedef eosio::multi_index<"accounts"_n, account> accounts_table;

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

EOSIO_DISPATCH_EX(tpflashloantest::flashloantest, (transfer)(testfloan)(testffloan)(testfloant)(testffloant))

#endif //WORK_FLASHLOANTEST_H
