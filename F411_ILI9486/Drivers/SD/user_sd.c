#include "user_sd.h"

uint8_t SD_Write(FIL* fp, const TCHAR* path, char* buffer, unsigned int len)
{
  f_mount(&fs, "", 1);
  f_open(fp, path, FA_OPEN_ALWAYS|FA_WRITE);
  UINT bytesWrote;
  f_write(fp, buffer, len, &bytesWrote);
  f_sync(fp);
  f_close(fp);
  f_mount(NULL, "", 0);
  return 0;
}

uint8_t SD_Read(FIL* fp, const TCHAR* path, char* buffer, unsigned int len)
{
  uint8_t inner_buf[50];
  uint8_t fres = f_mount(&fs, "", 1); //1=mount now
  DWORD free_clusters;
  FATFS* getFreeFs;
  fres = f_getfree("", &free_clusters, &getFreeFs);
  fres = f_open(fp, path, FA_READ);
  TCHAR* rres = f_gets((TCHAR*)inner_buf, len, fp);
  f_close(fp);
  f_mount(NULL, "", 0);
  return fres;
}