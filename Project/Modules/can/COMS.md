# Terminology

* Wheelboard  - wheel control
* Turnboard  - turning of the wheel
* Driveboard  - router on main bus controlling the wheels
* Child board -  Wheelboard and Turnboard, those that are on a separate CAN network

# Proposition
In the future, the drive CAN lines can potentially be combined literally by soldering them together. 
I propose the use of unique ids on each wheel (same ID on Wheelboard and Turnboard) to facilitate this change. 
For now the CAN networks stay separate.

id distribution:
```
     (front)
(0) O-|‾‾‾|-O (1)
(2) O-|   |-O (3)
(4) O-|___|-O (5)
```

In the current configuration there is one Driveboard controlling two wheels, for two Turnboards (except center wheels) and two Wheelboards in total. 

To stop the whole mobile platform when any of the boards encounters an error, each board is assigned a panic ID alongside a normal message ID. 

*Child boards*
If a child encounters an error/unrecoverable state it tries to send a panic message and reboots. 
Not receiving an instruction from a Driveboard in a set amount of time counts as an error.
If a child board receives any panic message it reboots to bring the whole system into a known state.
When booted, Child board waits for the first drive board instruction before rebooting on inactivity.

*Drive boards*
If a board encounters an error/unrecoverable state it tries to send a panic message and reboots. 
Any of the Child boards not responding for a set amount of time counts as an error. 
Any of the Driveboards not sending messages on main CAN for a set amount of time counts as an error.
When the Driveboard receives a panic message it propagates it to the drive or main CAN depending on the source and reboots.
When booted, Driveboard sends empty messages until all the Child boards start responding. 
 
# Drive CAN Communication

```
     (front)
(0) O-|‾‾‾|-O (1)
(2) O-|   |-O (3)
(4) O-|___|-O (5)
```

Each board is assigned a panic and a normal message ID

TODO decide on speed telemetry format
1. 32 bit float (raw data from controller)
2. 8 bit int as a percentage from max speed (ex. 30 kph) 

## Drive boards
### PANIC ID = 0
### NORMAL ID = 128 (WID 0), 130 (WID 2), 132 (WID 4)
* data[0] - speed left wheel (0, 2, 4)
* data[1] - angle left wheel (0, 4) 
* data[2 - 7] - unused, start/stop/speed multiplier

### NORMAL ID = 129 (WID 1), 131 (WID 3), 133 (WID 5)
* data[0] - speed right wheel (1, 3, 5)
* data[1] - angle right wheel (1, 5) 
* data[2 - 7] - unused, start/stop/speed multiplier


## Wheel boards
### PANIC ID = 1 - 6 (wheel ID + 1)
* data[0] - type of error id
* data[1] - subsystem
* data[4-7] - error code (ex. HAL subsystem error code)
        
### ID = 134 - 139 (panic ID + 133)
* data[0-7] - telemetry/alive

## Turn boards
### PANIC ID = 7 - 12 (wheel ID + 7)
* data[0] - type of error id
* data[1] - subsystem
* data[4-7] - error code (ex. HAL subsystem error code)

### ID = 140 - 145 (panic ID + 133)
* data[0-7] - telemetry/alive
