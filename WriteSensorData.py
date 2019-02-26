#Drexel University - Senior Design
#Hydration Sensor - Team 18
#Anav Pradhan, Anishi Patel, Tyler Harris, Nicholas Joseph

from tkinter import Tk, Label, Button
import matplotlib.pyplot as plt
import serial
import time
import os
import datetime

now = datetime.datetime.now()

current_dir = os.getcwd()
final_directory = os.path.join(current_dir, r'Files')
if not os.path.exists(final_directory):
    os.makedirs(final_directory)

final_dir = final_directory + "\\"

class MainProgram:

    def __init__(self, master):
        self.master = master
        self.com = "com8"
        self.connected = False
        self.ser = None
        self.fname = "data.txt"
        master.title("A simple GUI")

        self.label = Label(master, text="Hydration Sensor!")
        self.label.pack()

        self.greet_button = Button(master, text="Log Data", command=self.log_data)
        self.greet_button.pack()

        self.plot_button = Button(master, text="Plot Data", command=self.plotGraph)
        self.plot_button.pack()

        self.close_button = Button(master, text="Close", command=master.quit)
        self.close_button.pack()


    def save_result(self, serial, file_name, time_val=5):
        val = True
        text_file = open(file_name, 'w+')

        print(serial.readable())
        start = time.time()
        print(start)

        while val is True:
            msg = str(serial.readline())
            msg = msg[2:-1]
            msg = msg.replace('\\r\\n', '\n')
            msg = msg.replace('\\t', '\t')
            # print(msg)
            text_file.write(msg)
            end = time.time()

            if (end - start) >= time_val:
                print("done")
                val = False

        text_file.close()
        serial.close()

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

    def save_wave(self):


            
        counter = 0
        prev_count = 0
        ident = " \n"
        is670 = True
        is850 = False
        is950 = False

        CycleTime = 500
        CyclePeriod = 500

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

        avg670 = open(final_dir+"avg670_" + timestr + ".txt", 'w+')
        avg850 = open(final_dir+"avg850_" + timestr + ".txt", 'w+')
        avg950 = open(final_dir+"avg950_" + timestr + ".txt", 'w+')
        avgAllPoints = open(final_dir+"avgAllPoints_" + timestr + ".txt", 'w+')

        fileList = [file670,file850,file950,fileAllPoints,tempAllPoints,temp670,temp850,temp950,avg670,avg850,avg950,avgAllPoints]
        for file in fileList:
            file.write(self.add_headers())


        listOfVals =[]

        f = file670
        t = temp670

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
                    is670 = True
                    averagedVal = sum(listOfVals) / len(listOfVals)
                    avgLine = str(CycleTime) + '\t' + str(averagedVal)+ '\n'
                    avg950.write(avgLine)
                    avgAllPoints.write(avgLine)
                    listOfVals.clear()

                    CycleTime +=CyclePeriod

                    counter = 0

            elif line == ident and is670 == True:
                print("850")
                is850 = True
                is670 = False
                f = file850
                t = temp850
                averagedVal = sum(listOfVals)/len(listOfVals)
                avgLine = str(CycleTime) + '\t' + str(averagedVal) + '\n'
                avg670.write(avgLine)
                avgAllPoints.write(avgLine)
                listOfVals.clear()

            elif line == ident and is850 == True:
                print("950")
                is950 = True
                is850 = False
                f = file950
                t = temp950
                averagedVal = sum(listOfVals) / len(listOfVals)
                avgLine = str(CycleTime) + '\t' + str(averagedVal)+ '\n'
                avg850.write(avgLine)
                avgAllPoints.write(avgLine)
                listOfVals.clear()

            else:
                f.write(line)
                t.write(line)
                fileAllPoints.write(line)
                tempAllPoints.write(line)

                lineVal = line.rstrip('\n')
                listOfVals.append(float(lineVal.split("\t")[1]))
                # print(listOfVals)

        fh.close()


    def add_headers(self):

        line1 = now.strftime('%Y-%m-%d %H:%M')+'\n'
        line2 = "Team 18"+'\n'
        line3 = "Tyler Harris, Nicholas Joseph, Anishi Patel, Anav Pradhan"+'\n'
        line4 = 'Non-Invasive Hydration Sensing System'+'\n\n'

        return line1+line2+line3+line4


    def log_data(self):
        ##    try:
        print("Trying to connect to", self.com)
        self.ser = serial.Serial(self.com, 38400)
        print(self.ser)
        if self.ser is not None:
            print("Connected")

        self.save_result(self.ser, self.fname, 10)
        self.remove_lines(self.fname)
        self.save_wave()

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
                if i > 5:
                    x.append(int(line.split()[0]) / 1000.0)
                    y.append(float(line.split()[1])*1000)

        with open(final_dir+'file850.txt') as f:
            for i, line in enumerate(f):
                if i > 5:
                    x2.append(int(line.split()[0]) / 1000.0)
                    y2.append(float(line.split()[1])*1000)

        with open(final_dir+'file950.txt') as f:
            for i, line in enumerate(f):
                if i > 5:
                    x3.append(int(line.split()[0]) / 1000.0)
                    y3.append(float(line.split()[1])*1000)

        with open(final_dir+'fileAllPoints.txt') as f:
            for i, line in enumerate(f):
                if i > 5:
                    xall.append(int(line.split()[0]) / 1000.0)
                    yall.append(float(line.split()[1])*1000)


        plt.plot(x, y, 'rx')
        plt.plot(x2, y2, 'yx')
        plt.plot(x3, y3, 'gx')
        plt.plot(xall,yall, linestyle = '-')
        plt.ylim(0, 500)
        plt.show()

root = Tk()
root.geometry('200x200')
my_gui = MainProgram(root)
root.mainloop()
