#include "main.h"
#include "fatfs.h"
#include "user_diskio_spi.h"

static FATFS fs; // Work area (file system object) for logical drives
static FIL fdst, data_file; // file objects
static FILINFO finfo;

uint8_t SD_Write(FIL* fp, const TCHAR* path, char* buffer, unsigned int len);
uint8_t SD_Read(FIL* fp, const TCHAR* path, char* buffer, unsigned int len);