# tiagoprn's custom dwm-flexipatch build

## PRE-REQUISITES

- I customize the dwm status bar using [my fork of dwmblocks](https://github.com/tiagoprn/dwmblocks), which is in turn a fork of lukesmith's fork of it. The scripts that dwmblocks triggers are at `$HOME/apps/scripts/bin/statusbar`.

## INSTRUCTIONS
- Edit **only** the files with the `*.def.h` extension. The `.h` only files hold a copy of the previous configuration, and are not versioned.
- After finished editing, run `build.sh`. **This will override the `.h` files** with the new (versioned) content.
