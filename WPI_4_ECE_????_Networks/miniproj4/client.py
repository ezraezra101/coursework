import socket   #for sockets
import sys      #for exit

HOST = 'localhost';
PORT = 2305;

#create an INET, STREAMing socket
try:
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error:
  print 'Failed to create socket'
  sys.exit()

print 'Socket Created'

try:
  remote_ip = socket.gethostbyname(HOST)

except socket.gaierror:
  #could not resolve
  print 'Hostname could not be resolved. Exiting'
  sys.exit()

#Connect to remote server
s.connect((remote_ip , PORT))

print 'Socket Connected to ' + HOST + ' on ip ' + remote_ip

#Send some data to remote server

import atexit
atexit.register(s.close) # Quit gracefully

while 1:

  message = raw_input('Enter message ')

  try :
    #Set the whole string
    s.sendall(message)
  except socket.error:
    #Send failed
    print 'Send failed'
    sys.exit()

  print 'Message sent successfully'

  #Now receive data
  reply = s.recv(4096)


  print 'Server reply: ', reply
