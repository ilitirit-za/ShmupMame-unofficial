
class lsasquad_state : public driver_device
{
public:
	lsasquad_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) ,
		m_videoram(*this, "videoram"),
		m_scrollram(*this, "scrollram"),
		m_spriteram(*this, "spriteram"){ }

	/* memory pointers */
	required_shared_ptr<UINT8> m_videoram;
	required_shared_ptr<UINT8> m_scrollram;
	required_shared_ptr<UINT8> m_spriteram;

	/* misc */
	int m_sound_pending;
	int m_sound_nmi_enable;
	int m_pending_nmi;
	int m_sound_cmd;
	int m_sound_result;

	/* mcu */
	UINT8 m_from_main;
	UINT8 m_from_mcu;
	int m_mcu_sent;
	int m_main_sent;
	UINT8 m_port_a_in;
	UINT8 m_port_a_out;
	UINT8 m_ddr_a;
	UINT8 m_port_b_in;
	UINT8 m_port_b_out;
	UINT8 m_ddr_b;

	/* devices */
	cpu_device *m_maincpu;
	cpu_device *m_audiocpu;
	device_t *m_mcu;
	DECLARE_WRITE8_MEMBER(lsasquad_bankswitch_w);
	DECLARE_WRITE8_MEMBER(lsasquad_sh_nmi_disable_w);
	DECLARE_WRITE8_MEMBER(lsasquad_sh_nmi_enable_w);
	DECLARE_WRITE8_MEMBER(lsasquad_sound_command_w);
	DECLARE_READ8_MEMBER(lsasquad_sh_sound_command_r);
	DECLARE_WRITE8_MEMBER(lsasquad_sh_result_w);
	DECLARE_READ8_MEMBER(lsasquad_sound_result_r);
	DECLARE_READ8_MEMBER(lsasquad_sound_status_r);
	DECLARE_READ8_MEMBER(daikaiju_sh_sound_command_r);
	DECLARE_READ8_MEMBER(daikaiju_sound_status_r);
	DECLARE_READ8_MEMBER(lsasquad_68705_port_a_r);
	DECLARE_WRITE8_MEMBER(lsasquad_68705_port_a_w);
	DECLARE_WRITE8_MEMBER(lsasquad_68705_ddr_a_w);
	DECLARE_READ8_MEMBER(lsasquad_68705_port_b_r);
	DECLARE_WRITE8_MEMBER(lsasquad_68705_port_b_w);
	DECLARE_WRITE8_MEMBER(lsasquad_68705_ddr_b_w);
	DECLARE_WRITE8_MEMBER(lsasquad_mcu_w);
	DECLARE_READ8_MEMBER(lsasquad_mcu_r);
	DECLARE_READ8_MEMBER(lsasquad_mcu_status_r);
	DECLARE_READ8_MEMBER(daikaiju_mcu_status_r);
	DECLARE_WRITE8_MEMBER(unk);
	DECLARE_MACHINE_START(lsasquad);
	DECLARE_MACHINE_RESET(lsasquad);
	UINT32 screen_update_lsasquad(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	UINT32 screen_update_daikaiju(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	TIMER_CALLBACK_MEMBER(nmi_callback);
};
