#ifndef __CMT2300A_H
#define __CMT2300A_H

#include "typedefs.h"
#include "cmt2300a_defs.h"
#include "EasyCMT2300.h"
#include "cmt2300a_params.h"

#ifdef __cplusplus 
extern "C" { 
#endif

#define ENABLE_AUTO_SWITCH_CHIP_STATUS   /* Enable the auto switch chip status */

/* ************************************************************************
   The following are for chip status controls.
*  ************************************************************************ */
void CMT2300A_SoftReset(void);
u8 CMT2300A_GetChipStatus(void);
BOOL CMT2300A_AutoSwitchStatus(u8 nGoCmd);
BOOL CMT2300A_GoSleep(void);
BOOL CMT2300A_GoStby(void);
BOOL CMT2300A_GoTFS(void);
BOOL CMT2300A_GoRFS(void);
BOOL CMT2300A_GoTx(void);
BOOL CMT2300A_GoRx(void);


/* ************************************************************************
*  The following are for chip interrupts, GPIO, FIFO operations.
*  ************************************************************************ */
void CMT2300A_ConfigGpio(u8 nGpioSel);
void CMT2300A_ConfigInterrupt(u8 nInt1Sel, u8 nInt2Sel);
void CMT2300A_SetInterruptPolar(BOOL bActiveHigh);
void CMT2300A_SetFifoThreshold(u8 nFifoThreshold);
void CMT2300A_EnableAntennaSwitch(u8 nMode);
void CMT2300A_EnableInterrupt(u8 nEnable);
void CMT2300A_EnableRxFifoAutoClear(BOOL bEnable);
void CMT2300A_EnableFifoMerge(BOOL bEnable);
void CMT2300A_EnableReadFifo(void);
void CMT2300A_EnableWriteFifo(void);
void CMT2300A_RestoreFifo(void);
u8 CMT2300A_ClearTxFifo(void);
u8 CMT2300A_ClearRxFifo(void);
u8 CMT2300A_ClearInterruptFlags(void);


/* ************************************************************************
*  The following are for Tx DIN operations in direct mode.
*  ************************************************************************ */
void CMT2300A_ConfigTxDin(u8 nDinSel);
void CMT2300A_EnableTxDin(BOOL bEnable);
void CMT2300A_EnableTxDinInvert(BOOL bEnable);


/* ************************************************************************
*  The following are general operations.
*  ************************************************************************ */
BOOL CMT2300A_IsExist(void);
u8 CMT2300A_GetRssiCode(void);
int CMT2300A_GetRssiDBm(void);
void CMT2300A_SetFrequencyChannel(u8 nChann);
void CMT2300A_SetFrequencyStep(u8 nOffset);
void CMT2300A_SetPayloadLength(u16 nLength);
void CMT2300A_EnableLfosc(BOOL bEnable);
void CMT2300A_EnableLfoscOutput(BOOL bEnable);
void CMT2300A_EnableAfc(BOOL bEnable);
void CMT2300A_SetAfcOvfTh(u8 afcOvfTh);


/* ************************************************************************
*  The following are for chip initializes.
*  ************************************************************************ */
void CMT2300A_Init(void);
BOOL CMT2300A_ConfigRegBank(u8 base_addr, const u8 bank[], u8 len);

#ifdef __cplusplus 
} 
#endif

#endif


