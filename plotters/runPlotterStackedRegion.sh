#!/bin/bash

regions=( \ 
# "EleMuOSOF"     \
# "OneEleDY"      \
# "OneEleNoPair"  \
# "OneEleOffZ"    \
# "OneEleSig"     \
# "OneEleZH"      \
# "OneMuDY"       \
# "OneMuNoPair"   \
# "OneMuOffZ"     \
# "OneMuSig"      \
# "OneMuZH"       \
# "OnePho"        \
# "TwoEleDY"      \
# "TwoEleOffZ"    \
# "TwoEleSig"     \
# "TwoEleZH"      \
# "TwoMuDY"       \
 "TwoMuOffZ"     \
# "TwoMuSig"      \
# "TwoMuZH"       \
) 

dologs=( \
 "kFALSE" \
 #"kTRUE" \
)

doHIPs=( \
 "kFALSE" \
 #"kTRUE" \
)

#useEOS="kFALSE"
useEOS="kTRUE"
description=""

for region in ${regions[@]}
do
 for dolog in ${dologs[@]}
 do
  for doHIP in ${doHIPs[@]}
  do
   root -l -b -q  'plotter_stackedRegion.C('\""${region}"\"', '"${dolog}"', '"${doHIP}"', '"${useEOS}"', '\""${description}"\"')'
  done
 done
done
