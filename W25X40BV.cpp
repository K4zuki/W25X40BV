// W25X40BV.cpp

#include"W25X40BV.h"

// CONSTRUCTOR 
W25X40BV::W25X40BV(PinName mosi, PinName miso, PinName sclk, PinName cs) : _spi(mosi, miso, sclk), _cs(cs) {
    _spi.format(SPI_NBIT, SPI_MODE);
    _spi.frequency(SPI_FREQ);
    chipDisable();
}


// READING
int W25X40BV::read(int addr) {
    chipEnable();
    _spi.write(R_INST);
    _spi.write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    _spi.write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    _spi.write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    int response = _spi.write(DUMMY_ADDR);
    chipDisable();
    return response;
}
int W25X40BV::read(int a2, int a1, int a0) {
   chipEnable();
   _spi.write(R_INST);
   _spi.write(a2);
   _spi.write(a1);
   _spi.write(a0);
   int response = _spi.write(DUMMY_ADDR);
    chipDisable();
    return response;
}


// WRITING
void W25X40BV::write(int addr, int data) {
    writeEnable();
    chipEnable();
    _spi.write(W_INST);
    _spi.write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    _spi.write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    _spi.write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    _spi.write(data);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}
void W25X40BV::write(int a2, int a1, int a0, int data) {
    writeEnable();
    chipEnable();
    _spi.write(W_INST);
    _spi.write(a2);
    _spi.write(a1);
    _spi.write(a0);
    _spi.write(data);
    chipDisable();
     writeDisable();
     wait(WAIT_TIME);
}


//ERASING
void W25X40BV::chipErase() {
    writeEnable();
    chipEnable();
    _spi.write(C_ERASE_INST);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}
    

//ENABLE/DISABLE (private functions)
void W25X40BV::writeEnable() {
    chipEnable();
    _spi.write(WE_INST);
    chipDisable();
}
void W25X40BV::writeDisable() {
    chipEnable();
    _spi.write(WD_INST);
    chipDisable();
}
void W25X40BV::chipEnable() {
    _cs = 0;
}
void W25X40BV::chipDisable() {
    _cs = 1;
}