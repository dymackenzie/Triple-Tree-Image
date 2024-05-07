# Triple Tree Image Reducer

A school-based project that uses C++ to transform given PNGs into a Triple Tree structure. A Triple Tree holds the data of the PNG in specific levels, allowing different pruning techniques based on color tolerance with neighboring pixels. The result is a reduced quality output PNG.

This project is based off of an assignment by CPSC 221 at UBC.

Features include:
- Builds Triple Tree structure given a PNG image and populates it with Nodes.
- Copy function and deconstrutor.
- Image rotater through manipulating the Triple Tree structure.
- Image flipper through manipulating the structure.
- Reduces image quality through pruning Triple Tree structure based of color tolerance with neighboring pixels.
- Renders Triple Tree structure into appropiate PNG.