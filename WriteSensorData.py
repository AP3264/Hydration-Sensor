from tkinter import Tk, Label, Button
import matplotlib.pyplot as plt
import serial
import time
import os


class MainProgram:

    com = "com8"
    connected = False
    ser = None
    fname = "data.txt"

    def __init__(self, master):
        self.master = master
        self.com = "com4"
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

    def greet(self):
        print("Greetings!")


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
        # isPattern = False
        file670 = open("file670.txt", 'w+')
        file850 = open("file850.txt", 'w+')
        file950 = open("file950.txt", 'w+')
        f = file670

        fh = open("datatmp.txt")
        for line in fh:

            if line == ident and is950 == True:
                prev_count = counter
                counter += 1
                print(counter)
                # print(prevCount)
                if prev_count == 3:
                    is950 = False
                    # do sth
                    print("Happening")
                    f = file670
                    is670 = True
                    counter = 0
                    # prevCount = 0

            #        elif line == ident and isPattern == True:
            #            print("670")
            #            is670 = True
            #            isPattern = False
            #            f = file670

            elif line == ident and is670 == True:
                print("850")
                is850 = True
                is670 = False
                f = file850

            elif line == ident and is850 == True:
                print("950")
                is950 = True
                is850 = False
                f = file950

            else:
                f.write(line)

        fh.close()


    def log_data(self):
        ##    try:
        print("Trying to connect to", self.com)
        self.ser = serial.Serial(self.com, 38400)
        print(self.ser)
        if self.ser is not None:
            print("Connected")

        self.save_result(self.ser, self.fname, 60)
        self.remove_lines(self.fname)
        self.save_wave()

    def plotGraph(self):
        with open('file670.txt') as f:
            lines = f.readlines()
            x = [int(line.split()[0]) / 1000 for line in lines]
            y = [float(line.split()[1]) for line in lines]
        with open('file850.txt') as f:
            lines = f.readlines()
            x2 = [int(line.split()[0]) / 1000 for line in lines]
            y2 = [float(line.split()[1]) for line in lines]
        with open('file950.txt') as f:
            lines = f.readlines()
            x3 = [int(line.split()[0]) / 1000 for line in lines]
            y3 = [float(line.split()[1]) for line in lines]

        plt.plot(x, y, 'ro')
        plt.plot(x2, y2, 'yx')
        plt.plot(x3, y3, 'gx')
        plt.show()

root = Tk()
root.geometry('200x200')
my_gui = MainProgram(root)
root.mainloop()
