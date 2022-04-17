# COMP3004 Team 33
## Team Members
- Gurpiar Brar
  - Battery
  - Adjusting Intensity
  - Saving Preferences
  - Animations
  - Use Cases
- Kayla Gingras
  - UI
  - Documentation
  - Connection Test
- Christopher Shen
  - User Designated Sessions (Custom Time)
  - Session Recording
  - Back end for UI navigation

## File Organization
- The source and header files are in the oasisProSimulation folder.
- The icon resources are in oasisProSimulation/resources.
- The design documentation files (UML, Use Cases, etc.) are in the Design Documentation folder seperately.

## Tested Scenarios (all functional)
- Power On
  - Hold the Power button for 2 seconds while the device is off to turn on the device
- Power Off
  - Hold the Power button for 2 seconds while the device is on to turn off the device
- Battery Level Low
  - Run a session and observe as the battery level lowers. There will be a battery low warning.
- Selecting a session
   - There are 3 groups (20min, 45min, and user designated)
   - There are 4 session types (Delta, Sub Delta, Alpha, and Theta)
   - Press the power button while the device is powered on to shuffle groups
   - Press the up/down arrows while the device is powered on to shuffle session types
 - Connection Test
   - Press the confirm button to begin the Connection Test
   - If the skin is connected, the graph will show green/yellow and the session will begin to run shortly
   - If the skin is not connected, the graph will blink red and the session will not run until skin is connected.
 - Intensity
   - Press the Up/Down buttons while a session is being run to adjust the intensity up/down
   - Hold the confirm button to save the intensity level
 - Recording
   - Press the record button during a session. The session information will be added to the recording .txt file.

  ## Base Scenario
 - Hold the Power button for 2 seconds to power on the device
 - Press the Power button to shuffle through session groups
 - Press the Up/Down arrows to shuffle through session types
 - Toggle Skin Connection to attach clips to earloabs
 - Click the Confirm button to start session
 - During the session the user can click on the Record button to record the session to a .txt file stored in the debug folder
 - Recorded sessions can be view by manually locating .txt file in debug folder or pressing the print recorded sessions button (prints to console)
 - The user can hold the confirm button for 1 second during the session to save the intensity for that session preset
 - The device will power off after session is over or the power button is held

## Design Decisions
- The device only drains battery while a session is running. This is because we interpreted the specifications to mean that battery only depletes during a therapy, even though real devices typically always have their battery draining.
- User designated time has a minimum time of 20 minutes, and a maximum time of 3 hours and can be increased/decreased in 20 minute intervals. It was unclear how this element should be implemented so we capped the amount of time the user could select to 3 hours so that the device will be able to handle the amount of battery drained from the therapy.
- For implementing Saving History, we decided to add a record button. This button can be pressed during a session to add the Recording to a .txt file once the therapy has finished. We thought this was a suitable way to record the history, and in the future the history feature could even be expanded to allow playback by reading from the .txt file if desired.
- As it would be too lengthy to run a full 20 minute session for a demo, therapy minutes are represented in seconds.
