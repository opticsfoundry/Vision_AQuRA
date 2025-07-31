#!/usr/bin/env python3

import socket
import threading
import numpy as np
from pypylon import pylon, genicam
from time import sleep
import sys


hbin = 0
vbin = 0
xstart = 0
xmax = 0
ystart = 0
ymax = 0
width = 0
height = 0
ExposureTime = 0#float
Temperature = 0#float can be used as gain
ExternalTrigger = 0
vSpeed = 0
hSpeed = 0#float (brigtness)
FKSMode = 0
FKSHeight = 0
FKSDataImages = 0
FKSReferenceImages = 0
FKSvSpeed = 0
FKSExternalTriggerStart = 0
FKSDirtyLines = 0
NumberPictures = 0
PrePicture = 0
FlushWait = 0
DoInternaltriggerAfterExternalTrigger = 0
NumberOfAdditionalReferncePictures = 0


def get_char(connection):
    try:
        message = connection.recv(1)
        if len(message)>0:
            if (message[0] <= 31) or (message[0] >= 127):
                return False, " "
            else:
                return True, str(message, 'UTF-8')
        else:
            return False, ""
    except socket.error:
        pass
        #print ("connection lost")
    except BlockingIOError:
        pass
    return False, ""

def read_string(connection):
    reading = True
    read = ""
    while (reading):
        success, ret = get_char(connection)
        if success:
            if ret != "\n" and ret != "\r\n" and ret != "\n\r":
                read = read + ret
            else:
                reading = False
    return read

def get_command(connection):
    reading = True
    command_detected = False
    first_time = True
    read = ""
    success = False
    while (reading):
        success, ret = get_char(connection)
        if success:
            if command_detected:
                if ret != "#":
                    read = read + ret
            if ret == "*":
                command_detected = True
            if ret == "#":
                reading = False
        else:
            if first_time:
                reading = False
        first_time = False
    return success, read

def get_int(connection):
    success = False
    command = []
    while not success:
        success, command = get_command(connection)
    if success:
        return int(command)
    else:
        print("Error")
        return None

def get_float(connection):
    success = False
    command = []
    while not success:
        success, command = get_command(connection)
    if success:
        return float(command)
    else:
        print("Error")
        return None

def wait_for_ready(connection):
    success = False
    command = []
    while not success:
        success, command = get_command(connection)
    if success:
        return command.upper() == "READY"
    else:
        print("Error")
        return False

def extract_ip():
    st = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:       
        st.connect(('10.255.255.255', 1))
        IP = st.getsockname()[0]
    except Exception:
        IP = '127.0.0.1'
    finally:
        st.close()
    return IP

def initialize_cameras():
    try:
        tlFactory = pylon.TlFactory.GetInstance()
        devices = tlFactory.EnumerateDevices()
        if len(devices) == 0:
            raise RuntimeError("No camera present.")

        print("Number of detected cameras:", len(devices))
        cameras = pylon.InstantCameraArray(len(devices))
        for i, device in enumerate(devices):
            #cameras[i] = pylon.InstantCamera(tlFactory.CreateDevice(device))
            print(f"Camera {i}: Model {device.GetModelName()} : Serial {device.GetSerialNumber()} :  Version {device.GetDeviceVersion()} : IP:Port {device.GetAddress()} : MAC {device.GetMacAddress()}")
            #print("Class {device.GetDeviceClass()} : Vendor {device.GetVendorName()} : UserDefinedName {device.GetUserDefinedName()} :" 

    except Exception as e:
        print("An exception occurred:", str(e))
        return None, None, None

    return cameras, devices, tlFactory


def stop_camera(camera):
    try:
        camera.Close()
        print("camera is closed")
    except genicam.GenericException as e:
        # Error handling.
        print("An exception occurred.")
        print(e.GetDescription())
        exitCode = 1
        sys.exit(exitCode)
    return

def get_camera_img(camera, NumberPictures):
    '''Input:
     averag_img_per_grap: number of images
     Exposure_Time: Exposure_Time of the camera
     return:
     image
     '''
    print(f"Trying to take {int(NumberPictures)} images")
    try:
        camera.StartGrabbingMax(int(NumberPictures))
        images = []
        while camera.IsGrabbing():
            # Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            grabResult = camera.RetrieveResult(10000, pylon.TimeoutHandling_ThrowException)

            # Image grabbed successfully?
            if grabResult.GrabSucceeded():
                print("Images taken")
                images.append(grabResult.Array)

            else:
                print("Error: ", grabResult.ErrorCode, grabResult.ErrorDescription)
                grabResult.Release()
                camera.Close()
            grabResult.Release()
    except genicam.GenericException as e:
        # Error handling.
        print("An exception occurred.")
        print(e)
        exitCode = 1
        sys.exit(exitCode)
    return images

def start_camera(nr_camera, Exposure_Time, set_Gain, width, height, trigger_Mode, cameras, devices, tlFactory):
    try:
        # Create an instant camera object with the camera device found first.
        camera = cameras[int(nr_camera)]
        camera.Attach(tlFactory.CreateDevice(devices[int(nr_camera)]))
        camera.Open()
        # The parameter MaxNumBuffer can be used to control the count of buffers
        # allocated for grabbing. The default value of this parameter is 10.
        print("Using device:", camera.GetDeviceInfo().GetModelName())
        print("Serial Number:", camera.GetDeviceInfo().GetSerialNumber())
        camera.ExposureTime.SetValue(int(Exposure_Time*1e3))
        print("Exposure time:", camera.ExposureTime.GetValue()/1e3, "ms")
        camera.AcquisitionFrameRate.SetValue(int(50))
        #set frame rate set constant to max
        print("Frame rate:", camera.AcquisitionFrameRate.GetValue(), "fps")
        camera.GainAuto.SetValue('Off')
        camera.Gain.SetValue(int(set_Gain))
        print("Gain setting:", camera.Gain.GetValue(), "dB \nAuto Gain:", camera.GainAuto.GetValue())
        camera.Width.Value = int(width)
        print("Image width:", camera.Width.GetValue(), "pixel")
        camera.Height.Value = int(height)
        print("Image height:", camera.Height.GetValue(), "pixel")
        camera.PixelFormat.Value = "Mono12"
        print("PixelFormat: ", camera.PixelFormat.GetValue())
        camera.MaxNumBuffer.Value = 5
        if trigger_Mode == 1:
            camera.LineSelector.Value = "Line1"
            camera.LineMode.Value = "Input"
            camera.TriggerSelector.Value = "FrameStart"
            print('Trigger Selector: ', camera.TriggerSelector.GetValue())
            camera.TriggerSource.Value = "Line1"
            print('Trigger Source: ', camera.TriggerSource.GetValue())
            camera.TriggerMode.Value = "On"
            print('Trigger Mode: ', camera.TriggerMode.GetValue())
        else:
            camera.TriggerMode.SetValue('Off')
            print('Trigger Mode: ', camera.TriggerMode.GetValue())
    except genicam.GenericException as e:
        # Error handling.
        print("An exception occurred.")
        print(e)
        #print(e.__dict__)
        #print(e.GetDescription())
        exitCode = 1
        sys.exit(exitCode)
    return camera

# Per-camera server thread
def camera_server(port, camera_serial_nr, cameras, devices, tlFactory):
    nr_camera = 10000
    camera_found = False
    for i, device in enumerate(devices):
        if int(device.GetSerialNumber()) == camera_serial_nr:
            nr_camera = i
            camera_found = True
            break
    if not camera_found:
        print(f"Camera with Serial# {camera_serial_nr} not found. Exiting thread.")
        return
    if len(devices) <= nr_camera:
        print(f"Camera {nr_camera} does not exist. Exiting thread.")
        return
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setblocking(False)
    IPAddr = extract_ip()
    server.bind((IPAddr, port))
    server.listen(5)
    print(f"Camera {nr_camera} (Serial# {devices[nr_camera].GetSerialNumber()}) server listening on {IPAddr}:{port}\n")

    connections = []
    new_image = False
    images = None

    while True:
        sleep(0.01)
        try:
            conn, addr = server.accept()
            conn.setblocking(False)
            connections.append(conn)
            print(f"Camera {nr_camera}: New connection from {addr}")
        except BlockingIOError:
            pass

        for connection in connections[:]:
            try:
                success, message = get_command(connection)
                if success:
                    message = message.upper()
                    print(f"Camera {nr_camera} got command: {message}")

                    if message == "READY":
                        connection.send(b"*Ready#")

                    elif message == "PARAMETERS":
                        hbin = get_int(connection)
                        vbin = get_int(connection)
                        xstart = get_int(connection)
                        xmax = get_int(connection)
                        width = xmax - xstart + 1
                        ystart = get_int(connection)
                        ymax = get_int(connection)
                        height = ymax - ystart + 1
                        ExposureTime = get_float(connection)
                        Temperature = get_float(connection)
                        ExternalTrigger = get_int(connection)
                        vSpeed = get_int(connection)
                        hSpeed = get_float(connection)
                        FKSMode = get_int(connection)
                        FKSHeight = get_int(connection)
                        FKSDataImages = get_int(connection)
                        FKSReferenceImages = get_int(connection)
                        FKSvSpeed = get_int(connection)
                        FKSExternalTriggerStart = get_int(connection)
                        FKSDirtyLines = get_int(connection)
                        NumberPictures = get_int(connection)
                        PrePicture = get_int(connection)
                        FlushWait = get_int(connection)
                        DoInternaltriggerAfterExternalTrigger = get_int(connection)
                        NumberOfAdditionalReferncePictures = get_int(connection)
                        print(f"Camera {nr_camera} received PARAMETERS")

                    elif message == "IMAGE":
                        camera = start_camera(
                            nr_camera,
                            ExposureTime,
                            Temperature,
                            width,
                            height,
                            ExternalTrigger,
                            cameras,
                            devices,
                            tlFactory
                        )
                        images = get_camera_img(camera, NumberPictures)
                        stop_camera(camera)
                        new_image = True
                        connection.send(b"*Ready#")
                        print(f"Camera {nr_camera} captured image(s)")

                    elif message == "TRANSFER":
                        if new_image:
                            new_image = False
                            for i in range(NumberPictures):
                                ready = wait_for_ready(connection)
                                if ready:
                                    connection.send(bytes(f"*{width * height * 2}#", "utf8"))
                                    gray = images[i].astype(np.uint16) * 16
                                    connection.send(gray.data)
                                    print(f"Camera {nr_camera}: sent image {i}")
                        else:
                            connection.send(b"0\n")

                    elif message == "GOODBYE":
                        print(f"Camera {nr_camera}: closing connection")
                        connections.remove(connection)
                        connection.close()

            except (BlockingIOError, OSError):
                # no data available on socket
                continue

if __name__ == "__main__":
    cameras, devices, tlFactory = initialize_cameras()
    print("")
    if cameras is None or devices is None or tlFactory is None:
        print("Failed to initialize cameras.")
        exit(0) #for silent exit when debugging
        #sys.exit(1) #to pass error code on program exit, useful for script automation

    # Map cameras to desired ports
    # adjust range or mapping as needed:
    camera_ports = {
        40325878: 713 #, #serial number: 25055811: IP port for Vision: 713
        #25008526: 714  #serial number: 25008526: IP port for Vision: 714
        #24972368: 715,
        #25008525: 716,       
    }

    threads = []
    for cam_serial_nr, port in camera_ports.items():
        t = threading.Thread(
            target=camera_server,
            args=(port, cam_serial_nr, cameras, devices, tlFactory),
            daemon=True
        )
        t.start()
        threads.append(t)

    # Keep main thread alive
    try:
        while True:
            sleep(1)
    except KeyboardInterrupt:
        print("Server shutting down.")
