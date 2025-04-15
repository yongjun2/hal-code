void UART1_ISR (void)
{
#if (ENABLE_UART1)
	UINT8	INT_ID;
	UINT8	PendLen;
	UINT8	i;
	UINT8	NextNum;

	INT_ID = (xU1_IIR >> 1) & 0x07;
	
	if(INT_ID == 0x02)		// Received Data Available
	{
		for(i=0 ; i<8 ; i++)
		{
			NextNum = (r1_in+1) & (UART1_RXBUF_SIZE-1);
			if(NextNum != r1_out)
			{
				r1buf[r1_in] = xU1_RBR;
				r1_in = NextNum;
			}
			else		PendLen = xU1_RBR;
		}
	}
	else if(INT_ID == 0x06)	// Character Timeout
	{
		// Timeout Interrupt
		NextNum = (r1_in+1) & (UART1_RXBUF_SIZE-1);
		if(NextNum != r1_out)
		{
			r1buf[r1_in] = xU1_RBR;
			r1_in = NextNum;
		}
		else		PendLen = xU1_RBR;
	}
	else if(INT_ID == 0x01)	// Tx Holding Empty
	{		
		t1_fifo_empty = 1;
		if(t1_in != t1_out)		
		{
			xU1_THR = t1buf[t1_out];
			t1_out++;
			t1_out &= (UART1_TXBUF_SIZE-1);			
			t1_fifo_empty = 0;
		}				
	}
	else if(INT_ID == 0x03)	// Receiver Line Status
	{		
		NextNum = xU1_LSR;
	}
	else if(INT_ID == 0x00)	// Modem Status
	{

	}
	else						// Unknown ???
	{
	}	
#else
	UINT8	INT_ID;

	INT_ID = (xU1_IIR >> 1) & 0x07;
#endif
}

