# The power supply

The power supply consist of two switching power supplies:
- a 5V 5A 25W one and
- a 24V 4.5A 108W one.

We opted for the [MW LRS-100-24](https://www.meanwell.com/productPdf.aspx?i=411) and the [SNT RS 25 5](https://www.meanwell.com/productPdf.aspx?i=392), mainly because they were available at a decent price.

Both power supplies are controlled by a relais. We didn't want to tinker with high voltage. So we dedided to use a commercially available relais that is operated using 5V and is specified to switch up to 10A: the [COM-KY019RM](https://joy-it.net/en/products/COM-KY019RM). The relais is used by the Arduino to turn off mains after a period of inactivity on the dashboard.

Both power supplies and the relais are mounted on a board that nicely fits in the base part of the dashboard. We placed it there so it does not need to be moved when opening the dashboard.

We have installed a mains socket in the dashboard cover near the power supplies. This is to further increase safety: should someone trip over the cord powering the dashboard, only the plug gets pulled. Otherwise a ripped out cable could expose high voltage leads.

The Arduino is exclusively powered via the USB connection to the PC.

## Improvements for a future version
We're planning to further increase the safety by adding a protective hood made of acrylic. We want the mains to be protected if someone manages to spill soda or a coffee on the dashboard...



