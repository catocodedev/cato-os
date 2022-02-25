
//IMPLEMENT STACK SMASHING PROTECTOR!!!!

#include "../UsefulStuff/Typedefs.h"
#include "../Drivers/VGA_Text.h"
#include "../UsefulStuff/Conversions.h"
#include "../UsefulStuff/mem.h"
#include "../CPU/idt.h"
#include "../CPU/isr.h"
#include "../CPU/irq.h"
#include "../CPU/timer.h"
#include "../Drivers/Keyboard.h"
extern const char Fool[];			// Test included binary
extern const char John[];


extern "C" void main(){
	//asm volatile("1: jmp 1b");		// "entry breakpoint" (debug)
	SetCursorPos(0,0);
	print(Fool);
	print("Test, printing integer: ");
	print(toString(10 , 10));
	print(" | ");
	print(toString(10 , 16));
	print(" | ");
	print(toString(10 , 2));
	print("\n");
	//print(John);
	idt_install();
	isrs_install();
	irq_install();
	asm volatile ("sti");
	kb_install();
	
	
	//timer_install();
	//int a = 10/0;
	
	return;
}

