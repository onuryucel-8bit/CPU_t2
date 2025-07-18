import tkinter as tk
from tkinter.font import Font

import os

root = tk.Tk()

root.title("IDE CPU_t0")
#window size
root.geometry("1000x700")
font = Font(family="Courier", size=10)

#------------------------------------------#

def zoom(size):
    font.configure(size=size)

def saveFile():
    source = textbox.get("1.0", "end-1c")

    with open("program.txt", "w") as file:
        file.write(source)
    

def runFile():
    saveFile()
    #arg1 input 
    #arg2 output
    os.system("AssemblerCPU_t2.exe program.txt out")
    

def runEmu():
    saveFile()
    #arg1 input     
    os.system("PatlicanCPU.exe out.bin")
    pass

def openFile():
    with open("program.txt", "r") as source:
        contents = source.read()
        textbox.delete("1.0", "end")
        textbox.insert("1.0", contents)

#------------------------------------------#

#run button
runButton = tk.Button(root, text="Run", bg ="#5ff366")
runButton.config(command= runFile)

#save button
saveButton = tk.Button(root, text="Save")
saveButton.config(command=saveFile)

#open button
openButton = tk.Button(root, text="Open")
openButton.config(command=openFile)

#runEMU button
runEmuButton = tk.Button(root, text="RunEMU")
runEmuButton.config(command=runEmu)

#scale
zoom_scale = tk.Scale(root, orient='vertical', from_=20, to=100)
zoom_scale.config(command=zoom)

#textbox
textbox = tk.Text(root, font=font, bg="#b17474", undo=True)

zoom_scale.pack(fill='y', side='right')
textbox.place(x=0, y=40, width=900, height=680)

runButton.place(x=0, y=0, width= 80, height= 40)
saveButton.place(x=80, y=0, width= 80, height= 40)
runEmuButton.place(x=160, y=0, width= 80, height= 40)
openButton.place(x=160 + 80, y=0, width= 80, height= 40)
#------------------------------------------#



zoom_scale.set(10)

root.mainloop()