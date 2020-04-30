from PIL import Image
from path import Path
from enum import Enum
from itertools import chain
import copy
import json

class MarkerAnimation(str, Enum):
    APPROACH = "approach"
    MISS = "miss"
    POOR = "poor"
    GOOD = "good"
    GREAT = "great"
    PERFECT = "perfect"

FILENAMES = {
    MarkerAnimation.APPROACH : [f'ma{i:02}.png' for i in range(16)],
    MarkerAnimation.MISS : [f'ma{i:02}.png' for i in range(16, 24)],
    MarkerAnimation.POOR : [f'h1{i:02}.png' for i in range(16)],
    MarkerAnimation.GOOD : [f'h2{i:02}.png' for i in range(16)],
    MarkerAnimation.GREAT : [f'h3{i:02}.png' for i in range(16)],
    MarkerAnimation.PEEARLYRFECT : [f'h4{i:02}.png' for i in range(16)],
}

METADATA = {
    anim : {
        "sprite_sheet" : f"{anim.name.lower()}.png",
        "count": len(FILENAMES[anim]),
        "columns": 4,
        "rows": len(FILENAMES[anim]) // 4
    }
    for anim in MarkerAnimation
}

def convert_folder(src: Path, dst: Path):
    dst.mkdir_p()
    sprite_sizes = set(Image.open(src/path).size for path in chain(*FILENAMES.values()))
    assert(len(sprite_sizes) == 1)
    sprite_size = sprite_sizes.pop()
    assert(sprite_size[0] == sprite_size[1])
    size = sprite_size[0]
    marker_json = {
        "name": src.name,
        "size": size,
        "fps": 30
    }
    marker_json.update(**METADATA)
    for anim in MarkerAnimation:
        meta = METADATA[anim]
        sprite_sheet = Image.new('RGBA', (size*meta["columns"], size*meta["rows"]), (0,0,0,0))
        for i, path in enumerate(FILENAMES[anim]):
            sprite = Image.open(src/path)
            x = (i % meta["columns"])*size
            y = (i // meta["columns"])*size
            sprite_sheet.paste(sprite, (x,y))
        sprite_sheet.save(dst/meta["sprite_sheet"])
    json.dump(
        marker_json,
        open(dst/"marker.json", mode='w'),
        indent=4
    )

if __name__ == "__main__":
    from argparse import ArgumentParser

    parser = ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    args.output.mkdir_p()
    for folder in args.input.dirs():
        try:
            convert_folder(folder, args.output/folder.name)
            print(f"{folder.name} : OK")
        except Exception as e:
            print(f"{folder.name} : {type(e).__name__} : {e}")