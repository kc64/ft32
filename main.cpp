#include "mbed.h"
#include "bits.h"
#include "types.h"
#include "sequences.h"

byte *fnGetSeqencePtr(int sequence);

/* Determines the fastest and slowest sequence step timing. Times are in
    1/120th of a second (one clock).*/
#define FASTEST_TIME 10.0
#define SLOWEST_TIME 300.0
/* The AnalogIn function scales the voltage input to a float 0.0-1.0. */
#define SLOPE (SLOWEST_TIME - FASTEST_TIME)

/* These coefficients are used to convert the potentiometer input to a 
exponetial curve that mimic the desired response. */
#define A_COEFF 0.0207
#define B_COEFF 3.9
#define C_COEFF -0.0207

/* Setup a serial debug port on the USB interface. */
//Serial pc(USBTX, USBRX); // tx, rx

/* Setup the dipswitch input port. */
BusInOut dipswitch(P0_14, P0_13, P0_12, P0_11, P0_10, P0_9, P0_8, P0_7);
//BusInOut dipswitch(P0_7, P0_8, P0_9, P0_10, P0_11, P0_12, P0_13, P0_14);

/* Setup the pattern output port. */
BusOut lights(P0_23, P0_22, P0_21, P0_20, P0_19, P0_18, P0_17, P0_16);
//BusOut lights(P0_16, P0_17, P0_18, P0_19, P0_20, P0_21, P0_22, P0_23);

/* The potentiometer input port to select the speed of the sequence steps. */
AnalogIn potentiometer(P0_15);

Ticker heartbeat;       /* Simulates the 1/60th of a second clock from the future ZCD. */
//DigitalOut led1(P0_21); /* For debugging inside the interrupt routine. */

byte *ptrSequence;      /* A pointer to the desired sequence. */
word sequenceLength;    /* The length of the desired sequence. */
byte pattern;           /* The current output pattern. */
float speed;            /* The selected speed. */
int slot;               /* The "slot" that the speed potentiometer is in */
int speed_clks;         /* speed in clocks (1/60th sec). */
int clocks;             /* Incremented everytime the interrupt is called. */
int i;

// const float speeds[] = {
//     0.08,    /*  1 */
//     0.09,    /*  2 */
//     0.10,    /*  3 */
//     0.11,    /*  4 */
//     0.12,    /*  5 */
//     0.14,    /*  6 */
//     0.16,    /*  7 */
//     0.18,    /*  8 */
//     0.21,    /*  9 */
//     0.24,    /* 10 */
//     0.29,    /* 11 */
//     0.34,    /* 12 */
//     0.39,    /* 13 */
//     0.47,    /* 14 */
//     0.55,    /* 15 */
//     0.63,    /* 16 */
//     0.73,    /* 17 */
//     0.83,    /* 18 */
//     0.95,    /* 19 */
//     1.10,    /* 20 */
//     1.25,    /* 21 */
//     1.50,    /* 22 */
//     1.75,    /* 23 */
//     2.10,    /* 24 */
//     2.50     /* 25 */
// };

void HB()
{
    clocks++;
    if(clocks > speed_clks) {
        clocks = 0;
        i++;
        if(i >= sequenceLength) {
            i = 0;
        }
        //led1 = !led1;
        pattern = ~ptrSequence[i];
        lights = pattern;
    }
}

int main()
{
    lights.write(0xFF);     /* all lights off */
    
    byte sequence;          /* The current sequence. */

    /* Basic initialization. */
    dipswitch.mode(PullUp);
    dipswitch.input();

    //led1 = 1;
    clocks = 0;
    speed_clks = 100;
    i = 0;
    /* Simulate the 1/120th sec interrupt from the zero crossing detector. */
    heartbeat.attach(&HB, 0.004);

    /* Read the dipswitch */
    sequence = ~dipswitch.read();

    /* Loop through the sequence forever. */
    while(1) {

        /* Read the potentiometer. */
        // slot = (int)(25 * (1.0 - potentiometer)) - 1;
        // speed = speeds[slot];
        
        speed = A_COEFF * exp(B_COEFF*(1.0-potentiometer)) + C_COEFF;
        

        /* Use the inputs to determine internal values. */
        /* Set the sequence number and length. */
        __disable_irq();    // Disable Interrupts
        ptrSequence = fnGetSeqencePtr(sequence);
        sequenceLength = sequenceLengths[sequence];
        /* Changes the analog speed voltage to a time in clocks. */
        speed_clks = SLOPE * speed + FASTEST_TIME;
        __enable_irq();     // Enable Interrupts
        //pc.printf("sequence: %i, Length: %i\r\n", sequence, sequenceLength);


//        pc.printf("clocks: %i, speed_clks: %i\r\n", clocks, speed_clks);
//        pc.printf("i: %i, pattern = %d\r\n", i, pattern);

        wait(0.1);
    }
}

byte *fnGetSeqencePtr(int sequence) {

    byte *ptrSeq = 0;

    switch(sequence) {
        case 0: {
            ptrSeq = (byte *) sequence0;
            break;
        }
        case 1: {
            ptrSeq = (byte *) sequence1;
            break;
        }
        case 2: {
            ptrSeq = (byte *) sequence2;
            break;
        }
        case 3: {
            ptrSeq = (byte *) sequence3;
            break;
        }
        case 4: {
            ptrSeq = (byte *) sequence4;
            break;
        }
        case 5: {
            ptrSeq = (byte *) sequence5;
            break;
        }
        case 6: {
            ptrSeq = (byte *) sequence6;
            break;
        }
        case 7: {
            ptrSeq = (byte *) sequence7;
            break;
        }
        case 8: {
            ptrSeq = (byte *) sequence8;
            break;
        }
        case 9: {
            ptrSeq = (byte *) sequence9;
            break;
        }
        case 10: {
            ptrSeq = (byte *) sequence10;
            break;
        }
        case 11: {
            ptrSeq = (byte *) sequence11;
            break;
        }
        case 12: {
            ptrSeq = (byte *) sequence12;
            break;
        }
        case 13: {
            ptrSeq = (byte *) sequence13;
            break;
        }
        case 14: {
            ptrSeq = (byte *) sequence14;
            break;
        }
        case 15: {
            ptrSeq = (byte *) sequence15;
            break;
        }
        case 16: {
            ptrSeq = (byte *) sequence16;
            break;
        }
        case 17: {
            ptrSeq = (byte *) sequence17;
            break;
        }
        case 18: {
            ptrSeq = (byte *) sequence18;
            break;
        }
        case 19: {
            ptrSeq = (byte *) sequence19;
            break;
        }
        case 20: {
            ptrSeq = (byte *) sequence20;
            break;
        }
        case 21: {
            ptrSeq = (byte *) sequence21;
            break;
        }
        case 22: {
            ptrSeq = (byte *) sequence22;
            break;
        }
        case 23: {
            ptrSeq = (byte *) sequence23;
            break;
        }
        case 24: {
            ptrSeq = (byte *) sequence24;
            break;
        }
        case 25: {
            ptrSeq = (byte *) sequence25;
            break;
        }
        case 26: {
            ptrSeq = (byte *) sequence26;
            break;
        }
        case 27: {
            ptrSeq = (byte *) sequence27;
            break;
        }
        case 28: {
            ptrSeq = (byte *) sequence28;
            break;
        }
        case 29: {
            ptrSeq = (byte *) sequence29;
            break;
        }
        case 30: {
            ptrSeq = (byte *) sequence30;
            break;
        }
        case 31: {
            ptrSeq = (byte *) sequence31;
            break;
        }
        case 32: {
            ptrSeq = (byte *) sequence32;
            break;
        }
        case 33: {
            ptrSeq = (byte *) sequence33;
            break;
        }
        case 34: {
            ptrSeq = (byte *) sequence34;
            break;
        }
        case 35: {
            ptrSeq = (byte *) sequence35;
            break;
        }
        case 36: {
            ptrSeq = (byte *) sequence36;
            break;
        }
        case 37: {
            ptrSeq = (byte *) sequence37;
            break;
        }
        case 38: {
            ptrSeq = (byte *) sequence38;
            break;
        }
        case 39: {
            ptrSeq = (byte *) sequence39;
            break;
        }
        case 40: {
            ptrSeq = (byte *) sequence40;
            break;
        }
        case 41: {
            ptrSeq = (byte *) sequence41;
            break;
        }
        case 42: {
            ptrSeq = (byte *) sequence42;
            break;
        }
        case 43: {
            ptrSeq = (byte *) sequence43;
            break;
        }
        case 44: {
            ptrSeq = (byte *) sequence44;
            break;
        }
        case 45: {
            ptrSeq = (byte *) sequence45;
            break;
        }
        case 46: {
            ptrSeq = (byte *) sequence46;
            break;
        }
        case 47: {
            ptrSeq = (byte *) sequence47;
            break;
        }
        case 48: {
            ptrSeq = (byte *) sequence48;
            break;
        }
        case 49: {
            ptrSeq = (byte *) sequence49;
            break;
        }
        case 50: {
            ptrSeq = (byte *) sequence50;
            break;
        }
        case 51: {
            ptrSeq = (byte *) sequence51;
            break;
        }
        case 52: {
            ptrSeq = (byte *) sequence52;
            break;
        }
        case 53: {
            ptrSeq = (byte *) sequence53;
            break;
        }
        case 54: {
            ptrSeq = (byte *) sequence54;
            break;
        }
        case 55: {
            ptrSeq = (byte *) sequence55;
            break;
        }
        case 56: {
            ptrSeq = (byte *) sequence56;
            break;
        }
        case 57: {
            ptrSeq = (byte *) sequence57;
            break;
        }
        case 58: {
            ptrSeq = (byte *) sequence58;
            break;
        }
        case 59: {
            ptrSeq = (byte *) sequence59;
            break;
        }
        case 60: {
            ptrSeq = (byte *) sequence60;
            break;
        }
        case 61: {
            ptrSeq = (byte *) sequence61;
            break;
        }
        case 62: {
            ptrSeq = (byte *) sequence62;
            break;
        }
        case 63: {
            ptrSeq = (byte *) sequence63;
            break;
        }
        case 64: {
            ptrSeq = (byte *) sequence64;
            break;
        }
        case 65: {
            ptrSeq = (byte *) sequence65;
            break;
        }
        case 66: {
            ptrSeq = (byte *) sequence66;
            break;
        }
        case 67: {
            ptrSeq = (byte *) sequence67;
            break;
        }
        case 68: {
            ptrSeq = (byte *) sequence68;
            break;
        }
        case 69: {
            ptrSeq = (byte *) sequence69;
            break;
        }
        case 70: {
            ptrSeq = (byte *) sequence70;
            break;
        }
        case 71: {
            ptrSeq = (byte *) sequence71;
            break;
        }
        case 72: {
            ptrSeq = (byte *) sequence72;
            break;
        }
        case 73: {
            ptrSeq = (byte *) sequence73;
            break;
        }
        case 74: {
            ptrSeq = (byte *) sequence74;
            break;
        }
        case 75: {
            ptrSeq = (byte *) sequence75;
            break;
        }
        case 76: {
            ptrSeq = (byte *) sequence76;
            break;
        }
        case 77: {
            ptrSeq = (byte *) sequence77;
            break;
        }
        case 78: {
            ptrSeq = (byte *) sequence78;
            break;
        }
        case 79: {
            ptrSeq = (byte *) sequence79;
            break;
        }
        case 80: {
            ptrSeq = (byte *) sequence80;
            break;
        }
        case 81: {
            ptrSeq = (byte *) sequence81;
            break;
        }
        case 82: {
            ptrSeq = (byte *) sequence82;
            break;
        }
        case 83: {
            ptrSeq = (byte *) sequence83;
            break;
        }
        case 84: {
            ptrSeq = (byte *) sequence84;
            break;
        }
        case 85: {
            ptrSeq = (byte *) sequence85;
            break;
        }
        case 86: {
            ptrSeq = (byte *) sequence86;
            break;
        }
        case 87: {
            ptrSeq = (byte *) sequence87;
            break;
        }
        case 88: {
            ptrSeq = (byte *) sequence88;
            break;
        }
        case 89: {
            ptrSeq = (byte *) sequence89;
            break;
        }
        case 90: {
            ptrSeq = (byte *) sequence90;
            break;
        }
        case 91: {
            ptrSeq = (byte *) sequence91;
            break;
        }
        case 92: {
            ptrSeq = (byte *) sequence92;
            break;
        }
        case 93: {
            ptrSeq = (byte *) sequence93;
            break;
        }
        case 94: {
            ptrSeq = (byte *) sequence94;
            break;
        }
        case 95: {
            ptrSeq = (byte *) sequence95;
            break;
        }
        case 96: {
            ptrSeq = (byte *) sequence96;
            break;
        }
        case 97: {
            ptrSeq = (byte *) sequence97;
            break;
        }
        case 98: {
            ptrSeq = (byte *) sequence98;
            break;
        }
        case 99: {
            ptrSeq = (byte *) sequence99;
            break;
        }
        case 100: {
            ptrSeq = (byte *) sequence100;
            break;
        }
        case 101: {
            ptrSeq = (byte *) sequence101;
            break;
        }
        case 102: {
            ptrSeq = (byte *) sequence102;
            break;
        }
        case 103: {
            ptrSeq = (byte *) sequence103;
            break;
        }
        case 104: {
            ptrSeq = (byte *) sequence104;
            break;
        }
        case 105: {
            ptrSeq = (byte *) sequence105;
            break;
        }
        case 106: {
            ptrSeq = (byte *) sequence106;
            break;
        }
        case 107: {
            ptrSeq = (byte *) sequence107;
            break;
        }
        case 108: {
            ptrSeq = (byte *) sequence108;
            break;
        }
        case 109: {
            ptrSeq = (byte *) sequence109;
            break;
        }
        case 110: {
            ptrSeq = (byte *) sequence110;
            break;
        }
        case 111: {
            ptrSeq = (byte *) sequence111;
            break;
        }
        case 112: {
            ptrSeq = (byte *) sequence112;
            break;
        }
        case 113: {
            ptrSeq = (byte *) sequence113;
            break;
        }
        case 114: {
            ptrSeq = (byte *) sequence114;
            break;
        }
        case 115: {
            ptrSeq = (byte *) sequence115;
            break;
        }
        case 116: {
            ptrSeq = (byte *) sequence116;
            break;
        }
        case 117: {
            ptrSeq = (byte *) sequence117;
            break;
        }
        case 118: {
            ptrSeq = (byte *) sequence118;
            break;
        }
        case 119: {
            ptrSeq = (byte *) sequence119;
            break;
        }
        case 120: {
            ptrSeq = (byte *) sequence120;
            break;
        }
        case 121: {
            ptrSeq = (byte *) sequence121;
            break;
        }
        case 122: {
            ptrSeq = (byte *) sequence122;
            break;
        }
        case 123: {
            ptrSeq = (byte *) sequence123;
            break;
        }
        case 124: {
            ptrSeq = (byte *) sequence124;
            break;
        }
        case 125: {
            ptrSeq = (byte *) sequence125;
            break;
        }
        case 126: {
            ptrSeq = (byte *) sequence126;
            break;
        }
        case 127: {
            ptrSeq = (byte *) sequence127;
            break;
        }
        case 128: {
            ptrSeq = (byte *) sequence128;
            break;
        }
        case 129: {
            ptrSeq = (byte *) sequence129;
            break;
        }
        case 130: {
            ptrSeq = (byte *) sequence130;
            break;
        }
        case 131: {
            ptrSeq = (byte *) sequence131;
            break;
        }
        case 132: {
            ptrSeq = (byte *) sequence132;
            break;
        }
        case 133: {
            ptrSeq = (byte *) sequence133;
            break;
        }
        case 134: {
            ptrSeq = (byte *) sequence134;
            break;
        }
        case 135: {
            ptrSeq = (byte *) sequence135;
            break;
        }
        case 136: {
            ptrSeq = (byte *) sequence136;
            break;
        }
        case 137: {
            ptrSeq = (byte *) sequence137;
            break;
        }
        case 138: {
            ptrSeq = (byte *) sequence138;
            break;
        }
        case 139: {
            ptrSeq = (byte *) sequence139;
            break;
        }
        case 140: {
            ptrSeq = (byte *) sequence140;
            break;
        }
        case 141: {
            ptrSeq = (byte *) sequence141;
            break;
        }
        case 142: {
            ptrSeq = (byte *) sequence142;
            break;
        }
        case 143: {
            ptrSeq = (byte *) sequence143;
            break;
        }
        case 144: {
            ptrSeq = (byte *) sequence144;
            break;
        }
        case 145: {
            ptrSeq = (byte *) sequence145;
            break;
        }
        case 146: {
            ptrSeq = (byte *) sequence146;
            break;
        }
        case 147: {
            ptrSeq = (byte *) sequence147;
            break;
        }
        case 148: {
            ptrSeq = (byte *) sequence148;
            break;
        }
        case 149: {
            ptrSeq = (byte *) sequence149;
            break;
        }
        case 150: {
            ptrSeq = (byte *) sequence150;
            break;
        }
        case 151: {
            ptrSeq = (byte *) sequence151;
            break;
        }
        case 152: {
            ptrSeq = (byte *) sequence152;
            break;
        }
        case 153: {
            ptrSeq = (byte *) sequence153;
            break;
        }
        case 154: {
            ptrSeq = (byte *) sequence154;
            break;
        }
        case 155: {
            ptrSeq = (byte *) sequence155;
            break;
        }
        case 156: {
            ptrSeq = (byte *) sequence156;
            break;
        }
        case 157: {
            ptrSeq = (byte *) sequence157;
            break;
        }
        case 158: {
            ptrSeq = (byte *) sequence158;
            break;
        }
        case 159: {
            ptrSeq = (byte *) sequence159;
            break;
        }
        case 160: {
            ptrSeq = (byte *) sequence160;
            break;
        }
        case 161: {
            ptrSeq = (byte *) sequence161;
            break;
        }
        case 162: {
            ptrSeq = (byte *) sequence162;
            break;
        }
        case 163: {
            ptrSeq = (byte *) sequence163;
            break;
        }
        case 164: {
            ptrSeq = (byte *) sequence164;
            break;
        }
        case 165: {
            ptrSeq = (byte *) sequence165;
            break;
        }
        case 166: {
            ptrSeq = (byte *) sequence166;
            break;
        }
        case 167: {
            ptrSeq = (byte *) sequence167;
            break;
        }
        case 168: {
            ptrSeq = (byte *) sequence168;
            break;
        }
        case 169: {
            ptrSeq = (byte *) sequence169;
            break;
        }
        case 170: {
            ptrSeq = (byte *) sequence170;
            break;
        }
        case 171: {
            ptrSeq = (byte *) sequence171;
            break;
        }
        case 172: {
            ptrSeq = (byte *) sequence172;
            break;
        }
        case 173: {
            ptrSeq = (byte *) sequence173;
            break;
        }
        case 174: {
            ptrSeq = (byte *) sequence174;
            break;
        }
        case 175: {
            ptrSeq = (byte *) sequence175;
            break;
        }
        case 176: {
            ptrSeq = (byte *) sequence176;
            break;
        }
        case 177: {
            ptrSeq = (byte *) sequence177;
            break;
        }
        case 178: {
            ptrSeq = (byte *) sequence178;
            break;
        }
        case 179: {
            ptrSeq = (byte *) sequence179;
            break;
        }
        case 180: {
            ptrSeq = (byte *) sequence180;
            break;
        }
        case 181: {
            ptrSeq = (byte *) sequence181;
            break;
        }
        case 182: {
            ptrSeq = (byte *) sequence182;
            break;
        }
        case 183: {
            ptrSeq = (byte *) sequence183;
            break;
        }
        case 184: {
            ptrSeq = (byte *) sequence184;
            break;
        }
        case 185: {
            ptrSeq = (byte *) sequence185;
            break;
        }
        case 186: {
            ptrSeq = (byte *) sequence186;
            break;
        }
        case 187: {
            ptrSeq = (byte *) sequence187;
            break;
        }
        case 188: {
            ptrSeq = (byte *) sequence188;
            break;
        }
        case 189: {
            ptrSeq = (byte *) sequence189;
            break;
        }
        case 190: {
            ptrSeq = (byte *) sequence190;
            break;
        }
        case 191: {
            ptrSeq = (byte *) sequence191;
            break;
        }
        case 192: {
            ptrSeq = (byte *) sequence192;
            break;
        }
        case 193: {
            ptrSeq = (byte *) sequence193;
            break;
        }
        case 194: {
            ptrSeq = (byte *) sequence194;
            break;
        }
        case 195: {
            ptrSeq = (byte *) sequence195;
            break;
        }
        case 196: {
            ptrSeq = (byte *) sequence196;
            break;
        }
        case 197: {
            ptrSeq = (byte *) sequence197;
            break;
        }
        case 198: {
            ptrSeq = (byte *) sequence198;
            break;
        }
        case 199: {
            ptrSeq = (byte *) sequence199;
            break;
        }
        case 200: {
            ptrSeq = (byte *) sequence200;
            break;
        }
        case 201: {
            ptrSeq = (byte *) sequence201;
            break;
        }
        case 202: {
            ptrSeq = (byte *) sequence202;
            break;
        }
        case 203: {
            ptrSeq = (byte *) sequence203;
            break;
        }
        case 204: {
            ptrSeq = (byte *) sequence204;
            break;
        }
        case 205: {
            ptrSeq = (byte *) sequence205;
            break;
        }
        case 206: {
            ptrSeq = (byte *) sequence206;
            break;
        }
        case 207: {
            ptrSeq = (byte *) sequence207;
            break;
        }
        case 208: {
            ptrSeq = (byte *) sequence208;
            break;
        }
        case 209: {
            ptrSeq = (byte *) sequence209;
            break;
        }
        case 210: {
            ptrSeq = (byte *) sequence210;
            break;
        }
        case 211: {
            ptrSeq = (byte *) sequence211;
            break;
        }
        case 212: {
            ptrSeq = (byte *) sequence212;
            break;
        }
        case 213: {
            ptrSeq = (byte *) sequence213;
            break;
        }
        case 214: {
            ptrSeq = (byte *) sequence214;
            break;
        }
        case 215: {
            ptrSeq = (byte *) sequence215;
            break;
        }
        case 216: {
            ptrSeq = (byte *) sequence216;
            break;
        }
        case 217: {
            ptrSeq = (byte *) sequence217;
            break;
        }
        case 218: {
            ptrSeq = (byte *) sequence218;
            break;
        }
        case 219: {
            ptrSeq = (byte *) sequence219;
            break;
        }
        case 220: {
            ptrSeq = (byte *) sequence220;
            break;
        }
        case 221: {
            ptrSeq = (byte *) sequence221;
            break;
        }
        case 222: {
            ptrSeq = (byte *) sequence222;
            break;
        }
        case 223: {
            ptrSeq = (byte *) sequence223;
            break;
        }
        case 224: {
            ptrSeq = (byte *) sequence224;
            break;
        }
        case 225: {
            ptrSeq = (byte *) sequence225;
            break;
        }
        case 226: {
            ptrSeq = (byte *) sequence226;
            break;
        }
        case 227: {
            ptrSeq = (byte *) sequence227;
            break;
        }
        case 228: {
            ptrSeq = (byte *) sequence228;
            break;
        }
        case 229: {
            ptrSeq = (byte *) sequence229;
            break;
        }
        case 230: {
            ptrSeq = (byte *) sequence230;
            break;
        }
        case 231: {
            ptrSeq = (byte *) sequence231;
            break;
        }
        case 232: {
            ptrSeq = (byte *) sequence232;
            break;
        }
        case 233: {
            ptrSeq = (byte *) sequence233;
            break;
        }
        case 234: {
            ptrSeq = (byte *) sequence234;
            break;
        }
        case 235: {
            ptrSeq = (byte *) sequence235;
            break;
        }
        case 236: {
            ptrSeq = (byte *) sequence236;
            break;
        }
        case 237: {
            ptrSeq = (byte *) sequence237;
            break;
        }
        case 238: {
            ptrSeq = (byte *) sequence238;
            break;
        }
        case 239: {
            ptrSeq = (byte *) sequence239;
            break;
        }
        case 240: {
            ptrSeq = (byte *) sequence240;
            break;
        }
        case 241: {
            ptrSeq = (byte *) sequence241;
            break;
        }
        case 242: {
            ptrSeq = (byte *) sequence242;
            break;
        }
        case 243: {
            ptrSeq = (byte *) sequence243;
            break;
        }
        case 244: {
            ptrSeq = (byte *) sequence244;
            break;
        }
        case 245: {
            ptrSeq = (byte *) sequence245;
            break;
        }
        case 246: {
            ptrSeq = (byte *) sequence246;
            break;
        }
        case 247: {
            ptrSeq = (byte *) sequence247;
            break;
        }
        case 248: {
            ptrSeq = (byte *) sequence248;
            break;
        }
        case 249: {
            ptrSeq = (byte *) sequence249;
            break;
        }
        case 250: {
            ptrSeq = (byte *) sequence250;
            break;
        }
        case 251: {
            ptrSeq = (byte *) sequence251;
            break;
        }
        case 252: {
            ptrSeq = (byte *) sequence252;
            break;
        }
        case 253: {
            ptrSeq = (byte *) sequence253;
            break;
        }
        case 254: {
            ptrSeq = (byte *) sequence254;
            break;
        }
        case 255: {
            ptrSeq = (byte *) sequence255;
            break;
        }
    }
    return ptrSeq;
}