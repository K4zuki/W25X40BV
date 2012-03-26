// W25X40BV.cpp

#include"W25X40BV.h"

// CONSTRUCTOR 
W25X40BV::W25X40BV(PinName mosi, PinName miso, PinName sclk, PinName cs) : SPI(mosi, miso, sclk), _cs(cs) {
    this->format(SPI_NBIT, SPI_MODE);
    this->frequency(SPI_FREQ);
    chipDisable();
}


// READING
int W25X40BV::readByte(int addr) {
    chipEnable();
    this->write(R_INST);
    this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    int response = this->write(DUMMY_ADDR);
    chipDisable();
    return response;
}
int W25X40BV::readByte(int a2, int a1, int a0) {
   chipEnable();
   this->write(R_INST);
   this->write(a2);
   this->write(a1);
   this->write(a0);
   int response = this->write(DUMMY_ADDR);
    chipDisable();
    return response;
}
void W25X40BV::readStream(int addr, char* buf, int count) {
    if (count < 1)
        return;
    chipEnable();
    this->write(R_INST);
    this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    for (int i = 0; i < count; i++)
        buf[i] =  this->write(DUMMY_ADDR);
    chipDisable();
}

// WRITING
void W25X40BV::writeByte(int addr, int data) {
    writeEnable();
    chipEnable();
    this->write(W_INST);
    this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    this->write(data);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}
void W25X40BV::writeByte(int a2, int a1, int a0, int data) {
    writeEnable();
    chipEnable();
    this->write(W_INST);
    this->write(a2);
    this->write(a1);
    this->write(a0);
    this->write(data);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}
void W25X40BV::writeStream(int addr, char* buf, int count) {
    if (count < 1)
        return;
    writeEnable();
    chipEnable();
    this->write(W_INST);
    this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    for (int i = 0; i < count; i++)
        this->write(buf[i]);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}

//ERASING
void W25X40BV::chipErase() {
    writeEnable();
    chipEnable();
    this->write(C_ERASE_INST);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}
    

//ENABLE/DISABLE (private functions)
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
    _cs = 0;
}
void W25X40BV::chipDisable() {
    _cs = 1;
}