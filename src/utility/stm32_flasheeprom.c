#include "stm32_flasheeprom.h"
#include <string.h>

// static uint8_t pageBuffer[FLASH_PAGE_SIZE] __attribute__((aligned(8))) = {0};
static uint8_t pageBuffer[FLASH_PAGE_SIZE] __attribute__((aligned(8)));
uint32_t currentPage = 0;

void eeprom_fill_buffer() {
	// Load current page into buffer from flash to buffer
	memcpy(pageBuffer, (uint8_t *)((uint32_t)((FLASH_END + 1) - (FLASH_PAGE_SIZE) * (currentPage + 1))), FLASH_PAGE_SIZE);
}

void eeprom_load_page(uint32_t virtualAddress) {
	// Calculate page
	uint32_t page = virtualAddress / FLASH_PAGE_SIZE;

	if (page != currentPage) {
		// Flush current page
		eeprom_flush_page();
		// Set currentPage to new page
		currentPage = page;
		// Load new page from flash to buffer
		eeprom_fill_buffer();
	}
}

uint8_t eeprom_read_byte(uint32_t virtualAddress) {
	// Load correct page from EEPROM
	eeprom_load_page(virtualAddress);

	// Find actuall address relavant to the page
	uint32_t address = virtualAddress % FLASH_PAGE_SIZE;

	// Return value from buffer/EEPROM
	return pageBuffer[address];
}


void eeprom_write_byte(uint32_t virtualAddress, uint8_t data) {
	// Make sure we are writting to the correct page
	eeprom_load_page(virtualAddress);

	// Find actuall address relavant to the page
	uint32_t address = virtualAddress % FLASH_PAGE_SIZE;

	// Update buffer
	pageBuffer[address] = data;
	// eeprom_flush_page();
}

void eeprom_flush_page() {
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t offset = 0;
	uint32_t pageAddress = ((uint32_t)((FLASH_END + 1) - (FLASH_PAGE_SIZE) * (currentPage + 1)));
	uint32_t address = pageAddress;
	uint32_t address_end = pageAddress + FLASH_PAGE_SIZE - 1;

	// Variable to store page error
	uint32_t pageError = 0;

	// STM32F1 uses doubleword - FLASH_TYPEPROGRAM_DOUBLEWORD
	uint64_t data = 0;

	// Erase page
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks = FLASH_BANK_NUMBER;
	EraseInitStruct.PageAddress = pageAddress;
	// Erase only one page - current page
	EraseInitStruct.NbPages = 1;

	// Unlock flash
	if (HAL_FLASH_Unlock() == HAL_OK) {
		// Clear all errors
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
		// Erase entire page
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &pageError) == HAL_OK) {
			// Loop through buffer
			while (address <= address_end) {
				data = *((uint64_t *)((uint8_t *)pageBuffer + offset));
				// Write double word of buffer to flush
				if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data) == HAL_OK) {
					address += 8;
					offset += 8;
				} else {
					address = address_end + 1;
				}
			}
		}
		// Lock flush
		HAL_FLASH_Lock();
	}
}