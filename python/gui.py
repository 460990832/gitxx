# -*- coding: utf-8 -*-
"""
Created on Sat Mar 26 22:45:17 2016

@author: 46099
"""

import sys
import time
from PyQt4 import uic,QtCore,QtGui
import bluetooth
import threading
class Find(threading.Thread):
    def __init__(self,object):
        threading.Thread.__init__(self)
        self.thread_stop = False
    def run(self):
        while not self.thread_stop:
            nearby_devices = bluetooth.discover_devices(lookup_names=True)
            for addr, name in nearby_devices:
                print addr + ' ' + name
                object.text.appendPlainText(addr+' '+name)
#            print "Test"
#            time.sleep(1)
    def stop(self):
        self.thread_stop = True
class Demo(QtGui.QWidget):
    def __init__(self,parent=None):
        QtGui.QWidget.__init__(self,parent)
        self.resize(500,500)
        self.setWindowTitle('Demo')
        self.scan=QtGui.QPushButton('scan',self)
        self.scan.setGeometry(30,30,80,40)
        self.text=QtGui.QPlainTextEdit(self)
        self.text.setGeometry(30,100,400,300)
#        self.connect(self.scan, QtCore.SIGNAL('clicked()'),self.asdf)
        self.dxc=Find(self);
        self.dxc.setDaemon(True)
        self.connect(self.scan,QtCore.SIGNAL('clicked()'),self.dxc.start)
    def asdf(self):
        nearby_devices = bluetooth.discover_devices(lookup_names=True)
        for addr, name in nearby_devices:
            self.text.appendPlainText(addr+' '+name)
#        self.connect(scan,QtCore.SIGNAL('clicked()'),QtCore.SLOT('startscan()')
#        self.scan.click.connect(self.startscan)
app=QtGui.QApplication(sys.argv)
demo=Demo()
demo.show()
app.exec_()
