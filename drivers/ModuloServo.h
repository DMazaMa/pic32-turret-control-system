#ifndef MODULOSERVO_H
#define MODULOSERVO_H

void inicializacionServo(void);
void sumAngulo(int delta_grados);
void setGrados(int nuevo_grado);
int getGrados(void);
void delayServoPorAngulo(int delta);
int estadoBarrera();
void cerrarBarrera();
void activarBarrera();
void delayServoPorAngulo(int delta);
#endif
