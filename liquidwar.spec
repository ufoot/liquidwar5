Summary: A unique multiplayer wargame.
Summary(fr): Un "wargame" multijoueur inédit.
Summary(de): Ein einzigartiges Kriegspiel für mehrere Spieler.
Name: liquidwar
Version: 5.6.5
Release: 1
License: GPL
Group: Amusements/Games
Source: http://freesoftware.fsf.org/download/liquidwar/liquidwar-5.6.5.tar.gz
URL: http://www.ufoot.org/liquidwar/v5
Vendor: Christian Mauduit <ufoot@ufoot.org>
Packager: Christian Mauduit <ufoot@ufoot.org>
BuildRoot: %{_tmppath}/%{name}-buildroot

%description
Liquid War is a unique multiplayer wargame. Its rules are
truely original and have been invented by Thomas Colcombet.
You control an army of liquid and have to try and eat your
opponents. A single player mode is available, but the game is
definitely designed to be multiplayer, and has network support.

%description -l fr
Liquid War est un "wargame" multijoueur inédit. Ses règles sont vraiment
originales et ont été inventées par Thomas Colcombet. L'idée est de
contrôler une armée de liquide et d'essayer de "manger" ses adversaires.
Il est possible de jouer seul, mais le jeux est conçu pour se jouer à
plusieurs, un mode réseau étant disponible.

%description -l de
Liquid War ist ein einzigartiges Kriegsspiel für mehrere Spieler. Die
Regeln sind wahrhaft neuartig und wurden von Thomas Colcombet entwickelt.
Man steuert eine flüssige Armee und muss versuchen die Gegner aufzufressen.
Es gibt einen Einzelspielermodus, aber das Spiel ist eindeutig auf mehrere
Spieler ausgelegt und unterstützt das Spielen über Netzwerk.

%description -l dk
Liquid war er et unikt multiplayer krigsspil. Reglerne er
uhyre originale og er opfundet af Thomas Colcombet. Du styrer
en hær af væske og skal prøve at æde dine modstandere.
Liquid War kan spilles alene, men er helt afgjort designet
til multiplayer, og har netværks-support.

# Preparation of the package
%prep
%setup
./configure --prefix=%{_prefix} --target=%{_target}

# Building the package
%build
make 

# Installing the package
# The "make uninstall" is important since it prevents rpm from
# putting old unused files in the archive.
%install
export DESTDIR=%{buildroot} && make uninstall && make install 

# Cleaning
%clean
rm -rf %{buildroot}

# Pre-install script
%pre

# Post-install script
# It basically adds the info page to the Top info page.
%post
if which install-info; then if test -d %{_prefix}/info; then if test -f %{_prefix}/info/dir; then install-info --info-dir=%{_prefix}/info %{_prefix}/share/info/liquidwar.info.gz; fi; fi; fi

# Pre-uninstall script
%preun

# Post-uninstall script
# It basically removes the info page to the Top info page.
%postun
if which install-info; then if test -d %{_prefix}/info; then if test -f %{_prefix}/info/dir; then install-info --info-dir=%{_prefix}/info --remove liquidwar; fi; fi; fi

%files
%defattr(-,root,root)
%{_prefix}/games/liquidwar
%{_prefix}/games/liquidwar-server
%{_prefix}/games/liquidwar-mapgen
%{_prefix}/bin/liquidwar
%{_prefix}/bin/liquidwar-server
%{_prefix}/bin/liquidwar-mapgen
%{_prefix}/share/games/liquidwar/
%{_prefix}/share/doc/liquidwar/
%{_prefix}/share/man/man6/liquidwar.6*
%{_prefix}/share/man/man6/liquidwar-server.6*
%{_prefix}/share/man/man6/liquidwar-mapgen.6*
%{_prefix}/share/info/liquidwar.info*
%{_prefix}/share/pixmaps/liquidwar.xpm
%{_prefix}/share/applications/liquidwar.desktop

%changelog

* Thu Sep 27 2007 Christian Mauduit <ufoot@ufoot.org>
- Added mapgen files, fixed deprecated RPM syntax.

* Fri Dec 13 2002 Christian Mauduit <ufoot@ufoot.org>
- Added French and German informations.

* Fri Jul 12 2002 Christian Mauduit <ufoot@ufoot.org>
- Used wildcards for man pages names, to solve a bzip2 vs gzip issue.

* Wed Jul 10 2002 Christian Mauduit <ufoot@ufoot.org>
- Managed to make GPG signing to work, so RPMs should be signed now.

* Tue Mar 12 2002 Christian Mauduit <ufoot@ufoot.org>
- Fixed some buildroot bugs
- Replaced install-info --infodir=... by --info-dir=...

* Mon Mar 11 2002 Christian Mauduit <ufoot@ufoot.org> 
- Recompiled with Allegro's rpm (fixes the error:
  "/usr/lib/liballeg.so.4.0: undefined symbol: _poly_scanline_gcol8")
- Fixed the post-install script

* Sat Feb 23 2002 Christian Mauduit <ufoot@ufoot.org> 
- Added proper "--target" support.
- Added info documentation with install and uninstall scripts

* Thu Jan 28 2002 Christian Mauduit <ufoot@ufoot.org> 
- My first rpm build of Liquid War

