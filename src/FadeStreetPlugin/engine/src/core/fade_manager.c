#pragma bank 255

#include <gbdk/platform.h>
#include <string.h>

#include "compat.h"
#include "system.h"
#include "fade_manager.h"
#include "palette.h"
#include "fade_street.h"

UBYTE fade_frames_per_step;
UBYTE fade_style = 0;

void fade_init(void) BANKED {
	// reset all palettes to white
	DMG_palette[0] = 0x0;
	DMG_palette[1] = 0x0;
	DMG_palette[2] = 0x0;
#ifdef CGB
	if (_is_CGB) {
		memset(SprPalette, 0xff, 64);
		memset(BkgPalette, 0xff, 64);
		refresh_bkg_palettes = 1;
		refresh_obj_palettes = 1;
	}
#endif
	wait_vbl_done();
}

void fade_in(void) BANKED {
}

void fade_out(void) BANKED {
}

void fade_update(void) BANKED {
}

void fade_applypalettechange(void) BANKED {
}

void fade_setspeed(UBYTE speed) BANKED {
	speed;
}

void fade_in_modal(void) BANKED {
}

void fade_out_modal(void) BANKED {
}
