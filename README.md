# TraX
### TraX is a utility for finding files.
TraX is not a "query" tool. It does not keep a database of files and attributes. It is a traditional find tool, which searches the disk every time, looking for files that meet your criteria.
The find-criteria are not Haiku specific, and it does not (currently) search attributes.

## Varieties of TraX
TraX comes in two varieties.
* <b>FastTraX</b> is similar to the Windows find tool. Windows users should find its features very familiar.

![FastTraX](/FastTraX.PNG)

* <b>TraX</b> is a cross between the UNIX "find" command the the MacOS "Find Files" tool. It has much of the power of "find" with a Mac-like interface (a Mac on steroids that is).

![TraX](/TraX.png)

## Building, Compiling and Installing
#### To Build:
* On Haiku, run `make`
#### To Install:
* On Haiku, run `make install`

#### For more deatils, refer to the [Manual](http://htmlpreview.github.io/?https://github.com/HaikuArchives/TraX/master/Docs/Manual.html).

## Flaws in Haiku's Query System
Although Haiku comes with a very powerful query mechanism, there are some flaws that TraX does not have.
Haiku Queries..
* ... can only search on filesystems that keep indices, i.e. it only works on BFS formatted disks (no Windows, macOS or Linux filesystems).
* ... cannot search within certain folders only, but must search across the whole disk.
* ... cannot search for text inside a file.

## License
TraX is Free Software, distributed under the GPL.
See [Docs/Licence/GPL](Docs/Licence/GPL) for more info.
