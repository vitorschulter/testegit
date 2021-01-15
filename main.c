TESTETETSTETE
  }TESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETETESTETETSTETE
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
