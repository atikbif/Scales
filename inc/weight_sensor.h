#ifndef WEIGHT_SENSOR_H_INCLUDED
#define WEIGHT_SENSOR_H_INCLUDED

void weight_sensor_init(void);
unsigned char isWeightSensorReady(unsigned char canNum);
void getWeight(unsigned long* w1, unsigned long* w2);

#endif /* WEIGHT_SENSOR_H_INCLUDED */
