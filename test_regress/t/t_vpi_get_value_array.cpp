// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
//
// Copyright 2024 by Diego Roux. This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0
//
//*************************************************************************

#ifndef IS_VPI

#include "verilated.h"
#include "verilated_vcd_c.h"
#include "verilated_vpi.h"

#include "Vt_vpi_get_value_array.h"
#include "Vt_vpi_get_value_array__Dpi.h"
#include "svdpi.h"

#endif

#include <cstdio>
#include <cstring>
#include <iostream>

// These require the above. Comment prevents clang-format moving them
#include "TestSimulator.h"
#include "TestVpi.h"

// __FILE__ is too long
#define FILENM "t_vpi_get_value_array.cpp"

#define TEST_MSG \
    if (0) printf

//======================================================================

#define CHECK_RESULT_VH(got, exp) \
    if ((got) != (exp)) { \
        printf("%%Error: %s:%d: GOT = %p   EXP = %p\n", FILENM, __LINE__, (got), (exp)); \
        return __LINE__; \
    }

#define CHECK_RESULT_NZ(got) \
    if (!(got)) { \
        printf("%%Error: %s:%d: GOT = NULL  EXP = !NULL\n", FILENM, __LINE__); \
        return __LINE__; \
    }

// Use cout to avoid issues with %d/%lx etc
#define CHECK_RESULT(got, exp) \
    if ((got) != (exp)) { \
        std::cout << std::dec << "%Error: " << FILENM << ":" << __LINE__ << ": GOT = " << (got) \
                  << "   EXP = " << (exp) << std::endl; \
        return __LINE__; \
    }

#define CHECK_RESULT_HEX(got, exp) \
    if ((got) != (exp)) { \
        printf("%%Error: %s:%d GOT = 0x%lx EXP = 0x%lx\n", FILENM, __LINE__, \
               static_cast<uint64_t>(got), static_cast<uint64_t>(exp)); \
        return __LINE__; \
    }

#define CHECK_RESULT_CSTR(got, exp) \
    if (std::strcmp((got), (exp))) { \
        printf("%%Error: %s:%d: GOT = '%s'   EXP = '%s'\n", FILENM, __LINE__, \
               (got) ? (got) : "<null>", (exp) ? (exp) : "<null>"); \
        return __LINE__; \
    }

#define CHECK_RESULT_CSTR_STRIP(got, exp) CHECK_RESULT_CSTR(got + strspn(got, " "), exp)

int mon_check_props(void) {
    s_vpi_arrayvalue arrayVal = {0, 0, {NULL}};
    int indexArr[2] = {0};
    int num = 4;
    const int size = 4;

    {
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_bytes", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiRawTwoStateVal, 0, {nullptr}};
        int indexArr[1] = {rand() % size};
        int num = rand() % (size + 1);

        vpi_get_value_array(object, &arrayVal, indexArr, num);
        CHECK_RESULT_NZ(!vpi_chk_error(nullptr));

        PLI_UBYTE8* ptr = (PLI_UBYTE8*)arrayVal.value.rawvals;

        PLI_UBYTE8 expected[4] = {static_cast<PLI_UBYTE8>(0xde), static_cast<PLI_UBYTE8>(0xad),
                                  static_cast<PLI_UBYTE8>(0xbe), static_cast<PLI_UBYTE8>(0xef)};

        for (int i = 0; i < num; i++) {
            CHECK_RESULT_HEX(ptr[i], expected[(indexArr[0] + i) % size]);
        }
    }

    {
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_shorts", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiShortIntVal, 0, {nullptr}};
        int indexArr[1] = {rand() & size};
        int num = rand() % (size + 1);

        vpi_get_value_array(object, &arrayVal, indexArr, num);
        CHECK_RESULT_NZ(!vpi_chk_error(nullptr));

        PLI_UINT16* ptr = (PLI_UINT16*)arrayVal.value.shortints;

        PLI_UINT16 expected[4] = {0xdead, 0xbeef, 0xbeef, 0xdead};

        for (int i = 0; i < num; i++) {
            CHECK_RESULT_HEX(ptr[i], expected[(indexArr[0] + i) % size]);
        }
    }

    {
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiIntVal, 0, {nullptr}};
        int indexArr[1] = {rand() % size};
        int num = rand() % (size + 1);

        vpi_get_value_array(object, &arrayVal, indexArr, num);

        PLI_UINT32* ptr = (PLI_UINT32*)arrayVal.value.integers;

        PLI_UINT32 expected[4] = {0x00000000, 0xdeadbeef, 0x00000000, 0xdeadbeef};

        for (int i = 0; i < num; i++) {
            CHECK_RESULT_HEX(ptr[i], expected[(indexArr[0] + i) % size]);
        }
    }

    {
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words_rl", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiIntVal, 0, {nullptr}};
        int indexArr[1] = {rand() % size};
        int num = rand() % (size + 1);

        vpi_get_value_array(object, &arrayVal, indexArr, num);
        CHECK_RESULT_NZ(!vpi_chk_error(nullptr));

        PLI_UINT32* ptr = (PLI_UINT32*)arrayVal.value.integers;

        PLI_UINT32 expected[4] = {0xdeadbeef, 0xcafef00d, 0x01234567, 0x89abcdef};

        for (int i = 0; i < num; i++) CHECK_RESULT_HEX(ptr[i], expected[(indexArr[0] - i) % size]);
    }

    {
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_longs", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiLongIntVal, 0, {nullptr}};
        int indexArr[1] = {rand() % size};
        int num = rand() % (size + 1);

        vpi_get_value_array(object, &arrayVal, indexArr, num);
        CHECK_RESULT_NZ(!vpi_chk_error(nullptr));

        PLI_UINT64* ptr = (PLI_UINT64*)arrayVal.value.longints;

        PLI_UINT64 expected[4]
            = {0xdeadbeefcafef00d, 0x0123456789abcdef, 0xbeefdeadf00dcafe, 0x45670123cdef89ab};

        for (int i = 0; i < num; i++) CHECK_RESULT_HEX(ptr[i], expected[(indexArr[0] - i) % size]);
    }

    {
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_quads", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiRawFourStateVal, 0, {nullptr}};
        int indexArr[1] = {rand() % size};
        int num = rand() % (size + 1);
        vpi_get_value_array(object, &arrayVal, indexArr, num);

        PLI_UBYTE8* ptr = (PLI_UBYTE8*)arrayVal.value.rawvals;

        PLI_UINT32 expected[16]
            = {0x89abcdef, 0x01234567, 0xcafef00d, 0xdeadbeef, 0xcdef89ab, 0x45670123,
               0xf00dcafe, 0xbeefdead, 0xefcdab89, 0x67452301, 0x0df0feca, 0xefbeadde,
               0xba98fedc, 0x32107654, 0xd00fefac, 0xfeebdaed};

        printf("index=%u num=%u size=%u\n", indexArr[0], num, size);
        for (int i = 0; i < (num * 16); i++) {
            const int shiftedIndex = ((indexArr[0] * 16) + i) % (size * 16);
            // printf("shiftedIndex=%u\n",shiftedIndex);
            const int wordIndex = shiftedIndex / 4;
            // printf("wordIndex=%u\n",wordIndex);
            const uint32_t word = expected[wordIndex];

            const int byteIndex = shiftedIndex % size;
            // printf("byteIndex=%u\n",byteIndex);
            const uint8_t byte = (word >> (byteIndex * 8)) & UINT8_MAX;

            // printf("ptr[%u]=%x exp[?]=%x\n",i,ptr[i],byte);
            // CHECK_RESULT_HEX(ptr[i], byte);
        }
    }

    // {
    //     vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words", NULL);
    //     CHECK_RESULT_NZ(object);

    //     arrayVal.format = vpiVector;
    //     vpi_get_value_array(object, &arrayVal, indexArr, num);

    //     p_vpi_vecval ptr = (p_vpi_vecval)arrayVal.value.vectors;

    //     s_vpi_vecval expected[4] = {{0x00000000, 0x000000},
    //         {0xdeadbeef, 0x00000000}, {0x00000000, 0x00000000},
    //         {0xdeadbeef, 0x00000000}};

    //     for (int i = 0; i < num; i++) {
    //         CHECK_RESULT_HEX(ptr[i].aval, expected[i].aval);
    //         CHECK_RESULT_HEX(ptr[i].bval, expected[i].bval);
    //     }
    // }

    // {
    //     vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_integers", NULL);
    //     CHECK_RESULT_NZ(object);

    //     indexArr[0] = rand() & 0x3;

    //     arrayVal.format = vpiIntVal;
    //     vpi_get_value_array(object, &arrayVal, indexArr, num);

    //     PLI_INT32* ptr = arrayVal.value.integers;

    //     PLI_INT32 expected[4] = {INT32_MIN, INT32_MAX, 0, 1234567890};

    //     for (int i = 0; i < num; i++)
    //         CHECK_RESULT_HEX(ptr[i], expected[(indexArr[0]+i) & 0x3]);
    // }

    {
        // test unsupported format
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_longs", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiRealVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test unsupported foramt
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiShortRealVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test unsupported format
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_longs", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiTimeVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test unsupported vpiHandle
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiIntVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test unsupported type
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_scalar", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiIntVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test indexp out of bounds
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_bounds", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiIntVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {4};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));

        indexp[0] = {0};
        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test unsupported flags
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal = {vpiIntVal, vpiUserAllocFlag, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test unsupported format & vltype combination
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal = {vpiShortIntVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, num);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    {
        // test num out of bounds
        vpiHandle object = vpi_handle_by_name((PLI_BYTE8*)"TOP.test.read_words", NULL);
        CHECK_RESULT_NZ(object);

        s_vpi_arrayvalue arrayVal{vpiIntVal, 0, {nullptr}};
        PLI_INT32 indexp[1] = {0};

        vpi_get_value_array(object, &arrayVal, indexp, 5);
        CHECK_RESULT_NZ(vpi_chk_error(nullptr));
    }

    return 0;
}

extern "C" int mon_check(void) { return mon_check_props(); }

#ifndef IS_VPI

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    const std::unique_ptr<VM_PREFIX> top{new VM_PREFIX{contextp.get()}};
    contextp->fatalOnVpiError(0);
#ifdef TEST_VERBOSE
    contextp->internalsDump();
#endif

    while (!contextp->gotFinish()) {
        top->eval();
        VerilatedVpi::callValueCbs();
    }

    return 0;
}

#endif
