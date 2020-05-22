from math import sqrt, ceil
from PIL import Image
from path import Path
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("files", type=Path, nargs="+")
parser.add_argument("--columns", type=int)
args = parser.parse_args()

images = [Image.open(file) for file in args.files]
assert len(set(image.size for image in images)) == 1
sprite_size = images[0].size

columns = args.columns or ceil(sqrt(len(images)))
rows = len(images) // columns
width = columns * sprite_size[0]
height = rows * sprite_size[1]
sheet = Image.new('RGBA', (width, height), (0,0,0,0))
for index, sprite in enumerate(images):
    x = (index % columns)*sprite_size[0]
    y = (index // columns)*sprite_size[1]
    sheet.paste(sprite, (x,y))
sheet.save("sheet.png")

