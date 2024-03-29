##Hydration Sensor
##Senior Design - Team 18
##Anav Pradhan, Anishi Patel, Nicholas Joseph, Tyler Harris
##Python User Interface/Post Processing Script

from tkinter import *
from tkinter.filedialog import askopenfilenames
import matplotlib.pyplot as plt
import serial
import time
import os
import datetime
import numpy as np
import statistics
import serial.tools.list_ports
import warnings


#set datetime for folder naming
now = datetime.datetime.now()

#get current directory for folder creation
current_dir = os.getcwd()
final_directory = os.path.join(current_dir, r'Files')
if not os.path.exists(final_directory):
    os.makedirs(final_directory)
final_dir = final_directory + "\\"

class MainProgram:

    def __init__(self, master):
        self.master = master
       # self.com = "/dev/cu.usbmodem144302"
        self.connected = False
        self.ser = None
        self.fname = "data.txt"
        master.title("Hydration Sensor")
        self.running = True  # Global flag
        self.marker = False
        
        arduino_ports = [
           p.device
           for p in serial.tools.list_ports.comports()
           #if p.description != None  # may need tweaking to match new arduinos
               if "mEDBG" in p.description
       ]

        if not arduino_ports:
            raise IOError("No Arduino found")
        if len(arduino_ports) > 1:
            warnings.warn('Multiple Arduinos found - using the first')

        self.com = arduino_ports[0]
#         self.time = 15 # default test time duration is 15 seconds

        # create buttons for GUI
                
        self.img = PhotoImage(file='Drexel_Logo.gif')
        self.img = self.img.zoom(20, 5)
        self.img = self.img.subsample(60, 15)
        self.img_label = Label(master, image=self.img)
        self.img_label.pack()

        self.label = Label(master, text="Hydration Sensor")
        self.label.config(font=('Helvetica',16, 'bold'))
        self.label.pack(fill=X,pady=10)
        
        # Enter time duration first!!! and then hit log data
        time_duration = Label(master, text="Enter Time Duration for Test (in sec): ")
        time_duration.config(font=('Helvetica',10, 'bold'))
        time_duration.pack()
        
        self.time_duration = Entry(master)
        self.time_duration.pack()
        
        # Enter current level
        current_level = Label(master, text="Enter Current: ")
        current_level.config(font=('Helvetica',10, 'bold'))
        current_level.pack()
        
        self.current_level = Entry(master)
        self.current_level.pack()
        
        # Enter gain value
        gain_val = Label (master, text="Enter Gain: ")
        gain_val.config(font=('Helvetica',10, 'bold'))
        gain_val.pack()
        
        self.gain_val = Entry(master)
        self.gain_val.pack()
        
        # Enter test step description
        test_label = Label(master, text="Enter Test Description: ")
        test_label.config(font=('Helvetica',10, 'bold'))
        test_label.pack()
        
        self.test_label = Entry(master)
        self.test_label.pack()
        
        # Log Button
        self.log_button = Button(master, text="Log Data", command=self.log_data, fg = "light goldenrod", bg = 'navy blue',width=20)
        self.log_button.config(font=('Helvetica',12, 'bold'))
        self.log_button.pack(pady=5)
        
        # Plot Button
        self.plot_button = Button(master, text="Plot Graph", fg = "light goldenrod", bg = 'navy blue',command=self.plotGraph,width=20)
        self.plot_button.config(font=('Helvetica',12, 'bold'))
        self.plot_button.pack(pady=5)
        
        self.file_select = Button(master, text="Select Files to Plot",fg = "light goldenrod", bg = 'navy blue', command=self.plotSelectedGraph, width=20)
        self.file_select.config(font=('Helvetica',12, 'bold'))
        self.file_select.pack(pady=5)
        
        # Live Plot
        self.live_plot_button = Button(master, text="Live Plot", fg = "light goldenrod", bg = 'navy blue',command=self.LivePlot,width=20)
        self.live_plot_button.config(font=('Helvetica',12, 'bold'))
        self.live_plot_button.pack(pady=3)
        
        self.stop = Button(master, text="Stop", command=self.stop, bg = "light goldenrod", fg = 'navy blue',width = 10)
        self.stop.config(font=('Helvetica', 10,'bold'))
        self.stop.pack(pady=3)

        self.mark = Button(master, text="Mark Point", command=self.mark, bg = "light goldenrod", fg = 'navy blue',width = 10)
        self.mark.config(font=('Helvetica', 10,'bold'))
        self.mark.pack(pady=3)
        
        # Select Files to Plot

        


        # Close out GUI
        self.close_button = Button(master, text="Close", command=master.quit,fg = 'midnight blue', bg='red', width=15)
        self.close_button.config(font=('Helvetica',12, 'bold'))
        self.close_button.pack(pady=30)

    # open serial port for connection and start the program
    def log_data(self):
        ##    try:
        print("Trying to connect to", self.com)
        self.ser = serial.Serial(self.com, 38400)
        print(self.ser)
        if self.ser is not None:
            print("Connected")
        time.sleep(1)
            
        # Get User Input for Length of the Test
        global time_duration
        time_duration = float(self.time_duration.get())
        self.time = time_duration
        
        global gain_val
        gain_val = self.gain_val.get()
        
        global current_level #10,15,20,25,35
        current_level = self.current_level.get()
        
        
        if gain_val == '20':
            self.ser.write(b'5')
        elif gain_val == '5':
            self.ser.write(b'2')
            print("A1")
        elif gain_val == '10':
            self.ser.write(b'3')
            print("A2")
        elif gain_val == '15':
            self.ser.write(b'4')
            print("A3")
        else: # default is gain 1
            self.ser.write(b'1')
            
##        if current_level == "10":
##            self.ser.write(b'1')
##        elif current_level == '15':
##            self.ser.write(b'2')
##        elif current_level == '25':
##            self.ser.write(b'4')
##        elif current_level == '30':
##            self.ser.write(b'5')
##        elif current_level == '35':
##            self.ser.write(b'6')
##        else: # default is current 20
##            self.ser.write(b'3')
            
        

        self.save_result(self.ser, self.fname, self.time)
        self.remove_lines(self.fname)
        self.save_wave()
        print('Done!')

    # parse incoming data and save to file
    def save_result(self, serial, file_name, time_val=5):
        val = True
        text_file = open(file_name, 'w+')
        count = 0

        print('Readable =',serial.readable())
        start = time.time()
        print('Collecting data from serial', end='')

        while val is True:
            msg = str(serial.readline())
            msg = msg[2:-1]
            msg = msg.replace('\\r\\n', '\n')
            msg = msg.replace('\\t', '\t')
            # print(msg)
            text_file.write(msg)
            end = time.time()
            count += 1
            if (count % self.time*2) == 0:
                print('.', end='')

            if (end - start) >= time_val:
                print("Done collecting data from serial")
                val = False

        text_file.close()
        serial.close()

    # remove first empty lines from serial
    def remove_lines(self, file_name):
        n = 1
        n_first_lines = []

        with open(file_name) as f, open("datatmp.txt", "w") as out:
            for x in range(n):
                n_first_lines.append(next(f))
            for line in f:
                out.write(line)

        # NB : it seems that `os.rename()` complains on some systems
        # if the destination file already exists.
        ##    os.remove("data.txt")
        ##    os.rename("datatmp.txt", "data.txt")

        f.close()
        out.close()

    # folder and file creation, parsing and sorting
    def save_wave(self):
        print('Creating files and saving data', end='')

        count_dot = 0
        counter = 0
        prev_count = 0
        ident = " \n"
        is670 = True
        is850 = False
        is950 = False

        CycleTime = 500000
        CyclePeriod = 500000

        timestr = time.strftime("%Y%m%d-%H%M%S")
        
        # isPattern = False
        global current_level
        current_level = self.current_level.get()
        
        global gain_val
        gain_val = self.gain_val.get()
        
        file670 = open(final_dir+"file670_"+ current_level + "mA_gain" + gain_val + "_" + timestr + ".txt", 'w+')
        file850 = open(final_dir+"file850_"+ current_level + "mA_gain" + gain_val+ "_" + timestr + ".txt", 'w+')
        file950 = open(final_dir+"file950_"+ current_level + "mA_gain" + gain_val + timestr + ".txt", 'w+')
        fileAllPoints = open(final_dir+"fileAllPoints_"+ timestr +".txt", 'w+')

        temp670 = open(final_dir+"file670.txt", 'w+')
        temp850 = open(final_dir+"file850.txt", 'w+')
        temp950 = open(final_dir+"file950.txt", 'w+')
        tempAllPoints = open(final_dir+'fileAllPoints.txt', 'w+')
        tempAvgPoint = open(final_dir+'avgAllPoint.txt', 'w+')
        tempAvg670 = open(final_dir+"tempAvg670.txt", 'w+')
        tempAvg850 = open(final_dir+"tempAvg850.txt", 'w+')
        tempAvg950 = open(final_dir+"tempAvg950.txt", 'w+')

        avg670 = open(final_dir+"avg670_" + current_level + "mA_gain" + gain_val + "_" + timestr + ".txt", 'w+')
        avg850 = open(final_dir+"avg850_" + current_level + "mA_gain" + gain_val + "_" + timestr + ".txt", 'w+')
        avg950 = open(final_dir+"avg950_" + current_level+ "mA_gain" + gain_val + "_" + timestr + ".txt", 'w+')
        avgAllPoints = open(final_dir+"avgAllPoints_" + current_level + "mA_gain" + gain_val + "_" + timestr + ".txt", 'w+')

        fileList = [file670,file850,file950,fileAllPoints,tempAllPoints,tempAvg670, tempAvg850, tempAvg950, tempAvgPoint, temp670,temp850,temp950,avg670,avg850,avg950,avgAllPoints]
        for file in fileList:
            file.write(self.add_headers())

        listOfVals =[]

        list670=[]
        list850=[]
        list950=[]
        f = file670
        t = temp670
        a = tempAvg670
        l = list670

        fh = open("datatmp.txt")
        for line in fh:

            if line == "L3\n" and is950 == True:
                prev_count = counter
                counter += 1
                # print(prevCount)
                if prev_count == 0:
                    is950 = False
                    # do sth
                    # print("Happening")
                    f = file670
                    t = temp670
                    a = tempAvg670
                    l = list670
                    is670 = True
                    averagedVal = sum(listOfVals) / len(listOfVals)
                    avgLine = str(CycleTime) + '\t' + str(averagedVal)+ '\n'
                    avg950.write(avgLine)
                    tempAvg950.write(avgLine)
                    avgAllPoints.write(avgLine)
                    listOfVals.clear()

                    CycleTime +=CyclePeriod

                    counter = 0

            elif line == "L1\n" and is670 == True:
                prev_count = counter
                counter += 1
                # print(prevCount)
                if prev_count == 1:
                    #print("850")
                    is850 = True
                    is670 = False
                    f = file850
                    t = temp850
                    a = tempAvg850
                    l = list850
                    averagedVal = sum(listOfVals)/len(listOfVals)
                    avgLine = str(CycleTime) + '\t' + str(averagedVal) + '\n'
                    avg670.write(avgLine)
                    tempAvg670.write(avgLine)
                    avgAllPoints.write(avgLine)
                    listOfVals.clear()
                    
                    counter = 0

            elif line == "L2\n" and is850 == True:
                is950 = True
                is850 = False
                f = file950
                t = temp950
                a = tempAvg950
                l = list950
                averagedVal = sum(listOfVals) / len(listOfVals)
                avgLine = str(CycleTime) + '\t' + str(averagedVal)+ '\n'
                avg850.write(avgLine)
                tempAvg850.write(avgLine)
                avgAllPoints.write(avgLine)
                listOfVals.clear()

            else:
                count_dot += 1
                if (count_dot % 45) == 0:
                    print('.', end='')
                f.write(line)
                t.write(line)
                fileAllPoints.write(line)
                tempAllPoints.write(line)
        
                lineVal = line.rstrip('\n')
                listOfVals.append(float(lineVal.split("\t")[1]))
                l.append(float(lineVal.split("\t")[1]))
                
        
        list670 = list670[40:]
        list850 = list850[40:]
        list950 = list950[40:]
        
        mean670 = statistics.mean(list670)
        stdev670 = statistics.stdev(list670)
        if stdev670 != 0:
            snr670 = 20 * (np.log10(mean670/stdev670)) # snr val in dB
        else:
            snr670 = "invalid"
            
        file670.write("SNR: ")
        print("SNR 670: ")
        print(snr670)
        file670.write(str(snr670))
        
        mean850 = statistics.mean(list850)
        stdev850 = statistics.stdev(list850)
        if stdev850 != 0:
            snr850 = 20 * (np.log10(mean850/stdev850)) # snr val in dB
        else:
            snr850 = "invalid"
            
        file850.write("SNR: ")
        print("SNR 850: ")
        print(snr850)
        file850.write(str(snr850))
        
        mean950 = statistics.mean(list950)
        stdev950 = statistics.stdev(list950)
        if stdev950 != 0:
            snr950 = 20 * (np.log10(mean950/stdev950)) # snr val in dB
        else:
            snr950 = "invalid"
        file950.write("SNR: ")
        print("SNR 950: ")
        print(snr950)
        file950.write(str(snr950))
                
        fh.close()
        file670.close()
        file850.close()
        file950.close()
        
        fileAllPoints.close()
        avg670.close()
        avg850.close()
        avg950.close()
        avgAllPoints.close()
        temp670.close()
        temp850.close()
        temp950.close()
        tempAvg670.close()
        tempAvg850.close()
        tempAvg950.close()
        tempAllPoints.close()
        

    # add headers to text file
    def add_headers(self):
        
        global test_label
        test_label = self.test_label.get()

        line1 = now.strftime('%Y-%m-%d %H:%M')+'\n'
        line2 = "Team 18"+'\n'
        line3 = "Tyler Harris, Nicholas Joseph, Anishi Patel, Anav Pradhan"+'\n'
        line4 = 'Non-Invasive Hydration Sensing System'+'\n\n'
        line5 = "Test Description: " + test_label + '\n\n'
        line6 = 'Time(us)\tVoltage(mv)\n'
        line7 = "\n"

        return line1+line2+line3+line4+line5+line6+line7

    # submenu
    def file_selector(self):
        sub = Tk()
        sub.geometry('200x200')

        
    # grab data from text file and plot using matplotlib
    def plotGraph(self):
        x = []
        y = []
        x2 = []
        y2 = []
        x3 = []
        y3 = []
        xall = []
        yall = []
        with open(final_dir+'tempAvg670.txt') as f:
            for i, line in enumerate(f):
                if i > 14:
                    x.append(int(line.split()[0]) / 1000.0)
                    y.append(float(line.split()[1]))

        with open(final_dir+'tempAvg850.txt') as f:
            for i, line in enumerate(f):
                if i > 14:
                    x2.append(int(line.split()[0]) / 1000.0)
                    y2.append(float(line.split()[1]))

        with open(final_dir+'tempAvg950.txt') as f:
            for i, line in enumerate(f):
                if i > 14:
                    x3.append(int(line.split()[0]) / 1000.0)
                    y3.append(float(line.split()[1]))

        with open(final_dir+'avgAllPoint.txt') as f:
            for i, line in enumerate(f):
                if i > 14:
                    xall.append(int(line.split()[0]) / 1000.0)
                    yall.append(float(line.split()[1]))


        plt.plot(x, y, 'r-')
        plt.plot(x2, y2, 'y-')
        plt.plot(x3, y3, 'g-')
        #plt.plot(xall,yall, linestyle = '-')
        #plt.ylim(0, 5000)
        plt.title('Voltage Sweep Over Time')
        plt.xlabel('Time(ms)')
        plt.ylabel('Voltage(mV)')
        plt.gca().legend(('670nm','850nm', '950nm'))
        plt.show()

        
        
    # grab data from text file and plot using matplotlib
    def plotSelectedGraph(self):
        selectedFiles = askopenfilenames()
        x = []
        y = []
        x2 = []
        y2 = []
        x3 = []
        y3 = []
        x4 = []
        y4 = []
        x5 = []
        y5 = []
        x6 = []
        y6 = []
        xall = []
        yall = []
        if len(selectedFiles) == 1:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
        elif len(selectedFiles) == 2:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
            with open(selectedFiles[1]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x2.append(int(line.split()[0])/ 1000.0)
                        y2.append(float(line.split()[1]))
        elif len(selectedFiles) == 3:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
            with open(selectedFiles[1]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x2.append(int(line.split()[0])/ 1000.0)
                        y2.append(float(line.split()[1]))
            with open(selectedFiles[2]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x3.append(int(line.split()[0])/ 1000.0)
                        y3.append(float(line.split()[1]))
                        
        
        elif len(selectedFiles) == 6:
           with open(selectedFiles[0]) as f:
               for i, line in enumerate(f):
                   if i > 8:
                       x.append(int(line.split()[0])/ 1000.0)
                       y.append(float(line.split()[1]))
           with open(selectedFiles[1]) as f:
               for i, line in enumerate(f):
                   if i > 8:
                       x4.append(int(line.split()[0])/ 1000.0)
                       y4.append(float(line.split()[1]))
           
           with open(selectedFiles[2]) as f:
               for i, line in enumerate(f):
                   if i > 8:
                       x2.append(int(line.split()[0])/ 1000.0)
                       y2.append(float(line.split()[1]))
                       
           with open(selectedFiles[3]) as f:
               for i, line in enumerate(f):
                   if i > 8:
                       x5.append(int(line.split()[0])/ 1000.0)
                       y5.append(float(line.split()[1]))
           with open(selectedFiles[4]) as f:
               for i, line in enumerate(f):
                   if i > 8:
                       x3.append(int(line.split()[0])/ 1000.0)
                       y3.append(float(line.split()[1]))
                       
           with open(selectedFiles[5]) as f:
               for i, line in enumerate(f):
                   if i > 8:
                       x6.append(int(line.split()[0])/ 1000.0)
                       y6.append(float(line.split()[1]))
        # 4 files selected
        else:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
            with open(selectedFiles[1]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x2.append(int(line.split()[0])/ 1000.0)
                        y2.append(float(line.split()[1]))
            with open(selectedFiles[2]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        x3.append(int(line.split()[0])/ 1000.0)
                        y3.append(float(line.split()[1]))

            with open(selectedFiles[3]) as f:
                for i, line in enumerate(f):
                    if i > 8:
                        xall.append(int(line.split()[0]) / 1000.0)
                        yall.append(float(line.split()[1]))          


        plt.plot(x, y, 'r.-')
        plt.plot(x2, y2, 'y.-')
        plt.plot(x3, y3, 'g.-')
        plt.plot(x4, y4, 'y.-')
        plt.plot(x5, y5, 'kx')
        plt.plot(x6, y6, 'cx')
#        plt.plot(xall,yall, 'c')
#        plt.plot(xall,yall, linestyle = '-')
        #plt.ylim(0, 5000)
        plt.title('Voltage Sweep Over Time')
        plt.xlabel('Time(ms)')
        plt.ylabel('Voltage(mV)')
        plt.show()
        

    def mark(self):
        self.marker = True
        
    def markoff(self):
        self.marker = False
        
    def stop(self):
        global running
        self.running = False
        
    
        
    def LivePlot(self):
        
        self.running = True
        plt.ion()
        fig=plt.figure()
        i=500
        count = 1
        total670 = 0
        total850 = 0
        total950 = 0
        lenval = 0
        
        list_670, list_850, list_950 = [],[],[]
        time_670, time_850, time_950 = [],[],[]
        markers = []
        
        cont = True
        ser = serial.Serial(self.com, 38400)
        time.sleep(1)
        
        global gain_val
        gain_val = self.gain_val.get()
        
        global current_level #10,15,20,25,35
        current_level = self.current_level.get()
        
        
        if gain_val == '20':
            ser.write(b'5')
        elif gain_val == '5':
            self.ser.write(b'2')
            print("A1")
        elif gain_val == '10':
            ser.write(b'3')
            print("A2")
        elif gain_val == '15':
            ser.write(b'4')
            print("A3")
        else: # default is gain 1
            ser.write(b'1')
        
        xmin = 0
        xmax = 10000
        
        while True:
            
##            if idx % 500 == 0:
##                break
####                master.update()
                
            if self.running:
                msg = ser.readline()
                if count == 1:
                    msg = msg[1:-1]
                    count = 0
                val = msg.split()
                if val[0].decode() == "L1":
                    while cont:
                        msg = ser.readline()
                        val = msg.split()
                        if val[0].decode() == "L2":
                            plt.plot(i, total850/lenval, 'y.')
                            lenval = 0
                            if self.marker == True:
                                self.marker == False
                                plt.axvline(x = i)
                                markers.append(i)
                                self.markoff()
                                
                            break
                        if val[0].decode() != "L1":
                            total850 += float(val[1].decode())
                            time_850.append(int(val[0].decode()))
                            list_850.append(float(val[1].decode()))
                            lenval += 1
              
                if val[0].decode() == "L2":
                    while cont:

                        msg = ser.readline()
                        val = msg.split()
                        if val[0].decode() == "L3":
                            plt.plot(i, total950/lenval, 'g.')
                            lenval = 0
                            if self.marker == True:
                                self.marker == False
                                plt.axvline(x = i)
                                markers.append(i)
                                self.markoff()
                                
                            break
                        total950 += float(val[1].decode())
                        time_950.append(int(val[0].decode()))
                        list_950.append(float(val[1].decode()))
                        lenval += 1
                    
                if val[0].decode() == "L3":
                    while cont:
                        msg = ser.readline()
                        val = msg.split()
                        if val[0].decode() == "L1":
                            plt.plot(i, total670/lenval, 'r.')
                            lenval = 0
                            if self.marker == True:
                                self.marker == False
                                plt.axvline(x = i)
                                markers.append(i)
                                self.markoff()
                                
                            break

                        total670 += float(val[1].decode())
                        time_670.append(int(val[0].decode()))
                        list_670.append(float(val[1].decode()))
                        lenval += 1
                        
                        
                total670 = 0
                total950 = 0
                total850 = 0
                i += 500
                

                plt.xlim(xmin, xmax)
                if i > xmax:
                    xmin += 10000
                    xmax += 10000
                    
                plt.gca().legend(('850nm','950nm', '670nm'))    
                plt.show()
                plt.pause(0.0001)  # Note this correction
                
            else:
                break
            
        timestr = time.strftime("%Y%m%d-%H%M%S")
            
        file670 = open(final_dir+"file670_"+ current_level + "mA_gain" + gain_val + "_" + timestr + ".txt", 'w+')
        file850 = open(final_dir+"file850_"+ current_level + "mA_gain" + gain_val+ "_" + timestr + ".txt", 'w+')
        file950 = open(final_dir+"file950_"+ current_level + "mA_gain" + gain_val + timestr + ".txt", 'w+')
        
        fileList = [file670,file850,file950]
        for file in fileList:
            file.write('Important Markers: ')
            for item in markers:
                file.write(str(item) + ',')
            file.write('\t')
                
            file.write(self.add_headers())
            
        for tim in range(len(list_670)):
                line = str(time_670[tim]) + '\t' + str(list_670[tim]) + '\n'
                file670.write(line)
                
        for tim in range(len(list_850)):
                line = str(time_850[tim]) + '\t' + str(list_850[tim]) + '\n'
                file850.write(line)
                
        for tim in range(len(list_950)):
                line = str(time_950[tim]) + '\t' + str(list_950[tim]) + '\n'
                file950.write(line)
                
        print(time_670, list_670, time_850, list_850, time_950, list_950)


def main():
    root = Tk()
    root.geometry('400x600')
    my_gui = MainProgram(root)
    root.mainloop()

if __name__ == '__main__':
    main()
    

    




