#!/usr/bin/env python

import cgi
import cgitb
cgitb.enable()
#import os

print("Content-Type: text/html\n")

try:
  with open("messages.txt") as f:
    lines = f.readlines()
    lines = [x[:-1] for x in lines] # remove newlines
    print("Last message:'{}'".format(lines[-1]))
except FileNotFoundError:
    print("No messages")
