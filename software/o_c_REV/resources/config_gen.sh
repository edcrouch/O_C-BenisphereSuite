#!/bin/sh
# Autogenerates config file for customizable firmware
# Run from source directory, e.g. ./resources/config_gen.sh

filename=".hemisphere_config_custom.h"

applets=(
	"HEM_ADSREG|DECLARE_APPLET(  8, 0x01, ADSREG)"
	"HEM_ADEG|DECLARE_APPLET( 34, 0x01, ADEG)"
	"HEM_ASR|DECLARE_APPLET( 47, 0x09, ASR)"
	"HEM_ATTENUATEOFFSET|DECLARE_APPLET( 56, 0x10, AttenuateOffset)"
	"HEM_BINARY|DECLARE_APPLET( 41, 0x41, Binary)"
	"HEM_BOOTSNCAT|DECLARE_APPLET( 55, 0x80, BootsNCat)"
	"HEM_BOWTIESEQ|DECLARE_APPLET( 38, 0x80, BowTieSeq)"
	"HEM_BRANCHER|DECLARE_APPLET(  4, 0x14, Brancher)"
	"HEM_BUGCRACK|DECLARE_APPLET( 51, 0x80, BugCrack)"
	"HEM_BURST|DECLARE_APPLET( 31, 0x04, Burst)"
	"HEM_BUTTON|DECLARE_APPLET( 65, 0x10, Button)"
	"HEM_CALCULATE|DECLARE_APPLET( 12, 0x10, Calculate)"
	"HEM_CALIBR8|DECLARE_APPLET( 88, 0x10, Calibr8)"
	"HEM_CARPEGGIO|DECLARE_APPLET( 32, 0x0a, Carpeggio)"
	"HEM_CHORDINATOR|DECLARE_APPLET( 64, 0x08, Chordinator)"
	"HEM_CLOCKDIVIDER|DECLARE_APPLET(  6, 0x04, ClockDivider)"
	"HEM_CLOCKSKIP|DECLARE_APPLET( 28, 0x04, ClockSkip)"
	"HEM_COMPARE|DECLARE_APPLET( 30, 0x10, Compare)"
	"HEM_CVRECV2|DECLARE_APPLET( 24, 0x02, CVRecV2)"
	"HEM_DRUMMAP|DECLARE_APPLET( 57, 0x02, DrumMap)"
	"HEM_DUALQUANT|DECLARE_APPLET(  9, 0x08, DualQuant)"
	"HEM_DUALTM|DECLARE_APPLET( 18, 0x02, DualTM)"
	"HEM_EBBANDLFO|DECLARE_APPLET(  7, 0x01, EbbAndLfo)"
	"HEM_ENIGMAJR|DECLARE_APPLET( 45, 0x02, EnigmaJr)"
	"HEM_ENVFOLLOW|DECLARE_APPLET( 42, 0x11, EnvFollow)"
	"HEM_EUCLIDX|DECLARE_APPLET( 15, 0x02, EuclidX)"
	"HEM_GAMEOFLIFE|DECLARE_APPLET( 22, 0x01, GameOfLife)"
	"HEM_GATEDELAY|DECLARE_APPLET( 29, 0x04, GateDelay)"
	"HEM_GATEDVCA|DECLARE_APPLET( 17, 0x50, GatedVCA)"
	"HEM_LOFIPCM|DECLARE_APPLET( 16, 0x80, LoFiPCM)"
	"HEM_LOGIC|DECLARE_APPLET( 10, 0x44, Logic)"
	"HEM_LOWERRENZ|DECLARE_APPLET( 21, 0x01, LowerRenz)"
	"HEM_METRONOME|DECLARE_APPLET( 50, 0x04, Metronome)"
	"HEM_HMIDIIN|DECLARE_APPLET(150, 0x20, hMIDIIn)"
	"HEM_HMIDIOUT|DECLARE_APPLET( 27, 0x20, hMIDIOut)"
	"HEM_MIXERBAL|DECLARE_APPLET( 33, 0x10, MixerBal)"
	"HEM_PALIMPSEST|DECLARE_APPLET( 20, 0x02, Palimpsest)"
	"HEM_PROBABILITYDIVIDER|DECLARE_APPLET( 59, 0x04, ProbabilityDivider)"
	"HEM_PROBABILITYMELODY|DECLARE_APPLET( 62, 0x04, ProbabilityMelody)"
	"HEM_RNDWALK|DECLARE_APPLET( 69, 0x01, RndWalk)"
	"HEM_RUNGLEBOOK|DECLARE_APPLET( 44, 0x01, RunglBook)"
	"HEM_SAMPLEANDHOLD|DECLARE_APPLET( 1, 0x10, SampleAndHold)"
	"HEM_SCALEDUET|DECLARE_APPLET( 26, 0x08, ScaleDuet)"
	"HEM_SCHMITT|DECLARE_APPLET( 40, 0x40, Schmitt)"
	"HEM_SCOPE|DECLARE_APPLET( 23, 0x80, Scope)"
	"HEM_SEQUENCEX|DECLARE_APPLET( 14, 0x02, SequenceX)"
	"HEM_SHIFTGATE|DECLARE_APPLET( 48, 0x45, ShiftGate)"
	"HEM_SHREDDER|DECLARE_APPLET( 58, 0x01, Shredder)"
	"HEM_SHUFFLE|DECLARE_APPLET( 36, 0x04, Shuffle)"
	"HEM_SLEW|DECLARE_APPLET( 19, 0x01, Slew)"
	"HEM_SQUANCH|DECLARE_APPLET( 46, 0x08, Squanch)"
  "HEM_STAIRS|DECLARE_APPLET( 61, 0x01, Stairs)"
  "HEM_SWITCH|DECLARE_APPLET(  3, 0x10, Switch)"
  "HEM_TB3PO|DECLARE_APPLET( 60, 0x02, TB_3PO)"
  "HEM_TLNEURON|DECLARE_APPLET( 13, 0x40, TLNeuron)"
  "HEM_TRENDING|DECLARE_APPLET( 37, 0x40, Trending)"
  "HEM_TRIGSEQ|DECLARE_APPLET( 11, 0x06, TrigSeq)"
  "HEM_TRIGSEQ16|DECLARE_APPLET( 25, 0x06, TrigSeq16)"
  "HEM_TUNER|DECLARE_APPLET( 39, 0x80, Tuner)"
  "HEM_VECTOREG|DECLARE_APPLET( 52, 0x01, VectorEG)"
	"HEM_VECTORLFO|DECLARE_APPLET( 49, 0x01, VectorLFO)"
	"HEM_VECTORMOD|DECLARE_APPLET( 53, 0x01, VectorMod)"
	"HEM_VECTORMORPH|DECLARE_APPLET( 54, 0x01, VectorMorph)"
	"HEM_VOLTAGE|DECLARE_APPLET( 43, 0x10, Voltage)"
)

echo "// macros for custom hemispheres configurations" > $filename

for i in "${applets[@]}"
do
	IFS="|" read -r -a parts <<< "$i"
	echo "#ifdef ENABLE_${parts[0]}" >> $filename
	echo "    #define DECLARE_${parts[0]} ${parts[1]}," >> $filename
	echo "#else" >> $filename
	echo "    #define DECLARE_${parts[0]}" >> $filename
	echo "#endif" >> $filename
	echo >> $filename
done

echo "#define HEMISPHERE_APPLETS { \\" >> $filename

for i in "${applets[@]}"
do
	IFS="|" read -r -a parts <<< "$i"
	echo "    DECLARE_${parts[0]} \\" >> $filename
done

echo "}" >> $filename