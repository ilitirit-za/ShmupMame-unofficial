/***************************************************************************

    ef9340_1.h

    Thomson EF9340 + EF9341 teletext graphics chips with 1KB external
    character ram.

***************************************************************************/

#pragma once

#ifndef __EF9340_1_H__
#define __EF9340_1_H__

#include "emu.h"


#define MCFG_EF9340_1_ADD(_tag, _clock, _screen_tag) \
	MCFG_DEVICE_ADD(_tag, EF9340_1, _clock) \
	ef9340_1_device::set_screen_tag(*device, _screen_tag); \


class ef9340_1_device : public device_t
{
public:
    // construction/destruction
    ef9340_1_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	// static configuration helpers
	static void set_screen_tag(device_t &device, const char *screen_tag) { downcast<ef9340_1_device &>(device).m_screen_tag = screen_tag; }

	inline bitmap_ind16 *get_bitmap() { return &m_tmp_bitmap; }

	void ef9341_write( UINT8 command, UINT8 b, UINT8 data );
	UINT8 ef9341_read( UINT8 command, UINT8 b );

protected:
	// device-level overrides
	virtual void device_start();
	virtual void device_reset();
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr);

	inline UINT16 ef9340_get_c_addr(UINT8 x, UINT8 y);
	inline void ef9340_inc_c();

	// Calculate the external chargen address for a character and slice
	inline UINT16 external_chargen_address(UINT8 b, UINT8 slice);

	void ef9340_scanline(int vpos);

	/* timers */
	static const device_timer_id TIMER_LINE = 0;

	emu_timer *m_line_timer;

	const char *m_screen_tag;
	screen_device *m_screen;

	bitmap_ind16 m_tmp_bitmap;

	struct
	{
		UINT8   TA;
		UINT8   TB;
		UINT8   busy;
	} m_ef9341;
	struct
	{
		UINT8   X;
		UINT8   Y;
		UINT8   Y0;
		UINT8   R;
		UINT8   M;
	} m_ef9340;
	UINT8   m_ef934x_ram_a[1024];
	UINT8   m_ef934x_ram_b[1024];
	UINT8   m_ef934x_ext_char_ram[1024];
};


// device type definition
extern const device_type EF9340_1;

#endif  /* __EF9340_1_H__ */

