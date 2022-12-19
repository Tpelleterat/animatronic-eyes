#!/usr/bin/env python3

import time
from picamera import PiCamera

def testCamera():
    camera = PiCamera()

    camera.start_preview()
    time.sleep(5)
    camera.stop_preview()


def main():
    testCamera()

if __name__ == '__main__':
    main()