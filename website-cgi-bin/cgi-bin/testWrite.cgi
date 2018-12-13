#!/usr/bin/env python

import cgi
import cgitb

cgitb.enable()
print("Content-Type: text/html\n")
print("hello\n")

print("howdy")

form = cgi.FieldStorage()
#print(form.keys())
#print(form)
message = form.getvalue("message", "(no message)")
lastUser = "default"
#if message != "(no message)":
lastUser = message

print(lastUser)
