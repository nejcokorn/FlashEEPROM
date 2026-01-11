#ifndef __EEPROM_H
#define __EEPROM_H

#include <stm32_def.h>

#ifdef __cplusplus
extern "C" {
#endif

#define E2END (FLASH_PAGE_SIZE - 1)

/* Be able to change FLASH_BANK_NUMBER to use if relevant */
#if !defined(FLASH_BANK_NUMBER) && defined(FLASH_BANK_1) && !defined(STM32WBAxx)
#if defined(FLASH_BANK_2)
#define FLASH_BANK_NUMBER   FLASH_BANK_2
#else /* FLASH_BANK_1 */
#define FLASH_BANK_NUMBER   FLASH_BANK_1
#endif /* FLASH_BANK_2 */
#ifndef FLASH_BANK_NUMBER
#error "FLASH_BANK_NUMBER could not be defined"
#endif
#endif /* !FLASH_BANK_NUMBER */

/* Be able to change FLASH_DATA_SECTOR to use if relevant */
#if defined(FLASH_SECTOR_TOTAL) || defined(FLASH_SECTOR_NB)
#if !defined(FLASH_DATA_SECTOR)
#if defined(FLASH_SECTOR_TOTAL)
#define FLASH_DATA_SECTOR   ((uint32_t)(FLASH_SECTOR_TOTAL - 1))
#elif defined(FLASH_SECTOR_NB)
#define FLASH_DATA_SECTOR   ((uint32_t)(FLASH_SECTOR_NB - 1))
#endif
#else
#ifndef FLASH_BASE_ADDRESS
#error "FLASH_BASE_ADDRESS have to be defined when FLASH_DATA_SECTOR is defined"
#endif
#endif /* !FLASH_DATA_SECTOR */
#endif /* FLASH_SECTOR_TOTAL || FLASH_SECTOR_NB */

/* Be able to change EEPROM_FLASH_PAGE_NUMBER to use if relevant */
#if !defined(EEPROM_FLASH_PAGE_NUMBER) && defined(FLASH_PAGE_SIZE)
#if defined(STM32WB0x)
/* STM32WB0x define the FLASH_PAGE_NUMBER */
#define EEPROM_FLASH_PAGE_NUMBER   (FLASH_PAGE_NUMBER - 1)
#else
#define EEPROM_FLASH_PAGE_NUMBER   ((uint32_t)(((LL_GetFlashSize() * 1024) / FLASH_PAGE_SIZE) - 1))
#endif
#endif /* !EEPROM_FLASH_PAGE_NUMBER */

/* Be able to change FLASH_END to use */
#if !defined(FLASH_END)
#if defined(EEPROM_RETRAM_MODE)
#define FLASH_END  ((uint32_t)(EEPROM_RETRAM_START_ADDRESS + EEPROM_RETRAM_MODE_SIZE -1))
#elif defined(DATA_EEPROM_END)
#define FLASH_END DATA_EEPROM_END
#elif defined (FLASH_BANK2_END) && (FLASH_BANK_NUMBER == FLASH_BANK_2)
#define FLASH_END  FLASH_BANK2_END
#elif defined (FLASH_BANK1_END) && (FLASH_BANK_NUMBER == FLASH_BANK_1)
#define FLASH_END  FLASH_BANK1_END
#elif defined(FLASH_DATA_SECTOR)
#if defined(FLASH_BANK_2) && (FLASH_BANK_NUMBER == FLASH_BANK_2)
#define FLASH_END  ((uint32_t)(FLASH_BASE + FLASH_BANK_SIZE + (FLASH_DATA_SECTOR * FLASH_SECTOR_SIZE) + FLASH_SECTOR_SIZE - 1))
#else
#define FLASH_END  ((uint32_t)(FLASH_BASE + (FLASH_DATA_SECTOR * FLASH_SECTOR_SIZE) + FLASH_SECTOR_SIZE - 1))
#endif /* FLASH_BANK_2 */
#elif defined(FLASH_START_ADDR)
/* If EEPROM_FLASH_PAGE_NUMBER is defined by user, this is not really end of the flash */
#define FLASH_END  ((uint32_t)(FLASH_START_ADDR + (((EEPROM_FLASH_PAGE_NUMBER +1) * FLASH_PAGE_SIZE))-1))
#elif defined(FLASH_BASE) && defined(EEPROM_FLASH_PAGE_NUMBER) && defined (FLASH_PAGE_SIZE)
/* If EEPROM_FLASH_PAGE_NUMBER is defined by user, this is not really end of the flash */
#define FLASH_END  ((uint32_t)(FLASH_BASE + (((EEPROM_FLASH_PAGE_NUMBER +1) * FLASH_PAGE_SIZE))-1))
#endif
#ifndef FLASH_END
#error "FLASH_END could not be defined"
#endif
#endif /* FLASH_END */


/* Combine error flags */ 
#ifndef FLASH_FLAG_ALL_ERRORS
#if defined(FLASH_FLAG_PGERR)
#if defined(FLASH_FLAG_OPTVERR)
#define FLASH_FLAG_ALL_ERRORS     (FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_OPTVERR)
#else
#define FLASH_FLAG_ALL_ERRORS     (FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR)
#endif /* FLASH_FLAG_OPTVERR */
#else
#if defined(FLASH_PCROP_SUPPORT)
#define FLASH_FLAG_ALL_ERRORS     (FLASH_FLAG_OPERR   | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | \
                                   FLASH_FLAG_PGAERR  | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR | \
                                   FLASH_FLAG_MISERR  | FLASH_FLAG_FASTERR | FLASH_FLAG_RDERR  | \
                                   FLASH_FLAG_OPTVERR | FLASH_FLAG_ECCC    | FLASH_FLAG_ECCD)
#else
#define FLASH_FLAG_ALL_ERRORS     (FLASH_FLAG_OPERR   | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | \
                                   FLASH_FLAG_PGAERR  | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR | \
                                   FLASH_FLAG_MISERR  | FLASH_FLAG_FASTERR | \
                                   FLASH_FLAG_OPTVERR | FLASH_FLAG_ECCC    | FLASH_FLAG_ECCD)
#endif /* FLASH_PCROP_SUPPORT */
#endif /* FLASH_FLAG_PGERR */
#endif /* FLASH_FLAG_ALL_ERRORS */

/* Define eeprom flush functions */
void eeprom_fill_buffer();
uint8_t eeprom_read_byte(const uint32_t pos);
void eeprom_write_byte(uint32_t pos, uint8_t value);
void eeprom_flush_page();

#ifdef __cplusplus
}
#endif

#endif