# flashloantest 介绍

flash loan on EOS network, you can loan eos in one transaction without collateral. here are some flash loan test units

# flashloantest编译部署

## 依赖环境

1. eosio.cdt --- branch: release/1.5.x 或以上
2. eosio.contract --- tag:  v1.5.2 或以上

## 编译

#### 1. 将flashloantest目录放入eosio.contracts内，跟eosio.token同级即可。
#### 2. 编辑文件 eosio.contracts/CMakeLists.txt:

```
add_subdirectory(flashloantest)
```
#### 3. 运行eosio.contracts/build.sh完成编译
 ```
 ./build.sh
 ```
 
 ## 部署
```
//创建合约帐号
cleos -u 'http://kylin.meet.one:8888' system newaccount itokenpocket flashloan22t EOS61Lv9AArofgBUhE4JKtJ5zJrLdRbNBwgPc5sWsxMNT6HtNm5py EOS8gE7XgUVLmF8CN9CCixNaVGFzbxvagVdMm6pqP47eMpuRMUDx8 --buy-ram '20.0000 EOS' --stake-net '3.0000 EOS' --stake-cpu '20.0000 EOS'
//授权
cleos -u 'http://kylin.meet.one:8888' set account permission flashloan22t active '{"threshold": 1,"keys": [{"key": "EOS8gE7XgUVLmF8CN9CCixNaVGFzbxvagVdMm6pqP47eMpuRMUDx8", "weight": 1}],"accounts": [{"permission":{"actor":"flashloan22t","permission":"eosio.code"},"weight":1}]}' owner -p flashloan22t@owner

cd build
cleos -u 'http://kylin.meet.one:8888' set contract flashloan22t ./flashloantest -p flashloan22t
```
# flashloantest测试
## 充值
```
cleos -u 'http://kylin.meet.one:8888' push action eosio.token transfer '["tokenfletest", "flashloana25", "10.0000 EOS", "deposit:1"]' -p tokenfletest
cleos -u 'http://kylin.meet.one:8888' push action eosio.token transfer '["tokenfletes1", "flashloana25", "3.0000 EOS", "deposit:1"]' -p tokenfletes1
```
## 赎回
```
cleos -u 'http://kylin.meet.one:8888' get currency balance flashloant25 tokenfletest TEOS
cleos -u 'http://kylin.meet.one:8888' push action flashloant25 transfer '["tokenfletest", "flashloana25", "10.0000 TEOS", "redeem:1"]' -p tokenfletest
```
## 触发借贷
```
cleos -u 'http://kylin.meet.one:8888' push action eosio.token transfer '["itokenpocket", "flashloan22t", "20.0000 EOS", ""]' -p itokenpocket
cleos -u 'http://kylin.meet.one:8888' push action flashloan22t testfloan '["1.0000 EOS"]' -p flashloan22t
```
