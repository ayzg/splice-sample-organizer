# splice-sample-organizer

### Splice removed the ability to browse your sample library after unsubscribing. It's api is private. 
**I made this program help me to organize my own library, as with most producers I had a titanic amount of samples.
Protip: backup your entire splice library and organize before unsubsribing - or pay Splice 15$ to do it.**

This program organizes audio files from a Splice Samples folder into a
specified destination folder based on their content. The folder structure
and criteria for file organization are as follows:

- Drums:
  - 808: Files containing "808" in the filename
  - Snare: Files containing "snare," "_snr," or "snr_" in the filename
  - Kick: Files containing "kick," "_kck," or "kck_" in the filename
  - Clap: Files containing "clap," "_clp," or "clp_" in the filename
  - Hat: Files containing "hat," "ht_," or "_ht" in the filename
  - Other: Files containing "drum," "_drm," or "drm_" in the filename

- Other:
  - Loop: Files containing "loop" in the filename
  - Other: Files that don't match any of the above criteria

The program prompts the user for the source and destination folder names and
organizes the files accordingly. It also provides an option to print detailed
information about each file move.

Additional Requirements:
- Set the environment variable "MAX_PATH" to a greater value (e.g., 32767) to
  be able handle long file paths.
