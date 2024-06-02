#!/usr/bin/env python3

from io import StringIO
from os.path import relpath
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent

VGM_FOLDER = REPO / "MD_tracks"
RES_FOLDER = HERE / "res"

res_buff = StringIO()
res_buff.write((RES_FOLDER / "resources.res.head").read_text())
res_buff.write("\n")

for track_no, fname in enumerate(sorted(VGM_FOLDER.glob("*.vgm"))):
    basename = fname.name
    title = basename.split(".")[0].title().replace("_", " ")
    varname = "XGM_" + title.replace(" ", "_").upper()
    res_line = f'XGM {varname} "{relpath(fname, RES_FOLDER)}"\n'
    res_buff.write(res_line)

    print(res_line)

(RES_FOLDER / "resources.res").write_text(res_buff.getvalue())
