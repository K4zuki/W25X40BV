/** W25X40BV.cpp
*/
#include "W25X40BV.h"
#include "SWSPI.h"

//! CONSTRUCTOR 
W25X40BV::W25X40BV(PinName mosi, PinName miso, PinName sclk, PinName cs) : SWSPI(mosi, miso, sclk), _cs(cs) {
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
    if (count < 1)
        return;
    chipEnable();
    this->write(R_INST);
    this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    for (uint32_t i = 0; i < count; i++)
        buf[i] =  this->write(DUMMY_ADDR);
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
    wait(WAIT_TIME);
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
    wait(WAIT_TIME);
}
void W25X40BV::writeStream(uint32_t addr, uint8_t* buf, uint32_t count) {
    if (count < 1)
        return;
    writeEnable();
    chipEnable();
    this->write(W_INST);
    this->write((addr & ADDR_BMASK2) >> ADDR_BSHIFT2);
    this->write((addr & ADDR_BMASK1) >> ADDR_BSHIFT1);
    this->write((addr & ADDR_BMASK0) >> ADDR_BSHIFT0);
    for (uint32_t i = 0; i < count; i++)
        this->write(buf[i]);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}

//ERASING
void W25X40BV::pageErase(uint8_t page){
    writeEnable();
    chipEnable();
    this->write(C_ERASE_INST);
    this->write(DUMMY_ADDR);
    this->write((int)page);
    this->write(DUMMY_ADDR);
    chipDisable();
    writeDisable();
    wait(WAIT_TIME);
}

void W25X40BV::block4Erase(uint16_t block){
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
    wait(WAIT_TIME);
}

void W25X40BV::block32Erase(uint8_t block){
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
