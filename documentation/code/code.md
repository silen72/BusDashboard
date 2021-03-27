# A brief glimpse at what the code does

There are two main tasks the board has to perform:
- receive OMSI updates from the PC and turn lights on and off accordingly
- check for (newly) pressed or released buttons on the dashboard and send keystrokes to the PC accordingly

The challenge is to perform these tasks in a way that there is no noticable lag - neither between pressing a button on the dashboard and the corresponding action in the bus simulator nor between an update in OMSI like "the indicator light is now on" (and visibly so on the screen) and the actual indicator on the dashboard. In order to achieve this, the communication aspect is treated with the highest priority.

The code does this by breaking those two tasks down into subtasks:
- receive OMSI updates from the PC
- partially scan the buttons and switches on the dashboard
- update the lamp driver pcb if necessary
- send keystrokes to the PC if necessary
- added safety: turn off mains if there was no activity on the dashboard and from the PC for a certain time
- repeat

## Receive OMSI updates from the PC

KOMSI sends a stream like this every frame: A123B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0X0Y0Z0a0b0c0d0e0f0. This stream consists of pairs of commands (a single char, like 'A') and one assciated value. The value is transferred as ascii values, so 123 becomes '1' '2' '3'.

What OMSI value a command character represents is configurable in KOMSI. Therefore the code is also flexible regarding what command triggers which actions. The KOMSIHandler class takes chars from the stream (KOMSIHandler.processIncoming) and builds the complete pair (command and value). When a pair is complete it notifies listeners for that specific command that a value for that command has been received. To become a listener, a class must implement KomsiCommandListener.receiveCommand. To be notified it must be added as a listener to the KOMSIHandler (KOMSIHandler.addListener). The command to listen to must be specified upon adding.

For a give command it is possible to have no listener, one listener or multiple listeners.

For example this is used by the code that handles the indicator lights. KOMSI is only capable of sending the state of the indicator light (on or off). It does not send the context: is it on because the bus is taking a turn? Or is it on because the hazard warning lights are switched on? This is handled in two separate classes. Both listen to the indicator command and light their corrensponding light on the dashboard according to their state (they also handle "their" switch on the dashboard, so they can derive whether the indicator light is on because of them).

A KomsiCommandListener might want to toggle a light. It may request the lamp driver to do that (see "Update the lamp driver pcb if necessary").

## Partially scan the buttons and switches on the dashboard

The ButtonHandler class is responsible for scanning all the knobs, switches and buttons on the dashboard. The [buttonmatrix pcb](../buttonmatrix/buttonmatrix.md) is organized in rows and columms. Every time the buttonHandler().scan() is called it scans the next row. The scan frequency is limited (only every x ms) to prioritize receiving OMSI updates.

For each of the eight buttons in the row it determines the current state (connected or not connected = pressed or not pressed). It notifies listeners of the current state of the button as well as the state the button had on the last scan.

To react to button states the code takes the same flexible approach as with the KOMSIHandler (see above). To become a listener, a class must implement ButtonListener.setCurrentState. To be notified it must be added as a listener to the ButtonHandler (ButtonHandler.addListener). The button position to listen to must be specified upon adding.

A ButtonListener might want to toggle a light. It may request the lamp driver to do that (see "Update the lamp driver pcb if necessary").
A ButtonListener might also want to send a certain keystroke to the PC. It may request the keyboard handler to do that (see "Send keystrokes to the PC if necessary").

## Update the lamp driver pcb if necessary

Both KomsiCommandListener and ButtonListener might have requested to turn a light on or off. They have done so by calling LampHandler.setState, passing the position of the lamp on the [lamp driver pcb](../lampdriver/lampdriver.md) and the desired state.

The LampHandler class keeps track of the desired state and the real current state. Only if they differ, it takes the time to make the desired state the new real current state by actually communicating with the lamp driver pcb. The lamp driver update frequency is limited (only every x ms) to prioritize receiving OMSI updates.

## Send keystrokes to the PC if necessary

To connect dashboard components with its virtual counterparts in OMSI it is necessary to keep track of the state of the actual component and send keystrokes to the PC upon changing states. Keeping track of the state of the actual component is done by ButtonListeners (see above).

Sending keystrokes is done by either telling the KeyboardHandler class what action to take. It provides four actions:
- press a key (and don't release it)
- release a key
- press a key for a certain time then release it
- press a key for a certain time then release it and also press strg, shift, alt (or any combination of these)

Behind the scenes KeyboardHandler keeps a chain of KeyActions. Each of the aforementioned actions adds a KeyAction to the chain. A KeyAction may have follow-up KeyActions attached to it.

Every time KeyboardHandler.update() is called it takes one KeyAction from the chain and performs it (KeyAction.run). If the KeyAction is done and has no follow-up KeyActions it is removed from the chain. But if it has follow-up KeyActions it stays in the chain, marked as done. When its run()-method gets called next time, it simply calls KeyAction.run on its follow-up (which might in turn pass it to follow-ups and so on). This enables complex sequences of keystrokes. Only if all follow-up KeyActions are done, the KeyAction is removed from the chain.

## Added safety: turn off mains on inactivity

Let's face it: this is selfmade electronics including power supplies with considerable voltages and currents. We are pretty confident that it does what it is supposed to. But still: we simply do not want to keep it powered when it is not actively used to lower possible risks (and power consumption).

Therefore the code contains logic to keep track of the last activity and thus detect inactivity. After a configurable duration of inactivity it simply turns off the relay that powers the [two switching power supplies](../powersupply/powersupply.md).

What is considered activity? Everytime a state changes on the dashboard - it shows that a user is actually doing something with the dashboard. To avoid unwanted power offs (maybe it is a really looooong tour without turning, stopping at bus stops or whatever?) the code does a "knock knock" with the indicator relay shortly before it actually turns the power off. The user might then ~~wake up~~ do any activity on the dashboard to reset the idle timer.