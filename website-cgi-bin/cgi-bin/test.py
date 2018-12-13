#!/usr/bin/env python
import cgi
import cgitb
cgitb.enable()
print("Content-type: text/html\n")
print("Hello web")
form = cgi.FieldStorage()
print("Thank you for submitting your name!")
# Get data from fields
first_name = form.getvalue('firstname')
last_name  = form.getvalue('lastname')
with open("names.txt","a") as f:
        f.write(first_name + "," + last_name + "\n")
