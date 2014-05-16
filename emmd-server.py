##########################################################################
# Copyright 2014 A - Team
# Authors: Darshana Adikari, Yasinda Nimalasuriya, Nipuna Wickramanayake
#          Mihiravi Samaranayake, Malaka Sirinimal
#
# Web server & Serial port bridge for Autonomous boat
#
#
##########################################################################


import time
import serial
import serial.tools.list_ports
import socket
import http.server
import socketserver
import cgi
import json
import threading
#import fileinput
from os import curdir, sep

serialPortName="/dev/ttyACM0"
PORT = 8080

def sendPostResponce(handler,JSONData):
    
    handler.send_response(200)
    handler.send_header('Content-Type', 'application/json')
    handler.end_headers()
    handler.wfile.write(json.dumps(JSONData).encode('utf_8','ignore'))


class ServerHandler( http.server.BaseHTTPRequestHandler):

  
  def do_GET(self):
    print(self.path)
    if self.path=="/":
      self.path="index.html"

    try:
      sendReply = False
      if self.path.endswith(".html"):
        mimetype='text/html'
        sendReply = True
      if self.path.endswith(".jpg"):
        mimetype='image/jpg'
        sendReply = True
      if self.path.endswith(".png"):
        mimetype='image/png'
        sendReply = True
      if self.path.endswith(".gif"):
        mimetype='image/gif'
        sendReply = True
      if self.path.endswith(".js"):
        mimetype='application/javascript'
        sendReply = True
      if self.path.endswith(".css"):
        mimetype='text/css'
        sendReply = True

      if sendReply == True:
              #Open the static file requested and send it
        f = open(curdir + sep + self.path ,'rb') 
        self.send_response(200)
        self.send_header('Content-type',mimetype)
        self.end_headers()
        self.wfile.write(f.read())
        f.close()
      return


    except IOError:
      self.send_error(404,'File Not Found: %s' % self.path)

  def do_POST(self):
      output1=""
      print(self.headers)
      form = cgi.FieldStorage(
          fp=self.rfile,
          headers=self.headers,
          environ={'REQUEST_METHOD':'POST',
                    'CONTENT_TYPE':self.headers['Content-Type'],
                    })
      
      if(form.getlist("gps")):
 
        serialRunner.dataWrite("gps\n")
        time.sleep(1)
        output1=serialRunner.outputBuffer
        print (output1)
        z=sorter.getDataSerial(output1)
        
        value=form.getlist("gps")
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        
        #z= {'time':20434,"lattitude":1404.0,"longitude":10036.0}
        self.wfile.write(json.dumps(z).encode('ascii','ignore'))
        
        
      elif(form.getlist("heading")):
        value=form.getlist("heading")
        serialRunner.dataWrite("mag\n")
        time.sleep(1)
        output1=serialRunner.outputBuffer
        print (output1)
        z=sorter.getDataSerial(output1)
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        #z= {'time':20434,"lattitude":1404.0,"longitude":10036.0}
        self.wfile.write(json.dumps(z).encode('ascii','ignore'))
      
      elif(form.getlist("marker")):
        value=form.getlist("marker")
      
        markers=[]
        markers.append(form.getlist("markernumber")[0])
        markers.append(form.getlist("lattitude")[0])
        markers.append(form.getlist("longitude")[0])
        
        markerString="mar,"+",".join(markers)+"\n"
        serialRunner.dataWrite(markerString)
        
        time.sleep(0.5)
        output1=serialRunner.outputBuffer
        #print (output1)
        z=sorter.getDataSerial(output1)
        
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        #z= {'time':20434,"lattitude":1404.0,"longitude":10036.0}
        self.wfile.write(json.dumps(z).encode('ascii','ignore'))        
      elif(form.getlist("start")):
        serialRunner.dataWrite("start\n")
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        #z= {'time':20434,"lattitude":1404.0,"longitude":10036.0}
        self.wfile.write(json.dumps({"start":"done"}).encode('ascii','ignore'))        
      else:
        self.send_response(204)
      
        self.end_headers()
        
        #http.server.SimpleHTTPRequestHandler.do_GET(self)
  

class MyTCPServer(socketserver.TCPServer):
  def server_bind(self):
    self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    self.socket.bind(self.server_address)

class DataSorter:

  def isXORChecksumPass(self,targetString,checksum):
    calc_cksum = 0
    for s in targetString:
      calc_cksum ^= ord(s)
    #print(str(ord(s))+"p "+str(int(checksum)))
    print(format(calc_cksum,'02x')+" and "+checksum)
    return(format(calc_cksum,'02x')==checksum)

  def getDataSerial(self,data):
    bufList=[]
    bufLength=0
    buf2=""
    outputJSON={}
    print("data :"+str(data))
    if data:
      
      bufList=str(data).split('$') #message start with $, end with *   
      if(bufList[0]==''):
        bufList.pop(0)
        
      else:
        bufList[0]=buf2+bufList[0] #attach remainder piece of previous buffer to this one *** risky to use this piece of data!!!***
        
      bufLength=len(bufList)
      #if(bufLength==1 and bufList[0][-1]
      
      for i in range(0,bufLength):
     
        pp=bufList[i]
        print("yesdata"+pp)
        starIndex=bufList[i].find('*')
        pp=pp[:starIndex+1]
        if(len(pp)<2):
          continue
        #pp="".join(pp)
        print(pp[-3:-1])
        print(self.isXORChecksumPass(pp[:-4],pp[-3:-1]))
        if(pp[-1]=='*' and self.isXORChecksumPass(pp[:-4],pp[-3:-1])):# and pp[-4]==':' and int(pp[-3:])==len(pp[
          fields=pp[:-4].split(',')
          
          if(fields[0]=="GPCO"):#gps coordinates, seperated by comma $GPCO,TIME,12332,1332* -> 234 is message length
            print(fields[0])
            outputJSON.update({'time':fields[1],'lattitude':fields[2],'longitude':fields[3]})
            
          elif(fields[0]=="HEAD"):#heading
            print({'heading':fields[1],"distance":fields[2],"pos":fields[3]})
            
            outputJSON.update( {'heading':fields[1],"distance":fields[2],"pos":fields[3]})
          elif(fields[0]=="MARK"): #error!!! :o
            print("success")
            print(fields[1])
            outputJSON.update( {'mark':fields[1]})
          #elif(pp[1:4]=="poin"): #active point 0 index
            
          #elif(pp[1:4]=="gpla"): #
            
          #elif(pp[1:4]=="gpla"):
          else: #default
            gpio=pp
            
        elif(i==(bufLength-1) and pp[-1]!='*'):
          print("added to buf2")
          buf2=pp
        else:
          print("blabla:"+pp)
          continue
        
        bufferedLine=""
      #else:
        #print "-_-"
        #print bufferedLine
        ##s.send(bufferedLine)
        #bufferedLine=""
        ##print "bla",bufferedLine
    else:
      outputJSON.update( {"fail":"true"})
      print("read fail")
    return outputJSON

##########################################################################################################
class SerialHandler:
  
  def getPortList(self):
   
    portList=list(serial.tools.list_ports.comports())
    portList.sort()
    return portList
  
  def __init__(self):
    self.outputBuffer=""
    self.ser=serial.Serial()
    self.dataReady=False
    self._reader_alive = False
    self._port_open=False
    self.receiver_thread=threading.Thread()
    
  def getOutput(self):
    if(self.dataReady):
      self.dataReady=False
      return str(self.outputBuffer)
    else:
      return 0
  
   
  def setPort(self,port,baud, timeOut=50):
    
    try:
      self.ser=serial.Serial(port,baud,timeout=timeOut)
      self._port_open=self.ser.isOpen()
      print(self._port_open)
        
    except serial.SerialException as e:
      print(e)
      pass
    
  def read_from_port(self):
    try:
      while self._port_open and self._reader_alive:
        #print("loopy")
        reading = self.ser.readline().decode('ascii','ignore').rstrip('\r\n')
        starIndex=reading.find('*')
        dollarIndex=reading.find('$')
        if(reading and (starIndex!=-1) and (dollarIndex!=-1) and (starIndex>dollarIndex)):
          self.dataready=True
          self.outputBuffer=reading[dollarIndex:starIndex+1]
          self.ser.flushInput()
          #print(self.outputBuffer)
          
    except serial.SerialException as e:
      print(e)
      self._reader_alive = False
      pass
    except serial.SerialTimeoutException:
      print("Timeout")
      pass
  
  def dataWrite(self,data):
    try:
      if(self._port_open):
        self.ser.write(data.encode('ascii','ignore'))
        
    except serial.SerialException as e:
      print(e)
      self._reader_alive = False
      pass
    except serial.SerialTimeoutException:
      print("Timeout")
      pass    
        
  def start(self):
    """Start reader thread"""
    self._reader_alive = True
    # start serial->console thread
    self.receiver_thread = threading.Thread(target=self.read_from_port)
    #self.receiver_thread.setDaemon(True)
    self.receiver_thread.start()
    print("thread init")

  def stop(self):
    """Stop reader thread only, wait for clean exit of thread"""
    self._reader_alive = False
    self.receiver_thread.join()
    self.ser.close()
    print("thread stop")
##########################################################################################################

if __name__ == "__main__":
  serialRunner=SerialHandler()
  sorter=DataSorter()
  try:
    
    Handler = ServerHandler
    httpd = MyTCPServer(("localhost", PORT), Handler)
    
  except KeyboardInterrupt:
    print('^C received, shutting down the web server')
    httpd.socket.close()
    serialRunner.stop()
    
  try:
    
    portList=serialRunner.getPortList()
    for po in range(0,len(portList)): 
      print(str(po)+". : "+portList[po][0])
    mode=int(input('Select Port Number :'))
    if(mode <len(portList) and mode>=0):
      serialPortName=portList[mode][0]
  except ValueError:
    print("Not a number")
  try:
    serialRunner.setPort(serialPortName,115200,1)
    serialRunner.start()
    
    print("serving at port", PORT)
    httpd.serve_forever()

  except serial.SerialException as e:
    print(e)
    pass
