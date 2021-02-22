# S4H2021-Scarus

This project is SCARA-like robot picking and placing nuts (as in bolts, nuts, washers) with the help of a machine-vision system. 

## Hardware requirements
This section lists the hardware used when developing the project. Some parts could be swapped for variants, eg. the Raspberry Pi could have more RAM or be some more recent version.
- Raspberry Pi 4 Model B, 4GB (along with a power cable, microSD card, etc.)
- OpenCR by ROBOTIS
- [motors]
- [webcam]
- TODO

## Software requirements
This section lists the software used when developing the project. Some components could change in the future, eg. the Ubuntu version could be swapped for the most recent LTS release.

On the Raspberry Pi :
- Ubuntu Server 20.04 LTS, 64 bit
- Python 3, pip3, pyserial
- TODO

On a desktop/laptop : 
- App to ssh into the Raspberry Pi (PuTTY, WSL, PowerShell, etc.)
- TODO

## Installation
### On a desktop/laptop :
#### Install arduino-cli and upload code to the OpenCR
Even though the main program runs on a Raspberry Pi, a laptop/desktop is required to upload code to the OpenCR.
1. Visit the [installation page](https://arduino.github.io/arduino-cli/latest/installation/) and use the [installation script](https://arduino.github.io/arduino-cli/latest/installation/#use-the-install-script)
2. Follow the [steps](https://arduino.github.io/arduino-cli/latest/command-line-completion/) to enable command-line completion (optional but very useful)
3. [Create a config file](https://arduino.github.io/arduino-cli/latest/getting-started/#create-a-configuration-file) to [add](https://arduino.github.io/arduino-cli/latest/getting-started/#adding-3rd-party-cores) the [OpenCR libraries](https://emanual.robotis.com/docs/en/software/arduino_ide/#preferences)
4. Install the required compiler (as mentioned [here](https://emanual.robotis.com/docs/en/software/arduino_ide/#compiler-settings)): `sudo apt install libncurses5-dev:i386`
5. Run the following line : `arduino-cli core update-index && arduino-cli core install OpenCR:OpenCR`
6. Make sure your user is part of the 'dialout' group : `sudo adduser USER dialout` (you need to logout and log back in after doing this step)

To upload a sketch, go in the sketch directory (eg. *main/* for the main program) and run the following line : `arduino-cli compile --upload -b OpenCR:OpenCR:OpenCR -p PORT` where PORT is typically COM1 on Windows and /dev/ttyACM0 on Linux (modify the index accordingly).

#### TODO

## License
TODO
