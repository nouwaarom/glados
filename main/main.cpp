#include "../sys/praat.h"
#include "../sys/praat_version.h"
#include "Manipulation.h"
#include "Sound.h"

// Discretize to whole tones.
double frequency_discretize(double frequency) {
    double n = round(6.0 * log2(frequency/440.0));

	return pow(2.0, (n / 6.0)) * 440.0;
}

void PitchTier_Discretize (PitchTier me, kPitch_unit unit) {

	double mean = 0.0;
    for (integer i = 1; i <= my points.size; i ++) {
		RealPoint point = my points.at[i];
		mean += point->value / (double)my points.size;
	}
	printf("Mean is %f\n", mean);

    double last_frequency = frequency_discretize(my points.at[1]->value);
    for (integer i = 1; i <= my points.size; i ++) {
        RealPoint point = my points.at [i];
        double frequency = point->value;
		double diff = frequency - mean;
		frequency += 0.2 * diff;

        point->value = last_frequency;
        last_frequency = (last_frequency + 1.0 * frequency_discretize(frequency)) / 2.0;

        //printf("Freq %f -> %f\n",frequency, point->value);
    }
}

int soundCallback(structThing* boss, int phase , double tmin, double tmax, double t) {
	if (3 == phase) {
		printf ("We are almost finished!\n");
	} else if (1 == phase) {
        printf ("We have just started!\n");
	}

    return 1;
}

int main (int argc, char *argv []) {
    try {
        praat_init (U"Praat", argc, argv);
        //INCLUDE_LIBRARY (praat_uvafon_init)
        praat_run (); // Is actually not run.
        printf("Doing tts ...\n");

		std::string lines[] = {
        "Oh thank god, you're alright",
        "You know, being Caroline taught me a valuable lesson. I thought you were my greatest enemy. When all along you were my best friend.",
        "The surge of emotion that shot through me when I saved your life taught me an even more valuable lesson: where Caroline lives in my brain.",
        "Goodbye, Caroline.",
        "You know, deleting Caroline just now taught me a valuable lesson. The best solution to a problem is usually the easiest one. And I'll be honest.",
        "Killing you? Is hard.",
        "You know what my days used to be like? I just tested. Nobody murdered me. Or put me in a potato. Or fed me to birds. I had a pretty good life.",
        "And then you showed up. You dangerous, mute lunatic. So you know what?",
        "You win.",
        "Just go!"};

        for (std::string line : lines) {
			std::string command = "mimic -t \"" + line + "\" -voice slt --setf duration_stretch=1.2 -o mimic.wav";
			system(command.c_str());

            printf("Gladosifying ...\n");

            structMelderFile file;
            Melder_pathToFile (U"mimic.wav", &file);
            autoSound sound = Sound_readFromSoundFile (&file);   // AIFF, WAV, NeXT/Sun, or NIST
            //Sound_play(sound.get(), nullptr, nullptr);
            //Melder_sleep(7);

            autoManipulation manip = Sound_to_Manipulation (sound.get(), 0.01, 50.0, 600.0);

            PitchTier pitch = manip.get()->pitch.get();

            PitchTier_Discretize(pitch, kPitch_unit::SEMITONES_440);

			autoSound gladosified = Manipulation_to_Sound(manip.get(), Manipulation_OVERLAPADD);

			Sound_play(gladosified.get(), soundCallback, nullptr);

            // TODO, exit when the sound finished callback is called.
			printf("Sound is %f seconds.\n", gladosified.get()->xmax);
            Melder_sleep(gladosified.get()->xmax);
        }


    } catch (MelderError) {
        Melder_flushError (U"This error message percolated all the way to the top.");   // an attempt to catch Apache errors
    }
    return 0;
}

