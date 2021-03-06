#pragma once

#include "assert.h"
#include "ripemd160.h"
#include "sh256.h"
#include "stdint.h"

#include <string>
#include <vector>

typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

#include <vector>

std::vector<uint8_t> to_le_bytes(uint128_t value) {
    std::vector<uint8_t> ret(16);
    for (auto i = 0; i < 16; i++) {
        ret[0] = (value >> i * 8) & 0xff;
    }
    return ret;
}

#define CON(x) (std::isdigit((x)) ? ((x) - '0') : (std::tolower((x)) - 'W'))

std::vector<uint8_t> hex_string_to_bytes(std::string const & input) {
    assert(input.substr(0, 2) == "0x");
    assert(input.size() % 2 == 0);
    std::vector<uint8_t> res;

    printf("[debug][hex_string_to_bytes] input: %s \n", input.c_str());
    res.resize(input.size() / 2 - 1);

    for (std::size_t i = 1; i < input.size() / 2; ++i) {
        res[i - 1] = (CON(input[2 * i]) << 4) + CON(input[2 * i + 1]);
    }
    // printf("[debug][hex_string_to_bytes] ");
    // for (auto c : res) {
    //     printf("\\0x%x ", c);
    // }
    // printf("\n");
    return res;
}

std::vector<uint8_t> string_to_bytes(std::string const & input) {
    std::vector<uint8_t> res;
    res.resize(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) {
        res[i] = input[i];
    }
    return res;
}

std::vector<uint8_t> get_sha256(std::vector<uint8_t> const & input) {
    BYTE _input[input.size()];
    for (std::size_t index = 0; index < input.size(); ++index) {
        _input[index] = input[index];
    }

    BYTE _res[SHA256_BLOCK_SIZE];

    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, _input, input.size());
    sha256_final(&ctx, _res);

    std::vector<uint8_t> res;
    res.resize(SHA256_BLOCK_SIZE);
    for (std::size_t index = 0; index < SHA256_BLOCK_SIZE; ++index) {
        res[index] = _res[index];
    }
    return res;
}

std::vector<uint8_t> get_ripemd160(std::vector<uint8_t> const & input) {
    size_t msglen = input.size();
    size_t hashlen = 20;

    const uint8_t * msg = &input[0];
    std::vector<uint8_t> hash(hashlen);
    uint8_t * phash = &hash[0];

    ripemd160_cal(msg, msglen, phash);
    return hash;
}