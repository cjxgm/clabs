
#ifndef CAMERA_H
#define CAMERA_H

void camInit(void);
void camApply(void);
void camBegin(void);
void camEnd(void);
void camForward(float howmuch);
void camPan(float howmuch);
void camTurn(float howmuch);

#endif
