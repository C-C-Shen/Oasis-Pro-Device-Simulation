# COMP3004 Team 33
## Team Members
- Gurpiar Brar
  - Battery Stuff
  - Intensity Stuff
  - Documentation
- Kayla Gingras
  - UI
  - Documentation
- Christopher Shen
  - TODO
 
**Message To TA: Could you please check our design documents for any improvements? Thank you.**
 
  ## Current Functionailty 
 - Hold the Power Button for 2 Seconds to Turn on the device
 - Click the power button to shuffle through session types
 - Up and Down arrows to shuffle through session frequencies
 - Toggle Skin Connection to attach clips to earloabs
 - Click Confirm to start session
 - During the session user can click on the record button to record the session to a .txt file stored in the debug folder
 - The user can hold the confirm button for 1 second during the session to save the intensity for that session preset
 - The device will power off after session is over or the power button is held
 
 
 ## TODO
- User Designated Time
- Remove Extra UI
- Battery Function
- Rearrange Drain Battery
- Soft On Animation (1-8 using timers) If interrupted after the connection test, do not load saved preferences 
- Soft Off Animation (8-1 using timers) when powering off
- Onboot call displayBattery(0 sleep one second)
- Memory Mangement
- Fix new Session in ConfirmButtonRelease
- Some UI updates such as:
  - Intenisty number blinking when adjusted
  - Saving intensity animation
  - Record button icon
  - Indicate selected session with blinking number
- Sequence Diagrams
- Tracability Diagram
- UML Diagram
