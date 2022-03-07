#pragma once
#include "stdint.h"
#include "vm_context.h"
#include "vm_ext.h"
#include "vm_memory.h"
#include "vm_util.h"

#include <climits>
#include <map>
#include <vector>

class vm_logic {
public:
    uint64_t register_len(uint64_t register_id) {
        printf("[debug][register_len] size: %zu request: %lu \n", m_registers.size(), register_id);
        return m_registers.at(register_id).size();
    }

    void read_register(uint64_t register_id, uint64_t ptr) {
        std::vector<uint8_t> data = internal_read_register(register_id);
        printf("[debug][read_register] request: %lu \n ", register_id);
        // for (auto const & _c : data) {
        //     printf("%c", _c);
        // }
        // printf("\n");
        // printf("debug %lu \n",ptr);
        memory_set_slice(ptr, data);
    }

    void current_account_id(uint64_t register_id) {
        printf("[debug][current_account_id] request: %lu \n", register_id);
        internal_write_register(register_id, m_context.account_id);
    }

    void predecessor_account_id(uint64_t register_id) {
        printf("[debug][predecessor_account_id] request: %lu \n", register_id);
        internal_write_register(register_id, m_context.predecessor_account_id);
    }

    void signer_account_id(uint64_t register_id) {
        printf("[debug][signer_account_id] request: %lu\n", register_id);
        internal_write_register(register_id, m_context.signer_account_id);
    }

    void input(uint64_t register_id) {
        printf("[debug][input] request: %lu\n", register_id);
        internal_write_register(register_id, m_context.input);
        return;
    }

    // storage:
    uint64_t storage_read(uint64_t key_len, uint64_t key_ptr, uint64_t register_id) {
        printf("[debug][storage_read] request: %lu\n", register_id);
        std::vector<uint8_t> key = get_vec_from_memory_or_register(key_ptr, key_len);
        std::vector<uint8_t> read = m_ext.storage_get(key);
        if (!read.empty()) {
            internal_write_register(register_id, read);
            return 1;
        } else {
            return 0;
        }
    }

    uint64_t storage_write(uint64_t key_len, uint64_t key_ptr, uint64_t value_len, uint64_t value_ptr, uint64_t register_id) {
        printf("[debug][storage_write] request: %lu\n", register_id);
        std::vector<uint8_t> key = get_vec_from_memory_or_register(key_ptr, key_len);
        std::vector<uint8_t> value = get_vec_from_memory_or_register(value_ptr, value_len);

        std::vector<uint8_t> read_old_value = m_ext.storage_get(key);

        m_ext.storage_set(key, value);

        if (!read_old_value.empty()) {
            internal_write_register(register_id, read_old_value);
            return 1;
        } else {
            return 0;
        }
    }

    uint64_t storage_remove(uint64_t key_len, uint64_t key_ptr, uint64_t register_id) {
        printf("[debug][storage_remove] request: %lu\n", register_id);
        std::vector<uint8_t> key = get_vec_from_memory_or_register(key_ptr, key_len);
        std::vector<uint8_t> read = m_ext.storage_get(key);

        if (!read.empty()) {
            internal_write_register(register_id, read);
            m_ext.storage_remove(key);
            return 1;
        } else {
            return 0;
        }

        return 0;
    }

    void value_return(uint64_t key_len, uint64_t key_ptr) {
        printf("[debug][value_return]\n");
        return_data_value = get_vec_from_memory_or_register(key_ptr, key_len);
    }

    void account_balance(uint64_t balance_ptr) {
        printf("[debug][account_balance]\n");
        m_memory.write_memory(balance_ptr, to_le_bytes(current_account_balance));
    }

    void sha256(uint64_t value_len, uint64_t value_ptr, uint64_t register_id) {
        std::vector<uint8_t> value = get_vec_from_memory_or_register(value_ptr, value_len);

        auto value_hash = get_sha256(value);

        internal_write_register(register_id, value_hash);
    }

    void keccak256(uint64_t value_len, uint64_t value_ptr, uint64_t register_id) {
        sha256(value_len, value_ptr, register_id);
    }

    void ripemd160(uint64_t value_len, uint64_t value_ptr, uint64_t register_id) {
        std::vector<uint8_t> value = get_vec_from_memory_or_register(value_ptr, value_len);

        auto value_hash = get_ripemd160(value);

        internal_write_register(register_id, value_hash);
    }

    // MATH API
    void random_seed(uint64_t register_id) {
        internal_write_register(register_id, m_context.random_seed);
    }

    // LOG
    void log_utf8(uint64_t len, uint64_t ptr) {
        std::string message = get_utf8_string(len, ptr);
        printf("[log_utf8] VM_LOG: %s \n", message.c_str());
    }

private:
    vm_context m_context;
    vm_ext m_ext;
    std::map<uint64_t, std::vector<uint8_t>> m_registers;
    vm_memory m_memory;
    uint128_t current_account_balance;
    std::vector<uint8_t> return_data_value;

private:
    std::string get_utf8_string(uint64_t len, uint64_t ptr) {
        std::vector<uint8_t> buf;
        if (len != UINT64_MAX) {
            buf = memory_get_vec(ptr, len);
        } else {
            // todo
        }

        std::string res;
        for (auto const & c : buf) {
            res.push_back(c);
        }
        return res;
    }

    void internal_write_register(uint64_t register_id, std::vector<uint8_t> const & context_input) {
        // printf("[internal_write_register]before write register size: %zu\n", m_registers.size());
        m_registers[register_id] = context_input;
        // printf("[internal_write_register]after write register size: %zu\n", m_registers.size());
        // for (auto const & _p : m_registers) {
            // printf("[debug][internal_write_register] after debug: %zu : ", _p.first);
            // for (auto const & _c : _p.second) {
            //     printf("%c", _c);
            // }
            // printf("\n");
        // }
    }

    std::vector<uint8_t> get_vec_from_memory_or_register(uint64_t offset, uint64_t len) {
        if (len != UINT64_MAX) {
            return memory_get_vec(offset, len);
        } else {
            return internal_read_register(offset);
        }
    }

    void memory_set_slice(uint64_t offset, std::vector<uint8_t> buf) {
        m_memory.write_memory(offset, buf);
    }

    std::vector<uint8_t> memory_get_vec(uint64_t offset, uint64_t len) {
        std::vector<uint8_t> buf(len, 0);
        m_memory.read_memory(offset, buf);
        return buf;
    }

    std::vector<uint8_t> internal_read_register(uint64_t register_id) {
        return m_registers.at(register_id);
    }
};