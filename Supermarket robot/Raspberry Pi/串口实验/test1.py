'''import serial 
ser = serial.Serial("COM5:USB-SERIAL",115200);
print(ser.isOpen());
ser.write("abbbab".encode('utf-8'));
ser.close();
'''


import serial  
port = 0  
ser = serial.Serial()  
ser = serial.Serial("COM5:USB-SERIAL",115200);
print(ser.portstr)  
print(ser.isOpen())
print(ser)   
while(1):  
    datainput = input("Please input the character:\n")  
    n = ser.write(datainput)  
    data = ser.read(n)  
    print(data)  
