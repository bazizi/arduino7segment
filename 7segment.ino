//We always have to include the library
#include "LedControl.h"

#define NUMELEM(p) (sizeof(p)/sizeof(*(p)))

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(7,5,6,2);
unsigned int digitMap[] = {7, 6, 5, 4, 3, 2, 1, 0};
unsigned char segmentMap[] = {0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
unsigned char englishLetters[] = {
    // 0: a
    segmentMap[0] | segmentMap[1] | segmentMap[2] | segmentMap[4] | segmentMap[5] | segmentMap[6],
    // 1: b
    0x06,
    // 2: c
    segmentMap[3] | segmentMap[4] | segmentMap[5] | segmentMap[0],
    // 3: d
    segmentMap[1] | segmentMap[2] | segmentMap[3] | segmentMap[4] | segmentMap[6] ,
    // 4: e
    segmentMap[0] | segmentMap[1] | segmentMap[3] | segmentMap[4] | segmentMap[5] | segmentMap[6],
    // 5: f
    segmentMap[0] | segmentMap[4] | segmentMap[5] | segmentMap[6],
    // 6: g
    0x06,
    // 7: h
    segmentMap[1] | segmentMap[2] | segmentMap[4] | segmentMap[5] | segmentMap[6],
    // 8: i
    segmentMap[4],
    // 9: j
    0x06,
    // 10: k
    segmentMap[4] | segmentMap[5] | segmentMap[6],
    // 11: l
    0x06,
    // 12: m
    0x06,
    // 13: n
    segmentMap[2] | segmentMap[4] | segmentMap[6],
    // 14: o
    segmentMap[0] | segmentMap[1] | segmentMap[2] | segmentMap[3] | segmentMap[4] | segmentMap[5],
    // 15: p
    0x06,
    // 16: q
    0x06,
    // 17: r
    0x06,
    // 18: s
    segmentMap[0] | segmentMap[5] | segmentMap[6] | segmentMap[3] | segmentMap[2],
    // 19: t
    segmentMap[0] | segmentMap[4] | segmentMap[5],
    // 20: u
    segmentMap[1] | segmentMap[2] | segmentMap[3] | segmentMap[4] | segmentMap[5],
    // 21: v
    segmentMap[2] | segmentMap[3] | segmentMap[4],
    // 22: w
    0x06,
    // 23: x
    0x06,
    // 24: y
    segmentMap[1] | segmentMap[2] | segmentMap[3] | segmentMap[5] | segmentMap[6]
    // 25: z
};

/* we always wait a bit between updates of the display */
unsigned long delaytime=100;

void setup() {
    Serial.begin(9600); 
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

void printWordAtOffset(int phrase[], int cPhrase, int digitOffset)
{
    
    int phraseIndex = 0;
    if(digitOffset < 0)
    {
        phraseIndex = -digitOffset;
        digitOffset = 0;            
    }

    while(digitOffset < NUMELEM(digitMap) && phraseIndex < cPhrase)
    {
        lc.setRow(0, digitMap[digitOffset % NUMELEM(digitMap)], phrase[phraseIndex % cPhrase]);            
        phraseIndex++;
        digitOffset++;
    }

}

void writeOn7Segment() 
{
    char phrase[] = "have a nice day";
    int phraseBinary[sizeof(phrase)/sizeof(*phrase)] = {0};
    char* pPhrase = phrase;
    while(*pPhrase)
    {
        Serial.print(*pPhrase);
        phraseBinary[pPhrase - phrase] = ((*pPhrase == ' ')? 0x00 : englishLetters[*pPhrase - 'a']);
        pPhrase++;
    }

    int offset = NUMELEM(digitMap);
    while(true)
    {
        lc.clearDisplay(0);
        printWordAtOffset(phraseBinary, NUMELEM(phraseBinary), offset);
        offset--;
        delay(500);
        if(offset == -NUMELEM(phraseBinary))
        {
            offset = NUMELEM(digitMap);
        }
    }
} 

/*
  This method will scroll all the hexa-decimal
 numbers and letters on the display. You will need at least
 four 7-Segment digits. otherwise it won't really look that good.
 */
void scrollDigits() {
  
  // for(int i=0;i<13;i++) 
  {
    for(int j = 0; j < NUMELEM(digitMap); j++)
    {
        for(int i=j; i < j + NUMELEM(digitMap); i++)
        {
            lc.setDigit(0,digitMap[i % NUMELEM(digitMap)],i,false);
        }
        delay(250);
        // lc.clearDisplay(0);      
    }
  }
  // lc.clearDisplay(0);
  // delay(delaytime);
}

void loop() { 
  writeOn7Segment();
  // scrollDigits();
}
