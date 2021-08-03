from tkinter import *

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
        

        connectButton = Button(self, text="Connect", command=self.client_exit)
        connectButton.place(x=0, y=0)
        sendButton = Button(self, text="Send", command=e.get())
        sendButton.place(x=325, y=100)
        
        v1 = Label(self, text="Voltage 1")
        v1.place(x=50,y=150)
        v2 = Label(self, text="Voltage 2")
        v2.place(x=200,y=150)
        
        
    def client_exit(self):
        exit()
        

                
root = Tk()
root.geometry("400x300")

app = Window(root)

root.mainloop()

print("A")