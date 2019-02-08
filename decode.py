#!/usr/bin/env python3
import sys
import __future__

def decode(data):
    if len(data) != 16:
        raise Exception("Bad data length, 16 characters expected")

    header = int(data[0:2], 16)

    temperature = int(data[4:6] + data[2:4], 16) if header & 0x01 else None

    if temperature is not None:
        if temperature > 32768:
            temperature -= 65536
        temperature /= 2.0
        temperature = float(temperature)

    return {
        "temperature": temperature,
        "humidity": float(int(data[6:8], 16) / 2.0) if header & 0x02 else None,
        "illuminance": float(int(data[10:12] + data[8:10], 16) * 2) if header & 0x04 else None,
        "pressure": float(int(data[14:16] + data[12:14], 16) * 2) if header & 0x08 else None,
    }


def pprint(data):
    print('Temperature :', data['temperature'])
    print('Humidity :', data['humidity'])
    print('Illuminance :', data['illuminance'])
    print('Pressure :', data['pressure'])


if __name__ == '__main__':
    if len(sys.argv) != 2 or sys.argv[1] in ('help', '-h', '--help'):
        print("usage: python3 decode.py [data]")
        print("example: python3 decode.py 0f090072150048be")
        exit(1)

    data = decode(sys.argv[1])
    pprint(data)
