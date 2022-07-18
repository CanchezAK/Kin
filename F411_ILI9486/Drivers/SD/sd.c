#include "sd.h"
//----------------------------------------------
extern volatile uint16_t Timer10;
extern SPI_HandleTypeDef hspi2;
sd_info_ptr sdinfo;
char str1[60]={0};
uint8_t ocr[4];
uint8_t arg = 0;
#define CMD0 (0x40+0) // GO_IDLE_STATE
#define CMD1 (0x40+1) // SEND_OP_COND (MMC)
#define ACMD41 (0xC0+41) // SEND_OP_COND (SDC)
#define CMD8 (0x40+8) // SEND_IF_COND
#define CMD9 (0x40+9) // SEND_CSD
#define CMD16 (0x40+16) // SET_BLOCKLEN
#define CMD17 (0x40+17) // READ_SINGLE_BLOCK
#define CMD24 (0x40+24) // WRITE_BLOCK
#define CMD55 (0x40+55) // APP_CMD
#define CMD58 (0x40+58) // READ_OCR

//Создадим функцию включения, в которой мы просто подождём 20 милисекунд для того, чтобы установилось напряжение
//-----------------------------------------------
uint8_t SPIx_WriteRead(uint8_t Byte)
{
  uint8_t receivedbyte = 0;
  if(HAL_SPI_TransmitReceive(&hspi2,(uint8_t*) &Byte,(uint8_t*) &receivedbyte,1,0x1000)!=HAL_OK)
  {
    //Error();
  }
  return receivedbyte;
}

void SPI_SendByte(uint8_t bt)
{
  SPIx_WriteRead(bt);
}
//-----------------------------------------------
uint8_t SPI_ReceiveByte(void)
{
  uint8_t bt = SPIx_WriteRead(0xFF);
  return bt;
}
//-----------------------------------------------
void SPI_Release(void)
{
  SPIx_WriteRead(0xFF);
}

void SD_PowerOn(void)
{
  Timer10 = 0;
  while(Timer10<2); //ждём 20 милисекунд, для того, чтобы напряжение стабилизировалось
}

static uint8_t SD_cmd (uint8_t cmd, uint32_t arg)
{
  uint8_t n, res;
  if (cmd & 0x80)
  {
    cmd &= 0x7F;
    res = SD_cmd(CMD55, 0);
    if (res > 1) return res;
  }
  SS_SD_DESELECT();
  SPI_ReceiveByte();
  SS_SD_SELECT();
  SPI_ReceiveByte();
  SPI_SendByte(cmd); // Start + Command index
  SPI_SendByte((uint8_t)(arg >> 24)); // Argument[31..24]
  SPI_SendByte((uint8_t)(arg >> 16)); // Argument[23..16]
  SPI_SendByte((uint8_t)(arg >> 8)); // Argument[15..8]
  SPI_SendByte((uint8_t)arg);
  n = 0x01; // Dummy CRC + Stop
  if (cmd == CMD0) {n = 0x95;} // Valid CRC for CMD0(0)
  if (cmd == CMD8) {n = 0x87;} // Valid CRC for CMD8(0x1AA)
  SPI_SendByte(n);
  n = 10; // Wait for a valid response in timeout of 10 attempts
  do {
    res = SPI_ReceiveByte();
  } while ((res & 0x80) && --n);
  return res;
}

uint8_t SD_Read_Block (uint8_t *buff, uint32_t lba)
{
  uint8_t result;
  uint16_t cnt;
  while (result!=0x00) result=SD_cmd (CMD17, lba); //CMD17 даташит стр 50 и 96
  SPI_Release();
  cnt=0;
  do //Ждем начала блока
  {
    result=SPI_ReceiveByte();
    cnt++;
  } 
  while ((result!=0xFE)&&(cnt<0xFFFF));
  if (cnt>=0xFFFF) return 5;
  for (cnt=0;cnt<512;cnt++) buff[cnt]=SPI_ReceiveByte(); //получаем байты блока из шины в буфер
  SPI_Release(); //Пропускаем контрольную сумму
  SPI_Release();
  return 0;
}

uint8_t SD_Write_Block (uint8_t *buff, uint32_t lba)
{
  uint8_t result;
  uint16_t cnt;
  //result=SD_cmd(CMD24,lba); //CMD24 даташит стр 51 и 97-98
  //arg = result;
  while (result!=0x00) result=SD_cmd(CMD24,lba); //Выйти, если результат не 0x00
  SPI_Release();
  SPI_SendByte (0xFE); //Начало буфера
  for (cnt=0;cnt<512;cnt++)
  {
    SPI_SendByte(buff[cnt]); //Данные
  }
  SPI_Release(); //Пропустим котрольную сумму
  SPI_Release();
  result=SPI_ReceiveByte();
  if ((result&0x05)!=0x05)
  {
    arg = result;
    return 2; //Выйти, если результат не 0x05 (Даташит стр 111)
  }
  cnt=0;
  do //Ждем окончания состояния BUSY
  {
    result=SPI_ReceiveByte();
    cnt++;
  } while ((result!=0xFF)&&(cnt<0xFFFF));
  if (cnt>=0xFFFF)
  {
    arg = 15;
    return 3;
  }
  return 0;
}
//-----------------------------------------------
//Также ниже создадим в нём функцию инициализации карты SD
//-----------------------------------------------
uint8_t sd_ini(void)
{
  uint8_t i, cmd;
  //uint8_t ocr[4];
  int16_t tmr;
  uint32_t temp;
  sdinfo.type = 0;
  SS_SD_DESELECT();
  for(i=0;i<10;i++) //80 импульсов (не менее 74) Даташит стр 91
  {
    SPI_Release();
  }
  SS_SD_SELECT();
  if (SD_cmd(CMD0, 0) == 1) // Enter Idle state
  {
    SPI_Release();
    if (SD_cmd(CMD8, 0x1AA) == 1) // SDv2
    {
      for (i = 0; i < 4; i++) ocr[i] = SPI_ReceiveByte();
      if (ocr[2] == 0x01 && ocr[3] == 0xAA) // The card can work at vdd range of 2.7-3.6V
      {
        for (tmr = 12000; tmr && SD_cmd(ACMD41, 1UL << 30); tmr--);
        if (tmr && SD_cmd(CMD58, 0) == 0) // Check CCS bit in the OCR
        {  
          for (i = 0; i < 4; i++) ocr[i] = SPI_ReceiveByte();
        }
        sdinfo.type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; // SDv2 (HC or SC)
      }
    } 
  }
  else
  {
    if (SD_cmd(ACMD41, 0) <= 1)
    {
      sdinfo.type = CT_SD1; cmd = ACMD41; // SDv1
    }
    else
    {
      sdinfo.type = CT_MMC;
      cmd = CMD1; // MMCv3
    }
    for (tmr = 25000; tmr && SD_cmd(cmd, 0); tmr--) ; // Wait for leaving idle state
    if (!tmr || SD_cmd(CMD16, 512) != 0) // Set R/W block length to 512
    {
      sdinfo.type = 0;
    }
  }
  sprintf(str1,"Type SD: 0x%02Xrn",sdinfo.type);
  return 0;
}
//-----------------------------------------------