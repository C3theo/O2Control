/*CMS50D.h
UART Bridge Controller
5 Bytes

*/

class CMS50DPLUS(){
// 1st byte
signalStrength
fingerOut
droppingSpO2
beep
// 2nd byte
pulseWaveform

// 3rd byte
barGraph
probeError
searching
pulseRate

// 4th byte
pulseRate

// 5th byte
bloodSpO2



public:

private:
	unsigned int *registers;
	




};