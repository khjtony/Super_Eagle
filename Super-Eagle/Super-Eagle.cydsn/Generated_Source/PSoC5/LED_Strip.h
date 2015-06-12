/*******************************************************************************
* File Name: LED_Strip.h
* Version 2.50
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_LED_Strip_H)
#define CY_SPIM_LED_Strip_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component SPI_Master_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define LED_Strip_INTERNAL_CLOCK             (1u)

#if(0u != LED_Strip_INTERNAL_CLOCK)
    #include "LED_Strip_IntClock.h"
#endif /* (0u != LED_Strip_INTERNAL_CLOCK) */

#define LED_Strip_MODE                       (1u)
#define LED_Strip_DATA_WIDTH                 (8u)
#define LED_Strip_MODE_USE_ZERO              (1u)
#define LED_Strip_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define LED_Strip_TX_BUFFER_SIZE             (64u)
#define LED_Strip_RX_BUFFER_SIZE             (64u)
#define LED_Strip_INTERNAL_TX_INT_ENABLED    (1u)
#define LED_Strip_INTERNAL_RX_INT_ENABLED    (1u)

#define LED_Strip_SINGLE_REG_SIZE            (8u)
#define LED_Strip_USE_SECOND_DATAPATH        (LED_Strip_DATA_WIDTH > LED_Strip_SINGLE_REG_SIZE)

#define LED_Strip_FIFO_SIZE                  (4u)
#define LED_Strip_TX_SOFTWARE_BUF_ENABLED    ((0u != LED_Strip_INTERNAL_TX_INT_ENABLED) && \
                                                     (LED_Strip_TX_BUFFER_SIZE > LED_Strip_FIFO_SIZE))

#define LED_Strip_RX_SOFTWARE_BUF_ENABLED    ((0u != LED_Strip_INTERNAL_RX_INT_ENABLED) && \
                                                     (LED_Strip_RX_BUFFER_SIZE > LED_Strip_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
} LED_Strip_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  LED_Strip_Init(void)                           ;
void  LED_Strip_Enable(void)                         ;
void  LED_Strip_Start(void)                          ;
void  LED_Strip_Stop(void)                           ;

void  LED_Strip_EnableTxInt(void)                    ;
void  LED_Strip_EnableRxInt(void)                    ;
void  LED_Strip_DisableTxInt(void)                   ;
void  LED_Strip_DisableRxInt(void)                   ;

void  LED_Strip_Sleep(void)                          ;
void  LED_Strip_Wakeup(void)                         ;
void  LED_Strip_SaveConfig(void)                     ;
void  LED_Strip_RestoreConfig(void)                  ;

void  LED_Strip_SetTxInterruptMode(uint8 intSrc)     ;
void  LED_Strip_SetRxInterruptMode(uint8 intSrc)     ;
uint8 LED_Strip_ReadTxStatus(void)                   ;
uint8 LED_Strip_ReadRxStatus(void)                   ;
void  LED_Strip_WriteTxData(uint8 txData)  \
                                                            ;
uint8 LED_Strip_ReadRxData(void) \
                                                            ;
uint8 LED_Strip_GetRxBufferSize(void)                ;
uint8 LED_Strip_GetTxBufferSize(void)                ;
void  LED_Strip_ClearRxBuffer(void)                  ;
void  LED_Strip_ClearTxBuffer(void)                  ;
void  LED_Strip_ClearFIFO(void)                              ;
void  LED_Strip_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != LED_Strip_BIDIRECTIONAL_MODE)
    void  LED_Strip_TxEnable(void)                   ;
    void  LED_Strip_TxDisable(void)                  ;
#endif /* (0u != LED_Strip_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(LED_Strip_TX_ISR);
CY_ISR_PROTO(LED_Strip_RX_ISR);


/***************************************
*   Variable with external linkage
***************************************/

extern uint8 LED_Strip_initVar;


/***************************************
*           API Constants
***************************************/

#define LED_Strip_TX_ISR_NUMBER     ((uint8) (LED_Strip_TxInternalInterrupt__INTC_NUMBER))
#define LED_Strip_RX_ISR_NUMBER     ((uint8) (LED_Strip_RxInternalInterrupt__INTC_NUMBER))

#define LED_Strip_TX_ISR_PRIORITY   ((uint8) (LED_Strip_TxInternalInterrupt__INTC_PRIOR_NUM))
#define LED_Strip_RX_ISR_PRIORITY   ((uint8) (LED_Strip_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define LED_Strip_INT_ON_SPI_DONE    ((uint8) (0u   << LED_Strip_STS_SPI_DONE_SHIFT))
#define LED_Strip_INT_ON_TX_EMPTY    ((uint8) (0u   << LED_Strip_STS_TX_FIFO_EMPTY_SHIFT))
#define LED_Strip_INT_ON_TX_NOT_FULL ((uint8) (1u << \
                                                                           LED_Strip_STS_TX_FIFO_NOT_FULL_SHIFT))
#define LED_Strip_INT_ON_BYTE_COMP   ((uint8) (0u  << LED_Strip_STS_BYTE_COMPLETE_SHIFT))
#define LED_Strip_INT_ON_SPI_IDLE    ((uint8) (0u   << LED_Strip_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define LED_Strip_INT_ON_TX_NOT_FULL_DEF ((LED_Strip_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (LED_Strip_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define LED_Strip_TX_INIT_INTERRUPTS_MASK    (LED_Strip_INT_ON_SPI_DONE  | \
                                                     LED_Strip_INT_ON_TX_EMPTY  | \
                                                     LED_Strip_INT_ON_TX_NOT_FULL_DEF | \
                                                     LED_Strip_INT_ON_BYTE_COMP | \
                                                     LED_Strip_INT_ON_SPI_IDLE)

#define LED_Strip_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          LED_Strip_STS_RX_FIFO_FULL_SHIFT))
#define LED_Strip_INT_ON_RX_NOT_EMPTY    ((uint8) (1u << \
                                                                          LED_Strip_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define LED_Strip_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          LED_Strip_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define LED_Strip_RX_INIT_INTERRUPTS_MASK    (LED_Strip_INT_ON_RX_FULL      | \
                                                     LED_Strip_INT_ON_RX_NOT_EMPTY | \
                                                     LED_Strip_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define LED_Strip_BITCTR_INIT            (((uint8) (LED_Strip_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/
#if(CY_PSOC3 || CY_PSOC5)
    #define LED_Strip_TXDATA_REG (* (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F0_REG)
    #define LED_Strip_TXDATA_PTR (  (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F0_REG)
    #define LED_Strip_RXDATA_REG (* (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F1_REG)
    #define LED_Strip_RXDATA_PTR (  (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(LED_Strip_USE_SECOND_DATAPATH)
        #define LED_Strip_TXDATA_REG (* (reg16 *) \
                                          LED_Strip_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define LED_Strip_TXDATA_PTR (  (reg16 *) \
                                          LED_Strip_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define LED_Strip_RXDATA_REG (* (reg16 *) \
                                          LED_Strip_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define LED_Strip_RXDATA_PTR (  (reg16 *) \
                                          LED_Strip_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define LED_Strip_TXDATA_REG (* (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F0_REG)
        #define LED_Strip_TXDATA_PTR (  (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F0_REG)
        #define LED_Strip_RXDATA_REG (* (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F1_REG)
        #define LED_Strip_RXDATA_PTR (  (reg8 *) \
                                                LED_Strip_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (LED_Strip_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define LED_Strip_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        LED_Strip_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define LED_Strip_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        LED_Strip_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(LED_Strip_USE_SECOND_DATAPATH)
    #define LED_Strip_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        LED_Strip_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define LED_Strip_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        LED_Strip_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (LED_Strip_USE_SECOND_DATAPATH) */

#define LED_Strip_COUNTER_PERIOD_REG     (* (reg8 *) LED_Strip_BSPIM_BitCounter__PERIOD_REG)
#define LED_Strip_COUNTER_PERIOD_PTR     (  (reg8 *) LED_Strip_BSPIM_BitCounter__PERIOD_REG)
#define LED_Strip_COUNTER_CONTROL_REG    (* (reg8 *) LED_Strip_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define LED_Strip_COUNTER_CONTROL_PTR    (  (reg8 *) LED_Strip_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define LED_Strip_TX_STATUS_REG          (* (reg8 *) LED_Strip_BSPIM_TxStsReg__STATUS_REG)
#define LED_Strip_TX_STATUS_PTR          (  (reg8 *) LED_Strip_BSPIM_TxStsReg__STATUS_REG)
#define LED_Strip_RX_STATUS_REG          (* (reg8 *) LED_Strip_BSPIM_RxStsReg__STATUS_REG)
#define LED_Strip_RX_STATUS_PTR          (  (reg8 *) LED_Strip_BSPIM_RxStsReg__STATUS_REG)

#define LED_Strip_CONTROL_REG            (* (reg8 *) \
                                      LED_Strip_BSPIM_BidirMode_CtrlReg__CONTROL_REG)
#define LED_Strip_CONTROL_PTR            (  (reg8 *) \
                                      LED_Strip_BSPIM_BidirMode_CtrlReg__CONTROL_REG)

#define LED_Strip_TX_STATUS_MASK_REG     (* (reg8 *) LED_Strip_BSPIM_TxStsReg__MASK_REG)
#define LED_Strip_TX_STATUS_MASK_PTR     (  (reg8 *) LED_Strip_BSPIM_TxStsReg__MASK_REG)
#define LED_Strip_RX_STATUS_MASK_REG     (* (reg8 *) LED_Strip_BSPIM_RxStsReg__MASK_REG)
#define LED_Strip_RX_STATUS_MASK_PTR     (  (reg8 *) LED_Strip_BSPIM_RxStsReg__MASK_REG)

#define LED_Strip_TX_STATUS_ACTL_REG     (* (reg8 *) LED_Strip_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define LED_Strip_TX_STATUS_ACTL_PTR     (  (reg8 *) LED_Strip_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define LED_Strip_RX_STATUS_ACTL_REG     (* (reg8 *) LED_Strip_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define LED_Strip_RX_STATUS_ACTL_PTR     (  (reg8 *) LED_Strip_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(LED_Strip_USE_SECOND_DATAPATH)
    #define LED_Strip_AUX_CONTROLDP1     (LED_Strip_AUX_CONTROL_DP1_REG)
#endif /* (LED_Strip_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define LED_Strip_STS_SPI_DONE_SHIFT             (0x00u)
#define LED_Strip_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define LED_Strip_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define LED_Strip_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define LED_Strip_STS_SPI_IDLE_SHIFT             (0x04u)
#define LED_Strip_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define LED_Strip_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define LED_Strip_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define LED_Strip_STS_SPI_DONE           ((uint8) (0x01u << LED_Strip_STS_SPI_DONE_SHIFT))
#define LED_Strip_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << LED_Strip_STS_TX_FIFO_EMPTY_SHIFT))
#define LED_Strip_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << LED_Strip_STS_TX_FIFO_NOT_FULL_SHIFT))
#define LED_Strip_STS_BYTE_COMPLETE      ((uint8) (0x01u << LED_Strip_STS_BYTE_COMPLETE_SHIFT))
#define LED_Strip_STS_SPI_IDLE           ((uint8) (0x01u << LED_Strip_STS_SPI_IDLE_SHIFT))
#define LED_Strip_STS_RX_FIFO_FULL       ((uint8) (0x01u << LED_Strip_STS_RX_FIFO_FULL_SHIFT))
#define LED_Strip_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << LED_Strip_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define LED_Strip_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << LED_Strip_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define LED_Strip_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define LED_Strip_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define LED_Strip_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define LED_Strip_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define LED_Strip_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define LED_Strip_FIFO_CLR       (LED_Strip_TX_FIFO_CLR | LED_Strip_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define LED_Strip_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define LED_Strip_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define LED_Strip_AUX_CTRL_FIFO0_CLR         (0x01u)
#define LED_Strip_AUX_CTRL_FIFO1_CLR         (0x02u)
#define LED_Strip_AUX_CTRL_FIFO0_LVL         (0x04u)
#define LED_Strip_AUX_CTRL_FIFO1_LVL         (0x08u)
#define LED_Strip_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define LED_Strip_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define LED_Strip_IS_ENABLED (0u != (LED_Strip_TX_STATUS_ACTL_REG & LED_Strip_INT_ENABLE))

/* Retuns TX status register */
#define LED_Strip_GET_STATUS_TX(swTxSts) ( (uint8)(LED_Strip_TX_STATUS_REG | \
                                                          ((swTxSts) & LED_Strip_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define LED_Strip_GET_STATUS_RX(swRxSts) ( (uint8)(LED_Strip_RX_STATUS_REG | \
                                                          ((swRxSts) & LED_Strip_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define LED_Strip_WriteByte   LED_Strip_WriteTxData
#define LED_Strip_ReadByte    LED_Strip_ReadRxData
void  LED_Strip_SetInterruptMode(uint8 intSrc)       ;
uint8 LED_Strip_ReadStatus(void)                     ;
void  LED_Strip_EnableInt(void)                      ;
void  LED_Strip_DisableInt(void)                     ;

#define LED_Strip_TXDATA                 (LED_Strip_TXDATA_REG)
#define LED_Strip_RXDATA                 (LED_Strip_RXDATA_REG)
#define LED_Strip_AUX_CONTROLDP0         (LED_Strip_AUX_CONTROL_DP0_REG)
#define LED_Strip_TXBUFFERREAD           (LED_Strip_txBufferRead)
#define LED_Strip_TXBUFFERWRITE          (LED_Strip_txBufferWrite)
#define LED_Strip_RXBUFFERREAD           (LED_Strip_rxBufferRead)
#define LED_Strip_RXBUFFERWRITE          (LED_Strip_rxBufferWrite)

#define LED_Strip_COUNTER_PERIOD         (LED_Strip_COUNTER_PERIOD_REG)
#define LED_Strip_COUNTER_CONTROL        (LED_Strip_COUNTER_CONTROL_REG)
#define LED_Strip_STATUS                 (LED_Strip_TX_STATUS_REG)
#define LED_Strip_CONTROL                (LED_Strip_CONTROL_REG)
#define LED_Strip_STATUS_MASK            (LED_Strip_TX_STATUS_MASK_REG)
#define LED_Strip_STATUS_ACTL            (LED_Strip_TX_STATUS_ACTL_REG)

#define LED_Strip_INIT_INTERRUPTS_MASK  (LED_Strip_INT_ON_SPI_DONE     | \
                                                LED_Strip_INT_ON_TX_EMPTY     | \
                                                LED_Strip_INT_ON_TX_NOT_FULL_DEF  | \
                                                LED_Strip_INT_ON_RX_FULL      | \
                                                LED_Strip_INT_ON_RX_NOT_EMPTY | \
                                                LED_Strip_INT_ON_RX_OVER      | \
                                                LED_Strip_INT_ON_BYTE_COMP)
                                                
#define LED_Strip_DataWidth                  (LED_Strip_DATA_WIDTH)
#define LED_Strip_InternalClockUsed          (LED_Strip_INTERNAL_CLOCK)
#define LED_Strip_InternalTxInterruptEnabled (LED_Strip_INTERNAL_TX_INT_ENABLED)
#define LED_Strip_InternalRxInterruptEnabled (LED_Strip_INTERNAL_RX_INT_ENABLED)
#define LED_Strip_ModeUseZero                (LED_Strip_MODE_USE_ZERO)
#define LED_Strip_BidirectionalMode          (LED_Strip_BIDIRECTIONAL_MODE)
#define LED_Strip_Mode                       (LED_Strip_MODE)
#define LED_Strip_DATAWIDHT                  (LED_Strip_DATA_WIDTH)
#define LED_Strip_InternalInterruptEnabled   (0u)

#define LED_Strip_TXBUFFERSIZE   (LED_Strip_TX_BUFFER_SIZE)
#define LED_Strip_RXBUFFERSIZE   (LED_Strip_RX_BUFFER_SIZE)

#define LED_Strip_TXBUFFER       LED_Strip_txBuffer
#define LED_Strip_RXBUFFER       LED_Strip_rxBuffer

#endif /* (CY_SPIM_LED_Strip_H) */


/* [] END OF FILE */
