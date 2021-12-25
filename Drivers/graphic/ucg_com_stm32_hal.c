/*
  ******************************************************************************
  * File Name          : ucg_com_stm32_hal.c
  * Description        : Main program body
  ******************************************************************************
  * µC-spezifische Anweisungen zw. ucg und stm32
  * Hal-Driver im Einsatz
  ******************************************************************************
*/


#include "stm32f1xx_hal.h"
#include "ucg.h"
#include "ucg_com_stm32_hal.h"
#include "main.h"
#include <stdint.h>


static void SendByte(uint8_t data) {
    while ((SPI1->SR & SPI_SR_TXE) == RESET);
    SPI1->DR = data;
    /* HAL_SPI_Transmit(&ST7735_SPI_PORT, &data, sizeof(data), HAL_MAX_DELAY); */
}

static void WaitLastData() {
    while((SPI1->SR & SPI_SR_TXE) == RESET);
    while((SPI1->SR & SPI_SR_BSY) != RESET);
}

int16_t ucg_com_stm32_hal(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data)
{

	switch(msg)
	{
		case UCG_COM_MSG_POWER_UP: //10
			/* "data" is a pointer to ucg_com_info_t structure with the following information: */
			/*	((ucg_com_info_t *)data)->serial_clk_speed value in nanoseconds */
			/*	((ucg_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
			/* "arg" is not used */

			/* This message is sent once at the uC startup and for power up. */			// done by HAL_init
			/* setup i/o or do any other setup */
			break;

		case UCG_COM_MSG_POWER_DOWN:  //11
			/* "data" and "arg" are not used*/
			/* This message is sent for a power down request */

			break;
		case UCG_COM_MSG_DELAY:  //12
			/* "data" is not used */
			/* "arg" contains the number of microseconds for the delay */
			/* By receiving this message, the following code should delay by */
			/* "arg" microseconds. One microsecond is 0.000001 second */

			//HAL_Delay(arg);

			break;
		case UCG_COM_MSG_CHANGE_RESET_LINE: //13
			/* "data" is not used */
			/* "arg" = 1: set the reset output line to 1 */							// idle => high = 1
			/* "arg" = 0: set the reset output line to 0 */							// active => low = 0

			//--- spi ------------------------------------------------------------------

			if(arg == 0)
				HAL_GPIO_WritePin(Reset_GPIO_Port, Reset_Pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(Reset_GPIO_Port, Reset_Pin, GPIO_PIN_SET);

			/*--- 8-bit ----------------------------------------------------------------
			 *
			 * 	 if(arg == 0)
			 * 	    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
			 * 	 else
			 * 	  	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
			 *
			 --------------------------------------------------------------------------*/
			break;

		case UCG_COM_MSG_CHANGE_CD_LINE:  //15
			/* "ucg->com_status"  bit 0 contains the old level for the CD line */
			/* "data" is not used */
			/* "arg" = 1: set the command/data (a0) output line to 1 */				//high = 1 => data or parameter
			/* "arg" = 0: set the command/data (a0) output line to 0 */				//low  = 0 => command

			// --- spi -----------------------------------------------------------------

			if(arg == 0)
				HAL_GPIO_WritePin(CD_GPIO_Port, CD_Pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(CD_GPIO_Port, CD_Pin, GPIO_PIN_SET);

			/* --- 8-bit --------------------------------------------------------------
			 *
			 *    	 if(arg == 0)
			 *  		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
			 *  	else
			 *  		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
			 *
			 -------------------------------------------------------------------------- */

			break;

		case UCG_COM_MSG_CHANGE_CS_LINE:  //14
			/* "ucg->com_status"  bit 1 contains the old level for the CS line */
			/* "data" is not used */
			/* "arg" = 1: set the chipselect output line to 1 */
			/* "arg" = 0: set the chipselect output line to 0 */ 						//active => low = 0

			// --- spi -----------------------------------------------------------------

			if(arg == 0)
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

			/* --- 8-bit --------------------------------------------------------------
			 *
			 *  	if(arg == 0)
			 *  		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
			 *  	else
			 *  		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
			 *
			 -------------------------------------------------------------------------*/

			break;

		case UCG_COM_MSG_SEND_BYTE:  //16
			/* "data" is not used */
			/* "arg" contains one byte, which should be sent to the display */
			/* The current status of the CD line is available */
			/* in bit 0 of u8g->com_status */

			// --- spi ----------------------------------------------------------------

			/* HAL_SPI_Transmit(&SPI_HANDLER, (uint8_t *)&arg, 1, SPI_TIMEOUT); */
			SendByte(arg);
			WaitLastData();

			/* --- 8-bit -------------------------------------------------------------
			 *
			 * 	   ucg_com_stm32_send_8bit(arg);
			 *
			 --------------------------------------------------------------------------*/

			break;

		case UCG_COM_MSG_REPEAT_1_BYTE: //17
			/* "data[0]" contains one byte */
			/* repeat sending the byte in data[0] "arg" times */
			/* The current status of the CD line is available */
			/* in bit 0 of u8g->com_status */
			while( arg > 0 ) {

				// --- spi ----------------------------------------------------------------

				/* HAL_SPI_Transmit(&SPI_HANDLER, (uint8_t *)data, 1, SPI_TIMEOUT); */
				SendByte(arg);
				WaitLastData();

				/* --- 8-bit --------------------------------------------------------------
				 *
				 * 		ucg_com_stm32_send_8bit(data[0]);
				 *
				 ---------------------------------------------------------------------------*/

				arg--;
			}
			break;

		case UCG_COM_MSG_REPEAT_2_BYTES: //18
			/* "data[0]" contains first byte */
			/* "data[1]" contains second byte */
			/* repeat sending the two bytes "arg" times */
			/* The current status of the CD line is available */
			/* in bit 0 of u8g->com_status */
			while( arg > 0 ) {

				// --- spi -----------------------------------------------------------------

				/* HAL_SPI_Transmit(&SPI_HANDLER, (uint8_t *)data, 2, SPI_TIMEOUT); */
				SendByte(data[0]);
				SendByte(data[1]);
				WaitLastData();

				/* --- 8-bit ---------------------------------------------------------------
				 *
				 *    		ucg_com_stm32_send_8bit(data[0]);
				 *  		ucg_com_stm32_send_8bit(data[1]);
				 *
				 --------------------------------------------------------------------------- */

				arg--;
			}
			break;

		case UCG_COM_MSG_REPEAT_3_BYTES:  //19
			/* "data[0]" contains first byte */
			/* "data[1]" contains second byte */
			/* "data[2]" contains third byte */
			/* repeat sending the three bytes "arg" times */
			/* The current status of the CD line is available */
			/* in bit 0 of u8g->com_status */
			while( arg > 0 ) {

				// --- spi ------------------------------------------------------------------

				/* HAL_SPI_Transmit(&SPI_HANDLER, (uint8_t *)data, 3, SPI_TIMEOUT); */
				SendByte(data[0]);
				SendByte(data[1]);
				SendByte(data[2]);
				WaitLastData();

				/* --- 8-bit ----------------------------------------------------------------
				 *
				 *     		ucg_com_stm32_send_8bit(data[0]);
				 *  		ucg_com_stm32_send_8bit(data[1]);
				 *  		ucg_com_stm32_send_8bit(data[2]);
				 *
				 --------------------------------------------------------------------------- */

				arg--;
			}
			break;

		case UCG_COM_MSG_SEND_STR:  //20
			/* "data" is an array with "arg" bytes */
			/* send "arg" bytes to the display */

			// --- spi -------------------------------------------------------------------

			/* HAL_SPI_Transmit(&SPI_HANDLER, (uint8_t *)data, arg, SPI_TIMEOUT); */
			for (int16_t i = 0; i < arg; i++) {
				SendByte(*data);
				data++;
			}
			WaitLastData();

			/* --- 8-bit -----------------------------------------------------------------
			 *
			 *     	 while( arg > 0 ) {
			 *     	  ucg_com_stm32_send_8bit(*data++);
			 *  		arg--;
			 *  	      }
			 *
			 --------------------------------------------------------------------------- */

			break;

		case UCG_COM_MSG_SEND_CD_DATA_SEQUENCE:  //21
			/* "data" is a pointer too two bytes, which contain the cd line status and display data */
			/* "arg" contains the number of these two byte tuples which need to be analyzed and sent.*/
			/* Below is a example sequence. The content of bit 0 in u8g->com_status is undefined for this message */

			while(arg > 0)
			{
				if ( *data != 0 )
				{
					if ( *data == 1 )
					{
						/* set CD (=D/C=A0) line to low */

						// --- spi -------------------------------------------------------------------

						HAL_GPIO_WritePin(CD_GPIO_Port, CD_Pin, GPIO_PIN_RESET);

						/* --- 8-bit -----------------------------------------------------------------
						 *
						 *   		  	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
						 *
						 ---------------------------------------------------------------------------- */
					}
					else
					{
						/* set CD (=D/C=A0) line to high */

						// --- spi -------------------------------------------------------------------

						HAL_GPIO_WritePin(CD_GPIO_Port, CD_Pin, GPIO_PIN_SET);

						/* --- 8-bit -----------------------------------------------------------------
						 *
						 *   		    	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
						 *
						 ----------------------------------------------------------------------------- */
					}
				}
				data++;
				/* send *data to the display */

				// --- spi -------------------------------------------------------------------

				/* HAL_SPI_Transmit(&SPI_HANDLER, (uint8_t *)data, 1, SPI_TIMEOUT); */
				SendByte(*data);

				/* --- 8-bit -----------------------------------------------------------------
				 *
				 *    	  ucg_com_stm32_send_8bit(*data);
				 *
				 ----------------------------------------------------------------------------- */

				data++;
				arg--;
			}
			WaitLastData();
			break;
	}
	return 1;
}
// --- 8-bit ---------------------------------------------------------------------------------------

//void ucg_com_stm32_send_8bit(uint8_t eightbit)

//{
// with HAL-GPIO --- fast ---------------------------------------------------------------------------
/*
HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (eightbit & 1) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (eightbit & 2) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (eightbit & 4) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (eightbit & 8) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (eightbit & 16) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (eightbit & 32) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (eightbit & 64) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (eightbit & 128) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET);
*/
// with GPIO --- faster ----------------------------------------------------------------------------
/*
(eightbit & 1) == 0 ? (GPIOA->BSRR = (uint32_t)LCD_D0_Pin << 16) : (GPIOA->BSRR = LCD_D0_Pin);
(eightbit & 2) == 0 ? (GPIOC->BSRR = (uint32_t)LCD_D1_Pin << 16) : (GPIOC->BSRR = LCD_D1_Pin);
(eightbit & 4) == 0 ? (GPIOA->BSRR = (uint32_t)LCD_D2_Pin << 16) : (GPIOA->BSRR = LCD_D2_Pin);
(eightbit & 8) == 0 ? (GPIOB->BSRR = (uint32_t)LCD_D3_Pin << 16) : (GPIOB->BSRR = LCD_D3_Pin);
(eightbit & 16) == 0 ? (GPIOB->BSRR = (uint32_t)LCD_D4_Pin << 16) : (GPIOB->BSRR = LCD_D4_Pin);
(eightbit & 32) == 0 ? (GPIOB->BSRR = (uint32_t)LCD_D5_Pin << 16) : (GPIOB->BSRR = LCD_D5_Pin);
(eightbit & 64) == 0 ? (GPIOB->BSRR = (uint32_t)LCD_D6_Pin << 16) : (GPIOB->BSRR = LCD_D6_Pin);
(eightbit & 128) == 0 ? (GPIOA->BSRR = (uint32_t)LCD_D7_Pin << 16) : (GPIOA->BSRR = LCD_D7_Pin);
GPIOA->BSRR = (uint32_t)LCD_WR_Pin << 16;
GPIOA->BSRR = LCD_WR_Pin;
*/
// with Port B, Pin 0-7 --- fastest ----------------------------------------------------------------
/*
GPIOB->BSRR = 0xFFFF0000;
GPIOB->BSRR = (uint32_t)eightbit;
GPIOA->BSRR = 0x00020000;
//GPIOA->BSRR = (uint32_t)LCD_WR_Pin << 16;
GPIOA->BSRR = LCD_WR_Pin;
*/


//}
