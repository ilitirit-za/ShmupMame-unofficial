
#include "sound/msm5232.h"
#include "sound/dac.h"

#define POPDRUMKIT 0


class equites_state : public driver_device
{
public:
	equites_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) ,
		m_bg_videoram(*this, "bg_videoram"),
		m_spriteram(*this, "spriteram"),
		m_workram(*this, "workram"),
		m_spriteram_2(*this, "spriteram_2"),
		m_mcu_ram(*this, "mcu_ram"){ }

	/* memory pointers */
	required_shared_ptr<UINT16> m_bg_videoram;
	UINT8  *  m_fg_videoram;    // 8bits
	required_shared_ptr<UINT16> m_spriteram;
	optional_shared_ptr<UINT16> m_workram;
	optional_shared_ptr<UINT16> m_spriteram_2;
	required_shared_ptr<UINT8>  m_mcu_ram;      // 8bits
//  UINT16 *  m_nvram;    // currently this uses generic nvram handling

	/* video-related */
	tilemap_t *m_fg_tilemap;
	tilemap_t *m_bg_tilemap;
	int       m_fg_char_bank;
	UINT8     m_bgcolor;
	UINT16    m_splndrbt_bg_scrollx;
	UINT16    m_splndrbt_bg_scrolly;

	/* misc */
	int       m_sound_prom_address;
	UINT8     m_dac_latch;
	UINT8     m_eq8155_port_b;
	UINT8     m_eq8155_port_a;
	UINT8     m_eq8155_port_c;
	UINT8     m_ay_port_a;
	UINT8     m_ay_port_b;
	UINT8     m_eq_cymbal_ctrl;
	emu_timer *m_nmi_timer;
	emu_timer *m_adjuster_timer;
	float     m_cymvol;
	float     m_hihatvol;
	int       m_timer_count;
	int       m_unknown_bit;    // Gekisou special handling
#if POPDRUMKIT
	int       m_hihat;
	int       m_cymbal;
#endif

	/* devices */
	device_t *m_mcu;
	device_t *m_audio_cpu;
	msm5232_device *m_msm;
	dac_device *m_dac_1;
	dac_device *m_dac_2;
	DECLARE_WRITE8_MEMBER(equites_c0f8_w);
	DECLARE_WRITE8_MEMBER(equites_cymbal_ctrl_w);
	DECLARE_WRITE8_MEMBER(equites_dac_latch_w);
	DECLARE_WRITE8_MEMBER(equites_8155_portb_w);
	DECLARE_WRITE8_MEMBER(equites_8155_w);
	DECLARE_READ16_MEMBER(hvoltage_debug_r);
	DECLARE_WRITE16_MEMBER(gekisou_unknown_0_w);
	DECLARE_WRITE16_MEMBER(gekisou_unknown_1_w);
	DECLARE_READ16_MEMBER(equites_spriteram_kludge_r);
	DECLARE_READ16_MEMBER(mcu_r);
	DECLARE_WRITE16_MEMBER(mcu_w);
	DECLARE_WRITE16_MEMBER(mcu_halt_assert_w);
	DECLARE_WRITE16_MEMBER(mcu_halt_clear_w);
	DECLARE_READ16_MEMBER(equites_fg_videoram_r);
	DECLARE_WRITE16_MEMBER(equites_fg_videoram_w);
	DECLARE_WRITE16_MEMBER(equites_bg_videoram_w);
	DECLARE_WRITE16_MEMBER(equites_bgcolor_w);
	DECLARE_WRITE16_MEMBER(equites_scrollreg_w);
	DECLARE_WRITE16_MEMBER(splndrbt_selchar0_w);
	DECLARE_WRITE16_MEMBER(splndrbt_selchar1_w);
	DECLARE_WRITE16_MEMBER(equites_flip0_w);
	DECLARE_WRITE16_MEMBER(equites_flip1_w);
	DECLARE_WRITE16_MEMBER(splndrbt_flip0_w);
	DECLARE_WRITE16_MEMBER(splndrbt_flip1_w);
	DECLARE_WRITE16_MEMBER(splndrbt_bg_scrollx_w);
	DECLARE_WRITE16_MEMBER(splndrbt_bg_scrolly_w);
	DECLARE_CUSTOM_INPUT_MEMBER(gekisou_unknown_status);
	DECLARE_WRITE8_MEMBER(equites_8910porta_w);
	DECLARE_WRITE8_MEMBER(equites_8910portb_w);
	DECLARE_DRIVER_INIT(bullfgtr);
	DECLARE_DRIVER_INIT(kouyakyu);
	DECLARE_DRIVER_INIT(hvoltage);
	DECLARE_DRIVER_INIT(gekisou);
	DECLARE_DRIVER_INIT(splndrbt);
	DECLARE_DRIVER_INIT(equites);
	TILE_GET_INFO_MEMBER(equites_fg_info);
	TILE_GET_INFO_MEMBER(splndrbt_fg_info);
	TILE_GET_INFO_MEMBER(equites_bg_info);
	TILE_GET_INFO_MEMBER(splndrbt_bg_info);
	DECLARE_MACHINE_START(equites);
	DECLARE_MACHINE_RESET(equites);
	DECLARE_VIDEO_START(equites);
	DECLARE_PALETTE_INIT(equites);
	DECLARE_VIDEO_START(splndrbt);
	DECLARE_PALETTE_INIT(splndrbt);
	UINT32 screen_update_equites(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	UINT32 screen_update_splndrbt(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	TIMER_CALLBACK_MEMBER(equites_nmi_callback);
	TIMER_CALLBACK_MEMBER(equites_frq_adjuster_callback);
	TIMER_DEVICE_CALLBACK_MEMBER(equites_scanline);
	TIMER_DEVICE_CALLBACK_MEMBER(splndrbt_scanline);
	DECLARE_WRITE_LINE_MEMBER(equites_msm5232_gate);
};
