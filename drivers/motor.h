#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void initMotor();            // Inicialización del motor
void controlMotor(uint16_t speed);  // Ajusta la velocidad del motor (0-255)

#endif
