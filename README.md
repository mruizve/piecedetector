### piecedetector
Piece detector for the Cobra i600 scara robot.

#### what is missing
- Shapes lines clustering in two principal directions.
- Removal of outsider lines (those not aligned with a principal directions).
- Shapes cleaning.
- Shapes recognition/classification.

#### how to compile
```
git clone https://github.com/mruizve/piecedetector.git
cd piecedetector
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

#### how to use
after compiling
```
cd ../bin
./detector parameters_path image_path
```
**NOTE**: the file ``parameters_path`` if not exists will be created, otherwise will be updated.

To visualize the parameters tuning GUI, set ``<gui>1</gui>`` at ``parameters_path`` (the default value is ``<gui>0</gui>``).

#### example usage
```
./detector ../data/detector.xml ../data/source2.jpg
```
