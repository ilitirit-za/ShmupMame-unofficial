/***************************************************************************

    profiler.c

    Functions to manage profiling of MAME execution.

****************************************************************************

    Copyright Aaron Giles
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in
          the documentation and/or other materials provided with the
          distribution.
        * Neither the name 'MAME' nor the names of its contributors may be
          used to endorse or promote products derived from this software
          without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY AARON GILES ''AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL AARON GILES BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

****************************************************************************

    Profiling is scope-based. To start profiling, put a profiler_scope
    object on the stack. To end profiling, just end the scope:

    {
        profiler_scope scope(PROFILER_VIDEO);

        your_work_here();
    }

    the profiler handles a FILO list so calls may be nested.

***************************************************************************/

#include "emu.h"
#include "profiler.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

struct profile_string
{
	int         type;
	const char *string;
};



//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

profiler_state g_profiler;



//**************************************************************************
//  CONSTANTS
//**************************************************************************

#define TEXT_UPDATE_TIME        0.5



//**************************************************************************
//  DUMMY PROFILER STATE
//**************************************************************************

//-------------------------------------------------
//  dummy_profiler_state - constructor
//-------------------------------------------------

dummy_profiler_state::dummy_profiler_state()
{
}



//**************************************************************************
//  REAL PROFILER STATE
//**************************************************************************

//-------------------------------------------------
//  real_profiler_state - constructor
//-------------------------------------------------

real_profiler_state::real_profiler_state()
{
	memset(m_filo, 0, sizeof(m_filo));
	memset(m_data, 0, sizeof(m_data));
	reset(false);
}



//-------------------------------------------------
//  reset - initializes state
//-------------------------------------------------

void real_profiler_state::reset(bool enabled)
{
	m_text_time = attotime::never;

	if (enabled)
	{
		// we're enabled now
		m_filoptr = m_filo;

		// set up dummy entry
		m_filoptr->start = 0;
		m_filoptr->type = PROFILER_TOTAL;
	}
	else
	{
		// magic value to indicate disabled
		m_filoptr = NULL;
	}
}



//-------------------------------------------------
//  text - return the current text in an astring
//-------------------------------------------------

const char *real_profiler_state::text(running_machine &machine)
{
	start(PROFILER_PROFILER);

	// get the current time
	attotime current_time = machine.scheduler().time();

	// we only want to update the text periodically
	if ((m_text_time == attotime::never) || ((current_time - m_text_time).as_double() >= TEXT_UPDATE_TIME))
	{
		update_text(machine);
		m_text_time = current_time;
	}

	stop();
	return m_text;
}



//-------------------------------------------------
//  update_text - update the current astring
//-------------------------------------------------

void real_profiler_state::update_text(running_machine &machine)
{
	static const profile_string names[] =
	{
		{ PROFILER_DRC_COMPILE,      "DRC Compilation" },
		{ PROFILER_MEM_REMAP,        "Memory Remapping" },
		{ PROFILER_MEMREAD,          "Memory Read" },
		{ PROFILER_MEMWRITE,         "Memory Write" },
		{ PROFILER_VIDEO,            "Video Update" },
		{ PROFILER_DRAWGFX,          "drawgfx" },
		{ PROFILER_COPYBITMAP,       "copybitmap" },
		{ PROFILER_TILEMAP_DRAW,     "Tilemap Draw" },
		{ PROFILER_TILEMAP_DRAW_ROZ, "Tilemap ROZ Draw" },
		{ PROFILER_TILEMAP_UPDATE,   "Tilemap Update" },
		{ PROFILER_BLIT,             "OSD Blitting" },
		{ PROFILER_SOUND,            "Sound Generation" },
		{ PROFILER_TIMER_CALLBACK,   "Timer Callbacks" },
#ifdef USE_HISCORE
		//MKCHAMP - INCLUDING THE HISCORE ENGINE TO THE PROFILER
		{ PROFILER_HISCORE,          "Hiscore" },
#endif /* USE_HISCORE */
		{ PROFILER_INPUT,            "Input Processing" },
		{ PROFILER_MOVIE_REC,        "Movie Recording" },
		{ PROFILER_LOGERROR,         "Error Logging" },
		{ PROFILER_EXTRA,            "Unaccounted/Overhead" },
		{ PROFILER_USER1,            "User 1" },
		{ PROFILER_USER2,            "User 2" },
		{ PROFILER_USER3,            "User 3" },
		{ PROFILER_USER4,            "User 4" },
		{ PROFILER_USER5,            "User 5" },
		{ PROFILER_USER6,            "User 6" },
		{ PROFILER_USER7,            "User 7" },
		{ PROFILER_USER8,            "User 8" },
		{ PROFILER_PROFILER,         "Profiler" },
		{ PROFILER_IDLE,             "Idle" }
	};

	// compute the total time for all bits, not including profiler or idle
	UINT64 computed = 0;
	profile_type curtype;
	for (curtype = PROFILER_DEVICE_FIRST; curtype < PROFILER_PROFILER; curtype++)
		computed += m_data[curtype];

	// save that result in normalize, and continue adding the rest
	UINT64 normalize = computed;
	for ( ; curtype < PROFILER_TOTAL; curtype++)
		computed += m_data[curtype];

	// this becomes the total; if we end up with 0 for anything, we were just started, so return empty
	UINT64 total = computed;
	m_text.reset();
	if (total == 0 || normalize == 0)
	{
		return;
	}

	// loop over all types and generate the string
	device_iterator iter(machine.root_device());
	for (curtype = PROFILER_DEVICE_FIRST; curtype < PROFILER_TOTAL; curtype++)
	{
		// determine the accumulated time for this type
		computed = m_data[curtype];

		// if we have non-zero data and we're ready to display, do it
		if (computed != 0)
		{
			// start with the un-normalized percentage
			m_text.catprintf("%02d%% ", (int)((computed * 100 + total/2) / total));

			// followed by the normalized percentage for everything but profiler and idle
			if (curtype < PROFILER_PROFILER)
				m_text.catprintf("%02d%% ", (int)((computed * 100 + normalize/2) / normalize));

			// and then the text
			if (curtype >= PROFILER_DEVICE_FIRST && curtype <= PROFILER_DEVICE_MAX)
				m_text.catprintf("'%s'", iter.byindex(curtype - PROFILER_DEVICE_FIRST)->tag());
			else
				for (int nameindex = 0; nameindex < ARRAY_LENGTH(names); nameindex++)
					if (names[nameindex].type == curtype)
					{
						m_text.cat(names[nameindex].string);
						break;
					}

			// followed by a carriage return
			m_text.cat("\n");
		}
	}

	// reset data set to 0
	memset(m_data, 0, sizeof(m_data));
}
