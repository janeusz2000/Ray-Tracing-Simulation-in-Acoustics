#!/bin/bash
echo "Validation Start"

shopt -s nullglob
FILEARRAY=( "validationDiffusors/*" )

for FILEPATH in $FILEARRAY
do
   CURRENT_FILE=$(echo ${FILEPATH%.*} | cut -c21-)
   JS_FILE="./validationResults/$CURRENT_FILE.js"

   rm $JS_FILE
   touch $JS_FILE

   bazel-bin/validation $JS_FILE ./$FILEPATH

done