/*   
 *   File: atomic_ops.h
 *   Author: Tudor David <tudor.david@epfl.ch>
 *   Description: 
 *   atomic_ops.h is part of ASCYLIB
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Vasileios Trigonakis <vasileios.trigonakis@epfl.ch>
 *	      	      Distributed Programming Lab (LPD), EPFL
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/*
 *  File: atomic_ops.h
 *  Author: Tudor David
 *
 *  Created on: December 06, 2012
 *
 *  Description: 
 *      Cross-platform atomic operations
 */
#ifndef _ATOMIC_OPS_H_INCLUDED_
#define _ATOMIC_OPS_H_INCLUDED_

#include <inttypes.h>

#ifdef __sparc__
/*
 *  sparc code
 */

#  include <atomic.h>

//test-and-set uint8_t
static inline uint8_t tas_uint8(volatile uint8_t *addr) {
uint8_t oldval;
  __asm__ __volatile__("ldstub %1,%0"
        : "=r"(oldval), "=m"(*addr)
        : "m"(*addr) : "memory");
    return oldval;
}

//Compare-and-swap
#define CAS_PTR(a,b,c) atomic_cas_ptr(a,b,c)
#define CAS_U8(a,b,c) atomic_cas_8(a,b,c)
#define CAS_U16(a,b,c) atomic_cas_16(a,b,c)
#define CAS_U32(a,b,c) atomic_cas_32(a,b,c)
#define CAS_U64(a,b,c) atomic_cas_64(a,b,c)
//Swap
#define SWAP_PTR(a,b) atomic_swap_ptr(a,b)
#define SWAP_U8(a,b) atomic_swap_8(a,b)
#define SWAP_U16(a,b) atomic_swap_16(a,b)
#define SWAP_U32(a,b) atomic_swap_32(a,b)
#define SWAP_U64(a,b) atomic_swap_64(a,b)
//Fetch-and-increment
#define FAI_U8(a) (atomic_inc_8_nv(a)-1)
#define FAI_U16(a) (atomic_inc_16_nv(a)-1)
#define FAI_U32(a) (atomic_inc_32_nv(a)-1)
#define FAI_U64(a) (atomic_inc_64_nv(a)-1)
//Fetch-and-decrement
#define FAD_U8(a) (atomic_dec_8_nv(a,)+1)
#define FAD_U16(a) (atomic_dec_16_nv(a)+1)
#define FAD_U32(a) (atomic_dec_32_nv(a)+1)
#define FAD_U64(a) (atomic_dec_64_nv(a)+1)
//Increment-and-fetch
#define IAF_U8(a) atomic_inc_8_nv(a)
#define IAF_U16(a) atomic_inc_16_nv(a)
#define IAF_U32(a) atomic_inc_32_nv(a)
#define IAF_U64(a) atomic_inc_64_nv(a)
//Decrement-and-fetch
#define DAF_U8(a) atomic_dec_8_nv(a)
#define DAF_U16(a) atomic_dec_16_nv(a)
#define DAF_U32(a) atomic_dec_32_nv(a)
#define DAF_U64(a) atomic_dec_64_nv(a)
//Test-and-set
#define TAS_U8(a) tas_uint8(a)
//Memory barrier
#define MEM_BARRIER asm volatile("membar #LoadLoad | #LoadStore | #StoreLoad | #StoreStore"); 
//end of sparc code
#elif defined(__tile__)
/*
 *  Tilera code
 */
#include <arch/atomic.h>
#include <arch/cycle.h>
//atomic operations interface
//Compare-and-swap
#define CAS_PTR(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
#define CAS_U8(a,b,c)  arch_atomic_val_compare_and_exchange(a,b,c)
#define CAS_U16(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
#define CAS_U32(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
#define CAS_U64(a,b,c) arch_atomic_val_compare_and_exchange(a,b,c)
//Swap
#define SWAP_PTR(a,b) arch_atomic_exchange(a,b)
#define SWAP_U8(a,b) arch_atomic_exchange(a,b)
#define SWAP_U16(a,b) arch_atomic_exchange(a,b)
#define SWAP_U32(a,b) arch_atomic_exchange(a,b)
#define SWAP_U64(a,b) arch_atomic_exchange(a,b)
//Fetch-and-increment
#define FAI_U8(a) arch_atomic_increment(a)
#define FAI_U16(a) arch_atomic_increment(a)
#define FAI_U32(a) arch_atomic_increment(a)
#define FAI_U64(a) arch_atomic_increment(a)
//Fetch-and-decrement
#define FAD_U8(a) arch_atomic_decrement(a)
#define FAD_U16(a) arch_atomic_decrement(a)
#define FAD_U32(a) arch_atomic_decrement(a)
#define FAD_U64(a) arch_atomic_decrement(a)
//Increment-and-fetch
#define IAF_U8(a) (arch_atomic_increment(a)+1)
#define IAF_U16(a) (arch_atomic_increment(a)+1)
#define IAF_U32(a) (arch_atomic_increment(a)+1)
#define IAF_U64(a) (arch_atomic_increment(a)+1)
//Decrement-and-fetch
#define DAF_U8(a) (arch_atomic_decrement(a)-1)
#define DAF_U16(a) (arch_atomic_decrement(a)-1)
#define DAF_U32(a) (arch_atomic_decrement(a)-1)
#define DAF_U64(a) (arch_atomic_decrement(a)-1)
//Test-and-set
#define TAS_U8(a) arch_atomic_val_compare_and_exchange(a,0,0xff)
//Memory barrier
#define MEM_BARRIER arch_atomic_full_barrier()
//Relax CPU
//define PAUSE cycle_relax()

//end of tilera code
#else
/*
 *  x86 code
 */

#  include <xmmintrin.h>

#define _CAS_PTR(_p, _u, _v) (__atomic_compare_exchange(_p, _u, _v, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
#define _CAS(_p, _u, _v) (__atomic_compare_exchange_n(_p, _u, _v, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
#define _SWAP_PTR(_p, _v) (__atomic_exchange(_p, _v, __ATOMIC_SEQ_CST))
#define _SWAP(_p, _v) (__atomic_exchange_n(_p, _v, __ATOMIC_SEQ_CST))
#define _FAI(_p, _v) (__atomic_fetch_add(_p, _v, __ATOMIC_SEQ_CST))
#define _FAD(_p, _v) (__atomic_fetch_sub(_p, _v, __ATOMIC_SEQ_CST))
#define _IAF(_p, _v) (__atomic_add_fetch(_p, _v, __ATOMIC_SEQ_CST))
#define _DAF(_p, _v) (__atomic_sub_fetch(_p, _v, __ATOMIC_SEQ_CST))
#define _TAS(_p) (__atomic_test_and_set(_p, __ATOMIC_SEQ_CST))

//atomic operations interface
//Compare-and-swap
#define CAS_PTR(a,b,c) _CAS_PTR(a,b,c)
#define CAS_U8(a,b,c)  _CAS(a,b,c)
#define CAS_U8_NO_LOG(a,b,c)  __sync_val_compare_and_swap(a,b,c)
#define CAS_U16(a,b,c) _CAS(a,b,c)
#define CAS_U32(a,b,c) _CAS(a,b,c)
#define CAS_U64(a,b,c) _CAS(a,b,c)
//Swap
#define SWAP_PTR(a,b) _SWAP_PTR(a,b)
#define SWAP_U8(a,b) _SWAP(a,b)
#define SWAP_U16(a,b) _SWAP(a,b)
#define SWAP_U32(a,b) _SWAP(a,b)
#define SWAP_U64(a,b) _SWAP(a,b)
//Fetch-and-increment
#define FAI_U8(a) _FAI(a,1)
#define FAI_U16(a) _FAI(a,1)
#define FAI_U32(a) _FAI(a,1)
#define FAIV_U32(a,v) _FAI(a,v)
#define FAI_U64(a) _FAI(a,1)
//Fetch-and-decrement
#define FAD_U8(a) _FAD(a,1)
#define FAD_U16(a) _FAD(a,1)
#define FAD_U32(a) _FAD(a,1)
#define FAD_U64(a) _FAD(a,1)
//Increment-and-fetch
#define IAF_U8(a) _IAF(a,1)
#define IAF_U16(a) _IAF(a,1)
#define IAF_U32(a) _IAF(a,1)
#define IAF_U64(a) _IAF(a,1)
//Decrement-and-fetch
#define DAF_U8(a) _DAF(a,1)
#define DAF_U16(a) _DAF(a,1)
#define DAF_U32(a) _DAF(a,1)
#define DAF_U64(a) _DAF(a,1)
//Test-and-set
#define TAS_U8(a) _TAS(a)
//Memory barrier
#define MEM_BARRIER __sync_synchronize()
//Relax CPU
//#define PAUSE _mm_pause()

/*End of x86 code*/
#endif


#endif



