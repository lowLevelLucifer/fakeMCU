#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

//a place where the initialized variable values are stored and this is read-only
uint8_t flash_data_init[16] = {
	1,2,3,4,
	5,6,7,8,
	9,10,11,12,
	13,14,15,16,
};

//we have declared this to store our variable values from the flash which is read-only
uint8_t sram_data[16];
uint8_t sram_bss[16];

void embedded_main(void);

//this funtion helps us to store the global/intitalized/unitinialized variable values into the SRAM for the CPU to access.
uint8_t reset_handler(){
	for(uint8_t i = 0; i < sizeof(sram_data); i++){
		sram_data[i] = flash_data_init[i];
	}
	for(uint8_t i = 0; i < sizeof(sram_bss); i++){
		sram_bss[i] = 0;
	}
	//calling the main function
	embedded_main();
}


#define FLASH_SIZE (64*1024)
#define SRAM_SIZE (4*1024)

//defining the flash and sram size memory blocks.
uint8_t flash[FLASH_SIZE]; 
uint8_t sram[SRAM_SIZE];

//defining the base memory for addressing
#define FLASH_BASE 0x00000000
#define SRAM_BASE 0x20000000
#define PERIPHERAL_BASE 0x40000000

//the structure to hold register blocks
typedef struct{
	uint32_t ctrl; 
	uint32_t status; 
	uint32_t data;
}Peripheral;

//creating an instance of Peripheral
Peripheral peripheral;

//defining the end memory for addressing range
#define FLASH_END (FLASH_BASE + FLASH_SIZE)
#define SRAM_END (SRAM_BASE + SRAM_SIZE)
#define PERIPHERAL_END (PERIPHERAL_BASE + sizeof(peripheral))

// a bus access read funtion
uint8_t read8(uint32_t address){
	uint32_t index; 
	if(address >= FLASH_BASE && address < FLASH_END){
		index = address - FLASH_BASE; 
		return flash[index];
	}
	else if(address >= SRAM_BASE && address < SRAM_END){
		index = address - SRAM_BASE; 
		return sram[index];
	}
	else if(address >= PERIPHERAL_BASE && address < PERIPHERAL_END){
		index = address - PERIPHERAL_BASE; 
		return ((uint8_t*)&peripheral)[index];
	}
	else{
		printf("Read fault at address 0x%08X\n",address); 
		return 0;
	}
}

//a bus access write funtion
void write8(uint32_t address, uint8_t value){
	uint32_t index;
	if(address >= SRAM_BASE && address < SRAM_END){
		index = address - SRAM_BASE; 
		sram[index] = value;
	}
	else if(address >= PERIPHERAL_BASE && address < PERIPHERAL_END){
		index = address - PERIPHERAL_BASE; 
		((uint8_t*)&peripheral)[index] = value;
	}
	else if(address >= FLASH_BASE && address < FLASH_END){
		printf("Error at Write Flash is read-only\n"); 
	}else{
		printf("Error at write on address 0x%08X\n",address);
	}
}

//since our main function is running on the hosted OS, the OS requires an entry point which is our main(), that main only acts as our simulation harness
//our actual firmware entry point is rest_handler which initalize the memory and transfer control to embedded_main() which will mimick the MCU
void embedded_main(void){
	printf("Embedded main started\n");
    printf("sram_data[0] = %d\n", sram_data[0]);
    printf("sram_bss[0] = %d\n", sram_bss[0]);
    while(1){
		//this while loop is infinity since there is no return to the higher level runtime..
    }
}

int main(void){
	printf("System reset\n");
	//this fixes the infinity loop since we transfer the controls over to embedded main by the reset_handler as the entry point using the main fun in hosted OS
	reset_handler();//firmware entry point
    return 0;
}

