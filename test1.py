import tkinter
from tkinter import *
from tkinter import filedialog as fd
from tkinter import ttk  # Import Treeview from ttk module


# Function to open the data from the text file
def open_txt():
    global file_path
    file_path = fd.askopenfilename(filetypes=[("Text Files", "*.txt")])
    print("Selected file:", file_path)  # Print selected file path for debugging
    if file_path:
        global text_file
        text_file = open(file_path, 'r', encoding='utf-8')
        stuff = text_file.read()
        my_text.insert(END, stuff)

# Function to display the data from the last line, in the table
def display_table():
    table.delete(*table.get_children())


    contents = []
    f = open(file_path, 'r', encoding='utf-8')
    for line in f:
        pass
        last_line = line
    contents.append(last_line.split())

    if last_line:
        values = last_line.split(',')
        date, time, *prod_values = values[0], values[1], *values[2:]


    for i, prod_value in enumerate(prod_values):
        table_row = [str(i + 1), prod_value, date, time]
        table.insert("", "end", values=table_row,)

# Function which prints the data from the last line in a sorted way
def sort_table():
    table.delete(*table.get_children())

    contents = []

    f = open(file_path, 'r', encoding='utf-8')
    for line in f:
        last_line = line
    contents.append(last_line.split())

    if last_line:
        values = last_line.split(',')
        date, time, *prod_values = values[0], values[1], *values[2:]

    sorted_prod_values = sorted(prod_values, key=int)

    for i, sorted_prod_value in enumerate(sorted_prod_values):
        table_row = [str(i + 1), sorted_prod_value, date, time]
        table.insert("", "end", values=table_row)

    f.close()


# Function for removing the file
def remove_file():
    my_text.delete(1.0, END)  # Clear the text widget

# Function that clears the table
def clear_table():
    table.delete(*table.get_children())

# Root
root = Tk()
root.title('Lista ta de cumparaturi')
root.geometry("1000x600")

# Setting attribute
width= root.winfo_screenwidth()
height= root.winfo_screenheight()

# Setting tkinter window size
root.geometry("%dx%d" % (width, height))

# Style
style = ttk.Style()
style.theme_use("clam")
style.configure("Treeview",
                background="#c46b46",
                foreground="black",
                rowheight=25,
                fieldbackground="#c46b46")
style.map('Treeview',
                background=[('selected', '#653a27')])

# Adding image file
bg = PhotoImage(file="Images/Supermarket5.png")

#Label for image

my_label = Label(root, image=bg)
my_label.place(x=0, y=0, relwidth=1, relheight=1)



# Table frame
table_frame = Frame(root)
table_frame.pack(pady=200)


# Treeview widget 
table = ttk.Treeview(table_frame, columns=("Nr.crt.", "Prod", "Data", "Ora"), show="headings")
table.heading("Nr.crt.", text="Nr.crt.",anchor=CENTER)
table.heading("Prod", text="Prod",anchor=CENTER)
table.heading("Data", text="Data",anchor=CENTER)
table.heading("Ora", text="Ora",anchor=CENTER)


#Table columns
table.pack()

my_text = Text(root, width=40, height=10, font=("Helvetica", 18), bg="#c46b46")
my_text.pack(pady=20)


# Buttons area
open_button = Button(root, text='Import', font=("Helvetica", 12), height=2, width=20, bg="#1e81b0", command=open_txt)
open_button.pack(pady=30)
open_button.place(x=80, y=250)

display_button = Button(root, text='Export', font=("Helvetica", 12), height=2, width=20, bg="#1e81b0", command=display_table)
display_button.pack(pady=30)
display_button.place(x=80, y=350)

sort_button = Button(root, text='Sort Table', font=("Helvetica", 12), height=2, width=20, bg="#1e81b0", command=sort_table)
sort_button.pack(pady=30)
sort_button.place(x=80, y=450)

remove_button = Button(root, text='Remove File', font=("Helvetica", 12), height=2, width=20, bg="#1e81b0", command=remove_file)
remove_button.pack(pady=30)
remove_button.place(x=1650, y=250)

clear_button = Button(root, text="Clear Table", font=("Helvetica", 12), height=2, width=20, bg="#1e81b0", command=clear_table)
clear_button.pack(pady=30)
clear_button.place(x=1650, y=350)


root.mainloop()
