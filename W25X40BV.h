// W25X40BV.h

#ifndef W25X40BV_H
#define W25X40BV_H

#include "mbed.h"

#define SPI_FREQ        1000000
#define SPI_MODE        0
#define SPI_NBIT        8

#define WE_INST         0x06
#define WD_INST         0x04
#define R_INST          0x03
#define W_INST          0x02
#define C_ERASE_INST    0x60

#define DUMMY_ADDR      0x00
#define WAIT_TIME       1

#define ADDR_BMASK2     0x00ff0000
#define ADDR_BMASK1     0x0000ff00
#define ADDR_BMASK0     0x000000ff

#define ADDR_BSHIFT2    16
#define ADDR_BSHIFT1    8
#define ADDR_BSHIFT0    0

class W25X40BV {
public:
    W25X40BV(PinName mosi, PinName miso, PinName sclk, PinName cs);
    
    int readByte(int addr);                             // takes a 24-bit (3 bytes) address and returns the data (1 byte) at that location
    int readByte(int a2, int a1, int a0);               // takes the address in 3 separate bytes A[23,16], A[15,8], A[7,0]
    void read(int addr, char* buf, int count);          // takes a 24-bit address, reads count bytes, and stores results in buf
    
    void writeByte(int addr, int data);                 // takes a 24-bit (3 bytes) address and a byte of data to write at that location
    void writeByte(int a2, int a1, int a0, int data);   // takes the address in 3 separate bytes A[23,16], A[15,8], A[7,0]
    void write(int addr, char* buf, int count);         // write count bytes of data from buf to memory, starting at addr  
    
    void chipErase();                               // erase all data on chip
    
private:
    void writeEnable();                             // write enable
    void writeDisable();                            // write disable
    void chipEnable();                              // chip enable
    void chipDisable();                             // chip disable
    
    SPI _spi;
    DigitalOut _cs;
};

#endif