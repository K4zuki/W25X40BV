/* Copyright (c) 2016 Kazuki Yamamoto <k.yamamoto.08136891@gmail.com>
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/** W25X40BV.cpp
*/
#include "W25X40BV.h"
#include "SWSPI.h"

//! CONSTRUCTOR
W25X40BV::W25X40BV(PinName mosi, PinName miso, PinName sclk, PinName cs)
    : SWSPI(mosi, miso, sclk), _cs(cs) {
  this->format(SPI_NBIT, SPI_MODE);
  this->frequency(SPI_FREQ);
  chipDisable();
}

//! READING
uint32_t W25X40BV::readByte(uint32_t addr) {
  chipEnable();
  this->write(R_INST);
  this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
  this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
  this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
  uint32_t response = this->write(DUMMY_ADDR);
  chipDisable();
  return response;
}

uint32_t W25X40BV::readByte(uint32_t a2, uint32_t a1, uint32_t a0) {
  chipEnable();
  this->write(R_INST);
  this->write(a2);
  this->write(a1);
  this->write(a0);
  uint32_t response = this->write(DUMMY_ADDR);
  chipDisable();
  return response;
}

void W25X40BV::readStream(uint32_t addr, uint8_t* buf, uint32_t count) {
  if (count < 1) return;
  chipEnable();
  this->write(R_INST);
  this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
  this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
  this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
  for (uint32_t i = 0; i < count; i++) buf[i] = this->write(DUMMY_ADDR);
  chipDisable();
}

// WRITING
void W25X40BV::writeByte(uint32_t addr, uint32_t data) {
  writeEnable();
  chipEnable();
  this->write(W_INST);
  this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
  this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
  this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
  this->write(data);
  chipDisable();
  writeDisable();
  wait_us(10 * WAIT_TIME);
}

void W25X40BV::writeByte(uint32_t a2, uint32_t a1, uint32_t a0, uint32_t data) {
  writeEnable();
  chipEnable();
  this->write(W_INST);
  this->write(a2);
  this->write(a1);
  this->write(a0);
  this->write(data);
  chipDisable();
  writeDisable();
  wait_us(10 * WAIT_TIME);
}

void W25X40BV::writeStream(uint32_t addr, uint8_t* buf, uint32_t count) {
  if (count < 1) return;
  writeEnable();
  chipEnable();
  this->write(W_INST);
  this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
  this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
  this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
  for (uint32_t i = 0; i < count; i++) this->write(buf[i]);
  chipDisable();
  wait_ms(10 * WAIT_TIME);
  writeDisable();
}

// ERASING
void W25X40BV::pageErase(uint16_t page) {
  int msb = page >> 8;
  writeEnable();
  chipEnable();
  this->write(P_ERASE_INST);
  this->write(msb);
  this->write((int)page);
  this->write(DUMMY_ADDR);
  chipDisable();
  writeDisable();
  wait_ms(50 * WAIT_TIME);
}

void W25X40BV::block4Erase(uint16_t block) {
  uint8_t msb = (uint8_t)(block >> 4);
  block = (block << 4) & 0xF0;
  writeEnable();
  chipEnable();
  this->write(B4K_ERASE_INST);
  this->write((int)msb);
  this->write((int)block);
  this->write(DUMMY_ADDR);
  chipDisable();
  writeDisable();
  wait_ms(100 * WAIT_TIME);
}

void W25X40BV::block32Erase(uint8_t block) {
  block = (block << 3) & 0x18;
  writeEnable();
  chipEnable();
  this->write(B32K_ERASE_INST);
  this->write((int)block);
  this->write(DUMMY_ADDR);
  this->write(DUMMY_ADDR);
  chipDisable();
  writeDisable();
  wait(WAIT_TIME);
}

void W25X40BV::chipErase() {
  writeEnable();
  chipEnable();
  this->write(C_ERASE_INST);
  chipDisable();
  writeDisable();
  wait(3 * WAIT_TIME);
}

// ENABLE/DISABLE (private functions)
void W25X40BV::writeEnable() {
  chipEnable();
  this->write(WE_INST);
  chipDisable();
}

void W25X40BV::writeDisable() {
  chipEnable();
  this->write(WD_INST);
  chipDisable();
}

void W25X40BV::chipEnable() {
  _cs.output();
  _cs = 0;
}

void W25X40BV::chipDisable() {
  _cs = 1;
  _cs.input();
  _cs.mode(PullNone);
}
