# xkbklu

Another simple utility for XKB keyboard layout.

## Usage

*xkbklu* can be used to print current keyboard layout to *stdout* or switch to next one. 

**Print current layout:**

```bash
xkbklu status
```

**Switch to next layout:**

```bash
xkbklu switch
```

**Help:**

```bash
xkbklu --help
```

## Compilation and installing

**Dependencies:**

- make;
- gcc;
- libxkbfile.

**Resolving depdendencies for Debian:**

```bash
sudo apt install make gcc libxkbfile-dev
```

**Compile and install:**

```bash
make
sudo make install
```

## Uninstalling

```bash
sudo make uninstall
```

# License

*xkbklu* is released under the BSD-3-Clause License.
