/*
 * itoa.cpp
 *
 *  Created on: 29 февр. 2016 г.
 *      Author: Vitaliy
 */

#include <cstring>
#include "itoa.h"
#include <cmath>


size_t uint32_tToA(uint8_t* to, const uint32_t u)
{
    return uint32_tToA(reinterpret_cast<char*>(to), u);
}


size_t int32_tToA(uint8_t* to, const int32_t i)
{
    return int32_tToA(reinterpret_cast<char*>(to), i);
}



size_t uint32_tToA(char* to, const uint32_t u_)
{
    uint32_t u = u_;
    constexpr int maxSize = 12; /* Only for 32-bit integers: 32/Log_2[10]=9.63:*/
    char buf[maxSize];
    char* const ptrEnd = buf + sizeof(buf) - 1;
    char* ptr = ptrEnd;
    *ptr = 0;
    do {
        --ptr;
       *ptr = '0' + (u % 10);
       u /= 10;
    } while (u);
    const int len = ptrEnd - ptr;
    memcpy(to, ptr, len);
    return len;
}


size_t int32_tToA(char* to, const int32_t i)
{
    const bool flagNegative = i < 0;
    if (flagNegative) {
        const uint32_t u = i > 0 ? i : static_cast<uint32_t>((-(1 + i)) + 1);
        *to = '-';
        ++to;
        return uint32_tToA(to, u) + 1;
    } else {
        return uint32_tToA(to, i);
    }
}


size_t float3pToA(char* buf, const float v_)
{
	size_t s = 0;
	float v = roundf(v_ * 1000.0F) / 1000.0F;
	if (v < 0.0F) {
		v = -v;
		buf[s] = '-';
		++s;
	}
	int vint = static_cast<int>(v);
	s += int32_tToA(&buf[s], vint);
	buf[s] = '.';
	++s;
	v -= vint;
	vint = static_cast<int>(v * 1000.0F);
	buf[s] = vint / 100 + '0'; ++s;
	buf[s] = ((vint % 100) / 10) + '0'; ++s;
	buf[s] = (vint % 10) + '0'; ++s;
	return s;
}


size_t float3pToA(uint8_t* buf, const float v)
{
    return float3pToA(reinterpret_cast<char*>(buf), v);
}


size_t float6pToA(char* buf, const float v_)
{
    size_t s = 0;
    float v = roundf(v_ * 1000000.0F) / 1000000.0F;
    if (v < 0.0F) {
        v = -v;
        buf[s] = '-';
        ++s;
    }
    int vint = static_cast<int>(v);
    s += int32_tToA(&buf[s], vint);
    buf[s] = '.';
    ++s;
    v -= vint;
    vint = static_cast<int>(v * 1000000.0F);
    buf[s] = vint / 100000 + '0'; ++s;
    buf[s] = ((vint % 100000) / 10000) + '0'; ++s;
    buf[s] = ((vint % 10000) / 1000) + '0'; ++s;
    buf[s] = ((vint % 1000) / 100) + '0'; ++s;
    buf[s] = ((vint % 100) / 10) + '0'; ++s;
    buf[s] = (vint % 10) + '0'; ++s;
	return s;
}
