import argparse
from path import Path
from PIL import Image
import json
from faker import Faker
fake = Faker()

parser = argparse.ArgumentParser()
parser.add_argument("input_folder", type=Path)
parser.add_argument("output_folder", type=Path)
parser.add_argument("--music-folder-mode", action="store_true", dest="music_folder_mode")
parser.add_argument("--clean", action="store_true")
parser.add_argument("--random-name", action="store_true", dest="random_name")

args = parser.parse_args()

def create_song_folder_from_image(image: Path):
    song_name = fake.sentence().strip(".") if args.random_name else image.stem
    song_artist = fake.name()
    song_folder = args.output_folder / song_name
    song_folder.mkdir()
    output_image = song_folder / f"{song_name}.jpg"
    Image.open(str(image)).save(str(output_image))
    memon = {
        "version": "0.1.0",
        "metadata": {
            "song title": song_name,
            "artist": song_artist,
            "music path": "",
            "album cover path": output_image.name,
            "BPM": 120,
            "offset": 0,
        },
        "data": {
            "BSC" : {
                "level": 1,
                "resolution": 4,
                "notes": []
            }
        }
    }
    json.dump(
        memon,
        open(song_folder/f"{song_name}.memon", "w"),
        indent=4,
        sort_keys=True,
    )

if args.clean:
    args.output_folder.rmtree_p()
    args.output_folder.mkdir()

def walk_music_folder(folder: Path):
    for album in folder.dirs():
        yield from album.files()

files = walk_music_folder(args.input_folder) if args.music_folder_mode else args.input_folder.files()

for file in files:
    try:
        Image.open(str(file))
    except IOError as e:
        print(f"{file} : {e}")

    try:
        create_song_folder_from_image(file)
    except Exception as e:
        print(f"{file} : {e}")
    
    print(f"{file} : ok")