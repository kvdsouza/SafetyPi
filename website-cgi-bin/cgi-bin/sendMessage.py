#!/usr/bin/env python

import cgi
import cgitb
cgitb.enable()

print("Content-Type: text/html\n")

form = cgi.FieldStorage()

message = form.getvalue('message')

if message:
  with open("messages.txt", "w") as f:
    f.write(message+'\n')
  print("Message saved: '{}'".format(message))
else:
  print("No message to save sadly")
