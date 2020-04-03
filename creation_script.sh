#!/bin/bash

mkdir -p Home/WesternForest/SpellCastingAcademy/{Lessons,PracticeRoom/Box}

mkdir -p Home/NorthernMeadow/EasternMountains/Cave/{Staircase,DarkCorridor/DankRoom/{SmallHole,Tunnel/StoneChamber/Portal/TownSquare/{Marketplace,Library/BackRoom,RockyPath/Farm,ArtisanShop,BrokenBridge/Clearing/OminousLookingPath/CaveOfDisgruntledTrolls/Slide/KernelFiles/{Paradise,MoreKernelFiles}}}}

find Home -type f -exec chmod 777 {} \;

#Home
cp files/items/WelcomeLetter.txt Home/

#WesternForest
cp files/items/Sign.txt files/items/BackSign.txt Home/WesternForest/

#SpellCastingAcademy
cp files/items/HurryingStudent.txt Home/WesternForest/SpellCastingAcademy/

#Lessons
cp files/items/Professor.txt Home/WesternForest/SpellCastingAcademy/Lessons/

#PracticeRoom
cp files/items/Instructions.txt files/items/PracticeDummy1.txt files/items/PracticeDummy2.txt files/items/PracticeDummy3.txt files/items/PracticeDummy4.txt files/items/PracticeDummy5.txt Home/WesternForest/SpellCastingAcademy/PracticeRoom/

#NorthernMeadow
cp files/items/Pony.txt Home/NorthernMeadow/

#EasternMountains
cp files/items/OldMan.txt files/items/OldManuscripts.txt Home/NorthernMeadow/EasternMountains/

#Staircase
cp files/items/StaircaseSign.txt Home/NorthernMeadow/EasternMountains/Cave/Staircase/

#DankRoom
cp files/items/Boulder.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/

#Tunnel
cp files/items/Rat.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/

#TownSquare
cp files/items/RandomCitizen1.txt files/items/RandomCitizen2.txt files/items/DistraughtLady.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/

#Marketplace
cp files/items/Vendor.txt files/items/rmSpell.txt files/items/mkdirSpell.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/Marketplace/

#Library
cp files/items/HistoryOfTerminus.txt files/items/NostalgiaForHome.txt files/items/TotallyRadSpellbook.txt files/items/PaperbackRomance.txt files/items/InconscpicuousLever.txt  Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/Library/  

#BackRoom
cp files/items/Grep.txt files/items/Librarian.txt files/items/PracticeBook.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/Library/BackRoom/

#RockyPath
cp files/items/LargeBoulder.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/RockyPath/

#Farm
cp files/items/EarOfCorn.txt files/items/Farmer.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/RockyPath/Farm/

#ArtisanShop
cp files/items/StrangeTrinket.txt files/items/ClockWorkDragon.txt files/items/Artisan.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/ArtisanShop/

#Clearing
cp files/items/CryingMan.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/BrokenBridge/Clearing/

#OminousLookingPath
cp files/items/ThornyBrambles.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/BrokenBridge/Clearing/OminousLookingPath/

#CaveOfDisgruntledTroll
cp files/items/UglyTroll.txt files/items/UglierTroll.txt files/items/AbsolutelyHideousTroll.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/BrokenBridge/Clearing/OminousLookingPath/CaveOfDisgruntledTrolls/

#KernelFiles
cp files/items/Certificate.txt files/items/InstructionsKernel.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/BrokenBridge/Clearing/OminousLookingPath/CaveOfDisgruntledTrolls/Slide/KernelFiles/

#MoreKernelFiles
cp files/items/M_txt.txt files/items/L_txt.txt files/items/U_txt.txt files/items/V_txt.txt files/items/R_txt.txt files/items/S_txt.txt files/items/W_txt.txt files/items/T_txt.txt files/items/Q_txt.txt Home/NorthernMeadow/EasternMountains/Cave/DarkCorridor/DankRoom/Tunnel/StoneChamber/Portal/TownSquare/BrokenBridge/Clearing/OminousLookingPath/CaveOfDisgruntledTrolls/Slide/KernelFiles/MoreKernelFiles/

#Set file permissions to read only (it goes recursively through the contents of the Home directory and all of its subdirectories)
find Home -type f -exec chmod 660 {} \; 

#Set directory permissions to read and execute (allows listing of the contents of the directory as well as entering it)
find Home -type d -exec chmod 770 {} \; 
