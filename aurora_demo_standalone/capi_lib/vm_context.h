#pragma once
#include "vm_util.h"

#include <string>
#include <vector>

class vm_context {
public:
    std::vector<uint8_t> random_seed = hex_string_to_bytes("0x1234567890");
    
    std::vector<uint8_t> input = hex_string_to_bytes("0x608060405234801561001057600080fd5b50610168806100206000396000f3fe6080604052600436106100295760003560e01c8063c3da42b81461002e578063e1c7392a1461006d575b600080fd5b34801561003a57600080fd5b50610043610077565b604051808267ffffffffffffffff1667ffffffffffffffff16815260200191505060405180910390f35b610075610090565b005b6000809054906101000a900467ffffffffffffffff1681565b60016000809054906101000a900467ffffffffffffffff16016000806101000a81548167ffffffffffffffff021916908367ffffffffffffffff1602179055507fce5afaa0bb50af954d5977431c65c35b4417c30984ca04e53ce06ffeb4fc27146000809054906101000a900467ffffffffffffffff16604051808267ffffffffffffffff1667ffffffffffffffff16815260200191505060405180910390a156fea2646970667358221220609918375f4be9388512c0f6c641dae865436156ff2b6382f7157cfe819f424a64736f6c63430006040033");
    std::vector<uint8_t> account_id = string_to_bytes("aurora");
    std::vector<uint8_t> predecessor_account_id = string_to_bytes("carol");
    std::vector<uint8_t> signer_account_id = string_to_bytes("signer");
    // std::vector<uint8_t> input{'1', '2', '3'};
    // std::vector<uint8_t> account_id{'a', 'u', 'r', 'o', 'r', 'a'};
};