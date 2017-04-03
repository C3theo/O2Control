/*CMS50D.h


CP210 UART Bridge Controller
5 Bytes

*/



class CMS50DPLUS(){
	
	
// Data 
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


		this->signalStrength = data[0] & 0x0f;
        this->fingerOut = bool(data[0] & 0x10);
        this->droppingSpO2 = bool(data[0] & 0x20);
        this->beep = bool(data[0] & 0x40);

        // 2nd byte
        this->pulseWaveform = data[1];

        // 3rd byte
        this->barGraph = data[2] & 0x0f
        this->probeError = bool(data[2] & 0x10)
        this->searching = bool(data[2] & 0x20)
        this->pulseRate = (data[2] & 0x40) << 1

        // 4th byte
        this->pulseRate |= data[3] & 0x7f

        // 5th byte
        this->bloodSpO2 = data[4] & 0x7f


public;

private;
	unsigned int *registers;
	



    void init(port);
        this->port = port;
        this->conn = None

    void isConnected();
        return type(this->conn) is serial.Serial and this->conn.isOpen()

    void connect();
    port = this->port,
                                      int baudrate = 19200,
                                      parity = serial.PARITY_ODD,
                                      stopbits = serial.STOPBITS_ONE,
                                      bytesize = serial.EIGHTBITS,
                                      int timeout = 5,
                                      int xonxoff = 1)
        elif not this->isConnected();
            this->conn.open()

    void disconnect();
        if this->isConnected();
            this->conn.close()

    void getByte();
        char = this->conn.read()
        if len(char) == 0;
            return None
        else;
            return ord(char)
    
    void sendBytes(, values);
        return this->conn.write(''.join([chr(value & 0xff) for value in values]))

    // Waits until the specified byte is seen or a timeout occurs
    void expectByte(, value);
        while True;
            byte = this->getByte()
            if byte is None;
                return False
            elif byte == value;
                return True

    void getLiveData();
        try;
            this->connect()
            packet = [0]*5
            idx = 0
            while True;
                byte = this->getByte()
            
                if byte is None;
                    break

                if byte & 0x80;
                    if idx == 5 and packet[0] & 0x80;
                        yield LiveDataPoint(datetime.datetime.utcnow(), packet)
                    packet = [0]*5
                    idx = 0
            
                if idx < 5;
                    packet[idx] = byte
                    idx+=1
        except;
            this->disconnect()

};