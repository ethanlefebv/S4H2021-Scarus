# S4H2021-Scarus

This project is SCARA-like robot picking and placing nuts (as in bolts, nuts, washers) with the help of a computer vision system. 

Further informations about the vision system can be found in its own [documentation](Computer_Vision/README.md)

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
- For the computer vision, OpenCV for Python, Numpy and tflite_runtime is needed, all 3 can be obtained with this command : 
```bash
pip install -r requirements.txt
```
## License
TODO
