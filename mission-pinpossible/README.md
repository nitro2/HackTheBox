# Mission pinpossible

## CHALLENGE DESCRIPTION


## Walkthrough

We got 2 files:
```
2.0M Jul 10  2020 op_pinpossible.logicdata
1.7M Jul  8  2020 security_keypad.jpeg
```

`op_pinpossible.logicdata: CLIPPER COFF executable C1 R1 not stripped - version 30 -Ctnc -Cdnc` -> saleae.com Logic legacy software

We need to download the software to decode the data

![Alt text](logic1.png)


There are 2 lines and 1 repeat pattern =>  it's I2C protocol
Choose SCA for channel 0 and SCL for channel 1

=> Export data to data.txt file
```s
Time [s],Packet ID,Address,Data,Read/Write,ACK/NAK
0.448499000000000,0,0x27,0x08,Write,ACK
0.448728500000000,1,0x27,0x0C,Write,ACK
0.448958500000000,2,0x27,0x08,Write,ACK
0.449248000000000,3,0x27,0x18,Write,ACK
0.449478000000000,4,0x27,0x1C,Write,ACK
0.449707500000000,5,0x27,0x18,Write,ACK
0.452084500000000,6,0x27,0x88,Write,ACK
0.452314500000000,7,0x27,0x8C,Write,ACK
0.452544000000000,8,0x27,0x88,Write,ACK
```

Then we can extract the Data column: `cat data.txt | cut -d ',' -f4 > content.txt`

```s
Data
0x08
0x0C
0x08
0x18
0x1C
0x18
...
```

From the picture, we can see the LCD is connected to a PCF8574 I2C adapter.
So the keypad write some values to LCD to the address 0x27. 
Then we need to know what kind of LCD.

> PCF8574 chip from Texas Instruments, its I2C address is 0x27

Found a link Arduno LCD control by PCF8574: https://lastminuteengineers.com/i2c-lcd-arduino-tutorial/
PCF8574 datasheet: https://www.ti.com/lit/ds/symlink/pcf8574a.pdf
Datasheet LCD: https://components101.com/sites/default/files/component_datasheet/16x2%20LCD%20Datasheet.pdf
Datasheet of LCD controller: https://www.sparkfun.com/datasheets/LCD/HD44780.pdf

Check the Arduino library lib: https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp
I2C LCD adapter schematic: https://avrhelp.mcselec.com/index.html?lcd_i2c_pcf8574.htm



```cpp
void LiquidCrystal_I2C::init_priv()
{
	Wire.begin();
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS; // --> 0x00
	begin(_cols, _rows);  
}
```

So it uses 4 bit mode.

Some important flag:
```c
// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit
```

```cpp
void LiquidCrystal_I2C::expanderWrite(uint8_t _data){                                        
	Wire.beginTransmission(_Addr);
	printIIC((int)(_data) | _backlightval);
	Wire.endTransmission();   
}
```

```cpp
void LiquidCrystal_I2C::send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
       write4bits((highnib)|mode);
	write4bits((lownib)|mode); 
}
```

![Alt text](I2C_LCD_adapter.png)

```s
PCF8574       HD44780
  P7   <--->   DB7
  P6   <--->   DB6
  P5   <--->   DB5
  P4   <--->   DB4
  P3   <--->   K (BL-)
  P2   <--->   E
  P1   <--->   R/W
  P0   <--->   RS
```

![Alt text](LCD2.png)

There are 3 types of instructions: IR, BF+AC, and DR

For 4-bit mode, it sends 2 times on the DB7-DB4 bus to present 1 byte.


Let decode some first data bytes:
```s
Data
0x08 -> 
0x0C
0x08
0x18
0x1C
0x18
...
```

