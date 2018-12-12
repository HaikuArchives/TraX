# TraX
### TraX is a utility to assist in finding files on the BeOS.
TraX is not a "query" tool. It does not keep a database of files and attributes.
It is a tradition find tool. It searches the disk every time, looking for files that meet your criteria.
The find-criteria are not BeOS specific, and it does not (currently) search attributes.

## Install
#### To install:
* On Haiku, run `make install`

#### For more deatils, refer to the [Manual](Docs/Manual.html).

## Flaws
Although BeOS ships with a very powerful query mechanism, this feature has some flaws.
* It can only search on  Filesystems that keep indices. ie, It only works for BeOS (BFS) disks, not MacOS (HFS) or Windows (FAT) disks.
* It cannot seach within certain folders only, but must search across the whole disk.
* It cannot search for text included in the file.

## License
TraX is Free Software, distributed under the GPL.
See [Docs/Licence/GPL](Docs/Licence/GPL) for more info.
