# gbs-fadestreet

**Version 4.3.1. Requires GB Studio 4.3.0 or newer.**

[*Fade Street*](https://gearfo.itch.io/fade-street) is a GB Studio plugin for creating better looking colour fades, as well as many other types of palette effect. It is suitable for colour-only games, monochrome-only games, and mixed ("black cart") games, although some features are exclusive to certain modes.

GB Studio works out its colour fades on the Game Boy itself, which limits them to a very simple calculation. Fade Street works them out on your PC when you build the project and stores the results in the ROM, so your game plays the finished effect back. That buys you smooth perceptual fades, day to night transitions, animated waterfalls, flickering neon, and effects that fade and animate at the same time.

You build all of it from events in the editor. No scripting by hand is involved.

This version is a fork of the original Fade Street by [gearfo](https://gearfo.itch.io/).

<img src=".img/face.webp" width="240"> <img src=".img/snow.webp" width="240"> <img src=".img/light.webp" width="240">
<img src=".img/falls.webp" width="240"> <img src=".img/dissolve.webp" width="240"> <img src=".img/rain.webp" width="240">

---

## Table of Contents

1. [Concepts](#concepts)
2. [Project Setup](#project-setup)
3. [Size Limits and Restrictions](#size-limits-and-restrictions)
4. [Events Reference](#events-reference)
5. [FAQ](#faq)
6. [Media](#media)
7. [Memory Footprint](#memory-footprint)
8. [Bank 0 (HOME) Usage](#bank-0-home-usage)
9. [Changelog](#changelog)

---

## Concepts

### Fades and colour cycles

The effects Fade Street can create fall into two broad groups.

A **fade** takes a starting palette and varies the colours smoothly over time until they reach a target palette. That includes fading in and out to black or white, but also transitions such as day to night, or highlighting a certain area of the screen. Some colours can get darker while others get lighter, and individual colours can stay the same.

![A diagram showing how one set of colours fades to another over time.](.img/fade.svg)

A **colour cycle** shuffles the colours of the on-screen palettes in a predetermined order. As a colour moves through palette slots it creates a sense of motion, which is useful for animating things that flow, spin or flicker.

![A diagram showing how colours rotate through palette slots during a colour cycle.](.img/cycle.svg)

The cycle above takes four time steps per iteration; the first ends at t=3 and a second begins at t=4. All cycles created by Fade Street loop seamlessly this way.

When an event creates a fade **with** a colour cycle, the cycle's colours fade along with everything else, so a waterfall animated by a colour cycle can go from day to night without its animation stopping.

![A diagram showing how a colour palette is cycled and faded at the same time.](.img/fadecycle.svg)

### Simple, Standard, Special

Many Fade Street events are labelled with one of these words. They describe how complex the event's options are; the underlying colour calculations are the same.

| Label | Options |
|---|---|
| **Standard** | You choose the palettes at both the start and the end of the fade. |
| **Simple** | One endpoint is chosen from a list of presets, which speeds up common effects like "fade to black". |
| **Special** | The most complicated settings. You choose palettes for both endpoints *and* can apply a preset effect to either one. Useful for edge cases and for bridging the gap between the other events. |

### Bespoke colour cycles

The Simple, Standard and Special *Fade with Colour Cycle* events share a limitation: the order of the palette slots in the cycle is fixed, and the number of colours in the cycle must equal the number of slots. Slot 0 always comes before slot 1; slots in background palette 5 always come after those in palette 4.

When you need more flexibility, the **Bespoke Colour Cycle** events let you enter the exact ordering of palette slots to use, plus a list of colours of any length.

**Entering palette slots.** Slots are a comma-separated list of base-ten numbers, numbered 0–63 from the first slot of the first background palette to the last slot of the eighth sprite palette. The numbering includes the transparent slots of the sprite palettes, but writing to those has no effect.

![A diagram showing how palette slots are numbered by bespoke cycle events.](.img/slots.svg)

For background palettes, slots 0–3 of each palette are GB Studio's white, light grey, dark grey and black. For sprite palettes, slots 0–3 are transparent, white, light grey and black.

**Entering colours.** Colours are also a comma-separated list, all in the same format:

| Format | When to use it |
|---|---|
| **GBS representative hex** | Copying hex values from the GB Studio palettes screen, to match existing GB Studio palettes. |
| **sRGB 24-bit hex** or **linear RGB 24-bit hex** | Copying hex values from another graphics program. If you do not know which, try both. Only one will look right. |
| **RGB components** | Entering channels separately; three components per colour, each an integer 0–31. |
| **Game Boy 15-bit hex** | The native Game Boy format, useful for copying palettes from an emulator. |

A leading `#` or `0x` is optional in every hex format.

### Automagic fades

For the most part Fade Street manages palettes completely separately from GB Studio's own behaviour, and the palettes you set for a scene in the editor have no effect on what appears on screen.

The **Automagic** events are the exception. They work directly with the scene palettes set in the GB Studio editor, including automatic background palettes. They also work in both colour and monochrome modes: depending on your project settings they store a monochrome fade, a colour fade, or both, and the correct one is chosen at run time.

These are the closest thing Fade Street has to a drop-in replacement for the default Fade In and Fade Out events, which makes them useful for beginners and for migrating large existing projects.

### Monochrome mode

Two events are exclusively for monochrome mode: **DMG Fade** and **DMG Palette Cycle**.

DMG Fade can flicker intermediate colours, alternating quickly between two shades to suggest a third. That gives three extra shades between the four normal greys, for a smoother gradient.

Original Game Boy screens blur successive frames together, which makes this look good. On newer screens, or in some emulators, it can produce very unpleasant visible flicker. Use it with caution.

<img src=".img/dmg.webp" width="240"> <img src=".img/dmg2.webp" width="240">

### Running several effects at once

Traffic lights on a slow cycle and a neon sign on a faster one need two effects at once. **Multiple Fades With Colour Cycles** and **Multiple Bespoke Colour Cycles** each have eight tabs, one effect per tab, all running together.

Combining them into one script also costs less processor time and keeps them in step with each other.

Those two events also offer **BlendShift Cycling**, not available in the other cycle events, where cycle colours fade into one another instead of jumping. It can greatly enhance certain effects, but it forces palettes to update every frame, which creates very large scripts. Use it in moderation.

---

## Project Setup

1. Copy the `FadeStreetPlugin` folder into your project's `plugins` folder. The new events appear in the **Add Event** menu and the engine changes are applied when you build.
2. Automatic fades stop working once the plugin is installed, so add fade events to each scene to control fading in and out yourself.

A demo project is included in `FadeStreetPluginExample/`, containing examples of most Fade Street events in action, with comments filling in the practical details.

### Black cart (dual-compatible) games

The Automagic events are the only ones that work in both colour and monochrome mode automatically. The other events can still be used in dual-compatible games by wrapping them in an *If Color Mode Is Available* event: create one fade for monochrome mode and another for colour mode, and let that event choose between them at run time.

---

## Size Limits and Restrictions

- **Automatic fades and the built-in Fade In / Fade Out events are disabled.** They do not work at all in any situation once this plugin is installed. Every scene needs its fades set up manually.
- **The new events use more ROM than the automatic fades.** Fades with more colour steps take more space, and colour cycles can use quite a lot depending on the options chosen.
- **BlendShift Cycling creates very large scripts**, because it updates palettes every frame.
- **Multi-effect events run for the least common multiple of their cycle lengths.** If one cycle takes 25 frames and another 50, the event is 50 frames long; if one takes 19 and another 17, the event is 323 frames long. Pick cycle lengths that combine sensibly.
- **The plugin replaces several stock engine files**, covering fades, palettes, scene loading, music and the screen interrupts. Another plugin changing the same parts needs the engine files merged by hand.

---

## Events Reference

All events appear under the **Fade Street** group. Five of them also appear under **Fade Street - Beginner Friendly**. Those are the ones to start with, and they are marked below.

### Fades

| Event | Description |
|---|---|
| **Simple Fade** ⭐ | Fade between the current palettes and a preset endpoint. |
| **Standard Fade** ⭐ | Fade between two palettes you choose. |
| **Special Fade** | Fade between two palettes, with an optional preset effect applied to either endpoint. |
| **Looping Fade** ⭐ | A standard fade that repeats. |
| **Looping Special Fade** | A special fade that repeats. |
| **Single Colour Fade** | Fade a single palette slot. |
| **Looping Single Colour Fade** | A single-colour fade that repeats. |
| **Fade One Colour At A Time** | Fade the chosen slots one after another rather than together. |
| **Volume Fade** | Fade the master volume (left and right), affecting all music and sfx channels. Volume commands inside music and sfx may override the values you set. |

### Fades with colour cycles

| Event | Description |
|---|---|
| **Simple Fade with Colour Cycle** | A simple fade running together with a colour cycle. |
| **Standard Fade with Colour Cycle** | A standard fade running together with a colour cycle. |
| **Special Fade with Colour Cycle** | A special fade running together with a colour cycle. |
| **Multiple Fades With Colour Cycles** | Up to eight independent fade-plus-cycle effects in one event, optionally with BlendShift Cycling. |

### Colour cycles

| Event | Description |
|---|---|
| **Bespoke Colour Cycle** | A colour cycle with an explicit slot order and a colour list of any length. |
| **Multiple Bespoke Colour Cycles** | Up to eight independent bespoke cycles in one event, optionally with BlendShift Cycling. |

### Automagic

| Event | Description |
|---|---|
| **Automagic Fade In** ⭐ | Fade in using the scene's own palettes as set in the GB Studio editor. Works in both colour and monochrome modes. |
| **Automagic Fade Out** ⭐ | Fade out using the scene's own palettes. Works in both colour and monochrome modes. |
| **Automagic Special Effect** | The scene-palette-driven equivalent of a special fade. |

### Monochrome only

| Event | Description |
|---|---|
| **DMG Fade** | A fade for monochrome mode, with an option to flicker intermediate shades. |
| **DMG Palette Cycle** | A colour cycle for monochrome mode. |

### Utilities

| Event | Description |
|---|---|
| **Quick Load Palettes** | Load a set of palettes immediately, without a fade. |
| **Quick Load DMG Palettes** | The monochrome equivalent. |
| **Set All Palettes to One Colour** | Fill every palette slot with a single colour. |

---

## FAQ

**My fades look banded and muddy. Can this fix them?**
Yes. GB Studio calculates fades on the Game Boy with a very rough method. Fade Street works them
out on your PC and stores the result, so the steps land where your eye expects them.

**My Fade In and Fade Out events stopped working after installing this.**
That is expected. The plugin turns off automatic fades and the built-in fade events. Use
**Automagic Fade In** and **Automagic Fade Out**, which read the scene palettes you already set in
the editor.

**Which event should I start with?**
The five marked with a star, under **Fade Street - Beginner Friendly**. **Automagic Fade In** and
**Automagic Fade Out** are the closest replacements for the built-in ones.

**How do I do a day to night transition?**
Use **Standard Fade** with your daytime palettes at one end and your night palettes at the other.
Colours can move in different directions, so the sky can darken while lamps brighten.

**How do I animate water, fire or a spinning light?**
Use a colour cycle. Give it the palette slots the effect uses and the colours to move through them.
The cycle loops seamlessly.

**Can something animate and fade at the same time?**
Yes. The fade with colour cycle events do both together, so a waterfall can keep flowing while the
scene fades to night.

**How do I run two unrelated effects at once?**
Use **Multiple Fades With Colour Cycles** or **Multiple Bespoke Colour Cycles**. Each has eight
tabs and runs them all in one script, which costs less processor time and keeps them in step.

**Does it work on original Game Boy?**
Yes. **DMG Fade** and **DMG Palette Cycle** are for monochrome, and the Automagic events work in
both modes.

**How do I support both colour and monochrome from one project?**
The Automagic events handle both on their own. For the rest, wrap them in an **If Color Mode Is
Available** event and build one version for each mode.

**Why did my ROM grow so much?**
Each event stores its precalculated colours in the ROM. More steps means more data, and BlendShift
Cycling updates every frame, so it produces by far the largest scripts. Use it sparingly.

**My multi-effect event runs much longer than I expected.**
It runs for the shortest span that all its cycles fit into evenly. Cycles of 19 and 17 frames give
323 frames. Pick lengths that divide into each other, such as 25 and 50.

**The flicker option looks terrible on my screen.**
Original Game Boy screens blur frames together, which is what makes it work. Modern screens and
many emulators show the flicker directly. Leave it off unless you are targeting original hardware.

**Does it work with other plugins?**
It replaces the engine's fade, palette, scene loading, music and screen interrupt code, so another
plugin touching the same parts needs merging by hand. Compatibility files ship for ContinuousScene
and for ScreenScroll with Metatile.

---

## Media

<img src=".img/tri.webp" width="240"> <img src=".img/ball.webp" width="240"> <img src=".img/chomp.webp" width="240">
<img src=".img/bars.webp" width="240"> <img src=".img/end.webp" width="240">

---

## Memory Footprint

Measured against the stock GB Studio **4.3.0-e1** engine at default engine settings, report of 2026-08-13. Figures are the difference against a stock project: a file that replaces a stock engine file counts only the change, which is why a plugin can come out negative. Each event you use also compiles a few bytes of script into your project, on top of the fixed cost below.

| Budget | Cost |
|---|---|
| Bank 0 (HOME) | +16 bytes |
| WRAM | -2 bytes |
| Banked ROM | -159 bytes |

- **Bank 0:** 16 bytes sit in the fixed bank, in the screen interrupt code. Everything else lives in a switchable bank. See [Bank 0 (HOME) Usage](#bank-0-home-usage).
- **WRAM:** 2 bytes *less* than the stock fade code uses.
- **Banked ROM:** a 159-byte *saving*, because the stock fade code the plugin replaces is bigger than its own. That covers engine code only. The precalculated palette data each Fade Street event produces adds its own bytes on top, and a long fade or cycle can add a lot.
- **Engine WRAM headroom:** a stock GB Studio 4.3.0 project leaves about **854 bytes** of WRAM free (the engine has 7,776 bytes to work with and uses 6,922 of them). With this plugin installed roughly **856 bytes** remain. Adding more global variables to your project does not change that figure, because script memory is a fixed 3,584 byte block at stock engine settings.
- **SRAM:** not used.

---

<!-- BANK0:BEGIN -->
## Bank 0 (HOME) Usage

Bank 0 is the 16 KB fixed ROM bank shared by the GB Studio engine core, the
interrupt handlers and the GBDK runtime. Extra banked ROM is cheap to add,
bank 0 is not, so bank 0 is usually the first thing a project runs out of.

| | Bytes |
|---|---|
| Bank 0 used by this plugin | **+16** |
| Bank 0 free with this plugin installed | **1,435** of 16,384 (91% used) |

Everything else this plugin adds lives in banked ROM.

| Module | This plugin | Stock engine | Bank 0 cost |
|---|---|---|---|
| Screen interrupts | 261 | 245 | +16 |

A module that replaces a stock engine file costs only the *difference*, because
the stock version's bank 0 bytes were being spent anyway.

<details><summary>How this was measured</summary>

GB Studio 4.3.0-e1, default engine settings. Each module was compiled with the
toolchain and flags GB Studio itself uses, and the bank 0 size the compiler
recorded was read back. The stock column is the same compile of the engine file
the module replaces.

The "free" figure assumes a stock project with this plugin and nothing else.
Your own number will differ, because other plugins and any engine settings that
change what the core compiles move it too.

</details>
<!-- BANK0:END -->

## Changelog

Grouped by the date each change was merged into the official
[gb-studio-plugins](https://github.com/gb-studio-dev/gb-studio-plugins) repository.

Only bug fixes, new features and feature changes are listed. Engine version
bumps, patch regeneration, packaging fixes and documentation edits are omitted.

### 2026-06-28

- Added ContinuousScenePlugin compatibility.

### 2026-06-14

- Added custom script parameter and stack support to the events.

### 2026-02-14

- Added a compatibility file for the ScreenScroll and Metatile plugins used together.

### 2026-02-03

- Fork release.
- Fixed the crash when MOD music was playing.
