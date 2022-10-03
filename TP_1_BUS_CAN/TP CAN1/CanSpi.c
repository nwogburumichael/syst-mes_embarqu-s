unsigned char Can_Init_Flags, Can_Send_Flags, Can_Rcv_Flags;  // can flags
unsigned char Rx_Data_Len;                                    // received data length in bytes
char RxTx_Data[8];                                            // can rx/tx data buffer
char Msg_Rcvd;                                                // reception flag
const long ID_1st = 12111, ID_2nd = 3;                        // node IDs
long Rx_ID;
int BP1 = 0;
int C1 = 0;  //bouton 1
int C_rcpt = 0;
int BP2 = 0;
char buff_envoi[8];
char buffR[2];  //bouton2
char buffLcd[8];
int yourpot = 0;

// CANSPI module connections
sbit CanSpi_CS at RC0_bit;
sbit CanSpi_CS_Direction at TRISC0_bit;
sbit CanSpi_Rst at RC2_bit;
sbit CanSpi_Rst_Direction at TRISC2_bit;
// End CANSPI module connections

sbit LCD_RS at RB4_bit;  //connexion microcontroleur/lcd
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

unsigned int valeur_pot;
int Vitesse_pot;
int memo_ADC;

void LCD(){
     LCD_Cmd(_LCD_CLEAR);
     sprintf(buffLcd, "Tr %d,%04d", C1, valeur_pot);
     lcd_out(1, 1,buffLcd);
     sprintf(buffLcd, "Re %d,%04d",C_rcpt, yourpot);;
     lcd_out(2, 1, buffLcd);
}

void Transmission(){
    buff_envoi[0] = valeur_pot >> 8;
    buff_envoi[1] = valeur_pot & 0xFF;
    buff_envoi[2] = C1;
    CANSPIWrite(ID_1st, buff_envoi,3,Can_Send_Flags);
    LCD();
    /*sprintf(buffLcd, "Tr %d,%04d", C1, valeur_pot);
    lcd_cmd(_lcd_clear);
    lcd_out(1, 1,buffLcd);*/
}
void Reception(){
    yourpot = RxTx_Data[0] << 8;
    yourpot = yourpot|RxTx_Data[1];
    C_rcpt =  RxTx_Data[2];
    LCD();
    /*sprintf(buffLcd, "Re %d,%04d",C_rcpt, yourpot);
    lcd_cmd(_lcd_clear);
    lcd_out(2, 1, buffLcd);*/


}


/*void ADC() {
  valeur_pot = ADC_Read(0);  //prise de la valeur du potentiometre
  //Bcd2Dec(valeur_pot); //prise de valeur sous forme de vitesse
  Vitesse_pot = valeur_pot / 4;
  if (abs(memo_ADC - Vitesse_pot) > 30) {
    //sprintf(buff, "%04d, %04d", C1, Vitesse_pot);
    //lcd_out(1, 1, buff);
    //CANSPIWrite(ID_1st, buff, 3, Can_Send_Flags);
    //memo_ADC = Vitesse_pot;
  }
}  */

void Plus() {
  if (Button(&PORTD, 0, 1, 1)) {
    BP1 = 1;
  }
  if (BP1 & Button(&PORTD, 0, 1, 0)) {
    BP1 = 0;
    C1++;
    Transmission();
  }
}
void Moins() {
  if (Button(&PORTD, 1, 1, 1)) {
    BP2 = 1;
  }
  if (BP2 & Button(&PORTD, 1, 1, 0)) {
    BP2 = 0;
    C1--;
    Transmission();
  }
}

void main() {

  ANSELC = 0;
  ANSELA = 2;
  ANSELB = 0;
  TRISA.f1 = 1;
  ANSELD = 0;
  TRISD.f0 = 1;
  TRISD.f1 = 1;  // Configure AN pins as digital I/O
  //ANSELH = 0;

  // clear PORTB
  //TRISB = 0;                                                  // set PORTB as output

  Can_Init_Flags = 0;  //
  Can_Send_Flags = 0;  // clear flags
  Can_Rcv_Flags = 0;   //

   Can_Send_Flags = _CANSPI_TX_PRIORITY_0 &                    // form value to be used
                   _CANSPI_TX_XTD_FRAME &                     //     with CANSPIWrite
                   _CANSPI_TX_NO_RTR_FRAME;

  Can_Init_Flags = _CANSPI_CONFIG_SAMPLE_THRICE &             // Form value to be used
                   _CANSPI_CONFIG_PHSEG2_PRG_ON &             //  with CANSPIInit
                   _CANSPI_CONFIG_XTD_MSG &
                   _CANSPI_CONFIG_DBL_BUFFER_ON &
                   _CANSPI_CONFIG_VALID_XTD_MSG;


  Lcd_init();
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  lcd_out(1,1,"TEST");
  delay_ms(1000);
  SPI1_Init();  // initialize SPI1 module

  CANSPIInitialize(3, 8, 3, 3, 1, Can_Init_Flags);                        // Initialize external CANSPI module
  CANSPISetOperationMode(_CANSPI_MODE_CONFIG, 0xFF);                      // set CONFIGURATION mode
  CANSPISetMask(_CANSPI_MASK_B1, -1, _CANSPI_CONFIG_XTD_MSG);             // set all mask1 bits to ones
  CANSPISetMask(_CANSPI_MASK_B2, -1, _CANSPI_CONFIG_XTD_MSG);             // set all mask2 bits to ones
  CANSPISetFilter(_CANSPI_FILTER_B2_F4, ID_2nd, _CANSPI_CONFIG_XTD_MSG);  // set id of filter B2_F4 to 2nd node ID

  CANSPISetOperationMode(_CANSPI_MODE_NORMAL, 0xFF);  // set NORMAL mode
  RxTx_Data[0] = 9;  // set initial data to be sent
  CANSPIWrite(ID_1st, RxTx_Data, 1, Can_Send_Flags);  // send initial message



  while (1) {  // endless loop
    Msg_Rcvd = CANSPIRead(&Rx_ID, RxTx_Data, &Rx_Data_Len, &Can_Rcv_Flags);
    valeur_pot = ADC_Read(1);  //prise de la valeur du potentiometre
    Plus();
    Moins();

    if ( Msg_Rcvd) {   //(Rx_ID == ID_2nd) &&
       Reception();
      // send incremented data back
    }

    if (abs(memo_ADC - valeur_pot) > 50) {
      memo_ADC = valeur_pot;
      Transmission();
    }  
    
  }
}