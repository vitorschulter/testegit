
/* Header -----------------------------------------------------------*/
//COMPANY: AJK SOUND
//MICROCONTROLLER: STM8S003F3
//AUTHOR: VITOR SCHULTER
//WHATSAPP: 47 99111-9377
//09/2020
//CENTRAL STROBO RITIMICO 2.0

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "ws2811.h"
#include "setup.h"
#include "efects.h"

/* Private Variables -----------------------------------------------------------*/
uint16_t delay_ms = 0;
uint16_t audioSensibility = 25;
float adcValue = 0;
bool recordFactoryPattern_flag = 0;
bool newEEPROMRecord_flag = 0;
bool writeNewMemory = 0;
bool gainMode = 0;

uint8_t sampleAdcTimer = 0;
uint16_t newPeakTimer = 0;

uint8_t arrayRed[10];
uint8_t arrayGreen[10];
uint8_t arrayBlue[10];

auto_t	 autoMode = 0;
memory_t memory = 0;
power_t  power = on;
memory_model currentMemory;



extern bool recModeActive;
extern uint16_t timeOutRecMode;
extern uint8_t recRepeatCounter;
extern float audioMuti;
float adcBufferMaxValue = 0;
extern bool sensibility_flag;
extern bool flagButton;
extern bool flagPulse;


/* Private function prototypes -----------------------------------------------*/
void writeEEPROM(void);
void readEEPROM(void);
void readADC(void);

void main(void)
{
	peripheralsSetup();
	
	readEEPROM();
	
	GPIO_WriteHigh(OUT_RED);
	GPIO_WriteHigh(OUT_GREEN);
	GPIO_WriteHigh(OUT_BLUE);

  /* Infinite loop */
  while (1)
  {
		if(GPIO_ReadInputPin(IN_AUX))
		{
			if(flagPulse == 1)
			{
				if(memory < M7) memory++;
				else memory = M1;
				newEEPROMRecord_flag = 1;	
				
				flagPulse = 0;
			}
		}
		
		if(GPIO_ReadInputPin(IN_BTN))
		{
			if(flagButton == 1)
			{
				if(memory < M7) memory++;
				else memory = M1;
				newEEPROMRecord_flag = 1;	
			
				flagButton = 0;
			}
		}	
		
		if(newEEPROMRecord_flag)writeEEPROM(); //grava novo efeito na memoria
		else if(memory != custom) readEEPROM(); //busca na memoria efeitos das memorias gravadas
		
		if(recordFactoryPattern_flag)writeEEPROM(); //volta ao padrao de fabrica

		if(memory == custom)autoMode = autoOff; //desliga o auto caso não use memoria
		
		if(power == on)
		{
			if((currentMemory.efectMemory != audio1) && (currentMemory.efectMemory != audio2))GPIO_WriteLow(LED_AUDIO);
			else GPIO_WriteHigh(LED_AUDIO);
		}
		else
		{
			GPIO_WriteLow(LED_AUDIO);
		}
		
		runEfect(currentMemory.efectMemory);
		
		delay(1);
  }
}

void writeEEPROM(void)
{
	if(newEEPROMRecord_flag)
	{
		newEEPROMRecord_flag = 0;
	
		FLASH_ProgramByte((MEMORY_ADD), memory);
		FLASH_ProgramByte((VERIFY_ADD), VERIFY_VALUE);
		
		FLASH_ProgramByte((CUSTOM_COLOR_ADD), currentMemory.colorMemory);
		FLASH_ProgramByte((CUSTOM_EFECT_ADD), currentMemory.efectMemory);	
		
		if(writeNewMemory)//grava nova configuração na memoria apos apertar REC
		{
			timeOutRecMode = 0;
			recModeActive = 0;
			GPIO_WriteLow(LED_REC);
			recRepeatCounter = 0;
			
			switch(memory)
			{
				case M1:
					FLASH_ProgramByte((M1_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M1_EFECT_ADD), currentMemory.efectMemory);
				break;
				
				case M2:
					FLASH_ProgramByte((M2_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M2_EFECT_ADD), currentMemory.efectMemory);			
				break;		
				
				case M3:
					FLASH_ProgramByte((M3_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M3_EFECT_ADD), currentMemory.efectMemory);				
				break;		
		
				case M4:
					FLASH_ProgramByte((M4_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M4_EFECT_ADD), currentMemory.efectMemory);				
				break;
				
				case M5:
					FLASH_ProgramByte((M5_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M5_EFECT_ADD), currentMemory.efectMemory);			
				break;
		
				case M6:
					FLASH_ProgramByte((M6_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M6_EFECT_ADD), currentMemory.efectMemory);		
				break;	
				
				case M7:
					FLASH_ProgramByte((M7_COLOR_ADD), currentMemory.colorMemory);
					FLASH_ProgramByte((M7_EFECT_ADD), currentMemory.efectMemory);		
				break;	
			}
			writeNewMemory = 0;
		}
	}
	
	if(recordFactoryPattern_flag)//grava padrao de fabruca ba memoria
	{
		recRepeatCounter = 0;
		recordFactoryPattern_flag = 0;
		timeOutRecMode = 0;
		recModeActive = 0;
		GPIO_WriteLow(LED_REC);
		recRepeatCounter = 0;		
		
		FLASH_ProgramByte((MEMORY_ADD), M1);

		//M1
		FLASH_ProgramByte((M1_COLOR_ADD), fastChange);
		FLASH_ProgramByte((M1_EFECT_ADD), normalSpecialStrobe);
		
		//M2
		FLASH_ProgramByte((M2_COLOR_ADD), fastChange);
		FLASH_ProgramByte((M2_EFECT_ADD), normalFastFade);

		//M3
		FLASH_ProgramByte((M3_COLOR_ADD), blue);
		FLASH_ProgramByte((M3_EFECT_ADD), alternateSafetyCar);
		
		//M4
		FLASH_ProgramByte((M4_COLOR_ADD), red);
		FLASH_ProgramByte((M4_EFECT_ADD), normalSafetyCar);
		
		//M5
		FLASH_ProgramByte((M5_COLOR_ADD), fastChange);
		FLASH_ProgramByte((M5_EFECT_ADD), directON);
		
		//M6
		FLASH_ProgramByte((M6_COLOR_ADD), scan);
		FLASH_ProgramByte((M6_EFECT_ADD), degrade);

		//M7
		FLASH_ProgramByte((M7_COLOR_ADD), scan);
		FLASH_ProgramByte((M7_EFECT_ADD), audio1);
		
		//CUSTOM
		FLASH_ProgramByte((CUSTOM_COLOR_ADD), white);
		FLASH_ProgramByte((CUSTOM_EFECT_ADD), specialStrobeAlternate);		

		FLASH_ProgramByte((VERIFY_ADD), VERIFY_VALUE);//certifica que eeprom foi gravada*/
	}

}

void readEEPROM(void)
{
	if((FLASH_ReadByte(VERIFY_ADD)) == VERIFY_VALUE) //verifica se a memoria ja foi gravada
	{
		memory = FLASH_ReadByte(MEMORY_ADD);
		
		switch(memory)
		{
			case M1:
				currentMemory.colorMemory = FLASH_ReadByte(M1_COLOR_ADD); //cor memoria m1
				currentMemory.efectMemory = FLASH_ReadByte(M1_EFECT_ADD); //efeito memoria m1
			break;
			
			case M2:
				currentMemory.colorMemory = FLASH_ReadByte(M2_COLOR_ADD); //cor memoria m2
				currentMemory.efectMemory = FLASH_ReadByte(M2_EFECT_ADD); //efeito memoria m2
			break;		
			
			case M3:
				currentMemory.colorMemory = FLASH_ReadByte(M3_COLOR_ADD); //cor memoria m3
				currentMemory.efectMemory = FLASH_ReadByte(M3_EFECT_ADD); //efeito memoria m3
			break;		
	
			case M4:
				currentMemory.colorMemory = FLASH_ReadByte(M4_COLOR_ADD); //cor memoria m4
				currentMemory.efectMemory = FLASH_ReadByte(M4_EFECT_ADD); //efeito memoria m4
			break;
			
			case M5:
				currentMemory.colorMemory = FLASH_ReadByte(M5_COLOR_ADD); //cor memoria m5
				currentMemory.efectMemory = FLASH_ReadByte(M5_EFECT_ADD); //efeito memoria m5
			break;
	
			case M6:
				currentMemory.colorMemory = FLASH_ReadByte(M6_COLOR_ADD); //cor memoria m6
				currentMemory.efectMemory = FLASH_ReadByte(M6_EFECT_ADD); //efeito memoria m6
			break;
			
			case M7:
				currentMemory.colorMemory = FLASH_ReadByte(M7_COLOR_ADD); //cor memoria m7
				currentMemory.efectMemory = FLASH_ReadByte(M7_EFECT_ADD); //efeito memoria m7
			break;
			
			case custom:
				currentMemory.colorMemory = FLASH_ReadByte(CUSTOM_COLOR_ADD); //cor memoria CUSTOM
				currentMemory.efectMemory = FLASH_ReadByte(CUSTOM_EFECT_ADD); //efeito memoria CUSTOM			
		}
	}
	else 
	{
		recordFactoryPattern_flag = 1;
		writeEEPROM();
	}
}

void readADC(void)
{	
	uint8_t y = 0;
	static uint16_t adcBuffer[50];
	static uint8_t fifoCounter = 0;
	uint8_t i = 0;
	uint8_t blinkLedTime=0;
	
	ADC1->CSR |= 0x03; 
	ADC1_StartConversion();
	while (!ADC1_GetFlagStatus(ADC1_FLAG_EOC));
	adcValue = ADC1_GetConversionValue();
	ADC1_ClearFlag(ADC1_FLAG_EOC);
	ADC1->CSR &= 0xF0;
	
	if(adcValue > 420)adcValue = adcValue - 420;
	else adcValue = 0;

	if(sampleAdcTimer >= 250)//amostragem do buffer a cada 250ms
	{
		sampleAdcTimer = 0;
		adcBuffer[fifoCounter] = adcValue;
		if(fifoCounter < 49) fifoCounter++;
		else fifoCounter = 0;	
	}
	
	audioMuti = (adcValue / adcBufferMaxValue);
	
	if(audioMuti > 1.5)newPeakTimer = 10000;//caso sinal microfone maior que 50% do sinal adcBufferMaxValue ele faz atualizar o adcBufferMaxValue antes do tempo normal
	
	//liminta o audioMuti entre 0 e 1
	if(audioMuti > 1) audioMuti = 1; 
	if(audioMuti < 0) audioMuti = 0;	
	
	//verifica o buffer para carregar o maior valor em adcBufferMaxValue
	if(newPeakTimer >= 10000)
	{
		newPeakTimer = 0;
		adcBufferMaxValue = 0;
		
		for(i = 0; i < 49; i++)
		{
			if(adcBufferMaxValue < adcBuffer[i])adcBufferMaxValue = adcBuffer[i];
		}
	}

	if(power == on)
	{
		if(sensibility_flag)//faz o led do audio piscar quando o maximo/minimo é atingido
		{
			if(gainMode == 1)
			{
				GPIO_WriteHigh(OUT_GAIN);
				blinkLedTime = 200;			
			}
			
			else 
			{
				GPIO_WriteLow(OUT_GAIN);
				blinkLedTime = 100;
			}
	
			for(y = 0; y < 4; y++)
			{
				GPIO_WriteReverse(LED_AUDIO);		
				delay(blinkLedTime);
				GPIO_WriteReverse(LED_AUDIO);	
				delay(blinkLedTime);				
			}
			sensibility_flag = 0;
		}
	}
	else
	{
		GPIO_WriteLow(LED_AUDIO);
	}
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
