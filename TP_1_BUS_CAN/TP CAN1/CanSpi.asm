
_LCD:

;CanSpi.c,40 :: 		void LCD(){
;CanSpi.c,41 :: 		LCD_Cmd(_LCD_CLEAR);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;CanSpi.c,42 :: 		sprintf(buffLcd, "Tr %d,%04d", C1, valeur_pot);
	MOVLW       _buffLcd+0
	MOVWF       FARG_sprintf_wh+0 
	MOVLW       hi_addr(_buffLcd+0)
	MOVWF       FARG_sprintf_wh+1 
	MOVLW       ?lstr_1_CanSpi+0
	MOVWF       FARG_sprintf_f+0 
	MOVLW       hi_addr(?lstr_1_CanSpi+0)
	MOVWF       FARG_sprintf_f+1 
	MOVLW       higher_addr(?lstr_1_CanSpi+0)
	MOVWF       FARG_sprintf_f+2 
	MOVF        _C1+0, 0 
	MOVWF       FARG_sprintf_wh+5 
	MOVF        _C1+1, 0 
	MOVWF       FARG_sprintf_wh+6 
	MOVF        _valeur_pot+0, 0 
	MOVWF       FARG_sprintf_wh+7 
	MOVF        _valeur_pot+1, 0 
	MOVWF       FARG_sprintf_wh+8 
	CALL        _sprintf+0, 0
;CanSpi.c,43 :: 		lcd_out(1, 1,buffLcd);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _buffLcd+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_buffLcd+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;CanSpi.c,44 :: 		sprintf(buffLcd, "Re %d,%04d",C_rcpt, yourpot);;
	MOVLW       _buffLcd+0
	MOVWF       FARG_sprintf_wh+0 
	MOVLW       hi_addr(_buffLcd+0)
	MOVWF       FARG_sprintf_wh+1 
	MOVLW       ?lstr_2_CanSpi+0
	MOVWF       FARG_sprintf_f+0 
	MOVLW       hi_addr(?lstr_2_CanSpi+0)
	MOVWF       FARG_sprintf_f+1 
	MOVLW       higher_addr(?lstr_2_CanSpi+0)
	MOVWF       FARG_sprintf_f+2 
	MOVF        _C_rcpt+0, 0 
	MOVWF       FARG_sprintf_wh+5 
	MOVF        _C_rcpt+1, 0 
	MOVWF       FARG_sprintf_wh+6 
	MOVF        _yourpot+0, 0 
	MOVWF       FARG_sprintf_wh+7 
	MOVF        _yourpot+1, 0 
	MOVWF       FARG_sprintf_wh+8 
	CALL        _sprintf+0, 0
;CanSpi.c,45 :: 		lcd_out(2, 1, buffLcd);
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _buffLcd+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_buffLcd+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;CanSpi.c,46 :: 		}
L_end_LCD:
	RETURN      0
; end of _LCD

_Transmission:

;CanSpi.c,48 :: 		void Transmission(){
;CanSpi.c,49 :: 		buff_envoi[0] = valeur_pot >> 8;
	MOVF        _valeur_pot+1, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       _buff_envoi+0 
;CanSpi.c,50 :: 		buff_envoi[1] = valeur_pot & 0xFF;
	MOVLW       255
	ANDWF       _valeur_pot+0, 0 
	MOVWF       _buff_envoi+1 
;CanSpi.c,51 :: 		buff_envoi[2] = C1;
	MOVF        _C1+0, 0 
	MOVWF       _buff_envoi+2 
;CanSpi.c,52 :: 		CANSPIWrite(ID_1st, buff_envoi,3,Can_Send_Flags);
	MOVLW       79
	MOVWF       FARG_CANSPIWrite_id+0 
	MOVLW       47
	MOVWF       FARG_CANSPIWrite_id+1 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+2 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+3 
	MOVLW       _buff_envoi+0
	MOVWF       FARG_CANSPIWrite_wr_data+0 
	MOVLW       hi_addr(_buff_envoi+0)
	MOVWF       FARG_CANSPIWrite_wr_data+1 
	MOVLW       3
	MOVWF       FARG_CANSPIWrite_data_len+0 
	MOVF        _Can_Send_Flags+0, 0 
	MOVWF       FARG_CANSPIWrite_CANSPI_TX_MSG_FLAGS+0 
	CALL        _CANSPIWrite+0, 0
;CanSpi.c,53 :: 		LCD();
	CALL        _LCD+0, 0
;CanSpi.c,57 :: 		}
L_end_Transmission:
	RETURN      0
; end of _Transmission

_Reception:

;CanSpi.c,58 :: 		void Reception(){
;CanSpi.c,59 :: 		yourpot = RxTx_Data[0] << 8;
	MOVF        _RxTx_Data+0, 0 
	MOVWF       _yourpot+1 
	CLRF        _yourpot+0 
;CanSpi.c,60 :: 		yourpot = yourpot|RxTx_Data[1];
	MOVF        _RxTx_Data+1, 0 
	IORWF       _yourpot+0, 1 
	MOVLW       0
	IORWF       _yourpot+1, 1 
;CanSpi.c,61 :: 		C_rcpt =  RxTx_Data[2];
	MOVF        _RxTx_Data+2, 0 
	MOVWF       _C_rcpt+0 
	MOVLW       0
	MOVWF       _C_rcpt+1 
;CanSpi.c,62 :: 		LCD();
	CALL        _LCD+0, 0
;CanSpi.c,68 :: 		}
L_end_Reception:
	RETURN      0
; end of _Reception

_Plus:

;CanSpi.c,83 :: 		void Plus() {
;CanSpi.c,84 :: 		if (Button(&PORTD, 0, 1, 1)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	CLRF        FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	MOVLW       1
	MOVWF       FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_Plus0
;CanSpi.c,85 :: 		BP1 = 1;
	MOVLW       1
	MOVWF       _BP1+0 
	MOVLW       0
	MOVWF       _BP1+1 
;CanSpi.c,86 :: 		}
L_Plus0:
;CanSpi.c,87 :: 		if (BP1 & Button(&PORTD, 0, 1, 0)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	CLRF        FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	CLRF        FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVLW       0
	MOVWF       R1 
	MOVF        _BP1+0, 0 
	ANDWF       R0, 1 
	MOVF        _BP1+1, 0 
	ANDWF       R1, 1 
	MOVF        R0, 0 
	IORWF       R1, 0 
	BTFSC       STATUS+0, 2 
	GOTO        L_Plus1
;CanSpi.c,88 :: 		BP1 = 0;
	CLRF        _BP1+0 
	CLRF        _BP1+1 
;CanSpi.c,89 :: 		C1++;
	INFSNZ      _C1+0, 1 
	INCF        _C1+1, 1 
;CanSpi.c,90 :: 		Transmission();
	CALL        _Transmission+0, 0
;CanSpi.c,91 :: 		}
L_Plus1:
;CanSpi.c,92 :: 		}
L_end_Plus:
	RETURN      0
; end of _Plus

_Moins:

;CanSpi.c,93 :: 		void Moins() {
;CanSpi.c,94 :: 		if (Button(&PORTD, 1, 1, 1)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	MOVLW       1
	MOVWF       FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	MOVLW       1
	MOVWF       FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_Moins2
;CanSpi.c,95 :: 		BP2 = 1;
	MOVLW       1
	MOVWF       _BP2+0 
	MOVLW       0
	MOVWF       _BP2+1 
;CanSpi.c,96 :: 		}
L_Moins2:
;CanSpi.c,97 :: 		if (BP2 & Button(&PORTD, 1, 1, 0)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	MOVLW       1
	MOVWF       FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	CLRF        FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVLW       0
	MOVWF       R1 
	MOVF        _BP2+0, 0 
	ANDWF       R0, 1 
	MOVF        _BP2+1, 0 
	ANDWF       R1, 1 
	MOVF        R0, 0 
	IORWF       R1, 0 
	BTFSC       STATUS+0, 2 
	GOTO        L_Moins3
;CanSpi.c,98 :: 		BP2 = 0;
	CLRF        _BP2+0 
	CLRF        _BP2+1 
;CanSpi.c,99 :: 		C1--;
	MOVLW       1
	SUBWF       _C1+0, 1 
	MOVLW       0
	SUBWFB      _C1+1, 1 
;CanSpi.c,100 :: 		Transmission();
	CALL        _Transmission+0, 0
;CanSpi.c,101 :: 		}
L_Moins3:
;CanSpi.c,102 :: 		}
L_end_Moins:
	RETURN      0
; end of _Moins

_main:

;CanSpi.c,104 :: 		void main() {
;CanSpi.c,106 :: 		ANSELC = 0;
	CLRF        ANSELC+0 
;CanSpi.c,107 :: 		ANSELA = 2;
	MOVLW       2
	MOVWF       ANSELA+0 
;CanSpi.c,108 :: 		ANSELB = 0;
	CLRF        ANSELB+0 
;CanSpi.c,109 :: 		TRISA.f1 = 1;
	BSF         TRISA+0, 1 
;CanSpi.c,110 :: 		ANSELD = 0;
	CLRF        ANSELD+0 
;CanSpi.c,111 :: 		TRISD.f0 = 1;
	BSF         TRISD+0, 0 
;CanSpi.c,112 :: 		TRISD.f1 = 1;  // Configure AN pins as digital I/O
	BSF         TRISD+0, 1 
;CanSpi.c,118 :: 		Can_Init_Flags = 0;  //
	CLRF        _Can_Init_Flags+0 
;CanSpi.c,119 :: 		Can_Send_Flags = 0;  // clear flags
	CLRF        _Can_Send_Flags+0 
;CanSpi.c,120 :: 		Can_Rcv_Flags = 0;   //
	CLRF        _Can_Rcv_Flags+0 
;CanSpi.c,124 :: 		_CANSPI_TX_NO_RTR_FRAME;
	MOVLW       244
	MOVWF       _Can_Send_Flags+0 
;CanSpi.c,130 :: 		_CANSPI_CONFIG_VALID_XTD_MSG;
	MOVLW       211
	MOVWF       _Can_Init_Flags+0 
;CanSpi.c,133 :: 		Lcd_init();
	CALL        _Lcd_Init+0, 0
;CanSpi.c,134 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
	MOVLW       12
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;CanSpi.c,135 :: 		lcd_out(1,1,"TEST");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr3_CanSpi+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr3_CanSpi+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;CanSpi.c,136 :: 		delay_ms(1000);
	MOVLW       11
	MOVWF       R11, 0
	MOVLW       38
	MOVWF       R12, 0
	MOVLW       93
	MOVWF       R13, 0
L_main4:
	DECFSZ      R13, 1, 1
	BRA         L_main4
	DECFSZ      R12, 1, 1
	BRA         L_main4
	DECFSZ      R11, 1, 1
	BRA         L_main4
	NOP
	NOP
;CanSpi.c,137 :: 		SPI1_Init();  // initialize SPI1 module
	CALL        _SPI1_Init+0, 0
;CanSpi.c,139 :: 		CANSPIInitialize(1, 3, 3, 3, 1, Can_Init_Flags);                        // Initialize external CANSPI module
	MOVLW       1
	MOVWF       FARG_CANSPIInitialize_SJW+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_BRP+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_PHSEG1+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_PHSEG2+0 
	MOVLW       1
	MOVWF       FARG_CANSPIInitialize_PROPSEG+0 
	MOVF        _Can_Init_Flags+0, 0 
	MOVWF       FARG_CANSPIInitialize_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPIInitialize+0, 0
;CanSpi.c,140 :: 		CANSPISetOperationMode(_CANSPI_MODE_CONFIG, 0xFF);                      // set CONFIGURATION mode
	MOVLW       128
	MOVWF       FARG_CANSPISetOperationMode_mode+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetOperationMode_WAIT+0 
	CALL        _CANSPISetOperationMode+0, 0
;CanSpi.c,141 :: 		CANSPISetMask(_CANSPI_MASK_B1, -1, _CANSPI_CONFIG_XTD_MSG);             // set all mask1 bits to ones
	CLRF        FARG_CANSPISetMask_CANSPI_MASK+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+1 
	MOVWF       FARG_CANSPISetMask_val+2 
	MOVWF       FARG_CANSPISetMask_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetMask_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetMask+0, 0
;CanSpi.c,142 :: 		CANSPISetMask(_CANSPI_MASK_B2, -1, _CANSPI_CONFIG_XTD_MSG);             // set all mask2 bits to ones
	MOVLW       1
	MOVWF       FARG_CANSPISetMask_CANSPI_MASK+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+1 
	MOVWF       FARG_CANSPISetMask_val+2 
	MOVWF       FARG_CANSPISetMask_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetMask_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetMask+0, 0
;CanSpi.c,143 :: 		CANSPISetFilter(_CANSPI_FILTER_B2_F4, ID_2nd, _CANSPI_CONFIG_XTD_MSG);  // set id of filter B2_F4 to 2nd node ID
	MOVLW       5
	MOVWF       FARG_CANSPISetFilter_CANSPI_FILTER+0 
	MOVLW       3
	MOVWF       FARG_CANSPISetFilter_val+0 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+1 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+2 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetFilter_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetFilter+0, 0
;CanSpi.c,145 :: 		CANSPISetOperationMode(_CANSPI_MODE_NORMAL, 0xFF);  // set NORMAL mode
	CLRF        FARG_CANSPISetOperationMode_mode+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetOperationMode_WAIT+0 
	CALL        _CANSPISetOperationMode+0, 0
;CanSpi.c,146 :: 		RxTx_Data[0] = 9;  // set initial data to be sent
	MOVLW       9
	MOVWF       _RxTx_Data+0 
;CanSpi.c,147 :: 		CANSPIWrite(ID_1st, RxTx_Data, 1, Can_Send_Flags);  // send initial message
	MOVLW       79
	MOVWF       FARG_CANSPIWrite_id+0 
	MOVLW       47
	MOVWF       FARG_CANSPIWrite_id+1 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+2 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+3 
	MOVLW       _RxTx_Data+0
	MOVWF       FARG_CANSPIWrite_wr_data+0 
	MOVLW       hi_addr(_RxTx_Data+0)
	MOVWF       FARG_CANSPIWrite_wr_data+1 
	MOVLW       1
	MOVWF       FARG_CANSPIWrite_data_len+0 
	MOVF        _Can_Send_Flags+0, 0 
	MOVWF       FARG_CANSPIWrite_CANSPI_TX_MSG_FLAGS+0 
	CALL        _CANSPIWrite+0, 0
;CanSpi.c,151 :: 		while (1) {  // endless loop
L_main5:
;CanSpi.c,152 :: 		Msg_Rcvd = CANSPIRead(&Rx_ID, RxTx_Data, &Rx_Data_Len, &Can_Rcv_Flags);
	MOVLW       _Rx_ID+0
	MOVWF       FARG_CANSPIRead_id+0 
	MOVLW       hi_addr(_Rx_ID+0)
	MOVWF       FARG_CANSPIRead_id+1 
	MOVLW       _RxTx_Data+0
	MOVWF       FARG_CANSPIRead_rd_data+0 
	MOVLW       hi_addr(_RxTx_Data+0)
	MOVWF       FARG_CANSPIRead_rd_data+1 
	MOVLW       _Rx_Data_Len+0
	MOVWF       FARG_CANSPIRead_data_len+0 
	MOVLW       hi_addr(_Rx_Data_Len+0)
	MOVWF       FARG_CANSPIRead_data_len+1 
	MOVLW       _Can_Rcv_Flags+0
	MOVWF       FARG_CANSPIRead_CANSPI_RX_MSG_FLAGS+0 
	MOVLW       hi_addr(_Can_Rcv_Flags+0)
	MOVWF       FARG_CANSPIRead_CANSPI_RX_MSG_FLAGS+1 
	CALL        _CANSPIRead+0, 0
	MOVF        R0, 0 
	MOVWF       _Msg_Rcvd+0 
;CanSpi.c,153 :: 		valeur_pot = ADC_Read(1);  //prise de la valeur du potentiometre
	MOVLW       1
	MOVWF       FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _valeur_pot+0 
	MOVF        R1, 0 
	MOVWF       _valeur_pot+1 
;CanSpi.c,154 :: 		Plus();
	CALL        _Plus+0, 0
;CanSpi.c,155 :: 		Moins();
	CALL        _Moins+0, 0
;CanSpi.c,157 :: 		if ( Msg_Rcvd) {   //(Rx_ID == ID_2nd) &&
	MOVF        _Msg_Rcvd+0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main7
;CanSpi.c,158 :: 		Reception();
	CALL        _Reception+0, 0
;CanSpi.c,160 :: 		}
L_main7:
;CanSpi.c,162 :: 		if (abs(memo_ADC - valeur_pot) > 50) {
	MOVF        _valeur_pot+0, 0 
	SUBWF       _memo_ADC+0, 0 
	MOVWF       FARG_abs_a+0 
	MOVF        _valeur_pot+1, 0 
	SUBWFB      _memo_ADC+1, 0 
	MOVWF       FARG_abs_a+1 
	CALL        _abs+0, 0
	MOVLW       128
	MOVWF       R2 
	MOVLW       128
	XORWF       R1, 0 
	SUBWF       R2, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main15
	MOVF        R0, 0 
	SUBLW       50
L__main15:
	BTFSC       STATUS+0, 0 
	GOTO        L_main8
;CanSpi.c,163 :: 		memo_ADC = valeur_pot;
	MOVF        _valeur_pot+0, 0 
	MOVWF       _memo_ADC+0 
	MOVF        _valeur_pot+1, 0 
	MOVWF       _memo_ADC+1 
;CanSpi.c,164 :: 		Transmission();
	CALL        _Transmission+0, 0
;CanSpi.c,165 :: 		}
L_main8:
;CanSpi.c,167 :: 		}
	GOTO        L_main5
;CanSpi.c,168 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
