#include <stdint.h> 
#include <rv32i.h>


volatile uint32_t *mtime_low = (volatile uint32_t *) MTIME_BASE_ADDRESS;
volatile uint32_t *mtime_hi = (volatile uint32_t *) (MTIME_BASE_ADDRESS + 4);
volatile uint32_t *mtimecmp_low = (volatile uint32_t *) MTIMECMP_BASE_ADDRESS;
volatile uint32_t *mtimecmp_hi = (volatile uint32_t *) (MTIMECMP_BASE_ADDRESS + 4);
volatile uint32_t *software_interrupt = (volatile uint32_t *) MSIP_BASE_ADDRESS;

// Set current system time.
void mtime_set_time(uint64_t time) {

  union {
    uint64_t uint64;
    uint32_t uint32[sizeof(uint64_t)/sizeof(uint32_t)];
  } time_union;

  time_union.uint64 = time;
  //set it up this way to not trigger false timer interrupt
  *mtime_low = 0;
  *mtime_hi = time_union.uint32[1];
  *mtime_low = time_union.uint32[0];

}


// Get current system time.
uint64_t mtime_get_time(void) {

  union {
    uint64_t uint64;
    uint32_t uint32[sizeof(uint64_t)/sizeof(uint32_t)];
  } time_union;


  time_union.uint32[0] = *mtime_low;
  time_union.uint32[1] = *mtime_hi;

  return time_union.uint64;
}


// Set compare time register (MTIMECMP) for generating interrupts.
void mtime_set_timecmp(uint64_t timecmp) {

  union {
    uint64_t uint64;
    uint32_t uint32[sizeof(uint64_t)/sizeof(uint32_t)];
  } timecmp_union;

  timecmp_union.uint64 = timecmp;

  *mtimecmp_low = -1; // prevent MTIMECMP from temporarily becoming smaller than the lesser of the old and new values
  *mtimecmp_hi = timecmp_union.uint32[1];
  *mtimecmp_low = timecmp_union.uint32[0];
}


// Get compare time register (MTIMECMP).
uint64_t mtime_get_timecmp(void) {

  union {
    uint64_t uint64;
    uint32_t uint32[sizeof(uint64_t)/sizeof(uint32_t)];
  } timecmp_union;

  timecmp_union.uint32[0] = *mtimecmp_low;
  timecmp_union.uint32[1] = *mtimecmp_hi;

  return timecmp_union.uint64;
}

//setup trap handler by setting MTVEC and initially disabling all interrupts
//NOTE: trap handler function MUST HAVE ATTRIBUTE INTERRUPT
void trap_handler_setup(void (*trap_handler)(void)) { //this is a pointer to a function with void arguments and returns void
    csr_write(MTVEC,(uint32_t) trap_handler); //store the address of the function to MTVEC (the input is a pointer which is simply an address)
    //disable all interrupts
    csr_write(MSTATUS, 0);
    csr_write(MIE, 0);
    csr_write(MIP, 0);
}

// trurn on software interrupt
void enable_software_interrupt(void){
	*software_interrupt = 1;
}

// turn off software interrupt
void disable_software_interrupt(void){
	*software_interrupt = 0;
}
