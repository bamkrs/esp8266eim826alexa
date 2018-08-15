/**
 * Modified from:
 * https://sites.google.com/site/arduinohomeautomation/home/unitec_eim826
 */

#ifndef rfTX
#error "rfTX - Pin Undefined"
#endif

//--------------------------------------------------------------------------------
// 1200bps=833us/bit lng=70%=583, sht=30%=250 , cor=correction for pinset/pinclear
#define FRAMEREPEAT 15
#define cor 4
#define lng 583 - cor//583
#define sht 250 - cor//250
// Macros for PWM bit pulse 
#define rfBIT_H  {digitalWrite(rfTX, HIGH); delayMicroseconds(sht); digitalWrite(rfTX, LOW ); delayMicroseconds(lng-5);}
#define rfBIT_L  {digitalWrite(rfTX, HIGH); delayMicroseconds(lng); digitalWrite(rfTX, LOW ); delayMicroseconds(sht-5);}
//--------------------------------------------------------------------------------
/*
Button 1 = Channel 0:         ch_bit2='0' , ch_bit1='0' , ch_bit0='0' , 
Button 2 = Channel 1:         ch_bit2='0' , ch_bit1='0' , ch_bit0='1' ,   
Button 3 = Channel 2:         ch_bit2='0' , ch_bit1='1' , ch_bit0='0' ,  
Button 4 = Channel 3:         ch_bit2='0' , ch_bit1='1' , ch_bit0='1' 
Button All, Channel >= 4,     ch_bit2='1' , ch_bit1='0' , ch_bit0='0' ,  
*/
const byte rfFrame[5] = {
    B00000011, //1
    B00010011, //2
    B00001011, //3
    B00011011, //??4
    B00000111  //alle
};

void setSocket(int sNo, boolean OnOff)
{
  for ( byte j=0;j<FRAMEREPEAT;j++)         // 15 Frame repeats
  {
    for ( byte i=0;i<20;i++) {rfBIT_H}                 // 20 preamble bits
    if (rfFrame[sNo] & B00010000) {rfBIT_H} else {rfBIT_L}  // ChannelBit2
    if (rfFrame[sNo] & B00001000) {rfBIT_H} else {rfBIT_L}  // ChannelBit1
    if (rfFrame[sNo] & B00000100) {rfBIT_H} else {rfBIT_L}  // ChannelBit0
    if (OnOff) {rfBIT_L} else {rfBIT_H}  // LOW=On HIGH=Off Bit
    if (rfFrame[sNo] & B00000001) {rfBIT_H} else {rfBIT_H}  // stopbit always 1                         
    delayMicroseconds(4600); // interFrame delay
  }
}
