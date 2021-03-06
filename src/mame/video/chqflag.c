/***************************************************************************

  video.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "emu.h"
#include "video/konicdev.h"
#include "includes/chqflag.h"


/***************************************************************************

  Callbacks for the K051960

***************************************************************************/

void chqflag_sprite_callback( running_machine &machine, int *code, int *color, int *priority, int *shadow )
{
	chqflag_state *state = machine.driver_data<chqflag_state>();
	*priority = (*color & 0x10) >> 4;
	*color = state->m_sprite_colorbase + (*color & 0x0f);
}


/***************************************************************************

  Callbacks for the K051316

***************************************************************************/

void chqflag_zoom_callback_0( running_machine &machine, int *code, int *color, int *flags )
{
	chqflag_state *state = machine.driver_data<chqflag_state>();
	*code |= ((*color & 0x03) << 8);
	*color = state->m_zoom_colorbase[0] + ((*color & 0x3c) >> 2);
}

void chqflag_zoom_callback_1( running_machine &machine, int *code, int *color, int *flags )
{
	chqflag_state *state = machine.driver_data<chqflag_state>();
	*flags = TILE_FLIPYX((*color & 0xc0) >> 6);
	*code |= ((*color & 0x0f) << 8);
	*color = state->m_zoom_colorbase[1] + ((*color & 0x10) >> 4);
}

/***************************************************************************

    Start the video hardware emulation.

***************************************************************************/

void chqflag_state::video_start()
{
	m_generic_paletteram_8.allocate(0x800);

	m_sprite_colorbase = 0;
	m_zoom_colorbase[0] = 0x10;
	m_zoom_colorbase[1] = 0x02;
}

/***************************************************************************

    Display Refresh

***************************************************************************/

UINT32 chqflag_state::screen_update_chqflag(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bitmap.fill(0, cliprect);

	k051316_zoom_draw(m_k051316_2, bitmap, cliprect, TILEMAP_DRAW_LAYER1, 0);
	k051960_sprites_draw(m_k051960, bitmap, cliprect, 0, 0);
	k051316_zoom_draw(m_k051316_2, bitmap, cliprect, TILEMAP_DRAW_LAYER0, 0);
	k051960_sprites_draw(m_k051960, bitmap, cliprect, 1, 1);
	k051316_zoom_draw(m_k051316_1, bitmap, cliprect, 0, 0);
	return 0;
}
