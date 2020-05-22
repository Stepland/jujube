from itertools import product
from math import sqrt, ceil
from PIL import Image
from path import Path
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("file", type=Path)
parser.add_argument("sprite_size", type=int)
parser.add_argument("-o", "--output-folder", type=Path)
parser.add_argument("--name-prefix", default="")
parser.add_argument("--name-pattern", default="{i:0{n}}{ext}")
args = parser.parse_args()

if args.output_folder is not None:
    args.output_folder.mkdir_p()
else:
    args.output_folder = Path("./")
assert args.sprite_size > 0, f"Invalid sprite_size : {args.sprite_size}"
image = Image.open(args.file)
width, height = image.size
if width % args.sprite_size != 0:
    print(f"Error : image width ({width}) is not a multiple of {args.sprite_size}")
    exit(1)
if height % args.sprite_size != 0:
    print(f"Error : image height ({height}) is not a multiple of {args.sprite_size}")
    exit(1)
number_of_sprites = (width // args.sprite_size) * (height // args.sprite_size)
for i, (y, x) in enumerate(product(range(height // args.sprite_size), range(width // args.sprite_size))):
    sprite = image.crop(
        box=(
            x*args.sprite_size,
            y*args.sprite_size,
            (x+1)*args.sprite_size,
            (y+1)*args.sprite_size
        )
    )
    sprite.save(
        args.output_folder/
        args.name_prefix+
        args.name_pattern.format(
            i=i,
            n=len(str(number_of_sprites)),
            ext=args.file.ext
        )
    )