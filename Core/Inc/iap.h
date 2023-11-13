#ifndef __IAP_H__
#define __IAP_H__

#define APP_UPDATE 0x5A
#define APP_RUN 0xA5

#define APP_START_ADDR ((uint32_t)0x08008000)

typedef void (*pFunction)(void);

void LoadApplication(uint32_t AppAddr);

#endif
