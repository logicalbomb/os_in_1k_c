typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// Used for grabbing the address (hence the []) of various linker symbols 
extern char __bss[], __bss_end[], __stack_top[];

void *memset(void *buf, char c, size_t n) {
  uint8_t *p = (uint8_t *) buf;
  while (n--)
    *p++ = c;
  return buf;
}

void kernel_main(void) {
  // Zero-clear the .bss section
  memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
  
  for (;;);
}

// Tells the linker which section to put this function (needed for OpenSBI protocol)
__attribute__((section(".text.boot")))
// This attribute ensures the compiler does not generate junk around this function
__attribute__((naked))
void boot(void) {
  __asm__ __volatile__(
    "mv sp, %[stack_top]\n" // Set the stack pointer
    "j kernel_main\n"       // Jump to the kernel main function
    :
    : [stack_top] "r" (__stack_top) // Pass the stack top address
  );
}

