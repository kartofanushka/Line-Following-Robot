## Line following robot

This is my first semester university project.

A wheeled robot that waits for cargo to be loaded, autonomously navigates a predefined route by following a line on the floor, drops off the payload at a designated point, and stops at the finish.

**Cargo detection and start**

The robot begins in standby with `counter = 0`. An HC-SR04 ultrasonic sensor is mounted inside the cargo bed and continuously measures the distance to the floor of the bed. When an object is placed inside and the distance drops below 15 cm, the robot drives forward and sets `counter` to 1 — this is the only trigger that starts the robot. Without cargo the robot ignores the line sensors entirely and stays still.

**Line following**

Once `counter >= 1`, navigation is handed off to five IR sensors mounted across the front underside of the chassis. Each sensor reads `1` over the black line and `0` over the light surface. Every loop cycle all five values are evaluated together:

- Only centre sensor (S3) active → drive straight at PWM 100
- Left sensors (S1 or S2) lose the line → turn right at PWM 180
- Right sensors (S4 or S5) lose the line → turn left at PWM 180

Higher PWM during turns gives sharper correction to keep the robot on the line.

**Checkpoint counting**

When all five sensors read zero simultaneously — an intersection or a marked stop point — the robot drives forward and increments the counter. This is how the robot measures progress along the route. Each all-zero event is one checkpoint passed. The counter therefore reflects not just cargo state but how far along the route the robot has travelled.

Specific counter states and what happens:

| Counter | Event |
|---|---|
| 0 | Standby — waiting for cargo |
| 1 | Cargo loaded — line following begins |
| 2 | Delivery point reached — payload triggered, counter set to 3 |
| 3–6 | Continuing along the return route |
| 7 | Final stop — robot halts permanently |

**Payload delivery**

When `counter == 2` and all five sensors read zero, the robot stops both motors and calls `payload()`. The servo sweeps from 0° to 180° in steps of 3° (15 ms per step), then returns from 180° back to 0° in steps of 1° — tipping the bed to release the cargo. After the sweep the counter increments to 3 and line following resumes. The `counter == 2` guard ensures the delivery action fires exactly once.

**Stop condition**

Two conditions permanently halt the robot: counter reaching exactly 7 while all sensors read zero, or counter exceeding 7 for any reason. Both call `stop()` and set all motor driver pins low.

**Components**

- Arduino Uno (ATmega328)
- HC-SR04 ultrasonic sensor on pins 8 (Trig) and 11 (Echo) — mounted inside cargo bed
- 5× IR line sensors on analog pins A0–A4
- L298N dual H-bridge motor driver — direction pins 2, 3, 4, 7 and PWM on pins 5, 6
- SG90 servo motor on pin 12 — cargo release mechanism
- Four DC gear motors
