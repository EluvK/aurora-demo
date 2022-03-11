#include "../mock_top_api/evm_engine_interface.h"
#include "../mock_top_api/vm_import_instance.h"
#include "../mock_top_api/vm_util.h"

#include <gtest/gtest.h>

TEST(test_demo, deploy_code_twice) {
    auto & logic = vm_import_instance::instance()->get_vm_logic_ref();
    deploy_code();
    logic.context_ref().update_hex_string_input(
        "0x608060405234801561001057600080fd5b506101f7806100206000396000f3fe6080604052600436106100345760003560e01c80634f2be91f14610039578063c3da42b814610043578063c54124be1461008257"
        "5b600080fd5b61004161008c565b005b34801561004f57600080fd5b5061005861011a565b604051808267ffffffffffffffff1667ffffffffffffffff16815260200191505060405180910390f35b61008a610133"
        "565b005b60016000809054906101000a900467ffffffffffffffff16016000806101000a81548167ffffffffffffffff021916908367ffffffffffffffff1602179055506000809054906101000a900467ffffffff"
        "ffffffff1667ffffffffffffffff167f53777d0aaa80b55d855a69674c6ee34efa3ccb05a03ad90c38aaa0eacfe7dcd760405160405180910390a2565b6000809054906101000a900467ffffffffffffffff168156"
        "5b60016000809054906101000a900467ffffffffffffffff16036000806101000a81548167ffffffffffffffff021916908367ffffffffffffffff1602179055506000809054906101000a900467ffffffffffffff"
        "ff1667ffffffffffffffff167f09e6eb7229785cdf85405747f017ad07512b61743d1458c526daedb056f791a760405160405180910390a256fea2646970667358221220f5d0edd9d4d3902b094c01968b3fe1ea21"
        "1b6f8913755b092866166849deb75064736f6c63430006040033");
    deploy_code();
}

TEST(test_demo, call_contract) {
    deploy_code();
    auto & logic = vm_import_instance::instance()->get_vm_logic_ref();
    std::string contract_address = "fb29cba9b146786da16733f89982f7481effb094";
    std::string contract_function = "init()";
    logic.context_ref().update_input(noparam_function_input(contract_address, contract_function));
    call_contract();
    call_contract();
}


TEST(test_demo,erc20){
    
}