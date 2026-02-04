gcode-preview
=============

A minimalist, high-performance C utility to extract embedded base64 thumbnails from G-code files. Designed to be small, fast, and pipe-friendly.

## Features

- **Zero Bloat**: Focused solely on extracting and decoding embedded preview data.
- **Performance**: Uses Turbo-Base64 for accelerated decoding.
- **Unix-Philosphy**: Simple CLI flags, easy to integrate into shell scripts or file manager (Ranger/LF) previews.


Building and installing the C version
-------------------------------------

1. clone this repository recursively on your local machine
```Bash
git clone  --recursive https://github.com/8bitmcu/gcode-preview.git
```
2. run `make clean && sudo make install` from within the repository folder

Example usage
-------------

Basic extraction

`gcode-preview -i benchy.gcode -o thumb.png`

`gcode-preview-bash benchy.gcode thumb.png`

Generate thumbnails for an entire directory of G-code:

`for f in *.gcode; do gcode-preview -i "$f" -o "${f%.gcode}.png"; done`

Performance
-----------
Quick benchmark of `gcode-preview` vs `gcode-preview-bash` extracting a 800x600 png image from a gcode file:

```
$ time gcode-preview-bash benchy.gcode thumb.png

real    0m0.014s
user    0m0.016s
sys     0m0.004s
```

```
$ time gcode-preview -i benchy.gcode -o thumb.png

real    0m0.002s
user    0m0.001s
sys     0m0.000s
```


Ranger / lf integration
-----------------------

With image previews enabled in [ranger](https://github.com/ranger/ranger/wiki/Image-Previews) or [lf](https://github.com/slavistan/howto-lf-image-previews), it is possible to extend the functionality to get previews for .gcode files, here is an example with lf:

![preview](/assets/lf.png)
