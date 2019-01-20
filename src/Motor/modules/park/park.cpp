#include "park.h"


namespace park {

Park run(const float theta_pu, const clarke::Clarke& pInVec)
{
    const auto phasor = phasor::createFrom_pu(theta_pu);
    return run(phasor, pInVec);
}


Park run(const phasor::PhasorSinCos& phasor, const clarke::Clarke& pInVec)
{
    const auto sinTh = phasor.value[0];
    const auto cosTh = phasor.value[1];

    const auto v0 = pInVec.value[0];
    const auto v1 = pInVec.value[1];

    const float out0 = (v0 * cosTh) + (v1 * sinTh);
    const float out1 = (v1 * cosTh) - (v0 * sinTh);

    return {out0, out1};
}

} /* namespace park */


