// license:BSD-3-Clause
// copyright-holders:Nathan Woods
#ifndef MAME_SOUND_WAVE_H
#define MAME_SOUND_WAVE_H

#pragma once

#include "imagedev/cassette.h"


/*****************************************************************************
 *  CassetteWave interface
 *****************************************************************************/

class wave_device : public device_t, public device_sound_interface
{
public:
	template <typename T>
	wave_device(const machine_config &mconfig, const char *tag, device_t *owner, T &&cassette_tag)
		: wave_device(mconfig, tag, owner, uint32_t(0))
	{
		m_cass.set_tag(std::forward<T>(cassette_tag));
	}
	wave_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

	template <typename T> void set_cassette_tag(T &&cassette_tag) { m_cass.set_tag(std::forward<T>(cassette_tag)); }

protected:
	// device-level overrides
	virtual void device_start() override;

	// sound stream update overrides
	virtual void sound_stream_update_legacy(sound_stream &stream, stream_sample_t const * const *inputs, stream_sample_t * const *outputs, int samples) override;

private:
	required_device<cassette_image_device> m_cass;
};

DECLARE_DEVICE_TYPE(WAVE, wave_device)

#endif // MAME_SOUND_WAVE_H
