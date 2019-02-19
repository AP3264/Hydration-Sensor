import serial
import time

com = "com4"
connected = False
ser = None
##fname = "C:\\Users\\anavp\\Documents\\Drexel Terms\\Winter 2019\\Senior Design\\data.txt"
##locations=['COM1','COM4','COM8']
def saveResult(ser):
    val = True
    text_file = open("C:\\Users\\anavp\\Documents\\Drexel Terms\\Winter 2019\\Senior Design\\data.txt", 'w+')
    
    print(ser.readable())
    start = time.time()
    print(start)
    
    while val == True:
        msg = str(ser.readline())
        print(msg)
        msg = msg[2:-1]
        msg = msg.replace('\\r\\n','\r\n')
        msg = msg.replace('\\t', '\t')
        text_file.write(msg)
        end = time.time()
        
        if (end - start) >= 20:
            print("done")
            val = False
        
        
    text_file.close()
    ser.close()
        
def main():
##    try:
        print ("Trying to connect to", com)
        ser = serial.Serial(com, 38400)
        print(ser)
        if ser != None:
            print ("Connected")
            
        saveResult(ser)
        #break
##    except:
##        print ("Failed to connect")
    

if __name__ == "__main__":
    main()

    



