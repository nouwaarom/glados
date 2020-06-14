#ifndef PRAAT_FESTIFUCK_H
#define PRAAT_FESTIFUCK_H

// Because festival headers are a bit fucked, for example they add `using namespace std;` and just dont work unless you modify them we do festival stuff in this file.

void festifuck_init();

void festifuck(const char* text);

#endif   // PRAAT_FESTIFUCK_H
