# Control and communication libraries of the Smart Mini-Factory Laboratory

This repository contains a set of libraries and tools for interfacing, communicating and controlling different devices found in the Smart-Mini Factory Laboratory of the Free University of Bozen-Bolzano.

Currently, the repository contains the following libraries:

1. Tetrominoes detection (formerly piecedetector).
2. Flexibowl UDP communication library.
3. Pylon cameras interface (dramatically experimental state).
4. Network communications library (limited UDP messaging and collaborative discovery).
5. Miscellaneous utilities library (error handling, simplified fork/exec, ...).


## How to compile
```
git clone https://github.com/mruizve/smf.git
cd smf
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

After compilation libraries are found at ``smf/lib`` and some tools at ``smf/bin``. We invite you to read all of the ``CMakeLists.txt`` to understand the structure of the code and to read, modify and improve the code to suit your needs.


## Tetrominoes detection
Piece detector for the Cobra i600 scara robot. It detects, recognize and determines the pose of tetris like pieces within an image frame.

To try the detector, after compiling,
```
cd ../bin
./detector parameters_path image_path
```
**NOTE**: the file ``parameters_path`` if not exists will be created, otherwise will be updated.

To visualize the parameters tuning GUI, set ``<gui>1</gui>`` at ``parameters_path`` (the default value is ``<gui>0</gui>``).

Example usage:
```
./detector ../data/detector.xml ../data/source2.jpg
```

## Flexibowl UDP control
Control interface for the Adept/ARS Flexibowl parts feeder.

To try the interface, after compiling,
```
cd ../bin
./flexibowl ip port time
```
where ``ip`` can be either the IP of the robot or the string ``BORADCAST`` for cooperative discovery of the robot's IP, ``port`` the UDP port number and ``time`` the control loop time interval in milliseconds of the internal control thread (this functionality is obsolete, since the feeder doesn't lacks of a proper interface/mechanism for a real feedback control of it).

Example usage:
```
./flexibowl BROADCAST 5001 100
```
