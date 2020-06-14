#include "festifuck.h"

#include <festival/festival.h>
#include <estools/EST_Wave.h>

void festifuck_init() {
	festival_initialize (true, 10000000);
	festival_eval_command(EST_String("(voice_cmu_us_slt_arctic_hts)"));
}

void festifuck(const char* text) {
    EST_String filename = EST_String(text);

    EST_Wave wave;

    festival_text_to_wave(filename, wave);

    wave.save("/home/eput/Projects/Glados/festival.wav", "riff");
    festival_wait_for_spooler();
}
