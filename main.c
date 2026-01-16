#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

#define FLASH_SIZE (64*1024)
#define SRAM_SIZE (4*1024)

//defining the flash and sram size memory blocks.
uint8_t flash[FLASH_SIZE]; 
uint8_t sram[SRAM_SIZE];

//the structure to hold register blocks
typedef struct{
	uint32_t ctrl; 
	uint32_t status; 
	uint32_t data;
}Peripheral;

//creating an instance of Peripheral
Peripheral peripheral;

// a bus access read funtion
uint8_t read8(uint32_t address){
	return 0;
}

//a bus access write funtion
void write8(uint32_t address, uint8_t value){
	
}

