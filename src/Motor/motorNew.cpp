/*
 * motorNew.cpp
 *
 *  Created on: 06 окт. 2015 г.
 *      Author: bortsov
 *  Примечания.
 *    Цикл PWM делится на 2 фазы: stage1 и stage2. В первой фазе производится
 *    проверка на необходимость создания положительного фронта сингала ШИМ, во
 *    второй производится проверка на необходимость создания отрицательного
 *    фронта ШИМ.
 *    Каждый фронт для каждого сигнала ШИМ управляется своей инструкцией ECMP
 *    Подразумевается, что в модуле HET установлено hrc=128, иначе надо
 *    учитывать разрыв разрядов при доступе к полям данных инструкций ECMP и
 *    MCMP
 */

#include "vclk2.h"
#include "Motor/motorNew.h"
#include "blockingWait.h"
#include "getclock.h"
#include "pin/pin.h"
#include "Motor/MotorCurrentMeasure.h"
#include "Motor/controller_data_types.h"
#include "softwareSpi.h"

#include "modules/svgen/svgen.h"
#include "modules/phasor/phasor.h"
#include "modules/park/park.h"
#include "modules/ipark/ipark.h"
#include "modules/clarke/clarke.h"
#include "modules/pi/pi.h"
#include "modules/angle_gen/angle_gen.h"
#include "modules/trajectory/trajectory.h"
#include "reg_htu.h"
#include "reg_het.h"

#include "sys_pmu.h"
#include "main.h"
#include "LogPoint.h"

#include <cmath>


using het::HET1;

namespace motorNew {

/*
 * Motor ДВМ70М-ЭТ17ШТ
 */
#if 0
static constexpr float MAXIMUM_CURRENTS_A = 3.0F;
static constexpr float N_MOTOR_POLUS_PAIRS = 7.0F;
static constexpr int N_ENCODER_LINES = 500;
static constexpr float ENCODER_CALIBRATION_VOTAGE_PU = 0.1F;
static constexpr float KP_CURRENT_REGULATORS = xx;
static constexpr float KI_CURRENT_REGULATORS = xx;
static constexpr float KP_SPEED_REGULATOR = 0.03F;
static constexpr float KI_SPEED_REGULATOR = 0.001F;
static constexpr float MAX_SPEED_EPM = 4800.0F;
/* ускорение "по-умолчанию" составит 9600 электрических об/мин/с */
static constexpr float DEFAULT_ACCELERATION_EPM = 9600.0F;
#endif

/*
 * Motor Maxon EC-Max 30 + encoder MR
 */
#if 1
static constexpr float MAXIMUM_CURRENTS_A = 4.4F;
static constexpr float N_MOTOR_POLUS_PAIRS = 1.0F;
static constexpr int N_ENCODER_LINES = 1000;
static constexpr float ENCODER_CALIBRATION_VOTAGE_PU = 0.15F;
static constexpr float KP_CURRENT_REGULATORS = 0.045; /* 0.009 for 1/20 */
static constexpr float KI_CURRENT_REGULATORS = 0.444F;
static constexpr float KP_SPEED_REGULATOR = 0.01F;
static constexpr float KI_SPEED_REGULATOR = 0.001F;
static constexpr float MAX_SPEED_EPM = 4800.0F;

/* ускорение "по-умолчанию" составит 9600 электрических об/мин/с */
static constexpr float DEFAULT_ACCELERATION_EPM = 100.0F; /* для отладки временно 100 эл об/мин/с */
#endif

/*
 * Motor Teknic M-2310P-LN-04K
 */
#if 0
static constexpr float MAXIMUM_CURRENTS_A = 3.0F;
static constexpr float N_MOTOR_POLUS_PAIRS = 4.0F;
static constexpr int N_ENCODER_LINES = 1000;
static constexpr float ENCODER_CALIBRATION_VOTAGE_PU = 0.1F;
static constexpr float KP_CURRENT_REGULATORS = 0.105F;
static constexpr float KI_CURRENT_REGULATORS = 0.09F;
static constexpr float KP_SPEED_REGULATOR = 0.03F;
static constexpr float KI_SPEED_REGULATOR = 0.001F;
static constexpr float MAX_SPEED_EPM = 4800.0F;

/* ускорение "по-умолчанию" составит 9600 электрических об/мин/с */
static constexpr float DEFAULT_ACCELERATION_EPM = 9600.0F;
#endif


static constexpr float SCALE_HZ_TO_EPM = 60.0F;

static constexpr int N_PWM = 3;
static constexpr uint32_t N_EXPECTED_HRC_FOR_HET = 128;
static constexpr float PREFFERED_PWM_FREQUENCY = 20'000.0F;
static constexpr uint32_t DEFAULT_FOC_TICKS_FOR_SPEED = 10;
static constexpr uint32_t DEFAULT_PWM_TICKS_FOR_FOC = 1;

/* value 10 lead to 10% static error */
static constexpr int MINIMUM_DELTA_POSITIONS_TO_COUNT_ANGLE_SPEED = 50;

static constexpr uint32_t MAXIMUM_PWM_CYCLES_FOR_UPDATE_SPEED = DEFAULT_FOC_TICKS_FOR_SPEED;

static PinInterface* enGate;
static PinInterface* hallSensors[3];
static float nEncoderPulses; /* for reduce calculations */
static float nPolePairs;
static float realPwmFrequency_Hz;
static float realFocFrequency_Hz;
static float requestedStatorSpeed_Hz;
static float generatedStatorAngleSpeed_Hz;
static int32_t requestedStatorSpeed_epm;
static int32_t generatedStatorAngleSpeed_epm;
static float angleSpeed_epm;
static float angleSpeedFiltered_epm;
static float kForMeasureAngleSpeed_epm; /* constant to reduce calculations */
static float targetId_A;
static float targetIq_A;
static float targetAngleSpeed_epm;
static float betaAxisVoltageVectorCoordinate = 0.0F;
static float alfaAxisVoltageVectorCoordinate = 0.0F;
static float qAxisVoltageVectorCoordinate = 0.0F;
static float dAxisVoltageVectorCoordinate = 0.0F;
static trajectory::Trajectory* speedTrajectory;
static bool flag1ms;
static float torque;
static WORK_MODE workMode;

static int tableLinkLogChannelsAndStreams[4];

static uint32_t counterFocTicksForSpeedLoop;
static uint32_t counterPwmTicksForFocLoop;
static uint32_t nFocTicksForSpeedLoop;
static uint32_t base;

/* [0]: summary position; [1]: current position */
static volatile uint32_t encoderPositions[2];
static uint32_t lastSavedEncoderPosition;
static int counterTimeoutFromEncoder = 0;

static float thetaElectricalAngleGenerator_pu;
static float thetaElectricalEncoder_pu;
static float thetaMechanicalEncoder_pu;

static ::ipark::Ipark inversePark;
static ::svgen::Svgen out;
static MATH_vec2 Vdq;
static MATH_vec2 IacDirty;
static MATH_vec2 IacFiltered;
static MATH_vec2 Idq;
static MATH_vec2 Is_alfa_beta;
static MATH_vec2 phasorEncoder;
static MATH_vec2 phasorAngleGenerator;
static PiRegul* piId;
static PiRegul* piIq;
static PiRegul* piAngleSpeed;
static het::HET nh;


static uint32_t* ptrRealPwmCounter     = nullptr;
static uint32_t* ptrRealMedium         = nullptr;
static uint32_t* ptrRequestPwmCounter  = nullptr;
static uint32_t* ptrRequestMedium      = nullptr;
static uint32_t* ptrUpdatePwm          = nullptr;
static uint32_t  valueRequestMedium = 0;

static struct Pwm* pwms[N_PWM] = {nullptr};



struct Pwm {
    uint32_t& requestPwmDuty;
    uint32_t& realStage1;
    uint32_t& realStage2;
    Pwm(const uint32_t addrReq,
        const uint32_t addrRealStage1,
        const uint32_t addrRealStage2,
        const uint8_t npin) :
            requestPwmDuty(het::getDataFieldReference(nh, addrReq)),
            realStage1(het::getDataFieldReference(nh, addrRealStage1)),
            realStage2(het::getDataFieldReference(nh, addrRealStage2))
    {
        requestPwmDuty = 0;
        realStage1 = valueRequestMedium;
        realStage2 = valueRequestMedium;
        het::updatePinNumberInControlWordForMcmpInstruction(
                nh,
                addrRealStage1,
                npin);
        het::updatePinNumberInControlWordForMcmpInstruction(
                nh,
                addrRealStage2,
                npin);
        het::setupPinForOutput(nh, npin);
    }
    void updateDuty(const uint32_t d) {requestPwmDuty = d;}
};







static void requestToUpdatePwm()
{
    /* младшие 7 бит игнорируются инструкцией DJZ,
     * с помощью которой выполняется синхронное обновление */
    constexpr uint32_t datafieldValueForOnceUpdate = 0x80;
    *ptrUpdatePwm = datafieldValueForOnceUpdate;
}


static float setPrefferedFrequencyAndReturnRealFrequency(const float freq)
{
    const auto vclk2Mhz = vclk2::getFreqMhz();
    const auto countedFullCycleNumber =
            het::getLowerEvenHetCyclesNumberForFrequency(nh, freq, vclk2Mhz);
    const uint32_t halfScale = countedFullCycleNumber * N_EXPECTED_HRC_FOR_HET
            / 2;
    *ptrRequestPwmCounter = countedFullCycleNumber - 1;
    valueRequestMedium = halfScale;
    *ptrRequestMedium = halfScale;
    return het::countFreqencyForHetCyclesNumber(
            nh,
            countedFullCycleNumber,
            vclk2Mhz);
}



static uint32_t addrRealValueCnt()             {return base;}
static uint32_t addrUpdatePwm()                {return base + 6;}
static uint32_t addrRequestM1()                {return base + 3;}
static uint32_t addrRequestM2()                {return base + 4;}
static uint32_t addrRequestM3()                {return base + 5;}
static uint32_t addrRequestValueCnt()          {return base + 19;}
static uint32_t addrRequestMediumValueCnt()    {return base + 20;}
static uint32_t addrRealMediumValueCnt()       {return base + 21;}
static uint32_t addrRealM1stage1()             {return base + 22;}
static uint32_t addrRealM2stage1()             {return base + 23;}
static uint32_t addrRealM3stage1()             {return base + 24;}
static uint32_t addrRealM1stage2()             {return base + 25;}
static uint32_t addrRealM2stage2()             {return base + 26;}
static uint32_t addrRealM3stage2()             {return base + 27;}

/* must be with sequential increment addresses */
static uint32_t addrSummaryEncoderPosition()   {return base + 28;}
static uint32_t addrRealEncoderPosition()      {return base + 29;}

static uint32_t addrDecrementWithOverflowEncoderPositoin() {return base + 42;}


static void setBaseAddressForMotorBlock(const uint32_t startAddr)
{
    base = startAddr;
}


static void updatePwm()
{
/* slow working code */
    for (int i = 0; i < 3; ++i) {
        pwms[i]->updateDuty(valueRequestMedium * out.value[i]);
    }
    requestToUpdatePwm();
}


static void runPiParallelController()
{
    targetIq_A = pi::runParallel(
            piAngleSpeed,
            targetAngleSpeed_epm,
            angleSpeed_epm);
}



static void speedLoop()
{
    runPiParallelController();
}

static void count1ms()
{
    ++counterFocTicksForSpeedLoop;
    flag1ms = counterFocTicksForSpeedLoop >= nFocTicksForSpeedLoop;
    if (flag1ms) {
        counterFocTicksForSpeedLoop = 0;
    }
}


/*
Generated by:   http://www-users.cs.york.ac.uk/~fisher/mkfilter
Summary
You specified the following parameters:
filtertype  =   Bessel
passtype    =   Lowpass
ripple  =
order   =   1
samplerate  =   20680.1465
corner1 =   1
corner2 =
adzero  =
logmin  =
 */
static constexpr uint32_t NZEROS = 1;
static constexpr uint32_t NPOLES = 1;
static float xv[NZEROS+1], yv[NPOLES+1];
static float filter(const float w)
{
    constexpr float GAIN = 6.583695028e+03F;
    xv[0] = xv[1];
    xv[1] = w / GAIN;
    yv[0] = yv[1];
    yv[1] = xv[0] + xv[1] + 0.9996962192F * yv[0];
    return yv[1];
}


static float countAngleSpeed(const int d, const uint32_t t)
{
    return static_cast<float>(d) * kForMeasureAngleSpeed_epm
            / static_cast<float>(t);
}


static bool isDeltaPositionEnoughBig(const int d)
{
    return abs(d) >= MINIMUM_DELTA_POSITIONS_TO_COUNT_ANGLE_SPEED;
}


static bool isDeltaTimeEnoughBig(const uint32_t t)
{
    return t >= MAXIMUM_PWM_CYCLES_FOR_UPDATE_SPEED;
}


static void saveCurrentEncoderPositionForNextUse(const uint32_t p)
{
    lastSavedEncoderPosition = p;
}


static void resetDeltaTimeCounterForEncoder()
{
    counterTimeoutFromEncoder = 0;
}


static void increaseDeltaTimeCounterForEncoder()
{
    ++counterTimeoutFromEncoder;
}

static uint32_t getDeltaTimeCounterForEncoder()
{
    return counterTimeoutFromEncoder;
}

static uint32_t getSummaryEncoderPosition()
{
    return encoderPositions[0];
}


static uint32_t getCurrentEncoderPosition()
{
    return encoderPositions[1] >> 7;
}


static float getCurrentEncoderPosition_pu()
{
    return static_cast<float>(getCurrentEncoderPosition()) / nEncoderPulses;
}


static int countDeltaEncoderPosition(const uint32_t currentPosition)
{
    return currentPosition - lastSavedEncoderPosition;
}


static void processAngleSpeedMeasure()
{
    increaseDeltaTimeCounterForEncoder();
    const auto p = getSummaryEncoderPosition();
    const auto d = countDeltaEncoderPosition(p);
    const auto t = getDeltaTimeCounterForEncoder();
    if (isDeltaPositionEnoughBig(d) || isDeltaTimeEnoughBig(t)) {
        angleSpeed_epm = countAngleSpeed(d, t);
        saveCurrentEncoderPositionForNextUse(p);
        resetDeltaTimeCounterForEncoder();
    }
    angleSpeedFiltered_epm = filter(angleSpeed_epm);
}


static float countIsModule()
{
    const float v1 = Is_alfa_beta.value[0] * Is_alfa_beta.value[0];
    const float v2 = Is_alfa_beta.value[1] * Is_alfa_beta.value[1];
    return sqrtf(v1 + v2);
}


static float getStreamValueForLogChannel(const int nChannel)
{
    const auto nStream = tableLinkLogChannelsAndStreams[nChannel];
    switch (nStream) {
        case 0:     return IacDirty.value[0];
        case 1:     return -IacDirty.value[0]-IacDirty.value[1];
        case 2:     return IacDirty.value[1];
        case 3:     return Idq.value[0];
        case 4:     return Idq.value[1];
        case 5:     return targetId_A;
        case 6:     return targetIq_A;
        case 7:     return Vdq.value[0];
        case 8:     return Vdq.value[1];
        case 9:     return thetaMechanicalEncoder_pu;
        case 10:    return thetaElectricalEncoder_pu;
        case 11:    return thetaElectricalAngleGenerator_pu;
        case 12:    return torque;
        case 13:    return Is_alfa_beta.value[0];
        case 14:    return Is_alfa_beta.value[1];
        case 15:    return countIsModule();
        case 16:    return targetAngleSpeed_epm;
        case 17:    return angleSpeed_epm;
        case 18:    return angleSpeedFiltered_epm;
        case 19:    return pi::getIntegrator(piAngleSpeed);
        default:    return 0.0F;
    }
}


static void doLog()
{
    using namespace logpoint;
    const auto v1 = getStreamValueForLogChannel(0);
    const auto v2 = getStreamValueForLogChannel(1);
    const auto v3 = getStreamValueForLogChannel(2);
    const auto v4 = getStreamValueForLogChannel(3);
    const LogPoint p = {v1, v2, v3, v4};
    put(p);
}


static void countIacDirty(const float i1, const float i3)
{
    IacDirty = {i1, i3};
}


static void updateSumOfIacDirty()
{
    for (int i = 0; i < 2; ++i) {
        IacFiltered.value[i] += IacDirty.value[i];
    }
}


static void resetIacFiltered()
{
    constexpr MATH_vec2 zeroVec2 = {0.0F, 0.0F};
    IacFiltered = zeroVec2;
}


static void increaseCounterForFocLoop()
{
    ++counterPwmTicksForFocLoop;
}


static bool isTimeToRunFoc()
{
    return counterPwmTicksForFocLoop >= DEFAULT_PWM_TICKS_FOR_FOC;
}


static void resetCounterForFocLoop()
{
    counterPwmTicksForFocLoop = 0;
}


static void countIacFiltered()
{
    for (int i = 0; i < 2; ++i) {
        IacFiltered.value[i] /=  static_cast<float>(DEFAULT_PWM_TICKS_FOR_FOC);
    }
}


static void countThetaMechanicalEncoder()
{
    thetaMechanicalEncoder_pu = getCurrentEncoderPosition_pu();
}


static void countThetaElectricalEncoder()
{
    const float a1 = thetaMechanicalEncoder_pu * nPolePairs;
    const int a2 = static_cast<int>(a1);

    thetaElectricalEncoder_pu = a1 - static_cast<float>(a2);
}


static void countTorque()
{
    constexpr float SCALE = 1.0F;
    torque = Idq.value[1] * SCALE;
}


static void countIs_alfa_beta()
{
    Is_alfa_beta = clarke::run(IacFiltered);
}


static void countPhasorAngleGenerator()
{
    phasorAngleGenerator = phasor::createFrom_pu(thetaElectricalAngleGenerator_pu);
}

static void countThetaElectricalAngleGenerator()
{
    angle_gen::run();
    thetaElectricalAngleGenerator_pu = angle_gen::get_pu();
}

static void countPhasorEncoder()
{
    phasorEncoder = phasor::createFrom_pu(thetaElectricalEncoder_pu);
}


static void countIdq()
{
    Idq = park::run(phasorEncoder, Is_alfa_beta);
}


static void countSpacePwmValue()
{
    out = ::svgen::run(inversePark);
}


static void runPiCurrentControllers()
{
    const auto Id = Idq.value[0];
    const auto Iq = Idq.value[1];
    const auto Vd_pu = pi::runSeries(piId, targetId_A, Id);
    const auto Vq_pu = pi::runSeries(piIq, targetIq_A, Iq);
    Vdq = {Vd_pu, Vq_pu};
    inversePark = ::ipark::run(phasorEncoder, Vdq);
}



static void runTrajSpeed()
{
    if (flag1ms) {
        trajectory::setTargetValue(speedTrajectory, requestedStatorSpeed_Hz);
        trajectory::run(speedTrajectory);
        const auto newStatorAngleSpeed_Hz = trajectory::getIntermediateValue(
                speedTrajectory);
        if (newStatorAngleSpeed_Hz != generatedStatorAngleSpeed_Hz) {
            generatedStatorAngleSpeed_Hz = newStatorAngleSpeed_Hz;
            generatedStatorAngleSpeed_epm = roundf(
                    generatedStatorAngleSpeed_Hz * SCALE_HZ_TO_EPM);
            angle_gen::setElectricalFrequency_Hz(generatedStatorAngleSpeed_Hz);
        }
    }
}


static void dispatchMotorControlMode()
{
    switch (workMode) {
        case MODE_ENCODER_CALIBRATION:
            inversePark = {ENCODER_CALIBRATION_VOTAGE_PU, 0.0F};
            break;

        case MODE_CLOSE_SPEED_LOOP:         /* no break */
            if (flag1ms) {
                speedLoop();
            }
            /* no break */

        case MODE_CLOSE_CURRENT_LOOP:
        	runPiCurrentControllers();
        	break;


        case MODE_TRANSLATE_V_ALPHA_BETA:
            inversePark = {
                    alfaAxisVoltageVectorCoordinate,
                    betaAxisVoltageVectorCoordinate};
            break;

        case MODE_GENERATE_ANGLE:
            runTrajSpeed();
            countThetaElectricalAngleGenerator();
            countPhasorAngleGenerator();
            Vdq = {dAxisVoltageVectorCoordinate, qAxisVoltageVectorCoordinate};
            inversePark = ::ipark::run(phasorAngleGenerator, Vdq);
            break;

        case MODE_OFF: 						/* no break */
        case MODE_TRANSLATE_V_DQ:			/* no break */
        default:
            inversePark = {0.0F, 0.0F};
            break;
    }
}


static void handler(const float i1, const float i3)
{
    countIacDirty(i1, i3);
    updateSumOfIacDirty();
    increaseCounterForFocLoop();

    if (isTimeToRunFoc()) {
        resetCounterForFocLoop();
        countIacFiltered();
        countIs_alfa_beta();
        count1ms();
        processAngleSpeedMeasure();
        countThetaMechanicalEncoder();
        countThetaElectricalEncoder();
        countPhasorEncoder();
        countIdq();
        countTorque();

        dispatchMotorControlMode();

        countSpacePwmValue();
        updatePwm();
        resetIacFiltered();
    }

    if (logpoint::isNeedToDoLog()) {
        doLog();
    }
}



static void setHetNoForMotor(het::HET nHET)
{
    nh = nHET;
}


static void initPointersToWorkWithMotorBlockInHetProgram()
{
    ptrRealPwmCounter = &het::getControlFieldReference(
            nh, addrRealValueCnt());
    ptrRealMedium = &het::getDataFieldReference(
            nh, addrRealMediumValueCnt());
    ptrUpdatePwm = &het::getDataFieldReference(nh, addrUpdatePwm());
    ptrRequestPwmCounter = &het::getControlFieldReference(
            nh, addrRequestValueCnt());
    ptrRequestMedium = &het::getDataFieldReference(
            nh, addrRequestMediumValueCnt());
}


static void setRealPwmFrequency()
{
    realPwmFrequency_Hz = setPrefferedFrequencyAndReturnRealFrequency(
            PREFFERED_PWM_FREQUENCY);
}


static void setRealFocLoopFrequency()
{
    realFocFrequency_Hz = realPwmFrequency_Hz
            / static_cast<float>(DEFAULT_PWM_TICKS_FOR_FOC);
}


static void updateRealPwmCounterInHetProgram()
{
    *ptrRealPwmCounter = *ptrRequestPwmCounter;
}


static void updateRealMediumCompareValueInHetProgram()
{
    *ptrRealMedium = *ptrRequestMedium;
}


static void createMotorPwms(const PinsMotorInHet& mpins)
{
    pwms[M1] = new Pwm(addrRequestM1(), addrRealM1stage1(),
                       addrRealM1stage2(), mpins.m1);
    pwms[M2] = new Pwm(addrRequestM2(), addrRealM2stage1(),
                       addrRealM2stage2(), mpins.m2);
    pwms[M3] = new Pwm(addrRequestM3(), addrRealM3stage1(),
                       addrRealM3stage2(), mpins.m3);
}


/*
 * CTRL1 = 0x1018 -> 0 (W) + 0010 (D14..D11) + 00000 01 1 0 00 (D10..D0)
 *   GATE_CURRENT = 00 -> 1.7A
 *   GATE_RESET = 0 -> normal mode
 *   PWM_MODE = 1 -> 3 pwm inputs
 *   OCP_MODE = 01 -> (01: OC latch shutdown, 00: current limit)
 *   OC_ADJ_SET = 00000 = 0 -> 0.060V -> for SUM110N06-3m9H @ T=25C Ipeak=15.4A
 * CTRL2 = 0x180D -> 0 (W) + 0011 (D14..D11) + 0000 0 0 0 11 01 (D10..D0)
 *   OCTW_MODE = 1 -> report OT only
 *   GAIN = 3 -> 80 V/V
 *   DC_CAL_CH1 = 0
 *   DC_CAL2_CH2 = 0
 *   OC_TOFF = cycle by cycle
 */
static void writeControlRegistersToDrv8301()
{
    constexpr uint16_t CTRL1 = 0x1018;
    constexpr uint16_t CTRL2 = 0x180D;

    softspi::doExchange16bit(CTRL1);
    softspi::doExchange16bit(CTRL2);
}


/*
 * RTI must work before calling this function because of use time delays
 */
static void setupMotorDriver()
{
    softspi::create(
            pin::createHetPin(HET1, 30),
            pin::createHetPin(HET1, 20),
            pin::createHetPin(HET1, 15),
            pin::createHetPin(HET1, 31));
    enGate = pin::createGioPin(getPortA(), 4); /* GPIOA.4 as EN_GATE */
    enGate->initHw();
    enGate->setDirectionOutput();
    enGate->off();
    waitus(30); /* time of low EN_GATE to do full reset DRV8301 >= 20 us */
    enGate->on();
    waitms(10); /* SPI ready after EN_GATE transitions to HIGH <= 10 ms */
    writeControlRegistersToDrv8301(); /* here is setup gain for DRV8301 */
}



/*
 * setup control field of CNT instruction and data field of ADM32 instruction
 * for working with encoder; see HET program for additional information
 */
static void setCorrectEncoderLinesInHetProgram()
{
    const uint32_t dataFieldForEncoderLines = N_ENCODER_LINES * 4 - 1;
    auto& cnt = het::getControlFieldReference(nh, addrRealEncoderPosition());
    auto& adm32 = het::getDataFieldReference(
            nh,
            addrDecrementWithOverflowEncoderPositoin());
    cnt &= 0xF7000000;
    cnt |= dataFieldForEncoderLines;
    adm32 &= 0x7F;
    adm32 |= dataFieldForEncoderLines << 7; /* data field is in [31..7] bits */
}



static void setupCalibrationOffsetPin()
{
    PinInterface* p = ::pin::createGioPin(getPortB(), 4);
    p->off();
    delete p;
}


static void setDefaultPwmForMotorWindings()
{
	out = {0.5F, 0.5F, 0.5F};
	updatePwm();
}


static void initPinsForHallsSensors()
{
    hallSensors[0] = ::pin::createHetPin(het::HET1, 13); /* NHET1_13 as CAP1 */
    hallSensors[1] = ::pin::createHetPin(het::HET1, 17); /* NHET1_17 as CAP2 */
    hallSensors[2] = ::pin::createGioPin(getPortB(), 5); /* GIOB5 as CAP3 */

    for (auto& v : hallSensors) {
        v->initHw();
        v->setDirectionInput();
    }
}


/* configure DCP0, only CP A, for one trigger request */
static void configureDcp0()
{
    /* инструкция #N: N * 4 + 2 = 0xXX; номер инструкции умножается на число
     * слов в инструкции и потом добавляется смещение для поля данных */
    const uint32_t ADDRESS_SUMMARY_ENCODER_POSITION_DATAFIELD =
            addrSummaryEncoderPosition() * 4 + 2;

    auto& htuRam = getHtu1Ram();
    auto& dcp0 = htuRam.DCP[0];
    dcp0.IFADDRA = reinterpret_cast<uint32_t>(&encoderPositions);

    /* 2 elements in 1 frame are transfered after HTU request */
    dcp0.ITCOUNT = 0x20001;

    dcp0.IHADDRCT =
      (0 << 23) /* DIR = 0, from HET to RAM */
      | (0 << 22) /* SIZE = 0, transfer 32 bit */
      | (0 << 21) /* ADDMH = 0; increment 16 bytes */
      | (0 << 20) /* ADDMF = 0, do post-increment after each element transfer */
      | (1 << 18) /* TMBA = 0, circullar buffer mode for buffer A */
      | (ADDRESS_SUMMARY_ENCODER_POSITION_DATAFIELD << 2);
}


static void initHtu()
{
    auto& htu = getHtu1();
    htu.GC = 1;
    while (htu.GC & 1) { /* wait for end HTU1 reset */
        ;
    }
    configureDcp0();
    htu.CPENA = (1 << 0); /* CP A for DCP#0 is enabled */
    htu.GC = 1 << 16; /* enable HTU1 operation */
    auto& het1 = getHet1();
    het1.REQENS = 0x01; /* enable requests #0 */
    het1.REQDS = 0; /* use only HTU for requests, not DMA */
}


static void setupDefaultLinkBetweenLogChannelsAndStreams()
{
    tableLinkLogChannelsAndStreams[0] = 5;
    tableLinkLogChannelsAndStreams[1] = 3;
    tableLinkLogChannelsAndStreams[2] = 6;
    tableLinkLogChannelsAndStreams[3] = 4;
}


static void setupDefaultTrajectorySpeed()
{
    constexpr float MAX_SPEED_HZ = MAX_SPEED_EPM / SCALE_HZ_TO_EPM;
    constexpr float DEFAULT_ACCELERATION_HZ = DEFAULT_ACCELERATION_EPM
            / SCALE_HZ_TO_EPM;

    trajectory::setIntValue(speedTrajectory, 0.0F);
    trajectory::setMaxValue(speedTrajectory, MAX_SPEED_HZ);
    trajectory::setMinValue(speedTrajectory, -MAX_SPEED_HZ);
    trajectory::setTargetValue(speedTrajectory, 0.0F);
    set::accelerationForElectricalAngleSpeed(DEFAULT_ACCELERATION_HZ);
}





void create(
        const het::HET nHet,
        const uint32_t startAddr,
        const PinsMotorInHet& mpins)
{
    nFocTicksForSpeedLoop = DEFAULT_FOC_TICKS_FOR_SPEED;

    setHetNoForMotor(nHet);

    setBaseAddressForMotorBlock(startAddr);
    initPointersToWorkWithMotorBlockInHetProgram();
    setRealPwmFrequency();
    setRealFocLoopFrequency();

    requestedStatorSpeed_Hz = 0.0F;
    speedTrajectory = trajectory::create();
    setupDefaultTrajectorySpeed();
    angle_gen::setRunPeriod_s(1.0F / realFocFrequency_Hz);

    updateRealPwmCounterInHetProgram();
    updateRealMediumCompareValueInHetProgram();
    createMotorPwms(mpins);

    nEncoderPulses = static_cast<float>(N_ENCODER_LINES * 4);
    nPolePairs = N_MOTOR_POLUS_PAIRS;
    applyMotorTypeParameters();

    het::turnHetOn(nh);

    setupMotorDriver(); /* here is setup gain for DRV8301 */
    resetMotorDriver();
    setupCalibrationOffsetPin();

    constexpr float GAIN_EXT = 20.0F;
    motorCurrent::create(GAIN_EXT);

    motorCurrent::setCallbackOnConversationDoneIrq(handler);
    setDefaultPwmForMotorWindings();

    kForMeasureAngleSpeed_epm = realFocFrequency_Hz * SCALE_HZ_TO_EPM
            / nEncoderPulses;
    initPinsForHallsSensors();
    initHtu();

    setupDefaultLinkBetweenLogChannelsAndStreams();

    piId = pi::create();
    piIq = pi::create();
    pi::setGains(piId, KP_CURRENT_REGULATORS, KI_CURRENT_REGULATORS);
    pi::setGains(piIq, KP_CURRENT_REGULATORS, KI_CURRENT_REGULATORS);
    pi::setMinMax(piId, -1.0F, 1.0F);
    pi::setMinMax(piIq, -1.0F, 1.0F);
    targetId_A = 0.0F;
    targetIq_A = 0.0F;

    piAngleSpeed = pi::create();
    pi::setGains(piAngleSpeed, KP_SPEED_REGULATOR, KI_SPEED_REGULATOR);
    pi::setMinMax(piAngleSpeed, -MAXIMUM_CURRENTS_A, MAXIMUM_CURRENTS_A);

    motorNew::set::runMode(motorNew::MODE_ENCODER_CALIBRATION);
    waitms(500); /* 500 ms to move rotor to default position 0 degree */
    motorNew::set::encoderPosition_int(0);
    motorNew::set::runMode(motorNew::MODE_OFF);
}



void disableMotorDrive()
{
    enGate->off();
}


/* use quick fault reset mode without resetting control registers to theirs
*  default values, so no need to reprogram control registers using SPI after
*  calling this function
*/
void resetMotorDriver()
{
	const irq::IrqGuard g;
	enGate->off();
	enGate->on();
}


void applyMotorTypeParameters()
{
    setCorrectEncoderLinesInHetProgram();
}


namespace set {

void encoderPosition_int(const uint32_t newPosition)
{
    auto& data = het::getDataFieldReference(nh, addrRealEncoderPosition());
    data = newPosition << 7; /* data for counter: bits 31..8 in datafield */
}


void linkLogChannels(const int nChannel, const int nStream)
{
    tableLinkLogChannelsAndStreams[nChannel] = nStream;
}


void runMode(const WORK_MODE wm)
{
    workMode = wm;
    if (wm == MODE_OFF) {
        pi::setIntegrator(piAngleSpeed, 0.0F);
        pi::setIntegrator(piIq, 0.0F);
        pi::setIntegrator(piId, 0.0F);
    }
}


void Iq(const float iq_A)
{
	targetIq_A = iq_A;
}


void Id(const float id_A)
{
    targetId_A = id_A;
}

void angleSpeed_epm(const float w)
{
    targetAngleSpeed_epm = w;
}

void angleSpeedKi(const float ki)
{
    pi::setIntegralGain(piAngleSpeed, ki);
}


void angleSpeedKp(const float kp)
{
    pi::setProportionalGain(piAngleSpeed, kp);
}


void debugVoltageBetaAxis(const float voltage)
{
    betaAxisVoltageVectorCoordinate = voltage;
}


void debugVoltageAlfaAxis(const float voltage)
{
    alfaAxisVoltageVectorCoordinate = voltage;
}

void debugVoltageQAxis(const float v)
{
    qAxisVoltageVectorCoordinate = v;
}


void debugVoltageDAxis(const float v)
{
    dAxisVoltageVectorCoordinate = v;
}


void accelerationForElectricalAngleSpeed(const float a)
{
    const float delta = a * nFocTicksForSpeedLoop / realFocFrequency_Hz;
    trajectory::setMaxDelta(speedTrajectory, delta);
}

void ectricalAngleSpeed_Hz(const float angleSpeed)
{
    requestedStatorSpeed_Hz = angleSpeed;
    requestedStatorSpeed_epm = roundf(angleSpeed * SCALE_HZ_TO_EPM);
}


} /* namespace set */




namespace get {


bool hall(const HALLS h)
{
    return hallSensors[h]->getState();
}


/*
 * Link between windings and halls is describing in "hall sector"
 * Determined for clockwise direction, with generated angle_pu from 0 to 1
 * (angle from 0 to 360):
 * angle H1 H2 H3 sector
 *     0  1  0  0      1
 *    60  1  1  0      2
 *   120  0  1  0      3
 *   180  0  1  1      4
 *   240  0  0  1      5
 *   300  1  0  1      6
 */
HALL_SECTOR hallSector(const bool h1, const bool h2, const bool h3)
{
    if (h1 == true && h2 == false && h3 == false) {
        return SECTOR1;
    }
    if (h1 == true && h2 == true && h3 == false) {
        return SECTOR2;
    }
    if (h1 == false && h2 == true && h3 == false) {
        return SECTOR3;
    }
    if (h1 == false && h2 == true && h3 == true) {
        return SECTOR4;
    }
    if (h1 == false && h2 == false && h3 == true) {
        return SECTOR5;
    }
    return SECTOR6;
}


uint32_t encoderPosition_int()
{
    return getCurrentEncoderPosition();
}


float encoderPosition_pu()
{
    return thetaMechanicalEncoder_pu;
}


uint32_t summaryEncoderPosition()
{
    return getSummaryEncoderPosition();
}


float hallPosition_pu()
{
    const auto h1 = hall(H1);
    const auto h2 = hall(H2);
    const auto h3 = hall(H3);
    const auto s = hallSector(h1, h2, h3);
    switch (s) {
        case SECTOR1: return 0.0F   / 360.0F;
        case SECTOR2: return 60.0F  / 360.0F;
        case SECTOR3: return 120.0F / 360.0F;
        case SECTOR4: return 180.0F / 360.0F;
        case SECTOR5: return 240.0F / 360.0F;
        case SECTOR6: return 300.0F / 360.0F;
        default: return 0.0F;
    }
}


float measuredAngleSpeed_epm()
{
    return angleSpeed_epm;
}


float angleSpeedKi() {return pi::getIntegralGain(piAngleSpeed);}
float angleSpeedKp() {return pi::getProportionalGain(piAngleSpeed);}


float measuredAngleSpeedFiltered_epm()
{
    return angleSpeedFiltered_epm;
}


int nStreamForLogChannel(const int n)
{
    return tableLinkLogChannelsAndStreams[n];
}

} /* namespace get */





} /* namespace motorNew */


