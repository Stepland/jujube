# Folder Structure
This describes how one should lay out files and folders for jujube to properly work with them.

## Overview
A vanilla install of jujube should look like this :

```
jujube/
├─ assets/
├─ data/
├─ markers/
├─ songs/
└─ jujube
```

## `assets` folder
You should not touch this under normal circonstances, it contains things jujube needs to display the fallback theme

## `data` folder
For now this folder only contains a json representation of the user's preferences

## `markers` folder
This folder contains your markers, the structure is very simple. Just take a look at what's already there.

## `songs` folder
At startup, jujube will search for songs recursively from here, this means you *can*, unlike stepmania, have subfolders.

If you don't now how you should arrange you songs, I suggest sticking to the stepmania way :

```
songs/
└ pack/
   └ song/
     ├ song.memon
     ├ song.ogg
     └ song.png
```