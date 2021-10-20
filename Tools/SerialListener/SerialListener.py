import serial
import argparse
import re

def printRaw(port, baud):
  ser = serial.Serial(
    port='/dev/tty.usbserial-0001',
    baudrate=9600,
  )

  while True:
    data = ser.readline()
    if data:
      str = data.decode('unicode_escape').strip("\r").strip("\n")
      print(str)

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

  printRaw(args.port, baud)
