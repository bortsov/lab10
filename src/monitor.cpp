/*
* monitor.c
*
*  Created on: 30.10.2012
*      Author: bortsov
*/

#include "monitor.h"
#include "uart_baseclass.h"
#include "userExitCodes.h"

#include <cstring>
#include <cctype>

namespace mon {



static uart::Uart* cb = nullptr;
static constexpr size_t inputBufferSize = 40;
static char inputBuffer[inputBufferSize];
static size_t positionInputBuffer = 0;
static constexpr int maxCommands = 20;

static const Command* commandList[maxCommands] = {nullptr};

static constexpr  char  newLinePrompt[]="\n>";
static constexpr  char  inputBufferOverflowMessage[]="Переполнение входного буфера";
static constexpr  char  invalidCommandMessage[] = "Несуществующая команда";
static constexpr  char  errorInParametersMessage[] = "Ошибка в параметре команды";


namespace vt {
static constexpr char ESC = 0x1B;
static constexpr char CSI[] = {ESC, '['};
static __attribute__((unused)) constexpr char ED2[] = {CSI[0], CSI[1], '2', 'J'}; /* clear all screen */
static constexpr size_t maxLengthForSGR = 6;
enum SGR {
    NORMAL,
    BOLD,
    UNDERLINED     = 4,
    BLINK,
    INVERSE        = 7,
    NORMAL2        = 22,
    NOT_UNDERLINED = 24,
    NOT_BLINKING,
    NOT_INVERSE    = 27,
    FG_BLACK       = 30,
    FG_RED,
    FG_GREEN,
    FG_YELLOW,
    FG_BLUE,
    FG_MAGENTA,
    FG_CYAN,
    FG_WHITE,
    FG_DEFAULT     = 39,
    BG_BLACK,
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE,
    BG_DEFAULT     = 49,
    FG_GRAY        = 90,
    FG_BRIGHTRED,
    FG_BRIGTHGREEN,
    FG_BRIGHTYELLOW,
    FG_BRIGHTBLUE,
    FG_BRIGHTMAGENTA,
    FG_BRIGHTCYAN,
    FG_BRIGHTWHITE,
    BG_GRAY        = 100,
    BG_BRIGHTRED,
    BG_BRIGTHGREEN,
    BG_BRIGHTYELLOW,
    BG_BRIGHTBLUE,
    BG_BRIGHTMAGENTA,
    BG_BRIGHTCYAN,
    BG_BRIGHTWHITE
};

static size_t generateSGR (SGR s, char* buf)
{
    buf[0] = CSI[0], buf[1] = CSI[1];
    if (s >= 100) {
        buf[2] = '0' + s / 100;
        buf[3] = '0' + (s % 100) / 10;
        buf[4] = '0' + s % 10;
        buf[5] = 'm';
        return 6;
    } else if (s >= 10) {
        buf[2] = '0' + s / 10;
        buf[3] = '0' + (s % 10);
        buf[4] = 'm';
        return 5;
    } else {
        buf[2] = '0' + s;
        buf[3] = 'm';
        return 4;
    }
}

} /* namespace vt */



static void clearCommandList()
{
    for (auto& v : commandList) {
        v = nullptr;
    }
}


static int findEmptyRecordsInCommandList()
{
    size_t i = 0;
    while (i < maxCommands && commandList[i]) {
        ++i;
    }
    return i;
}


static bool isCharsEqual(char ch1, char ch2)
{
    return toupper(ch1) == toupper(ch2);
}


static bool isClearChar(char ch)
{
    if (ch == ' ' || ch == '\t') {
        return true;
    }
    return false;
}

static bool isStringConsist(const char*  const buf, const char*  const str)
{
    size_t i = 0;
    while (buf[i] && str[i] && isCharsEqual(buf[i], str[i])) ++i;
    bool rc = false;
    if (i == strlen(str)) {
        if (!buf[i] || isClearChar(buf[i])) rc = true;
    }
    return rc;
}


static int findCommand()
{
    size_t i = 0;
    while (i < maxCommands && commandList[i] && !isStringConsist(inputBuffer, commandList[i]->cmd)) {
        ++i;
    }
    if (i < maxCommands && !commandList[i]) {
        i = maxCommands;
    }
    return i;
}



static void createImpl(uart::Uart* _cb)
{
    if (cb) {
        return;
    }
    if (!_cb) {
    	exit(EXIT_MONITOR_INVALID_OBJECT_TO_USE);
    }
    cb = _cb;
    positionInputBuffer = 0;
    clearCommandList();
}

static void destroyImpl()
{
    if (cb) {
        cb = nullptr;
    }
}

static void processBackspace()
{
    if (!positionInputBuffer) return;
    cb->putString("\b \b");
    positionInputBuffer--;
}


static void putSGR(vt::SGR sgr, uart::Uart& cb)
{
    using namespace vt;
    char buf[maxLengthForSGR];
    const size_t length = generateSGR(sgr, buf);
    cb.putArray(buf, length);
}

static void processOverflowInInput()
{
    cb->putChar('\n');
    putSGR(vt::FG_RED, *cb);
    putSGR(vt::BOLD, *cb);
    cb->putString(inputBufferOverflowMessage);
    putSGR(vt::NORMAL, *cb);
    cb->putString(newLinePrompt);
    positionInputBuffer = 0;
}

static const char* findSomething(int n)
{
    const char* ptr = inputBuffer + strlen(commandList[n]->cmd);
    while (isClearChar(*ptr)) ++ptr;
    return ptr;
}

static void processHelp()
{
    int i = 0;
    const Command* cmd = commandList[i];
    while (i < maxCommands && cmd) {
        cb->putChar('\n');
        putSGR(vt::FG_BLUE, *cb);
        putSGR(vt::BOLD, *cb);
        cb->putString(cmd->cmd);
        if (cmd->parameters && cmd->parameters[0]) {
            cb->putChar(' ');
            putSGR(vt::FG_MAGENTA, *cb);
            cb->putString(cmd->parameters);
        }
        putSGR(vt::NORMAL2, *cb);
        putSGR(vt::FG_BLUE, *cb);
        cb->putString(" - ");
        cb->putString(cmd->help);
        cb->sync();
        if (cmd->parametersDescription && cmd->parametersDescription[0]) {
            cb->putChar('\n');
            cb->putString(cmd->parametersDescription);
            cb->sync();
        }
        ++i;
        cmd = commandList[i];
    }
    putSGR(vt::NORMAL, *cb);
}

static void processInvalidCommand()
{
    cb->putChar('\n');
    putSGR(vt::FG_RED, *cb);
    putSGR(vt::BOLD, *cb);
    cb->putString(invalidCommandMessage);
    putSGR(vt::NORMAL, *cb);
}

static void processErrorInParameters()
{
    cb->putChar('\n');
    putSGR(vt::FG_RED, *cb);
    putSGR(vt::BOLD, *cb);
    cb->putString(errorInParametersMessage);
    putSGR(vt::NORMAL, *cb);
}

static void processCommand(int n)
{
    const bool success = commandList[n]->run(findSomething(n), *cb);
    if (!success) processErrorInParameters();
}

static void processNewLine()
{
    inputBuffer[positionInputBuffer] = 0;
    const size_t n = findCommand();
    if (n < maxCommands) {
        processCommand(n);
    } else {
        if (isStringConsist(inputBuffer, "help")) {
            processHelp();
        } else if (positionInputBuffer) {
            processInvalidCommand();
        }
    }
    cb->putString(newLinePrompt);
    positionInputBuffer = 0;
}


static void processInputByte(char ch)
{
    if (positionInputBuffer >= inputBufferSize-1) {
        processOverflowInInput();
        return;
    }
    if ('\b' == ch) {
        processBackspace();
        return;
    }
    if ('\n' == ch || '\r' == ch) {
        processNewLine();
        return;
    }
    if (0x1B == ch) {
        positionInputBuffer = 0;
        processNewLine();
    } else if (ch) {
        inputBuffer[positionInputBuffer] = ch;
        cb->putChar(ch);
        ++positionInputBuffer;
    }
}

static bool findDecimalPoint(const char* buf)
{
    while (*buf != '\0' && *buf != '.' && isdigit(*buf)) {
        ++buf;
    }
    return (*buf == '.') ? true : false;
}

static size_t getPositionDecimalPoint(const char* buf)
{
    size_t idx = 0;
    while(*buf != '.') ++buf, ++idx;
    return idx;
}

static int getUnsignedNumberFromCstring(const char* buf)
{
    const size_t length = strlen(buf);
    switch (length) {
    case 1: return getDigit(buf);
    case 2: return getTens(buf);
    case 3: return getHundred(buf);
    case 4: return getThousand(buf);
    case 5: return getTenThousand(buf);
    case 6: return getHundredThousand(buf);
    case 7: return getMillion(buf);
    case 8: return getTenMillion(buf);
    case 9: return getHundredMillion(buf);
    default: return 0;
    }
}


void (*create)(uart::Uart* cb) = createImpl;
void (*destroy)() = destroyImpl;

void taskBody()
{
    if (cb->isNewChar()) {
        processInputByte(cb->getChar());
    }
}


void registerCmd(const Command& cmd)
{
    const size_t n = findEmptyRecordsInCommandList();
    if (n >= maxCommands) {
    	exit(EXIT_MONITOR_NO_SPACE_TO_REGISTER_COMMAND);
    }
    commandList[n] = &cmd;
}


const char* readOneParameterWithCapitalConversation(char* to, const char* from, size_t maxBytes)
{
    size_t i = 0;
    while (*from && isblank(*from)) {
        ++from;
    }
    while (*from && isgraph(*from) && i < maxBytes-1) {
        *to = toupper(*from);
        ++i, ++to, ++from;
    }
    *to = 0;
    return from;
}


const char* readOneParameter(char* to, const char* from, size_t maxBytes)
{
    size_t i = 0;
    while (*from && isblank(*from)) ++from;
    while (i < maxBytes-1 && *from && (isalnum(*from) || *from == ':' || *from == '\\' || *from == '.' || *from == '-')) {
        *to = *from;
        ++i, ++to, ++from;
    }
    *to = 0;
    return from;
}

bool isStrEmpty(const char* str)
{
    while (*str && isspace(*str)) {
        ++str;
    }
    return (*str) ? false : true;
}

bool checkStrForDigit(const char* s)
{
    while (*s && isdigit(*s)) ++s;
    return (*s) ? false : true;
}


int getDigit(const char* s)
{
    return isdigit(*s) ? (*s - '0') : 0;
}

int getTens(const char* s)
{
    int rc = getDigit(s++) * 10;
    return rc + getDigit(s);
}

int getHundred(const char* s)
{
    int rc = getDigit(s++) * 100;
    return rc + getTens(s);
}

int getThousand(const char* s)
{
    int rc = getDigit(s++) * 1000;
    return rc + getHundred(s);
}

int getTenThousand(const char* s)
{
    int rc = getDigit(s++) * 10000;
    return rc + getThousand(s);
}

int getHundredThousand(const char* s)
{
    int rc = getDigit(s++) * 100000;
    return rc + getTenThousand(s);
}

int getMillion(const char* s)
{
    int rc = getDigit(s++) * 1000000;
    return rc + getHundredThousand(s);
}

int getTenMillion(const char* s)
{
    int rc = getDigit(s++) * 10000000;
    return rc + getMillion(s);
}


int getHundredMillion(const char* s)
{
    int rc = getDigit(s++) * 100000000;
    return rc + getTenMillion(s);
}



/*
 * Предполагаемый формат данных: X.XX (положительное число, не более 9 цифр до точки и не более 9 цифр после точки)
 */
float getUnsignedFloatValueFromCstring(const char* buf2)
{
    char buf[20] = {0};
    strcpy(buf, buf2);
    if (!findDecimalPoint(buf)) {
    	return getUnsignedNumberFromCstring(buf);
    }
    const size_t idxDecimalPoint = getPositionDecimalPoint(buf);
    buf[idxDecimalPoint] = '\0';
    const int n1 = getUnsignedNumberFromCstring(buf);
    const char* reminder = &buf[idxDecimalPoint + 1];
    const int n2 = getUnsignedNumberFromCstring(reminder);
    const size_t n2digit = strlen(reminder);
    uint32_t divider = 1;
    for (size_t i = 0; i < n2digit; ++i) {
    	divider *= 10;
    }
    return static_cast<float>(n1) + static_cast<float>(n2) / divider;
}


float getSignedFloatValueFromCstring(const char* s)
{
    if (*s == '-') {
        return -getUnsignedFloatValueFromCstring(&s[1]);
    } else {
        return getUnsignedFloatValueFromCstring(&s[0]);
    }
}


int readUnsignedIntegerParameter(const char* s)
{
    if (!checkStrForDigit(s)) {
        return -1;
    }
    const int len = strlen(s);
    switch (len) {
        case 1: return getDigit(s);
        case 2: return getTens(s);
        case 3: return getHundred(s);
        case 4: return getThousand(s);
        case 5: return getTenThousand(s);
        default: return -1;
    }
}


int readSignedIntegerParameter(const char* s)
{
    if (*s == '-') {
        return -readUnsignedIntegerParameter(&s[1]);
    } else {
        return readUnsignedIntegerParameter(&s[0]);
    }
}


uint8_t readOneDigitHEXParameter(const char* s)
{
    if (*s >= '0' && *s <='9') return *s - '0';
    else {
        const uint8_t ch = toupper(*s);
        return ch - 'A' + 10;
    }
}

uint8_t readTwoDigitHEXParameter(const char* s)
{
    return (readOneDigitHEXParameter(s) << 4) | readOneDigitHEXParameter(s+1);
}

uint32_t readSixDigitHEXParameter(const char* s)
{
    const uint8_t b1 = readTwoDigitHEXParameter(s);
    const uint8_t b2 = readTwoDigitHEXParameter(&s[2]);
    const uint8_t b3 = readTwoDigitHEXParameter(&s[4]);
    return (b1 << 16) | (b2 << 8) | b3;
}

size_t getInputBufferSize()
{
    return inputBufferSize;
}


void setBlueFont(uart::Uart& cb)
{
    putSGR(vt::FG_BLUE, cb);
}


void setNormalFont(uart::Uart& cb)
{
    putSGR(vt::NORMAL, cb);
}


void setRedFont(uart::Uart& cb)
{
    putSGR(vt::FG_RED, cb);
}

} /* namespace mon */
