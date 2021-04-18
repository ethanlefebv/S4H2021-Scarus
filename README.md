# S4H2021-Scarus

This project is a SCARA-like robot picking and placing nuts (as in bolts, nuts, washers) with the help of a computer vision system. 

## Table of contents
1. [Hardware requirements](#hardware-requirements)
2. [Software requirements](#software-requirements)
3. [Installation](#installation)
4. [Running the program](#running-the-main-program)
5. [More information](#more-information)
6. [License](#license)

## Hardware requirements
This section lists the hardware used when developing the project. Some parts could be swapped for variants, eg. the Raspberry Pi could have more RAM or be some more recent version.
- Raspberry Pi 4 Model B, 4GB (along with a power cable, microSD card, etc.)
- OpenCR 1.0 by ROBOTIS
- XM430-W350 (2x) by ROBOTIS
- A USB webcam ([link](https://www.amazon.ca/Microphone-Otooking-Streaming-Conferencing-Recording/dp/B08HYDZ6TN))

A 3D printer is also required to print most parts. A laser cutter was also used for the slide. More details can be found [here](Mechanical_Components/README.md).


## Software requirements
This section lists the software used when developing the project. Some components could change in the future, eg. the Ubuntu version could be swapped for the most recent LTS release.

On a desktop/laptop : 
- App to ssh into the Raspberry Pi (cmd, PowerShell, WSL, PuTTY, etc.)
- Arduino IDE or arduino-cli
- This repo (mainly to upload code to the OpenCR)

On the Raspberry Pi :
- Ubuntu Server 20.04 LTS, 64 bit
- Python 3 (the modules are listed [below](#install-needed-programs-for-computer-vision))
- This repo (mainly to run the main program)


## Installation
### On a desktop/laptop :
#### Install Arduino IDE or arduino-cli and the OpenCR libraries
A laptop/desktop is required to upload code to the OpenCR (the libraries are currently not compatible with a Raspberry Pi).

##### Steps for Arduino IDE
1. Install Arduino IDE using the [official guide](https://www.arduino.cc/en/Guide)
2. Follow the instructions [here](https://emanual.robotis.com/docs/en/software/arduino_ide/#preferences) to add the additional Boards Manager URL
3. Follow the instructions [here](https://emanual.robotis.com/docs/en/software/arduino_ide/#install-the-board-package-via-boards-manager) to install the OpenCR board and libraries

##### Steps for arduino-cli
1. Install arduino-cli

    Visit the [installation page](https://arduino.github.io/arduino-cli/latest/installation/) and use the [installation script](https://arduino.github.io/arduino-cli/latest/installation/#use-the-install-script)
2. Enable command-line completion (optional but very useful)

    Follow these [steps](https://arduino.github.io/arduino-cli/latest/command-line-completion/)
3. Add the OpenCR board info to arduino-cli

    [Create a config file](https://arduino.github.io/arduino-cli/latest/getting-started/#create-a-configuration-file) to [add](https://arduino.github.io/arduino-cli/latest/getting-started/#adding-3rd-party-cores) the [OpenCR libraries](https://emanual.robotis.com/docs/en/software/arduino_ide/#preferences)
4. Install the required compiler (as mentioned [here](https://emanual.robotis.com/docs/en/software/arduino_ide/#compiler-settings)) :
    ```bash
    sudo apt install libncurses5-dev:i386
    ```
5. Install the OpenCR board and libraries : 
    ```bash
    arduino-cli core update-index
    arduino-cli core install OpenCR:OpenCR
    ```
6. If on Linux, make sure your user is part of the 'dialout' group (you need to logout and log back in after doing this step) :
    ```bash
    sudo adduser USER dialout
    ```


### On the Raspberry Pi :
#### Install needed programs for computer vision
For the computer vision, three programs/modules are needed : OpenCV for Python, Numpy and tflite_runtime. They can be installed by running the following lines :
```bash
pip install -r requirements.txt
```
```bash
pip install --extra-index-url https://google-coral.github.io/py-repo/ tflite_runtime
```


## Running the main program
### Upload the main sketch to the OpenCR
Start by connecting the OpenCR to a laptop with a USB cable. After uploading, the code runs in loop automatically. To start the main program, jump to the [next section](#run-the-main-python-program-on-the-pi).
#### Upload the sketch with Arduino IDE
1. Open the [main sketch](main/main.ino) with Arduino IDE
2. Under Tools > Board, select OpenCR
3. Under Tools > Port, select the correct port (COMX on Windows, /dev/ttyACMX on Linux, where X is a number)
4. Press the Upload button

#### Upload the sketch with arduino-cli
Go in the root directory of the repo and run the following line :
```bash
arduino-cli compile --upload -b OpenCR:OpenCR:OpenCR -p PORT main
```
where PORT is COMX on Windows and /dev/ttyACMX on Linux (where X is a number, modify the index accordingly).

### Run the main Python program on the Pi
Start by connecting the OpenCR to the Raspberry Pi. Next, connect to the Raspberry Pi using ssh.

On the Pi, go in the root directory of the repo and run the following line :
```bash
python3 main.py ARGS
```
and replace ARGS with the necessary arguments (they can be listed by running `python3 main.py --help`).

## More information

Further information about the vision system can be found [here](Computer_Vision/README.md).

A detailed BOM and assembly instructions are listed [here](Mechanical_Components/README.md).

A diagram of the program structure can be found [here](main/README.md).

A video explaining the project can be watched [here](https://youtu.be/wnFR6_URe1g) (in French with English subtitles).

## License
Apache Version 2.0

See [LICENSE](LICENSE)
