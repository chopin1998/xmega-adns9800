#!/usr/bin/python


import sys
import time
import serial

from PyQt4 import QtCore, QtGui
import Image, ImageQt


class ADNS3080( object ):

    def __init__(self, dev='/dev/ttyACM0', baud=921600):

        self.dev = serial.Serial(dev, baud)
        self.dev.setTimeout(0.1)
        self.dev.flush()


    def capture(self):

        self.dev.flush()
        self.dev.write('[capture]')

        try:
            rev = self.dev.read(900)
        except Exception as ex:
            return (False, ex)

        return (True, rev)

    def motion(self):

        self.dev.flush()
        self.dev.write('[motion]')

        try:
            rev = self.dev.readline()
        except Exception as ex:
            return (False, ex)

        return (True, rev.strip())


class CapGui( QtGui.QWidget ):

    def __init__(self):
        self.adns3080 = ADNS3080()
        
        QtGui.QWidget.__init__(self)

        self.img_label = QtGui.QLabel(self)
        self.img_label.resize(600, 600)

        self.btn = QtGui.QPushButton(self)
        self.btn.clicked.connect(self.click)

        timer = QtCore.QTimer(self)
        timer.timeout.connect(self.show_img)
        timer.start(40)

    def click(self):
        print 'click'

    def show_img(self):

        if (1):
            data = self.adns3080.capture()
            if data[0]:
                #print 'ok'
                img = Image.new('L', (30, 30))
                img.frombytes(data[1])
                img = img.resize((600, 600))
                img = ImageQt.ImageQt(img)
                
                self.img_label.setPixmap(QtGui.QPixmap.fromImage(img))
                #self.img_label.setPixmap(QtGui.QPixmap.fromString(img.tobytes()))
                self.img_label.show()
            else:
                print 'failed to capture frame'
                
        else:
            data = self.adns3080.motion()
            if data[0]:
                print data[1]
            else:
                print 'failed to get motion info'
    

#################################3

if __name__ == '__main__':

    #s = ImageSource()
    #print s.capture()
    app = QtGui.QApplication(sys.argv)

    w = CapGui()
    w.show()

    app.exec_()
