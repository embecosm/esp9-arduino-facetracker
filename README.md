# Arduino/Shrimping.it Face Tracker for Linux Host

Based on the face tracking project on
[Instructables](http://www.instructables.com/id/Face-detection-and-tracking-with-Arduino-and-OpenC/),
but using a Linux host.

## Prequisites

You'll need the Arduino IDE to program the Arduino and the OpenCV computer
vision development package to do the face tracking. On Fedora/Red Hat/Centos
systems these can be installed using:

    sudo yum install Arduino
    sudo yum install opencv-devel

On Ubuntu/Debian/Linux Mint systems these can be installed using:

    sudo apt-get install Arduino
    sudo apt-get install opencv-dev

## Modifications to the standard version

The main host program now takes an argument to specify the device used for
serial communication. Typically this might be /dev/ttyACM<n> with Arduino Uno
or /dev/ttyUSB<n> with Shrimping.it. You can find out which by running

    $ udevadm monitor --udev

Then remove and reinsert the USB to which the Arduino or Shrimping.it board is
attached. The output will be something like this:

    monitor will print the received events for: UDEV - the event which
    udev sends out after rule processing
    
    UDEV  [2066308.770931] remove   /devices/pci0000:00/0000:00:1d.2/usb4/4-1/4-1:1.0/ttyUSB0/tty/ttyUSB0 (tty)
    UDEV  [2066308.771396] remove   /devices/pci0000:00/0000:00:1d.2/usb4/4-1/4-1:1.0/ttyUSB0 (usb-serial)
    UDEV  [2066308.772100] remove   /devices/pci0000:00/0000:00:1d.2/usb4/4-1/4-1:1.0 (usb)
    UDEV  [2066308.772851] remove   /devices/pci0000:00/0000:00:1d.2/usb4/4-1 (usb)
    UDEV  [2066311.957084] add      /devices/pci0000:00/0000:00:1d.2/usb4/4-1 (usb)
    UDEV  [2066311.958732] add      /devices/pci0000:00/0000:00:1d.2/usb4/4-1/4-1:1.0 (usb)
    UDEV  [2066311.959361] add      /devices/pci0000:00/0000:00:1d.2/usb4/4-1/4-1:1.0/ttyUSB0 (usb-serial)
    UDEV  [2066311.961690] add      /devices/pci0000:00/0000:00:1d.2/usb4/4-1/4-1:1.0/ttyUSB0/tty/ttyUSB0 (tty)

The last line shows that the device is connecting through /dev/ttyUSB0.

In addition the camera selected is specified as -1 (which should find any
camera), and the code uses the Eserial.h header, rather thn the original
Tserial.h header.

## Building and running the system

The Arduino code is loaded into the Aduino IDE, and from their downloaded onto
the Arduino or Shrimping.it board. **Note.** You should do this *before*
running .techbitarFaceDetection, since both require use of the serial
connection.

The face tracker is built using cmake:

    cmake .
    make

and then run on the Linux host, specifying the appropraite USB device

    ./techbitarFaceDetection /dev/ttyUSB0

## Problems

See the Embecosm [Blog Post](http://www.embecosm.com/2013/07/) by Adam,
describing his experience building this system.
