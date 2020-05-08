from math import sqrt, ceil
from PIL import Image
from path import Path
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("files", type=Path, nargs="+")
args = parser.parse_args()

images = [Image.open(file) for file in args.files]
assert len(set(image.size for image in images)) == 1
sprite_size = images[0].size
sheet_square_side = ceil(sqrt(len(images)))
sheet = Image.new(
    'RGBA',
    (
        sheet_square_side*sprite_size[0],
        sheet_square_side*sprite_size[1]
    ),
    (0,0,0,0)
)
for index, sprite in enumerate(images):
    x = (index % sheet_square_side)*sprite_size[0]
    y = (index // sheet_square_side)*sprite_size[1]
    sheet.paste(sprite, (x,y))
sheet.save("sheet.png")

