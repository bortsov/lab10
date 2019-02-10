#include "main.h"

#include "sys_core.h"
#include "sys_pmu.h"
#include "pinmux.h"

#include "jlink.h"
#include "HW.h"
#include "monitorCmd.h"
#include "init.h"
#include "rti.h"
#include "het1program.h"
#include "SciLog.h"
#include "testpins.h"
#include "internalAdc1.h"
#include "LogPoint.h"

#include "Motor/motorNew.h"
#include "Motor/MotorCurrentMeasure.h"
#include "Motor/controller_data_types.h"


static constexpr int PERIOD_LOG_CURRENTS_MS = 1;

static bool flagLogCurrents  = false;
static int cntLogMotor = 0;
static float avgExt_ia;
static float avgExt_ic;


static void processLogCurrents()
{
    if (flagLogCurrents) {
        avgExt_ia += motorCurrent::getCurrent(motorCurrent::EXT_IA);
        avgExt_ic += motorCurrent::getCurrent(motorCurrent::EXT_IC);
        ++cntLogMotor;
        if (cntLogMotor >= PERIOD_LOG_CURRENTS_MS) {
            avgExt_ia /= static_cast<float>(cntLogMotor);
            avgExt_ic /= static_cast<float>(cntLogMotor);
            cntLogMotor = 0;
            constexpr float SCALE_A_TO_MA = 1000.0F;
            logSci::log9(
                    static_cast<int32_t>(avgExt_ia * SCALE_A_TO_MA),
                    static_cast<int32_t>(avgExt_ic * SCALE_A_TO_MA)
                    );
            avgExt_ia = 0.0F;
            avgExt_ic = 0.0F;
        }
    }
}


static void doBigCycle(uart::Uart& cb)
{
	mon::setBlueFont(cb);
	cb << "\nЗапуск целевой программы!\n";
	mon::setNormalFont(cb);
    for (;;) {
        mcal::rti::resetFlagSystick();
        while (mcal::rti::isFlagSystick() == false) {
            jlink::process();
            mon::taskBody();
        }
        mcal::internalAdc1::process();
        processLogCurrents();
    }
}



static void registerUsedMonitorCommands(uart::Uart& cb)
{
    mon::registerCmd(mon::drv8301::getCommand());
    mon::registerCmd(mon::showcurrents::getCommand());

    mon::registerCmd(mon::menable::getCommand());
    mon::registerCmd(mon::mlogcnt::getCommand());
    mon::registerCmd(mon::mchl::getCommand());
    mon::registerCmd(mon::mlogsave::getCommand());
    mon::registerCmd(mon::mlogshow::getCommand());
    mon::registerCmd(mon::mmode::getCommand());
    mon::registerCmd(mon::mw::getCommand());
    mon::registerCmd(mon::mwki::getCommand());
    mon::registerCmd(mon::mwkp::getCommand());
    mon::registerCmd(mon::mwon::getCommand());
    mon::registerCmd(mon::mva::getCommand());
    mon::registerCmd(mon::mvb::getCommand());
    mon::registerCmd(mon::mvd::getCommand());
    mon::registerCmd(mon::mvq::getCommand());
    mon::registerCmd(mon::mf::getCommand());
}



static void initFirstStage()
{
    init::peripheral();
    init::vimSetAllHandlerToPhantomInterrupt();
    init::rti();
}


static void initSecondStage()
{
    init::pll();
    init::pinmux();
    init::flash();
    init::trimLpo();
    init::mapClocks();
    init::vim();
    init::internalAdc1();
}


static void enableIrq()
{
#if 1 /* включение автовекторизации IRQ (FIQ не имеет такой возможности) */
/* для использования автовекторизации IRQ требуется:
 * 1. использование обработчиков IRQ и FIQ, скомпилированных
 *    в ARM режиме __attribute__((target("arm")))
 * 2. наличие __attribute__((interrupt("IRQ"))) в объявлении обработчиков IRQ
 *    п.1 и 2 можно объединить:
 *    static __attribute__((target("arm"), interrupt("IRQ"))) void handler(){}
 * 3. вызов _coreEnableIrqVicOffset_();
 * 4. наличие ldr pc,[pc,#-0x1b0] в обработчике FIQ по адресу 0x1C
 *    (если нужен FIQ)
 * 5. желательно также наличие ldr pc,[pc,#-0x1b0] в обработчике IRQ
 *    по адресу 0x18; это спасёт ситуацию если возникнет прерывание IRQ до
 *    включения автовекторизации.
 */
    _coreEnableIrqVicOffset_();
#else /* отключение автовекторизации */
/* Возможно упростить код двумя действиями:
 * 1. отказаться от __attribute__((interrupt("IRQ"))) и
 *    __attribute__((interrupt("FIQ")))
 * 2. использовать во всём коде режим THUMB
 * Для этого следует:
 * 1. отключить автовекторизацию (не вызывать _coreEnableIrqVicOffset_(); )
 * 2. использовать ассемблерные "обёртки" для IRQ, FIQ, которые производят
 *    сохранение используемых регистров, переключают режим работы процессора и
 *    вызывают пользовательские обработчики, которые могут быть скомпилированы
 *    как в ARM так и в THUMB режиме.
 *    Вызов обёрток _isrStub, _fiqStub прямо прописывается в таблице векторов
 *    прерываний.
 *
 */
#endif
    _enableIrqInterrupt_();
}






int main(void)
{
    initFirstStage();
    jlink::create();
    uart::Uart& debugCommunicaton = *jlink::get(jlink::TERMINAL);
    mon::create(&debugCommunicaton);
    hw::create();
    testpins::create();
    initSecondStage();
    enableIrq();
#if 0 /* uncomment if need to use FIQ. There may be problem with phantom IRQ (from previous blocks wich used FIQ) if program loads to ram */
    _enableFiqInterrupt_();
#endif

    _pmuInit_();
    constexpr uint32_t c0 = PMU_LSU_BUSY_STALL;
    constexpr uint32_t c1 = PMU_INST_BUFFER_STALL;
    constexpr uint32_t c2 = PMU_DATA_DEPENDENCY_INST_STALL;
    _pmuSetCountEvent_(0, c0);
    _pmuSetCountEvent_(1, c1);
    _pmuSetCountEvent_(2, c2);

    logpoint::create();
    het::create(het::HET1,
            128,
            (const char* )HET_INIT0_PST,
            sizeof(HETPROGRAM0_UN),
            irq::IRQ);
    motorNew::create(het::HET1, 0, {8, 4, 0});

    registerUsedMonitorCommands(debugCommunicaton);

    logSci::create();

    doBigCycle(debugCommunicaton);

    return 0;
}


void toggleLogCurrents()
{
    flagLogCurrents = !flagLogCurrents;
}


void disableLogCurrents()
{
    flagLogCurrents = false;
}

