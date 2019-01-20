#include "svgen.h"


#include <cstdint>


static constexpr float SVGEN_SQRT3_OVER_2 = 0.8660254038F;


namespace svgen {


Svgen run(const ipark::Ipark& Valfa_beta_pu)
{
    float Vmax_pu = 0.0F;
    float Vmin_pu = 0.0F;

    const auto Va_pu = Valfa_beta_pu.value[0];
    const auto Vbeta_pu = Valfa_beta_pu.value[1];

    const float Va_tmp = -0.5F * Va_pu;
    const float Vb_tmp = SVGEN_SQRT3_OVER_2 * Vbeta_pu;
    const float Vb_pu = Va_tmp + Vb_tmp; // -0.5*Valpha + sqrt(3)/2 * Vbeta;
    const float Vc_pu = Va_tmp - Vb_tmp; // -0.5*Valpha - sqrt(3)/2 * Vbeta;

    // find Vmax and Vmin
    if (Va_pu > Vb_pu) {
        Vmax_pu = Va_pu;
        Vmin_pu = Vb_pu;
    } else {
        Vmax_pu = Vb_pu;
        Vmin_pu = Va_pu;
    }
    if (Vc_pu > Vmax_pu) {
        Vmax_pu = Vc_pu;
    } else if (Vc_pu < Vmin_pu) {
        Vmin_pu = Vc_pu;
    }

    // compute Vcom
    const float Vcom_pu = 0.5F * (Vmax_pu + Vmin_pu);

    // Subtract common-mode term to achieve SV modulation
    // result is unsigned (ranged [0,1])
    return {
        (Vc_pu - Vcom_pu) / 2.0F + 0.5F,
        (Vb_pu - Vcom_pu) / 2.0F + 0.5F,
        (Va_pu - Vcom_pu) / 2.0F + 0.5F
    };
}

} /* namespace svgen */
