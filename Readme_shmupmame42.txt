Shmupmame v4.2
by nimitz@shmupsfourm

Based on mame 0.148/mameplus

The main goal of this fork is to remove the delay caused by frame/sprite buffer 
emulation in the various mame drivers, the result is that games respond faster
by at least one frame. The downside is that the background layer will not be completely
synced with the sprite layer. That being said, if you want the game to *feel* like
it does in an arcade setting, the benefits of this build far outweights the slight
visual downsides. The list of drivers with loer latency/input lag is at the end of this file.


A few weeks ago I decided to take another look at namco system 1 and realized it has a MCU to process input, investigated a bit and realized 
that I could make the input ports available to the CPU faster by sending them to shared ram instead of letting the MCU do it. Result is one 
less frame of lag for all games on namco system 1, then I decided to try something similar for namco system 2 and it worked... 

So pushing the idea a little further I decided to check some other still laggy shmups to see if it was possible to feed the CPU the input 
port values a bit faster. This is basically a new way to reduce input lag that doesn't have any disadvantage (unlike the removal of sprite 
buffers).

New drivers/games fixed in that manner:
-Namco System 1 (Dragon Spirit, Dangerous seed, Blast off, Galaga 88, etc)
-Namco System 2 (Dragon Saber, Phelios, Ordyne, Cosmo gang, etc)
-Psikyo old hardware (Strikers 1945, Gunbird, Sengoku Ace, Sengoku Blade)
-Mars Matrix (this was a game specific fix, most of the other cps2 games didn't have input delay)


In addition, version 4.2 adds quite a few feature to make shmupmame more solid than ever those new features are:
-Added "cleanratio" option which finds the largest integer scaling for the current view and then sets the correct aspect ratio
-Added "Viewpercent" option which determines which percentage of the screen will be used for drawing the game, can be used in combination with cleanratio or alone
to specify an exact view size (range is floating point 0-1, 1 being 100%).
-Added autofire settings saving for individual games
-Added autofire rate display in Hertz in addition to "delays"
-Added functionality for different autofire rates for main buttons and custom buttons
-Added a few more artwork types (scanlines every 4 lines)


Over the years mame has become quite a monster. Version 0.148 has 28,403 sets! and the vast majority of these are not "arcade games" in any
way shape or form. This caused a few problems namely that the UI version took forever to load due to the massive amount of "useless" sets.
So I removed all the sets that fell into these categories: 
	-Fruit Machines
	-Mechanical (no display) "games"
	-Gambling/Casino "games" (video poker etc)
	-Pinball chip emulation
	-The Mahjong only drivers
	-Drivers without a single working game
	
The end result is that this version has "only" 7,300 sets instead of the 28,403 (of Mame 0.148), Has a much leaner binary and takes a lot less time to fire up.


This version being based on Mameplus also has support for custom buttons and autofire.


List of lower input lag (lagless) drivers:
			cave
			m72
			m107
			namcos1
			pgm
			cps1
			cps2
			cps3
			nmk16
			taito_f2
			taito_f3
			taito_l
			psikyo
			toaplan1
			gp9001 (most toaplan2)
			seibuspi
			raiden
			commando
			exedexes
			slapfght
			twincobr
			twin16
			armedf
			mcatadv
			bbusters
			bionicc
			blktiger
			boogwing
			cninja
			crshrace
			darkseal
			dassault
			dec8
			deco32
			dooyong
			dynduke
			gaelco2
			gng
			hcastle
			karnov
			lastduel
			lemmings
			lwings
			m92
			psikyosh
			rohga
			sidearms
			srumbler
			tigeroad
			toki
			vaportra
			wwfwfest
