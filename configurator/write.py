from vnpy import *
ez = EzAsyncData.connect('/dev/ttyUSB0', 115200)

ez.sensor
# ez.sensor.write_