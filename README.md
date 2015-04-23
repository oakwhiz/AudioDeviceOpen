# AudioDeviceOpen
Windows program that opens the default audio device and then does nothing. Somehow fixes certain audio driver bugs. Use at your own risk.

# Why?
On Windows, certain Realtek audio chipsets (e.g. ALC889) come with terrible drivers supplied by motherboard manufacturers.
However, Windows includes a generic "High Definition Audio Device" driver which can be used instead. This generic driver seems to work better, in my experience.
The only problem with the generic driver is that the audio output sometimes contains annoying high-pitched noises whenever no applications are currently playing audio.
The noises are different every time, and sometimes they are inaudible. It seems like the chip is being put into some kind of sleep mode, but maybe its buffers are not being cleared first.
Whatever the cause, it is quite annoying, especially when using headphones.
To try to fix this, I put together this C program which can be run on login with Task Scheduler. It sits in the background and continuously plays silence, in order to trick the audio chipset into not making those noises.
Sometimes it seems to work but sometimes it doesn't
