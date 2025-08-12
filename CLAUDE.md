# Liquid War 5 - Project Analysis

## Project Overview

**Liquid War 5** is a classic multiplayer wargame originally created by Christian Mauduit (U-Foot) and first developed in 1998. The game features unique mechanics where players control armies of liquid and must absorb opposing forces to win.

- **License**: GPL v2
- **Language**: C (with some assembly optimizations)
- **Version**: 5.7.0
- **Platform**: Cross-platform (Linux, Windows, macOS)
- **Graphics**: Allegro 5 game library
- **Homepage**: https://ufoot.org/liquidwar/v5

## Project Structure

### Core Directories

```
liquidwar5/
├── src/           # Main source code (~100+ C files)
├── data/          # Game assets (maps, textures, sounds, music)
│   ├── map/       # Game level files (.pcx/.bmp + .txt configs)
│   ├── texture/   # Background textures (.pcx files)
│   ├── music/     # MIDI background music
│   ├── sfx/       # Sound effects (.wav files)
│   ├── font/      # Game fonts (.pcx)
│   └── back/      # Background images
├── utils/         # Utility programs (map generators, converters)
├── doc/           # Documentation generation system
├── custom/        # User-customizable content
├── misc/          # Platform-specific files, icons, packaging
└── www/           # Web components (metaserver PHP code)
```

### Key Executables Built

- `liquidwar` - Main game client
- `liquidwar-server` - Dedicated game server
- `liquidwar-mapgen` - Map generation utility

## Build System

### Dependencies

**Required:**
- **Allegro 5** - Main graphics/audio library (5.0+ required)
- **pkg-config** - For finding Allegro
- **pthread** - Threading support
- **Standard C libraries** - Network, system headers

**Optional:**
- **Python** - Documentation generation
- **LaTeX/PDFLaTeX** - PDF documentation
- **makeinfo** - Info page generation
- **gzip** - Compression

### Build Process

1. **Configure**: `./configure` (autotools-based)
2. **Build**: `make` or `gmake`
3. **Install**: `make install`

**Configuration Options:**
- `--enable-debug` - Debug build
- `--enable-static` - Static linking
- `--disable-doc-*` - Disable specific documentation formats

## Key Source Files

### Game Engine Core (`src/`)

| Module | Purpose |
|--------|---------|
| `main.c` | Program entry point |
| `game.c` | Core game logic |
| `army.c` | Army/unit management |
| `fighter.c` | Individual unit behavior |
| `move.c` | Movement algorithms |
| `mesh.c` | Game field mesh system |
| `map.c` | Level loading/management |
| `graphics.c` | Rendering system |
| `menu.c` | User interface |

### Network System
- `network.c`, `netgame.c`, `netplay.c` - Multiplayer networking
- `server.c`, `protocol.c` - Server implementation
- `sock*.c` - Socket abstraction (Unix/Windows)

### Platform Support
- `exec*.c` - Platform-specific execution
- `mutx*.c` - Mutex implementations per platform
- `thrd*.c` - Threading abstractions

### Allegro 4 to Allegro 5 Migration
- `backport.h` and `backport.c` - **Backward compatibility module** containing functions that disappeared in Allegro 5 and need to be reimplemented. This includes GUI functions, input handling, and other utilities that Liquid War 5 requires. It implements only what LW5 needs from the deprecated Allegro 4 API.
- **Pixel functions**: Use `putpixel()` and `getpixel()` from the backport layer instead of `al_put_pixel()` and `al_get_pixel()`. The backport functions work identically to the original Allegro 4 functions and should be favored for compatibility.

## Game Features

### Core Gameplay
- **Liquid Army Control**: Unique mechanic where armies flow like liquid
- **Multiplayer Support**: Up to 6 players locally or networked
- **Network Play**: TCP/IP multiplayer with dedicated server support
- **Custom Content**: User maps, textures, and music

### Technical Features
- **Cross-platform**: Linux, Windows, macOS support
- **Assembly Optimizations**: i386 assembly for performance-critical code
- **Flexible Graphics**: Multiple resolution support
- **Modular Design**: Extensible map and texture system

## Development Workflow

### Building from Source
```bash
# Configure with standard options
./configure

# Build everything
make

# Install system-wide
sudo make install

# Build documentation
make -C doc
```

### Development Commands
- `make clean` - Clean build files
- `make dep` - Update dependencies
- `make check` - Run tests
- `make package_source` - Create source distribution

### Testing the Build
- Run `./src/liquidwar` to test the game client
- Check `liquidwar --help` for command-line options
- Test network functionality with `liquidwar-server`

### Building Individual Files
- Use `make -C src/ filename.o` to build individual object files from the src directory
- Example: `make -C src/ backport.o` or `make -C src/ init.o`
- This is useful for testing compilation of specific modules during development

## Platform-Specific Notes

### Linux/Unix
- Primary development platform
- Uses X11 for graphics (through Allegro)
- Requires development headers for dependencies

### Windows
- MinGW/MSYS2 supported
- Visual C++ project files in `misc/`
- Uses DirectX backend through Allegro

### macOS
- Builds as native application
- Creates `.app` bundle with `make package_macosx_tgz`
- Handles dynamic library dependencies automatically

## Packaging System

The project includes extensive packaging support:
- **Source tarballs** (`make package_source`)
- **Binary packages** for multiple platforms
- **RPM packages** (`make package_binary_rpm`)
- **Windows installers** (NSIS-based)
- **Docker containers** for server deployment

## Legacy Considerations

This is a **20+ year old codebase** with significant historical context:
- Originally developed without version control
- Migrated from SourceForge to GitHub
- **Liquid War 6** exists but this version (5) remains widely used
- Code style reflects late 1990s C programming practices
- Contains platform compatibility layers for systems no longer common

## Current Status

- **Maintenance mode**: No active development
- **Stable**: Still compiles and runs on modern systems
- **Package availability**: Available in major Linux distributions
- **Community**: Small but dedicated user base
- **Successor**: Liquid War 6 exists but hasn't replaced this version

## For Contributors

If you want to contribute or experiment:
1. Focus on bug fixes rather than major features
2. Maintain compatibility with the existing codebase style
3. Test across multiple platforms if possible
4. Contact the original author (ufoot@ufoot.org) for guidance

The codebase, while old, demonstrates solid C programming practices and cross-platform development techniques from the era before modern game engines.
