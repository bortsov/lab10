/*
 * motorNew.h
 *
 *  Created on: 06 окт. 2015 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_MOTORNEW_H_
#define INCLUDE_MOTORNEW_H_

#include <cstdint>
#include "het.h"


namespace motorNew {

enum MOTORWIND {
    M1,
    M2,
    M3
};

enum HALLS {
    H1,
    H2,
    H3
};

enum HALL_SECTOR {
    SECTOR1 = 1,
    SECTOR2,
    SECTOR3,
    SECTOR4,
    SECTOR5,
    SECTOR6
};


enum DRIVER_STATE {
    DRIVER_OFF,
    DRIVER_ON
};


enum WORK_MODE {
    MODE_OFF,
    MODE_ENCODER_CALIBRATION,
    MODE_GENERATE_ANGLE,
    MODE_TRANSLATE_I_ALPHA_BETA,
    MODE_TRANSLATE_V_DQ,
	MODE_CLOSE_CURRENT_LOOP,
};

struct PinsMotorInHet {
    uint8_t m1;
    uint8_t m2;
    uint8_t m3;
};

void create(
        const het::HET nHET,
        const uint32_t startAddr,
        const PinsMotorInHet& mpins);
void resetMotorDriver();
void disableMotorDrive();
void applyMotorTypeParameters();

namespace set {
    void encoderPosition_int(const uint32_t newPosition);
    void linkLogChannels(const int nChannel, const int nStream);
    void runMode(const WORK_MODE wm);
    void Iq(const float iq_A);
    void Id(const float id_A);
} /* namespace set */



namespace get {
    bool            hall(const HALLS);
    HALL_SECTOR     hallSector(const bool h1, const bool h2, const bool h3);
    uint32_t        encoderPosition_int();
    float           encoderPosition_pu();
    uint32_t        summaryEncoderPosition();
    float           hallPosition_pu();
    float           measuredAngleSpeed_epm();
    float           measuredAngleSpeedFiltered_epm();
    int             nStreamForLogChannel(const int n);
} /* namespace get */

} /* namespace motorNew */




#endif /* INCLUDE_MOTORNEW_H_ */
