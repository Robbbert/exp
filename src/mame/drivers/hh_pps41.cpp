// license:BSD-3-Clause
// copyright-holders:hap
// thanks-to:Sean Riddle, Kevin Horton
/***************************************************************************

  Rockwell PPS-4/1 MCU series handhelds

***************************************************************************/

#include "emu.h"

#include "cpu/pps41/mm75.h"
#include "video/pwm.h"
#include "sound/spkrdev.h"
#include "speaker.h"

// internal artwork
#include "mastmind.lh"
#include "memoquiz.lh"

//#include "hh_pps41_test.lh" // common test-layout - use external artwork


class hh_pps41_state : public driver_device
{
public:
	hh_pps41_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_display(*this, "display"),
		m_speaker(*this, "speaker"),
		m_inputs(*this, "IN.%u", 0)
	{ }

	// devices
	required_device<pps41_base_device> m_maincpu;
	optional_device<pwm_display_device> m_display;
	optional_device<speaker_sound_device> m_speaker;
	optional_ioport_array<5> m_inputs; // max 5

	u16 m_inp_mux = 0;

	// MCU output pin state
	u16 m_d = 0;
	u8 m_r = ~0;

	u8 read_inputs(int columns);

protected:
	virtual void machine_start() override;
	virtual void machine_reset() override;
};


// machine start/reset

void hh_pps41_state::machine_start()
{
	// register for savestates
	save_item(NAME(m_inp_mux));
	save_item(NAME(m_d));
	save_item(NAME(m_r));
}

void hh_pps41_state::machine_reset()
{
}



/***************************************************************************

  Helper Functions

***************************************************************************/

// generic input handlers

u8 hh_pps41_state::read_inputs(int columns)
{
	u8 ret = 0;

	// read selected input rows
	for (int i = 0; i < columns; i++)
		if (m_inp_mux >> i & 1)
			ret |= m_inputs[i]->read();

	return ret;
}



/***************************************************************************

  Minidrivers (subclass, I/O, Inputs, Machine Config, ROM Defs)

***************************************************************************/

namespace {

/***************************************************************************

  Invicta Electronic Master Mind
  * MM75 MCU (label MM75 A7525-11, die label A7525)
  * 9-digit 7seg VFD display (Futaba 9-ST)

  Invicta is the owner of the Mastermind game rights. The back of the unit
  says (C) 1977, but this electronic handheld version came out in 1979.
  Or maybe there's an older revision.

***************************************************************************/

class mastmind_state : public hh_pps41_state
{
public:
	mastmind_state(const machine_config &mconfig, device_type type, const char *tag) :
		hh_pps41_state(mconfig, type, tag)
	{ }

	void update_display();
	void write_d(u16 data);
	void write_r(u8 data);
	u8 read_p();
	void mastmind(machine_config &config);
};

// handlers

void mastmind_state::update_display()
{
	m_display->matrix(m_inp_mux, ~m_r);
}

void mastmind_state::write_d(u16 data)
{
	// DIO0-DIO7: digit select (DIO7 N/C on mastmind)
	// DIO0-DIO3: input mux
	m_inp_mux = data;
	update_display();
}

void mastmind_state::write_r(u8 data)
{
	// RIO1-RIO7: digit segment data
	m_r = data;
	update_display();
}

u8 mastmind_state::read_p()
{
	// PI1-PI4: multiplexed inputs
	return ~read_inputs(4);
}

// config

static INPUT_PORTS_START( mastmind )
	PORT_START("IN.0") // DIO0
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_ENTER) PORT_CODE(KEYCODE_ENTER_PAD) PORT_NAME("Try")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_F) PORT_NAME("Fail")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_UNUSED ) // display test?
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("IN.1") // DIO1
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_8) PORT_CODE(KEYCODE_8_PAD) PORT_NAME("8")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_9) PORT_CODE(KEYCODE_9_PAD) PORT_NAME("9")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_S) PORT_NAME("Set")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_DEL) PORT_CODE(KEYCODE_BACKSPACE) PORT_NAME("Clear")

	PORT_START("IN.2") // DIO2
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_4) PORT_CODE(KEYCODE_4_PAD) PORT_NAME("4")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_5) PORT_CODE(KEYCODE_5_PAD) PORT_NAME("5")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_6) PORT_CODE(KEYCODE_6_PAD) PORT_NAME("6")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_7) PORT_CODE(KEYCODE_7_PAD) PORT_NAME("7")

	PORT_START("IN.3") // DIO3
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_0) PORT_CODE(KEYCODE_0_PAD) PORT_NAME("0")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_1) PORT_CODE(KEYCODE_1_PAD) PORT_NAME("1")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_2) PORT_CODE(KEYCODE_2_PAD) PORT_NAME("2")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_3) PORT_CODE(KEYCODE_3_PAD) PORT_NAME("3")
INPUT_PORTS_END

void mastmind_state::mastmind(machine_config &config)
{
	/* basic machine hardware */
	MM75(config, m_maincpu, 100000); // approximation
	m_maincpu->write_d().set(FUNC(mastmind_state::write_d));
	m_maincpu->write_r().set(FUNC(mastmind_state::write_r));
	m_maincpu->read_p().set(FUNC(mastmind_state::read_p));

	/* video hardware */
	PWM_DISPLAY(config, m_display).set_size(8, 7);
	m_display->set_segmask(0xff, 0x7f);
	config.set_default_layout(layout_mastmind);

	/* no sound! */
}

// roms

ROM_START( mastmind )
	ROM_REGION( 0x0400, "maincpu", ROMREGION_ERASE00 )
	ROM_LOAD( "mm75_a7525-11", 0x0000, 0x0200, CRC(39dbdd50) SHA1(72fa5781e9df62d91d57437ded2931fab8253c3c) )
	ROM_CONTINUE(              0x0380, 0x0080 )

	ROM_REGION( 314, "maincpu:opla", 0 )
	ROM_LOAD( "mm76_mastmind_output.pla", 0, 314, CRC(c936aee7) SHA1(e9ec08a82493d6b63e936f82deeab3e4449b54c3) )
ROM_END





/***************************************************************************

  M.E.M. Belgium Memoquiz
  * PCB label: MEMOQUIZ MO3
  * MM75 MCU (label M7505 A7505-12, die label A7505)
  * 9-digit 7seg VFD display, no sound

  It's a Mastermind game, not as straightforward as Invicta's version.
  To start, press the "?" button to generate a new code, then try to guess it,
  confirming with the "=" button. CD reveals the answer, PE is for player entry.

  known releases:
  - Europe: Memoquiz
  - UK: Memoquiz, published by Polymark
  - USA: Mind Boggler (model 2626), published by Mattel

***************************************************************************/

class memoquiz_state : public hh_pps41_state
{
public:
	memoquiz_state(const machine_config &mconfig, device_type type, const char *tag) :
		hh_pps41_state(mconfig, type, tag)
	{ }

	DECLARE_INPUT_CHANGED_MEMBER(digits_switch) { set_digits(); }
	void set_digits();

	void update_display();
	void write_d(u16 data);
	void write_r(u8 data);
	u8 read_p();
	void memoquiz(machine_config &config);

protected:
	virtual void machine_reset() override;
};

void memoquiz_state::machine_reset()
{
	hh_pps41_state::machine_reset();
	set_digits();
}

// handlers

void memoquiz_state::set_digits()
{
	// digits switch is tied to MCU interrupt pins
	u8 inp = m_inputs[4]->read();
	m_maincpu->set_input_line(0, (inp & 1) ? CLEAR_LINE : ASSERT_LINE);
	m_maincpu->set_input_line(1, (inp & 2) ? ASSERT_LINE : CLEAR_LINE);
}

void memoquiz_state::update_display()
{
	m_display->matrix(m_inp_mux, (m_inp_mux << 2 & 0x80) | (~m_r & 0x7f));
}

void memoquiz_state::write_d(u16 data)
{
	// DIO0-DIO7: digit select, DIO5 is also DP segment
	// DIO0-DIO3: input mux
	m_inp_mux = data;
	update_display();

	// DIO08: N/C, looks like they planned to add sound, but didn't
}

void memoquiz_state::write_r(u8 data)
{
	// RIO1-RIO7: digit segment data
	m_r = data;
	update_display();
}

u8 memoquiz_state::read_p()
{
	// PI1-PI4: multiplexed inputs
	return ~read_inputs(4);
}

// config

static INPUT_PORTS_START( memoquiz )
	PORT_START("IN.0") // DIO0
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_3) PORT_CODE(KEYCODE_3_PAD) PORT_NAME("3")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_2) PORT_CODE(KEYCODE_2_PAD) PORT_NAME("2")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_1) PORT_CODE(KEYCODE_1_PAD) PORT_NAME("1")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_0) PORT_CODE(KEYCODE_0_PAD) PORT_NAME("0")

	PORT_START("IN.1") // DIO1
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_7) PORT_CODE(KEYCODE_7_PAD) PORT_NAME("7")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_6) PORT_CODE(KEYCODE_6_PAD) PORT_NAME("6")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_5) PORT_CODE(KEYCODE_5_PAD) PORT_NAME("5")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_4) PORT_CODE(KEYCODE_4_PAD) PORT_NAME("4")

	PORT_START("IN.2") // DIO2
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_A) PORT_NAME("AC")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_DEL) PORT_CODE(KEYCODE_BACKSPACE) PORT_NAME("CE")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_9) PORT_CODE(KEYCODE_9_PAD) PORT_NAME("9")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_8) PORT_CODE(KEYCODE_8_PAD) PORT_NAME("8")

	PORT_START("IN.3") // DIO3
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_ENTER) PORT_CODE(KEYCODE_ENTER_PAD) PORT_NAME("=")
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_SLASH) PORT_NAME("?")
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_E) PORT_NAME("PE")
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_KEYPAD ) PORT_CODE(KEYCODE_C) PORT_NAME("CD")

	PORT_START("IN.4")
	PORT_CONFNAME( 0x03, 0x01, "Digits" ) PORT_CHANGED_MEMBER(DEVICE_SELF, memoquiz_state, digits_switch, 0)
	PORT_CONFSETTING(    0x01, "3" ) // INT0, Vdd when closed, pulled to GND when open
	PORT_CONFSETTING(    0x02, "4" ) // INT1, GND when closed, pulled to Vdd when open
	PORT_CONFSETTING(    0x00, "5" )
INPUT_PORTS_END

void memoquiz_state::memoquiz(machine_config &config)
{
	/* basic machine hardware */
	MM75(config, m_maincpu, 100000); // approximation
	m_maincpu->write_d().set(FUNC(memoquiz_state::write_d));
	m_maincpu->write_r().set(FUNC(memoquiz_state::write_r));
	m_maincpu->read_p().set(FUNC(memoquiz_state::read_p));

	/* video hardware */
	PWM_DISPLAY(config, m_display).set_size(8, 8);
	m_display->set_segmask(0xff, 0xff);
	config.set_default_layout(layout_memoquiz);

	/* no sound! */
}

// roms

ROM_START( memoquiz )
	ROM_REGION( 0x0400, "maincpu", ROMREGION_ERASE00 )
	ROM_LOAD( "m7505_a7505-12", 0x0000, 0x0200, CRC(47223508) SHA1(97b62e0c453ae2e65d48e039ad65857dae2d4d76) )
	ROM_CONTINUE(               0x0380, 0x0080 )

	ROM_REGION( 314, "maincpu:opla", 0 )
	ROM_LOAD( "mm76_memoquiz_output.pla", 0, 314, CRC(a5799b50) SHA1(9b4923b37c9ba8221ecece5a3370c605a880a453) )
ROM_END



} // anonymous namespace

/***************************************************************************

  Game driver(s)

***************************************************************************/

//    YEAR  NAME      PARENT  CMP MACHINE   INPUT     CLASS           INIT        COMPANY, FULLNAME, FLAGS
CONS( 1979, mastmind, 0,       0, mastmind, mastmind, mastmind_state, empty_init, "Invicta Plastics", "Electronic Master Mind (Invicta)", MACHINE_SUPPORTS_SAVE | MACHINE_NO_SOUND_HW )

CONS( 1978, memoquiz, 0,       0, memoquiz, memoquiz, memoquiz_state, empty_init, "M.E.M. Belgium", "Memoquiz", MACHINE_SUPPORTS_SAVE | MACHINE_NO_SOUND_HW )
