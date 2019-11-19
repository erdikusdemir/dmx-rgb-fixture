# dmx-rgb-fixture
rgb fixture controller by DMX512 protocol.

I wrote this code when I was working for a production rental company in 2014 to paint the stages with different colors completely.
The code receives the DMX512 protocol from RX0 pin of the Arduino and uses 3 channels of the DMX universe. 
Channel 1 = color,  
Channel 2 = brightness,  
Channel 3 = shutter.  

Hardware:
A MAX485 or similar RS485 converter chip is connected to the RX0 pin of the Arduino UNO. This chip converts the symmetrical DMX signal into the UART signal.
