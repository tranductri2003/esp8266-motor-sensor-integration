#include <Stepper.h>

const int stepsPerRevolution = 1024;  // Số bước mỗi vòng quay của động cơ bước

// Khởi tạo động cơ bước với các chân IN1, IN2, IN3, IN4:
Stepper myStepper(stepsPerRevolution, 0, 4, 5, 16);

void setup() {
  // Đặt tốc độ động cơ (vòng quay trên phút)
  myStepper.setSpeed(60);
}

void loop() {
  // Bước tới 1 vòng (số bước theo chiều thuận)
  myStepper.step(stepsPerRevolution);
  delay(1000);  // Đợi một giây

  // Bước ngược lại 1 vòng (số bước theo chiều nghịch)
  myStepper.step(-stepsPerRevolution);
  delay(1000);  // Đợi một giây
}
