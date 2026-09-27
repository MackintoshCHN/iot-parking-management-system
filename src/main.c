#include "my_include.h"

#define F_SIZE      12 //Define display font size; Chinese characters can only be displayed after defining the font bitmap
#define MyLCD_Show(m,n,p)     LCD_ShowString(LCD_GetPos_X(F_SIZE,m),LCD_GetPos_Y(24,n),p,F_SIZE,false)  //Display function 

void scanKeyAnddealKey(void);
void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str);
unsigned char makeSureLinkCount=0; //Ensure connection variable
void displayOfCollectedData(void);//Display collected data

unsigned int i;
char dis0[128];//Temporary array for LCD display
char dis1[22];//Temporary array for LCD display

unsigned char reInFlag = 0;//Prevent repeated refresh
unsigned char checkCarDelay=0;//Delay after vehicle detection
unsigned char jyCheckDelay=0;//Voice reminder detection

#define OPEN_ANGLE 80 //Servo action angle
#define CLOSE_ANGLE -80

#define CYCLE_NUM 10 //Billing cycle; charge 1 yuan when this time is reached

unsigned int moneyA = 0;//Charging amount 
unsigned int moneyB = 0;//Charging amount 
unsigned int moneyC = 0;//Charging amount 

unsigned char timeCountA = 0;//Billing cycle counter 
unsigned char timeCountB = 0;//Billing cycle counter 
unsigned char timeCountC = 0;//Billing cycle counter 

//The following strings correspond to the numbers of the voice audio file names
#define HYNSY       "01111"
#define YYADD_0     "21751"
#define YYADD_1     "21752"
#define YYADD_2     "21753"
#define YYADD_3     "21754"

unsigned char needPlay=0;//Voice playback required
unsigned char delayPlay=0;//Playback delay

#define carPinA PinRead(PA8) //Parking space detection interface
#define carPinB PinRead(PA7) //Parking space detection interface
#define carPinC PinRead(PA6) //Parking space detection interface
#define key_dw  PinRead(PA5) //Position switch detection interface

u8 startCheckWireLess = 0;//Start detecting wireless type
u8 wirelessFlag = 0;//Wireless type: 0 Bluetooth or no wireless, 1 WiFi

int main(void)
{
    USARTx_Init(USART1,9600);//Initialize serial port to 9600
    USARTx_Init(USART2,9600);//Initialize serial port to 9600
    
    My_KEY_Init();//Initialize input
    My_LED_Init(); //Initialize output 
    My_SG90_Init(0 ,TIM2,TIM_CH_2);//Initialize servo
    
    LCD_Init();   //Initialize TFT    
    LCD_Clear(Color16_BLACK);//Clear full screen
    
    BACK_COLOR=Color16_BLACK;FRONT_COLOR=Color16_RED;	 //Set display color  
    MyLCD_Show(5,0,"欢迎您使用");//Display
    MyLCD_Show(4,1,"init..");//Display

    dcs=1;delay_ms(200);dcs=0;
    
    My_SG90_SetAngle(0,20); //Control rotation direction    
    My_JR6001_Init(31); //Initialize voice playback
    delay_ms(200);
    My_SG90_SetAngle(0,CLOSE_ANGLE); //Control rotation direction    
    if(My_JR6001_IsBusy() == false ) My_JR6001_PlayByName(HYNSY); //Play "Welcome"    

    LCD_Clear(Color16_BLACK);//Clear full screen  
    MyLCD_Show(5,0,"位置示意图");//Display
    MyLCD_Show(0,3,"        入口      ");delay_ms(200);//Display data
    MyLCD_Show(0,1,"  A位   B位   C位");delay_ms(200);//Display data     
    LCD_Fill(18,38,18+2,60, Color16_YELLOW);delay_ms(200); //Draw a solid rectangle
    LCD_Fill(18,60,90,62,   Color16_YELLOW);delay_ms(200);//Draw a solid rectangle
    LCD_Fill(90,38,90+2,62, Color16_YELLOW);delay_ms(200);//Draw a solid rectangle
    LCD_Fill(56,38,56+2,68, Color16_YELLOW);    //Draw a solid rectangle    
    
    i = 5;startCheckWireLess=1;//Start detection and cancel after a period of time
    while(i-- && wirelessFlag == 0) //Determine the wireless type at this time
    {USARTSendString(USART1,"ATE0\r\n");  delay_ms(100);}  
    startCheckWireLess = 0;//Cancel detection
    if(wirelessFlag == 1)
    {    
        MyLCD_Show(3,4,"WIFI... ");//Display 
        i = 50;while(i--){delay_ms(100);}      //WiFi startup requires a delay of 5 seconds or more   
//    USARTSendString(USART1,"AT+CWMODE_CUR=3\r\n");// Set mode
//    delay_ms(50); 
//    USARTSendString(USART1,"AT+CWSAP=\"AUAISOUT000\",\"\",1,0\r\n");//Set network name and password
//    delay_ms(50);
//    USARTSendString(USART1,"AT+CIPAP_CUR=\"10.10.10.11\"\r\n");//Set local IP
//    delay_ms(50);
//    USARTSendString(USART1,"AT+CIPMODE=0\r\n");//Set IP mode
//    delay_ms(50);

    }
    else
    {
        for(i=0; i<25; i++)delay_ms(100);//Wait for a while after power-on before stable reading  
    }
   
    LCD_Clear(Color16_BLACK);//Clear full screen    
    BACK_COLOR=Color16_BLACK;FRONT_COLOR=Color16_RED;	 //Set display color  
    FRONT_COLOR=Color16_WHITE;	 //Set display color  
    
    while(1)
    {
        scanKeyAnddealKey();//Scan and process keys

        if(key_dw == 0)//Vehicle entry detected
        {
            checkCarDelay = 10;//Vehicle entry detected
            jyCheckDelay = 10;//Display suggestion
        }
        
        if(myReadFlag_tick == true ) //Scheduled reading time reached
        {
            myReadFlag_tick = false; //Clear flag

            if(checkCarDelay>0)//Countdown
            {
                checkCarDelay--;My_SG90_SetAngle(0,OPEN_ANGLE);//Open servo
                
            }//Control rotation direction

            
            if(carPinA == 0) //Vehicle parked
            {
                timeCountA++;//Start timing
                if(timeCountA >= CYCLE_NUM)
                {
                    timeCountA = 0;//End one timing cycle
                    if(moneyA<20)moneyA++;//Increase amount
                }
            }
  

            if(carPinB == 0) //Vehicle parked
            {
                timeCountB++;//Start timing
                if(timeCountB >= CYCLE_NUM)
                {
                    timeCountB = 0;//End one timing cycle
                    if(moneyB<20)moneyB++;//Increase amount
                }
            }
        

            if(carPinC == 0) //Vehicle parked
            {
                timeCountC++;//Start timing
            
            }
            else
            {timeCountC=0;}//Clear billing timer
            
            displayOfCollectedData();//Display collected data
            
            if(My_JR6001_IsBusy() == 0 )//Voice module is idle
            {
                delayPlay++;//Playback delay to prevent continuous repeated playback
                if(delayPlay>=8)//Play again after stopping playback for a period of time
                {
                    if(needPlay != 0)//Whether voice playback is required
                    {
                        switch (needPlay)
                        {
                        case 1 ://Play voice corresponding to the address
                            My_JR6001_PlayByName(YYADD_0); //Play
                            break;
                        case 2 ://Play voice corresponding to the address
                                My_JR6001_PlayByName(YYADD_1); //Play
                            break;
                        case 3 ://Play voice corresponding to the address
                                My_JR6001_PlayByName(YYADD_2); //Play
                            break;
                        case 4 ://Play voice corresponding to the address
                                My_JR6001_PlayByName(YYADD_3); //Play
                            break;                            
                        default :
                            break;
                        }
                        needPlay = 0;//End playback
                        delayPlay = 0;//Clear delay data
                    }
                }
            }            
        }
                
        if(mySendFlag_tick == true )//Scheduled sending time reached
        {
            mySendFlag_tick = false;//Clear flag
            
            sprintf(dis0,"*SA%dSB%dSC%d",carPinA==0?1:0,carPinB==0?1:0,carPinC==0?1:0);//Print voltage value  
            sprintf(dis0,"%sMA%02dMB%02dMC%02d#\r\n",dis0,(int)moneyA,(int)moneyB,(int)moneyC);//Print voltage value                    
        }
        My_UartMessage_Process();//Process serial port data
        
    }
}


void scanKeyAnddealKey(void)
{
    My_KeyScan();//Scan keys
    
    if(KeyIsPress(KEY_1))
    {
        moneyA = 0;//Clear billing
        checkCarDelay = 10;//Vehicle exit detected
    }

    if(KeyIsPress(KEY_3))
    {
        moneyC = 0;//Clear billing
        checkCarDelay = 10;//Vehicle exit detected  
    }   
    if(KeyIsPress(KEY_4))//Key pressed; used to recalculate after calculation error
    {
        LCD_Clear(Color16_BLACK);FRONT_COLOR=Color16_RED;	 //Set display color  //Clear full screen  
        MyLCD_Show(5,0,"位置示意图");//Display
        MyLCD_Show(0,3,"        入口      ");delay_ms(200);//Display data
        MyLCD_Show(0,1,"  A位   B位   C位");delay_ms(200);//Display data     
        LCD_Fill(18,38,18+2,60, Color16_YELLOW);delay_ms(200); //Draw a solid rectangle
        LCD_Fill(18,60,90,62,   Color16_YELLOW);delay_ms(200);//Draw a solid rectangle
        LCD_Fill(90,38,90+2,62, Color16_YELLOW);delay_ms(200);//Draw a solid rectangle
        LCD_Fill(56,38,56+2,68, Color16_YELLOW);    //Draw a solid rectangle             
        for(i=0; i<25; i++)delay_ms(100);//Wait for a while after power-on before stable reading  
        LCD_Clear(Color16_BLACK);//Clear full screen    
        BACK_COLOR=Color16_BLACK;FRONT_COLOR=Color16_RED;	 //Set display color  
        FRONT_COLOR=Color16_WHITE;	 //Set display color  
        MyLCD_Show(0,1,"车位: A位   B位   C位");//Display data
    }  
    if(KeyIsPress(KEY_5))
    {
        dcs = !dcs;//Switch state
    }   
}

void displayOfCollectedData(void)
{
    FRONT_COLOR=Color16_YELLOW;	 //Set display color  
    sprintf(dis0,"状态:%s  %s  %s",carPinA==0?"已停":"____",carPinB==0?"已停":"____",carPinC==0?"已停":"____");//Print voltage value
    MyLCD_Show(0,2,dis0); //Detect fall

    FRONT_COLOR=Color16_RED;	 //Set display color  
    sprintf(dis1,"收费:%02d元  %02d元  %02d元",(int)moneyA,(int)moneyB,(int)moneyC);//Print voltage value
    MyLCD_Show(0,3,dis1); //Detect fall

    FRONT_COLOR=Color16_YELLOW;	 //Set display color  
    if(jyCheckDelay>0)//Vehicle entered; display suggestion
    {
        jyCheckDelay--;//Vehicle detected; countdown
        if(carPinA == 1)//No vehicle in space A
        {
            MyLCD_Show(0,4,"停车建议:A位 向左走<<");//Suggested parking space and direction
            if(needPlay == 0)needPlay = 1;//Trigger voice playback
        }

        else if(carPinC == 1)//No vehicle in space C
        {
            MyLCD_Show(0,4,"停车建议:C位 向右走>>");//Suggested parking space and direction
            if(needPlay == 0)needPlay = 3;//Trigger voice playback
        }       
             
    }
       
}

void OnGetUartMessage(const _uart_msg_obj *uartMsgRec)
{
    char *strPtr;
    if((strPtr=strstr(uartMsgRec->payload,"CA"))!=NULL)//String received
    {
//        setNum=ParseInteger(strPtr+2,3);//Extract setting parameter        
        moneyA = 0;//Clear billing
        checkCarDelay = 10;//Vehicle exit detected
    }

    if((strPtr=strstr(uartMsgRec->payload,"CC"))!=NULL)//String received
    {
        moneyC = 0;//Clear billing
        checkCarDelay = 10;//Vehicle exit detected  
    }       
   
}


void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str)
{
//    u8 i;
//    for(i=0;i<2;i++)
    {
        My_USART_printf(USARTx,"AT+CIPSEND=%d,%d\r\n",0,strlen(str));  
        delay_ms(10);
        USARTSendBytes(USARTx,(const uint8_t *)str,strlen(str));//Send data
        delay_ms(5);
    }
}

void checkWireLessMode(u8 recBuf)//Check wireless type
{
    static u8 checkIn = 0;//Check whether the first flag has been received
    if(startCheckWireLess == 1 && wirelessFlag == 0)//Start wireless detection
    {
        if(recBuf == 'O')checkIn = 1;
           
    }
}
