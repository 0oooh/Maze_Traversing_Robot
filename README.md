# Maze_Traversing_Robot

This project controls a robot that traverses a maze, performs a 720-degree turn, and returns along its path. The robot uses infrared (IR) sensors to detect the maze and completes the mission by navigating the maze, rotating 720 degrees, and retracing its steps before stopping at the end.

Project Goals
	•	Maze Traversal: The robot uses IR sensors to detect walls and navigate the maze to the end.
	•	720-degree Rotation: After traversing the maze, the robot performs a 720-degree turn and retraces its path.
	•	Turn Correction: The robot corrects turn errors that occur when the IR sensor values are the same.
	•	Final Stop: The robot stops and waits when it reaches the last line.

Components Used
	•	Servo Motors: Controls the robot’s movement and rotation.
	•	Infrared (IR) Sensors: Used to detect the walls and navigate the maze.
	•	Arduino Board: The central controller for the robot.

Code Explanation
	•	move(): Moves the robot forward, backward, or adjusts its direction (left/right) using servo motors.
	•	irDistance(): Measures distance using the IR sensors.
	•	irDetect(): Emits an IR signal at a given frequency and reads the received signal.
	•	turnLeft() / turnRight(): Rotates the robot left or right for a specified time.
	•	moveBack(): Moves the robot backward for a given duration.

Key Features
	•	Straight Movement and Direction Adjustment: The robot adjusts its direction (left or right) based on IR sensor readings to move straight.
	•	Turn Error Correction: If the sensor values are identical, the robot corrects its turn direction to fix any errors.
	•	720-degree Turn and Return: After 10 turns, the robot performs a 720-degree turn and retraces its steps back.
	•	Final Straight Line: When the robot reaches the last line, it stops and waits.

Usage Instructions
	1.	Hardware Setup: Connect the infrared (IR) sensors and servo motors to the robot.
	2.	Upload the Code: Use the Arduino IDE to upload the code to the Arduino board.
	3.	Run the Robot: Place the robot in the maze, power it on, and it will automatically traverse the maze, perform a 720-degree turn, and return to its starting point.

Notes

This project helps in understanding robotics and automation systems and learning how to control robots using IR sensors. The setup and code are simple, but the implementation of maze navigation and turn correction algorithms is beneficial for learning.





미로 주파 로봇 (Maze Traversing Robot)

이 프로젝트는 미로를 주파하고, 720도 회전 후 돌아오는 로봇을 제어하는 코드입니다. 이 로봇은 적외선(IR) 센서를 사용하여 미로를 인식하고, 주어진 미션을 완료합니다. 로봇은 미로를 주파한 후, 720도 회전을 하고 다시 돌아오는 동작을 수행합니다. 마지막에는 멈추고 대기합니다.

프로젝트 목표
	•	미로 주파: 로봇은 IR 센서를 사용해 벽을 감지하고, 미로를 끝까지 주파합니다.
	•	720도 회전: 미로를 주파한 후, 로봇은 720도 회전하여 반대 방향으로 돌아옵니다.
	•	회전 보정: IR 센서 값이 같을 때 발생할 수 있는 회전 오류를 보정합니다.
	•	마지막 멈춤: 마지막 선에 도달하면 로봇은 멈추고 대기합니다.

사용 부품
	•	서보 모터 (Servo Motors): 로봇의 이동 및 회전 제어
	•	적외선(IR) 센서 (IR Sensors): 벽 감지 및 미로 탐색
	•	Arduino 보드 (Arduino Board): 로봇 제어

코드 설명
	•	move(): 서보 모터를 사용해 로봇을 전진/후진 또는 좌회전/우회전 시킵니다.
	•	irDistance(): IR 센서를 사용해 거리 값을 측정합니다.
	•	irDetect(): 주어진 주파수로 IR 신호를 출력하고, 수신된 신호를 읽습니다.
	•	turnLeft() / turnRight(): 주어진 시간만큼 로봇을 왼쪽 또는 오른쪽으로 회전시킵니다.
	•	moveBack(): 로봇을 일정 시간 동안 후진시킵니다.

주요 기능
	•	직진 및 방향 조정: IR 센서 값을 기준으로 로봇이 직진하거나 좌/우로 미세 조정을 합니다.
	•	회전 오류 보정: 센서 값이 동일한 경우 회전 방향을 수정하여 오류를 보정합니다.
	•	720도 회전 후 돌아가기: 10번의 회전 후, 로봇은 720도 회전하고 되돌아옵니다.
	•	마지막 직선: 마지막 선에 도달하면 로봇은 멈추고 대기합니다.

사용 방법
	1.	하드웨어 준비: 로봇에 적외선(IR) 센서와 서보 모터를 연결합니다.
	2.	코드 업로드: 아두이노 IDE를 사용하여 이 코드를 아두이노 보드에 업로드합니다.
	3.	실행: 로봇을 미로에 배치하고, 전원을 켜면 자동으로 미로를 주파하고, 720도 회전 후 돌아옵니다.

참고

이 프로젝트는 로봇 공학 및 자동화 시스템에 대한 이해를 돕고, IR 센서를 통한 로봇 제어 방법을 배울 수 있습니다. 코드와 하드웨어 구성은 간단하지만, 미로 탐색 및 회전 보정과 같은 알고리즘을 구현하는 데 유용합니다.

