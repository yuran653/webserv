#!/usr/local/bin/python3.9

import cgi

form = cgi.FieldStorage()

if "text" in form:
	src_txt = form.getvalue("text")
else:
	src_txt = ""

# CGI response
print("Content-type: text/html\r\n\r\n")
print(src_txt)