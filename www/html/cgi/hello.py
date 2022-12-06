#!/usr/bin/python3

import cgi, cgitb 
form = cgi.FieldStorage() 

first_name = form.getvalue('fname')
last_name  = form.getvalue('lname')


print ("""<html><body><p>your name is : %s, %s</p></body></html>"""%(first_name, last_name));
