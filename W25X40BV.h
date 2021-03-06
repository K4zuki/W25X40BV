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
/** W25X40BV.h
*/

#ifndef W25X40BV_H_
#define W25X40BV_H_

#include "SWSPI.h"
#include "mbed.h"  // NOLINT

#define SPI_FREQ 10000000
#define SPI_MODE 0
#define SPI_NBIT 8

#define WE_INST 0x06
#define WD_INST 0x04
#define R_INST 0x03
#define W_INST 0x02
#define C_ERASE_INST 0x60
#define P_ERASE_INST 0x81
#define B4K_ERASE_INST 0x20
#define B32K_ERASE_INST 0x52

#define DUMMY_ADDR 0xAA
#define WAIT_TIME 2

#define ADDR_BMASK2 0x001f0000
#define ADDR_BMASK1 0x0000ff00
#define ADDR_BMASK0 0x000000ff

#define ADDR_BSHIFT2 16
#define ADDR_BSHIFT1 8
#define ADDR_BSHIFT0 0

/**
W25X40BV
*/
class W25X40BV : public SWSPI {
 public:
  W25X40BV(PinName mosi, PinName miso, PinName sclk, PinName cs);

  //! takes a 24-bit (3 bytes) address and returns the data (1 byte) at that
  //! location
  uint32_t readByte(uint32_t addr);
  //! takes the address in 3 separate bytes A[23,16], A[15,8], A[7,0]
  uint32_t readByte(uint32_t a2, uint32_t a1, uint32_t a0);
  //! takes a 24-bit address, reads count bytes, and stores results in buf
  void readStream(uint32_t addr, uint8_t* buf, uint32_t count);

  //! takes a 24-bit (3 bytes) address and a byte of data to write at that
  //! location
  void writeByte(uint32_t addr, uint32_t data);
  //! takes the address in 3 separate bytes A[23,16], A[15,8], A[7,0]
  void writeByte(uint32_t a2, uint32_t a1, uint32_t a0, uint32_t data);
  /** write count bytes of data from buf to memory, starting at addr
  * addr is expected to be 256 byte alignment: 0x000000,0x000100,...,0x01FF00
  * even if not, it works anyway. but should care count and 256byte edge:
  * if count=3 while addr=0x????FE, 3rd byte will be written in 0x????00
  * @param addr 24 bit address
  * @param buf data buffer
  * @param count data count in byte
  */
  void writeStream(uint32_t addr, uint8_t* buf, uint32_t count);

  //! erase a page data(256bytes) on chip
  void pageErase(uint16_t page);

  /** erase a block data(4Kbytes) on chip
  * @param block block number 0x0000 - 0x001F
  * block number will be left-shifted by 4 bits: 0x0000 - 0x01F0
  * and a dummy address byte (0x00) will be added
  */
  void block4Erase(uint16_t block);

  /** erase a block data(32Kbytes) on chip
  * @param block number 0x00 - 0x03
  * block number will be left-shifted by 3 bits: 0x00 - 0x18
  * and two dummy address bytes (0x0000) will be added
  */
  void block32Erase(uint8_t block);
  void chipErase();  //! erase all data on chip

 private:
  void writeEnable();   //! write enable
  void writeDisable();  //! write disable
  void chipEnable();    //! chip enable
  void chipDisable();   //! chip disable

  //! SPI _spi;
  DigitalInOut _cs;
};

#endif  // W25X40BV_H_
