# Stegoooo — an ImHex plugin

Steganography and image-analysis blocks for the ImHex **Data Processor** (the node editor).

Built against the **ImHex 1.38.1** SDK. A plugin only loads into the ImHex version it was compiled
for, so the version is pinned in `CMakeLists.txt` via `IMHEX_VERSION`.

## Blocks

All blocks live under the **Stegoooo** category in the Data Processor's right-click menu.

### Split YCbCr channels

Decodes an image and shows the Y, Cb and Cr channels side by side as three separate pictures.

| Port | Direction | Type | Meaning |
|---|---|---|---|
| `Image`  | in  | Buffer  | The raw bytes of a BMP file (the only format accepted for now) |
| `Y`      | out | Buffer  | Luma plane, one byte per pixel, row-major, top row first |
| `Cb`     | out | Buffer  | Blue-difference chroma plane, same layout |
| `Cr`     | out | Buffer  | Red-difference chroma plane, same layout |
| `Width`  | out | Integer | Image width in pixels |
| `Height` | out | Integer | Image height in pixels |

Node controls:

- **Preview** — `Grayscale` shows each channel's raw values as brightness. `Colorized` puts the
  channel back into RGB with the other two held at neutral (128), so Cb reads along the
  blue↔yellow axis and Cr along the red↔cyan one.
- **Size** — preview height in pixels. Hold **Shift** while hovering a preview for a 3x tooltip zoom.

The conversion is full-range BT.601, the same one JPEG uses:

```
Y  =  0.299    R + 0.587    G + 0.114    B
Cb = -0.168736 R - 0.331264 G + 0.5      B + 128
Cr =  0.5      R - 0.418688 G - 0.081312 B + 128
```

Feed the node from the built-in `Read data` / provider nodes, and wire the plane outputs into other
Buffer nodes to keep analysing them.

## Building

You need the ImHex SDK for the matching version. On Linux it usually sits in
`/usr/share/imhex/sdk`; this repo is developed against the copy in `../imhex-sdk/share/imhex/sdk`.

```bash
export IMHEX_SDK_PATH=/path/to/imhex/sdk
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The result is `build/stegoooo.hexplug`. Install it by copying it next to ImHex's other plugins:

```bash
cp build/stegoooo.hexplug ~/.local/share/imhex/plugins/
```

Then restart ImHex. The plugin shows up under *Help → About → Plugins*.

## Layout

```
include/content/
    nodes.hpp              registration entry points, one per node group
    helpers/image.hpp      Image: an RGBA pixel buffer
    helpers/color_space.hpp YCbCrPlanes and the channel preview rendering
source/
    plugin_stegoooo.cpp    IMHEX_PLUGIN_SETUP, calls the register* functions
    content/nodes/         one file per group of Data Processor nodes
    content/helpers/       format decoding and colour maths, free of any UI or GPU code
romfs/                     bundled resources, reachable through romfs::get()
```

Threading rule worth knowing before adding a block: `dp::Node::process()` runs on a Data Processor
worker thread, while `drawNode()` runs on the render thread. OpenGL textures may only be created in
`drawNode()`, so `process()` does the decoding and hands finished pixel buffers over under a lock.

## Notes

- The CI workflow in `.github/workflows/build.yml` still pins `WerWolv/imhex-download-sdk@v1.32.2`.
  Bump it to the 1.38.1 release before relying on the built artifacts.
