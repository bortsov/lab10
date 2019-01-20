#include "clarke.h"


static constexpr float MATH_ONE_OVER_SQRT_THREE = 0.57735026918962576450914878050196F; /* 1 / sqrt(3) */

namespace clarke {

Clarke run(const MATH_vec3& pInVec)
{
    const auto Isa = pInVec.value[0];
    const auto Isb = pInVec.value[1];
    const auto Isc = pInVec.value[2];
    const float Is_alfa = Isa;
    const float Is_beta = (Isb - Isc) * MATH_ONE_OVER_SQRT_THREE;
    return {Is_alfa, Is_beta};
}


Clarke run(const MATH_vec2& pInVec)
{
    const auto Isa = pInVec.value[0];
    const auto Isc = pInVec.value[1];
    const float Is_alfa = Isa;
    const float Is_beta = (Isa + Isc * 2.0F) * (-MATH_ONE_OVER_SQRT_THREE);
    return {Is_alfa, Is_beta};
}

/*
 sa + 2 * sb = sa + (-sa - sc) * 2 = sa - 2*sa - 2 *sc = -sa-2*sc
 */


} /* namespace clarke */
