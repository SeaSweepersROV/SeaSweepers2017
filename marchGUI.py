import Tkinter as tk
from Tkinter import *
import time
import math
from serial import *


#Setting up Serial port
#for raspberry pi use serialPort = "/dev/ttyACM0"
serialPort = "/dev/ttyACM0"
#serialPort = "/dev/cu.usbmodemFA131"
baudRate = 9600
ser = Serial(serialPort , baudRate, timeout=0, writeTimeout=0) #ensure non-blocking, code will not run if the port is not connected




class App():


	def __init__(self):
		while True:
			self.root = tk.Tk()
			self.root.title("SeaSweepers RoBoT NaMe")
			self.root.option_add("*Font", "roboto 20") #Use with MACBOOK
			#self.root.option_add("*Font", "Rockwell 15") what we used in comp
			#self.root.minsize(width=1440, height=880)
			#self.root.maxsize(width=1440, height=880)
			self.root.configure(bg ="black") #background color

        	        #make labels for data
			self.voltDataLabel = tk.Label(fg="white",bg="black",text="Volts(v)",relief= tk.GROOVE,width=17,height=2).grid(                               column=0,row=0,columnspan=1)
			self.depthLabel = tk.Label(fg="white",bg="black",text="Depth(m)",relief=tk.GROOVE,width=17,height=2).grid(                                   column=0,row=1,columnspan=1)
			self.outerTempFLabel = tk.Label(fg="white",bg="black",text="Outer Temp. (C)",relief=tk.GROOVE,width=17,height=2).grid(                       column=0,row=2,columnspan=1)
			self.innerTempFLabel = tk.Label(fg="white",bg="black",text="Inner Temp. (C)",relief=tk.GROOVE,width=17,height=2).grid(                       column=0,row=3,columnspan=1)
			self.pressureLabel = tk.Label(fg="white",bg="black",text="Pressure(mbar)",relief=tk.GROOVE,width=17,height=2).grid(                          column=0,row=4,columnspan=1)
			self.waterLeakLabel = tk.Label(fg="white",bg="black",text="Water Leak", relief=tk.GROOVE, width=27, height =2,).grid(                        column=0,row=5,columnspan=2)
		
			voltStr = tk.StringVar()
			self.voltData = tk.Label(fg="white",bg="black",text="TBD",relief= tk.GROOVE,width=10,height=2,textvariable=voltStr).grid(                    column=1,row=0,columnspan=1)	
			depthStr = tk.StringVar()
			self.depth = tk.Label(fg="white",bg="black",text="TBD",relief=tk.GROOVE,width=10,height=2,textvariable=depthStr).grid(                       column=1,row=1,columnspan=1)
			outerTempStr = tk.StringVar()
			self.outerTempF = tk.Label(fg="white",bg="black",text="TBD",relief=tk.GROOVE,width=10,height=2,textvariable=outerTempStr).grid(              column=1,row=2,columnspan=1)
			innerTempStr = tk.StringVar()
			self.innerTempF = tk.Label(fg="white",bg="black",text="TBD",relief=tk.GROOVE,width=10,height=2,textvariable=innerTempStr).grid(              column=1,row=3,columnspan=1)
			pressureStr = tk.StringVar()
			self.pressure = tk.Label(fg="white",bg="black",text="TBD",relief=tk.GROOVE,width=10,height=2,textvariable=pressureStr).grid(                 column=1,row=4,columnspan=1)
		

			data = "woo"
			print data
			voltStr.set(data)
			depthStr.set("New Text!")
			outerTempStr.set("New Text!")
			innerTempStr.set("New Text!")
			pressureStr.set("New Text!")

			self.root.mainloop()
		


app=App()
