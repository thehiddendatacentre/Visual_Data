// Pin assignments
#define HSYNC_PIN 7
#define BLANK_PIN 5
#define VSYNC_PIN 6

// Timing macros
#define NOP __asm__ __volatile__ ("nop\n\t")
#define NOP_8 NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP
                      // 10 cycles
#define H_FRONT_PORCH NOP_8
                      // 59 cycles = 7 * NOP_8 + 3 * NOP
#define H_SYNC_PULSE  NOP_8;NOP_8;NOP_8;NOP_8;NOP_8;NOP_8;NOP_8;NOP;NOP;NOP
                      // 28 cycles = 3 * NOP_8 + 4 * NOP
#define H_BACK_PORCH  NOP_8;NOP_8;NOP_8;NOP;NOP;NOP;NOP

// Pin status macros
#define BLANK_LOW  PORTD &= 0b10111111
#define BLANK_HIGH PORTD |= 0b01000000
#define HSYNC_LOW  PORTD &= 0b01111111
#define HSYNC_HIGH PORTD |= 0b10000000
#define VSYNC_LOW  PORTD &= 0b11011111
#define VSYNC_HIGH PORTD |= 0b00100000

// Timer calculation for each horizontal line:
//    Each HSYNC pulse occurs at 31.46875 kHz
//    No prescaler (_BV(CS10))
//    16 MHz = 16000 kHz
//    Number of cycles (for 31.46875 kHz) = 16000/1/31.46875 = 508 cycles
#define CYCLES_PER_LINE 508

volatile int linecount;

void setup() {
    // Initialise pins
    pinMode(HSYNC_PIN, OUTPUT);
    pinMode(BLANK_PIN, OUTPUT);
    pinMode(VSYNC_PIN, OUTPUT);

    // Initialise timer
    noInterrupts();         // Disable interrupts during timer setup
    TCNT1  = 0;
    TCCR1A = 0;
    OCR1A = CYCLES_PER_LINE;
    TCCR1B = _BV(WGM12);    // Set compare match (CTC) mode
    TCCR1B |= _BV(CS10);    // Set no scalar
    TIMSK1 = _BV(OCIE1A);   // Enable timer compare interrupt
    linecount = 0;
    interrupts();           // Start the interrupt timer and all other interrupts
}

// timer compare interrupt service routine
ISR(TIMER1_COMPA_vect) {
    // Timings for horizontal line:
    //     one CPU cycle : 62.5 ns
    //     front porch   : 635.55 ns = 10 cycles
    //     sync pulse    : 3813.31 ns = 61 cycles
    //     back porch    : 1906.65 ns = 30 cycles
    //     toggling pin  : 2 cycles, so subtract 2 from each above

    // set blanking pin low
    BLANK_LOW;
    // Wait 8 cycles
    H_FRONT_PORCH;
    // set h sync pin low
    HSYNC_LOW;
    // Wait 59 cycles
    H_SYNC_PULSE;
    // set h sync pin high
    HSYNC_HIGH;
    // Wait 28 cycles
    H_BACK_PORCH;
    // set blanking pin high, if we are not in a vertical sync period
    if (linecount > 45)
       BLANK_HIGH;

    // Timings for vertical line:
    //      front porch   :  317.77 µs = 10 lines
    //      sync pulse    :   63.55 µs = 2 lines
    //      back porch    : 1048.65 µs = 33 lines

    // process the vertical sync
    switch(linecount) {
        case 0:
            // vertical front porch - 10 lines of BLANK_PIN LOW, and VSYNC_PIN HIGH
            BLANK_LOW;
            break;
        case 10:
            // vertical sync - 2 lines of BLANK_PIN LOW, and VSYNC_PIN LOW
            VSYNC_LOW;
            break;
        case 12:
            // vertical back porch - 33 lines of BLANK_PIN LOW, and VSYNC_PIN HIGH
            VSYNC_HIGH;
            break;
        case 45:
            // end of blanking period - set BLANK_PIN HIGH, VSYNC_PIN remains HIGH
            BLANK_HIGH;
            break;
        case 524:
            // 525 lines - reset lineCount
            linecount = -1;
            break;
    }
    linecount++ ;
}


void loop() {
}
