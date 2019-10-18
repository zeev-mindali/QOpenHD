## QOpenHD

![OSD](https://raw.githubusercontent.com/infincia/QOpenHD/master/wiki/osd.jpg)

This is a companion app for the Open.HD drone platform.

The code is functional but very new, more of a technical preview than an alpha or beta in terms of features, but it should still be *stable* in that it shouldn't crash or have weird glitchy behavior.

Binaries are available in the [releases tab in GitHub](https://github.com/infincia/QOpenHD/releases/latest). 

### Features

* Live digital video stream in-app
* Drag and drop OSD widgets (if you've ever moved apps around on an iPhone/Android, it's exactly like that)
* Tap OSD widgets to bring up popup detail panels with more infomation
* Full control over all GroundPi settings from inside the app
* OpenGL hardware accelerated UI on every platform
    * GroundPi (using touchscreen or HDMI output+mouse)
    * Windows
    * Mac
    * Linux
    * iPhone/iPad
    * Android
* Mavlink telemetry (read-only at the moment)

### Planned features

* Full video decode acceleration (present in code but not enabled yet due to GStreamer quirks)
* Fully localized and translated UI
* LTM/MSPv2/FrSky telemetry
* Mavlink commands
    * Arm/disarm
    * Start mission
    * Setting waypoints
    * Rebooting flight controller

## State of the code

### ***[There are bugs](https://github.com/infincia/QOpenHD/issues?q=is%3Aissue+is%3Aopen+label%3Abug)***.

However, as of the `v0.1` tag the app itself should run the same on every platform. I have not even tried to run it on Linux or Windows though, so there may be some build script changes needed there.

Things like RC and video rendering are inherently dependent on platform support to some extent. For example, iOS is never going to support USB connected joysticks or TX, but will support things like bluetooth gamepads if the GroundPi is 5.8Ghz (the interference is **horrible** and easily noticed if the GroundPi is 2.4Ghz, stick movements have huge lag). 

| Feature | GroundPi | Windows | Mac | Linux | Android | iOS | 
| --- | --- | --- | --- | --- | --- | --- |
| Video | Yes | Untested | Yes | Untested | [High Latency](https://github.com/infincia/QOpenHD/issues/1) | Yes |
| Settings | Yes | Yes | Yes | Yes | Yes | Yes |
| RC | Yes | [Disabled](https://github.com/infincia/QOpenHD/issues/10) | [Disabled](https://github.com/infincia/QOpenHD/issues/10) | [Disabled](https://github.com/infincia/QOpenHD/issues/10) | [Disabled](https://github.com/infincia/QOpenHD/issues/10) | [Disabled](https://github.com/infincia/QOpenHD/issues/10) |
| Backlight Control | Yes | N/A | N/A | N/A | N/A | N/A |
| Mavlink | Yes | Yes | Yes | Yes | Yes | Yes |
| LTM | No | No | No | No | No | No |
| FRSky | No | No | No | No | No | No |
| MSPv2 | No | No | No | No | No | No |
| Voice Feedback | Yes | Yes | Yes | Yes | Yes | Yes |

## Platforms

The code is mostly C++ and uses the Qt framework, specifically QtQuick which is designed for portability and renders using OpenGL.

This allows the same app to run on Windows, Mac, Linux, iOS, and Android, as well as directly on the GroundPi itself (either using the official touchscreen or an HDMI screen + mouse).

## Code architecture

The core is C++ (in `src`), and the UI is designed with QtQuick, which is an OpenGL accelerated, declarative UI framework. You can find the UI files in `qml`.

QtQuick is designed to be an MVC code architecture, and QOpenHD follows that pattern for the most part. The UI layer is separated into declarative UI "forms" with a matching logic-only layer them, you can see that in the file names for most of the components (there will be a -Form.ui.qml for each one).

There is a small amount of "glue" code in the QML layer, the language is basically Javascript but designed to integrate with QML. There is more of this in the QML layer than I would like at the moment but some of it can
be moved down to C++ in the future.

## Drag-and-drop widgets

![Ground Pi Radio Settings](https://raw.githubusercontent.com/infincia/QOpenHD/master/wiki/dragdrop-adjustment.jpg)

The OSD widgets can all be dragged around the screen and position wherever you like. They will stay where you put them after a reboot.

To move them around:

1. Tap and hold a widget until it starts to wiggle and "unlocks". 
2. Drag the widget to a new location
3. Tap and hold it again (or hit the checkmark button) to lock it again.

While a widget is unlocked, a box will be drawn around it to indicate where the edges are. This makes it easier
to avoid overlapping widgets, or overflowing the edge of the screen. Some widgets, like flight mode, are larger 
than they seem due to variable sized contents.

If you can't place the widget exactly where you want by touch alone, you can use the fine adjustment controls on
the screen to move it up/down/left/right pixel-by-pixel, as well as set horizontal/vertical centering. When centering is enabled, the widget will "snap" back to the horizontal/vertical center once you lock it again.

On platforms that have resizable windows, you can also set a corner affinity to ensure that the widget stays where you want it when the window is resized.

Only one widget can be unlocked at a time, to prevent accidentally moving the others when positioning them near each other.

Widgets can all be completely enabled/disabled individually in settings, and some of them have settings of their
own that can be accessed by tapping once on the widget to open the detail panel.

## Telemetry

The Open.HD telemetry is handled via UDP when the app is running on a phone or a computer, and via the same shared memory system used by the original OSD when running on GroundPi.

For vehicle telemetry, only Mavlink is fully integrated at the moment, but [other protocols are being added](https://github.com/infincia/QOpenHD/issues/17).

## Settings

![Ground Pi Radio Settings](https://raw.githubusercontent.com/infincia/QOpenHD/master/wiki/settings-radio.png)

The app has a full touch interface for GroundPi settings, including radio frequency, video resolution and bitrate, the Wi-Fi and Ethernet hotspot settings, etc.

Some settings are treated specially and presented in a specific tab. This allows them to use appropriate UI controls and to limit the possible choices where that makes sense.

However *all* settings, including any new settings added to the GroundPi in the future, can be changed from the app. This does not require an app update, the new settings will simply show up in the "other" tab with a plain text editing field ensuring they can be changed no matter what the value is supposed to be. Just be careful not to break anything by entering the wrong value in those fields :)

**Note**: When the app is running on the GroundPi itself, changing some settings requires a USB keyboard (those that use a text field rather than a dropdown or number picker).

There is an on-screen keyboard for the ground station but it is not enabled yet. Once it is tested and working well, this will not be required anymore.

## RC

This is currently handled a little differently on the GroundPi, because Open.HD itself can already handle RC
and therefore it works the same as it always has.

On other platforms, RC is currently disabled via compiler flag to prevent anyone from using it and accidentally causing a flyaway or getting injured. The code is not yet finished and has a few bugs to resolve before it can be trusted.

## Video streaming

On the GroundPi, the app is simply an overlay on `hello_video` just like the original OSD, so video should work exactly the same as it always has.

On other platforms, `QtGStreamer` is used to decode and render the video stream using available hardware decoders and OpenGL. This is the same code that QGroundControl uses at the moment.

The QtGStreamer code itself is very old and mostly unmaintained upstream, it is likely to be replaced very soon with another video component based on qmlglsink or one based on libavcodec + GL shader rendering that I have been working on.

On Android there seem to be some hardware acceleration issues with the currently committed QtGStreamer code. QGroundControl has a newer version of it where they have implemented hardware acceleration on Android, but it didn't actually work on my test device (the `androidmedia` GStreamer plugin can't be loaded, so the accelerated decoders are not avalable).

Video should be working fine on iOS, Mac, Windows and Linux, as most machines can handle software decoding without trouble (it works, it's just not efficient and wastes battery power).

## Building

Binaries and GroundPi images are available in the [releases tab in GitHub](https://github.com/infincia/QOpenHD/releases/latest).

However if you still want to build it yourself, you can.

These are only a rough outline rather than exhaustive build steps (that would fill several pages and be very complciated to write and keep updated).

The build process is dependent on which platform you're building *on* and which platform you're building *for*. It can be quite complicated and irritating when something doesn't work right, or if you aren't familiar with all these development frameworks and toolchains.

This will be less complicated once QtGStreamer is replaced.

In general, you'll need Qt 5.13.1+ and the GStreamer development package, specifically version 1.16.1 (which seems to handle video packet corruption much better than 1.14.4 does).

#### Mac

1. Install Xcode from the Mac App Store.

2. Install Qt using the [Qt online installer](https://www.qt.io/download-qt-installer)

3. Have the Qt Installer download Qt 5.13.1+ and Qt Creator

4. Download the [GStreamer development kit](https://gstreamer.freedesktop.org/download/), both the Runtime and Development packages.

Once you have those installed you can open `QOpenHD.pro` with Qt Creator, build and run it.

#### Windows

I haven't tried building on Windows yet, but it should be largely identical to the Mac instructions, except you'll need Visual Studio rather than Xcode, and building code on Windows can be complex if you aren't familiar with the process.

I will update these instructions once I have a chance to try it.

#### Linux

1. Install Qt using the package manager (if they have a new enough version of Qt), or the [Qt online installer](https://www.qt.io/download-qt-installer)

2. If using the Qt online installer, have it download Qt 5.13.1+ for Linux 

3. Install GStreamer development packages from the package manager. On Ubuntu this would be `gstreamer1.0-gl`, `libgstreamer1.0-dev`, `libgstreamer-plugins-good1.0-dev`, and `libgstreamer-plugins-base1.0-dev`, and those should pull in any others that are needed as well.

You can then open `QOpenHD.pro` using Qt Creator, build and run the app.

#### Android

1. Install [Android Studio](https://developer.android.com/studio)

2. Use Android Studio to install Android SDK level 28, along with NDK r18b and build toolchain.

3. Install Qt using the [Qt online installer](https://www.qt.io/download-qt-installer)

4. Have the Qt Installer download Qt 5.13.1+ for Android 

5. Download the [GStreamer development kit](https://gstreamer.freedesktop.org/download/) for Android 1.16.1

6. Unzip the GStreamer archive inside the QOpenHD directory.

You can then open `QOpenHD.pro` using Qt Creator and set up the Android kit (left side of the Projects tab), build and run the app on your device.

I have never tried to run it on a simulator, I doubt it would work very well (particularly GStreamer).

#### GroundPi

Building GroundPi images with QOpenHD integrated is handled by the [Open.HD image builder](https://github.com/infincia/Open.HD_Image_Builder) as it is very complicated to get right. It requires a specific set of packages to be preinstalled on the image, and requires building Qt from source to enable `eglfs`. 

Prebuild images are available on the [releases tab in GitHub](https://github.com/infincia/QOpenHD/releases/latest). 

For the GroundPi, you want to download the `*.img.xz` file.

To write it to the MicroSD card, use [Etcher](https://www.balena.io/etcher/) or, if you're familiar with the command line, `dd` or `cat` (both work, `cat` is more "correct").

**Note**: these images are only tested on the GroundPi at the moment, they should work on the AirPi too but I have not tried it. 

The images do respect the setting that disables the OSD, but do not currently disable QOpenHD when running on the air side. In practice it may not matter as QOpenHD only uses 30-40MB of memory and barely uses the CPU unless
the settings area is open and the list is being scrolled.


