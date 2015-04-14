# Zombu for Linux 64bit

This package contains the binaries of the Zombu Game for Linux 64bit.

Just run the `Zombu` executable to launch the game.

If you're bored, head over to the `res` folder and edit some things =)

## Supported distributions

Should work on any recent Linux distribution with Qt support.

Tested, working out of the box:
- Ubuntu 14.04
- Linux Mint 17
- Fedora 21
- OpenSUSE 13.2

Tested, working after installing Qt (see Dependencies):
- Ubuntu Studio 14.04
- Xubuntu 14.04

Tested, not working:
- Debian Wheezy 7.8 (Qt5 is available in `wheezy-backports`, but `libc6 2.14` or newer isn't available) 

This package does not support 32bit systems !

## Dependencies

Depends on Qt5, which is already installed in nearly all major distributions.

On Xubuntu, Ubuntu Studio or other Debian-based systems which don't work out of the box you should be able to install the required packages with:

    sudo apt-get install libqt5widgets5

## License

Zombu Copyright (c) 2014 - 2015 Clark Winkelmann

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Links

- Website: <http://zombu.rip/>
- Source code: <https://github.com/clarkwinkelmann/zombu>
- Support & Issues: <https://github.com/clarkwinkelmann/zombu/issues>

