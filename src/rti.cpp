#include <cstdint>

#include "rti.h"
#include "system.h"
#include "irq.h"

namespace mcal {
namespace rti {

static class Rti* rti = nullptr;

struct rtiBASE_t
{
    uint32_t GCTRL;          /**< 0x0000: Global Control Register   */
    uint32_t TBCTRL;         /**< 0x0004: Timebase Control Register */
    uint32_t CAPCTRL;        /**< 0x0008: Capture Control Register  */
    uint32_t COMPCTRL;       /**< 0x000C: Compare Control Register  */
    struct
    {
        uint32_t FRCx;       /**< 0x0010,0x0030: Free Running Counter x Register         */
        uint32_t UCx;        /**< 0x0014,0x0034: Up Counter x Register                   */
        uint32_t CPUCx;      /**< 0x0018,0x0038: Compare Up Counter x Register           */
        uint32_t   rsvd1;    /**< 0x001C,0x003C: Reserved                                */
        uint32_t CAFRCx;     /**< 0x0020,0x0040: Capture Free Running Counter x Register */
        uint32_t CAUCx;      /**< 0x0024,0x0044: Capture Up Counter x Register           */
        uint32_t   rsvd2[2U]; /**< 0x0028,0x0048: Reserved                                */
    } CNT[2U];               /**< Counter x selection:
                                    - 0: Counter 0
                                    - 1: Counter 1                                       */
    struct
    {
        uint32_t COMPx;      /**< 0x0050,0x0058,0x0060,0x0068: Compare x Register        */
        uint32_t UDCPx;      /**< 0x0054,0x005C,0x0064,0x006C: Update Compare x Register */
    } CMP[4U];               /**< Compare x selection:
                                    - 0: Compare 0
                                    - 1: Compare 1
                                    - 2: Compare 2
                                    - 3: Compare 3                                       */
    uint32_t TBLCOMP;        /**< 0x0070: External Clock Timebase Low Compare Register   */
    uint32_t TBHCOMP;        /**< 0x0074: External Clock Timebase High Compare Register  */
    uint32_t   rsvd3[2U];    /**< 0x0078: Reserved                                       */
    uint32_t SETINTENA;      /**< 0x0080: Set/Status Interrupt Register                  */
    uint32_t CLEARINTENA;    /**< 0x0084: Clear/Status Interrupt Register                */
    uint32_t INTFLAG;        /**< 0x0088: Interrupt Flag Register                        */
    uint32_t   rsvd4;        /**< 0x008C: Reserved                                       */
    uint32_t DWDCTRL;        /**< 0x0090: Digital Watchdog Control Register   */
    uint32_t DWDPRLD;        /**< 0x0094: Digital Watchdog Preload Register */
    uint32_t WDSTATUS;       /**< 0x0098: Watchdog Status Register  */
    uint32_t WDKEY;          /**< 0x009C: Watchdog Key Register  */
    uint32_t DWDCNTR;        /**< 0x00A0: Digital Watchdog Down Counter   */
    uint32_t WWDRXNCTRL;     /**< 0x00A4: Digital Windowed Watchdog Reaction Control */
    uint32_t WWDSIZECTRL;    /**< 0x00A8: Digital Windowed Watchdog Window Size Control  */
    uint32_t INTCLRENABLE;   /**< 0x00AC: RTI Compare Interrupt Clear Enable Register  */
    uint32_t COMP0CLR;       /**< 0x00B0: RTI Compare 0 Clear Register   */
    uint32_t COMP1CLR;       /**< 0x00B4: RTI Compare 1 Clear Register */
    uint32_t COMP2CLR;       /**< 0x00B8: RTI Compare 2 Clear Register  */
    uint32_t COMP3CLR;       /**< 0x00BC: RTI Compare 3 Clear Register  */
};


class Rti : private ::irq::IrqCapable {
    volatile rtiBASE_t& rtiREG1;
    inline void clearInterruptFlagForCompare0() {rtiREG1.INTFLAG = 1;}
    void enableInterruptForCompare0() {rtiREG1.SETINTENA = 1;}
    void setCompare0(uint32_t v) {rtiREG1.CMP[0].COMPx = v - 1; rtiREG1.CMP[0].UDCPx = v - 1;}
    void enableTimer0() {rtiREG1.GCTRL |= 1;}
    void setPrescalerForCounter0(uint32_t ps) {rtiREG1.CNT[0].CPUCx = ps - 1;}
    volatile bool flagSystick;
    uint32_t counterSysticks;
    virtual bool handlerIrq() {
        flagSystick = true;
        ++counterSysticks;
        clearInterruptFlagForCompare0();
        return false;
    }
public:
    Rti() :
                    IrqCapable(irq::IRQ_RTI_COMPARE_0, irq::IRQ),
                    rtiREG1(*reinterpret_cast<rtiBASE_t *>(0xFFFFFC00U)),
                    flagSystick(false),
                    counterSysticks(0)
    {
        constexpr uint32_t interval_us = 1000;
        constexpr float freqRTICLKinMhz = getFreqOsc() / 4.0F;
        getSystem1().RCLKSRC = (1 << 24) | (9 << 16) | (2 << 8); /* RTICLK = OSCIN / 4 */
        setPrescalerForCounter0(freqRTICLKinMhz); /* для получения 32 битного таймера, считающего на частоте 1 МГц; предделитель OSC/4 */
        setCompare0(interval_us);
        enableInterruptForCompare0(); /* необходимо для работы аппаратных блоков, триггерирующихся от RTI, даже если прерывание не достигает ЦПУ */
        enableTimer0();
    }
    uint32_t getFRC0() const {return rtiREG1.CNT[0].FRCx;}
    bool getFlagSystick() const {return flagSystick;}
    bool getCounterSysticks() const {return counterSysticks;}
    void resetFlagSystick() {flagSystick = 0;}
};





void create()                   {rti = new Rti();                   }
uint32_t get()                  {return rti->getFRC0();             }
bool isFlagSystick()            {return rti->getFlagSystick();      }
void resetFlagSystick()         {rti->resetFlagSystick();           }
uint32_t getCounterSysticks()   {return rti->getCounterSysticks();  }


} /* namespace rti */
} /* namespace mcal */

