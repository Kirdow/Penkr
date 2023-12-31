# Penkr <a href="https://github.com/Kirdow/Penkr/blob/master/LICENSE"><img src="https://img.shields.io/badge/license-MIT-green.svg"></a>
Penkr is a C++ port of [ohyicong/decrypt-chrome-passwords](https://github.com/ohyicong/decrypt-chrome-passwords). I made this because I wanted to show my family why they need a proper password manager and why not to store passwords directly in the browser.

## Why not use the python script?
Python isn't as straight forward as a standalone exe file and some dll files. So I decided to make this C++ port as I don't wanna go mess around installing Python on their PC. This project runs on 2 exe files, and libraries use 2 dll files or are linked statically, the rest is built into Windows with the C++ redistributable installation that most people already have installed.

## And what IS 'Penkr'?
The name consists of 2 parts.

The first part is ``Pen`` which comes from ``Pen-testing`` which also comes from ``Penetration Testing``. But also, ``Pen`` is also what is used to describe ``Pen-drives`` which is something I carry with me all the time, to both run portable Linux on any machine, as well as having most of my files with me.

The second part is ``Kr`` which is the first two consonants in my nickname.

# Setting up
This repository is a bit tricky to setup but premake should get most of it done for you.

### Pre-Requisites:
- Penkr is using Visual Studio 2022. If you use another version of Visual Studio, you need to edit ``scripts/Win-GenProjects.bat`` and change ``vs2022`` to whichever IDE you use. For a list of all IDEs supported by premake, visit [Premake's website](https://premake.github.io/docs/using-premake).

### Setup:
1. Clone the repository. Remember to clone recursively ``git clone --recursive https://github.com/Kirdow/Penkr`` as it does use a submodule for Crypto++.
2. Open ``scripts/`` and run ``Setup.bat``. The python script should go through and install all the requisites needed in order to build and run the project.
3. Inside ``Penkr/vendor/cryptopp`` there is a file named ``cryptlib.vcxproj``. Open it using Visual Studio.
4. At the top for the build, select ``Release`` and ``x64``.
5. In the project list, right click ``cryptlib`` and hit ``Build``.
6. Wait until the build finishes.
7. At the top for the build, select ``Debug`` and ``x64``.
8. In the project list, right click ``cryptlib`` and hit ``Build``.
9. Once the build finishes, close Visual Studio.
10. Now go back into the repository folder, and open ``Penkr.sln`` using Visual Studio.
11. At the top, select either Debug, Release or Dist, then under ``Build``, click ``Build Solution``.
12. Now in the repository folder, your binaries will be in the ``bin/<config>/Penkr`` directory.
13. In order for ``PenkrClear.exe`` to work with ``Penkr.exe`` it has to be moved to the same folder. Simply navigate to ``bin/<config>/PenkrClear`` and move ``PenkrClear.exe`` into ``bin/<config>/Penkr``.
14. Some DLLs aren't automatically copied to the binary folder, in case you're missing DLL files after the build, you'll find their locations below.

# Running Penkr
- Run ``Penkr.exe`` to fetch the passwords. Current supported browsers are listed below. Once it's finished it will save all passwords to pass.csv in the current working directory.
- ``pass.csv`` contains passwords across all browsers stored in columns of ``index``, ``url``, ``username`` and ``password``. Note that for usernames and passwords containing a comma the format may break. URLs containing a comma is handled though.
- Run ``PenkrClear.exe`` to remove the ``pass.csv`` file. This does the same as just deleting it normally except it doesn't stay in the recycle bin. I made this in order to avoid the risk of having someone getting the file back via the recycle bin. It still risk being reinstated with disk recovery apps though.
- If ``PenkrClear.exe`` successfully removes the file you should hear a ``recycle`` sound effect. Any other sound indicates that the file couldn't be removed. This could be due to the file being open in something like Microsoft Excel or similar.

# Currently supported browsers
Only Chromium based browser are supported. The currently supported browsers are:
- Google Chrome (Chromium, Chrome, Chrome Beta & Chrome Canary)
- Brave
- Opera (Opera & Opera GX)
- Microsoft Edge

This list may be outdated at some points, so check [directories.cpp](./Penkr/src/directories.cpp) to see the most up to date list currently in use.

# Required DLLs
- Crypto++ (see [their website](https://www.cryptopp.com/) and [their github](https://github.com/weidai11/cryptopp)) - This one is statically linked and shouldn't require a DLL. But its ``.lib`` file is located in ``Penkr/vendor/cryptopp/x64/Output/<config>/cryptlib.lib``.
- SQLiteCpp (see [their website](https://srombauts.github.io/SQLiteCpp/) and [their github](https://github.com/SRombauts/SQLiteCpp)) - This one is pre-compiled for the repository, and is located in ``Penkr/vendor/sqlite/`` with the ``.lib`` files at ``lib/sqlite3.lib`` and ``lib/SQLiteCpp.lib``, as well as the ``.dll`` files at ``dll/sqlite3.dll`` and ``dll/SQLiteCpp.dll``.
- Crypt32 (see [Microsoft Docs](https://learn.microsoft.com/en-us/windows/win32/seccrypto/crypt32-dll-versions)) - This one is used the decrypt the initial key as is included with your Windows installation. This is simply linked as ``Crypt32.lib`` and is located at ``C:/Windows/System32/crypt32.dll``.

# License
This software is provided with [MIT License](https://github.com/Kirdow/Penkr/blob/master/LICENSE).