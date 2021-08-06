from tkinter import *
import serial
from serial import *

class Window(Frame):
    def __init__(self, master = None):
        Frame.__init__(self, master)
        self.master = master 
        self.init_window()
        
    def init_window(self):
        
        self.master.title ("GUI")
        self.pack(fill=BOTH, expand=1)
        e = Entry(root, width=35)
        e.place(x=25, y=100)
        e.insert(0, "Agregue un valor 0-255 para el contador")        
        

        connectButton = Button(self, text="Connect", command=self.connect)
        connectButton.place(x=0, y=0)
        sendButton = Button(self, text="Send")
        sendButton.place(x=325, y=100)
        
        v_header = Label(self, text="Voltage 1               |              Voltage 2")
        v_header.place(x=50,y=150)
        
        v_values = Label(self, text=ser.read(1000))
        v_values.place(x=50, y=200)
        
    def connect(self):
        ser = serial.Serial("COM6", 9600)
        

                
root = Tk()
root.geometry("400x300")

app = Window(root)

root.mainloop()
