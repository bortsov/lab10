/*
 * reg_vim.cpp
 *
 *  Created on: 13 окт. 2016 г.
 *      Author: bortsov
 */
#include "reg_vim.h"

static volatile VimBase * const vimREG = reinterpret_cast<volatile VimBase *>(0xFFFFFE00U);


volatile VimBase& getVim() {return *vimREG;}



