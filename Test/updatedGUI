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
#from pydrive.auth import GoogleAuth
#from pydrive.drive import GoogleDrive

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
        self.com = "/dev/cu.usbmodem141302"
        self.connected = False
        self.ser = None
        self.fname = "data.txt"
        master.title("Hydration Sensor")
#        self.time = 15

        # create buttons for GUI
#        self.img = PhotoImage(file='Drexel_Logo.gif')
#        self.img = self.img.zoom(20, 5)
#        self.img = self.img.subsample(60, 15)
#        self.img_label = Label(master, image=self.img)
#        self.img_label.pack()

        self.label = Label(master, text="Hydration Sensor")
        self.label.config(font=('Helvetica',16, 'bold'))
        self.label.pack(fill=X,pady=10)
        
        # Enter time duration first!!! and then hit log data
        time_duration = Label(master, text="Enter Time Duration for Test (in sec): ")
        time_duration.pack()
        
        self.time_duration = Entry(master)
        self.time_duration.pack()

        self.log_button = Button(master, text="Log Data", command=self.log_data, bg = 'chartreuse3',width=20)
        self.log_button.config(font=('Helvetica',12, 'bold'))
        self.log_button.pack(pady=5)
    
        self.plot_button = Button(master, text="Plot Graph", bg = 'azure',command=self.plotGraph,width=20)
        self.plot_button.config(font=('Helvetica',12, 'bold'))
        self.plot_button.pack(pady=5)
        
        self.file_select = Button(master, text="Select up to 4 Files to Plot", bg = 'orange', command=self.plotSelectedGraph, width=20)
        self.file_select.config(font=('Helvetica',12, 'bold'))
        self.file_select.pack(pady=5)

        self.close_button = Button(master, text="Close", command=master.quit, bg='orangered2', width=20)
        self.close_button.config(font=('Helvetica',12, 'bold'))
        self.close_button.pack(pady=5)

    # open serial port for connection and start the program
    def log_data(self):
        ##    try:
        print("Trying to connect to", self.com)
        self.ser = serial.Serial(self.com, 38400)
        print(self.ser)
        if self.ser is not None:
            print("Connected")
            
        # Get User Input for Length of the Test
        global time_duration
        time_duration = float(self.time_duration.get())
        self.time = time_duration

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
        n = 3
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
        file670 = open(final_dir+"file670_"+ timestr +".txt", 'w+')
        file850 = open(final_dir+"file850_"+ timestr +".txt", 'w+')
        file950 = open(final_dir+"file950_"+ timestr +".txt", 'w+')
        fileAllPoints = open(final_dir+"fileAllPoints_"+ timestr +".txt", 'w+')

        temp670 = open(final_dir+"file670.txt", 'w+')
        temp850 = open(final_dir+"file850.txt", 'w+')
        temp950 = open(final_dir+"file950.txt", 'w+')
        tempAllPoints = open(final_dir+'fileAllPoints.txt', 'w+')
        tempAvgPoint = open(final_dir+'avgAllPoint.txt', 'w+')
        tempAvg670 = open(final_dir+"tempAvg670.txt", 'w+')
        tempAvg850 = open(final_dir+"tempAvg850.txt", 'w+')
        tempAvg950 = open(final_dir+"tempAvg950.txt", 'w+')

        avg670 = open(final_dir+"avg670_" + timestr + ".txt", 'w+')
        avg850 = open(final_dir+"avg850_" + timestr + ".txt", 'w+')
        avg950 = open(final_dir+"avg950_" + timestr + ".txt", 'w+')
        avgAllPoints = open(final_dir+"avgAllPoints_" + timestr + ".txt", 'w+')

        fileList = [file670,file850,file950,fileAllPoints,tempAllPoints,tempAvg670, tempAvg850, tempAvg950, tempAvgPoint, temp670,temp850,temp950,avg670,avg850,avg950,avgAllPoints]
        for file in fileList:
            file.write(self.add_headers())

        listOfVals =[]

        f = file670
        t = temp670
        a = tempAvg670

        fh = open("datatmp.txt")
        for line in fh:

            if line == ident and is950 == True:
                prev_count = counter
                counter += 1
                # print(counter)
                # print(prevCount)
                if prev_count == 3:
                    is950 = False
                    # do sth
                    # print("Happening")
                    f = file670
                    t = temp670
                    a = tempAvg670
                    is670 = True
                    averagedVal = sum(listOfVals) / len(listOfVals)
                    avgLine = str(CycleTime) + '\t' + str(averagedVal)+ '\n'
                    avg950.write(avgLine)
                    tempAvg950.write(avgLine)
                    avgAllPoints.write(avgLine)
                    listOfVals.clear()

                    CycleTime +=CyclePeriod

                    counter = 0

            elif line == ident and is670 == True:
                #print("850")
                is850 = True
                is670 = False
                f = file850
                t = temp850
                a = tempAvg850
                averagedVal = sum(listOfVals)/len(listOfVals)
                avgLine = str(CycleTime) + '\t' + str(averagedVal) + '\n'
                avg670.write(avgLine)
                tempAvg670.write(avgLine)
                avgAllPoints.write(avgLine)
                listOfVals.clear()

            elif line == ident and is850 == True:
                #print("950")
                is950 = True
                is850 = False
                f = file950
                t = temp950
                a = tempAvg950
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
                # print(listOfVals)
                
              
        ## DONT FORGET TO ENABLE GOOGLE DRIVE API IN THE CREDENTIALS PAGE
        
#        gauth = GoogleAuth()
#        gauth.LocalWebserverAuth()
#        # need to download client_secrets.json file:
#        # Google development console https://console.developers.google.com/projectselector2/apis/credentials?supportedpurview=project
#        drive = GoogleDrive(gauth)
#        
#        filepath="/Users/anishipatel/Documents/Spring 2019/ECE 493/data.txt"
#        file1 = drive.CreateFile({'title': 'data.txt'})  # Create GoogleDriveFile instance with title 'Hello.txt'.
#        file1.SetContentFile(filepath) # Set content of the file from given string.
#        file1.Upload()

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

        line1 = now.strftime('%Y-%m-%d %H:%M')+'\n'
        line2 = "Team 18"+'\n'
        line3 = "Tyler Harris, Nicholas Joseph, Anishi Patel, Anav Pradhan"+'\n'
        line4 = 'Non-Invasive Hydration Sensing System'+'\n\n'
        line5 = 'Time(us)\tVoltage(mv)\n'

        return line1+line2+line3+line4+line5

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
        with open(final_dir+'file670.txt') as f:
            for i, line in enumerate(f):
                if i > 6:
                    x.append(int(line.split()[0]) / 1000.0)
                    y.append(float(line.split()[1]))

        with open(final_dir+'file850.txt') as f:
            for i, line in enumerate(f):
                if i > 6:
                    x2.append(int(line.split()[0]) / 1000.0)
                    y2.append(float(line.split()[1]))

        with open(final_dir+'file950.txt') as f:
            for i, line in enumerate(f):
                if i > 6:
                    x3.append(int(line.split()[0]) / 1000.0)
                    y3.append(float(line.split()[1]))

        with open(final_dir+'fileAllPoints.txt') as f:
            for i, line in enumerate(f):
                if i > 6:
                    xall.append(int(line.split()[0]) / 1000.0)
                    yall.append(float(line.split()[1]))


        plt.plot(x, y, 'rx-')
        plt.plot(x2, y2, 'yx-')
        plt.plot(x3, y3, 'gx-')
        #plt.plot(xall,yall, linestyle = '-')
        #plt.ylim(0, 5000)
        plt.title('Voltage Sweep Over Time')
        plt.xlabel('Time(ms)')
        plt.ylabel('Voltage(mV)')
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
        xall = []
        yall = []
        if len(selectedFiles) == 1:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
        elif len(selectedFiles) == 2:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
            with open(selectedFiles[1]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x2.append(int(line.split()[0])/ 1000.0)
                        y2.append(float(line.split()[1]))
        elif len(selectedFiles) == 3:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
            with open(selectedFiles[1]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x2.append(int(line.split()[0])/ 1000.0)
                        y2.append(float(line.split()[1]))
            with open(selectedFiles[2]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x3.append(int(line.split()[0])/ 1000.0)
                        y3.append(float(line.split()[1]))
        # 4 files selected
        else:
            with open(selectedFiles[0]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x.append(int(line.split()[0])/ 1000.0)
                        y.append(float(line.split()[1]))
            with open(selectedFiles[1]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x2.append(int(line.split()[0])/ 1000.0)
                        y2.append(float(line.split()[1]))
            with open(selectedFiles[2]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        x3.append(int(line.split()[0])/ 1000.0)
                        y3.append(float(line.split()[1]))

            with open(selectedFiles[3]) as f:
                for i, line in enumerate(f):
                    if i > 6:
                        xall.append(int(line.split()[0]) / 1000.0)
                        yall.append(float(line.split()[1]))          


        plt.plot(x, y, 'r-')
        plt.plot(x2, y2, 'y-')
        plt.plot(x3, y3, 'g-')
#        plt.plot(xall,yall, 'c')
#        plt.plot(xall,yall, linestyle = '-')
        #plt.ylim(0, 5000)
        plt.title('Voltage Sweep Over Time')
        plt.xlabel('Time(ms)')
        plt.ylabel('Voltage(mV)')
        plt.show()
        

def main():
    root = Tk()
    root.geometry('400x250')
    my_gui = MainProgram(root)
    root.mainloop()

if __name__ == '__main__':
    main()

