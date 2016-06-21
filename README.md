## Packages
Packages for following distributions are available from [here](https://software.opensuse.org/download.html?project=home%3Aelchaschab&package=breezy):
- Debian 8.0
- Fedora 22
- Fedora 23
- openSUSE 13.2
- openSUSE 42.1
- Ubuntu 15.04
- Ubuntu 15.10
- Ubuntu 16.04

    
## Build on Debian or Ubuntu
    # clone repo
    git clone https://github.com/kallaballa/Breezy.git
    cd Breezy
    
    # install build dependencies
    apt-get update
    apt-get install libpulse-dev libgstreamer0.10-dev gstreamer0.10-alsa gstreamer0.10-plugins-base gstreamer0.10-plugins-bad gstreamer0.10-plugins-good
    
    # build
    make
    
