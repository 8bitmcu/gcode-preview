gcode-preview
=============

Extract images as png/jpg from .gcode files with C or Bash


Building and installing the C version
-------------------------------------

1. clone this repository locally on your machine
2. run `make clean && sudo make install` from within the repository folder

Performance
-----------
Quick benchmark of `gcode-preview` vs `gcode-preview-bash` extracting a 800x600 png image from a gcode file:

`gcode-preview-bash`:
```
real    0m0.014s
user    0m0.016s
sys     0m0.004s
```

`gcode-preview`:
```
real    0m0.002s
user    0m0.001s
sys     0m0.000s
```

Example usage
-------------

`gcode-preview -i benchy.gcode -o thumb.png`

`gcode-preview-bash benchy.gcode thumb.png`

Ranger / lf integration
-----------------------

With image previews enabled in [ranger](https://github.com/ranger/ranger/wiki/Image-Previews) or [lf](https://github.com/slavistan/howto-lf-image-previews), it is possible to extend the functionality to get previews for .gcode files, here is an example with lf:

![preview](/assets/lf.png)
