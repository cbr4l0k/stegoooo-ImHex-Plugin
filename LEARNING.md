# Learning Stegoooo by fixing it

This plugin ships with **17 deliberate mistakes** spread over six new nodes: RGB split, LSB hide,
LSB retrieve, PSNR, SSIM and Save as BMP. Fix them in order and you will have touched every part of
how an ImHex Data Processor plugin works, and every idea behind the steganography.

Each mistake has an id. Search the source for `HINT(<id>)` to find a one-line nudge next to it. This
file gives, for each one, what you will see, a bigger nudge, where to look in the docs, and the
answer (folded away, so you only see it if you open it).

| Stage | Ids | How you find them |
|---|---|---|
| 1. Compile | C1–C6, T1–T4 | `ninja -C build` stops with errors |
| 2. Load | C7 | The build passes, but ImHex refuses to load the plugin |
| 3. Behaviour | R1–R6 | Everything loads, but the results are wrong |

```sh
export IMHEX_SDK_PATH=$(realpath ../imhex-sdk/share/imhex/sdk)   # needed whenever CMake re-runs
ninja -C build -k 0      # -k 0 keeps going after the first failing file, so you see all errors at once
```

---

## 0. How the plugin fits together (read this first)

```
plugin_stegoooo.cpp        IMHEX_PLUGIN_SETUP(...) { registerXxxNodes(); ... }        docs §3
        │
        ▼
content/nodes/*.cpp        ContentRegistry::DataProcessor::add<NodeXxx>("Stegoooo", "Name")  docs §4.4
        │
        ▼
class NodeXxx : dp::Node   constructor = ordered list of dp::Attribute (the ports)   docs §4.1, §4.3
   ├─ process()            WORKER thread: read inputs, compute, set outputs.   No GPU/ImGui here!
   ├─ drawNode()           RENDER thread: ImGui widgets, textures.
   └─ m_pending* + mutex   the hand-off from process() to drawNode()
        │
        ▼
content/helpers/*.cpp      plain C++ with no ImHex types: decode, color math, LSB, metrics, BMP
```

**Images travel between nodes as encoded file bytes** (a `Buffer`). Every node decodes its input
with `decodeImage()`. Nodes that produce an image output a **BMP**, because BMP is lossless and
keeps every least-significant bit. PNG would work too. JPEG would not: its lossy compression wipes
out the hidden bits.

**The threading rule is the most important idea in this code.** ImHex runs `process()` on a
background thread. OpenGL textures can only be created on the render thread. So `process()` puts
its results in `m_pending...` under a mutex, and `drawNode()` picks them up and turns them into
textures. Keep this in mind for R5.

---

## Stage 1: compile errors

### C1: `'registerRgbNodes' was not declared in this scope`
- **Concept:** the plugin's entry point only knows the functions declared in `content/nodes.hpp`.
- **Nudge:** C++ names are case-sensitive. Compare the call with the declaration.
- **Docs:** §3 Plugin entry points.
<details><summary>Solution</summary>

`source/plugin_stegoooo.cpp`: `registerRgbNodes()` → `registerRGBNodes()`.
</details>

### C2: `'Double' is not a member of 'hex::dp::Attribute::Type'`
- **Concept:** a port can only have one of three types. The second error, `no matching function for call to Node::Node`, is a knock-on effect of the first. Always fix the **first** error before worrying about the rest.
- **Nudge:** look up `enum class Attribute::Type`.
- **Docs:** §4.1 `dp::Attribute`.
<details><summary>Solution</summary>

`metrics.cpp`, SSIM node: `Type::Double` → `Type::Float`. The value in a Float port is still a C++ `double` (`setFloatOnOutput(u32, double)`).
</details>

### C3: `'NodePSNR::process() const' marked 'override', but does not override`
- **Concept:** `override` asks the compiler to check that you really are replacing a virtual function from the base class. A `const` method has a different signature, so it replaces nothing. The flood of `discards qualifiers` errors and `invalid new-expression of abstract class type` all come from this one mistake: `process()` must change the node's state (outputs, pending data, the mutex), so it cannot be `const`.
- **Nudge:** compare with `virtual void process() = 0;` in `hex/data_processor/node.hpp`.
- **Docs:** §4.2 `dp::Node`.
<details><summary>Solution</summary>

`void process() const override` → `void process() override`.
</details>

### C4: `'this' was not captured for this lambda function`
- **Concept:** the file dialog is **asynchronous**. The lambda runs later, possibly after the node has been deleted. So the lambda should carry **its own copy** of the data. Capturing `this` would compile, but it could crash if the user deletes the node while the dialog is open.
- **Nudge:** lambda init-capture: `[name = expression]`.
- **Docs:** header-index row for `hex/helpers/fs.hpp` → `openFileBrowser`.
<details><summary>Solution</summary>

```cpp
[bmp = m_bmp](const std::fs::path &path) {
    wolv::io::File file(path, wolv::io::File::Mode::Create);
    file.writeVector(bmp);
}
```
</details>

### C5: `use of deleted function 'ImGuiExt::Texture& operator=(const Texture&)'`
- **Concept:** `Texture` wraps a GPU object, so it is **move-only**. If it could be copied, two objects would each try to free the same GPU texture.
- **Nudge:** you don't need `texture` after this line.
- **Docs:** `hex/ui/imgui_imhex_extensions.h`: look at which constructors are `= delete`.
<details><summary>Solution</summary>

`m_textures[i] = std::move(texture);` Or assign `fromBitmap(...)` straight into `m_textures[i]`, as `ycbcr_channels.cpp` does.
</details>

### C6: `cannot convert 'std::optional<std::vector<u8>>' to 'std::span<const u8>'`
- **Concept:** `lsbExtract` can fail, so it returns an `optional`. Once you have checked `has_value()`, you have to **unwrap** it to get at the vector.
- **Docs:** §4.6 Buffer nodes (`setBufferOnOutput` takes `std::span<const u8>`).
<details><summary>Solution</summary>

`lsb.cpp`: `setBufferOnOutput(1, message)` → `setBufferOnOutput(1, *message)`.
</details>

### T1–T4: `static assertion failed: TODO(Tn): ...`
These are gaps for you to fill in. Write the code, then delete the `STEGO_TODO` line. The macro is in `include/content/helpers/todo.hpp`.

#### T1: LSB capacity (`stego.cpp`, `lsbCapacity`)
Each pixel gives 3 bits (the lowest bit of R, G and B; alpha is left alone). The function then subtracts the 4-byte length header for you.
<details><summary>Solution</summary>

`const size_t bytes = image.pixelCount() * 3 / 8;`
</details>

#### T2: write one bit (`stego.cpp`, `lsbEmbed`)
Clear the lowest bit with a mask, then OR in the new one. Only ±1 is ever added to a pixel value. That tiny change is exactly why LSB is hard to see (you will measure it with PSNR).
<details><summary>Solution</summary>

`value = u8((value & 0xFE) | bit);`
</details>

#### T3: PSNR (`stego.cpp`, `psnr`)
PSNR = 10 · log₁₀(MAX² / MSE), with MAX = 255 for 8-bit images. Identical images have MSE = 0, and the code above already handles that case as +∞.
<details><summary>Solution</summary>

`return 10.0 * std::log10(255.0 * 255.0 / error);`
</details>

#### T4: BMP row padding (`bmp.cpp`, `encodeBMP`)
A 24-bit row takes `width*3` bytes, rounded **up** to the next multiple of 4. Check your answer against these: width 1 → 4, width 3 → 12, width 5 → 16.
<details><summary>Solution</summary>

`const size_t rowSize = (size_t(image.width) * 3 + 3) / 4 * 4;`
</details>

> Look at the warnings too. `unused parameter 'modified'` in `ssim()` is already pointing at R3.

---

## Stage 2: the build succeeds, but the plugin won't load

### C7: ImHex's log says something like `undefined symbol: _ZN3hex6plugin8stegoooo9encodeBMP...`
- **Concept:** a plugin is a shared library. The linker lets a shared library leave symbols undefined, on the assumption that something else will provide them when it loads. Nothing does, so the error only appears when ImHex loads the plugin.
- **Find it yourself:** `nm -DC --undefined-only build/stegoooo.hexplug | grep stegoooo`. Anything in *your* namespace that is still undefined is a bug.
- **Docs:** §12 CMake/plugin build API (`add_imhex_plugin(SOURCES ...)`).
<details><summary>Solution</summary>

Add `source/content/helpers/bmp.cpp` to `SOURCES` in `CMakeLists.txt`. Heads-up: once it is compiled, T4 is in that file, so if you skipped it the compiler will stop you there.
</details>

---

## Stage 3: wrong results

Build this graph in the Data Processor to test with. Two Load File nodes each read the same PNG
(the cover), and a Buffer node holds your message text.

```
cover ─┬─────────────────────────────┬─► PSNR.Original      ┌─► SSIM.Original
       └─► LSB hide.Cover ──► stego ─┼─► PSNR.Modified      ├─► SSIM.Modified
message ─► LSB hide.Message          ├─► LSB retrieve ──► message out
                                     └─► Save as BMP ──► open in an image viewer
```

What correct results look like: PSNR is **above 48 dB** (the shorter the message, the higher it goes, but it is always finite),
SSIM is **≈ 0.99+**, the retrieved text equals the text you put in, and the saved BMP is the right
way up.

### R6: PSNR says `inf dB`, and retrieve returns garbage or "length exceeds capacity"
- **Concept:** metrics double as tests. A PSNR of ∞ means the "stego" image is *identical* to the cover, so nothing was hidden.
- **Nudge:** which image gets encoded to BMP in `NodeLSBEmbed::process()`?
<details><summary>Solution</summary>

`encodeBMP(*image)` → `encodeBMP(*stego)`.
</details>

### R2: PSNR is only ~28 dB for an LSB change that is at most ±1
- **Concept:** with a maximum change of ±1, the MSE is at most 1, so PSNR must be at least 48.1 dB. That kind of sanity bound catches bugs. Here, `3 - 4` stored in a `u8` wraps round to 255, and 255² makes the error look huge.
- **Nudge:** HINT(R2) in `mse()`.
<details><summary>Solution</summary>

`const auto difference = double(original.rgba[...]) - double(modified.rgba[...]);`
</details>

### R1: PSNR looks right now, but the retrieved message is still garbage
- **Concept:** hiding and retrieving are two ends of one **protocol**. Bit order, byte order and channel order all have to match. `lsbEmbed` writes each byte **most significant bit first**.
- **Nudge:** trace what `readByte` builds when it reads the bits `0,1,0,0,1,0,0,0` (`'H'`).
<details><summary>Solution</summary>

`value = u8((value << 1) | (image.rgba[pixel * 4 + channel] & 1));`
</details>

### R3: SSIM says `1.0000`, even for a completely different image
- **Concept:** SSIM compares the **mean (luminance), variance (contrast) and covariance (structure)** of two images over small windows. If both inputs are the same image, it is 1 by definition. The compiler warned you about this in Stage 1.
<details><summary>Solution</summary>

`const auto modifiedLuma = toLuma(modified);`
</details>

### R4: the saved BMP is upside down
- **Concept:** BMP stores rows **bottom-up** when `biHeight` is positive. (A negative height means top-down.) The decoder is fine; the encoder writes the rows in the wrong order.
<details><summary>Solution</summary>

Loop `for (u32 y = image.height; y > 0; y -= 1)` and use `(y - 1)` in the offset. Or write `-height` in the header, but then remember that `putLE` takes a `u32`.
</details>

### R5: Split RGB channels shows no pictures until you change the Preview combo
- **Concept:** the worker-to-render hand-off. `collectPendingImage()` picks up the new image but never tells `refreshTextures()` that the textures are now out of date. Changing the combo sets the flag, which is why that "fixes" it.
- **Nudge:** compare with `collectPendingPlanes()` in `ycbcr_channels.cpp`.
<details><summary>Solution</summary>

Add `m_texturesDirty = true;` at the end of `collectPendingImage()`.
</details>

---

## Going further
- Try hiding the message, then saving the stego image as **JPEG** in an image editor and retrieving it. LSB will not survive, and that is the reason F5 exists. F5 is the next phase.
- Hide messages that fill 10%, 50% and 100% of the capacity, and plot PSNR against how much you hid.
