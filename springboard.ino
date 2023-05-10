#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <LinkedList.h>

// var setup
LinkedList<uint16_t> distances = LinkedList<uint16_t>(); // 16bit unsigned Limit: 65535
LinkedList<unsigned long> times = LinkedList<unsigned long>();
unsigned sumOverX = 0; // alias times
unsigned sumOverY = 0; // alias distances
unsigned long startingTime;
const uint8_t normSpeed = 255;
const uint8_t noSpeed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Program started!");
}

unsigned long linear_regression(void){
  unsigned n = distances.size();
  uint16_t yMean = sumOverY / n;
  unsigned long xMean = sumOverX / n;

  long numerator, denominator;
  numerator = denominator = 0;
  unsigned long x_i;
  uint16_t y_i;

  for (unsigned i=0; i < n; i++) {
    y_i = distances.get(i);
    x_i = times.get(i);
    denominator += pow((x_i - xMean), 2);
    numerator += ((x_i - xMean)*(y_i - yMean));
  }

  long beta = numerator / denominator;
  long alpha = yMean - (beta * xMean);
  long targetMillis = (-alpha/beta);
  return (unsigned long)targetMillis;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t measuredD;
  unsigned long currentTime;
  startingTime = millis();
  // motor -> normSpeed

  do {
    delay(1000);
    //measure D
    currentTime = millis();
    distances.add(measuredD);
    times.add(currentTime - startingTime);
    sumOverY += measuredD;
    sumOverX += (currentTime - startingTime);
    Serial.print("Measured distance: ");
    Serial.print(measuredD);
    Serial.println(); // just a line break
  }
  while (measuredD > 60);
  Serial.println("60cm reached. Starting linear regression, stopping motor.");
  unsigned long targetMillis = linear_regression();
  Serial.println("Linear regression completed.");
  Serial.print("Milliseconds  to target: ");
  Serial.print(targetMillis - millis());
  Serial.println(); // just a line break

  while (millis() < targetMillis) {
    delay(10);
  }
  // motor -> noSpeed

}
