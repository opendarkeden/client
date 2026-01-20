# TextLib - Text Rendering Library for OpenDarkEden

## Directory Structure

```
Client/TextLib/
├── README.md              (this file)
├── CTextEdit.h            (STB textedit adapter)
├── stb_textedit.h         (MUST BE DOWNLOADED - see below)
├── CGlyphCache.h/cpp      (Font Atlas implementation)
├── CTextLayout.h/cpp      (Text layout engine)
└── CTextSpriteBatch.h/cpp (Batch rendering)
```

## Required: Download stb_textedit.h

**STB Textedit** is a single-header text editing library used by Dear ImGui.

### Download Method 1: curl
```bash
cd Client/TextLib
curl -O https://raw.githubusercontent.com/nothings/stb/master/stb_textedit.h
```

### Download Method 2: wget
```bash
cd Client/TextLib
wget https://raw.githubusercontent.com/nothings/stb/master/stb_textedit.h
```

### Download Method 3: Manual
1. Visit: https://github.com/nothings/stb
2. Find `stb_textedit.h` in the repository
3. Download and save to `Client/TextLib/stb_textedit.h`

## What is STB Textedit?

STB Textedit provides:
- Cursor movement (arrow keys, Home/End, Ctrl+arrows)
- Text selection (Shift+arrows, Ctrl+A)
- Delete operations (Delete, Backspace)
- Clipboard operations (Ctrl+X/C/V)
- Undo/Redo (Ctrl+Z/Y)
- UTF-8 ↔ UTF-32 conversion

It is used by Dear ImGui and is production-proven code.

## Integration Status

- [ ] stb_textedit.h (needs to be downloaded)
- [x] CTextEdit.h (adapter created)
- [ ] CGlyphCache implementation
- [ ] CTextLayout implementation
- [ ] CTextSpriteBatch implementation
