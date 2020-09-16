// license:GPL-2.0+
// copyright-holders:Ron Fries,Dan Boris
#ifndef MAME_SOUND_TIAINTF_H
#define MAME_SOUND_TIAINTF_H

#pragma once

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> tia_device

class tia_device : public device_t, public device_sound_interface
{
public:
	tia_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	void tia_sound_w(offs_t offset, uint8_t data);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_stop() override;

	// sound stream update overrides
	virtual void sound_stream_update_legacy(sound_stream &stream, stream_sample_t const * const *inputs, stream_sample_t * const *outputs, int samples) override;

private:
	sound_stream *m_channel;
	void *m_chip;
};

DECLARE_DEVICE_TYPE(TIA, tia_device)

#endif // MAME_SOUND_TIAINTF_H
