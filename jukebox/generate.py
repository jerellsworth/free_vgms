#!/usr/bin/env python3

from io import StringIO
from os.path import relpath
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent

VGM_FOLDER = REPO / "MD_tracks"
RES_FOLDER = HERE / "res"
SRC_FOLDER = HERE / "src"

res_buff = StringIO()
res_buff.write((RES_FOLDER / "resources.res.head").read_text())
res_buff.write("\n")

#fn_vars_buff.write("const u8 *XGM_PTRS[NUM_XGMS] = {\n")
#fn_titles_buff.write("const char *XGM_TITLES[NUM_XGMS] = {\n")
fn_vars = []
fn_titles = []

for track_no, fname in enumerate(sorted(VGM_FOLDER.glob("*.vgm"))):
    basename = fname.name
    title = basename.split(".")[0].title().replace("_", " ")
    varname = "XGM_" + title.replace(" ", "_").upper()
    res_line = f'XGM {varname} "{relpath(fname, RES_FOLDER)}"\n'
    res_buff.write(res_line)
    fn_vars.append(varname)
    fn_titles.append(title)

(RES_FOLDER / "resources.res").write_text(res_buff.getvalue())

h_buff = StringIO()
h_buff.write("#ifndef SONGS_H\n")
h_buff.write("#define SONGS_H\n")
h_buff.write("#include <genesis.h>\n")
h_buff.write(f"#define NUM_XGMS {track_no + 1}\n")
h_buff.write("extern const u8 *XGM_PTRS[NUM_XGMS];\n")
h_buff.write("extern const char *XGM_TITLES[NUM_XGMS];\n")
h_buff.write("#endif\n")
songs_h = SRC_FOLDER / "songs.h"
songs_h.touch(exist_ok=True)
songs_h.write_text(h_buff.getvalue())

c_buff = StringIO()
c_buff.write('#include "songs.h"\n')
c_buff.write("const u8 *XGM_PTRS[NUM_XGMS] = {\n")
c_buff.write(",\n".join(fn_vars))
c_buff.write("};\n")
c_buff.write("const char *XGM_TITLES[NUM_XGMS] = {\n")
c_buff.write(",\n".join(f'"{t}"' for t in fn_titles))
c_buff.write("};\n")

songs_c = SRC_FOLDER / "songs.c"
songs_c.touch(exist_ok=True)
songs_c.write_text(c_buff.getvalue())
