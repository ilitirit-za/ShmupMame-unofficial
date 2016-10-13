/***************************************************************************

  M.A.M.E.UI  -  Multiple Arcade Machine Emulator with User Interface
  Win32 Portions Copyright (C) 1997-2003 Michael Soderstrom and Chris Kirmse,
  Copyright (C) 2003-2007 Chris Kirmse and the MAME32/MAMEUI team.

  This file is part of MAMEUI, and may only be used, modified and
  distributed under the terms of the MAME license, in "readme.txt".
  By continuing to use, modify or distribute this file you indicate
  that you have read the license and understand and accept it fully.

 ***************************************************************************/

#pragma once

#ifndef DATAFILE_H
#define DATAFILE_H

class windows_options;

extern void winui_datafile_init(windows_options &options);
extern void winui_datafile_exit(void);

extern int load_driver_history(const game_driver *drv, char *buffer, int bufsize);
#ifdef STORY_DATAFILE
extern int load_driver_story(const game_driver *drv, char *buffer, int bufsize);
#endif /* STORY_DATAFILE */
extern int load_driver_mameinfo(const game_driver *drv, char *buffer, int bufsize);
extern int load_driver_drivinfo(const game_driver *drv, char *buffer, int bufsize);

#endif
