import serial
import argparse
import re
import os
import time
from datetime import datetime

def printRaw(port, baud):
  ser = serial.Serial(
    port=port,
    baudrate=baud,
  )

  while True:
    data = ser.readline()
    if data:
      str = data.decode('unicode_escape').strip("\r").strip("\n")
      tm = datetime.now().time()
      print("{} {}".format(tm, str))
      #os.system('clear')

if __name__ == '__main__':

  parser = argparse.ArgumentParser()
  parser.add_argument("port", help = "Serial Port")
  parser.add_argument("-b", "--baud", help = "BAUD Rate")
  args = parser.parse_args()

  if not args.port:
    print("Please specify port to listen to. Exiting ....")
    exit

  baud = 9600
  if args.baud:
    baud = args.baud

  printRaw(port=args.port, baud=baud)
