	UINT8	INT_SPI = 0;
	UINT8	SPI_RD_DATA;
	UINT8     SPI_RD_NUM=0;
	UINT8 	SPI_RD_BUFF[4];

void SPI_ISR(void) interrupt 13
{	
	INT_SPI = 1;
	while (1)
	{
		SPI_RD_DATA = xSPDR;
		if(SPI_RD_NUM < 4)	SPI_RD_BUFF[SPI_RD_NUM] = SPI_RD_DATA;
		SPI_RD_NUM++;
		if(xSPSR & 0x01)	break;	// Read FIFO Empty
	}

	if(SPI_RD_NUM > 4) 	SPI_RD_NUM = 0;	// Error case
	xSPSR = 0x00;
}
UINT8 HAL_SPI_MASTER(UINT8 SendByte)
{
	xSPDR = SendByte;
	while(!INT_SPI);
	INT_SPI = 0;
	return	SPI_RD_DATA;
}

	extern	UINT8 	SPI_RD_BUFF[];
	extern	UINT8     SPI_RD_NUM;
	
// Return
//	1 : Received Data OK
// 	0 : No Received Data
UINT8 HAL_SPI_SLAVE(UINT8* pReceiverBuff, UINT8 NextSendByte)
{
	UINT8 lmt, index;

	lmt = index=0;  	
	xSPCR &= 0x7F;
	if(SPI_RD_NUM)
	{				
		lmt = SPI_RD_NUM;
		while(1)
		{
			pReceiverBuff[index] = SPI_RD_BUFF[index];
			index++;
			if(index >= SPI_RD_NUM)	break;
		}				
		SPI_RD_NUM = 0;
	}	
	xSPCR |= 0x80;

	for(index=0;index<lmt;index++)	xSPDR = NextSendByte;
		
	return	lmt;
}
