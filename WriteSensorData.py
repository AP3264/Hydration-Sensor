import serial
import time
import os

com = "com4"
connected = False
ser = None
##fname = "C:\\Users\\anavp\\Documents\\Drexel Terms\\Winter 2019\\Senior Design\\data.txt"
##locations=['COM1','COM4','COM8']
def saveResult(ser, timeVal = 5):

    val = True
    text_file = open("C:\\Users\\anavp\\Documents\\Drexel Terms\\Winter 2019\\Senior Design\\data.txt", 'w+')
    
    print(ser.readable())
    start = time.time()
    print(start)
    
    while val == True:
        msg = str(ser.readline())
        msg = msg[2:-1]
        msg = msg.replace('\\r\\n','\n')
        msg = msg.replace('\\t', '\t')
        #print(msg)
        text_file.write(msg)
        end = time.time()
        
        if (end - start) >= timeVal:
            print("done")
            val = False
        
        
    text_file.close()
    ser.close()

def removeLines():
    n = 3
    nfirstlines = []

    with open("data.txt") as f, open("datatmp.txt", "w") as out:
        for x in range(n):
            nfirstlines.append(next(f))
        for line in f:
            out.write(line)

# NB : it seems that `os.rename()` complains on some systems
# if the destination file already exists.
##    os.remove("data.txt")
##    os.rename("datatmp.txt", "data.txt")
    
    f.close()
    out.close()
    
def saveWave():
    counter = 0
    prevCount = 0
    ident = " \n"
    is670 = True
    is850 = False
    is950 = False
    isPattern = False
    file670 = open("file670.txt", 'w+')
    file850 = open("file850.txt", 'w+')
    file950 = open("file950.txt", 'w+')
    f = file670
    
    fh = open("C:\\Users\\anavp\\Documents\\Drexel Terms\\Winter 2019\\Senior Design\\datatmp.txt")
    for line in fh:
        if line == ident and is950 == True:
            prevCount = counter
            counter += 1
            print (counter)
            #print(prevCount)
            if prevCount == 3:
                is950 = False
    		#do sth
                print("Happening")
                f = file670
                is670 = True
                counter = 0
                prevCount = 0

##        elif line == ident and isPattern == True:
##            print("670")
##            is670 = True
##            isPattern = False
##            f = file670


        elif line == ident and is670 == True:
            print("850")
            is850 = True
            is670= False
            f = file850

        elif line == ident and is850== True:
            print("950")
            is950 = True
            is850 = False
            f = file950
        
        else:
            f.write(line)
            
    fh.close()
            
        
def main():
##    try:
        print ("Trying to connect to", com)
        ser = serial.Serial(com, 38400)
        print(ser)
        if ser != None:
            print ("Connected")
            
        saveResult(ser,60)
        removeLines()
        saveWave()
        #break
##    except:
##        print ("Failed to connect")
    

if __name__ == "__main__":
    main()

    



