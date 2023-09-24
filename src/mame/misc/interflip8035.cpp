// license:BSD-3-Clause
// copyright-holders: Roberto Fresca, Grull Osgo

/******************************************************************************************************************************************************

  Interflip slots - INTERFLIP / RECREATIVOS FRANCO
  ------------------------------------------------

  Driver for early 1982 dual I8035 CPU based Interflip electromechanical slots.
  These machines were the first homologated titles class C in Spain.

  * Toledo

  https://www.recreativas.org/toledo-3365-interflip

  * Sevilla

  https://www.recreativas.org/sevilla-3363-interflip

  * Costa Brava

  https://www.recreativas.org/costa-brava-3364-interflip


*******************************************************************************************************************************************************

  Hardware Notes...


  Main CPU: I8035 @ 6 MHz.
  2x 5101  (SRAM, 256 bytes)
  4K EPROMs

  Sound CPU: I8035 @ 4 MHz.
  2K EPROM

  4051 (8-channel analog multiplexers/demultiplexers) as DAC + resistor 

  3x I8243 (I/O expander)
  1x I8279 (programmable keyboard/display interface)


*******************************************************************************************************************************************************

  * Technical Documentation *
  ===========================

  Lever and Reel Mechanics:

  * Lever:

  This is an electromechanical assembly used by the player to initiate the game by pulling the lever, transitioning it from its resting position
  to its end-of-travel position. The lever mechanism begins in an initial/rest state and is secured by a locking cam, preventing any movement.
  When the player inserts a coin that is accepted by the system, an electromagnet is activated. This electromagnet disengages the cam, thus releasing
  the lever mechanism for the player to actuate.

  During both forward and reverse motion, the lever activates an internal ratchet, creating audio feedback reminiscent of the iconic sounds associated
  with purely mechanical reel-based slot machines. Alongside the ratchet mechanism, a compression spring is engaged, providing a controlled level of
  mechanical resistance to enhance the player's experience. The same ratchet mechanism prevents any backward movement of the lever until it reaches
  its end-of-travel position. Furthermore, once the lever starts its return motion, it can only move in that direction until it reaches its resting point.

  Attached to the axis of this mechanical assembly is a cam designed to interact with two microswitches known as "Lever" and "Auxiliary Lever".

    - Auxiliary Lever Activation: The cam activates the Auxiliary Lever microswitch as it initiates the mechanism's forward movement
      when the player pulls the lever.
    
    - Lever Microswitch Activation: The Lever microswitch is engaged when the cam reaches the end-of-tour position.

  The game is initiated by processing the combination of these signal events within the system.


  * Reels System:

  The reels system comprises a collection of components mounted on a chassis. This system incorporates an electric motor equipped with an integrated reducer,
  facilitating the transmission of motion to an integral shaft through a belt system operating at a 1:1 transmission ratio.

  This primary shaft serves as the conduit for transmitting motion to all the reels in unison. Each individual reel features a clutch mechanism, enabling the
  independent halting of each reel's rotation, even while the primary shaft continues to rotate. Each reel is equipped with a toothed disc to which an electro
  mechanical plunger is affixed. The plunger's function is to disengage the rotation of the reel when its coil is energized.

  The coils, each corresponding to a specific reel, are situated on a bracket affixed to the chassis. Each reel is paired with its dedicated coil and plunger
  assembly. In addition, each reel incorporates a secondary disc with two sets of teeth. One set of teeth is employed for detecting the alignment of each symbol
  or figure, while the other set, featuring a solitary tooth, serves to identify the zero point of rotation.

  The optical detectors, one for each roller, are strategically positioned on a specialized support structure atop the chassis. These detectors comprise an
  electronic board housing two optocouplers, meticulously aligned with the corresponding toothed discs. The reference point concerning the optical detector
  is established at the precise moment when the tooth exits the optical obstruction, a transition from the "On" to the "Off" state in terms of detection logic.

  The operational sequence of the machine during gameplay is as follows:

    1. Initiate the engine.
    2. Activate all coils to permit the rotation of all reels.
    3. Read the sensors to determine the final positions and deactivate the coils individually to halt the reels.
    4. Once all reels have come to a stop, deactivate the engine.


*******************************************************************************************************************************************************

  Games Info
  ==========


  General Test (DSW5 mode test on):

    Test all Input/Output devices :

      1) Blinks on Coin lock, Diverter and Unlock Lever Coils.

      2) Reels Test, one by one, from position 20 to position 1, full round. Count on display, reel number, step number.

      3) Sound Test. Plays coin out sound once.

      4) Coin 1, Coin 2, Coin 3 lamp test, blinking once.

      5) Accepted coin, Insert coin, Fault lamps test, blinking four times all together.

      6) Test switches. Waits for a switch be pressed following the number on display:

          1 - Auxiliary Lever Switch (key 2)
          2 - Door Switch (key O)
          3 - Lever Switch (key 1)
          4 - Reset Payout Switch (key 9)
          5 - Reset Displays Switch (key 8)
          6 - Coin In Switch. (key 5)

          After this, hopper motor turns out to get one coin out to test the switch.

          7 - Hopper coin out (key I) and turn off hopper motor.

      7)  Electro Mechanical Counters test. Send several count impulses to each counter.

    Once finished, the test starts again.


  Reels Test (DSW6 mode test on):

    It's a complete reels test, one by one, stepping from figure to figure.
    Once finished, the test starts again.


  Timing Test (DSW7 mode test on):

    It's to test the KBDC interrupts on Main CPU. Pressing on Reset Payout swith, the counter on display increments when interrupts are taken.
    When other switches are pressed, some activity is shown in the displays, but these signals are not counted.


  Test mode selection has lower priority. If DSW5, DSW6 and DSW7 are all in mode test on, DSW5 test is selected, and so on.
  To select Game Mode all DSW5, DSW6 and DSW7 test modes must be off. All others are ignored.


*******************************************************************************************************************************************************

  Error codes:
  
  01: Physical RAM error.
  02: CPU/MCU error.
  03: Coin-In error.
  04: Coin-Out/Hopper error.
  05: Reels error.
  06: DATA error.
  07: Door open error.


*******************************************************************************************************************************************************

  TODO:
  
  - Hopper support.
  - Trace the main program to find why you can't coin-in without end with an error code.


*******************************************************************************************************************************************************

  Placement of the reels stripes and paytables for all machines...
  (from original notes, subject to verification)

    .---------------------------------------------------.        .--------------------------------------.
    |                    T O L E D O                    |        |            S E V I L L A             |
    |                                                   |        |                                      |
    +---------------------------------------------------+        +--------------------------------------+
    |    LEFT    |  CENTER L  |  CENTER R  |   RIGHT    |        |    LEFT    |   CENTER   |   RIGHT    |
    +------------+------------+------------+------------+        +------------+------------+------------+
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   IF       |   IF       |   IF       |   IF       |        |   IF       |   IF       |   IF       |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   CHERRY   |   BELL     |   2 BAR    |        |   ORANGE   |   1 BAR    |   ORANGE   |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   2 BAR    |   2 BAR    |   PLUM     |   ORANGE   |        |   SEVEN    |   BELL     |   BELL     |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   BELL     |   CHERRY   |   BELL     |        |   BELL     |   PLUM     |   SEVEN    |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   PLUM     |   CHERRY   |   PLUM     |   ORANGE   |        |   ORANGE   |   BELL     |   ORANGE   |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   BELL     |   ORANGE   |   PLUM     |        |   PLUM     |   SEVEN    |   ORANGE   |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   PLUM     |   1 BAR    |   PLUM     |   CHERRY   |        |   ORANGE   |   CHERRY   |   PLUM     |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   CHERRY   |   CHERRY   |   ORANGE   |        |   PLUM     |   2 BAR    |   PLUM     |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   CHERRY   |   BELL     |   2 BAR    |   BELL     |        |   SEVEN    |   2 BAR    |   CHERRY   |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   PLUM     |   CHERRY   |   ORANGE   |        |   CHERRY   |   1 BAR    |   SEVEN    |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   1 BAR    |   BELL     |   PLUM     |   1 BAR    |        |   ORANGE   |   BELL     |   ORANGE   |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   CHERRY   |   CHERRY   |   ORANGE   |        |   2 BAR    |   PLUM     |   1 BAR    |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   PLUM     |   BELL     |   PLUM     |   PLUM     |        |   PLUM     |   1 BAR    |   PLUM     |
    +------------+------------+------------+------------+        +------------+------------+------------+
    |   ORANGE   |   ORANGE   |   CHERRY   |   ORANGE   |        |   ORANGE   |   CHERRY   |   ORANGE   |
    +------------+------------+------------+------------+        +------------+------------##############
    |   PLUM     |   CHERRY   |   PLUM     |   BELL     |        |   ORANGE   |   1 BAR    #   PLUM     #
    +------------########################################        +------------+------------##############
    |   ORANGE   #   BELL     #   ORANGE   #   CHERRY   #        |   1 BAR    |   BELL     |   CHERRY   |
    +------------########################################        ###########################------------+
    |   BELL     |   ORANGE   |   PLUM     |   ORANGE   |        #   PLUM     #   ORANGE   #   ORANGE   |
    ##############------------+------------+------------+        ###########################------------+
    #   ORANGE   #   BELL     |   1 BAR    |   PLUM     |        |   CHERRY   |   BELL     |   PLUM     |
    ##############------------+------------+------------+        +------------+------------+------------+
    |   PLUM     |   CHERRY   |   CHERRY   |   ORANGE   |             LEFT       T-89.15       RIGHT
    +------------+------------+------------+------------+
    |   ORANGE   |   ORANGE   |   PLUM     |   ORANGE   |
    +------------+------------+------------+------------+
         1-T       2-T-95.28       3-T          4-T


    * IF = InterFlip logo.

    The highlighted combination should be placed in the center line when both optos of each reel are in position.
    For Costa Brava, the combination that will be set in the central line is IF-IF-IF.


=================================================================================================================

  Toledo...


       LEFT REEL       CENTER L REEL    CENTER R REEL     RIGHT REEL
    ---------------  ---------------   ---------------  ---------------
       ORANGE           ORANGE            CHERRY           ORANGE
       PLUM             BELL              PLUM             PLUM
       ORANGE           CHERRY            CHERRY           ORANGE
       PLUM             BELL              PLUM             1 BAR
       ORANGE           PLUM              CHERRY           ORANGE
       1 BAR            BELL              2 BAR            BELL
       ORANGE           CHERRY            CHERRY           ORANGE
       CHERRY           1 BAR             PLUM             CHERRY
       ORANGE           BELL              ORANGE           PLUM
       PLUM             CHERRY            PLUM             ORANGE
       ORANGE           BELL              CHERRY           BELL
       PLUM             2 BAR             PLUM             ORANGE
       ORANGE           CHERRY            BELL             2 BAR
       2 BAR            IF                IF               IF
       ORANGE           ORANGE            PLUM             ORANGE
       IF               CHERRY            CHERRY           ORANGE
       ORANGE           BELL              1 BAR            PLUM
       PLUM             ORANGE            PLUM             ORANGE
       ORANGE           BELL              ORANGE           CHERRY
       BELL             CHERRY            PLUM             BELL


         TOLEDO PAYTABLE                      PRIZE
       ------------------------------------- --------

       CHERRY    X         X         X           2
       X         X         X         CHERRY      2
       CHERRY    CHERRY    X         X           4
       X         X         CHERRY    CHERRY      4
       CHERRY    CHERRY    CHERRY    X           8
       X         CHERRY    CHERRY    CHERRY      8
       ORANGE    ORANGE    ORANGE    X          10
       X         ORANGE    ORANGE    ORANGE     10
       PLUM      PLUM      PLUM      X          14
       X         PLUM      PLUM      PLUM       14
       BELL      BELL      BELL      X          20
       X         BELL      BELL      BELL       20
       1 BAR     1 BAR     2 BAR     X          20
       X         2 BAR     1 BAR     1 BAR      20
       1 BAR     2 BAR     1 BAR     X          20
       X         1 BAR     1 BAR     2 BAR      20
       1 BAR     2 BAR     2 BAR     X          20
       X         2 BAR     1 BAR     2 BAR      20
       2 BAR     1 BAR     1 BAR     X          20
       X         1 BAR     2 BAR     1 BAR      20
       2 BAR     1 BAR     2 BAR     X          20
       X         2 BAR     2 BAR     1 BAR      20
       2 BAR     2 BAR     1 BAR     X          20
       X         1 BAR     2 BAR     2 BAR      20
       CHERRY    CHERRY    CHERRY    CHERRY     20
       ORANGE    ORANGE    ORANGE    ORANGE     20
       PLUM      PLUM      PLUM      PLUM       20
       BELL      BELL      BELL      BELL       50
       1 BAR     1 BAR     1 BAR     X          50
       X         1 BAR     1 BAR     1 BAR      50
       2 BAR     2 BAR     2 BAR     X         100
       X         2 BAR     2 BAR     2 BAR     100
       IF        IF        IF        X         200
       X         IF        IF        IF        200
       1 BAR     1 BAR     1 BAR     1 BAR     250
       2 BAR     2 BAR     2 BAR     2 BAR     250
       IF        IF        IF        IF        250...4000


=================================================================================================================

  Costa Brava...


      Central stripe codification T_89.15

      - Left and Right stripes never change.

       LEFT REEL       CENTER REEL       RIGHT REEL
    ---------------  ---------------   ---------------
      CHERRY   19      BELL     19       PLUM     19
      PLUM     18      ORANGE   18       ORANGE   18
      1 BAR    17      BELL     17       CHERRY   17
      ORANGE   16      1 BAR    16       PLUM     16
      ORANGE   15      CHERRY   15       ORANGE   15
      PLUM     14      1 BAR    14       PLUM     14
      2 BAR    13      PLUM     13       1 BAR    13
      ORANGE   12      2 BAR    12       ORANGE   12
      PLUM     11      CHERRY   11       2 BAR    11
      ORANGE   10      BELL     10       ORANGE   10
      CHERRY    9      1 BAR     9       SEVEN     9
      SEVEN     8      2 BAR     8       CHERRY    8
      PLUM      7      2 BAR     7       PLUM      7
      ORANGE    6      CHERRY    6       PLUM      6
      PLUM      5      SEVEN     5       ORANGE    5
      ORANGE    4      BELL      4       ORANGE    4
      BELL      3      PLUM      3       SEVEN     3
      SEVEN     2      BELL      2       BELL      2
      ORANGE    1      1 BAR     1       ORANGE    1
      IF        0      IF        0       IF        0
      CHERRY           BELL              PLUM
      PLUM             ORANGE            ORANGE


      REEL SYMBOLS
      -------------------

      CODE // SYMBOL
      -----------------
         1 .. IF.
         2 .. SEVEN.
         3 .. 2 BAR.
         4 .. 1 BAR.
         5 .. BELL.
         6 .. PLUM.
         7 .. ORANGE.
         8 .. CHERRY.


     Costa Brava plays from 1 to 3 tokens.
     Token 1 bets in the central line.
     Token 2 bets in the central line.
     Token 3 bets in the central line.


         COSTA BRAVA PAYTABLE       PRIZE
       --------------------------  --------
        CHERRY    x         x          2
        x         x         CHERRY     2
        CHERRY    CHERRY    x          5
        x         CHERRY    CHERRY     5
        ORANGE    ORANGE    ORANGE     8
        ORANGE    SEVEN     ORANGE     8
        PLUM      PLUM      PLUM      14
        PLUM      SEVEN     PLUM      14
        CHERRY    CHERRY    CHERRY    14
        CHERRY    SEVEN     CHERRY    14
        BELL      BELL      BELL      18
        BELL      SEVEN     BELL      18
        1 BAR     1 BAR     1 BAR     18
        1 BAR     SEVEN     1 BAR     18
        2 BAR     2 BAR     2 BAR     20
        2 BAR     SEVEN     2 BAR     20
        SEVEN     SEVEN     SEVEN     50
        SEVEN     IF        SEVEN     50
        IF        IF        IF       100 200 300


=================================================================================================================

  Sevilla...


        SEVILLA PAYTABLE            PRIZE
       --------------------------  --------
        CHERRY    X         X          2
        X         X         CHERRY     2
        CHERRY    CHERRY    X          5
        X         CHERRY    CHERRY     5
        ORANGE    ORANGE    ORANGE     8
        ORANGE    SEVEN     ORANGE     8
        PLUM      PLUM      PLUM      14
        PLUM      SEVEN     PLUM      14
        CHERRY    CHERRY    CHERRY    14
        CHERRY    SEVEN     CHERRY    14
        BELL      BELL      BELL      18
        BELL      SEVEN     BELL      18
        1 BAR     1 BAR     1 BAR     18
        1 BAR     SEVEN     1 BAR     18
        2 BAR     2 BAR     2 BAR     20
        2 BAR     SEVEN     2 BAR     20
        SEVEN     SEVEN     SEVEN     50
        SEVEN     IF        SEVEN     50
        IF        IF        IF       100



       LEFT REEL       CENTER REEL       RIGHT REEL
    ---------------  ---------------   ---------------
        ORANGE           1 BAR             PLUM
        ORANGE           CHERRY            1 BAR
        PLUM             1 BAR             ORANGE
        2 BAR            PLUM              2 BAR
        ORANGE           2 BAR             ORANGE
        PLUM             CHERRY            SEVEN
        ORANGE           BELL              CHERRY
        CHERRY           1 BAR             PLUM
        SEVEN            2 BAR             PLUM
        PLUM             2 BAR             ORANGE
        ORANGE           CHERRY            ORANGE
        PLUM             SEVEN             SEVEN
        ORANGE           BELL              BELL
        BELL             PLUM              ORANGE
        SEVEN            BELL              IF
        ORANGE           1 BAR             PLUM
        IF               IF                ORANGE
        CHERRY           BELL              CHERRY
        PLUM             ORANGE            PLUM
        1 BAR            BELL              ORANGE


******************************************************************************************************************************************************/

#include "emu.h"

#include "cpu/mcs48/mcs48.h"
#include "machine/em_reel.h"
#include "machine/i8243.h"
#include "machine/i8279.h"
#include "machine/nvram.h"
#include "sound/dac.h"
#include "sound/samples.h"
#include "speaker.h"

#include "ifslots.lh"


namespace {

class interflip8035_state : public driver_device
{
public:
	interflip8035_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_data_ram(*this, "data_ram", 0x100, ENDIANNESS_LITTLE)
		, m_maincpu(*this, "maincpu")
		, m_audiocpu(*this, "audiocpu")
		, m_ioexp(*this, "ioexp%u", 0U)
		, m_kbdc(*this, "kbdc")
		, m_reels(*this, "emreel%u", 1U)
		, m_samples(*this, "samples")
		, m_outbit(*this, "outbit%u", 0U)
		, m_outbyte(*this, "outbyte%u", 0U)
	{ }

	enum { STEPS_PER_SYMBOL = 168 };

	void add_em_reels(machine_config &config, int symbols, attotime period);	

	void interflip(machine_config &config);

	template <unsigned Reel> int symbol_opto_r();
	template <unsigned Reel> int reel_opto_r();


protected:
	virtual void machine_start() override;
	virtual void machine_reset() override;
	memory_share_creator<uint8_t> m_data_ram;

private:
	void audio_program_map(address_map &map);
	void audio_io_map(address_map &map);
	void main_program_map(address_map &map);
	void main_io_map(address_map &map);

	// Main MCU Interface
	u8 main_io_r(offs_t offset);
	void main_io_w(offs_t offset, u8 data);
	void main_p1_data_w(u8 data);
	void main_p2_w(u8 data);
	u8 main_p2_r();
	u8 m_mp1, m_mp2;
	u8 m_int_flag;

	// Audio MCU Interface	
	u8 audio_io_r(offs_t offset);
	void audio_io_w(offs_t offset, u8 data);	
	u8 audio_p2_r();
	void audio_p2_w(u8 data);
	u8 m_audio;

	// I8243 IO Expander x 3
	void exp2_p4_w(u8 data);
	void exp2_p5_w(u8 data);
	void exp2_p6_w(u8 data);
	void exp2_p7_w(u8 data);
	void exp3_p4_w(u8 data);
	void exp3_p6_w(u8 data);

	// I8279 Interface
	u8 kbd_rl_r();
	void kbd_sl_w(u8 data);
	void disp_w(u8 data);
	void irq_w(int state);
	void output_digit(int i, u8 data);
	u8 m_kbd_sl;

	// other
	required_device<i8035_device> m_maincpu;
	required_device<i8035_device> m_audiocpu;
	required_device_array<i8243_device, 3> m_ioexp;
	required_device<i8279_device> m_kbdc;
	required_device_array<em_reel_device, 4> m_reels;
	required_device<samples_device> m_samples;	

	// object finders
	output_finder<50> m_outbit;
	output_finder<20> m_outbyte;

};


#define MAIN_CLOCK      XTAL(6'000'000)
#define SND_CLOCK       XTAL(4'000'000)


/******************************************
*          Machine Start & Reset          *
******************************************/

void interflip8035_state::machine_start()
{
	m_outbit.resolve();
	m_outbyte.resolve();
}

void interflip8035_state::machine_reset()
{}


/*********************************************
*           Memory Map Information           *
*********************************************/

void interflip8035_state::main_program_map(address_map &map)
{
	map(0x000, 0xfff).rom();
}

void interflip8035_state::main_io_map(address_map &map)
{
	map(0x00, 0xff).rw(FUNC(interflip8035_state::main_io_r), FUNC(interflip8035_state::main_io_w));
}


void interflip8035_state::audio_program_map(address_map &map)
{
	map(0x000, 0x7ff).rom();
}

void interflip8035_state::audio_io_map(address_map &map)
{
	map(0x00, 0xff).rw(FUNC(interflip8035_state::audio_io_r), FUNC(interflip8035_state::audio_io_w));
}


/*************************************************
*     I8035 MPU Interface (Main & Audio)         *
**************************************************/

u8 interflip8035_state::main_io_r(offs_t offset)
{
	u8 ret;
	switch (m_mp2)
	{
		case 0x3f:  // gpkd A0 (data read/write)
		{
			ret = m_kbdc->data_r();
			// logerror("KDBC Data Read Offs:%02X - Data:%02X\n", offset, ret);
			break;
		}
		case 0x7f:  // gpkd A1 (stat read / cmd write)
		{
			ret = m_kbdc->status_r();
			logerror("KDBC Status Read Offs:%02X - Data:%02X\n", offset, ret);
			break;
		}
		case 0xef:  // NVRAM access
		{
			ret = m_data_ram[offset];
			// logerror("Data RAM Read Offs:%02X - Data:%02X\n", offset, ret);
			break;
		}
		default: 
			ret =  0xff;
	}
	return ret;
}

void interflip8035_state::main_io_w(offs_t offset, u8 data)
{
	switch (m_mp2)
	{
		case 0x3f:  // gpkd A0 (data read/write)
		{
			m_kbdc->data_w(data);
			// logerror("GPKD DATA Write Offs:%02X - Data:%02X\n", offset, data);
			break;
		}
		case 0x7f:  // gpkd A1 (stat read/ cmd write)
		{
			m_kbdc->cmd_w(data);
			// logerror("GPKD CMD Write Offs:%02X - Data:%02X\n", offset, data);
			break;
		}
		case 0xef:  // NVRAM access
		{
			m_data_ram[offset] = data;
			// logerror("Data RAM Write Offs:%02X - Data:%02X\n", offset, data);
			break;
		}
	}
	// logerror("%s: Main I/O Write Offs:%02X - Data: %02X\n", machine().describe_context(), offset, data);	

/* gpkd commands ( under revision)

CMD  Binary
---- ---- ---- 
0x25 0010-0101 -> 001 (1) - 00101	Code=1: Program Clock Divider 5 

0x0C 0000-1100 -> 000 (0) - 01-100	Code=0: Keyboard Display Mode Set. 
									DD=01: 16 8bit character display.
									KKK=100: Encoded Scan Sensor Matrix.

0xD6 1101-0110 -> 110 (6) - 101-10	Code=6: Clear
									CD CD CD = 101: Clear all Zeroes
									CF = 1: Fifo Status Cleared an the interrupt output line is reset.
									        Also, the sensor raw pointer is set to row 0.
									CA = 0: No Clear all bit.

0xA5 1010-0101 -> 101 (5) - 0-0101  Code=5: Display Write inhibit/blanking
									x       = 0: 
									IWA IWB = 01: Inhibit port B
									BLA BLB = 01: Blank port B

0x90 1001-0000 -> 100 (4) - 1-0000  Code = 4: Write display Ram
									AI = 1  : Autoincrement
									AD AD AD AD = 0000: Buffer base address to write.

Then MPU sends gpkd data.

*/

}


u8 interflip8035_state::audio_io_r(offs_t offset)
{
	// logerror("Audio I/O Read Offs:%02X\n", offset);
	return 0xff;
}

void interflip8035_state::audio_io_w(offs_t offset, u8 data)
{
	// logerror("Audio I/O Write Offs:%02X - Data: %02X\n", offset, data);
}


void interflip8035_state::main_p1_data_w(u8 data)
{
/*
	Port P1 Maincpu
	===============

	P1.0 Lamp 1st. Coin
	P1.1 Lamp 2nd. Coin
	P1.2 Lamp 3rd. Coin
	P1.3 Interrupt Flag (enable/disable /INT via NAND Gate.)
	P1.4 /CS PIA 1 - Debug: MPU usually writes 0x67, 0x6f, 0xe7 or 0xef to enable PIA access
	P1.5 /CS PIA 2 - Debug: MPU usually writes 0x57, 0x5f, 0x57 or 0x5f to enable PIA access
	P1.6 /CS PIA 3 - Debug: MPU usually writes 0x37, 0x3f, 0x37 or 0x3f to enable PIA access
	P1.7 /GPKD Reset (Not implemented on device)
*/

	m_mp1 = data;
	m_outbit[0] = data & 0x01;             // Lamp: 1st. Coin
	m_outbit[1] = ( data >> 1 ) & 0x01;    // Lamp: 2nd. Coin
	m_outbit[2] = ( data >> 2 ) & 0x01;    // Lamp: 3rd. Coin
	m_int_flag = ( data >> 3 ) & 0x01;     // Main Interrupt Flag
	m_ioexp[0]->cs_w((data >> 4) & 0x01);  // Chip Select IO Expander_1 
	m_ioexp[1]->cs_w((data >> 5) & 0x01);  // Chip Select IO Expander_2
	m_ioexp[2]->cs_w((data >> 6) & 0x01);  // Chip Select IO Expander_3
	// m_kbdc->reset((data>>7) & 0x01);    // Reset GPKD (not implemented on device)

	// logerror("Main P1 Write: %02X\n", data);
	// logerror("Main Interrupt Flag: %02X\n", m_int_flag);
	m_outbit[44] = m_int_flag;
}


void interflip8035_state::main_p2_w(u8 data)
{
/*
	Port P2 Maincpu
	===============

	P2.0 A8  Address line
	P2.1 A9  Address line
	P2.2 A10 Address line
	P2.3 A11 Address line
	P2.4 /CE1 RAM 1 - Debug: MPU usually writes 0xef to enable NVRAM access
	P2.5 /CE1 RAM 2 - Unused on these games
	P2.6 /A0 GPKD   - Selects -> [Data (rw)] or [Status(r)/ Control(w)] access on I8279
	P2.7 /CS GPKD   - Enable I8279 access. 

	GPKD Notes: MPU writes 0x3f to data access and 0x7f to status/control
*/

	m_mp2 = data;
	m_ioexp[0]->p2_w(data);
	m_ioexp[1]->p2_w(data);
	m_ioexp[2]->p2_w(data);

	// debug
	// logerror("Main P2 Write: %02X\n", data);
}


u8 interflip8035_state::main_p2_r()
{
/*
	P1.4 /CS PIA 1 - Debug: MPU usually writes 0x67, 0x6f, 0xe7 or 0xef to enable PIA access
	P1.5 /CS PIA 2 - Debug: MPU usually writes 0x57, 0x5f, 0xd7 or 0xdf to enable PIA access
	P1.6 /CS PIA 3 - Debug: MPU usually writes 0x37, 0x3f, 0xb7 or 0xbf to enable PIA access
*/
	u8 opt, ret;
	opt = (m_mp1 & 0x70) >> 4;  // valid PIA selectors are 3, 5, 6
	switch (opt)
	{
		case 0x6:  // PIA 1 access
		{
			ret = m_ioexp[0]->p2_r();
			// logerror("%s:PIA 1 Data Read Data:%02X\n", machine().describe_context(), ret);
			break;
		}
		case 0x5:  // PIA 2 access
		{
			ret = m_ioexp[1]->p2_r();
			// logerror("%s:PIA 2 Data Read Data:%02X\n", machine().describe_context(), ret);
			break;
		}
		case 0x3:  // PIA 3 access
		{
			ret = m_ioexp[2]->p2_r();
			// logerror("%s:PIA 3 Data Read Data:%02X\n", machine().describe_context(),ret);
			break;
		}
		default:
		{
			ret = 0xff;
			// logerror("%s:Unk. Main P2 Data\n", machine().describe_context()); // debug
		}
	}
	return ret;
}


u8 interflip8035_state::audio_p2_r()
{
/*
	P2.4 Sound Code 0
	P2.5 Sound Code 1
	P2.6 Sound Code 2
*/
	return m_audio;
}


void interflip8035_state::audio_p2_w(u8 data)
{
	m_outbit[28] = ( data >> 7 ) & 1;  //	P2.7 Topper Lamp

	if(!m_outbit[28])
		m_samples->start(0, 0, true);
	else
		m_samples->stop(0);
}


/****************************************************************

             I8243 IO Expander Interface x 3                

	Access:
	P1 -> Enable PIA Access
	IORW -> Destination Port (4, 5, 6, 7) 0xf4, 0xf5, 0xf6, 0xf7  

****************************************************************/

void interflip8035_state::exp2_p4_w(u8 data)
{
// All active "0" via PNP + NPN open colector transistor driver

	m_outbit[10] = data & 0x01;         // Coil: Lock Reel D (only "Toledo" model)
	m_outbit[11] = (data >> 1) & 0x01;  // Coil: Lock Reel C
	m_outbit[12] = (data >> 2) & 0x01;  // Coil: Lock Reel B
	m_outbit[13] = (data >> 3) & 0x01;  // Coil: Lock Reel A

	m_reels[0]->set_state(!m_outbit[10]);
	m_reels[1]->set_state(!m_outbit[11]);
	m_reels[2]->set_state(!m_outbit[12]);
	m_reels[3]->set_state(!m_outbit[13]);
}

void interflip8035_state::exp2_p5_w(u8 data)
{
// All active "0" via PNP + NPN open colector transistor driver

	m_outbit[14] = data & 0x01;         // Coil: Coin Lock
	m_outbit[15] = (data >> 1) & 0x01;  // Coil: Coin Diverter
	m_outbit[16] = (data >> 2) & 0x01;  // Coil: Unlock Lever
	m_outbit[17] = (data >> 3) & 0x01;  // Unused
}

void interflip8035_state::exp2_p6_w(u8 data)
{
// All active "1" via ULN2803 Darlington array

	m_outbit[20] = data & 0x01;         // EM.Counter: Coin In
	m_outbit[21] = (data >> 1) & 0x01;  // EM.Counter: Coin Out
	m_outbit[22] = (data >> 2) & 0x01;  // EM.Counter: Coin Drop
	m_outbit[23] = (data >> 3) & 0x01;  // EM.Counter: Jackpot Times

	machine().bookkeeping().coin_counter_w(0, BIT(data, 0));  // EM.Counter: Coin In
	machine().bookkeeping().coin_counter_w(1, BIT(data, 1));  // EM.Counter: Coin Out
	machine().bookkeeping().coin_counter_w(2, BIT(data, 2));  // EM.Counter: Coin Drop
	machine().bookkeeping().coin_counter_w(3, BIT(data, 3));  // EM.Counter: Jackpot Times
}

void interflip8035_state::exp2_p7_w(u8 data)
{
// All active "1" via ULN2803 Darlington array

	m_outbit[24] = data & 0x01;         // Relay: Hopper Motor
	m_outbit[25] = (data >> 1) & 0x01;  // Relay: Reels Motor (Motoreductor)
	m_outbit[26] = (data >> 2) & 0x01;  // Unused
	m_outbit[27] = (data >> 3) & 0x01;  // Unused
}

void interflip8035_state::exp3_p4_w(u8 data)
{
// All active "0" via PNP + NPN open colector transistor driver

	m_outbit[3] = data & 0x01;         // Lamp: Accepted Coin
	m_outbit[4] = (data >> 1) & 0x01;  // Lamp: Insert Coin
	m_outbit[5] = (data >> 2) & 0x01;  // Lamp: Fault
}

void interflip8035_state::exp3_p6_w(u8 data)
{
/*  Swapped Sound Control bits
	==========================
	IO_Exp_Data    ->	Audio MPU P2
	bit 0 - /Int		/Int
	bit 1 - Code2		P2.6
	bit 2 - Code1		P2.5
	bit 3 - Code0		P2.4
*/

	u8 state;
	state = data & 0x01;

	m_audio = bitswap<8>(data, 0, 1, 2, 3, 7, 6, 5, 4);  // IO Expander_3 to Sound Board
	m_audiocpu->set_input_line(INPUT_LINE_IRQ0, state ? CLEAR_LINE : ASSERT_LINE);
	//logerror("I8243_3: Sound Code + Int   Data:%02X  State:%02X\n", data, state);
}


/*********************************************
*      I8279 Keyboard-Disply Interface       *
*********************************************/

void interflip8035_state::kbd_sl_w(u8 data)
{
//  Scan Line
	m_kbd_sl = data;
    // logerror("I8279: Scan Line: %02X\n", data);
}

u8 interflip8035_state::kbd_rl_r()
{
//  Keyboard read (only scan line 0 is used)
	if(m_kbd_sl == 0)
	{
		logerror("I8279: Read Line0: %02X\n", ioport("IN0")->read());
		return ioport("IN0")->read();
	}
	return 0xff;
}

void interflip8035_state::disp_w(u8 data)
{
//  Display Data
	output_digit(m_kbd_sl, data >> 4);
}

void interflip8035_state::output_digit(int i, u8 data)
{
//  Segment Decode
	static const u8 led_map[16] =
		{ 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67, 0x58, 0x4c, 0x62, 0x69, 0x78, 0x00 }; 
//  show layout 
	m_outbyte[i] = led_map[data & 0x0f];
}

void interflip8035_state::irq_w(int state)
{
//  KBD Interrupt ( Enabled by maincpu P1.3 )
	if(m_int_flag == 1)
		m_maincpu->set_input_line(INPUT_LINE_IRQ0, state ? ASSERT_LINE : CLEAR_LINE);
	//logerror("I8279: irq state: %s  | Int_flag T0:%02x\n", state ? "Assert Line":"Clear Line" , m_int_flag);
}


/*************************************************
*                Reels Emulation                 *
*************************************************/

void interflip8035_state::add_em_reels(machine_config &config, int symbols, attotime period)
{
	for(int i = 0; i < 4; i++)
	{
		std::set<uint16_t> detents;
		for(int i = 0; i < symbols; i++)
			detents.insert(i * STEPS_PER_SYMBOL);

		EM_REEL(config, m_reels[i], symbols * STEPS_PER_SYMBOL, detents, period);
		m_reels[i]->set_direction(em_reel_device::dir::FORWARD);
	}
}

template <unsigned Reel>
int interflip8035_state::symbol_opto_r()
{
	uint8_t const sym_pos = m_reels[Reel]->get_pos() % STEPS_PER_SYMBOL;
	m_outbit[34 + Reel] = (sym_pos >= 12 && sym_pos <= 19);  // internal layout opto state
	return (sym_pos >= 12 && sym_pos <= 19);                 // symbol tab is on every symbol
}

template <unsigned Reel>
int interflip8035_state::reel_opto_r()
{
	uint16_t const pos = m_reels[Reel]->get_pos();
	m_outbit[30 + Reel] = (pos >= 3278 && pos <= 3359);  // internal layout opto state
	m_outbit[40 + Reel] = pos / 2.8;                     // internal layout reel position
	return (pos >= 3278 && pos <= 3359);                 // reel tab is only on the first symbol
}


/*************************************************
*                 Sound Samples                  *
*************************************************/

static const char *const interflip8035_sample_names[] =
{
	"*samples",
	"ringbellm",        // ring bell
	nullptr
};


/*********************************************
*                Input Ports                 *
*********************************************/

static INPUT_PORTS_START( interflip )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START2 )  PORT_NAME("Auxiliary Lever")                     // auxiliary lever
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_GAMBLE_DOOR )                                              // door
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_GAMBLE_PAYOUT )                                            // payout
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_START1 )  PORT_NAME("Lever")                               // lever
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )                                                   // unused
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER )   PORT_NAME("Display Reset") PORT_CODE(KEYCODE_8)  // display reset
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER )   PORT_NAME("Payout Reset")  PORT_CODE(KEYCODE_9)  // payout reset
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN1 )                                                    // coin in

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER )   PORT_NAME("IN1-1")  PORT_CODE(KEYCODE_Q)  PORT_TOGGLE  // Hopper Full Sensor
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("OPTOS_A")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, reel_opto_r<0>)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, reel_opto_r<1>)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, reel_opto_r<2>)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, reel_opto_r<3>)

	PORT_START("OPTOS_B")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, symbol_opto_r<0>)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, symbol_opto_r<1>)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, symbol_opto_r<2>)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_CUSTOM ) PORT_READ_LINE_MEMBER(interflip8035_state, symbol_opto_r<3>)

//  Test mode selection has lower priority. If DSW5, DSW6 and DSW7 are all Off, DSW5 is selected, and so on.
//  To select Game Mode all DSW5, DSW6 and DSW7 must be On. All others are ignored.

	PORT_START("DSW_A")
	PORT_DIPNAME(0x01, 0x00, DEF_STR(Unused))	PORT_DIPLOCATION("SW1:1")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x01, DEF_STR(On))
	PORT_DIPNAME(0x02, 0x00, DEF_STR(Unused))	PORT_DIPLOCATION("SW1:2")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x02, DEF_STR(On))
	PORT_DIPNAME(0x04, 0x00, DEF_STR(Unused))	PORT_DIPLOCATION("SW1:3")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x04, DEF_STR(On))
	PORT_DIPNAME(0x08, 0x00, DEF_STR(Unused))	PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x08, DEF_STR(On))

	PORT_START("DSW_B")
	PORT_DIPNAME(0x01, 0x00, "General Test")	PORT_DIPLOCATION("SW1:5")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x01, DEF_STR(On))
	PORT_DIPNAME(0x02, 0x02, "Reels Test")		PORT_DIPLOCATION("SW1:6")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x02, DEF_STR(On))
	PORT_DIPNAME(0x04, 0x00, "Timing Test")		PORT_DIPLOCATION("SW1:7")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x04, DEF_STR(On))
	PORT_DIPNAME(0x08, 0x00, DEF_STR(Unused))	PORT_DIPLOCATION("SW1:8")
	PORT_DIPSETTING(0x00, DEF_STR(Off))
	PORT_DIPSETTING(0x08, DEF_STR(On))
INPUT_PORTS_END


/*********************************************
*              Machine Drivers               *
*********************************************/

void interflip8035_state::interflip(machine_config &config)
{
	// basic machine hardware
	I8035(config, m_maincpu, MAIN_CLOCK);  // 6 MHz.
	m_maincpu->set_addrmap(AS_PROGRAM, &interflip8035_state::main_program_map);
	m_maincpu->set_addrmap(AS_IO , &interflip8035_state::main_io_map);
	m_maincpu->p1_out_cb().set(FUNC(interflip8035_state::main_p1_data_w));
	m_maincpu->p2_in_cb().set(FUNC(interflip8035_state::main_p2_r));
	m_maincpu->p2_out_cb().set(FUNC(interflip8035_state::main_p2_w));
	m_maincpu->prog_out_cb().set(m_ioexp[0], FUNC(i8243_device::prog_w));
	m_maincpu->prog_out_cb().append(m_ioexp[1], FUNC(i8243_device::prog_w));
	m_maincpu->prog_out_cb().append(m_ioexp[2], FUNC(i8243_device::prog_w));

	I8035(config, m_audiocpu, SND_CLOCK);  // 4 MHz.
	m_audiocpu->set_addrmap(AS_PROGRAM, &interflip8035_state::audio_program_map);
	m_audiocpu->set_addrmap(AS_IO,      &interflip8035_state::audio_io_map);
	m_audiocpu->p1_out_cb().set("dac", FUNC(dac_byte_interface::data_w));
	m_audiocpu->p2_in_cb().set(FUNC(interflip8035_state::audio_p2_r));
	m_audiocpu->p2_out_cb().set(FUNC(interflip8035_state::audio_p2_w));

	I8243(config, m_ioexp[0]);  // PIA 1: Optos (reels) & Dip Switches
	m_ioexp[0]->p4_in_cb().set_ioport("OPTOS_A");
	m_ioexp[0]->p5_in_cb().set_ioport("OPTOS_B");
	m_ioexp[0]->p6_in_cb().set_ioport("DSW_B");
	m_ioexp[0]->p7_in_cb().set_ioport("DSW_A");

	I8243(config, m_ioexp[1]);  // PIA 2: All Activation Coils, EM Counters, Relays to motors.
	m_ioexp[1]->p4_out_cb().set(FUNC(interflip8035_state::exp2_p4_w));
	m_ioexp[1]->p5_out_cb().set(FUNC(interflip8035_state::exp2_p5_w));
	m_ioexp[1]->p6_out_cb().set(FUNC(interflip8035_state::exp2_p6_w));
	m_ioexp[1]->p7_out_cb().set(FUNC(interflip8035_state::exp2_p7_w));

	I8243(config, m_ioexp[2]);  // PIA 3: Other Lamps, Hopper load switch, Sound control.
	m_ioexp[2]->p4_out_cb().set(FUNC(interflip8035_state::exp3_p4_w));
	m_ioexp[2]->p5_in_cb().set_ioport("IN1");
	m_ioexp[2]->p6_out_cb().set(FUNC(interflip8035_state::exp3_p6_w));

	I8279(config, m_kbdc, MAIN_CLOCK / 3);  // 2 MHz. (Derived from Main CPU that gives Main Clock / 3  frequency.
	m_kbdc->out_sl_callback().set(FUNC(interflip8035_state::kbd_sl_w));  // scan SL lines
	m_kbdc->out_disp_callback().set(FUNC(interflip8035_state::disp_w));  // display A&B
	m_kbdc->in_rl_callback().set(FUNC(interflip8035_state::kbd_rl_r));   // kbd RL lines
	m_kbdc->out_irq_callback().set(FUNC(interflip8035_state::irq_w));
	m_kbdc->in_shift_callback().set_constant(0);
	m_kbdc->in_ctrl_callback().set_constant(0);

	NVRAM(config, "data_ram", nvram_device::DEFAULT_ALL_0);

	// electromechanics
	add_em_reels(config, 20, attotime::from_double(2));

	// video layout
	config.set_default_layout(layout_ifslots);

	// sound stuff
	SPEAKER(config, "mono").front_center();

	MC1408(config, "dac", 0).add_route(ALL_OUTPUTS, "mono", 2.0);

	SAMPLES(config, m_samples);
	m_samples->set_channels(1);
	m_samples->set_samples_names(interflip8035_sample_names);
	m_samples->add_route(ALL_OUTPUTS, "mono", 2.0);

}


/*********************************************
*                  Rom Load                  *
*********************************************/

ROM_START( cbrava )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "cbr2p81.pal", 0x0000, 0x1000, CRC(89209629) SHA1(8f2e6acfcb3f9d3663a40b6714bc6c784a2af8db) )

	ROM_REGION( 0x800, "audiocpu", 0 ) // same as sevilla
	ROM_LOAD( "sonsev.pal", 0x000, 0x800, CRC(1043a346) SHA1(3d45e3795653a51dca7992848eb4b9ed66492b0c) )
ROM_END

ROM_START( sevilla )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "sev2p81.pal", 0x0000, 0x1000, CRC(362acdf4) SHA1(82913fe5c646be9c10252c2337ceaac2fc8173df) )

	ROM_REGION( 0x800, "audiocpu", 0 ) // same as cbrava
	ROM_LOAD( "sonsev.pal", 0x000, 0x800, CRC(1043a346) SHA1(3d45e3795653a51dca7992848eb4b9ed66492b0c) )
ROM_END

ROM_START( toledo )
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "told2p87.pal", 0x0000, 0x1000, CRC(9990f5ed) SHA1(b556eb3c9ebec7b974a19ec077e81ef0429ccfe0) )

	ROM_REGION( 0x800, "audiocpu", 0 )
	ROM_LOAD( "sontol.pal", 0x000, 0x800, CRC(5066dc8c) SHA1(9bb81671525c645a633db2b8f6aed0dfe198fe63) )
ROM_END

ROM_START( jackuse )  // jackpot settings...
	ROM_REGION( 0x1000, "maincpu", 0 )
	ROM_LOAD( "jackuse.pal", 0x0000, 0x1000, CRC(6adc3fcf) SHA1(cb63a0dcf9accf283a9aeddb2e9e120c19483b13) )

	ROM_REGION( 0x800, "audiocpu", 0 )
	ROM_LOAD( "sontol.pal", 0x000, 0x800, CRC(5066dc8c) SHA1(9bb81671525c645a633db2b8f6aed0dfe198fe63) )
ROM_END


} // anonymous namespace


/*********************************************
*                Game Drivers                *
*********************************************/

//    YEAR  NAME     PARENT  MACHINE    INPUT      STATE                INIT        ROT    COMPANY      FULLNAME      FLAGS
GAME( 1982, cbrava,  0,      interflip, interflip, interflip8035_state, empty_init, ROT0, "Interflip", "Costa Brava", MACHINE_NOT_WORKING | MACHINE_MECHANICAL )
GAME( 1982, sevilla, 0,      interflip, interflip, interflip8035_state, empty_init, ROT0, "Interflip", "Sevilla",     MACHINE_NOT_WORKING | MACHINE_MECHANICAL )
GAME( 1982, toledo,  0,      interflip, interflip, interflip8035_state, empty_init, ROT0, "Interflip", "Toledo",      MACHINE_NOT_WORKING | MACHINE_MECHANICAL )
GAME( 1982, jackuse, 0,      interflip, interflip, interflip8035_state, empty_init, ROT0, "Interflip", "Jack Use (Jackpot settings for Interflip slots machines)",  MACHINE_NOT_WORKING | MACHINE_MECHANICAL )
