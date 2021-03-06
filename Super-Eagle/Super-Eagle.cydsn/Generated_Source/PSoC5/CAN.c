/*******************************************************************************
* File Name: CAN.c
* Version 2.30
*
* Description:
*  The CAN Component provide functionality of Control Area Network.
*  Two types of mailbox configurations available "Full" and "Basic".
*
* Note:
*  CAN configuration puts as constant to ROM and could change only directly
*  registers write by user.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CAN.h"

uint8 CAN_initVar = 0u;


/*******************************************************************************
* FUNCTION NAME:   CAN_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default CAN configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  Indication whether the configuration has been accepted or rejected.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
* Side Effects:
*  All registers will be reset to their initial values. This will re-initialize 
*  the component with the following exceptions - it will not clear data from 
*  the mailboxes.
*  Enable power to the CAN Core.
*
*******************************************************************************/
uint8 CAN_Init(void) 
{
    /* Iniital values of CAN RX and TX registers */
    static const CAN_RX_CFG CYCODE CAN_RXConfigStruct[] =
    {
        { 0u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 1u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 2u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 3u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 4u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 5u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 6u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 7u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 8u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 9u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 10u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 11u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 12u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 13u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 14u, 0x8000A8u, 0xFFFFFFFFu, 0x0u },
        { 15u, 0x8000A8u, 0xFFFFFFFFu, 0x0u }
    };

    static const CAN_TX_CFG CYCODE CAN_TXConfigStruct[] =
    {
        { 0u, 0x880008u, 0x0u },
        { 1u, 0x880008u, 0x0u },
        { 2u, 0x880008u, 0x0u },
        { 3u, 0x880008u, 0x0u },
        { 4u, 0x880008u, 0x0u },
        { 5u, 0x880008u, 0x0u },
        { 6u, 0x880008u, 0x0u },
        { 7u, 0x880008u, 0x0u }
    };
    
    uint8 i;
    uint8 result = CAN_FAIL;
    uint8 localResult = CAN_FAIL;
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    
    /* Enable CAN block in Active mode */
    CAN_PM_ACT_CFG_REG |= CAN_ACT_PWR_EN;
        
    /* Enable CAN block in Alternate Active (Standby) mode */
    CAN_PM_STBY_CFG_REG |= CAN_STBY_PWR_EN; 
    
    CyExitCriticalSection(enableInterrupts);
    
    /* Sets the CAN controller to stop mode */
    CAN_CMD_REG.byte[0u] &= (uint8) (~CAN_MODE_MASK);

    /* Verify that bit is cleared */
    if ((CAN_CMD_REG.byte[0u] & CAN_MODE_MASK) == 0u)
    {                
        /* Disable Interrupt. */
        CyIntDisable(CAN_ISR_NUMBER);
        
        /* Set the ISR to point to the CAN_ISR Interrupt. */
        (void) CyIntSetVector(CAN_ISR_NUMBER, & CAN_ISR);
        
        /* Set the priority. */
        CyIntSetPriority(CAN_ISR_NUMBER, CAN_ISR_PRIORITY);

        if (CAN_SetPreScaler(CAN_BITRATE) == CYRET_SUCCESS)
        {
            if (CAN_SetArbiter(CAN_ARBITER) == CYRET_SUCCESS)
            {
                if (CAN_SetTsegSample(CAN_CFG_REG_TSEG1, CAN_CFG_REG_TSEG2,
                    CAN_CFG_REG_SJW, CAN_SAMPLING_MODE) == CYRET_SUCCESS)
                {
                    if (CAN_SetRestartType(CAN_RESET_TYPE) == CYRET_SUCCESS)
                    {
                        if (CAN_SetEdgeMode(CAN_SYNC_EDGE) == CYRET_SUCCESS)
                        {
                            localResult = CYRET_SUCCESS;
                        }
                    }
                }
            }
        }
        
        if (localResult == CYRET_SUCCESS)
        {
            /* Initialize TX maiboxes */
            for (i = 0u; i < CAN_NUMBER_OF_TX_MAILBOXES; i++)
            {
                if (CAN_TxBufConfig((const CAN_TX_CFG *)
                    (&CAN_TXConfigStruct[i])) != CYRET_SUCCESS)
                {
                    localResult = CAN_FAIL;
                    break;
                }
            }

            if (localResult == CYRET_SUCCESS)
            {
                /* Initialize RX mailboxes */
                for (i = 0u; i < CAN_NUMBER_OF_RX_MAILBOXES; i++)
                {
                    if (CAN_RxBufConfig((const CAN_RX_CFG *)
                        (&CAN_RXConfigStruct[i])) != CYRET_SUCCESS)
                    {
                        localResult = CAN_FAIL;
                        break;
                    }
                }

                if (localResult == CYRET_SUCCESS)
                {
                    /* Write IRQ Mask */
                    if (CAN_SetIrqMask(CAN_INIT_INTERRUPT_MASK) ==
                        CYRET_SUCCESS)
                    {
                        /* Active mode always */
                        if (CAN_SetOpMode(CAN_ACTIVE_MODE) == CYRET_SUCCESS)
                        {
                            result = CYRET_SUCCESS;
                        }
                    }
                }
            }
        }
                            
    }

    return (result);
}


/*******************************************************************************
* Function Name: CAN_Enable
********************************************************************************
*
* Summary:   
*  This function enable CAN Component and ISR of CAN Component. 
*
* Parameters:  
*  None.
*
* Return: 
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_Enable(void) 
{
    uint8 result = CAN_FAIL;
    uint8 enableInterrupts;
    
    enableInterrupts = CyEnterCriticalSection();
    
    /* Enable CAN block in Active mode */
    CAN_PM_ACT_CFG_REG |= CAN_ACT_PWR_EN;

    /* Enable CAN block in Alternate Active (Standby) mode */
    CAN_PM_STBY_CFG_REG |= CAN_STBY_PWR_EN;
    
    CyExitCriticalSection(enableInterrupts);
    
    /* Enable isr */
    CyIntEnable(CAN_ISR_NUMBER);


    /* Sets the CAN controller to run mode */
    CY_SET_REG32(CAN_CMD_PTR, (CY_GET_REG32(CAN_CMD_PTR) | CAN_MODE_MASK));
    
    /* Verify that bit is set */
    if (0u != (CY_GET_REG32(CAN_CMD_PTR) & CAN_MODE_MASK))
    {
        result = CYRET_SUCCESS;
    }
    
    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_Start
********************************************************************************
*
* Summary:
*  This function sets CAN Component into Run mode. Starts the Rate Counter if
*  polling mailboxes available.
*
* Parameters:
*  None. 
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
* Global variables:
*  CAN_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 CAN_Start(void) 
{
    uint8 result = CYRET_SUCCESS;
    
    if (CAN_initVar == 0u)
    {
        result = CAN_Init();
        CAN_initVar = 1u;        
    }
    
    if (result == CYRET_SUCCESS)
    {
        result = CAN_Enable();
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_Stop
********************************************************************************
*
* Summary:
*  This function sets CAN Component into Stop mode.
*
* Parameters:
*  None.
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
* Side Effects:
*  Disable power to the CAN Core.
*
*******************************************************************************/
uint8 CAN_Stop(void) 
{
    uint8 result = CAN_FAIL;
    uint8 enableInterrupts;        
    
    /* Sets the CAN controller to stop mode */
    CY_SET_REG32(CAN_CMD_PTR, (CY_GET_REG32(CAN_CMD_PTR) & 
                                            ((uint8) (~CAN_MODE_MASK))));

    /* Verify that bit is cleared */
    if (0u == (CY_GET_REG32(CAN_CMD_PTR) & CAN_MODE_MASK))
    {
        result = CYRET_SUCCESS;
    }
    
    /* Disable isr */
    CyIntDisable(CAN_ISR_NUMBER);

    enableInterrupts = CyEnterCriticalSection(); 

    /* Disable CAN block in Active mode */
    CAN_PM_ACT_CFG_REG &= (uint8) (~CAN_ACT_PWR_EN);

    /* Disable CAN block in Alternate Active (Standby) mode template */
    CAN_PM_STBY_CFG_REG &= (uint8) (~CAN_STBY_PWR_EN);
    
    CyExitCriticalSection(enableInterrupts);

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_GlobalIntEnable
********************************************************************************
*
* Summary:
*  This function enables Global Interrupts from CAN Core.
*
* Parameters:
*  None.
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_GlobalIntEnable(void) 
{
    uint8 result = CAN_FAIL;

    CAN_INT_EN_REG.byte[0u] |= CAN_GLOBAL_INT_MASK;

    /* Verify that bit is set */
    if ((CAN_INT_EN_REG.byte[0u] & CAN_GLOBAL_INT_MASK) == CAN_GLOBAL_INT_MASK)
    {
        result = CYRET_SUCCESS;
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_GlobalIntDisable
********************************************************************************
*
* Summary:
*  This function disables Global Interrupts from CAN Core.
*
* Parameters:
*  None.
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_GlobalIntDisable(void) 
{
    uint8 result = CAN_FAIL;

    CAN_INT_EN_REG.byte[0u] &= (uint8) (~CAN_GLOBAL_INT_MASK);

    /* Verify that bit is cleared */
    if ((CAN_INT_EN_REG.byte[0u] & CAN_GLOBAL_INT_MASK) == 0u)
    {
        result = CYRET_SUCCESS;
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_SetPreScaler
********************************************************************************
*
* Summary:
*  This function sets PreScaler for generation the time quantum which defines
*  the time quanta. Value between 0x0 and 0x7FFF are valid.
*
* Parameters:
*  bitrate: PreScaler value.
*   Value           Description
*    0               One time quantum equals 1 clock cycle
*    1               One time quantum equals 2 clock cycles
*    ...             ...
*    32767           One time quantum equals 32768 clock cycles
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*    CAN_OUT_OF_RANGE      Function parameter is out of range
*
*******************************************************************************/
uint8 CAN_SetPreScaler(uint16 bitrate) 
{
    uint8 result = CAN_OUT_OF_RANGE;

    if (bitrate <= CAN_BITRATE_MASK)
    {
        result = CAN_FAIL;

        /* Set prescaler */
        CY_SET_REG16((reg16 *) &CAN_CFG_REG.byte[2u], bitrate);

        /* Verify that prescaler is set */
        if (CY_GET_REG16((reg16 *) &CAN_CFG_REG.byte[2u]) == bitrate)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_SetArbiter
********************************************************************************
*
* Summary:
*  This function sets arbitration type for transmit mailboxes. Types of
*  arbiters are Round Robin and Fixed priority. Value 0 and 1 are valid.
*
* Parameters:
*  arbiter: Arbitaration type for transmit mailboxes.
*   Value                              Description
*    CAN_ROUND_ROBIN       Round robin arbitration
*    CAN_FIXED_PRIORITY    Fixed priority arbitration.
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_SetArbiter(uint8 arbiter) 
{
    uint8 result = CAN_FAIL;

    if (arbiter == CAN_ROUND_ROBIN)
    {
        CAN_CFG_REG.byte[1u] &= (uint8) (~CAN_ARBITRATION_MASK);    /* Round Robin */

        /* Verify that bit is cleared */
        if ((CAN_CFG_REG.byte[1u] & CAN_ARBITRATION_MASK) == 0u)
        {
            result = CYRET_SUCCESS;
        }        
    }
    else
    {
        CAN_CFG_REG.byte[1u] |= CAN_ARBITRATION_MASK;    /* Fixed priority */

        /* Verify that bit is set */
        if ((CAN_CFG_REG.byte[1u] & CAN_ARBITRATION_MASK) ==
            CAN_ARBITRATION_MASK)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME: CAN_SetTsegSample
********************************************************************************
*
* Summary:
*  This function configures: Time segment 1, Time segment 2, Sampling Mode
*  and Synchronization Jump Width.
*
* Parameters:
*  cfgTseg1: Length of time segment 1, value between 0x2 and 0xF are valid;
*  cfgTseg2: Length of time segment 2, value between 0x1 and 0x7 are valid;
*  sjw: Synchronization Jump Width, value between 0x0 and 0x3 are valid;
*  sm: Sampling Mode.
*   Define                               Description
*   CAN_ONE_SAMPLE_POINT     one sampling points is used
*   CAN_THREE_SAMPLE_POINTS  three sampling points are used
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*    CAN_OUT_OF_RANGE      Function parameter is out of range
*
*******************************************************************************/
uint8 CAN_SetTsegSample(uint8 cfgTseg1, uint8 cfgTseg2, uint8 sjw, uint8 sm)
                                     
{
    uint8 result = CAN_OUT_OF_RANGE;
    uint8 cfgTemp;

    if ((cfgTseg1 >= CAN_CFG_REG_TSEG1_LOWER_LIMIT) && (cfgTseg1 <=
         CAN_CFG_REG_TSEG1_UPPER_LIMIT))
    {
        if (((cfgTseg2 >= CAN_CFG_REG_TSEG2_LOWER_LIMIT) &&
             (cfgTseg2 <= CAN_CFG_REG_TSEG2_UPPER_LIMIT)) || ((sm == CAN_ONE_SAMPLE_POINT) &&
             (cfgTseg2 == CAN_CFG_REG_TSEG2_EXCEPTION)))
        {
            if ((sjw <= CAN_CFG_REG_SJW_LOWER_LIMIT) && (sjw <= cfgTseg1) && (sjw <= cfgTseg2))
            {
                result = CAN_FAIL;

                cfgTemp = CAN_CFG_REG.byte[1];
                cfgTemp &= (uint8) (~CAN_CFG_REG_TSEG1_MASK);
                cfgTemp |= cfgTseg1;

                /* write register byte 1 */
                CAN_CFG_REG.byte[1u] = cfgTemp;

                /* Verify 2nd byte of CAN_CFG_REG register */
                if (CAN_CFG_REG.byte[1u] == cfgTemp)
                {
                    cfgTemp = 0u;
                    /* Set appropriate bits */
                    if (sm != CAN_ONE_SAMPLE_POINT)
                    {
                        cfgTemp = CAN_SAMPLE_MODE_MASK;
                    }
                    cfgTemp |= ((uint8) (cfgTseg2 << CAN_CFG_REG_TSEG2_SHIFT)) |
                               ((uint8) (sjw << CAN_CFG_REG_SJW_SHIFT));

                    /* write register byte 0 */
                    CAN_CFG_REG.byte[0u] = cfgTemp;

                    /* Verify 1st byte of CAN_CFG_REG register */
                    if (CAN_CFG_REG.byte[0u] == cfgTemp)
                    {
                        result = CYRET_SUCCESS;
                    }
                }
            }
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_SetRestartType
********************************************************************************
*
* Summary:
*  This function sets Reset type. Types of Reset are Automatic and Manual.
*  Manual Reset is recommended setting.
*
* Parameters:
*  reset: Reset Type.
*   Define                             Description
*    CAN_MANUAL_RESTART    After bus-off, the CAN must be restarted
*                                       by hand. This is the recommended 
*                                       setting.
*    CAN_AUTO_RESTART      After bus-off, the CAN is restarting 
*                                       automatically after 128 groups of 11 
*                                       recessive bits.
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_SetRestartType(uint8 reset) 
{
    uint8 result = CAN_FAIL;

    if (reset == CAN_RESTART_BY_HAND)
    {
        CAN_CFG_REG.byte[0u] &= (uint8) (~CAN_RESET_MASK);   /* Manual reset */

        /* Verify that bit is cleared */
        if ((CAN_CFG_REG.byte[0u] & CAN_RESET_MASK) == 0u)
        {
            result = CYRET_SUCCESS;
        }
    }
    else
    {
        CAN_CFG_REG.byte[0u] |= CAN_RESET_MASK;    /* Automatic restart */

        /* Verify that bit is set */
        if ((CAN_CFG_REG.byte[0u] & CAN_RESET_MASK) == CAN_RESET_MASK)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_SetEdgeMode
********************************************************************************
*
* Summary:
*  This function sets Edge Mode. Modes are 'R' to 'D'(Reccessive to Dominant)
*  and Both edges are used.
*
* Parameters:
*  edge: Edge Mode.
*   Define                             Description
*    CAN_EDGE_R_TO_D       Edge from R to D is used for 
*                                       synchronization
*    CAN_BOTH_EDGES        Both edges are used
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_SetEdgeMode(uint8 edge) 
{
    uint8 result = CAN_FAIL;

    if (edge == CAN_EDGE_R_TO_D)
    {
        /* Recessive to Dominant is used for synchronization */
        CAN_CFG_REG.byte[0u] &= (uint8) (~CAN_EDGE_MODE_MASK);

        /* Verify that bit is cleared */
        if ((CAN_CFG_REG.byte[0u] & CAN_EDGE_MODE_MASK) == 0u)
        {
            result = CYRET_SUCCESS;
        }                      
    }
    else
    {
        /* Both edges are to be used */
        CAN_CFG_REG.byte[0u] |= CAN_EDGE_MODE_MASK;

        /* Verify that bit is set */
        if ((CAN_CFG_REG.byte[0u] & CAN_EDGE_MODE_MASK) == CAN_EDGE_MODE_MASK)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_SetOpMode
********************************************************************************
*
* Summary:
*  This function sets Operation Mode. Operations Modes are Active of Listen
*  Only.
*
* Parameters:
*  opMode: Operation Mode value.
*   Define                             Description
*    CAN_ACTIVE_MODE       Active mode
*    CAN_LISTEN_ONLY       CAN listen only
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_SetOpMode(uint8 opMode) 
{
    uint8 result = CAN_FAIL;

    if (opMode == CAN_ACTIVE_MODE)
    {
        /* CAN in Active mode */
        CAN_CMD_REG.byte[0u] &= (uint8) (~CAN_OPMODE_MASK);

        /* Verify that bit is cleared */
        if ((CAN_CMD_REG.byte[0u] & CAN_OPMODE_MASK) == 0u)
        {
            result = CYRET_SUCCESS;
        }                
    }
    else
    {
        /* CAN listen only */
        CAN_CMD_REG.byte[0u] |= CAN_OPMODE_MASK;

        /* Verify that bit is set */
        if ((CAN_CMD_REG.byte[0u] & CAN_OPMODE_MASK) == CAN_OPMODE_MASK)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_RXRegisterInit
********************************************************************************
*
* Summary:
*  This function writes only receive CAN registers.
*
* Parameters:
*  regAddr: Pointer to CAN receive register;
*  config:  Value that will be written in register.
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*    CAN_OUT_OF_RANGE      Function parameter is out of range
*
*******************************************************************************/
uint8 CAN_RXRegisterInit(reg32 *regAddr, uint32 config)
                                      
{
    uint8 result = CAN_OUT_OF_RANGE;    
    
    if ((((uint32) regAddr & 0x0000FFFFu) >= ((uint32) CAN_RX_FIRST_REGISTER_PTR & 0x0000FFFFu)) &&
         ((((uint32) regAddr & 0x0000FFFFu)) <= ((uint32) CAN_RX_LAST_REGISTER_PTR & 0x0000FFFFu)))
    {
        result = CAN_FAIL;
        
        if ((((uint32) regAddr & 0x0000FFFFu) % CAN_RX_CMD_REG_WIDTH) == 0u)
        {
            config |= CAN_RX_WPN_SET;
            
    /* Disable isr */
    CyIntDisable(CAN_ISR_NUMBER);

            /* Write defined RX CMD registers */
            CY_SET_REG32(regAddr, config);
            
    /* Enable isr */
    CyIntEnable(CAN_ISR_NUMBER);

            /* Verify register */
            if ((CY_GET_REG32(regAddr) & CAN_RX_READ_BACK_MASK) ==
                (config & CAN_RX_READ_BACK_MASK))
            {
                result = CYRET_SUCCESS;
            }
        }
        /* All registers except RX CMD*/
        else
        {            
    /* Disable isr */
    CyIntDisable(CAN_ISR_NUMBER);

            /* Write defined CAN receive register */
            CY_SET_REG32(regAddr, config);
            
    /* Enable isr */
    CyIntEnable(CAN_ISR_NUMBER);

            /* Verify register */
            if (CY_GET_REG32(regAddr) == config)
            {
                result = CYRET_SUCCESS;
            }
        }
    }
    
    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_SetIrqMask
********************************************************************************
*
* Summary:
*  This function sets to enable/disable particular interrupt sources. Interrupt
*  Mask directly write to CAN Interrupt Enable register. A particular interrupt
*  source is enabled by setting its respective flag to 1.
*
* Parameters:
*  mask: Interrupt enable/disable request. 1 bit per interrupt source.
*   Define                                    Description
*    CAN_GLOBAL_INT_ENABLE        Global Interrupt Enable Flag
*    CAN_ARBITRATION_LOST_ENABLE  Arbitration Loss Interrupt Enable
*    CAN_OVERLOAD_ERROR_ENABLE    Overload Interrupt Enable
*    CAN_BIT_ERROR_ENABLE         Bit Error Interrupt Enable
*    CAN_STUFF_ERROR_ENABLE       Stuff Error Interrupt Enable
*    CAN_ACK_ERROR_ENABLE         Ack Error Interrupt Enable
*    CAN_FORM_ERROR_ENABLE        Form Error Interrupt Enable
*    CAN_CRC_ERROR_ENABLE         CRC Error Interrupt Enable
*    CAN_BUS_OFF_ENABLE           Busoff State Interrupt Enable
*    CAN_RX_MSG_LOST_ENABLE       Rx Msg Loss Interrupt Enable
*    CAN_TX_MESSAGE_ENABLE        Tx Msg Sent Interrupt Enable
*    CAN_RX_MESSAGE_ENABLE        Msg Recived Interrupt Enable
*
* Return:
*  Indication whether register is written and verified.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_SetIrqMask(uint16 mask) 
{
    uint8 result = CAN_FAIL;

    /* Write byte 0 and 1 of CAN_INT_EN_REG register */
    CY_SET_REG16((reg16 *) &CAN_INT_EN_REG, mask);

    /* Verify CAN_INT_EN_REG register */
    if (CY_GET_REG16((reg16 *) &CAN_INT_EN_REG) == mask)
    {
        result = CYRET_SUCCESS;
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_GetTXErrorflag
********************************************************************************
*
* Summary:
*  This function returns the bit that indicates if the number of TX errors
*  exceeds 0x60.
*
* Parameters:
*  None.
*
* Return:
*  Indication whether the number of TX errors exceeds 0x60.
*
*******************************************************************************/
uint8 CAN_GetTXErrorFlag(void) 
{
    /* Get the state of the transmit error flag */
    return (((CAN_ERR_SR_REG.byte[2u] & CAN_TX_ERROR_FLAG_MASK) ==
              CAN_TX_ERROR_FLAG_MASK) ? 1u : 0u);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_GetRXErrorFlag
********************************************************************************
*
* Summary:
*  This function returns the bit that indicates if the number of RX errors
*  exceeds 0x60.
*
* Parameters:
*  None.
*
* Return:
*  Indication whether the number of TX errors exceeds 0x60.
*
*******************************************************************************/
uint8 CAN_GetRXErrorFlag(void) 
{
    /* Get the state of the receive error flag */
    return (((CAN_ERR_SR_REG.byte[2u] & CAN_RX_ERROR_FLAG_MASK) ==
              CAN_RX_ERROR_FLAG_MASK) ? 1u : 0u);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_TXErrorCount
********************************************************************************
*
* Summary:
*  This function returns the number of Transmit Errors.
*
* Parameters:
*  None.
*
* Return:
*  Number of Transmit Errors.
*
*******************************************************************************/
uint8 CAN_GetTXErrorCount(void) 
{
    /* Get the state of the transmit error count */
    return (CAN_ERR_SR_REG.byte[0u]);    /* bits 7-0 */
}


/*******************************************************************************
* FUNCTION NAME:   CAN_RXErrorCount
********************************************************************************
*
* Summary:
*  This function returns the number of Receive Errors.
*
* Parameters:
*  None.
*
* Return:
*  Number of Receive Errors.
*
*******************************************************************************/
uint8 CAN_GetRXErrorCount(void) 
{
    /* Get the state of the receive error count */
    return (CAN_ERR_SR_REG.byte[1u]);    /* bits 15-8 */
}


/*******************************************************************************
* FUNCTION NAME:   CAN_GetErrorState
********************************************************************************
*
* Summary:
*  This function returns error status of CAN Component.
*
* Parameters:
*  None.
*
* Return:
*  Error status.
*
*******************************************************************************/
uint8 CAN_GetErrorState(void) 
{
    /* Get the error state of the receiver */
    return (CAN_ERR_SR_REG.byte[2u] & CAN_ERROR_STATE_MASK);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_RxBufConfig
********************************************************************************
*
* Summary:
*  This function configures all receive registers for particular mailbox.
*
* Parameters:
*  rxConfig: Pointer to structure that contain all required values to configure
*  all receive registers for particular mailbox.
*
* Return:
*  Indication if particular configuration has been accepted or rejected.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_RxBufConfig(const CAN_RX_CFG *rxConfig)
                                   
{
    uint8 result = CAN_FAIL;

    /* Write RX CMD Register */
    CY_SET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxcmd, (rxConfig->rxcmd |
                 CAN_RX_WPN_SET));
    if ((CY_GET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxcmd) &
         CAN_RX_READ_BACK_MASK) == (rxConfig->rxcmd & CAN_RX_WPN_CLEAR))
    {
        /* Write RX AMR Register */
        CY_SET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxamr, rxConfig->rxamr);
        if (CY_GET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxamr) == rxConfig->rxamr)
        {
            /* Write RX ACR Register */
            CY_SET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxacr, rxConfig->rxacr);
            if (CY_GET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxacr) == rxConfig->rxacr)
            {
                /* Write RX AMRD Register */
                CY_SET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxamrd, 0xFFFFFFFFu);
                if (CY_GET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxamrd) == 0xFFFFFFFFu)
                {
                    /* Write RX ACRD Register */
                    CY_SET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxacrd, 0x00000000u);
                    if (CY_GET_REG32((reg32 *) &CAN_RX[rxConfig->rxmailbox].rxacrd) == 0x00000000u)
                    {
                            result = CYRET_SUCCESS;
                    }
                }
            }
        }
    }

    return (result);
}


/*******************************************************************************
* FUNCTION NAME:   CAN_TxBufConfig
********************************************************************************
*
* Summary:
*  This function configures all transmit registers for particular mailbox.
*  Mailbox number contains CAN_TX_CFG structure.
*
* Parameters:
*  txConfig: Pointer to structure that contain all required values to configure
*  all transmit registers for particular mailbox.
*
* Return:
*  Indication if particular configuration has been accepted or rejected.
*   Define                             Description
*    CYRET_SUCCESS                      Function passed successfully
*    CAN_FAIL              Function failed
*
*******************************************************************************/
uint8 CAN_TxBufConfig(const CAN_TX_CFG *txConfig) 
                                   
{
    uint8 result = CAN_FAIL;

    /* Write TX CMD Register */
    CY_SET_REG32((reg32 *) &CAN_TX[txConfig->txmailbox].txcmd, (txConfig->txcmd |
                 CAN_TX_WPN_SET));
    if ((CY_GET_REG32((reg32 *) & CAN_TX[txConfig->txmailbox].txcmd) &
        CAN_TX_READ_BACK_MASK) == (txConfig->txcmd & CAN_TX_WPN_CLEAR))
    {
        /* Write TX ID Register */
        CY_SET_REG32( (reg32 *) &CAN_TX[txConfig->txmailbox].txid, txConfig->txid);
        if (CY_GET_REG32( (reg32 *) &CAN_TX[txConfig->txmailbox].txid) == txConfig->txid)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result);
}


/* [] END OF FILE */
