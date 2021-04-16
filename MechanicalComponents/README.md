# Mechanical assembly
This section lists the steps for the full assembly of the SCARA robot.

### BOM
Robot:
-   2 x Carbon tubing Ø 10 mm x 285 mm (or aluminium)
-   2 x Aluminium tubing Ø ½” x 185 mm
-   2 x DYNAMIXEL XM430-W350-T
-   16 x 608-ZZ Bearings
-   6 x Screws M3 x 10 mm (Tighten arms)
-   2 x Screws M3 x 25 mm (Motor joints arms)
-   2 x Screws M3 x 35 mm (Motor joints threaded rods)
-   2 x Screws M3 x 20 mm (Coupling threaded rods)
-   12 x Nuts M3
-   3 x Screws M8 x 50 mm (Joints)
-   3 x Nuts M8
-	2 x Threaded rods 5/16-18 x 60 mm
-	16 x Screws M2.5 x 10 mm (Dynamixels' sides)
-	8 x Screws M2.5 x 20 mm (Dynamixels' bottom)
-	8 x Screws M2 x 8 mm (Dynamixels' coupling)

Gripper:
-	2 x MOSFET FQP30N06L
-	2 x MOSFET driver MCP1407-E/P
-	4 x Resistance 2000 ohms
-	2 x Resistance 100 ohms
-	2 x Diode 1N4934RLG
-	1 x Linear solenoid (DC 12V 5N 10mm)
-	1 x Magnet wire roll (30 AWG)
-   1 x Fishing wire thread
-	4 x Screws M4 x 15 mm (Fan)
-	2 x Screws M3 x 8 mm (linear solenoid fixation)
-	1 x Screws M3 x 30 mm (Solenoid core)
-   2 x Nuts M3
-	1 x Screws M2 x 12 mm (Electromagnet fixation)
-	1 x Nuts M2

Robot's base and slide:
-	2 x Birch plywood 4x8 feet
-	1 x Wood glue
-	1 x Parchment paper (Slide bed)
-	7 x Screws M10 x 50 mm or 3/8-24 x 2 in (Adjustable stands)
-	14 x Nuts M10 or 3/8-24
-	4 x Screws M4 x 20 mm (Motor bases fixation)
-	4 x Screws M4 x 25 mm (Dove tails fixation)
-	12 x Nuts M4

General:
-	1 x OpenCR 1.0
-	4 x Screws M4 x 15 mm (OpenCR fixation)
-	4 x Nuts M4
-	1 x Raspberry Pi 4B 4GB
-	4 x Screws M2.5 x 12 mm (Raspberry Pi fixation)
-	4 x Nuts M2.5
-	1 x Webcam Otooking 1080p (any USB webcam would do the job)
-	2 x Tie-wraps (Camera fixation)
-	1 x Electric tape roll
-	1 x 1 kg PLA roll
-	1 x Loctite bottle
-	10 x Nuts M5
-	10 x Lock-nuts M5

### 3D prints
All the 3D print files are located in the STL folder

### 2D laser cuts
All the laser cut files are located in the DXF folder

### Joints
3D print all the .stl files located in the STL/Robot/Joints sub-folder
1.  Press fit the 4 bearings in their 2 housings.
2.  Pass a M8 x 50 mm screw through 1 housing, then through a spacer, and through the other housing.
3.  Apply Loctite to the protruding end of the M8 screw and screw the M8 nut with as much force as possible
    without impairing the bearings' rotation. (Let the Loctite cure)
4.  Clip the 2 caps to each end of the joint.
5.  Repeat steps 1 to 4 for the other joints.

Link to the joint assembly video:

### Motors
3D print all the .stl files located in the STL/Robot/Motors sub-folder
1.  Put the 3D printed coupling on the Dynamixel's coupling and screw it on with 4x M2 x 8 mm screws.
2.  Drill 2 holes in a threaded rod as per the CAD.
3.  Insert the threaded rod in the 3D printed coupling and screw it in with a M3 x 20 mm screw and a M3 nut.
4.  Insert bearings in the bearing housing. They should be press fitted.
5.  Put the cap bearings over the bearings (Clip on the bearing housing).
6.  Screw the subassembly (bearings + housing + caps) on the motor. Screw it with 8x M2.5 x 10 mm screws in the motor's casing.
7.  Screw the motor joint on the threaded rod with a M3 x 35 mm and a M3 nut.
8.  Screw a motor 3D printed motor stand to the base of the motor with 4x M2.5 x 20 mm.
9.  Repeat steps 1. to 8. for the other motors.

Link to the motors assembly video:

### Gripper
3D print all the .stl files located in the STL/Gripper sub-folder
1.  Screw the LIFTING_SOLENOID_HOLDER and a M3 nut within it onto the linear solenoid with a M2 x 12 mm screw and a M2 nut.
2.  Pass a M3 x 30 mm screw through the LIFTING_SOLENOID_CORE and use a M3 nut on the other end.
3.  Fill the LIFTING_SOLENOID_CORE by rolling magnet wire arround it with a drill.
4.  Wrap the magnet wire in electric tape while ensuring that the 2 ends of the magnet wire are still protruding.
5.  Screw the protruding end of the M3 x 30 mm screw into the encastrated M3 nut from step 1.
    (Optional: Add washers with hot glue to the end of the M3 x 30 mm screw to widen the contact surface between the nuts and the gripper)
6.  Use fishing wire and thread it around the assembly to prevent the lifting solenoid from spinning freely around the linear solenoid.
6.  Slide the linear solenoid into the SOLENOID_COVER and crew it into the SOLENOID_DOOR with 2x M3 x 8 mm screws.
7.  Clip the SOLENOID_DOOR into the SOLENOID_COVER.
8.  Screw the fan in the SOLENOID_COVER with 4x M4 x 15 mm.
9.  Screw the gripper onto the end of the robot.

Link to the gripper assembly video:

### Arms
1.  Put tape at both ends of the long arms (around one layer but could be more).They will be press fitted, so it is best if they are tight and do not slip.
2.  Insert one long arm in the top part of the joint with the screw thread for the gripper facing down and the other long arm in the bottom piece of the same joint.
3.  Insert the other end of the 2 long arms in the top part of 2 other joints.
4.  Insert the short arms in the bottom parts of those joints.
5.  Put the M3 x 10 mm screws and nuts in the joints to tighten it around the arms.

Link to the arms assembly video:

### Robot's base
Laser cut all the .dxf files located in the DXF/ROBOT_BASE sub-folder
1.  Assemble all the wooden parts as per the CAD.
2.  Apply wood glue in every slots and between the side walls and the slider support.
3.  Secure the assembly with clamps and let the glue cure.
4.  Screw the OpenCR with 4x M4 x 15 mm and 4x M4 nuts to the slider.
5.  Screw the Raspberry Pi with 4x M2.5 x 12 mm and 4x M2.5 nuts to the slider.
6.  Insert the slider into the assembly.
7.  Screw the 4 stands (4x M10 x 50 mm screws + 8x M10 nuts) in the 4 corresponding 10 mm holes.
8.  Screw the 2 motor stands on the top plate with 8x M4 x 20 mm screws and 8x M4 nuts.

### Slide
Laser cut all the .dxf files located in the DXF/SLIDE sub-folder

3D print the STL/PLA_BUCKET.stl file with a PLA color constrasting with the items to pick up
1.  Assemble all the wooden parts as per the CAD.
2.  Apply wood glue in every slots and between the 3D printed bucket and the wooden bucket.
3.  Secure the assembly with clamps and let the glue cure.
4.  Apply a few layers of parchment paper onto the surface of the slide (where the nuts will slide) and scotch tape the edges.
6.  Screw the 3 stands (3x M10 x 50 mm screws + 6x M10 nuts) in the 3 corresponding 10 mm holes.
7.  Screw the 4 dove tails to the front of the base with 4x M4 x 25 mm and 4x M4 nuts.
8.  Fix the camera on the camera support using tie-wraps passing through the holes.
 
### Final assembly of the SCARUS
1.  Insert the slide's dove tail into the robot's base's dove tail shaped slots.
2.  Adjust the gripper's height relative to the slide's bucket with the 7 stands.
3.  You're done, press start!
