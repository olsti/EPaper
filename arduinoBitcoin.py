import serial
import time
from dataCollector import DataCollector
from pyfirmata2 import Arduino #import library from pyfirmata2 to detect Arduino
import time #time library to be able setup lenght of led lighting

comPortName = "/dev/tty.wchusbserialfa120"
dataCollector = DataCollector()
baudrate = 115200

ARDUINOSTARTMARKER = 60 #"<"
ARDUINOENDMARKER = 62 #">"
ARDUINOREADY = 'ARD_READY'

ser = serial.Serial(port=comPortName, baudrate=baudrate)
print("Serial port " + comPortName + "/ Baudrate " + str(baudrate))

def sendStrToArduino(startLabel, outputStr):
    outputStr = startLabel + outputStr;
    print("SEND Data to Arduino: "+outputStr)
    ser.write(outputStr.encode('utf-8'))

def recieveFromArduino():
    recieveStr = ""
    temp = "z"  # init with not an end- or startMarker
    byteCount = -1  # to allow for the fact that the last increment will be one too many

    # wait for the start character
    # while ord(temp) != ARDUINOSTARTMARKER:
    #     temp = ser.read()
    #
    # save data until the end marker is found
    # while ord(temp) != ARDUINOENDMARKER:
    #     if ord(temp) != ARDUINOSTARTMARKER:
    #         recieveStr = recieveStr + temp.decode("utf-8")  # change for Python3
    #         byteCount += 1
    #     temp = ser.read()
    recieveStr = str(ser.read_all())
    print("RECIEVE " + recieveStr)
    return (recieveStr)

def waitForArduino():
    # waits until arduino send ready
    print("Wait until Ready")
    recievedMessage = ""
    while recievedMessage.find(ARDUINOREADY) == -1: #find arduinoready string
        while ser.inWaiting() == 0:
            pass
        recievedMessage = recieveFromArduino()
    print("Wait until Ready DONE")

def updateCycle():
    lastBTCPriceUsd = dataCollector.get_latest_crypto_price('btc')
    print(lastBTCPriceUsd)
    sendStrToArduino("[BTC]", lastBTCPriceUsd+"$");

def main():
    waitForArduino()
    while True:
        updateCycle()
        waitForArduino()

main()
