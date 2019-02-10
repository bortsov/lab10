/*
 * monitorCmd.h
 *
 *  Created on: 26 дек. 2014 г.
 *      Author: bortsov
 */

#ifndef INCLUDE_MONITORCMD_H_
#define INCLUDE_MONITORCMD_H_

#include "monitor.h"

#define MONITOR_CMD(nameCmd) \
    namespace mon {                    \
    namespace nameCmd {                \
    const Command& getCommand();       \
    } /* namespace nameCmd */          \
    } /* namespace mon */

MONITOR_CMD(showcurrents)
MONITOR_CMD(drv8301)

MONITOR_CMD(menable)
MONITOR_CMD(mchl)
MONITOR_CMD(mlogsave)
MONITOR_CMD(mlogshow)
MONITOR_CMD(mmode)
MONITOR_CMD(mlogcnt)
MONITOR_CMD(mw)
MONITOR_CMD(mwki)
MONITOR_CMD(mwkp)
MONITOR_CMD(mwon)
MONITOR_CMD(mva)
MONITOR_CMD(mvb)

#endif /* INCLUDE_MONITORCMD_H_ */
