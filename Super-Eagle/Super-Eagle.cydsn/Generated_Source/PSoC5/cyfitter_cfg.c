/*******************************************************************************
* FILENAME: cyfitter_cfg.c
* 
* PSoC Creator  3.2
*
* DESCRIPTION:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2015 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "cyfitter_cfg.h"

#define CY_NEED_CYCLOCKSTARTUPERROR 1


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define CYPACKED 
    #define CYPACKED_ATTR __attribute__ ((packed))
    #define CYALIGNED __attribute__ ((aligned))
    #define CY_CFG_UNUSED __attribute__ ((unused))
    #define CY_CFG_SECTION __attribute__ ((section(".psocinit")))
    
    #if defined(__ARMCC_VERSION)
        #define CY_CFG_MEMORY_BARRIER() __memory_changed()
    #else
        #define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
    #endif
    
#elif defined(__ICCARM__)
    #include <intrinsics.h>

    #define CYPACKED __packed
    #define CYPACKED_ATTR 
    #define CYALIGNED _Pragma("data_alignment=4")
    #define CY_CFG_UNUSED _Pragma("diag_suppress=Pe177")
    #define CY_CFG_SECTION _Pragma("location=\".psocinit\"")
    
    #define CY_CFG_MEMORY_BARRIER() __DMB()
    
#else
    #error Unsupported toolchain
#endif


CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n);
CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n)
{
	(void)memset(s, 0, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}



/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u

#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* `#START CyClockStartupError` */

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

    /* `#END` */

    /* If nothing else, stop here since the clocks have not started         */
    /* correctly.                                                           */
    while(1) {}
}
#endif

#define CY_CFG_BASE_ADDR_COUNT 32u
CYPACKED typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED_ATTR cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_XTND_REG8((void CYFAR *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}

/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*  Performs the initialization of all of the clocks in the device based on the
*  settings in the Clock tab of the DWR.  This includes enabling the requested
*  clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void ClockSetup(void);
static void ClockSetup(void)
{
	uint32 timeout;
	uint8 pllLock;


	/* Configure Digital Clocks based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_DCFG0_CFG0), 0x000Bu);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_DCFG0_CFG0 + 0x2u), 0x18u);
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_DCFG1_CFG0), 0x095Fu);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_DCFG1_CFG0 + 0x2u), 0x18u);
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_DCFG2_CFG0), 0xEA5Fu);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_DCFG2_CFG0 + 0x2u), 0x19u);

	/* Configure ILO based on settings from Clock DWR */
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_SLOWCLK_ILO_CR0), 0x02u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_CR), 0x08u);

	/* Configure IMO based on settings from Clock DWR */
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_FASTCLK_IMO_CR), 0x03u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_IMO_TR1), (CY_GET_XTND_REG8((void CYFAR *)CYREG_FLSHID_CUST_TABLES_IMO_3MHZ)));

	/* Configure PLL based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_FASTCLK_PLL_P), 0x0008u);
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_FASTCLK_PLL_CFG0), 0x1251u);
	/* Wait up to 250us for the PLL to lock */
	pllLock = 0u;
	for (timeout = 250u / 10u; (timeout > 0u) && (pllLock != 0x03u); timeout--)
	{ 
		pllLock = 0x03u & ((uint8)((uint8)pllLock << 1) | ((CY_GET_XTND_REG8((void CYFAR *)CYREG_FASTCLK_PLL_SR) & 0x01u) >> 0));
		CyDelayCycles(10u * 48u); /* Delay 10us based on 48MHz clock */
	}
	/* If we ran out of time the PLL didn't lock so go to the error function */
	if (timeout == 0u)
	{
		CyClockStartupError(CYCLOCKSTART_PLL_ERROR);
	}

	/* Configure Bus/Master Clock based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_MSTR0), 0x0100u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_MSTR0), 0x07u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_BCFG0), 0x00u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_BCFG2), 0x48u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_MSTR0), 0x00u);
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CLKDIST_LD), 0x02u);

	CY_SET_XTND_REG8((void CYFAR *)(CYREG_PM_ACT_CFG2), ((CY_GET_XTND_REG8((void CYFAR *)CYREG_PM_ACT_CFG2) | 0x07u)));
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	uint8 bg_xover_inl_trim = CY_GET_XTND_REG8((void CYFAR *)(CYREG_FLSHID_MFG_CFG_BG_XOVER_INL_TRIM + 1u));
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_BG_DFT0), (bg_xover_inl_trim & 0x07u));
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_BG_DFT1), ((bg_xover_inl_trim >> 4) & 0x0Fu));
	CY_SET_XTND_REG8((void CYFAR *)CYREG_PUMP_CR0, 0x44u);
}


/*******************************************************************************
* Function Name: SetAnalogRoutingPumps
********************************************************************************
*
* Summary:
* Enables or disables the analog pumps feeding analog routing switches.
* Intended to be called at startup, based on the Vdda system configuration;
* may be called during operation when the user informs us that the Vdda voltage
* crossed the pump threshold.
*
* Parameters:
*  enabled - 1 to enable the pumps, 0 to disable the pumps
*
* Return:
*  void
*
*******************************************************************************/
void SetAnalogRoutingPumps(uint8 enabled)
{
	uint8 regValue = CY_GET_XTND_REG8((void CYFAR *)CYREG_PUMP_CR0);
	if (enabled != 0u)
	{
		regValue |= 0x00u;
	}
	else
	{
		regValue &= (uint8)~0x00u;
	}
	CY_SET_XTND_REG8((void CYFAR *)CYREG_PUMP_CR0, regValue);
}

#define CY_AMUX_UNUSED CYREG_BOOST_SR


/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:  
*   void
*
* Return:
*   void
*
*******************************************************************************/

void cyfitter_cfg(void)
{
	/* IOPINS0_0 Address: CYREG_PRT0_DM0 Size (bytes): 8 */
	static const uint8 CYCODE BS_IOPINS0_0_VAL[] = {
		0x02u, 0x01u, 0x01u, 0x00u, 0x01u, 0x00u, 0x00u, 0x00u};

	/* IOPINS0_1 Address: CYREG_PRT1_DM0 Size (bytes): 8 */
	static const uint8 CYCODE BS_IOPINS0_1_VAL[] = {
		0x00u, 0x20u, 0x20u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

	/* IOPINS0_2 Address: CYREG_PRT2_DM0 Size (bytes): 8 */
	static const uint8 CYCODE BS_IOPINS0_2_VAL[] = {
		0x00u, 0x7Fu, 0x7Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

	/* IOPINS0_4 Address: CYREG_PRT4_DR Size (bytes): 10 */
	static const uint8 CYCODE BS_IOPINS0_4_VAL[] = {
		0x80u, 0x00u, 0x10u, 0xACu, 0x2Cu, 0x00u, 0x2Cu, 0x00u, 0x00u, 0x00u};

	/* IOPINS0_5 Address: CYREG_PRT5_DM0 Size (bytes): 8 */
	static const uint8 CYCODE BS_IOPINS0_5_VAL[] = {
		0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

	/* IOPINS0_6 Address: CYREG_PRT6_DR Size (bytes): 10 */
	static const uint8 CYCODE BS_IOPINS0_6_VAL[] = {
		0x80u, 0x00u, 0x00u, 0x82u, 0x02u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

#ifdef CYGlobalIntDisable
	/* Disable interrupts by default. Let user enable if/when they want. */
	CYGlobalIntDisable
#endif

	/* Enable/Disable Debug functionality based on settings from System DWR */
	CY_SET_XTND_REG8((void CYFAR *)CYREG_MLOGIC_DEBUG, (CY_GET_XTND_REG8((void CYFAR *)CYREG_MLOGIC_DEBUG) | 0x04u));

	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x40005206u, /* Base address: 0x40005200 Count: 6 */
			0x40006403u, /* Base address: 0x40006400 Count: 3 */
			0x40006501u, /* Base address: 0x40006500 Count: 1 */
			0x4001005Fu, /* Base address: 0x40010000 Count: 95 */
			0x4001012Bu, /* Base address: 0x40010100 Count: 43 */
			0x40010223u, /* Base address: 0x40010200 Count: 35 */
			0x4001034Au, /* Base address: 0x40010300 Count: 74 */
			0x40010446u, /* Base address: 0x40010400 Count: 70 */
			0x40010541u, /* Base address: 0x40010500 Count: 65 */
			0x40010702u, /* Base address: 0x40010700 Count: 2 */
			0x40010903u, /* Base address: 0x40010900 Count: 3 */
			0x40010A15u, /* Base address: 0x40010A00 Count: 21 */
			0x40010B12u, /* Base address: 0x40010B00 Count: 18 */
			0x40010D02u, /* Base address: 0x40010D00 Count: 2 */
			0x40011433u, /* Base address: 0x40011400 Count: 51 */
			0x40011518u, /* Base address: 0x40011500 Count: 24 */
			0x40011901u, /* Base address: 0x40011900 Count: 1 */
			0x40011B03u, /* Base address: 0x40011B00 Count: 3 */
			0x40014010u, /* Base address: 0x40014000 Count: 16 */
			0x40014117u, /* Base address: 0x40014100 Count: 23 */
			0x40014206u, /* Base address: 0x40014200 Count: 6 */
			0x40014305u, /* Base address: 0x40014300 Count: 5 */
			0x40014402u, /* Base address: 0x40014400 Count: 2 */
			0x40014502u, /* Base address: 0x40014500 Count: 2 */
			0x40014603u, /* Base address: 0x40014600 Count: 3 */
			0x40014701u, /* Base address: 0x40014700 Count: 1 */
			0x40014801u, /* Base address: 0x40014800 Count: 1 */
			0x40014904u, /* Base address: 0x40014900 Count: 4 */
			0x40014C01u, /* Base address: 0x40014C00 Count: 1 */
			0x40014D0Au, /* Base address: 0x40014D00 Count: 10 */
			0x40015008u, /* Base address: 0x40015000 Count: 8 */
			0x40015102u, /* Base address: 0x40015100 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0x00u, 0x01u},
			{0x20u, 0x24u},
			{0x21u, 0x2Cu},
			{0x24u, 0x90u},
			{0x2Cu, 0x01u},
			{0x34u, 0x80u},
			{0x82u, 0x0Du},
			{0x83u, 0x0Fu},
			{0x85u, 0x0Du},
			{0x85u, 0x0Du},
			{0x00u, 0x84u},
			{0x04u, 0x80u},
			{0x05u, 0x04u},
			{0x06u, 0x04u},
			{0x08u, 0x01u},
			{0x09u, 0x01u},
			{0x0Bu, 0x06u},
			{0x0Cu, 0x60u},
			{0x0Du, 0x04u},
			{0x10u, 0x07u},
			{0x12u, 0x38u},
			{0x14u, 0x56u},
			{0x15u, 0x07u},
			{0x16u, 0x09u},
			{0x18u, 0x84u},
			{0x1Bu, 0x04u},
			{0x1Cu, 0x84u},
			{0x1Du, 0x04u},
			{0x22u, 0x80u},
			{0x24u, 0x04u},
			{0x25u, 0x01u},
			{0x27u, 0x06u},
			{0x28u, 0x0Au},
			{0x2Au, 0x10u},
			{0x2Bu, 0x03u},
			{0x2Cu, 0x04u},
			{0x2Du, 0x04u},
			{0x2Eu, 0x80u},
			{0x32u, 0x7Fu},
			{0x34u, 0x80u},
			{0x35u, 0x07u},
			{0x38u, 0x08u},
			{0x3Bu, 0x20u},
			{0x3Eu, 0x10u},
			{0x40u, 0x41u},
			{0x49u, 0xFFu},
			{0x4Au, 0xFFu},
			{0x4Bu, 0xFFu},
			{0x4Fu, 0x01u},
			{0x50u, 0x18u},
			{0x54u, 0x40u},
			{0x56u, 0x04u},
			{0x58u, 0x04u},
			{0x59u, 0x04u},
			{0x5Au, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Du, 0x09u},
			{0x5Fu, 0x01u},
			{0x60u, 0x40u},
			{0x62u, 0x40u},
			{0x64u, 0x40u},
			{0x65u, 0x40u},
			{0x66u, 0x80u},
			{0x68u, 0x80u},
			{0x6Au, 0x80u},
			{0x6Cu, 0x80u},
			{0x6Eu, 0x80u},
			{0x83u, 0x10u},
			{0x89u, 0x0Eu},
			{0x92u, 0x01u},
			{0x95u, 0x04u},
			{0x97u, 0x09u},
			{0x99u, 0x03u},
			{0x9Bu, 0x04u},
			{0xA9u, 0x05u},
			{0xABu, 0x0Au},
			{0xAFu, 0x10u},
			{0xB2u, 0x01u},
			{0xB5u, 0x0Fu},
			{0xB7u, 0x10u},
			{0xB9u, 0x20u},
			{0xBFu, 0x40u},
			{0xC0u, 0x41u},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCDu, 0x83u},
			{0xCEu, 0x03u},
			{0xCFu, 0x01u},
			{0xD0u, 0x18u},
			{0xD8u, 0x04u},
			{0xD9u, 0x04u},
			{0xDAu, 0x04u},
			{0xDCu, 0x02u},
			{0xDDu, 0x09u},
			{0xDFu, 0x01u},
			{0xE0u, 0x40u},
			{0xE2u, 0x40u},
			{0xE4u, 0x40u},
			{0xE5u, 0x40u},
			{0xE6u, 0x80u},
			{0xE8u, 0x80u},
			{0xEAu, 0x80u},
			{0xECu, 0x80u},
			{0xEEu, 0x80u},
			{0x00u, 0x08u},
			{0x02u, 0x02u},
			{0x03u, 0x58u},
			{0x08u, 0x08u},
			{0x0Au, 0x85u},
			{0x0Fu, 0x80u},
			{0x10u, 0x13u},
			{0x11u, 0x10u},
			{0x12u, 0x40u},
			{0x13u, 0x04u},
			{0x1Bu, 0x18u},
			{0x1Eu, 0x10u},
			{0x22u, 0x10u},
			{0x23u, 0x04u},
			{0x25u, 0x10u},
			{0x26u, 0x40u},
			{0x28u, 0x11u},
			{0x29u, 0x20u},
			{0x2Eu, 0xA0u},
			{0x31u, 0x04u},
			{0x33u, 0x50u},
			{0x34u, 0x08u},
			{0x35u, 0x24u},
			{0x37u, 0x08u},
			{0x38u, 0x80u},
			{0x39u, 0x18u},
			{0x3Au, 0x20u},
			{0x3Du, 0x20u},
			{0x3Eu, 0x02u},
			{0x42u, 0x82u},
			{0x46u, 0x82u},
			{0x61u, 0x20u},
			{0x62u, 0x40u},
			{0x63u, 0x60u},
			{0x87u, 0x40u},
			{0xC0u, 0x0Fu},
			{0xC2u, 0x1Fu},
			{0xC4u, 0x0Fu},
			{0xCAu, 0x3Eu},
			{0xCCu, 0x6Eu},
			{0xCEu, 0xAEu},
			{0xD0u, 0x99u},
			{0xD8u, 0x0Fu},
			{0x09u, 0x02u},
			{0x0Bu, 0x05u},
			{0x19u, 0x04u},
			{0x1Bu, 0x02u},
			{0x33u, 0x01u},
			{0x35u, 0x06u},
			{0x40u, 0x41u},
			{0x45u, 0xECu},
			{0x47u, 0x20u},
			{0x48u, 0x23u},
			{0x49u, 0xFFu},
			{0x4Au, 0xFFu},
			{0x4Bu, 0xFFu},
			{0x4Du, 0x83u},
			{0x4Eu, 0x03u},
			{0x4Fu, 0x01u},
			{0x50u, 0x18u},
			{0x52u, 0x80u},
			{0x54u, 0x01u},
			{0x56u, 0x04u},
			{0x59u, 0x04u},
			{0x5Au, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Cu, 0x20u},
			{0x5Du, 0x99u},
			{0x5Fu, 0x01u},
			{0x60u, 0x40u},
			{0x62u, 0x40u},
			{0x64u, 0x40u},
			{0x65u, 0x40u},
			{0x66u, 0x80u},
			{0x68u, 0x80u},
			{0x6Au, 0x80u},
			{0x6Cu, 0x80u},
			{0x6Eu, 0x80u},
			{0x00u, 0x10u},
			{0x09u, 0x08u},
			{0x0Au, 0x80u},
			{0x13u, 0x08u},
			{0x18u, 0x10u},
			{0x20u, 0x04u},
			{0x21u, 0x08u},
			{0x22u, 0x40u},
			{0x23u, 0x08u},
			{0x24u, 0x08u},
			{0x27u, 0x20u},
			{0x28u, 0x69u},
			{0x2Au, 0x04u},
			{0x31u, 0x08u},
			{0x32u, 0x50u},
			{0x34u, 0x40u},
			{0x37u, 0x60u},
			{0x38u, 0x80u},
			{0x39u, 0x18u},
			{0x3Eu, 0x10u},
			{0x40u, 0x0Cu},
			{0x41u, 0x0Au},
			{0x42u, 0x20u},
			{0x46u, 0x82u},
			{0x49u, 0xA8u},
			{0x4Du, 0x01u},
			{0x4Fu, 0x04u},
			{0x51u, 0x20u},
			{0x52u, 0x46u},
			{0x56u, 0x02u},
			{0x5Du, 0x01u},
			{0x5Eu, 0x80u},
			{0x5Fu, 0x04u},
			{0x60u, 0x10u},
			{0x66u, 0x01u},
			{0x68u, 0x06u},
			{0x69u, 0x18u},
			{0x6Au, 0x04u},
			{0x6Bu, 0x40u},
			{0x70u, 0x40u},
			{0x72u, 0x01u},
			{0x76u, 0x80u},
			{0x80u, 0x04u},
			{0x82u, 0x01u},
			{0x8Au, 0x80u},
			{0x92u, 0x30u},
			{0x94u, 0x80u},
			{0x95u, 0x08u},
			{0x96u, 0x85u},
			{0x98u, 0x01u},
			{0x99u, 0x80u},
			{0x9Du, 0x20u},
			{0x9Eu, 0xE2u},
			{0x9Fu, 0x48u},
			{0xA2u, 0x40u},
			{0xA4u, 0x06u},
			{0xA5u, 0x20u},
			{0xA6u, 0x02u},
			{0xA9u, 0x10u},
			{0xABu, 0x40u},
			{0xADu, 0x80u},
			{0xB3u, 0x08u},
			{0xC0u, 0x02u},
			{0xC2u, 0x0Cu},
			{0xC4u, 0x04u},
			{0xCAu, 0x0Fu},
			{0xCCu, 0x2Eu},
			{0xCEu, 0x2Eu},
			{0xD0u, 0x97u},
			{0xD2u, 0x0Cu},
			{0xD6u, 0xD0u},
			{0xD8u, 0x82u},
			{0xE2u, 0x80u},
			{0xEAu, 0x20u},
			{0x00u, 0x0Du},
			{0x04u, 0x0Du},
			{0x05u, 0x02u},
			{0x08u, 0x02u},
			{0x09u, 0x02u},
			{0x0Au, 0x0Du},
			{0x0Cu, 0x0Du},
			{0x0Du, 0x02u},
			{0x10u, 0x01u},
			{0x12u, 0x12u},
			{0x13u, 0x02u},
			{0x14u, 0x12u},
			{0x15u, 0x01u},
			{0x16u, 0x04u},
			{0x18u, 0x0Du},
			{0x19u, 0x01u},
			{0x1Du, 0x02u},
			{0x24u, 0x02u},
			{0x26u, 0x18u},
			{0x29u, 0x01u},
			{0x2Cu, 0x0Du},
			{0x31u, 0x02u},
			{0x32u, 0x0Fu},
			{0x36u, 0x10u},
			{0x37u, 0x01u},
			{0x3Au, 0x08u},
			{0x3Eu, 0x40u},
			{0x3Fu, 0x41u},
			{0x54u, 0x09u},
			{0x56u, 0x04u},
			{0x58u, 0x04u},
			{0x59u, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Fu, 0x01u},
			{0x82u, 0x01u},
			{0x96u, 0x01u},
			{0x9Bu, 0x03u},
			{0x9Fu, 0x02u},
			{0xA1u, 0x01u},
			{0xABu, 0x03u},
			{0xB0u, 0x01u},
			{0xB7u, 0x03u},
			{0xBEu, 0x01u},
			{0xC0u, 0x46u},
			{0xC5u, 0xECu},
			{0xC6u, 0x20u},
			{0xC8u, 0x0Bu},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCFu, 0x01u},
			{0xD0u, 0x18u},
			{0xD2u, 0x80u},
			{0xD4u, 0x01u},
			{0xD6u, 0x04u},
			{0xD8u, 0x04u},
			{0xD9u, 0x04u},
			{0xDAu, 0x04u},
			{0xDBu, 0x04u},
			{0xDDu, 0x11u},
			{0xDFu, 0x01u},
			{0xE0u, 0x40u},
			{0xE2u, 0x40u},
			{0xE4u, 0x40u},
			{0xE5u, 0x40u},
			{0xE6u, 0x80u},
			{0xE8u, 0x80u},
			{0xEAu, 0x80u},
			{0xECu, 0x80u},
			{0xEEu, 0x80u},
			{0x00u, 0x58u},
			{0x03u, 0x40u},
			{0x06u, 0x80u},
			{0x09u, 0x08u},
			{0x0Au, 0x84u},
			{0x0Bu, 0x80u},
			{0x0Fu, 0x20u},
			{0x10u, 0x02u},
			{0x12u, 0x20u},
			{0x13u, 0x08u},
			{0x18u, 0x10u},
			{0x19u, 0x40u},
			{0x1Eu, 0x80u},
			{0x21u, 0x82u},
			{0x22u, 0x01u},
			{0x26u, 0x80u},
			{0x29u, 0x20u},
			{0x2Eu, 0x20u},
			{0x2Fu, 0x40u},
			{0x30u, 0x02u},
			{0x31u, 0x08u},
			{0x32u, 0x20u},
			{0x33u, 0x40u},
			{0x36u, 0xA0u},
			{0x38u, 0xC0u},
			{0x39u, 0x18u},
			{0x46u, 0x80u},
			{0x4Du, 0x05u},
			{0x4Fu, 0x20u},
			{0x57u, 0x10u},
			{0x58u, 0x20u},
			{0x59u, 0x48u},
			{0x5Au, 0x02u},
			{0x5Du, 0x05u},
			{0x5Eu, 0x80u},
			{0x61u, 0x80u},
			{0x63u, 0x40u},
			{0x65u, 0x02u},
			{0x76u, 0x80u},
			{0x85u, 0x02u},
			{0x87u, 0x40u},
			{0x90u, 0x80u},
			{0x91u, 0x18u},
			{0x92u, 0x84u},
			{0x98u, 0x20u},
			{0x99u, 0x28u},
			{0x9Au, 0x20u},
			{0x9Bu, 0x48u},
			{0xA0u, 0x06u},
			{0xA1u, 0x08u},
			{0xA5u, 0x02u},
			{0xA6u, 0x80u},
			{0xB5u, 0x20u},
			{0xB6u, 0x40u},
			{0xC0u, 0x1Fu},
			{0xC2u, 0x2Eu},
			{0xC4u, 0x05u},
			{0xCAu, 0xA4u},
			{0xCCu, 0x3Fu},
			{0xCEu, 0x0Eu},
			{0xD0u, 0x80u},
			{0xD2u, 0x20u},
			{0xD6u, 0xDFu},
			{0xD8u, 0x89u},
			{0xE8u, 0x80u},
			{0xEAu, 0x01u},
			{0xEEu, 0x08u},
			{0x9Cu, 0x20u},
			{0xA8u, 0x01u},
			{0xAFu, 0x01u},
			{0x04u, 0x01u},
			{0x06u, 0x06u},
			{0x14u, 0x02u},
			{0x16u, 0x01u},
			{0x17u, 0x01u},
			{0x18u, 0x10u},
			{0x1Au, 0x20u},
			{0x24u, 0x20u},
			{0x26u, 0x10u},
			{0x27u, 0x02u},
			{0x2Eu, 0x08u},
			{0x30u, 0x03u},
			{0x32u, 0x04u},
			{0x33u, 0x02u},
			{0x34u, 0x08u},
			{0x35u, 0x01u},
			{0x36u, 0x30u},
			{0x58u, 0x04u},
			{0x59u, 0x04u},
			{0x5Cu, 0x22u},
			{0x5Fu, 0x01u},
			{0x03u, 0x20u},
			{0x0Au, 0x18u},
			{0x10u, 0x02u},
			{0x13u, 0x08u},
			{0x18u, 0x01u},
			{0x1Au, 0x10u},
			{0x1Bu, 0x21u},
			{0x22u, 0x04u},
			{0x23u, 0x08u},
			{0x28u, 0x20u},
			{0x32u, 0x04u},
			{0xA0u, 0x02u},
			{0xC0u, 0x04u},
			{0xC2u, 0x06u},
			{0xC4u, 0x05u},
			{0xCAu, 0x02u},
			{0xCCu, 0x02u},
			{0xE8u, 0x80u},
			{0xA0u, 0x02u},
			{0xB0u, 0x02u},
			{0x40u, 0x41u},
			{0x49u, 0xFFu},
			{0x4Au, 0xFFu},
			{0x4Bu, 0xFFu},
			{0x4Fu, 0x01u},
			{0x50u, 0x18u},
			{0x52u, 0x80u},
			{0x5Au, 0x04u},
			{0x5Bu, 0x04u},
			{0x5Du, 0x11u},
			{0x5Fu, 0x01u},
			{0x60u, 0x40u},
			{0x62u, 0x40u},
			{0x64u, 0x40u},
			{0x65u, 0x40u},
			{0x66u, 0x80u},
			{0x68u, 0x80u},
			{0x6Au, 0x80u},
			{0x6Cu, 0x80u},
			{0x6Eu, 0x80u},
			{0x82u, 0x01u},
			{0x92u, 0x01u},
			{0xB0u, 0x01u},
			{0xBEu, 0x01u},
			{0xC0u, 0x14u},
			{0xC5u, 0xECu},
			{0xC6u, 0x02u},
			{0xC8u, 0x07u},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCDu, 0x83u},
			{0xCEu, 0x03u},
			{0xCFu, 0x01u},
			{0xD0u, 0x18u},
			{0xD4u, 0x01u},
			{0xD6u, 0x04u},
			{0xD8u, 0x04u},
			{0xDAu, 0x04u},
			{0xDBu, 0x04u},
			{0xDDu, 0x11u},
			{0xDFu, 0x01u},
			{0xE0u, 0x40u},
			{0xE2u, 0x40u},
			{0xE4u, 0x40u},
			{0xE5u, 0x40u},
			{0xE6u, 0x80u},
			{0xE8u, 0x80u},
			{0xEAu, 0x80u},
			{0xECu, 0x80u},
			{0xEEu, 0x80u},
			{0x06u, 0x40u},
			{0x0Du, 0x02u},
			{0x1Eu, 0x80u},
			{0x41u, 0x02u},
			{0x43u, 0x80u},
			{0x46u, 0x40u},
			{0x47u, 0x80u},
			{0x4Du, 0x04u},
			{0x4Fu, 0x01u},
			{0x56u, 0x40u},
			{0x57u, 0x80u},
			{0x5Du, 0x04u},
			{0x5Eu, 0x80u},
			{0x5Fu, 0x01u},
			{0x67u, 0x80u},
			{0x71u, 0x02u},
			{0x73u, 0x80u},
			{0x87u, 0x40u},
			{0xC0u, 0x10u},
			{0xC2u, 0x10u},
			{0xD0u, 0x99u},
			{0xD6u, 0xD0u},
			{0xD8u, 0x80u},
			{0xE2u, 0x10u},
			{0x9Cu, 0x20u},
			{0x8Cu, 0x20u},
			{0x9Cu, 0x20u},
			{0xE2u, 0x10u},
			{0x0Bu, 0x10u},
			{0x0Du, 0x20u},
			{0x33u, 0x82u},
			{0x37u, 0x12u},
			{0x3Bu, 0x20u},
			{0x59u, 0x40u},
			{0x5Fu, 0x80u},
			{0x66u, 0x08u},
			{0x87u, 0x02u},
			{0xC2u, 0x90u},
			{0xCCu, 0xF0u},
			{0xCEu, 0x10u},
			{0xD4u, 0x80u},
			{0xD6u, 0x20u},
			{0xD8u, 0x80u},
			{0xE6u, 0x40u},
			{0x05u, 0x02u},
			{0x32u, 0x02u},
			{0x33u, 0x20u},
			{0x36u, 0x20u},
			{0x57u, 0x10u},
			{0x82u, 0x20u},
			{0x83u, 0x10u},
			{0x89u, 0x40u},
			{0x97u, 0x20u},
			{0x9Du, 0x40u},
			{0x9Fu, 0x10u},
			{0xA7u, 0x82u},
			{0xA9u, 0x20u},
			{0xABu, 0x80u},
			{0xAFu, 0x10u},
			{0xB2u, 0x08u},
			{0xC0u, 0x20u},
			{0xCCu, 0x70u},
			{0xD4u, 0x40u},
			{0xE2u, 0x20u},
			{0xE6u, 0x80u},
			{0xE8u, 0x30u},
			{0xEAu, 0x80u},
			{0x87u, 0x90u},
			{0x97u, 0x20u},
			{0xA6u, 0x02u},
			{0xA7u, 0xB2u},
			{0xB1u, 0x02u},
			{0xE6u, 0x50u},
			{0xA7u, 0x12u},
			{0xAAu, 0x02u},
			{0xAFu, 0x20u},
			{0xEAu, 0x80u},
			{0xEEu, 0x10u},
			{0x0Cu, 0x80u},
			{0xC2u, 0x01u},
			{0xB0u, 0x80u},
			{0xEEu, 0x01u},
			{0x98u, 0x20u},
			{0xB0u, 0x20u},
			{0xE8u, 0x01u},
			{0x98u, 0x20u},
			{0xA7u, 0x12u},
			{0x87u, 0x02u},
			{0x8Fu, 0x10u},
			{0xA7u, 0x12u},
			{0xE2u, 0x10u},
			{0x98u, 0x20u},
			{0x01u, 0x02u},
			{0x18u, 0x10u},
			{0x78u, 0x20u},
			{0x8Cu, 0x10u},
			{0x8Du, 0x02u},
			{0x98u, 0x20u},
			{0xC0u, 0x08u},
			{0xC6u, 0x08u},
			{0xDEu, 0x02u},
			{0xE2u, 0x0Au},
			{0x00u, 0x07u},
			{0x01u, 0x01u},
			{0x08u, 0x05u},
			{0x09u, 0x01u},
			{0x0Au, 0x03u},
			{0x0Cu, 0x04u},
			{0x10u, 0x02u},
			{0x1Au, 0x02u},
			{0x00u, 0xFFu},
			{0x01u, 0xFFu},
		};



		CYPACKED typedef struct {
			void CYFAR *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;


		CYPACKED typedef struct {
			void CYFAR *dest;
			const void CYCODE *src;
			uint16 size;
		} CYPACKED_ATTR cfg_memcpy_t;

		static const cfg_memset_t CYCODE cfg_memset_list [] = {
			/* address, size */
			{(void CYFAR *)(CYREG_PRT3_DR), 16u},
			{(void CYFAR *)(CYREG_PRT12_DR), 16u},
			{(void CYFAR *)(CYREG_PRT15_DR), 16u},
			{(void CYFAR *)(CYDEV_UCFG_B0_P0_U0_BASE), 640u},
			{(void CYFAR *)(CYDEV_UCFG_B0_P1_ROUTE_BASE), 3328u},
			{(void CYFAR *)(CYDEV_UCFG_B1_P2_U0_BASE), 2048u},
			{(void CYFAR *)(CYDEV_UCFG_DSI0_BASE), 2560u},
			{(void CYFAR *)(CYDEV_UCFG_DSI12_BASE), 512u},
			{(void CYFAR *)(CYREG_BCTL0_MDCLK_EN), 32u},
		};

		/* UDB_1_4_0_CONFIG Address: CYDEV_UCFG_B0_P1_U1_BASE Size (bytes): 128 */
		static const uint8 CYCODE BS_UDB_1_4_0_CONFIG_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x18u, 0x00u, 0x02u, 0x02u, 0x02u, 0x0Du, 0x18u, 0x00u, 0x1Au, 0x00u, 0x00u, 
			0x03u, 0x00u, 0x0Cu, 0x00u, 0x05u, 0xC1u, 0x0Au, 0x38u, 0x00u, 0x18u, 0x00u, 0x00u, 0x00u, 0x33u, 0x00u, 0x44u, 
			0x00u, 0x1Au, 0x00u, 0x00u, 0x09u, 0x82u, 0x06u, 0x00u, 0x00u, 0x9Cu, 0x00u, 0x03u, 0x00u, 0x18u, 0x00u, 0x02u, 
			0x00u, 0x00u, 0x0Fu, 0x0Fu, 0x00u, 0xE0u, 0x00u, 0x13u, 0x00u, 0x00u, 0x08u, 0x28u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x32u, 0x06u, 0x40u, 0x00u, 0x05u, 0x0Eu, 0xFDu, 0xBCu, 0x3Du, 0xFFu, 0xFFu, 0xFFu, 0x22u, 0x00u, 0xF0u, 0x08u, 
			0x04u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x02u, 0x28u, 0x04u, 0x04u, 0x04u, 0x04u, 0x00u, 0x00u, 0x00u, 0x01u, 
			0x00u, 0x00u, 0xC0u, 0x00u, 0x40u, 0x01u, 0x10u, 0x11u, 0xC0u, 0x01u, 0x00u, 0x11u, 0x40u, 0x01u, 0x40u, 0x01u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u};

		static const cfg_memcpy_t CYCODE cfg_memcpy_list [] = {
			/* dest, src, size */
			{(void CYFAR *)(CYDEV_UCFG_B0_P1_U1_BASE), BS_UDB_1_4_0_CONFIG_VAL, 128u},
		};

		uint8 CYDATA i;

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, (size_t)(uint32)(ms->size));
		}

		/* Copy device configuration data into registers */
		for (i = 0u; i < (sizeof(cfg_memcpy_list)/sizeof(cfg_memcpy_list[0])); i++)
		{
			const cfg_memcpy_t CYCODE * CYDATA mc = &cfg_memcpy_list[i];
			void * CYDATA destPtr = mc->dest;
			const void CYCODE * CYDATA srcPtr = mc->src;
			uint16 CYDATA numBytes = mc->size;
			CYCONFIGCPYCODE(destPtr, srcPtr, numBytes);
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* Enable digital routing */
		CY_SET_XTND_REG8((void CYFAR *)CYREG_BCTL0_BANK_CTL, CY_GET_XTND_REG8((void CYFAR *)CYREG_BCTL0_BANK_CTL) | 0x02u);
		CY_SET_XTND_REG8((void CYFAR *)CYREG_BCTL1_BANK_CTL, CY_GET_XTND_REG8((void CYFAR *)CYREG_BCTL1_BANK_CTL) | 0x02u);

		/* Enable UDB array */
		CY_SET_XTND_REG8((void CYFAR *)CYREG_PM_ACT_CFG0, CY_GET_XTND_REG8((void CYFAR *)CYREG_PM_ACT_CFG0) | 0x40u);
		CY_SET_XTND_REG8((void CYFAR *)CYREG_PM_AVAIL_CR2, CY_GET_XTND_REG8((void CYFAR *)CYREG_PM_AVAIL_CR2) | 0x10u);
	}


	/* Perform second pass device configuration. These items must be configured in specific order after the regular configuration is done. */
	CYCONFIGCPYCODE((void CYFAR *)(CYREG_PRT0_DM0), (const void CYCODE *)(BS_IOPINS0_0_VAL), 8u);
	CYCONFIGCPYCODE((void CYFAR *)(CYREG_PRT1_DM0), (const void CYCODE *)(BS_IOPINS0_1_VAL), 8u);
	CYCONFIGCPYCODE((void CYFAR *)(CYREG_PRT2_DM0), (const void CYCODE *)(BS_IOPINS0_2_VAL), 8u);
	CYCONFIGCPYCODE((void CYFAR *)(CYREG_PRT4_DR), (const void CYCODE *)(BS_IOPINS0_4_VAL), 10u);
	CYCONFIGCPYCODE((void CYFAR *)(CYREG_PRT5_DM0), (const void CYCODE *)(BS_IOPINS0_5_VAL), 8u);
	CYCONFIGCPYCODE((void CYFAR *)(CYREG_PRT6_DR), (const void CYCODE *)(BS_IOPINS0_6_VAL), 10u);
	/* Switch Boost to the precision bandgap reference from its internal reference */
	CY_SET_REG8((void CYXDATA *)CYREG_BOOST_CR2, (CY_GET_REG8((void CYXDATA *)CYREG_BOOST_CR2) | 0x08u));

	/* Set Flash Cycles based on max possible frequency in case a glitch occurs during ClockSetup(). */
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CACHE_CC_CTL), (((CYDEV_INSTRUCT_CACHE_ENABLED) != 0) ? 0x01u : 0x00u));
	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();
	/* Set Flash Cycles based on newly configured 24.00MHz Bus Clock. */
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_CACHE_CC_CTL), (((CYDEV_INSTRUCT_CACHE_ENABLED) != 0) ? 0x81u : 0x80u));
	CY_SET_XTND_REG8((void CYFAR *)(CYREG_PANTHER_WAITPIPE), 0x01u);

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

	/* Configure alternate active mode */
	CYCONFIGCPY((void CYFAR *)CYDEV_PM_STBY_BASE, (const void CYFAR *)CYDEV_PM_ACT_BASE, 14u);
}
