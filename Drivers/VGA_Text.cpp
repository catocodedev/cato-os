#include "../UsefulStuff/Typedefs.h"
#include <port_io.h>

/*********************
* TEXT MODE: 0xB8000 *
* GR.  MODE: 0xA000  *
*********************/

#define VIDEO_MEMORY		(char*)0xB8000
#define VGA_WIDTH	80


/*********************** FUNCTIONS *************************
* SetCursorPosRaw / SetCursorPos: set cursor positon	     *
* print: prints string					     *
* other commented, currently red. or unnec. ones	     *
* 							     *
* 							     *
***********************************************************/

//thanks, OSDEV wiki, for being a thing.

uint16_t CursorPos = 0; 		// Holds the current position of the cursor


void SetCursorPosRaw(uint16_t pos){	// Does some I/O black magic 
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t)(pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
	CursorPos = pos;
	return;
}


void SetCursorPos(int x, int y){
	uint16_t pos = y * VGA_WIDTH + x;
	SetCursorPosRaw(pos);
	
	return;
}

/*uint16_t GetCursorPos()		//Commented for no particular reason, uncomment if needed
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
*/


void print(const char* s){		// Just a simple print function. Prints to screen at cursor position, moves the cursor at the end. 
  uint8_t* charPtr = (uint8_t*)s;
  uint16_t i = CursorPos;
  while(*charPtr != 0)
  {
    switch (*charPtr) {
      case 10:	
      		i+= VGA_WIDTH - i % VGA_WIDTH;	// ALSO ADDS RETURN TO NEWLINE!!
        	break;
      case 13:
        	i -= i % VGA_WIDTH;
        	break;
      default:
      *(VIDEO_MEMORY + i * 2) = *charPtr;
      i++;
    }

    charPtr++;
  }
  SetCursorPosRaw(i);
  return;
}

void MoveCursorLR(int i){
	CursorPos += i;
	SetCursorPosRaw(CursorPos);
	return;
}

void MoveCursorUD(int i){
	CursorPos += VGA_WIDTH * i;
	SetCursorPosRaw(CursorPos);
	return;
}

void ScrollVMem(){
	int i;
	for(i = 0xB8000 + 1998 ; i >= CursorPos; i--){
		*(VIDEO_MEMORY + i * 2) = *(VIDEO_MEMORY + i * 2 - 2);
	}
	//*(VIDEO_MEMORY + i * 2) = 0;
}


void printChar(const char c, bool caps){
	if(c == 8 || c == 10 || (c >= 32 && c <= 255)) {
	switch(c){
		case 10: 						// newline
			CursorPos+=VGA_WIDTH - CursorPos % VGA_WIDTH;	
			break;
		case 8: 						// backspace
			if(CursorPos > 0){
			CursorPos--;
			*(VIDEO_MEMORY + CursorPos * 2) = (char)0;
			}
			break;
		case 127: 						// del
			if(CursorPos < 2000){
			*(VIDEO_MEMORY + CursorPos * 2 + 2) = (char)0;
			}
			break;
		default:
			if(c >= 97 && c <= 172 && caps){
				
				ScrollVMem();
				
				*(VIDEO_MEMORY + CursorPos * 2) = c - 32;
				CursorPos++;
			}
			else if(c >= 48 && c <= 57 && caps){
				
			}
			else{
				
				ScrollVMem();
				
				*(VIDEO_MEMORY + CursorPos * 2) = c;
				CursorPos++;
			}
			
		}
	}
	
	SetCursorPosRaw(CursorPos);
	return;
}



